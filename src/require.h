#include <mruby.h>

mrb_bool mrb_require(mrb_state*, mrb_value feature);
void mrb_require_init(mrb_state*);

#ifdef CONTROLS
mrb_value mrb_require_controls_parse_load_paths(mrb_state*, mrb_value self);
#endif /* CONTROLS */
