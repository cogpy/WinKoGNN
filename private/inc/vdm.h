/*++ BUILD Version: 0001
 * vdm.h — Virtual DOS Machine compatibility stub for NetCog unified build
 *--*/
#ifndef _VDM_H_
#define _VDM_H_
#ifdef __cplusplus
extern "C" {
#endif
/* VDM (Virtual DOS Machine) types used by ntvdm subsystem */
#ifndef _VDM_TYPES_DEFINED
#define _VDM_TYPES_DEFINED
typedef ULONG VDM_INTERRUPT_HANDLER;
typedef PVOID PVDM_TIB;
typedef PVOID PVDM_PROCESS_OBJECTS;
#endif
#ifdef __cplusplus
}
#endif
#endif /* _VDM_H_ */
