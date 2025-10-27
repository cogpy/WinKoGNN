"""Configuration management for OpenCog Workbench."""

import json
import logging
from typing import Dict, Any, Optional
from pathlib import Path

logger = logging.getLogger(__name__)


class ConfigManager:
    """Manages configuration for the OpenCog Workbench"""
    
    DEFAULT_CONFIG = {
        'orchestrator': {
            'max_agents': 50,
            'task_queue_size': 1000,
            'coordination_timeout': 30
        },
        'hypervisor': {
            'total_memory_kb': 1024 * 1024,  # 1GB
            'total_cpu_units': 100,
            'nt4_integration': True,
            'default_isolation': 'process'
        },
        'gnn': {
            'input_dim': 128,
            'hidden_dim': 256,
            'output_dim': 64,
            'num_layers': 3,
            'learning_rate': 0.001
        },
        'atomspace': {
            'max_atoms': 1000000,
            'attention_decay': 0.9,
            'enable_persistence': False
        },
        'reasoning': {
            'max_inference_steps': 100,
            'confidence_threshold': 0.7,
            'enabled_rules': ['deduction', 'modus_ponens', 'similarity']
        },
        'nt4_bridge': {
            'enable_kernel_calls': True,
            'default_privilege_level': 3,
            'memory_pool_size_kb': 512 * 1024  # 512MB
        },
        'logging': {
            'level': 'INFO',
            'format': '%(asctime)s - %(name)s - %(levelname)s - %(message)s',
            'file': None
        }
    }
    
    def __init__(self, config_path: Optional[str] = None):
        self.config_path = config_path
        self.config = self.DEFAULT_CONFIG.copy()
        
        if config_path:
            self.load_config(config_path)
            
        self._setup_logging()
        logger.info("ConfigManager initialized")
        
    def load_config(self, config_path: str) -> bool:
        """Load configuration from file"""
        try:
            path = Path(config_path)
            if not path.exists():
                logger.warning(f"Config file not found: {config_path}, using defaults")
                return False
                
            with open(config_path, 'r') as f:
                loaded_config = json.load(f)
                
            # Merge with defaults
            self._merge_config(self.config, loaded_config)
            
            logger.info(f"Loaded configuration from {config_path}")
            return True
            
        except Exception as e:
            logger.error(f"Error loading config: {e}")
            return False
            
    def save_config(self, config_path: Optional[str] = None) -> bool:
        """Save configuration to file"""
        try:
            path = config_path or self.config_path
            if not path:
                logger.error("No config path specified")
                return False
                
            with open(path, 'w') as f:
                json.dump(self.config, f, indent=2)
                
            logger.info(f"Saved configuration to {path}")
            return True
            
        except Exception as e:
            logger.error(f"Error saving config: {e}")
            return False
            
    def _merge_config(self, base: Dict[str, Any], updates: Dict[str, Any]):
        """Recursively merge configuration updates"""
        for key, value in updates.items():
            if key in base and isinstance(base[key], dict) and isinstance(value, dict):
                self._merge_config(base[key], value)
            else:
                base[key] = value
                
    def _setup_logging(self):
        """Setup logging based on configuration"""
        log_config = self.config.get('logging', {})
        level = getattr(logging, log_config.get('level', 'INFO'))
        format_str = log_config.get('format', '%(asctime)s - %(name)s - %(levelname)s - %(message)s')
        
        logging.basicConfig(
            level=level,
            format=format_str,
            filename=log_config.get('file')
        )
        
    def get(self, key: str, default: Any = None) -> Any:
        """Get configuration value by key"""
        keys = key.split('.')
        value = self.config
        
        for k in keys:
            if isinstance(value, dict) and k in value:
                value = value[k]
            else:
                return default
                
        return value
        
    def set(self, key: str, value: Any):
        """Set configuration value by key"""
        keys = key.split('.')
        config = self.config
        
        for k in keys[:-1]:
            if k not in config:
                config[k] = {}
            config = config[k]
            
        config[keys[-1]] = value
        
    def get_section(self, section: str) -> Dict[str, Any]:
        """Get entire configuration section"""
        return self.config.get(section, {})
