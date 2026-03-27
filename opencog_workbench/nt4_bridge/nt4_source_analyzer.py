"""
NT4 Source Analyzer
Scans the Windows NT4 kernel source tree (private/ntos) and catalogs
exported function signatures to provide the kernel bridge with an accurate
map of available NT4 APIs.
"""

import logging
import os
import re
from typing import Dict, List, Optional

logger = logging.getLogger(__name__)

# Matches standard NT4 C function definitions, e.g.:
#   NTSTATUS
#   KeInitializeProcess (
#       IN PKPROCESS Process,
#       ...
#   )
_FUNC_HEADER_RE = re.compile(
    r'^(?P<ret>[A-Z_][A-Z0-9_]*)\s*\n'      # return type on its own line
    r'(?P<name>(?:Ke|Mm|Ps|Ex|Ob|Io|Lpc|Se|Rtl|Zw|Nt)[A-Za-z0-9_]+)'
    r'\s*\(',                                # opening parenthesis
    re.MULTILINE,
)

# Simpler single-line variant: NTSTATUS KeXxx(...)
_FUNC_INLINE_RE = re.compile(
    r'(?:^|(?<=\n))(?:[A-Z_][A-Z0-9_]+\s+)+'
    r'(?P<name>(?:Ke|Mm|Ps|Ex|Ob|Io|Lpc|Se|Rtl|Zw|Nt)[A-Za-z0-9_]+)\s*\(',
    re.MULTILINE,
)


class NT4FunctionInfo:
    """Holds metadata about a discovered NT4 kernel function."""

    __slots__ = ("name", "source_file", "return_type", "line_number")

    def __init__(self, name: str, source_file: str,
                 return_type: str = "UNKNOWN", line_number: int = 0):
        self.name = name
        self.source_file = source_file
        self.return_type = return_type
        self.line_number = line_number

    def to_dict(self) -> Dict:
        return {
            "name": self.name,
            "source_file": self.source_file,
            "return_type": self.return_type,
            "line_number": self.line_number,
        }

    def __repr__(self) -> str:
        return (
            f"NT4FunctionInfo(name={self.name!r}, "
            f"return_type={self.return_type!r}, "
            f"source_file={os.path.basename(self.source_file)!r})"
        )


class NT4SourceAnalyzer:
    """
    Scans a Windows NT4 kernel source tree and extracts function signatures.

    Usage::

        analyzer = NT4SourceAnalyzer("/path/to/private/ntos")
        analyzer.scan()
        funcs = analyzer.get_functions_by_subsystem("Ke")  # kernel executive
        print(analyzer.summarize())
    """

    # NT4 subsystem prefixes and their descriptions
    SUBSYSTEMS: Dict[str, str] = {
        "Ke": "Kernel Executive",
        "Mm": "Memory Manager",
        "Ps": "Process/Thread Manager",
        "Ex": "Executive Support",
        "Ob": "Object Manager",
        "Io": "I/O Manager",
        "Lpc": "Local Procedure Call",
        "Se": "Security",
        "Rtl": "Runtime Library",
        "Zw": "System Call Wrappers (Zw)",
        "Nt": "System Call Wrappers (Nt)",
    }

    def __init__(self, source_root: str):
        self.source_root = source_root
        self._functions: Dict[str, NT4FunctionInfo] = {}  # name -> info
        self._scanned = False

    # ------------------------------------------------------------------
    # Public API
    # ------------------------------------------------------------------

    def scan(self, extensions: Optional[List[str]] = None) -> int:
        """
        Recursively scan *source_root* for C source files and extract
        NT4 kernel function signatures.

        Parameters
        ----------
        extensions:
            File extensions to process.  Defaults to ``['.c', '.h']``.

        Returns
        -------
        int
            Number of unique functions discovered.
        """
        if extensions is None:
            extensions = ['.c', '.h']

        if not os.path.isdir(self.source_root):
            logger.warning("NT4 source root not found: %s", self.source_root)
            return 0

        self._functions.clear()
        file_count = 0

        for dirpath, _dirs, filenames in os.walk(self.source_root):
            for filename in filenames:
                if not any(filename.lower().endswith(ext) for ext in extensions):
                    continue
                fpath = os.path.join(dirpath, filename)
                try:
                    self._scan_file(fpath)
                    file_count += 1
                except Exception as exc:
                    logger.debug("Skipping %s: %s", fpath, exc)

        self._scanned = True
        logger.info(
            "NT4 source scan complete: %d files scanned, %d functions discovered",
            file_count, len(self._functions),
        )
        return len(self._functions)

    def get_all_functions(self) -> List[NT4FunctionInfo]:
        """Return all discovered function infos, sorted by name."""
        return sorted(self._functions.values(), key=lambda f: f.name)

    def get_function(self, name: str) -> Optional[NT4FunctionInfo]:
        """Look up a function by exact name."""
        return self._functions.get(name)

    def get_functions_by_subsystem(self, prefix: str) -> List[NT4FunctionInfo]:
        """
        Return all functions whose names start with *prefix*
        (e.g. ``"Ke"`` for Kernel Executive functions).
        """
        return sorted(
            (f for f in self._functions.values() if f.name.startswith(prefix)),
            key=lambda f: f.name,
        )

    def get_subsystem_summary(self) -> Dict[str, int]:
        """Return a mapping of subsystem prefix → function count."""
        summary: Dict[str, int] = {}
        for prefix in self.SUBSYSTEMS:
            count = sum(1 for f in self._functions.values() if f.name.startswith(prefix))
            if count:
                summary[prefix] = count
        return summary

    def summarize(self) -> str:
        """Return a human-readable summary of the scan results."""
        if not self._scanned:
            return "Source tree has not been scanned yet. Call scan() first."

        lines = [
            f"NT4 Source Analysis — root: {self.source_root}",
            f"Total functions discovered: {len(self._functions)}",
            "",
            "Breakdown by subsystem:",
        ]
        for prefix, description in self.SUBSYSTEMS.items():
            count = sum(1 for f in self._functions.values() if f.name.startswith(prefix))
            if count:
                lines.append(f"  {prefix:6s} ({description}): {count}")

        return '\n'.join(lines)

    def to_dict(self) -> Dict:
        """Serialisable representation of all discovered functions."""
        return {
            "source_root": self.source_root,
            "total_functions": len(self._functions),
            "subsystems": self.get_subsystem_summary(),
            "functions": [f.to_dict() for f in self.get_all_functions()],
        }

    # ------------------------------------------------------------------
    # Internal helpers
    # ------------------------------------------------------------------

    def _scan_file(self, fpath: str):
        """Extract function signatures from a single source file."""
        with open(fpath, 'r', encoding='utf-8', errors='replace') as fh:
            content = fh.read()

        # Pass 1 — multi-line "return type on own line" pattern
        for m in _FUNC_HEADER_RE.finditer(content):
            name = m.group("name")
            ret = m.group("ret")
            line_no = content[:m.start()].count('\n') + 1
            self._register(name, fpath, ret, line_no)

        # Pass 2 — single-line pattern (catches inline / header declarations)
        for m in _FUNC_INLINE_RE.finditer(content):
            name = m.group("name")
            line_no = content[:m.start()].count('\n') + 1
            if name not in self._functions:
                self._register(name, fpath, "UNKNOWN", line_no)

    def _register(self, name: str, fpath: str, return_type: str, line_no: int):
        """Add or update a function entry (first occurrence wins for location)."""
        if name not in self._functions:
            self._functions[name] = NT4FunctionInfo(
                name=name,
                source_file=fpath,
                return_type=return_type,
                line_number=line_no,
            )
