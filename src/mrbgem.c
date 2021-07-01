#include <mruby.h>

#include "dynamic_library.h"
#include "load_error.h"
#include "load.h"
#include "require_relative.h"

#ifdef CONTROLS
#include "controls.h"
#endif

void
mrb_mruby_require_gem_init(mrb_state* mrb) {
  mrb_load_error_init(mrb);
  mrb_require_dynamic_library_init(mrb);
  mrb_require_load_init(mrb);
  mrb_require_relative_init(mrb);

#ifdef CONTROLS
  mrb_require_controls_init(mrb);
#endif
}

void
mrb_mruby_require_gem_final(mrb_state* mrb) {
  mrb_require_dynamic_library_final(mrb);
}
