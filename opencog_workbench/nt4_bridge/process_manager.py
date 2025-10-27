"""NT4 Process Manager."""

import logging

logger = logging.getLogger(__name__)


class NT4ProcessManager:
    """Manages processes for NT4 kernel bridge"""
    
    def __init__(self):
        self.processes = {}
        logger.info("NT4ProcessManager initialized")
