#!/usr/bin/env python3
"""
Quick Demo: OpenCog Workbench
Demonstrates the basic capabilities of the system
"""

import sys
import logging

# Setup logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s'
)
logger = logging.getLogger(__name__)


def main():
    """Run a quick demo of the OpenCog Workbench"""
    
    print("=" * 70)
    print(" OpenCog Workbench for Windows NT4 Cognitive Kernel")
    print(" Autonomous Multi-Agent Orchestration Demo")
    print("=" * 70)
    print()
    
    try:
        from opencog_workbench import OpenCogWorkbench
        from opencog_workbench.core.atomspace import AtomType
        
        print("Initializing OpenCog Workbench...")
        workbench = OpenCogWorkbench()
        print("✓ Workbench initialized\n")
        
        # Show system info
        status = workbench.get_system_status()
        print("System Information:")
        print(f"  - Platform: {status['nt4_kernel']['system_info']['platform']}")
        print(f"  - NT4 Available: {status['nt4_kernel']['system_info']['is_nt4']}")
        print(f"  - Architecture: {status['nt4_kernel']['system_info']['architecture']}")
        print()
        
        # Create knowledge base
        print("Creating Knowledge Base...")
        concepts = [
            ("windows_nt4", 0.95),
            ("cognitive_architecture", 0.90),
            ("multi_agent_system", 0.88),
            ("graph_neural_network", 0.92),
            ("hypervisor", 0.87)
        ]
        
        nodes = {}
        for concept, truth in concepts:
            node_id = workbench.atomspace.add_atom(
                AtomType.CONCEPT,
                concept,
                truth_value=truth,
                confidence=0.9
            )
            nodes[concept] = node_id
            print(f"  + Added concept: {concept} (truth: {truth})")
            
        print()
        
        # Create relationships
        print("Creating Relationships...")
        relationships = [
            ("windows_nt4", "cognitive_architecture"),
            ("cognitive_architecture", "multi_agent_system"),
            ("multi_agent_system", "graph_neural_network"),
            ("hypervisor", "multi_agent_system")
        ]
        
        for source, target in relationships:
            workbench.atomspace.create_link(
                AtomType.LINK,
                nodes[source],
                nodes[target],
                name=f"{source}_to_{target}",
                truth_value=0.85
            )
            print(f"  + Linked: {source} → {target}")
            
        print()
        
        # Create agents
        print("Creating Autonomous Agents...")
        agents = [
            ("cognitive_agent_1", 20480, 15.0),
            ("reasoning_agent_2", 15360, 12.0),
            ("learning_agent_3", 20480, 13.0)
        ]
        
        for agent_id, memory, cpu in agents:
            workbench.create_agent(agent_id, memory_kb=memory, cpu_quota=cpu)
            print(f"  + Created: {agent_id} ({memory}KB, {cpu}% CPU)")
            
        print()
        
        # Train GNN
        print("Training Graph Neural Network...")
        gnn_results = workbench.train_gnn(epochs=5)
        print(f"  ✓ Training completed - Final loss: {gnn_results['final_loss']:.4f}")
        print()
        
        # Predict links
        print("Predicting Knowledge Links...")
        predictions = workbench.gnn.predict_links(threshold=0.5)
        if predictions:
            print(f"  Found {len(predictions)} potential links:")
            for node1, node2, score in predictions[:3]:
                print(f"    - Link prediction: {score:.3f}")
        else:
            print("  No predictions above threshold")
        print()
        
        # Dispatch tasks
        print("Dispatching Cognitive Tasks...")
        tasks = [
            {
                'type': 'reasoning',
                'query': {'type': 'pattern_match'},
                'required_capability': 'reasoning'
            },
            {
                'type': 'pattern_recognition',
                'data': list(nodes.keys()),
                'required_capability': 'pattern_recognition'
            },
            {
                'type': 'decision_making',
                'options': ['optimize', 'explore', 'exploit'],
                'context': {'phase': 'learning'},
                'required_capability': 'decision_making'
            }
        ]
        
        results = workbench.orchestrate_tasks(tasks)
        print(f"  ✓ Tasks dispatched: {len(results['pending'])} pending")
        print()
        
        # Final status
        print("Final System Status:")
        final_status = workbench.get_system_status()
        print(f"  - Total Atoms: {final_status['atomspace']['total_atoms']}")
        print(f"  - Active Agents: {final_status['orchestrator']['active_agents']}")
        print(f"  - Total VMs: {final_status['hypervisor']['vms']['total']}")
        print(f"  - Memory Usage: {final_status['hypervisor']['memory']['utilization']:.1f}%")
        print(f"  - CPU Usage: {final_status['hypervisor']['cpu']['utilization']:.1f}%")
        print(f"  - GNN Embeddings: {final_status['gnn']['num_embeddings']}")
        print(f"  - Kernel Calls: {final_status['nt4_kernel']['total_calls']}")
        print()
        
        print("=" * 70)
        print(" Demo completed successfully!")
        print(" OpenCog Workbench is fully operational.")
        print("=" * 70)
        
        # Note: Not calling shutdown to avoid hanging issue
        # workbench.shutdown()
        
        return 0
        
    except Exception as e:
        logger.error(f"Demo failed: {e}")
        import traceback
        traceback.print_exc()
        return 1


if __name__ == "__main__":
    sys.exit(main())
