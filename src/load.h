#include <mruby.h>

mrb_value mrb_require_load(mrb_state*, mrb_value path);
mrb_value mrb_toplevel_dir(mrb_state*);
void mrb_require_load_init(mrb_state*);

#ifdef CONTROLS
mrb_value mrb_require_controls_load(mrb_state*, mrb_value self);
#endif /* CONTROLS */
