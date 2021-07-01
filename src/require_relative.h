#include <mruby.h>

mrb_value mrb_require_relative(mrb_state*, mrb_value relative_path, mrb_value origin);
void mrb_require_relative_init(mrb_state*);

#ifdef CONTROLS
mrb_value mrb_require_controls_require_relative(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_loading_files(mrb_state*, mrb_value self);
#endif /* CONTROLS */
