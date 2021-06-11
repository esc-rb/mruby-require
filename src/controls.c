#ifdef CONTROLS
#include <mruby.h>

#include "controls.h"
#include "load_error.h"

void
mrb_require_controls_init(mrb_state* mrb) {
  if(!mrb_class_defined(mrb, "Controls")) {
    mrb_define_module(mrb, "Controls");
  }

  struct RClass* controls_module = mrb_module_get(mrb, "Controls");

  struct RClass* require_module = mrb_define_module_under(mrb, controls_module, "Require");

  struct RClass* load_error_module = mrb_define_module_under(mrb, require_module, "LoadError");
  mrb_define_class_method(mrb, load_error_module, "raise", mrb_require_controls_load_error_raise, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, load_error_module, "raise_reason", mrb_require_controls_load_error_raise_reason, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, load_error_module, "raise_message", mrb_require_controls_load_error_raise_message, MRB_ARGS_REQ(1));
}
#endif /* CONTROLS */
