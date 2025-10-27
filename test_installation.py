"""
Simple test to verify OpenCog Workbench installation
"""

import sys
import os

# Add parent directory to path
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

def test_imports():
    """Test that all modules can be imported"""
    print("Testing imports...")
    
    try:
        from opencog_workbench import (
            MultiAgentOrchestrator,
            AgentZeroHypervisor,
            GraphNeuralNetwork,
            NT4CognitiveKernel
        )
        print("✓ Main imports successful")
    except ImportError as e:
        print(f"✗ Import error: {e}")
        return False
        
    try:
        from opencog_workbench.core.atomspace import AtomSpaceManager, AtomType
        from opencog_workbench.core.reasoning import ReasoningEngine
        print("✓ Core modules imported")
    except ImportError as e:
        print(f"✗ Core import error: {e}")
        return False
        
    try:
        from opencog_workbench.agents.cognitive_agent import CognitiveAgent
        print("✓ Agent modules imported")
    except ImportError as e:
        print(f"✗ Agent import error: {e}")
        return False
        
    try:
        from opencog_workbench.config.config_manager import ConfigManager
        print("✓ Config modules imported")
    except ImportError as e:
        print(f"✗ Config import error: {e}")
        return False
        
    return True


def test_basic_functionality():
    """Test basic functionality"""
    print("\nTesting basic functionality...")
    
    try:
        from opencog_workbench import OpenCogWorkbench
        from opencog_workbench.core.atomspace import AtomType
        
        # Create workbench
        workbench = OpenCogWorkbench()
        print("✓ Workbench created")
        
        # Add atoms
        node1 = workbench.atomspace.add_atom(AtomType.CONCEPT, "test_concept")
        print(f"✓ Added atom: {node1}")
        
        # Create agent
        success = workbench.create_agent("test_agent", memory_kb=10240, cpu_quota=10.0)
        if success:
            print("✓ Agent created")
        else:
            print("✗ Agent creation failed")
            return False
            
        # Get status
        status = workbench.get_system_status()
        print(f"✓ System status retrieved: {status['orchestrator']['total_agents']} agents")
        
        # Shutdown
        workbench.shutdown()
        print("✓ Workbench shutdown")
        
        return True
        
    except Exception as e:
        print(f"✗ Functionality test error: {e}")
        import traceback
        traceback.print_exc()
        return False


def main():
    """Run all tests"""
    print("=" * 60)
    print("OpenCog Workbench Installation Test")
    print("=" * 60)
    
    all_passed = True
    
    # Test imports
    if not test_imports():
        all_passed = False
        
    # Test functionality
    if not test_basic_functionality():
        all_passed = False
        
    print("\n" + "=" * 60)
    if all_passed:
        print("✓ All tests passed!")
        print("OpenCog Workbench is ready to use.")
    else:
        print("✗ Some tests failed")
        print("Please check the errors above.")
    print("=" * 60)
    
    return 0 if all_passed else 1


if __name__ == "__main__":
    sys.exit(main())
