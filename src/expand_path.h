#include <mruby.h>

mrb_value mrb_require_expand_relative_path(mrb_state*, mrb_value path);
mrb_value mrb_require_expand_path(mrb_state* mrb, mrb_value path, mrb_value origin);

#ifdef CONTROLS
mrb_value mrb_require_controls_expand_path_join_segment(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_expand_path_join_path(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_expand_path_initial(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_expand_path_home_directory(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_expand_path_current_home_directory(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_expand_path_current_directory(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_expand_path_relative(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_expand_path(mrb_state*, mrb_value self);
#endif /* CONTROLS */
