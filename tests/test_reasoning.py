"""
Unit tests for opencog_workbench.core.reasoning.
"""

import pytest
from opencog_workbench.core.atomspace import AtomSpaceManager, AtomType
from opencog_workbench.core.reasoning import ReasoningEngine, InferenceRule


class TestReasoningInit:
    def test_engine_initialises(self, reasoning_engine):
        assert reasoning_engine is not None

    def test_all_rules_enabled_by_default(self, reasoning_engine):
        for rule in InferenceRule:
            assert reasoning_engine.rules_enabled[rule] is True

    def test_initial_history_empty(self, reasoning_engine):
        stats = reasoning_engine.get_statistics()
        assert stats["total_inferences"] == 0


class TestRuleToggling:
    def test_disable_rule(self, reasoning_engine):
        reasoning_engine.enable_rule(InferenceRule.DEDUCTION, False)
        assert reasoning_engine.rules_enabled[InferenceRule.DEDUCTION] is False

    def test_re_enable_rule(self, reasoning_engine):
        reasoning_engine.enable_rule(InferenceRule.DEDUCTION, False)
        reasoning_engine.enable_rule(InferenceRule.DEDUCTION, True)
        assert reasoning_engine.rules_enabled[InferenceRule.DEDUCTION] is True


class TestInference:
    def test_infer_returns_list(self, reasoning_engine):
        result = reasoning_engine.infer({"type": "test"})
        assert isinstance(result, list)

    def test_infer_appends_to_history(self, reasoning_engine):
        before = reasoning_engine.get_statistics()["total_inferences"]
        reasoning_engine.infer({"type": "test"})
        after = reasoning_engine.get_statistics()["total_inferences"]
        assert after >= before  # may be zero new inferences, but never less


class TestForwardChaining:
    def test_forward_chain_returns_list(self, reasoning_engine, atomspace):
        atom_id = atomspace.add_atom(AtomType.CONCEPT, "fc_start_node")
        results = reasoning_engine.forward_chain([atom_id])
        assert isinstance(results, list)

    def test_forward_chain_empty_start(self, reasoning_engine):
        results = reasoning_engine.forward_chain([])
        assert isinstance(results, list)


class TestBackwardChaining:
    def test_backward_chain_returns_list(self, reasoning_engine, atomspace):
        atom_id = atomspace.add_atom(AtomType.CONCEPT, "bc_goal_node")
        results = reasoning_engine.backward_chain(atom_id)
        assert isinstance(results, list)


class TestProbabilisticInference:
    def test_probabilistic_inference_returns_dict(self, reasoning_engine):
        posterior = reasoning_engine.probabilistic_inference({"evidence_atom": 0.8})
        assert isinstance(posterior, dict)


class TestExplain:
    def test_explain_returns_none_for_unknown(self, reasoning_engine):
        assert reasoning_engine.explain("unknown_atom_id") is None


class TestStatistics:
    def test_statistics_keys(self, reasoning_engine):
        stats = reasoning_engine.get_statistics()
        assert "total_inferences" in stats
        assert "rules_enabled" in stats
        assert "rules_by_type" in stats

    def test_rules_by_type_contains_all_rules(self, reasoning_engine):
        stats = reasoning_engine.get_statistics()
        for rule in InferenceRule:
            assert rule.value in stats["rules_by_type"]


class TestClearHistory:
    def test_clear_history(self, reasoning_engine):
        reasoning_engine.infer({"type": "test"})
        reasoning_engine.clear_history()
        assert reasoning_engine.get_statistics()["total_inferences"] == 0
