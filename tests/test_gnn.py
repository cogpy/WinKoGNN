"""
Unit tests for opencog_workbench.gnn (GraphNeuralNetwork and GNNLayer).
"""

import pytest
import numpy as np

from opencog_workbench.core.atomspace import AtomSpaceManager, AtomType
from opencog_workbench.gnn.graph_network import GraphNeuralNetwork, GraphStructure, GNNLayer


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def _populated_atomspace(config=None):
    """Return an AtomSpaceManager with a small connected graph."""
    asm = AtomSpaceManager(config)
    a = asm.add_atom(AtomType.CONCEPT, "ai")
    b = asm.add_atom(AtomType.CONCEPT, "ml")
    c = asm.add_atom(AtomType.CONCEPT, "deep_learning")
    asm.create_link(AtomType.LINK, a, b)
    asm.create_link(AtomType.LINK, b, c)
    return asm


# ---------------------------------------------------------------------------
# Initialisation
# ---------------------------------------------------------------------------

class TestGNNInit:
    def test_gnn_initializes(self, gnn):
        assert gnn is not None

    def test_layer_count(self, gnn):
        from tests.conftest import TEST_CONFIG
        assert len(gnn.layers) == TEST_CONFIG["gnn"]["num_layers"]

    def test_not_trained_initially(self, gnn):
        assert gnn.is_trained is False

    def test_statistics_keys(self, gnn):
        stats = gnn.get_statistics()
        for key in ("num_layers", "input_dim", "hidden_dim", "output_dim",
                    "is_trained", "num_embeddings", "training_epochs"):
            assert key in stats


# ---------------------------------------------------------------------------
# Graph extraction
# ---------------------------------------------------------------------------

class TestGraphExtraction:
    def test_extract_graph_structure(self, gnn, atomspace):
        atomspace.add_atom(AtomType.CONCEPT, "alpha")
        atomspace.add_atom(AtomType.CONCEPT, "beta")
        graph = gnn.extract_graph_from_atomspace()
        assert isinstance(graph, GraphStructure)
        assert len(graph.nodes) >= 2

    def test_extract_without_atomspace_raises(self):
        from tests.conftest import TEST_CONFIG
        net = GraphNeuralNetwork(TEST_CONFIG["gnn"])
        with pytest.raises(ValueError):
            net.extract_graph_from_atomspace()

    def test_adjacency_matrix_shape(self, gnn, atomspace):
        atomspace.add_atom(AtomType.CONCEPT, "n1")
        atomspace.add_atom(AtomType.CONCEPT, "n2")
        graph = gnn.extract_graph_from_atomspace()
        n = len(graph.nodes)
        assert graph.adjacency_matrix.shape == (n, n)


# ---------------------------------------------------------------------------
# Forward pass
# ---------------------------------------------------------------------------

class TestForwardPass:
    def test_forward_pass_returns_embeddings(self, gnn, atomspace):
        atomspace.add_atom(AtomType.CONCEPT, "fp_node_a")
        atomspace.add_atom(AtomType.CONCEPT, "fp_node_b")
        graph = gnn.extract_graph_from_atomspace()
        embeddings = gnn.forward_pass(graph)
        assert isinstance(embeddings, dict)
        assert len(embeddings) == len(graph.nodes)

    def test_embedding_dimension(self, gnn, atomspace):
        from tests.conftest import TEST_CONFIG
        atomspace.add_atom(AtomType.CONCEPT, "dim_test_node")
        graph = gnn.extract_graph_from_atomspace()
        embeddings = gnn.forward_pass(graph)
        for emb in embeddings.values():
            assert emb.shape == (TEST_CONFIG["gnn"]["output_dim"],)


# ---------------------------------------------------------------------------
# Training
# ---------------------------------------------------------------------------

class TestTraining:
    def test_train_returns_results(self, gnn, atomspace):
        atomspace.add_atom(AtomType.CONCEPT, "train_node_x")
        atomspace.add_atom(AtomType.CONCEPT, "train_node_y")
        graph = gnn.extract_graph_from_atomspace()
        results = gnn.train(graph, epochs=3)
        assert "final_loss" in results
        assert "epochs" in results
        assert results["epochs"] == 3

    def test_is_trained_after_training(self, gnn, atomspace):
        atomspace.add_atom(AtomType.CONCEPT, "trained_node")
        graph = gnn.extract_graph_from_atomspace()
        gnn.train(graph, epochs=1)
        assert gnn.is_trained is True

    def test_training_history_length(self, gnn, atomspace):
        atomspace.add_atom(AtomType.CONCEPT, "hist_node")
        graph = gnn.extract_graph_from_atomspace()
        epochs = 5
        gnn.train(graph, epochs=epochs)
        assert len(gnn.training_history) == epochs


# ---------------------------------------------------------------------------
# Similarity & link prediction
# ---------------------------------------------------------------------------

class TestSimilarityAndLinkPrediction:
    def _train_gnn(self, gnn, atomspace):
        atomspace.add_atom(AtomType.CONCEPT, "sim_node_a")
        atomspace.add_atom(AtomType.CONCEPT, "sim_node_b")
        atomspace.add_atom(AtomType.CONCEPT, "sim_node_c")
        graph = gnn.extract_graph_from_atomspace()
        gnn.train(graph, epochs=1)

    def test_compute_similarity_range(self, gnn, atomspace):
        self._train_gnn(gnn, atomspace)
        nodes = list(gnn.embeddings.keys())
        if len(nodes) >= 2:
            sim = gnn.compute_similarity(nodes[0], nodes[1])
            assert -1.0 <= sim <= 1.0

    def test_similarity_nonexistent_node(self, gnn, atomspace):
        self._train_gnn(gnn, atomspace)
        assert gnn.compute_similarity("bad_id_1", "bad_id_2") == 0.0

    def test_get_node_embedding_trained(self, gnn, atomspace):
        self._train_gnn(gnn, atomspace)
        node_id = list(gnn.embeddings.keys())[0]
        emb = gnn.get_node_embedding(node_id)
        assert emb is not None
        assert isinstance(emb, np.ndarray)

    def test_get_node_embedding_untrained(self, gnn):
        assert gnn.get_node_embedding("fake_id") is None

    def test_predict_links_returns_list(self, gnn, atomspace):
        self._train_gnn(gnn, atomspace)
        predictions = gnn.predict_links(threshold=0.0)
        assert isinstance(predictions, list)


# ---------------------------------------------------------------------------
# GNNLayer
# ---------------------------------------------------------------------------

class TestGNNLayer:
    def test_layer_forward(self):
        from tests.conftest import TEST_CONFIG
        cfg = TEST_CONFIG["gnn"]
        layer = GNNLayer(cfg["input_dim"], cfg["hidden_dim"], "test_layer")

        nodes = ["n0", "n1", "n2"]
        features = {n: np.random.randn(cfg["input_dim"]) for n in nodes}
        adj = np.eye(3)

        output = layer.forward(features, adj, nodes)
        assert set(output.keys()) == set(nodes)
        for emb in output.values():
            assert emb.shape == (cfg["hidden_dim"],)
            assert np.all(emb >= 0)  # ReLU non-negativity
