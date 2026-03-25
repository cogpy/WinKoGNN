"""Node embedding utilities."""

import numpy as np
from typing import Dict, List, Optional, Tuple


class NodeEmbedding:
    """
    Manages node embeddings produced by the GNN.

    Provides similarity search, nearest-neighbour lookup, and
    simple embedding arithmetic (analogy queries).
    """

    def __init__(self, dim: int = 64):
        self.dim = dim
        self.embeddings: Dict[str, np.ndarray] = {}

    def update(self, embeddings: Dict[str, np.ndarray]):
        """Replace stored embeddings with a new set."""
        self.embeddings = {k: v.copy() for k, v in embeddings.items()}

    def get(self, node_id: str) -> Optional[np.ndarray]:
        """Return the embedding vector for *node_id*, or None."""
        return self.embeddings.get(node_id)

    def similarity(self, node_a: str, node_b: str) -> float:
        """Cosine similarity between two node embeddings (0 if unknown)."""
        va = self.embeddings.get(node_a)
        vb = self.embeddings.get(node_b)
        if va is None or vb is None:
            return 0.0
        norm_a = np.linalg.norm(va)
        norm_b = np.linalg.norm(vb)
        if norm_a < 1e-9 or norm_b < 1e-9:
            return 0.0
        return float(np.dot(va, vb) / (norm_a * norm_b))

    def nearest_neighbours(self, node_id: str, k: int = 5,
                           exclude_self: bool = True) -> List[Tuple[str, float]]:
        """Return the *k* most similar nodes to *node_id* by cosine similarity."""
        query = self.embeddings.get(node_id)
        if query is None or len(self.embeddings) == 0:
            return []

        scores = []
        q_norm = np.linalg.norm(query)
        for nid, vec in self.embeddings.items():
            if exclude_self and nid == node_id:
                continue
            v_norm = np.linalg.norm(vec)
            if q_norm < 1e-9 or v_norm < 1e-9:
                sim = 0.0
            else:
                sim = float(np.dot(query, vec) / (q_norm * v_norm))
            scores.append((nid, sim))

        scores.sort(key=lambda x: x[1], reverse=True)
        return scores[:k]

    def analogy(self, a: str, b: str, c: str, k: int = 1) -> List[Tuple[str, float]]:
        """
        Solve the analogy *a* is to *b* as *c* is to ?.

        Returns the *k* best candidates (excluding a, b, c).
        """
        va = self.embeddings.get(a)
        vb = self.embeddings.get(b)
        vc = self.embeddings.get(c)
        if va is None or vb is None or vc is None:
            return []

        target = vb - va + vc
        t_norm = np.linalg.norm(target)
        exclude = {a, b, c}

        scores = []
        for nid, vec in self.embeddings.items():
            if nid in exclude:
                continue
            v_norm = np.linalg.norm(vec)
            if t_norm < 1e-9 or v_norm < 1e-9:
                sim = 0.0
            else:
                sim = float(np.dot(target, vec) / (t_norm * v_norm))
            scores.append((nid, sim))

        scores.sort(key=lambda x: x[1], reverse=True)
        return scores[:k]

    def __len__(self) -> int:
        return len(self.embeddings)
