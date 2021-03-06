#ifndef HMR_STATE_H
#define HMR_STATE_H

enum hmr_state
{
    HMR_FSM_BEGIN,
    HMR_FSM_HEADER,
    HMR_FSM_NAME,
    HMR_FSM_CONTENT,
    HMR_FSM_SYMBOL,
    HMR_FSM_ARROW,
    HMR_FSM_COMPO,
    HMR_FSM_INSERT,
    HMR_FSM_MATCH,
    HMR_FSM_TRANS,
    HMR_FSM_PAUSE,
    HMR_FSM_SLASHED,
    HMR_FSM_END,
    HMR_FSM_ERROR
};

#endif
