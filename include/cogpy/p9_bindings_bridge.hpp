/*++ BUILD Version: 0001
 *
 * p9_bindings_bridge.hpp — Unified 9P Multi-Language Binding Bridge
 *
 * Maps 9P2000 protocol types across four language implementations
 * (C/diod, Go/go9p, Lua/lua9p+luadata, C++/CogChannel) into the
 * NetCog L4 IPC layer for the NT-OC AGI-OS.
 *
 * Composition: /netcog(/vmad(diod | go9p | lua9p | luadata))
 *
 * Layer 4 (IPC) in the 7-layer NetCog integration model:
 *   NT4 LPC ports ↔ 9P2000 protocol ↔ OpenCog CogChannel
 *
 * Copyright (c) 2026 cogpy/WinKoGNN
 *--*/
#ifndef _P9_BINDINGS_BRIDGE_HPP_
#define _P9_BINDINGS_BRIDGE_HPP_

#ifdef __cplusplus

#include <cstdint>
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace cog {
namespace p9 {

/* ================================================================== */
/* 9P2000 Protocol Constants (shared across all bindings)              */
/* Canonical source: go9p/p9.go, diod/libnpfs/npfs.h, lua9p/9p.lua   */
/* ================================================================== */

/* Message types — 9P2000 T/R pairs */
enum class MsgType : uint8_t {
    Tversion = 100, Rversion = 101,
    Tauth    = 102, Rauth    = 103,
    Tattach  = 104, Rattach  = 105,
    /* Terror = 106 — never sent */
    Rerror   = 107,
    Tflush   = 108, Rflush   = 109,
    Twalk    = 110, Rwalk    = 111,
    Topen    = 112, Ropen    = 113,
    Tcreate  = 114, Rcreate  = 115,
    Tread    = 116, Rread    = 117,
    Twrite   = 118, Rwrite   = 119,
    Tclunk   = 120, Rclunk   = 121,
    Tremove  = 122, Rremove  = 123,
    Tstat    = 124, Rstat    = 125,
    Twstat   = 126, Rwstat   = 127,
};

/* Qid types — file identity */
enum QidType : uint8_t {
    QTFILE    = 0x00,
    QTLINK    = 0x01,
    QTSYMLINK = 0x02,
    QTTMP     = 0x04,
    QTAUTH    = 0x08,
    QTMOUNT   = 0x10,
    QTEXCL    = 0x20,
    QTAPPEND  = 0x40,
    QTDIR     = 0x80,
};

/* Open/Create mode flags */
enum OpenMode : uint8_t {
    OREAD   = 0, OWRITE  = 1, ORDWR   = 2, OEXEC   = 3,
    OTRUNC  = 16, OCEXEC  = 32, ORCLOSE = 64,
};

/* Directory mode bits */
constexpr uint32_t DMDIR       = 0x80000000;
constexpr uint32_t DMAPPEND    = 0x40000000;
constexpr uint32_t DMEXCL      = 0x20000000;
constexpr uint32_t DMMOUNT     = 0x10000000;
constexpr uint32_t DMAUTH      = 0x08000000;
constexpr uint32_t DMTMP       = 0x04000000;

/* Protocol constants */
constexpr uint16_t NOTAG       = 0xFFFF;
constexpr uint32_t NOFID       = 0xFFFFFFFF;
constexpr uint32_t IOHDRSZ     = 24;
constexpr uint32_t MSIZE       = 1048576 + IOHDRSZ;
constexpr uint16_t PORT        = 564;

/* ================================================================== */
/* Unified Qid — maps to all four implementations                      */
/*   C/diod:   Npqid { type, version, path }                          */
/*   Go/go9p:  Qid   { Type, Version, Path }                          */
/*   Lua/lua9p: table { type, version, path }                         */
/*   C++:      cog::p9::Qid                                           */
/* ================================================================== */
struct Qid {
    uint8_t  type;
    uint32_t version;
    uint64_t path;
};

/* ================================================================== */
/* Unified Dir — directory entry                                       */
/* ================================================================== */
struct Dir {
    uint16_t    type;
    uint32_t    dev;
    Qid         qid;
    uint32_t    mode;
    uint32_t    atime;
    uint32_t    mtime;
    uint64_t    length;
    std::string name;
    std::string uid;
    std::string gid;
    std::string muid;
};

/* ================================================================== */
/* Binding Abstraction — language-agnostic 9P operations               */
/* Each binding (diod, go9p, lua9p) implements this interface.         */
/* ================================================================== */

/* Fid handle — opaque per-binding */
using FidHandle = uint32_t;

/* 9P operation callbacks — mirrors diod's Npsrv ops and go9p's Srv */
struct P9Ops {
    std::function<int(const std::string& version, uint32_t msize)>
        attach;
    std::function<FidHandle(FidHandle root, const std::vector<std::string>& names)>
        walk;
    std::function<Qid(FidHandle fid, uint8_t mode)>
        open;
    std::function<std::vector<uint8_t>(FidHandle fid, uint64_t offset, uint32_t count)>
        read;
    std::function<uint32_t(FidHandle fid, uint64_t offset, const std::vector<uint8_t>& data)>
        write;
    std::function<void(FidHandle fid)>
        clunk;
    std::function<Dir(FidHandle fid)>
        stat;
    std::function<void(FidHandle fid)>
        remove;
};

/* ================================================================== */
/* Binding Registry — tracks available 9P implementations              */
/* ================================================================== */

enum class BindingType {
    Diod,       /* C — kernel-space, RDMA, multi-threaded */
    Go9p,       /* Go — user-space, goroutine-based, aifs */
    Lua9p,      /* Lua — lightweight scripting client */
    CogChannel, /* C++ — OpenCog AtomSpace persistence */
};

struct BindingInfo {
    BindingType type;
    std::string name;
    std::string language;
    std::string description;
    bool        available;
};

/* ================================================================== */
/* NT LPC ↔ 9P Bridge                                                  */
/*                                                                     */
/* Maps NT4 Local Procedure Call (LPC) port operations to 9P messages: */
/*   NtCreatePort      → Tversion + Tattach (create 9P session)       */
/*   NtConnectPort     → Tattach (client attach to server)            */
/*   NtRequestPort     → Twrite  (send request data)                  */
/*   NtReplyPort       → Rread   (receive reply data)                 */
/*   NtRequestWaitReplyPort → Twrite + Rread (synchronous RPC)        */
/*   NtClosePort       → Tclunk  (release fid)                        */
/*                                                                     */
/* The 9P namespace serves as the unified IPC substrate:               */
/*   /kernel/objects/   → NT object manager (ObCreateObject)          */
/*   /kernel/processes/ → NT process manager (PsCreateProcess)        */
/*   /kernel/memory/    → NT memory manager (MmCreateVad)             */
/*   /kernel/io/        → NT I/O manager (IoCallDriver)               */
/*   /ai/sessions/      → go9p aifs AI sessions                       */
/*   /ai/knowledge/     → go9p aifs knowledge graph (AtomSpace)       */
/*   /ai/topology/      → go9p aifs topology lifecycle                */
/* ================================================================== */

struct LpcTo9pBridge {
    /* The 9P namespace root for this bridge instance */
    std::string mount_point;

    /* Active binding for this bridge */
    BindingType active_binding;

    /* 9P operations (dispatched to the active binding) */
    P9Ops ops;

    /* NT LPC port handle (opaque, maps to HANDLE on Windows) */
    uint64_t lpc_port_handle;

    /* Root fid for the 9P session */
    FidHandle root_fid;
};

/* ================================================================== */
/* luadata Integration — Binary Data Serialization                     */
/*                                                                     */
/* luadata provides bit-level binary data manipulation used by lua9p   */
/* for 9P packet construction. We expose its layout system for         */
/* serializing NT kernel structures to/from 9P messages.               */
/*                                                                     */
/* Key types from luadata/data.h:                                      */
/*   data_t  { handle, offset, length, layout }                       */
/*   layout  { field_name → {offset, length, type, endian} }          */
/* ================================================================== */

struct DataLayout {
    struct Field {
        uint32_t    offset_bits;
        uint32_t    length_bits;
        std::string type;    /* "number" or "string" */
        std::string endian;  /* "le", "be", "host" */
    };
    std::vector<std::pair<std::string, Field>> fields;
};

/* Pre-defined layouts for common 9P packet types */
inline DataLayout qid_layout() {
    return {{
        {"type",    {0,   8,  "number", "le"}},
        {"version", {8,  32,  "number", "le"}},
        {"path",    {40, 64,  "number", "le"}},
    }};
}

inline DataLayout stat_layout() {
    return {{
        {"size",    {0,  16, "number", "le"}},
        {"type",    {16, 16, "number", "le"}},
        {"dev",     {32, 32, "number", "le"}},
        /* qid embedded at offset 64 */
        {"mode",    {168, 32, "number", "le"}},
        {"atime",   {200, 32, "number", "le"}},
        {"mtime",   {232, 32, "number", "le"}},
        {"length",  {264, 64, "number", "le"}},
    }};
}

/* ================================================================== */
/* Factory — create binding instances                                  */
/* ================================================================== */

inline std::vector<BindingInfo> list_bindings() {
    return {
        {BindingType::Diod,       "diod",       "C",   "Kernel-space 9P server (RDMA, multi-threaded)", true},
        {BindingType::Go9p,       "go9p",       "Go",  "User-space 9P client/server + AI filesystem",  true},
        {BindingType::Lua9p,      "lua9p",      "Lua", "Lightweight scripting client",                  true},
        {BindingType::CogChannel, "cogchannel", "C++", "OpenCog AtomSpace persistence channel",         true},
    };
}

} // namespace p9
} // namespace cog

#endif /* __cplusplus */
#endif /* _P9_BINDINGS_BRIDGE_HPP_ */
