"""
Unit tests for opencog_workbench.config.config_manager (ConfigManager).
"""

import json
import pytest
from opencog_workbench.config.config_manager import ConfigManager


# ---------------------------------------------------------------------------
# Initialisation
# ---------------------------------------------------------------------------

class TestConfigManagerInit:
    def test_default_config_loaded(self):
        cm = ConfigManager()
        assert "orchestrator" in cm.config
        assert "gnn" in cm.config
        assert "atomspace" in cm.config

    def test_load_from_file(self, tmp_json):
        data = {"orchestrator": {"max_agents": 99}}
        path = tmp_json(data)
        cm = ConfigManager(path)
        assert cm.get("orchestrator.max_agents") == 99

    def test_missing_file_uses_defaults(self):
        cm = ConfigManager("/nonexistent/path/config.json")
        # Should still have default keys
        assert "orchestrator" in cm.config

    def test_load_invalid_json_uses_defaults(self, tmp_path):
        bad_path = str(tmp_path / "bad.json")
        with open(bad_path, "w") as f:
            f.write("not valid json {{{")
        cm = ConfigManager(bad_path)
        assert "orchestrator" in cm.config


# ---------------------------------------------------------------------------
# Get / set
# ---------------------------------------------------------------------------

class TestGetSet:
    def test_get_top_level_section(self):
        cm = ConfigManager()
        section = cm.get_section("gnn")
        assert isinstance(section, dict)
        assert "input_dim" in section

    def test_get_nested_key(self):
        cm = ConfigManager()
        val = cm.get("gnn.num_layers")
        assert isinstance(val, int)

    def test_get_missing_key_returns_default(self):
        cm = ConfigManager()
        assert cm.get("does.not.exist", "default_val") == "default_val"

    def test_set_and_get_value(self):
        cm = ConfigManager()
        cm.set("gnn.learning_rate", 0.005)
        assert cm.get("gnn.learning_rate") == pytest.approx(0.005)

    def test_set_creates_nested_path(self):
        cm = ConfigManager()
        cm.set("new_section.new_key", 42)
        assert cm.get("new_section.new_key") == 42

    def test_get_section_returns_empty_dict_for_missing(self):
        cm = ConfigManager()
        assert cm.get_section("nonexistent_section") == {}


# ---------------------------------------------------------------------------
# Save / load round-trip
# ---------------------------------------------------------------------------

class TestSaveLoad:
    def test_save_creates_file(self, tmp_path):
        cm = ConfigManager()
        path = str(tmp_path / "saved_config.json")
        assert cm.save_config(path) is True
        assert (tmp_path / "saved_config.json").exists()

    def test_save_and_reload(self, tmp_path):
        cm = ConfigManager()
        cm.set("gnn.hidden_dim", 512)
        path = str(tmp_path / "round_trip.json")
        cm.save_config(path)

        cm2 = ConfigManager(path)
        assert cm2.get("gnn.hidden_dim") == 512

    def test_save_without_path_returns_false(self):
        cm = ConfigManager()
        assert cm.save_config(None) is False


# ---------------------------------------------------------------------------
# Config merge
# ---------------------------------------------------------------------------

class TestConfigMerge:
    def test_loaded_values_override_defaults(self, tmp_json):
        data = {"orchestrator": {"max_agents": 7}}
        path = tmp_json(data)
        cm = ConfigManager(path)
        assert cm.get("orchestrator.max_agents") == 7

    def test_unspecified_defaults_retained(self, tmp_json):
        data = {"orchestrator": {"max_agents": 7}}
        path = tmp_json(data)
        cm = ConfigManager(path)
        # task_queue_size should still be present from defaults
        assert cm.get("orchestrator.task_queue_size") is not None
