#ifndef CONTROLS
# error "CONTROLS must be defined, e.g. -D CONTROLS=on"
#endif

#include <mruby.h>

void mrb_require_controls_init(mrb_state*);
