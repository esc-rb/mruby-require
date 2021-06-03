#ifdef CONTROLS
#include <mruby.h>

#include "controls.h"

void
mrb_require_controls_init(mrb_state* mrb) {
  if(!mrb_class_defined(mrb, "Controls")) {
    mrb_define_module(mrb, "Controls");
  }
}
#endif /* CONTROLS */
