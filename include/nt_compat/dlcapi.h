/*++ BUILD Version: 0001
 * dlcapi.h — DLC API Compatibility Shim
 *--*/
#ifndef _DLCAPI_H_
#define _DLCAPI_H_

#include "dlc.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── DLC API function stubs ── */
static inline ULONG AcsLan(PLLC_CCB pCcb, PVOID pBadCcb) {
    (void)pCcb; (void)pBadCcb;
    return LLC_STATUS_SUCCESS;
}

static inline ULONG GetAdapterStatus(UCHAR AdapterNumber, PVOID pBuffer, ULONG BufferSize) {
    (void)AdapterNumber; (void)pBuffer; (void)BufferSize;
    return LLC_STATUS_SUCCESS;
}

#ifdef __cplusplus
}
#endif

#endif /* _DLCAPI_H_ */
