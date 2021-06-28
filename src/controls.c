#ifdef CONTROLS
#include <mruby.h>
#include <mruby/string.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "controls.h"
#include "file_extension.h"
#include "expand_path.h"
#include "load_error.h"

static mrb_value
mrb_require_controls_file_extension(mrb_state* mrb, mrb_value self) {
  mrb_value path;

  mrb_get_args(mrb, "S", &path);

  const char* path_extension = get_file_extension(mrb, path);

  if(path_extension == NULL) {
    return mrb_nil_value();
  } else {
    return mrb_str_new_cstr(mrb, path_extension);
  }
}

static mrb_value
mrb_require_controls_environment_home_directory(mrb_state* mrb, mrb_value self) {
  char* home = getenv("HOME");

  return mrb_str_new_cstr_frozen(mrb, (const char*)home);
}

static mrb_value
mrb_require_controls_environment_current_user(mrb_state* mrb, mrb_value self) {
  char* user = getenv("USER");

  return mrb_str_new_cstr_frozen(mrb, (const char*)user);
}

static mrb_value
mrb_require_controls_environment_current_directory(mrb_state* mrb, mrb_value self) {
  char* current_directory = mrb_malloc(mrb, PATH_MAX);

  if(getcwd(current_directory, PATH_MAX) == NULL) {
    mrb_raisef(mrb, E_RUNTIME_ERROR, "Current directory length exceeds PATH_MAX limit (PATH_MAX: %d)", PATH_MAX);
  }

  size_t current_directory_length = strnlen(current_directory, PATH_MAX);

  mrb_value current_directory_str = mrb_str_new(mrb, current_directory, current_directory_length);

  mrb_free(mrb, current_directory);

  return current_directory_str;
}

void
mrb_require_controls_init(mrb_state* mrb) {
  if(!mrb_class_defined(mrb, "Controls")) {
    mrb_define_module(mrb, "Controls");
  }

  struct RClass* controls_module = mrb_module_get(mrb, "Controls");

  struct RClass* require_module = mrb_define_module_under(mrb, controls_module, "Require");
  mrb_define_class_method(mrb, require_module, "file_extension", mrb_require_controls_file_extension, MRB_ARGS_REQ(1));

  struct RClass* load_error_module = mrb_define_module_under(mrb, require_module, "LoadError");
  mrb_define_class_method(mrb, load_error_module, "raise", mrb_require_controls_load_error_raise, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, load_error_module, "raise_reason", mrb_require_controls_load_error_raise_reason, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, load_error_module, "raise_message", mrb_require_controls_load_error_raise_message, MRB_ARGS_REQ(1));

  struct RClass* expand_path_module = mrb_define_module_under(mrb, require_module, "ExpandPath");
  mrb_define_class_method(mrb, expand_path_module, "join_segment", mrb_require_controls_expand_path_join_segment, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  mrb_define_class_method(mrb, expand_path_module, "join", mrb_require_controls_expand_path_join_path, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, expand_path_module, "initial", mrb_require_controls_expand_path_initial, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, expand_path_module, "home_directory", mrb_require_controls_expand_path_home_directory, MRB_ARGS_REQ(2) | MRB_ARGS_OPT(1));
  mrb_define_class_method(mrb, expand_path_module, "current_home_directory", mrb_require_controls_expand_path_current_home_directory, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(2));
  mrb_define_class_method(mrb, expand_path_module, "current_directory", mrb_require_controls_expand_path_current_directory, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));

  struct RClass* environment_module = mrb_define_module_under(mrb, require_module, "Environment");
  mrb_define_class_method(mrb, environment_module, "home_directory", mrb_require_controls_environment_home_directory, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, environment_module, "current_user", mrb_require_controls_environment_current_user, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, environment_module, "current_directory", mrb_require_controls_environment_current_directory, MRB_ARGS_NONE());
}
#endif /* CONTROLS */
