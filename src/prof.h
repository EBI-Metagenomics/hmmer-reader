#ifndef PROF_H
#define PROF_H

#include "hmr/rc.h"
#include <stdio.h>

enum hmr_state;
struct hmr_aux;
struct hmr_prof;
struct hmr_tok;

void prof_init(struct hmr_prof *prof, char *error);

enum hmr_rc prof_next_node(struct hmr_prof *prof, FILE *restrict fp,
                           struct hmr_aux *aux, enum hmr_state *state,
                           struct hmr_tok *tok);

enum hmr_rc prof_next_prof(struct hmr_prof *prof, FILE *restrict fp,
                           struct hmr_aux *aux, enum hmr_state *state,
                           struct hmr_tok *tok);

#endif
