#include "hmr/prof.h"
#include "aux.h"
#include "error.h"
#include "fsm.h"
#include "hmr/hmr.h"
#include "hmr/tok.h"
#include "node.h"
#include "prof.h"
#include "tok.h"
#include <limits.h>
#include <stdlib.h>

void hmr_prof_dump(struct hmr_prof const *prof, FILE *restrict fp)
{
    fprintf(fp, "HEADER: %s\n", prof->header);
    fprintf(fp, "  Name: %s\n", prof->meta.name);
    fprintf(fp, "   Acc: %s\n", prof->meta.acc);
    fprintf(fp, "  Desc: %s\n", prof->meta.desc);
    fprintf(fp, "  Leng: %s\n", prof->meta.leng);
    fprintf(fp, "  Alph: %s\n", prof->meta.alph);
    fprintf(fp, "  Name: %s\n", prof->meta.name);
    fprintf(fp, "  ");
    for (unsigned i = 0; i < prof->symbols_size; ++i)
    {
        fprintf(fp, "       %c", prof->symbols[i]);
    }
    fprintf(fp, "\n");
}

void hmr_prof_init(struct hmr_prof *prof, struct hmr *hmr)
{
    prof_init(prof, hmr->error);
}

void prof_init(struct hmr_prof *prof, char *error)
{
    prof->header[0] = '\0';
    prof->meta.name[0] = '\0';
    prof->meta.acc[0] = '\0';
    prof->meta.desc[0] = '\0';
    prof->meta.leng[0] = '\0';
    prof->meta.alph[0] = '\0';
    prof->symbols_size = 0;
    prof->symbols[0] = '\0';
    prof->error = error;
    node_init(&prof->node);
}

enum hmr_rc prof_next_node(struct hmr_prof *prof, FILE *restrict fp,
                           struct hmr_aux *aux, enum hmr_state *state,
                           struct hmr_tok *tok)
{
    if (*state != HMR_FSM_PAUSE)
        return error(HMR_RUNTIMEERROR, prof->error,
                     "unexpected prof_next_node call");

    aux_init(aux);
    do
    {
        enum hmr_rc rc = HMR_SUCCESS;
        if ((rc = tok_next(tok, fp))) return rc;

        *state = fsm_next(*state, tok, aux, prof);
        if (*state == HMR_FSM_ERROR) return HMR_PARSEERROR;
        if (*state == HMR_FSM_BEGIN)
        {
            if (hmr_prof_length(prof) != prof->node.idx)
                return error_parse(tok, "profile length mismatch");
            return HMR_ENDNODE;
        }

    } while (*state != HMR_FSM_PAUSE);

    return HMR_SUCCESS;
}

enum hmr_rc prof_next_prof(struct hmr_prof *prof, FILE *restrict fp,
                           struct hmr_aux *aux, enum hmr_state *state,
                           struct hmr_tok *tok)
{
    if (*state != HMR_FSM_BEGIN)
        return error(HMR_RUNTIMEERROR, prof->error,
                     "unexpected prof_next_prof call");

    prof_init(prof, tok->error);
    aux_init(aux);
    do
    {
        enum hmr_rc rc = HMR_SUCCESS;
        if ((rc = tok_next(tok, fp))) return rc;

        if ((*state = fsm_next(*state, tok, aux, prof)) == HMR_FSM_ERROR)
            return HMR_PARSEERROR;

    } while (*state != HMR_FSM_PAUSE && *state != HMR_FSM_END);

    if (*state == HMR_FSM_END) return HMR_ENDFILE;

    return HMR_SUCCESS;
}

unsigned hmr_prof_length(struct hmr_prof const *prof)
{
    long v = strtol(prof->meta.leng, NULL, 10);
    if (v == LONG_MAX) return UINT_MAX;
    if (v == LONG_MIN) return 0;
    return (unsigned)v;
}
