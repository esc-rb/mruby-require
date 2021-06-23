#include <mruby.h>

#ifdef CONTROLS
mrb_value mrb_require_controls_expand_path_join_segment(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_expand_path_join_path(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_expand_path_initial(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_expand_path_home_directory(mrb_state*, mrb_value self);
#endif /* CONTROLS */
