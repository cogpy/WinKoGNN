# NT Source Registry — External Repos for WinKoGNN Integration

This document catalogs all discovered GitHub repositories containing NT kernel
headers, types, and source code that can be used to repair, enrich, and extend
the WinKoGNN NT compatibility layer.

## Tier 1 — Production-Quality Header Sources

| Repository | Stars | License | Headers Provided | Integration Status |
|-----------|-------|---------|-----------------|-------------------|
| [reactos/reactos](https://github.com/reactos/reactos) | 17.3k | GPL-2.0+ | ntddk.h, ntifs.h, ndis.h, dderror.h, miniport.h, prefix.h, wdm.h, tdikrnl.h, scsi.h, video.h + 100 more | Reference source for enriching shims |
| [winsiderss/phnt](https://github.com/winsiderss/phnt) | 1.4k | MIT | 43 headers: ntrtl.h, ntpsapi.h, ntmmapi.h, ntioapi.h, ntobapi.h, ntlpcapi.h, ntkeapi.h, ntseapi.h, ntzwapi.h + 34 more | Types integrated into nt_enriched_types.h |
| [arizvisa/ndk](https://github.com/arizvisa/ndk) | ~50 | GPL | 80+ headers split by subsystem: iotypes.h, pstypes.h, obtypes.h, lpctypes.h, ketypes.h, mmtypes.h + funcs | Kernel-internal types integrated |

## Tier 2 — NT4-Specific Source Repos

| Repository | Stars | Description | Value |
|-----------|-------|-------------|-------|
| [ZoloZiak/WinNT4](https://github.com/ZoloZiak/WinNT4) | ~100 | NT4 kernel source | Direct comparison for private/ntos tree |
| [lianthony/NT4.0](https://github.com/lianthony/NT4.0) | ~50 | NT4.0 source code | May have SDK/DDK headers |
| [fungosforks/Windows-NT-4.0-Source](https://github.com/fungosforks/Windows-NT-4.0-Source) | ~30 | Academic program source | Build tools and headers |
| [smartmaster/opennt](https://github.com/smartmaster/opennt) | ~10 | Buildable OpenNT | Has build scripts for full NT4 compilation |

## Tier 3 — Reference and Documentation

| Repository | Stars | Description | Value |
|-----------|-------|-------------|-------|
| [mic101/windows](https://github.com/mic101/windows) | ~100 | WRK v1.2 (NT 5.2) | Reference ntos.h, ke.h, mm.h, io.h, ob.h |
| [zhuhuibeishadiao/ntoskrnl](https://github.com/zhuhuibeishadiao/ntoskrnl) | ~200 | WRK mirror | Additional kernel reference |
| [tpn/winsdk-10](https://github.com/tpn/winsdk-10) | ~500 | Windows SDK 10 | Modern DDK headers for reference |
| [m417z/ntdoc](https://github.com/m417z/ntdoc) | ~300 | NT API documentation | Headers from official MS sources |
| [AdaCore/gsh](https://github.com/AdaCore/gsh) | ~50 | Public domain ntddk.h | Clean-room DDK header |

## Integration Map

The following table shows which WinKoGNN missing headers are covered by which
external source, and the current integration status.

| Missing Header | Refs | Our Shim | Best External Source | Status |
|---------------|------|----------|---------------------|--------|
| nt.h | 519 | Yes (enriched) | phnt + NDK combined | Integrated |
| ntrtl.h | 100+ | Yes (enriched) | phnt ntrtl.h (massive) | Integrated |
| ntdef.h | 100+ | Yes (enriched) | phnt phnt_ntdef.h | Integrated |
| ntstatus.h | 50+ | Yes | phnt phnt_windows.h | Integrated |
| ntddk.h | 32 | Yes | ReactOS sdk/include/ddk/ntddk.h | Shim + reference |
| ntifs.h | 62 | Yes | ReactOS sdk/include/ddk/ntifs.h | Shim + reference |
| ndis.h | 66 | Yes | ReactOS sdk/include/ddk/ndis.h | Shim + reference |
| ntsrv.h | 64 | Yes | None (NT4-specific) | Our shim only |
| dderror.h | 82 | Yes | ReactOS sdk/include/ddk/dderror.h | Shim + reference |
| miniport.h | 7 | Yes | ReactOS sdk/include/ddk/miniport.h | Shim + reference |
| oscfg.h | 22 | Yes | None (NT4 network stack) | Our shim only |
| nbtprocs.h | 22 | Yes | None (NT4 NetBT) | Our shim only |
| ftk_defs.h | 15 | Yes | None (NT4 FDDI) | Our shim only |
| dlc.h/llc.h | 25 | Yes | None (NT4 DLC) | Our shim only |
| prefix.h | 5 | Yes | ReactOS sdk/include/ddk/prefix.h | Shim + reference |

## Cognitive Integration Points

The enriched types from these sources enable the following cognitive mappings:

1. **OBJECT_TYPE → AtomType**: NDK obtypes.h provides the full OBJECT_TYPE
   structure with TypeInfo callbacks, enabling 1:1 mapping to OpenCog atom
   type handlers.

2. **LPCP_PORT_OBJECT → Cognitive Channel**: NDK lpctypes.h provides the
   complete LPC port internal structure, enabling message queue integration
   with ECAN attention allocation.

3. **EPROCESS → MindAgent**: NDK pstypes.h provides EPROCESS quota blocks
   that map directly to attention budgets (STI/LTI/VLTI).

4. **OBJECT_DIRECTORY → AtomSpace Namespace**: NDK obtypes.h provides the
   hash-bucket directory structure that maps to AtomSpace partitioning.

5. **IO_TIMER → Cognitive Scheduler**: NDK iotypes.h provides the I/O timer
   structure for periodic MindAgent scheduling.
