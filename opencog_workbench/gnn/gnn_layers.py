"""GNN Layer implementations."""

import numpy as np
from typing import Dict, List, Optional


class GNNLayer:
    """Base GNN layer with mean-aggregation graph convolution."""

    def __init__(self, input_dim: int, output_dim: int, layer_id: str = ""):
        self.input_dim = input_dim
        self.output_dim = output_dim
        self.layer_id = layer_id

        # Glorot uniform initialization
        limit = np.sqrt(6.0 / (input_dim + output_dim))
        self.weights = np.random.uniform(-limit, limit, (input_dim, output_dim))
        self.bias = np.zeros(output_dim)

    def forward(self, node_features: Dict[str, np.ndarray],
                adjacency_matrix: np.ndarray,
                nodes: List[str]) -> Dict[str, np.ndarray]:
        """Forward pass: aggregate neighbor features then apply linear transform."""
        node_index = {node: i for i, node in enumerate(nodes)}
        output = {}

        for node_id in nodes:
            if node_id not in node_features:
                continue

            idx = node_index[node_id]
            neighbor_indices = np.where(adjacency_matrix[:, idx] > 0)[0]

            if len(neighbor_indices) > 0:
                neighbor_feats = [
                    node_features[nodes[i]]
                    for i in neighbor_indices
                    if nodes[i] in node_features
                ]
                aggregated = np.mean(neighbor_feats, axis=0) if neighbor_feats else node_features[node_id]
            else:
                aggregated = node_features[node_id]

            # Include self in aggregation
            aggregated = 0.5 * aggregated + 0.5 * node_features[node_id]
            transformed = np.dot(aggregated, self.weights) + self.bias
            output[node_id] = np.maximum(0, transformed)  # ReLU

        return output


class GraphConvolutionLayer(GNNLayer):
    """
    Spectral-style graph convolution layer (GCN).

    Applies symmetric degree normalization: H' = D^{-1/2} A D^{-1/2} H W
    where A is the adjacency matrix with added self-loops.
    """

    def forward(self, node_features: Dict[str, np.ndarray],
                adjacency_matrix: np.ndarray,
                nodes: List[str]) -> Dict[str, np.ndarray]:
        """Degree-normalized graph convolution forward pass."""
        n = len(nodes)
        node_index = {node: i for i, node in enumerate(nodes)}

        # Build feature matrix
        feat_dim = next(iter(node_features.values())).shape[0]
        H = np.zeros((n, feat_dim))
        for node_id in nodes:
            if node_id in node_features:
                H[node_index[node_id]] = node_features[node_id]

        # Add self-loops: A_hat = A + I
        A_hat = adjacency_matrix + np.eye(n)

        # Degree matrix D_hat
        degree = A_hat.sum(axis=1)
        D_inv_sqrt = np.diag(np.where(degree > 0, 1.0 / np.sqrt(degree), 0.0))

        # Normalized adjacency: D^{-1/2} A_hat D^{-1/2}
        A_norm = D_inv_sqrt @ A_hat @ D_inv_sqrt

        # Convolution: A_norm H W + b
        H_out = A_norm @ H @ self.weights + self.bias
        H_out = np.maximum(0, H_out)  # ReLU

        return {nodes[i]: H_out[i] for i in range(n)}


class AttentionLayer:
    """
    Graph Attention Network (GAT) layer.

    Computes attention coefficients for each edge and uses them to
    weight neighbor aggregation.

    *output_dim* must be divisible by *num_heads* so each head receives an
    equal-sized projection (head_dim = output_dim // num_heads).
    """

    def __init__(self, input_dim: int, output_dim: int, num_heads: int = 1,
                 layer_id: str = ""):
        if num_heads < 1:
            raise ValueError("num_heads must be >= 1")
        if output_dim % num_heads != 0:
            raise ValueError(
                f"output_dim ({output_dim}) must be divisible by num_heads ({num_heads})"
            )
        self.input_dim = input_dim
        self.output_dim = output_dim
        self.num_heads = num_heads
        self.layer_id = layer_id

        head_dim = output_dim // num_heads
        limit = np.sqrt(6.0 / (input_dim + head_dim))
        self.W = np.random.uniform(-limit, limit, (num_heads, input_dim, head_dim))
        self.a = np.random.uniform(-0.1, 0.1, (num_heads, 2 * head_dim))
        self.bias = np.zeros(output_dim)

    def forward(self, node_features: Dict[str, np.ndarray],
                adjacency_matrix: np.ndarray,
                nodes: List[str]) -> Dict[str, np.ndarray]:
        """Attention-weighted aggregation forward pass."""
        n = len(nodes)
        node_index = {node: i for i, node in enumerate(nodes)}
        head_dim = self.output_dim // max(self.num_heads, 1)

        feat_dim = next(iter(node_features.values())).shape[0]
        H = np.zeros((n, feat_dim))
        for node_id in nodes:
            if node_id in node_features:
                H[node_index[node_id]] = node_features[node_id]

        all_head_outputs = []

        for h in range(self.num_heads):
            # Linear projection for this head
            Wh = H @ self.W[h]  # (n, head_dim)

            head_out = np.zeros((n, head_dim))
            for i in range(n):
                # Neighbours including self
                neighbor_mask = adjacency_matrix[:, i] > 0
                neighbor_mask[i] = True
                neighbor_indices = np.where(neighbor_mask)[0]

                # Attention scores e_{ij} = LeakyReLU(a^T [Wh_i || Wh_j])
                scores = []
                for j in neighbor_indices:
                    concat = np.concatenate([Wh[i], Wh[j]])
                    e = np.dot(self.a[h], concat)
                    e = np.where(e >= 0, e, 0.01 * e)  # LeakyReLU
                    scores.append(e)

                # Softmax
                scores = np.array(scores)
                scores = scores - scores.max()
                exp_scores = np.exp(scores)
                alpha = exp_scores / (exp_scores.sum() + 1e-9)

                # Weighted sum
                head_out[i] = sum(alpha[k] * Wh[j] for k, j in enumerate(neighbor_indices))

            all_head_outputs.append(head_out)

        # Concatenate heads — dimensions match exactly since output_dim % num_heads == 0
        if self.num_heads > 1:
            combined = np.concatenate(all_head_outputs, axis=1)
        else:
            combined = all_head_outputs[0]

        combined = combined + self.bias
        combined = np.maximum(0, combined)  # ReLU

        return {nodes[i]: combined[i] for i in range(n)}
