"""
Example: Basic OpenCog Workbench Usage
Demonstrates autonomous multi-agent orchestration
"""

import sys
import logging
from opencog_workbench import OpenCogWorkbench
from opencog_workbench.core.atomspace import AtomType

# Setup logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)


def main():
    """Main example function"""
    logger.info("=== OpenCog Workbench Example ===")
    
    # Initialize workbench
    with OpenCogWorkbench() as workbench:
        logger.info("Workbench initialized")
        
        # Create some knowledge in AtomSpace
        logger.info("\n1. Creating knowledge base...")
        node1 = workbench.atomspace.add_atom(
            AtomType.CONCEPT, 
            "artificial_intelligence",
            truth_value=0.95,
            confidence=0.9
        )
        
        node2 = workbench.atomspace.add_atom(
            AtomType.CONCEPT,
            "machine_learning",
            truth_value=0.9,
            confidence=0.85
        )
        
        node3 = workbench.atomspace.add_atom(
            AtomType.CONCEPT,
            "neural_networks",
            truth_value=0.88,
            confidence=0.9
        )
        
        # Create relationships
        link1 = workbench.atomspace.create_link(
            AtomType.LINK,
            node1,
            node2,
            name="ai_includes_ml",
            truth_value=0.95
        )
        
        link2 = workbench.atomspace.create_link(
            AtomType.LINK,
            node2,
            node3,
            name="ml_uses_nn",
            truth_value=0.9
        )
        
        logger.info(f"Created {len(workbench.atomspace.atomspace)} atoms")
        
        # Create autonomous agents
        logger.info("\n2. Creating autonomous agents...")
        workbench.create_agent("agent_1", memory_kb=20480, cpu_quota=15.0)
        workbench.create_agent("agent_2", memory_kb=15360, cpu_quota=10.0)
        workbench.create_agent("agent_3", memory_kb=10240, cpu_quota=10.0)
        
        # Get system status
        status = workbench.get_system_status()
        logger.info(f"Active agents: {status['orchestrator']['active_agents']}")
        logger.info(f"Total agents: {status['orchestrator']['total_agents']}")
        
        # Dispatch tasks to agents
        logger.info("\n3. Dispatching tasks to agents...")
        tasks = [
            {
                'type': 'reasoning',
                'query': {'type': AtomType.CONCEPT},
                'required_capability': 'reasoning'
            },
            {
                'type': 'pattern_recognition',
                'data': ['ai', 'ml', 'nn'],
                'required_capability': 'pattern_recognition'
            },
            {
                'type': 'decision_making',
                'options': ['option_a', 'option_b', 'option_c'],
                'context': {'priority': 'high'},
                'required_capability': 'decision_making'
            }
        ]
        
        results = workbench.orchestrate_tasks(tasks)
        logger.info(f"Tasks orchestrated - Pending: {len(results['pending'])}, "
                   f"Failed: {len(results['failed'])}")
        
        # Train GNN on knowledge graph
        logger.info("\n4. Training Graph Neural Network...")
        gnn_results = workbench.train_gnn(epochs=5)
        logger.info(f"GNN training completed - Final loss: {gnn_results['final_loss']:.4f}")
        
        # Spread activation through knowledge graph
        logger.info("\n5. Performing activation spread...")
        workbench.atomspace.spread_activation(node1, decay=0.85)
        
        # Get final system status
        logger.info("\n6. Final system status:")
        final_status = workbench.get_system_status()
        
        logger.info(f"  AtomSpace: {final_status['atomspace']['total_atoms']} atoms")
        logger.info(f"  Agents: {final_status['orchestrator']['total_agents']}")
        logger.info(f"  Hypervisor VMs: {final_status['hypervisor']['vms']['total']}")
        logger.info(f"  Memory utilization: {final_status['hypervisor']['memory']['utilization']:.1f}%")
        logger.info(f"  CPU utilization: {final_status['hypervisor']['cpu']['utilization']:.1f}%")
        logger.info(f"  GNN embeddings: {final_status['gnn']['num_embeddings']}")
        logger.info(f"  NT4 kernel calls: {final_status['nt4_kernel']['total_calls']}")
        
        logger.info("\n=== Example completed successfully ===")


if __name__ == "__main__":
    main()
