/*++ BUILD Version: 0001
 *
 * vmad_cluster_bridge.hpp — VMAD HPC Cluster Management Bridge for NetCog
 *
 * Maps HPC cluster management daemons (conman, munge, mrsh, powerman, pdsh)
 * to NT kernel subsystems and OpenCog cognitive architecture primitives.
 *
 * VMAD Principle: Each daemon is a service-as-a-skill with a persistent
 * name, address, and stock-standard port assignment.
 *
 * Layer Mapping:
 *   conman   → L6 (Self)      → NT KD (kernel debugger) → Autognosis L0-L1
 *   munge    → L0/L4 (Types/IPC) → NT Se (security)     → TruthValue identity
 *   mrsh     → L2/L4 (Proc/IPC)  → NT Ps (processes)    → MindAgent remote exec
 *   powerman → L6 (Self)      → NT Po (power manager)   → Autognosis L3
 *   pdsh     → L2 (Processes) → NT Ps (scheduler)       → Workflow parallel
 *
 *--*/
#ifndef _VMAD_CLUSTER_BRIDGE_HPP_
#define _VMAD_CLUSTER_BRIDGE_HPP_

#include <cstdint>
#include <cstddef>
#include <array>

namespace cog {
namespace vmad {

// ============================================================================
// VMAD Service Registry — stock-standard port assignments
// ============================================================================

struct ServiceEndpoint {
    const char* name;       // daemon name
    const char* p9_path;    // 9P namespace path
    uint16_t    port;       // stock port
    const char* protocol;   // transport protocol
};

// Per VMAD principle: all ports are stock-standard, no non-standard assignments
static constexpr ServiceEndpoint CLUSTER_SERVICES[] = {
    { "conman",   "/dev/console",  7890,  "serial/tcp" },
    { "munged",   "/auth/munge",   4096,  "unix/tcp"   },
    { "mrshd",    "/exec/mrsh",    514,   "tcp"        },
    { "powermand","/power/ctl",    10101, "tcp"        },
    { "pdsh",     "/exec/pdsh",    0,     "client"     },  // client-only
};
static constexpr size_t NUM_CLUSTER_SERVICES = 5;

// ============================================================================
// L0/L4: MUNGE — Authentication Credential Service
// Maps to: NT Security (Se), OpenCog TruthValue identity
// ============================================================================

namespace munge {

    // MUNGE credential — tamper-proof identity encoding
    // NT mapping: SECURITY_TOKEN + LUID
    struct Credential {
        uint32_t uid;           // Unix UID → NT SID
        uint32_t gid;           // Unix GID → NT SID group
        uint64_t encode_time;   // Timestamp of credential creation
        uint32_t ttl;           // Time-to-live in seconds
        uint8_t  cipher;        // Cipher type (AES-128/256)
        uint8_t  mac;           // MAC type (SHA-256/512)
        uint8_t  zip;           // Compression (none/zlib)
        uint8_t  realm_id;      // Security realm identifier
    };

    // Maps to NT Se token operations
    enum class Op : uint8_t {
        ENCODE  = 0,    // Se: NtCreateToken
        DECODE  = 1,    // Se: NtQueryInformationToken
        VALIDATE= 2,    // Se: SeAccessCheck
        REVOKE  = 3,    // Se: NtAdjustPrivilegesToken
    };

    // OpenCog mapping: Credential → TruthValue
    // strength = credential validity (0.0 = expired, 1.0 = fresh)
    // confidence = realm trust level
    struct CredentialTruth {
        float strength;     // validity: ttl_remaining / ttl_total
        float confidence;   // realm trust: 1.0 for local, 0.8 for remote
    };

} // namespace munge

// ============================================================================
// L2: PDSH — Parallel Distributed Shell
// Maps to: NT Process Scheduler (Ps), OpenCog Workflow parallel verb
// ============================================================================

namespace pdsh {

    // Fanout mode — how to distribute work across nodes
    // NT mapping: PsCreateProcess scheduling hints
    enum class FanoutMode : uint8_t {
        ALL     = 0,    // Execute on all nodes in collective
        WINDOW  = 1,    // Execute with sliding window (max N concurrent)
        BLOCK   = 2,    // Block until all complete
    };

    // Remote command module — pluggable transport
    // NT mapping: I/O driver model (IRP dispatch table)
    enum class RcmdType : uint8_t {
        RSH     = 0,    // Classic rsh (port 514)
        MRSH    = 1,    // Munge-authenticated rsh
        SSH     = 2,    // OpenSSH
        EXEC    = 3,    // Local exec (no remote)
    };

    // Parallel execution request
    // NT mapping: PsCreateProcess + batch scheduler
    // OpenCog mapping: cog::grip::Workflow with Verb::ACT in parallel
    struct ParallelExec {
        const char** hostlist;      // Target nodes
        size_t       host_count;    // Number of targets
        const char*  command;       // Command to execute
        FanoutMode   fanout;        // Distribution mode
        RcmdType     rcmd;          // Transport module
        uint32_t     window_size;   // Max concurrent (for WINDOW mode)
        uint32_t     timeout_sec;   // Per-node timeout
    };

    // Execution result per node
    struct NodeResult {
        const char* hostname;
        int32_t     exit_code;      // NT: NTSTATUS
        const char* stdout_buf;
        const char* stderr_buf;
        uint32_t    elapsed_ms;
    };

} // namespace pdsh

// ============================================================================
// L2/L4: MRSH — Munge Remote Shell
// Maps to: NT Ps (process creation) + Se (authentication) + LPC (IPC)
// ============================================================================

namespace mrsh {

    // Remote shell session — authenticated via MUNGE
    // NT mapping: NtCreateProcess + NtCreatePort (LPC)
    struct Session {
        munge::Credential   cred;       // Authentication credential
        const char*         remote_host;
        uint16_t            remote_port;
        int32_t             pty_fd;     // Pseudo-terminal (for mrlogin)
        bool                is_login;   // mrlogin vs mrsh
    };

    // Operations map to NT process + IPC syscalls
    enum class Op : uint8_t {
        CONNECT     = 0,    // NtCreatePort (LPC connect)
        EXEC        = 1,    // NtCreateProcess
        COPY        = 2,    // NtWriteFile (mrcp)
        LOGIN       = 3,    // NtCreateProcess + pty allocation
        DISCONNECT  = 4,    // NtClose
    };

} // namespace mrsh

// ============================================================================
// L6: CONMAN — Console Manager
// Maps to: NT KD (kernel debugger), Autognosis L0-L1
// ============================================================================

namespace conman {

    // Console connection — serial/telnet/IPMI SOL
    // NT mapping: KD serial port interface
    struct Console {
        const char* name;           // Console name (e.g., "node01-con")
        const char* device;         // Serial device or host:port
        uint32_t    baud_rate;      // Serial baud (9600, 115200, etc.)
        bool        is_serial;      // true = serial, false = telnet
        bool        logging;        // Log to file
    };

    // Console operations — map to Autognosis observation levels
    enum class Op : uint8_t {
        CONNECT     = 0,    // Autognosis L0: begin observation
        MONITOR     = 1,    // Autognosis L0: continuous observation
        BROADCAST   = 2,    // Autognosis L1: pattern broadcast
        REPLAY      = 3,    // Autognosis L1: pattern replay from log
        FORCE_CLOSE = 4,    // Autognosis L2: meta-cognitive intervention
    };

} // namespace conman

// ============================================================================
// L6: POWERMAN — Power Control Manager
// Maps to: NT Power Manager (Po), Autognosis L3 (self-modification)
// ============================================================================

namespace powerman {

    // Power state — maps to NT S-states (S0-S5)
    enum class PowerState : uint8_t {
        ON      = 0,    // S0: Working
        OFF     = 1,    // S5: Soft Off
        CYCLE   = 2,    // S5 → S0: Power cycle
        RESET   = 3,    // Hardware reset
        UNKNOWN = 4,    // Query failed
    };

    // Power device — maps to NT DEVICE_OBJECT with Po capabilities
    struct PowerDevice {
        const char* name;           // Device name
        const char* driver;         // Driver script (expect-like)
        const char* transport;      // telnet/serial/snmp/ipmi/redfish
        PowerState  current_state;
    };

    // Power operations — map to Autognosis L3 self-modification
    enum class Op : uint8_t {
        QUERY   = 0,    // Po: PoQueryPowerState → Autognosis L0
        ON      = 1,    // Po: PoSetPowerState(S0) → Autognosis L3
        OFF     = 2,    // Po: PoSetPowerState(S5) → Autognosis L3
        CYCLE   = 3,    // Po: S5→S0 transition → Autognosis L3
        RESET   = 4,    // HAL: HalReturnToFirmware → Autognosis L3
    };

} // namespace powerman

// ============================================================================
// Unified VMAD Cluster Orchestrator
// Composes all 5 daemons into a single cognitive cluster management layer
// ============================================================================

struct ClusterNode {
    const char*             hostname;
    munge::Credential       identity;
    powerman::PowerState    power_state;
    conman::Console         console;
    bool                    reachable;
};

// The VMAD cluster bridge — orchestrates all services
struct VmadClusterBridge {
    // Service registry
    const ServiceEndpoint*  services;
    size_t                  num_services;

    // Cluster state
    ClusterNode*            nodes;
    size_t                  num_nodes;

    // Operations (function pointers for runtime binding)
    // L0/L4: Authentication
    int (*munge_encode)(munge::Credential* cred, const void* payload, size_t len);
    int (*munge_decode)(const void* token, size_t len, munge::Credential* cred);

    // L2: Parallel execution
    int (*pdsh_exec)(const pdsh::ParallelExec* req, pdsh::NodeResult* results);

    // L2/L4: Remote shell
    int (*mrsh_connect)(mrsh::Session* session);
    int (*mrsh_exec)(mrsh::Session* session, const char* cmd);

    // L6: Console management
    int (*conman_connect)(conman::Console* con);
    int (*conman_monitor)(conman::Console* con);

    // L6: Power management
    int (*powerman_query)(powerman::PowerDevice* dev);
    int (*powerman_set)(powerman::PowerDevice* dev, powerman::PowerState state);
};

} // namespace vmad
} // namespace cog

#endif /* _VMAD_CLUSTER_BRIDGE_HPP_ */
