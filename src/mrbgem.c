#include <mruby.h>

#ifdef CONTROLS
#include "controls.h"
#endif

void
mrb_mruby_require_gem_init(mrb_state* mrb) {
#ifdef CONTROLS
  mrb_require_controls_init(mrb);
#endif
}

void
mrb_mruby_require_gem_final(mrb_state* mrb) {
}
