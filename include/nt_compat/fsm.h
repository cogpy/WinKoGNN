/*++ BUILD Version: 0001
 * fsm.h — Finite State Machine Compatibility Shim
 *
 * Used by DLC/LLC protocol state machines.
 *--*/
#ifndef _FSM_H_
#define _FSM_H_

#include "ntddk.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── FSM input events ── */
typedef enum _FSM_INPUT {
    FSM_INPUT_NONE = 0,
    FSM_INPUT_LOCAL_BUSY_SET,
    FSM_INPUT_LOCAL_BUSY_CLEAR,
    FSM_INPUT_RECEIVE_I_FRAME,
    FSM_INPUT_RECEIVE_RR,
    FSM_INPUT_RECEIVE_RNR,
    FSM_INPUT_RECEIVE_REJ,
    FSM_INPUT_RECEIVE_SABME,
    FSM_INPUT_RECEIVE_DISC,
    FSM_INPUT_RECEIVE_UA,
    FSM_INPUT_RECEIVE_DM,
    FSM_INPUT_RECEIVE_FRMR,
    FSM_INPUT_SEND_I_FRAME,
    FSM_INPUT_T1_EXPIRED,
    FSM_INPUT_T2_EXPIRED,
    FSM_INPUT_TI_EXPIRED,
    FSM_INPUT_CONNECT_REQUEST,
    FSM_INPUT_DISCONNECT_REQUEST,
    FSM_INPUT_RESET_REQUEST,
    FSM_INPUT_MAX
} FSM_INPUT;

/* ── FSM states ── */
typedef enum _FSM_STATE {
    FSM_STATE_ADM = 0,          /* Asynchronous Disconnect Mode */
    FSM_STATE_SETUP,
    FSM_STATE_RESET_WAIT,
    FSM_STATE_RESET_CHECK,
    FSM_STATE_RESET,
    FSM_STATE_D_CONN,
    FSM_STATE_ERROR,
    FSM_STATE_NORMAL,
    FSM_STATE_BUSY,
    FSM_STATE_REJECT,
    FSM_STATE_AWAIT,
    FSM_STATE_AWAIT_BUSY,
    FSM_STATE_AWAIT_REJECT,
    FSM_STATE_MAX
} FSM_STATE;

/* ── FSM action codes ── */
typedef enum _FSM_ACTION {
    FSM_ACTION_NONE = 0,
    FSM_ACTION_SEND_SABME,
    FSM_ACTION_SEND_UA,
    FSM_ACTION_SEND_DM,
    FSM_ACTION_SEND_DISC,
    FSM_ACTION_SEND_FRMR,
    FSM_ACTION_SEND_RR,
    FSM_ACTION_SEND_RNR,
    FSM_ACTION_SEND_REJ,
    FSM_ACTION_START_T1,
    FSM_ACTION_STOP_T1,
    FSM_ACTION_START_TI,
    FSM_ACTION_STOP_TI,
    FSM_ACTION_INDICATE_CONNECT,
    FSM_ACTION_INDICATE_DISCONNECT,
    FSM_ACTION_INDICATE_RESET,
    FSM_ACTION_MAX
} FSM_ACTION;

/* ── FSM transition table entry ── */
typedef struct _FSM_ENTRY {
    FSM_STATE   NextState;
    FSM_ACTION  Actions[4];
} FSM_ENTRY, *PFSM_ENTRY;

/* ── FSM context ── */
typedef struct _FSM_CONTEXT {
    FSM_STATE   CurrentState;
    PVOID       pLinkStation;
} FSM_CONTEXT, *PFSM_CONTEXT;

static inline void FsmInit(PFSM_CONTEXT Ctx) {
    Ctx->CurrentState = FSM_STATE_ADM;
    Ctx->pLinkStation = NULL;
}

#ifdef __cplusplus
}
#endif

#endif /* _FSM_H_ */
