#include <mruby.h>

struct DynamicLibrary {
  void* resource;

  mrb_value path;

  size_t name_segment_length;
  char name_segment[];
};
typedef const struct DynamicLibrary* const DynamicLibrary;
typedef struct DynamicLibrary* const DynamicLibrary_mut;

mrb_bool mrb_load_dynamic_library(mrb_state*, mrb_value path_str);
void mrb_require_dynamic_library_init(mrb_state*);

#ifdef CONTROLS
mrb_value mrb_require_controls_dynamic_library_initialize(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_dynamic_library_path(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_dynamic_library_init_function_name(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_dynamic_library_final_function_name(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_dynamic_library_inspect(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_dynamic_library_load(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_dynamic_library_unload(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_dynamic_library_loaded_p(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_dynamic_library_loaded_libraries(mrb_state*, mrb_value self);
mrb_value mrb_require_controls_dynamic_library_class_load(mrb_state*, mrb_value self);
#endif /* CONTROLS */
