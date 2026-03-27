/*++ BUILD Version: 0001
 * p9.h — 9P2000.L Protocol Types for NT-OC L4 IPC Layer
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides 9P protocol types derived from diod/libnpfs for use as the
 * universal IPC transport between NT kernel objects and OpenCog AtomSpace.
 *
 * In Plan 9, everything is a file accessed via 9P.
 * In NetCog, every kernel object is an atom accessed via 9P.
 *
 * Mapping:
 *   NT LPC ports      → 9P connections (Np9conn)
 *   NT HANDLEs         → 9P fids (Np9fid)
 *   NT PORT_MESSAGE    → 9P fcalls (Np9fcall)
 *   NT OBJECT_TYPE     → 9P qid type (Np9qid)
 *   NT IRP             → 9P request (Np9req)
 *
 * Source: diod (LLNL) — MIT licensed 9P2000.L server
 *   https://github.com/chaos/diod
 *--*/
#ifndef _NT_COMPAT_P9_H_
#define _NT_COMPAT_P9_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------ */
/* 9P2000.L base integer types (wire format: little-endian)            */
/* ------------------------------------------------------------------ */
typedef UCHAR   p9_u8;
typedef USHORT  p9_u16;
typedef ULONG   p9_u32;
typedef ULONGLONG p9_u64;

/* ------------------------------------------------------------------ */
/* 9P string — length-prefixed, no NULL terminator on wire             */
/* ------------------------------------------------------------------ */
typedef struct _Np9str {
    p9_u16  len;
    char   *str;
} Np9str;

/* ------------------------------------------------------------------ */
/* 9P QID — unique file identity (13 bytes on wire)                    */
/* Maps to: NT OBJECT_ATTRIBUTES / OpenCog Handle                      */
/* ------------------------------------------------------------------ */
typedef struct _Np9qid {
    p9_u8   type;       /* QTDIR=0x80, QTAPPEND=0x40, QTEXCL=0x20, QTFILE=0 */
    p9_u32  version;    /* version counter (monotonic) */
    p9_u64  path;       /* unique path identifier */
} Np9qid;

/* QID type bits */
#define P9_QTDIR     0x80
#define P9_QTAPPEND  0x40
#define P9_QTEXCL    0x20
#define P9_QTMOUNT   0x10
#define P9_QTAUTH    0x08
#define P9_QTTMP     0x04
#define P9_QTFILE    0x00

/* ------------------------------------------------------------------ */
/* 9P message types — T=request, R=response                            */
/* Maps to: NT IRP major function codes                                */
/* ------------------------------------------------------------------ */
enum Np9MsgType {
    /* 9P2000.L operations */
    P9_Tlerror      = 6,   P9_Rlerror      = 7,
    P9_Tstatfs      = 8,   P9_Rstatfs      = 9,
    P9_Tlopen       = 12,  P9_Rlopen       = 13,
    P9_Tlcreate     = 14,  P9_Rlcreate     = 15,
    P9_Tsymlink     = 16,  P9_Rsymlink     = 17,
    P9_Tmknod       = 18,  P9_Rmknod       = 19,
    P9_Trename      = 20,  P9_Rrename      = 21,
    P9_Treadlink    = 22,  P9_Rreadlink    = 23,
    P9_Tgetattr     = 24,  P9_Rgetattr     = 25,
    P9_Tsetattr     = 26,  P9_Rsetattr     = 27,
    P9_Txattrwalk   = 30,  P9_Rxattrwalk   = 31,
    P9_Txattrcreate = 32,  P9_Rxattrcreate = 33,
    P9_Treaddir     = 40,  P9_Rreaddir     = 41,
    P9_Tfsync       = 50,  P9_Rfsync       = 51,
    P9_Tlock        = 52,  P9_Rlock        = 53,
    P9_Tgetlock     = 54,  P9_Rgetlock     = 55,
    P9_Tlink        = 70,  P9_Rlink        = 71,
    P9_Tmkdir       = 72,  P9_Rmkdir       = 73,
    P9_Trenameat    = 74,  P9_Rrenameat    = 75,
    P9_Tunlinkat    = 76,  P9_Runlinkat    = 77,
    /* 9P2000 base operations */
    P9_Tversion     = 100, P9_Rversion     = 101,
    P9_Tauth        = 102, P9_Rauth        = 103,
    P9_Tattach      = 104, P9_Rattach      = 105,
    P9_Tflush       = 108, P9_Rflush       = 109,
    P9_Twalk        = 110, P9_Rwalk        = 111,
    P9_Tread        = 116, P9_Rread        = 117,
    P9_Twrite       = 118, P9_Rwrite       = 119,
    P9_Tclunk       = 120, P9_Rclunk       = 121,
    P9_Tremove      = 122, P9_Rremove      = 123,
};

/* Maximum walk elements (from diod protocol.h) */
#define P9_MAXWELEM 16

/* ------------------------------------------------------------------ */
/* 9P FID — client file handle                                         */
/* Maps to: NT HANDLE / OpenCog AtomRef                                */
/* ------------------------------------------------------------------ */
typedef struct _Np9fid {
    p9_u32      fid;        /* numeric fid identifier */
    p9_u8       type;       /* qid type of the referenced object */
    PVOID       conn;       /* back-pointer to connection */
    PVOID       user;       /* authenticated user context */
    PVOID       aux;        /* server-specific auxiliary data */
    char       *aname;      /* attach name (mount point) */
    int         flags;      /* FID_FLAGS_* */
    int         refcount;   /* reference count */
} Np9fid, *PNp9fid;

#define P9_FID_FLAGS_ZOMBIE  0x01
#define P9_FID_FLAGS_ROFS    0x02

/* ------------------------------------------------------------------ */
/* 9P Fcall — protocol message (request or response)                   */
/* Maps to: NT PORT_MESSAGE / OpenCog Channel::Message                 */
/* ------------------------------------------------------------------ */
typedef struct _Np9fcall {
    p9_u32      size;       /* total message size */
    p9_u8       type;       /* Np9MsgType */
    p9_u16      tag;        /* request tag for multiplexing */
    p9_u8      *pkt;        /* raw packet data */
    /* Decoded fields — union selected by type */
    union {
        /* Tversion/Rversion */
        struct { p9_u32 msize; Np9str version; } version;
        /* Tattach */
        struct { p9_u32 fid; p9_u32 afid; Np9str uname; Np9str aname; p9_u32 n_uname; } tattach;
        /* Rattach */
        struct { Np9qid qid; } rattach;
        /* Twalk */
        struct { p9_u32 fid; p9_u32 newfid; p9_u16 nwname; Np9str wnames[P9_MAXWELEM]; } twalk;
        /* Rwalk */
        struct { p9_u16 nwqid; Np9qid wqids[P9_MAXWELEM]; } rwalk;
        /* Tread */
        struct { p9_u32 fid; p9_u64 offset; p9_u32 count; } tread;
        /* Rread */
        struct { p9_u32 count; p9_u8 *data; } rread;
        /* Twrite */
        struct { p9_u32 fid; p9_u64 offset; p9_u32 count; p9_u8 *data; } twrite;
        /* Rwrite */
        struct { p9_u32 wcount; } rwrite;
        /* Tclunk/Tremove */
        struct { p9_u32 fid; } tclunk;
        /* Rlerror */
        struct { p9_u32 ecode; } rlerror;
        /* Tlopen */
        struct { p9_u32 fid; p9_u32 flags; } tlopen;
        /* Rlopen/Rlcreate */
        struct { Np9qid qid; p9_u32 iounit; } rlopen;
        /* Tgetattr */
        struct { p9_u32 fid; p9_u64 request_mask; } tgetattr;
        /* Tflush */
        struct { p9_u16 oldtag; } tflush;
    } u;
} Np9fcall, *PNp9fcall;

/* ------------------------------------------------------------------ */
/* 9P Transport — abstraction over TCP/RDMA/FD                         */
/* Maps to: NT TDI_TRANSPORT / OpenCog Channel::Transport              */
/* ------------------------------------------------------------------ */
typedef struct _Np9trans {
    PVOID   aux;            /* transport-specific context */
    int     (*recv)(PNp9fcall *fc, p9_u32 msize, PVOID aux);
    int     (*send)(PNp9fcall fc, PVOID aux);
    void    (*destroy)(PVOID aux);
} Np9trans, *PNp9trans;

/* ------------------------------------------------------------------ */
/* 9P Request — pending operation                                      */
/* Maps to: NT IRP / OpenCog grip::Verb                                */
/* ------------------------------------------------------------------ */
typedef struct _Np9req {
    PVOID       conn;       /* Np9conn* — owning connection */
    p9_u16      tag;        /* request tag */
    PNp9fcall   tcall;      /* T-message (request) */
    PNp9fcall   rcall;      /* R-message (response) */
    PNp9fid     fid;        /* associated fid */
    struct _Np9req *next;   /* linked list */
    struct _Np9req *prev;
} Np9req, *PNp9req;

/* ------------------------------------------------------------------ */
/* 9P Server — the daemon instance                                     */
/* Maps to: NT LPC_PORT_OBJECT / OpenCog AtomSpace                     */
/* This is the VMAD: a persistent daemon with a name and address.      */
/* ------------------------------------------------------------------ */
typedef struct _Np9srv {
    p9_u32      msize;      /* maximum message size */
    PVOID       srvaux;     /* server-specific context */
    int         flags;      /* SRV_FLAGS_* */

    /* 9P operation callbacks — the "file system" interface */
    /* Each maps to an NT I/O operation and an AtomSpace operation */
    PNp9fcall   (*version)(PVOID conn, p9_u32 msize, Np9str *ver);
    PNp9fcall   (*attach)(PNp9fid fid, PNp9fid afid, Np9str *aname);
    int         (*clone)(PNp9fid fid, PNp9fid newfid);
    int         (*walk)(PNp9fid fid, Np9str *wname, Np9qid *wqid);
    PNp9fcall   (*read)(PNp9fid fid, p9_u64 offset, p9_u32 count, PNp9req req);
    PNp9fcall   (*write)(PNp9fid fid, p9_u64 offset, p9_u32 count, p9_u8 *data, PNp9req req);
    PNp9fcall   (*clunk)(PNp9fid fid);
    PNp9fcall   (*remove)(PNp9fid fid);
    PNp9fcall   (*statfs)(PNp9fid fid);
    PNp9fcall   (*lopen)(PNp9fid fid, p9_u32 flags);
    PNp9fcall   (*lcreate)(PNp9fid fid, Np9str *name, p9_u32 flags, p9_u32 mode, p9_u32 gid);
    PNp9fcall   (*readdir)(PNp9fid fid, p9_u64 offset, p9_u32 count, PNp9req req);
    PNp9fcall   (*getattr)(PNp9fid fid, p9_u64 request_mask);
    PNp9fcall   (*setattr)(PNp9fid fid, p9_u32 valid, p9_u32 mode, p9_u32 uid, p9_u32 gid,
                           p9_u64 size, p9_u64 atime_sec, p9_u64 atime_nsec,
                           p9_u64 mtime_sec, p9_u64 mtime_nsec);
    PNp9fcall   (*fsync)(PNp9fid fid, p9_u32 datasync);
    PNp9fcall   (*link)(PNp9fid dfid, PNp9fid fid, Np9str *name);
    PNp9fcall   (*mkdir)(PNp9fid fid, Np9str *name, p9_u32 mode, p9_u32 gid);
    PNp9fcall   (*unlinkat)(PNp9fid fid, Np9str *name, p9_u32 flags);
    PNp9fcall   (*renameat)(PNp9fid olddirfid, Np9str *oldname,
                            PNp9fid newdirfid, Np9str *newname);
} Np9srv, *PNp9srv;

/* ------------------------------------------------------------------ */
/* 9P Server flags (from diod)                                         */
/* ------------------------------------------------------------------ */
#define P9_SRV_FLAGS_DEBUG_9PTRACE  0x00000001
#define P9_SRV_FLAGS_DEBUG_USER     0x00000002
#define P9_SRV_FLAGS_SETFSID        0x00010000
#define P9_SRV_FLAGS_AUTHCONN       0x00020000
#define P9_SRV_FLAGS_DAC_BYPASS     0x00200000

/* ------------------------------------------------------------------ */
/* 9P Connection — client session                                      */
/* Maps to: NT LPC_CONNECTION / OpenCog Channel::Connection            */
/* ------------------------------------------------------------------ */
typedef struct _Np9conn {
    p9_u32      msize;      /* negotiated message size */
    char        client_id[128];
    int         flags;      /* CONN_FLAGS_* */
    PNp9srv     srv;        /* owning server */
    PNp9trans   trans;      /* transport layer */
    PVOID       aux;        /* connection-specific context */
} Np9conn, *PNp9conn;

#define P9_CONN_FLAGS_PRIVPORT  0x00000001

/* ------------------------------------------------------------------ */
/* 9P ↔ NT IPC mapping macros                                          */
/* These map 9P operations to NT kernel equivalents                    */
/* ------------------------------------------------------------------ */

/* Map 9P attach to NT port connection */
#define Np9AttachToPort(fid, aname) \
    /* NtConnectPort equivalent — bind fid to named namespace */

/* Map 9P walk to NT object name resolution */
#define Np9WalkToObject(fid, path) \
    /* ObOpenObjectByName equivalent — resolve path to object */

/* Map 9P read/write to NT file I/O */
#define Np9ReadFromHandle(fid, offset, count) \
    /* NtReadFile equivalent — read data from object */

#define Np9WriteToHandle(fid, offset, count, data) \
    /* NtWriteFile equivalent — write data to object */

/* ------------------------------------------------------------------ */
/* Default 9P port (IANA assigned for 9P)                              */
/* Per VMAD: stock standard port assignments only                      */
/* ------------------------------------------------------------------ */
#define P9_DEFAULT_PORT     564
#define P9_DIOD_PORT        5641    /* diod default */

#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_P9_H_ */
