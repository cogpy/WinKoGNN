"""
OpenCog AtomSpace Manager
Manages knowledge representation in hypergraph format
"""

import logging
from typing import Dict, List, Any, Set, Optional
from dataclasses import dataclass
from enum import Enum

logger = logging.getLogger(__name__)


class AtomType(Enum):
    """Types of atoms in the AtomSpace"""
    NODE = "node"
    LINK = "link"
    CONCEPT = "concept"
    PREDICATE = "predicate"
    EVALUATION = "evaluation"


@dataclass
class Atom:
    """Represents an atom in the AtomSpace hypergraph"""
    atom_id: str
    atom_type: AtomType
    name: str
    truth_value: float = 1.0  # Probabilistic truth value
    confidence: float = 1.0   # Confidence in the truth value
    attention_value: float = 0.0  # Importance for reasoning
    incoming: Set[str] = None  # IDs of atoms pointing to this one
    outgoing: Set[str] = None  # IDs of atoms this one points to
    
    def __post_init__(self):
        if self.incoming is None:
            self.incoming = set()
        if self.outgoing is None:
            self.outgoing = set()


class AtomSpaceManager:
    """
    Manages OpenCog AtomSpace for knowledge representation
    Integrates with Windows NT4 cognitive kernel for persistent storage
    """
    
    def __init__(self, config: Optional[Dict[str, Any]] = None):
        self.config = config or {}
        self.atomspace: Dict[str, Atom] = {}
        self.type_index: Dict[AtomType, Set[str]] = {t: set() for t in AtomType}
        self.name_index: Dict[str, Set[str]] = {}
        
        logger.info("AtomSpaceManager initialized")
        
    def add_atom(self, atom_type: AtomType, name: str, **kwargs) -> str:
        """Add a new atom to the AtomSpace"""
        atom_id = f"{atom_type.value}_{name}_{id(name)}"
        
        if atom_id in self.atomspace:
            logger.debug(f"Atom {atom_id} already exists, returning existing")
            return atom_id
            
        atom = Atom(
            atom_id=atom_id,
            atom_type=atom_type,
            name=name,
            **kwargs
        )
        
        self.atomspace[atom_id] = atom
        self.type_index[atom_type].add(atom_id)
        
        if name not in self.name_index:
            self.name_index[name] = set()
        self.name_index[name].add(atom_id)
        
        logger.debug(f"Added atom {atom_id} of type {atom_type.value}")
        return atom_id
        
    def create_link(self, link_type: AtomType, source_id: str, target_id: str, 
                   name: str = None, **kwargs) -> str:
        """Create a link between two atoms"""
        if source_id not in self.atomspace or target_id not in self.atomspace:
            logger.error(f"Cannot create link: source or target atom not found")
            return None
            
        link_name = name or f"link_{source_id}_to_{target_id}"
        link_id = self.add_atom(link_type, link_name, **kwargs)
        
        link = self.atomspace[link_id]
        link.outgoing.add(target_id)
        
        source = self.atomspace[source_id]
        source.outgoing.add(link_id)
        
        target = self.atomspace[target_id]
        target.incoming.add(link_id)
        
        logger.debug(f"Created link {link_id} from {source_id} to {target_id}")
        return link_id
        
    def get_atom(self, atom_id: str) -> Optional[Atom]:
        """Retrieve an atom by ID"""
        return self.atomspace.get(atom_id)
        
    def get_atoms_by_type(self, atom_type: AtomType) -> List[Atom]:
        """Get all atoms of a specific type"""
        atom_ids = self.type_index.get(atom_type, set())
        return [self.atomspace[aid] for aid in atom_ids]
        
    def get_atoms_by_name(self, name: str) -> List[Atom]:
        """Get all atoms with a specific name"""
        atom_ids = self.name_index.get(name, set())
        return [self.atomspace[aid] for aid in atom_ids]
        
    def query_pattern(self, pattern: Dict[str, Any]) -> List[Atom]:
        """Query atoms matching a pattern"""
        # Simple pattern matching implementation
        results = []
        
        atom_type = pattern.get('type')
        name_pattern = pattern.get('name')
        min_truth = pattern.get('min_truth', 0.0)
        
        candidates = self.atomspace.values()
        
        if atom_type:
            candidates = [a for a in candidates if a.atom_type == atom_type]
            
        if name_pattern:
            candidates = [a for a in candidates if name_pattern in a.name]
            
        results = [a for a in candidates if a.truth_value >= min_truth]
        
        logger.debug(f"Pattern query returned {len(results)} results")
        return results
        
    def update_attention(self, atom_id: str, attention_value: float):
        """Update attention value of an atom for importance weighting"""
        if atom_id in self.atomspace:
            self.atomspace[atom_id].attention_value = attention_value
            logger.debug(f"Updated attention for {atom_id} to {attention_value}")
            
    def spread_activation(self, source_id: str, decay: float = 0.9):
        """Spread activation from a source atom through the hypergraph"""
        if source_id not in self.atomspace:
            return
            
        visited = set()
        to_visit = [(source_id, 1.0)]
        
        while to_visit:
            current_id, activation = to_visit.pop(0)
            
            if current_id in visited:
                continue
                
            visited.add(current_id)
            current = self.atomspace[current_id]
            
            # Update attention value
            current.attention_value = max(current.attention_value, activation)
            
            # Spread to outgoing atoms
            for next_id in current.outgoing:
                if next_id not in visited:
                    to_visit.append((next_id, activation * decay))
                    
        logger.info(f"Activation spread from {source_id} to {len(visited)} atoms")
        
    def get_statistics(self) -> Dict[str, Any]:
        """Get AtomSpace statistics"""
        return {
            'total_atoms': len(self.atomspace),
            'atoms_by_type': {t.value: len(ids) for t, ids in self.type_index.items()},
            'unique_names': len(self.name_index)
        }
        
    def clear(self):
        """Clear all atoms from the AtomSpace"""
        self.atomspace.clear()
        self.type_index = {t: set() for t in AtomType}
        self.name_index.clear()
        logger.info("AtomSpace cleared")
