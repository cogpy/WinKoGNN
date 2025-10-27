"""
Graph Neural Network Integration
Provides neural network processing over knowledge graphs
Integrates with OpenCog AtomSpace for cognitive processing
"""

import logging
from typing import Dict, List, Any, Optional, Tuple
from dataclasses import dataclass
import numpy as np

logger = logging.getLogger(__name__)


@dataclass
class GraphStructure:
    """Represents a graph structure for GNN processing"""
    nodes: List[str]  # Node IDs
    edges: List[Tuple[str, str]]  # (source, target) pairs
    node_features: Dict[str, np.ndarray]  # Node ID -> feature vector
    edge_features: Dict[Tuple[str, str], np.ndarray]  # Edge -> feature vector
    adjacency_matrix: Optional[np.ndarray] = None
    
    def __post_init__(self):
        if self.adjacency_matrix is None:
            self._build_adjacency_matrix()
            
    def _build_adjacency_matrix(self):
        """Build adjacency matrix from edges"""
        n = len(self.nodes)
        node_index = {node: i for i, node in enumerate(self.nodes)}
        
        self.adjacency_matrix = np.zeros((n, n))
        for source, target in self.edges:
            if source in node_index and target in node_index:
                i, j = node_index[source], node_index[target]
                self.adjacency_matrix[i, j] = 1
                

class GraphNeuralNetwork:
    """
    Graph Neural Network for processing knowledge graphs
    Integrates with OpenCog AtomSpace and Windows NT4 kernel
    """
    
    def __init__(self, config: Optional[Dict[str, Any]] = None):
        self.config = config or {}
        
        # Network architecture
        self.input_dim = self.config.get('input_dim', 128)
        self.hidden_dim = self.config.get('hidden_dim', 256)
        self.output_dim = self.config.get('output_dim', 64)
        self.num_layers = self.config.get('num_layers', 3)
        
        # Components
        self.layers = []
        self.embeddings: Dict[str, np.ndarray] = {}
        
        # AtomSpace integration
        self.atomspace_manager = None
        
        # Training state
        self.is_trained = False
        self.training_history: List[Dict[str, float]] = []
        
        self._initialize_network()
        logger.info(f"GraphNeuralNetwork initialized with {self.num_layers} layers")
        
    def _initialize_network(self):
        """Initialize network layers"""
        # Create layer stack
        dims = [self.input_dim] + [self.hidden_dim] * (self.num_layers - 1) + [self.output_dim]
        
        for i in range(self.num_layers):
            layer = GNNLayer(
                input_dim=dims[i],
                output_dim=dims[i + 1],
                layer_id=f"layer_{i}"
            )
            self.layers.append(layer)
            
        logger.debug(f"Initialized {len(self.layers)} GNN layers")
        
    def set_atomspace_manager(self, atomspace_manager):
        """Connect to AtomSpace manager for graph data"""
        self.atomspace_manager = atomspace_manager
        logger.info("Connected to AtomSpace manager")
        
    def extract_graph_from_atomspace(self) -> GraphStructure:
        """Extract graph structure from AtomSpace"""
        if not self.atomspace_manager:
            raise ValueError("AtomSpace manager not connected")
            
        nodes = []
        edges = []
        node_features = {}
        edge_features = {}
        
        # Extract nodes and edges from AtomSpace
        for atom_id, atom in self.atomspace_manager.atomspace.items():
            nodes.append(atom_id)
            
            # Create feature vector from atom properties
            features = self._create_node_features(atom)
            node_features[atom_id] = features
            
            # Extract edges from outgoing connections
            for target_id in atom.outgoing:
                edges.append((atom_id, target_id))
                edge_features[(atom_id, target_id)] = self._create_edge_features(atom, target_id)
                
        graph = GraphStructure(
            nodes=nodes,
            edges=edges,
            node_features=node_features,
            edge_features=edge_features
        )
        
        logger.info(f"Extracted graph with {len(nodes)} nodes and {len(edges)} edges")
        return graph
        
    def _create_node_features(self, atom) -> np.ndarray:
        """Create feature vector for a node"""
        # Combine various atom properties into feature vector
        features = np.zeros(self.input_dim)
        
        # Encode atom type (one-hot or embedding)
        features[0] = hash(atom.atom_type.value) % 100 / 100.0
        
        # Truth value and confidence
        features[1] = atom.truth_value
        features[2] = atom.confidence
        features[3] = atom.attention_value
        
        # Structural features
        features[4] = len(atom.incoming) / 10.0  # Normalized in-degree
        features[5] = len(atom.outgoing) / 10.0  # Normalized out-degree
        
        # Hash of name for semantic information
        name_hash = hash(atom.name) % (self.input_dim - 6)
        features[6 + name_hash] = 1.0
        
        return features
        
    def _create_edge_features(self, source_atom, target_id) -> np.ndarray:
        """Create feature vector for an edge"""
        features = np.zeros(16)
        
        # Simple edge features
        features[0] = source_atom.truth_value
        
        return features
        
    def forward_pass(self, graph: GraphStructure) -> Dict[str, np.ndarray]:
        """
        Perform forward pass through the network
        Returns embeddings for each node
        """
        # Start with input features
        current_embeddings = graph.node_features.copy()
        
        # Pass through each layer
        for layer in self.layers:
            current_embeddings = layer.forward(
                current_embeddings,
                graph.adjacency_matrix,
                graph.nodes
            )
            
        # Store final embeddings
        self.embeddings = current_embeddings
        
        logger.debug(f"Forward pass completed, generated embeddings for {len(current_embeddings)} nodes")
        return current_embeddings
        
    def train(self, graph: GraphStructure, labels: Optional[Dict[str, np.ndarray]] = None,
             epochs: int = 10) -> Dict[str, Any]:
        """
        Train the GNN on the graph
        Uses self-supervised learning if no labels provided
        """
        logger.info(f"Starting GNN training for {epochs} epochs")
        
        for epoch in range(epochs):
            # Forward pass
            embeddings = self.forward_pass(graph)
            
            # Compute loss (simplified)
            if labels:
                loss = self._supervised_loss(embeddings, labels)
            else:
                loss = self._self_supervised_loss(embeddings, graph)
                
            # Backward pass (simplified - in practice would use autodiff)
            self._backward_pass(loss)
            
            # Track training
            metrics = {
                'epoch': epoch,
                'loss': loss,
                'num_nodes': len(graph.nodes)
            }
            self.training_history.append(metrics)
            
            if epoch % 5 == 0:
                logger.info(f"Epoch {epoch}: loss = {loss:.4f}")
                
        self.is_trained = True
        logger.info("GNN training completed")
        
        return {
            'final_loss': loss,
            'epochs': epochs,
            'history': self.training_history
        }
        
    def _supervised_loss(self, embeddings: Dict[str, np.ndarray], 
                        labels: Dict[str, np.ndarray]) -> float:
        """Compute supervised loss"""
        total_loss = 0.0
        count = 0
        
        for node_id, embedding in embeddings.items():
            if node_id in labels:
                # MSE loss
                diff = embedding - labels[node_id]
                total_loss += np.sum(diff ** 2)
                count += 1
                
        return total_loss / max(count, 1)
        
    def _self_supervised_loss(self, embeddings: Dict[str, np.ndarray],
                             graph: GraphStructure) -> float:
        """Compute self-supervised loss (e.g., link prediction)"""
        total_loss = 0.0
        count = 0
        
        # Predict edges based on node similarity
        for source, target in graph.edges:
            if source in embeddings and target in embeddings:
                # Positive samples (existing edges)
                similarity = np.dot(embeddings[source], embeddings[target])
                loss = -np.log(1 / (1 + np.exp(-similarity)) + 1e-10)
                total_loss += loss
                count += 1
                
        return total_loss / max(count, 1)
        
    def _backward_pass(self, loss: float):
        """Simplified backward pass"""
        # In full implementation, would compute gradients and update weights
        pass
        
    def get_node_embedding(self, node_id: str) -> Optional[np.ndarray]:
        """Get embedding for a specific node"""
        return self.embeddings.get(node_id)
        
    def compute_similarity(self, node_id1: str, node_id2: str) -> float:
        """Compute similarity between two nodes based on embeddings"""
        emb1 = self.get_node_embedding(node_id1)
        emb2 = self.get_node_embedding(node_id2)
        
        if emb1 is None or emb2 is None:
            return 0.0
            
        # Cosine similarity
        norm1 = np.linalg.norm(emb1)
        norm2 = np.linalg.norm(emb2)
        
        if norm1 == 0 or norm2 == 0:
            return 0.0
            
        return np.dot(emb1, emb2) / (norm1 * norm2)
        
    def predict_links(self, threshold: float = 0.5) -> List[Tuple[str, str, float]]:
        """Predict potential links based on node embeddings"""
        predictions = []
        nodes = list(self.embeddings.keys())
        
        for i, node1 in enumerate(nodes):
            for node2 in nodes[i+1:]:
                similarity = self.compute_similarity(node1, node2)
                if similarity > threshold:
                    predictions.append((node1, node2, similarity))
                    
        predictions.sort(key=lambda x: x[2], reverse=True)
        logger.info(f"Predicted {len(predictions)} potential links")
        return predictions
        
    def get_statistics(self) -> Dict[str, Any]:
        """Get GNN statistics"""
        return {
            'num_layers': self.num_layers,
            'input_dim': self.input_dim,
            'hidden_dim': self.hidden_dim,
            'output_dim': self.output_dim,
            'is_trained': self.is_trained,
            'num_embeddings': len(self.embeddings),
            'training_epochs': len(self.training_history)
        }


class GNNLayer:
    """A single Graph Neural Network layer"""
    
    def __init__(self, input_dim: int, output_dim: int, layer_id: str):
        self.input_dim = input_dim
        self.output_dim = output_dim
        self.layer_id = layer_id
        
        # Initialize weights (simplified)
        self.weights = np.random.randn(input_dim, output_dim) * 0.01
        self.bias = np.zeros(output_dim)
        
    def forward(self, node_features: Dict[str, np.ndarray],
               adjacency_matrix: np.ndarray,
               nodes: List[str]) -> Dict[str, np.ndarray]:
        """Forward pass through the layer"""
        output_features = {}
        
        node_index = {node: i for i, node in enumerate(nodes)}
        
        for node_id in nodes:
            if node_id not in node_features:
                continue
                
            # Get node index
            idx = node_index[node_id]
            
            # Aggregate neighbor features
            neighbors = np.where(adjacency_matrix[:, idx] > 0)[0]
            
            if len(neighbors) > 0:
                neighbor_ids = [nodes[i] for i in neighbors if nodes[i] in node_features]
                neighbor_features = [node_features[nid] for nid in neighbor_ids]
                
                if neighbor_features:
                    # Mean aggregation
                    aggregated = np.mean(neighbor_features, axis=0)
                else:
                    aggregated = node_features[node_id]
            else:
                aggregated = node_features[node_id]
                
            # Transform
            output = np.dot(aggregated, self.weights) + self.bias
            
            # ReLU activation
            output = np.maximum(0, output)
            
            output_features[node_id] = output
            
        return output_features
