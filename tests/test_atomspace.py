"""
Unit tests for opencog_workbench.core.atomspace.
"""

import pytest
from opencog_workbench.core.atomspace import AtomSpaceManager, AtomType, Atom


# ---------------------------------------------------------------------------
# Basic CRUD
# ---------------------------------------------------------------------------

class TestAtomCreation:
    def test_add_concept_returns_id(self, atomspace):
        atom_id = atomspace.add_atom(AtomType.CONCEPT, "intelligence")
        assert isinstance(atom_id, str)
        assert len(atom_id) > 0

    def test_add_atom_is_retrievable(self, atomspace):
        atom_id = atomspace.add_atom(AtomType.NODE, "node_alpha")
        atom = atomspace.get_atom(atom_id)
        assert atom is not None
        assert atom.name == "node_alpha"
        assert atom.atom_type == AtomType.NODE

    def test_add_duplicate_returns_same_id(self, atomspace):
        id1 = atomspace.add_atom(AtomType.CONCEPT, "same_name")
        id2 = atomspace.add_atom(AtomType.CONCEPT, "same_name")
        assert id1 == id2

    def test_default_truth_value(self, atomspace):
        atom_id = atomspace.add_atom(AtomType.PREDICATE, "is_true")
        atom = atomspace.get_atom(atom_id)
        assert atom.truth_value == 1.0
        assert atom.confidence == 1.0

    def test_custom_truth_value(self, atomspace):
        atom_id = atomspace.add_atom(
            AtomType.PREDICATE, "probably_true", truth_value=0.8, confidence=0.9
        )
        atom = atomspace.get_atom(atom_id)
        assert atom.truth_value == pytest.approx(0.8)
        assert atom.confidence == pytest.approx(0.9)

    def test_all_atom_types(self, atomspace):
        for atom_type in AtomType:
            atom_id = atomspace.add_atom(atom_type, f"test_{atom_type.value}")
            assert atomspace.get_atom(atom_id) is not None


class TestAtomRetrieval:
    def test_get_atoms_by_type(self, atomspace):
        atomspace.add_atom(AtomType.CONCEPT, "c1")
        atomspace.add_atom(AtomType.CONCEPT, "c2")
        atomspace.add_atom(AtomType.NODE, "n1")
        concepts = atomspace.get_atoms_by_type(AtomType.CONCEPT)
        names = {a.name for a in concepts}
        assert "c1" in names
        assert "c2" in names
        assert "n1" not in names

    def test_get_atoms_by_name(self, atomspace):
        atomspace.add_atom(AtomType.CONCEPT, "shared_name")
        atoms = atomspace.get_atoms_by_name("shared_name")
        assert len(atoms) >= 1
        assert all(a.name == "shared_name" for a in atoms)

    def test_get_nonexistent_atom(self, atomspace):
        assert atomspace.get_atom("nonexistent_id") is None


# ---------------------------------------------------------------------------
# Links
# ---------------------------------------------------------------------------

class TestLinks:
    def test_create_link(self, atomspace):
        src_id = atomspace.add_atom(AtomType.CONCEPT, "source_node")
        tgt_id = atomspace.add_atom(AtomType.CONCEPT, "target_node")
        link_id = atomspace.create_link(AtomType.LINK, src_id, tgt_id)
        assert link_id is not None

    def test_link_connects_atoms(self, atomspace):
        src_id = atomspace.add_atom(AtomType.CONCEPT, "src")
        tgt_id = atomspace.add_atom(AtomType.CONCEPT, "tgt")
        link_id = atomspace.create_link(AtomType.LINK, src_id, tgt_id)

        src = atomspace.get_atom(src_id)
        tgt = atomspace.get_atom(tgt_id)
        assert link_id in src.outgoing
        assert link_id in tgt.incoming

    def test_link_with_invalid_source_returns_none(self, atomspace):
        tgt_id = atomspace.add_atom(AtomType.CONCEPT, "valid_tgt")
        link_id = atomspace.create_link(AtomType.LINK, "bad_src_id", tgt_id)
        assert link_id is None

    def test_link_with_invalid_target_returns_none(self, atomspace):
        src_id = atomspace.add_atom(AtomType.CONCEPT, "valid_src")
        link_id = atomspace.create_link(AtomType.LINK, src_id, "bad_tgt_id")
        assert link_id is None


# ---------------------------------------------------------------------------
# Pattern queries
# ---------------------------------------------------------------------------

class TestPatternQuery:
    def test_query_by_type(self, atomspace):
        atomspace.add_atom(AtomType.PREDICATE, "pred_one")
        results = atomspace.query_pattern({"type": AtomType.PREDICATE})
        assert any(a.name == "pred_one" for a in results)

    def test_query_by_name_substring(self, atomspace):
        atomspace.add_atom(AtomType.CONCEPT, "neural_network")
        results = atomspace.query_pattern({"name": "neural"})
        assert any("neural" in a.name for a in results)

    def test_query_by_min_truth(self, atomspace):
        atomspace.add_atom(AtomType.CONCEPT, "high_conf", truth_value=0.95)
        atomspace.add_atom(AtomType.CONCEPT, "low_conf", truth_value=0.1)
        results = atomspace.query_pattern({"min_truth": 0.9})
        names = {a.name for a in results}
        assert "high_conf" in names
        assert "low_conf" not in names


# ---------------------------------------------------------------------------
# Attention / activation spread
# ---------------------------------------------------------------------------

class TestAttention:
    def test_update_attention(self, atomspace):
        atom_id = atomspace.add_atom(AtomType.CONCEPT, "important_concept")
        atomspace.update_attention(atom_id, 0.75)
        assert atomspace.get_atom(atom_id).attention_value == pytest.approx(0.75)

    def test_spread_activation(self, atomspace):
        src_id = atomspace.add_atom(AtomType.CONCEPT, "activation_source")
        mid_id = atomspace.add_atom(AtomType.CONCEPT, "middle_node")
        atomspace.create_link(AtomType.LINK, src_id, mid_id)
        atomspace.spread_activation(src_id, decay=0.9)
        assert atomspace.get_atom(src_id).attention_value > 0

    def test_spread_activation_nonexistent_source(self, atomspace):
        # Should not raise
        atomspace.spread_activation("no_such_atom")


# ---------------------------------------------------------------------------
# Statistics
# ---------------------------------------------------------------------------

class TestStatistics:
    def test_statistics_keys(self, atomspace):
        stats = atomspace.get_statistics()
        assert "total_atoms" in stats
        assert "atoms_by_type" in stats
        assert "unique_names" in stats

    def test_statistics_count_increases(self, atomspace):
        before = atomspace.get_statistics()["total_atoms"]
        atomspace.add_atom(AtomType.NODE, "stat_node_unique_1")
        after = atomspace.get_statistics()["total_atoms"]
        assert after == before + 1


# ---------------------------------------------------------------------------
# Persistence (save / load)
# ---------------------------------------------------------------------------

class TestPersistence:
    def test_save_and_load(self, tmp_path):
        mgr = AtomSpaceManager()
        atom_id = mgr.add_atom(AtomType.CONCEPT, "persistent_concept", truth_value=0.7)

        save_path = str(tmp_path / "atomspace.json")
        assert mgr.save(save_path)

        mgr2 = AtomSpaceManager()
        assert mgr2.load(save_path)
        atom = mgr2.get_atom(atom_id)
        assert atom is not None
        assert atom.truth_value == pytest.approx(0.7)

    def test_load_merge(self, tmp_path):
        mgr = AtomSpaceManager()
        mgr.add_atom(AtomType.CONCEPT, "first_atom")
        save_path = str(tmp_path / "merge_test.json")
        mgr.save(save_path)

        mgr2 = AtomSpaceManager()
        mgr2.add_atom(AtomType.CONCEPT, "second_atom")
        mgr2.load(save_path, merge=True)

        names = {a.name for a in mgr2.atomspace.values()}
        assert "first_atom" in names
        assert "second_atom" in names

    def test_load_missing_file(self):
        mgr = AtomSpaceManager()
        assert mgr.load("/nonexistent/path/file.json") is False

    def test_clear(self, atomspace):
        atomspace.add_atom(AtomType.CONCEPT, "to_be_cleared")
        atomspace.clear()
        assert atomspace.get_statistics()["total_atoms"] == 0
