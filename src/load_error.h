#include <mruby.h>

#define E_LOAD_ERROR (mrb_class_get(mrb, "LoadError"))

mrb_noreturn void mrb_raise_load_error(mrb_state*, mrb_value path);
mrb_noreturn void mrb_raise_load_error_reason(mrb_state*, mrb_value path, const char* reason);
mrb_noreturn void mrb_raise_load_error_message(mrb_state*, mrb_value message);
#define mrb_raise_load_error_message_lit(mrb, message_lit) (mrb_raise_load_error_message(mrb, mrb_str_new_lit(mrb, message_lit))

void mrb_load_error_init(mrb_state*);

#ifdef CONTROLS
mrb_value mrb_require_controls_load_error_raise(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_load_error_raise_reason(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_load_error_raise_message(mrb_state*, mrb_value self);
#endif /* CONTROLS */
