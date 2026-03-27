/*++ BUILD Version: 0001
 *
 * p9_aifs_bridge.hpp — 9cog AI Filesystem Bridge for NetCog
 *
 * Maps the 9fs9rc AI filesystem namespace to NT kernel I/O and
 * OpenCog AtomSpace persistence via the 9P2000.L protocol.
 *
 * 9P Namespace (from 9fs9rc ARCHITECTURE.md):
 *   /ai/sessions/   → Active cognitive sessions (AtomSpace instances)
 *   /ai/models/     → Loaded LLM/cognitive models
 *   /ai/knowledge/  → Knowledge graph (AtomSpace persistence)
 *   /ai/topology/   → 120-cell Phoenix topology
 *
 * NT Mapping:
 *   /ai/sessions/   → IoCreateFile + ObCreateObject
 *   /ai/models/     → MmMapViewOfSection (model memory mapping)
 *   /ai/knowledge/  → CcInitializeCacheMap (knowledge cache)
 *   /ai/topology/   → HAL multiprocessor topology
 *
 * Z++ Formal Spec Integration:
 *   9p.z Qid/Stat/Message types → C++ structs
 *   rc.z Node/Variable/Function types → AtomSpace atoms
 *
 *--*/
#ifndef _P9_AIFS_BRIDGE_HPP_
#define _P9_AIFS_BRIDGE_HPP_

#include <cstdint>
#include <cstddef>

namespace cog {
namespace aifs {

// ============================================================================
// Z++ 9P Types (from specs/9p.z)
// ============================================================================

// Qid — server-side unique file identifier (Z++ SCHEMA Qid)
struct Qid {
    uint8_t  type;      // QTDIR, QTFILE, etc.
    uint32_t version;   // Version for cache coherence
    uint64_t path;      // Unique path identifier
};

// Stat — file metadata (Z++ SCHEMA Stat)
struct Stat {
    uint16_t size;
    uint16_t type;
    uint32_t dev;
    Qid      qid;
    uint32_t mode;
    uint32_t atime;
    uint32_t mtime;
    uint64_t length;
    char     name[256];
    char     uid[64];
    char     gid[64];
    char     muid[64];
};

// ============================================================================
// Z++ rc Shell Types (from specs/rc.z)
// ============================================================================

// NodeType — rc AST node types (Z++ FREE TYPE NodeType)
enum class RcNodeType : uint8_t {
    ASSIGN    = 0,  // Variable assignment
    COMMAND   = 1,  // Command execution
    PIPE      = 2,  // Pipeline
    AND       = 3,  // && conjunction
    OR        = 4,  // || disjunction
    SUBSHELL  = 5,  // () subshell
    IF        = 6,  // if/else
    WHILE     = 7,  // while loop
    FOR       = 8,  // for loop
    CASE      = 9,  // switch/case
};

// OpenCog mapping: rc AST → AtomSpace
// Each rc node type maps to an Atom type:
//   ASSIGN  → StateLink
//   COMMAND → ExecutionLink
//   PIPE    → SequentialAndLink
//   AND     → AndLink
//   OR      → OrLink
//   IF      → ConditionalLink (PLN)
//   FOR     → MapLink
//   CASE    → ChoiceLink

// ============================================================================
// AI Filesystem Namespace Entries
// ============================================================================

// Namespace path constants
static constexpr const char* AIFS_ROOT       = "/ai";
static constexpr const char* AIFS_SESSIONS   = "/ai/sessions";
static constexpr const char* AIFS_MODELS     = "/ai/models";
static constexpr const char* AIFS_KNOWLEDGE  = "/ai/knowledge";
static constexpr const char* AIFS_TOPOLOGY   = "/ai/topology";

// File type flags (9P Qid.type)
static constexpr uint8_t QTDIR  = 0x80;
static constexpr uint8_t QTFILE = 0x00;
static constexpr uint8_t QTAUTH = 0x08;

// ============================================================================
// AI Session — maps to AtomSpace instance + NT process
// ============================================================================

struct AiSession {
    uint32_t    session_id;     // Unique session ID
    Qid         qid;            // 9P file identity
    const char* model_name;     // Active model (e.g., "gpt-4", "echoself")
    uint64_t    atomspace_ptr;  // Pointer to AtomSpace instance
    uint32_t    nt_process_id;  // NT PID (PsCreateProcess)
    uint32_t    nt_thread_id;   // NT TID (primary thread)
};

// ============================================================================
// AI Model — maps to memory-mapped model file + NT section
// ============================================================================

struct AiModel {
    const char* name;           // Model name
    const char* path;           // Filesystem path
    uint64_t    size_bytes;     // Model size
    uint64_t    mmap_base;      // MmMapViewOfSection base address
    uint32_t    nt_section_handle; // NT section object handle
    bool        is_loaded;      // Currently in memory
};

// ============================================================================
// Knowledge Entry — maps to AtomSpace atom + NT cache
// ============================================================================

struct KnowledgeEntry {
    Qid         qid;            // 9P identity
    uint64_t    atom_handle;    // AtomSpace Handle
    uint16_t    atom_type;      // AtomType (Node, Link, etc.)
    float       sti;            // Short-Term Importance (ECAN)
    float       lti;            // Long-Term Importance (ECAN)
    float       tv_strength;    // TruthValue strength
    float       tv_confidence;  // TruthValue confidence
};

// ============================================================================
// 9P Operations for AI Filesystem
// ============================================================================

// AI-specific 9P operations (extend standard walk/open/read/write)
enum class AifsOp : uint8_t {
    // Standard 9P
    ATTACH      = 0,    // Connect to AI filesystem
    WALK        = 1,    // Navigate namespace
    OPEN        = 2,    // Open session/model/knowledge
    READ        = 3,    // Read data
    WRITE       = 4,    // Write data
    CLUNK       = 5,    // Close handle
    STAT        = 6,    // Get metadata

    // AI-specific extensions
    QUERY       = 10,   // AtomSpace query (pattern matching)
    INFER       = 11,   // PLN inference request
    ATTEND      = 12,   // ECAN attention update
    EVAL        = 13,   // rc shell evaluation
    GENERATE    = 14,   // LLM generation request
};

// ============================================================================
// AIFS Server — the cognitive filesystem server
// ============================================================================

struct AifsServer {
    // 9P server state
    uint16_t    port;           // Listen port (default: 564)
    uint32_t    max_sessions;   // Max concurrent sessions
    uint32_t    active_sessions;

    // Operation callbacks (9P + AI extensions)
    int (*on_attach)(AiSession* session, const char* aname);
    int (*on_walk)(AiSession* session, const char** wnames, size_t nwname, Qid* wqids);
    int (*on_open)(AiSession* session, uint32_t fid, uint8_t mode);
    int (*on_read)(AiSession* session, uint32_t fid, uint64_t offset, uint32_t count, void* buf);
    int (*on_write)(AiSession* session, uint32_t fid, uint64_t offset, uint32_t count, const void* buf);
    int (*on_clunk)(AiSession* session, uint32_t fid);
    int (*on_stat)(AiSession* session, uint32_t fid, Stat* stat);

    // AI extensions
    int (*on_query)(AiSession* session, const char* pattern, KnowledgeEntry* results, size_t max_results);
    int (*on_infer)(AiSession* session, const char* premise, KnowledgeEntry* conclusion);
    int (*on_attend)(AiSession* session, uint64_t atom_handle, float sti_delta, float lti_delta);
    int (*on_eval)(AiSession* session, const char* rc_script, char* output, size_t max_output);
    int (*on_generate)(AiSession* session, const char* prompt, char* response, size_t max_response);
};

} // namespace aifs
} // namespace cog

#endif /* _P9_AIFS_BRIDGE_HPP_ */
