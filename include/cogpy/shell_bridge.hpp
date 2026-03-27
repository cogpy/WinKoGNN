/*++ BUILD Version: 0001
 *
 * shell_bridge.hpp — Unified Shell Bridge for NetCog
 *
 * Maps two shell layers into the NT-OC cognitive kernel:
 *
 *   airc   (C)          → L2 (Process) + L4 (IPC)
 *     rc AST node types → AtomSpace atom types
 *     9cog.rc commands  → AIFS 9P operations
 *     rc shell process  → NT PsCreateProcess + cmd.exe host
 *
 *   cogpwsh (PowerShell) → L5 (Drivers/Reasoning) + L6 (Self)
 *     AtomSpace.psm1    → oc::AtomSpace (direct mapping)
 *     PatternMatcher    → oc::PatternMatcher
 *     PLN/Deduction     → oc::pln::PLNReasoner
 *     KernelGenerator   → cog::grip::Workflow kernel synthesis
 *     PowerShell host   → NT cmd.exe / pwsh.exe process
 *
 * Integration with cog0_orchestrator.hpp:
 *   OrcMode::SCRIPT → airc rc shell evaluation
 *   OrcMode::REASON → cogpwsh PLN deduction
 *   OrcMode::QUERY  → cogpwsh PatternMatcher
 *
 *--*/
#ifndef _SHELL_BRIDGE_HPP_
#define _SHELL_BRIDGE_HPP_

#include <cstdint>
#include <cstddef>

namespace cog {
namespace shell {

// ============================================================================
// airc — rc Shell AST Types (from rc.h)
// Maps to: NT Process (Ps), OpenCog AtomSpace atoms
// ============================================================================

namespace rc {

    // rc AST node types — direct mapping from rc.h nodetype enum
    // Each maps to an OpenCog atom type for knowledge representation
    enum class NodeType : uint8_t {
        ANDALSO   = 0,   // → AndLink
        ASSIGN    = 1,   // → StateLink
        BACKQ     = 2,   // → ExecutionOutputLink
        BANG      = 3,   // → NotLink
        BODY      = 4,   // → SequentialAndLink
        CBODY     = 5,   // → SequentialAndLink (case body)
        NOWAIT    = 6,   // → ParallelLink (background)
        BRACE     = 7,   // → ScopeLink
        CONCAT    = 8,   // → ListLink
        COUNT     = 9,   // → ArityLink
        ELSE      = 10,  // → ConditionalLink (else branch)
        FLAT      = 11,  // → FlattenLink
        DUP       = 12,  // → NtDuplicateObject
        EPILOG    = 13,  // → SequentialAndLink (cleanup)
        NEWFN     = 14,  // → DefinedSchemaNode
        FORIN     = 15,  // → MapLink
        IF        = 16,  // → ConditionalLink
        ORELSE    = 17,  // → OrLink
        PIPE      = 18,  // → PipeLink (9P channel)
        PRE       = 19,  // → PredicateNode
        REDIR     = 20,  // → NtCreateFile (I/O redirect)
        RMFN      = 21,  // → DeleteLink
        ARGS      = 22,  // → ListLink (arguments)
        SUBSHELL  = 23,  // → PsCreateProcess (child)
        CASE      = 24,  // → ChoiceLink
        SWITCH    = 25,  // → ChoiceLink (multi-branch)
        MATCH     = 26,  // → PatternLink
        VAR       = 27,  // → VariableNode
        VARSUB    = 28,  // → GetLink
        WHILE     = 29,  // → WhileLink
        WORD      = 30,  // → ConceptNode (literal)
        LAPPEND   = 31,  // → AppendLink
        NMPIPE    = 32,  // → NamedPipeLink (NT named pipe)
    };

    // rc exception codes — map to NT SEH exception types
    enum class ExceptionCode : uint8_t {
        ERROR     = 0,   // STATUS_UNSUCCESSFUL
        BREAK     = 1,   // STATUS_BREAKPOINT
        RETURN    = 2,   // STATUS_SUCCESS
        VARSTACK  = 3,   // Stack variable scope exit
        ARENA     = 4,   // Memory arena cleanup
        FIFO      = 5,   // FIFO/pipe cleanup
        FD        = 6,   // File descriptor cleanup
        CONTINUE  = 7,   // STATUS_CONTINUE
    };

    // rc I/O redirection types — map to NT NtCreateFile dispositions
    enum class RedirType : uint8_t {
        FROM        = 0, // FILE_OPEN (read)
        CREATE      = 1, // FILE_OVERWRITE_IF (write)
        APPEND      = 2, // FILE_OPEN_IF + FILE_APPEND_DATA
        HEREDOC     = 3, // Temporary file + FILE_OPEN
        HERESTRING  = 4, // Temporary file + FILE_OPEN
    };

    // 9cog shell command — maps to AIFS 9P operation
    // From 9cog.rc: ai, aichat, know, model, topo, features, sessions
    enum class AifsCommand : uint8_t {
        AI          = 0,  // Write to /ai/sessions/$pid/ctl → AifsOp::GENERATE
        AICHAT      = 1,  // Multi-turn → AifsOp::GENERATE (with history)
        KNOW        = 2,  // Write to /ai/knowledge/query → AifsOp::QUERY
        MODEL       = 3,  // Read/write /ai/models/active → AifsOp::OPEN
        TOPO        = 4,  // Read /ai/topology/* → AifsOp::READ
        FEATURES    = 5,  // Read /ai/features/* → AifsOp::STAT
        SESSIONS    = 6,  // Manage /ai/sessions/* → AifsOp::WALK
        STATUS      = 7,  // Read /ai/status → AifsOp::STAT
    };

    // rc shell AST node — lightweight representation for NT-OC bridge
    struct AstNode {
        NodeType    type;
        uint32_t    atom_handle;    // AtomSpace handle (0 = not yet registered)
        const char* word;           // For WORD nodes: the literal text
        AstNode*    left;           // First child / condition
        AstNode*    right;          // Second child / body
    };

    // rc shell variable — maps to AtomSpace StateLink
    struct Variable {
        const char*  name;
        const char** values;        // rc variables are lists of strings
        size_t       count;
        uint32_t     atom_handle;   // StateLink handle in AtomSpace
    };

    // rc shell function — maps to DefinedSchemaNode
    struct Function {
        const char*  name;
        AstNode*     body;          // AST of function body
        uint32_t     atom_handle;   // DefinedSchemaNode handle
    };

} // namespace rc

// ============================================================================
// cogpwsh — Cognitive PowerShell Types
// Maps to: L5 (Reasoning) + L6 (Self/Autognosis)
// ============================================================================

namespace pwsh {

    // OpenCog atom types from cogpwsh Atoms.psm1
    enum class AtomType : uint8_t {
        // Nodes
        CONCEPT_NODE        = 0,
        PREDICATE_NODE      = 1,
        VARIABLE_NODE       = 2,
        NUMBER_NODE         = 3,
        SCHEMA_NODE         = 4,
        GROUNDED_SCHEMA_NODE= 5,
        TYPE_NODE           = 6,

        // Links
        INHERITANCE_LINK    = 20,
        EVALUATION_LINK     = 21,
        LIST_LINK           = 22,
        AND_LINK            = 23,
        OR_LINK             = 24,
        NOT_LINK            = 25,
        MEMBER_LINK         = 26,
        SUBSET_LINK         = 27,
        IMPLICATION_LINK    = 28,
        EXECUTION_LINK      = 29,
        BIND_LINK           = 30,
        SATISFACTION_LINK   = 31,
        STATE_LINK          = 32,
    };

    // TruthValue from cogpwsh TruthValues.psm1
    struct TruthValue {
        float strength;     // [0.0, 1.0]
        float confidence;   // [0.0, 1.0]
    };

    // Atom from cogpwsh Atoms.psm1
    struct Atom {
        uint32_t    handle;
        AtomType    type;
        const char* name;           // For nodes
        TruthValue  tv;
        uint32_t*   outgoing;       // For links: handles of target atoms
        size_t      outgoing_count;
    };

    // PLN Deduction Rule from cogpwsh DeductionRules.psm1
    // Maps to oc::pln::PLNReasoner
    struct DeductionResult {
        Atom        conclusion;
        TruthValue  computed_tv;
        float       confidence_gain;
    };

    // Universal Kernel Generator from cogpwsh KernelGenerator/
    // Maps to cog::grip::Workflow kernel synthesis
    struct KernelSpec {
        const char* domain;         // "physics", "chemistry", "computing", etc.
        uint32_t    order;          // B-series expansion order
        float       grip_contact;   // 40% weight
        float       grip_coverage;  // 30% weight
        float       grip_efficiency;// 20% weight
        float       grip_stability; // 10% weight
        float       grip_product;   // Weighted product
    };

    // PowerShell cmdlet → NT-OC operation mapping
    enum class CmdletOp : uint8_t {
        // AtomSpace operations
        NEW_ATOMSPACE       = 0,    // → oc::AtomSpace::init()
        ADD_ATOM            = 1,    // → oc::AtomSpace::add_node/link
        GET_ATOM            = 2,    // → oc::AtomSpace::get_atom
        REMOVE_ATOM         = 3,    // → oc::AtomSpace::remove_atom
        GET_ATOMS_BY_TYPE   = 4,    // → oc::AtomSpace::get_atoms_by_type

        // Pattern matching
        NEW_PATTERN_QUERY   = 10,   // → oc::PatternMatcher::match
        INVOKE_PATTERN_MATCH= 11,   // → oc::PatternMatcher::execute

        // PLN reasoning
        INVOKE_DEDUCTION    = 20,   // → oc::pln::PLNReasoner::deduce
        GET_TRUTH_VALUE     = 21,   // → oc::TruthValue::get

        // Kernel generation
        NEW_KERNEL_GENERATOR= 30,   // → cog::grip::Workflow::synthesize
        INVOKE_KERNEL_GEN   = 31,   // → cog::grip::Workflow::execute
    };

} // namespace pwsh

// ============================================================================
// Unified Shell Bridge — orchestrates both shell layers
// ============================================================================

struct ShellBridge {
    // rc shell state
    rc::Variable*   rc_variables;
    size_t          rc_var_count;
    rc::Function*   rc_functions;
    size_t          rc_fn_count;

    // cogpwsh state
    uint64_t        atomspace_ptr;      // Pointer to oc::AtomSpace
    uint32_t        atom_count;
    pwsh::KernelSpec current_kernel;

    // AIFS connection (from p9_aifs_bridge.hpp)
    const char*     aifs_mount_point;   // Default: "/ai"
    uint32_t        session_pid;        // Current session PID

    // Operation callbacks
    // rc shell operations
    int (*rc_parse)(const char* script, rc::AstNode** ast);
    int (*rc_eval)(rc::AstNode* ast, char* output, size_t max_output);
    int (*rc_aifs_cmd)(rc::AifsCommand cmd, const char* arg, char* result, size_t max_result);

    // cogpwsh operations
    int (*pwsh_cmdlet)(pwsh::CmdletOp op, const void* input, void* output);
    int (*pwsh_deduction)(const pwsh::Atom* premise_a, const pwsh::Atom* premise_b,
                          pwsh::DeductionResult* result);
    int (*pwsh_kernel_gen)(const pwsh::KernelSpec* spec, char* kernel_code, size_t max_code);
};

} // namespace shell
} // namespace cog

#endif /* _SHELL_BRIDGE_HPP_ */
