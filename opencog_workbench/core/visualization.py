"""
AtomSpace Visualization
Exports the AtomSpace hypergraph to DOT (Graphviz) and JSON formats for
external visualization tools.
"""

import json
import logging
from typing import TYPE_CHECKING, Dict, Any, Optional

if TYPE_CHECKING:
    from .atomspace import AtomSpaceManager

logger = logging.getLogger(__name__)


class AtomSpaceVisualizer:
    """
    Generates visual representations of an AtomSpace hypergraph.

    Supports:
    * Graphviz DOT format  — render with ``dot -Tpng -o graph.png graph.dot``
    * JSON node-link format — compatible with D3.js / NetworkX
    """

    # Graphviz colour map keyed by AtomType.value
    _TYPE_COLOURS: Dict[str, str] = {
        "node": "#AED6F1",
        "link": "#A9DFBF",
        "concept": "#F9E79F",
        "predicate": "#F1948A",
        "evaluation": "#D7BDE2",
    }

    def __init__(self, atomspace: "AtomSpaceManager"):
        self.atomspace = atomspace

    # ------------------------------------------------------------------
    # DOT export
    # ------------------------------------------------------------------

    def to_dot(self, title: str = "AtomSpace", show_truth_values: bool = True,
               highlight_top_n: int = 5) -> str:
        """
        Return a Graphviz DOT representation of the AtomSpace.

        Parameters
        ----------
        title:
            Graph title rendered as a label.
        show_truth_values:
            When True each node label includes its truth value.
        highlight_top_n:
            Nodes with the highest attention value are rendered with a bold
            border so important atoms are easy to spot.
        """
        atoms = list(self.atomspace.atomspace.values())
        if not atoms:
            return 'digraph AtomSpace { label="(empty)"; }'

        # Determine top-attention atoms
        top_ids = set()
        if highlight_top_n > 0:
            sorted_by_attn = sorted(
                atoms, key=lambda a: a.attention_value, reverse=True
            )
            top_ids = {a.atom_id for a in sorted_by_attn[:highlight_top_n]}

        lines = [
            f'digraph {json.dumps(title)} {{',
            f'  label={json.dumps(title)};',
            '  rankdir=LR;',
            '  node [shape=box, style="rounded,filled", fontname="Helvetica"];',
            '  edge [fontname="Helvetica", fontsize=9];',
            '',
        ]

        # Nodes
        for atom in atoms:
            colour = self._TYPE_COLOURS.get(atom.atom_type.value, "#EAECEE")
            if show_truth_values:
                label = f"{atom.name}\\n[tv={atom.truth_value:.2f} conf={atom.confidence:.2f}]"
            else:
                label = atom.name
            penwidth = "3" if atom.atom_id in top_ids else "1"
            node_id = self._dot_id(atom.atom_id)
            lines.append(
                f'  {node_id} [label={json.dumps(label)}, '
                f'fillcolor={json.dumps(colour)}, penwidth={penwidth}];'
            )

        lines.append('')

        # Edges (outgoing relationships)
        for atom in atoms:
            src_id = self._dot_id(atom.atom_id)
            for tgt_atom_id in atom.outgoing:
                if tgt_atom_id in self.atomspace.atomspace:
                    tgt_id = self._dot_id(tgt_atom_id)
                    lines.append(f'  {src_id} -> {tgt_id};')

        lines.append('}')
        return '\n'.join(lines)

    def save_dot(self, path: str, **kwargs) -> bool:
        """Write DOT output to *path*. Returns True on success."""
        try:
            dot = self.to_dot(**kwargs)
            with open(path, 'w', encoding='utf-8') as fh:
                fh.write(dot)
            logger.info("AtomSpace DOT graph written to %s", path)
            return True
        except Exception as exc:
            logger.error("Failed to write DOT graph to %s: %s", path, exc)
            return False

    # ------------------------------------------------------------------
    # JSON node-link export
    # ------------------------------------------------------------------

    def to_json(self) -> Dict[str, Any]:
        """
        Return a node-link JSON representation.

        Schema::

            {
              "nodes": [{"id": ..., "name": ..., "type": ...,
                         "truth_value": ..., "confidence": ...,
                         "attention_value": ...}, ...],
              "links": [{"source": ..., "target": ...}, ...]
            }

        Compatible with `networkx.node_link_graph` and D3.js force layouts.
        """
        nodes = []
        links = []
        seen_links: set = set()

        for atom in self.atomspace.atomspace.values():
            nodes.append({
                "id": atom.atom_id,
                "name": atom.name,
                "type": atom.atom_type.value,
                "truth_value": atom.truth_value,
                "confidence": atom.confidence,
                "attention_value": atom.attention_value,
            })
            for tgt_id in atom.outgoing:
                key = (atom.atom_id, tgt_id)
                if key not in seen_links and tgt_id in self.atomspace.atomspace:
                    links.append({"source": atom.atom_id, "target": tgt_id})
                    seen_links.add(key)

        return {"nodes": nodes, "links": links}

    def save_json(self, path: str) -> bool:
        """Write JSON node-link data to *path*. Returns True on success."""
        try:
            data = self.to_json()
            with open(path, 'w', encoding='utf-8') as fh:
                json.dump(data, fh, indent=2)
            logger.info(
                "AtomSpace JSON graph written to %s (%d nodes, %d links)",
                path, len(data['nodes']), len(data['links']),
            )
            return True
        except Exception as exc:
            logger.error("Failed to write JSON graph to %s: %s", path, exc)
            return False

    # ------------------------------------------------------------------
    # Helpers
    # ------------------------------------------------------------------

    @staticmethod
    def _dot_id(raw_id: str) -> str:
        """Convert an arbitrary atom ID to a safe Graphviz node identifier."""
        safe = ''.join(c if c.isalnum() or c == '_' else '_' for c in raw_id)
        # DOT ids must not start with a digit
        if safe and safe[0].isdigit():
            safe = 'n_' + safe
        return safe
