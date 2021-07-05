#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/error.h>
#include <mruby/string.h>
#include <mruby/variable.h>

#include <string.h>

#include "load.h"
#include "dynamic_library.h"
#include "expand_path.h"
#include "file_extension.h"

static inline mrb_value
expand_directory(mrb_state* mrb, mrb_value path) {
  return mrb_require_expand_path(mrb, mrb_str_new_lit(mrb, ".."), path);
}

static mrb_value
load_file(mrb_state* mrb, void* path_str_ptr) {
  mrb_value path_str = *((mrb_value*) path_str_ptr);

  const char* path = RSTRING_CSTR(mrb, path_str);

  FILE* file = fopen(path, "r");
  if(file == NULL) {
    return mrb_false_value();
  }

  mrbc_context* mrbc_context = mrbc_context_new(mrb);
  mrbc_filename(mrb, mrbc_context, path);

  mrb_load_detect_file_cxt(mrb, file, mrbc_context);

  return mrb_true_value();
}

mrb_value
mrb_require_load(mrb_state* mrb, mrb_value path_str) {
  const char* path = RSTRING_CSTR(mrb, path_str);

  const char* path_extension = get_file_extension_cstr(mrb, path, RSTRING_LEN(path_str));
  if(strcmp(".so", path_extension) == 0) {
    if(mrb_load_dynamic_library(mrb, path_str)) {
      return mrb_true_value();
    } else {
      return mrb_false_value();
    }
  }

  mrb_sym dir_sym = mrb_intern_lit(mrb, "__dir__");
  mrb_value directory = expand_directory(mrb, path_str);
  mrb_value previous_directory = mrb_gv_get(mrb, dir_sym);

  mrb_gv_set(mrb, dir_sym, directory);

  mrb_value return_value = mrb_protect_error(mrb, load_file, &path_str, NULL);

  mrb_gv_set(mrb, dir_sym, previous_directory);

  return return_value;
}

mrb_value
mrb_toplevel_dir(mrb_state* mrb) {
  mrb_value __dir__ = mrb_gv_get(mrb, mrb_intern_lit(mrb, "__dir__"));

  if(!mrb_nil_p(__dir__)) {
    return __dir__;
  }

  mrb_value program_name_str = mrb_gv_get(mrb, mrb_intern_lit(mrb, "$0"));

  if(mrb_nil_p(program_name_str)) {
    return mrb_nil_value();
  }

  const char* program_name = RSTRING_CSTR(mrb, program_name_str);
  if(program_name[0] == '-') {
    if(strcmp(program_name, "-") == 0) {
      return mrb_nil_value();
    } else if(strcmp(program_name, "-e") == 0) {
      return mrb_nil_value();
    }
  }

  const char* program_name_extension = get_file_extension_cstr(mrb, program_name, RSTRING_LEN(program_name_str));
  if(strcmp(program_name_extension, ".rb") == 0 || strcmp(program_name_extension, ".mrb") == 0) {
    return expand_directory(mrb, program_name_str);
  }

  return mrb_nil_value();
}

static mrb_value
mrb_toplevel___dir__(mrb_state* mrb, mrb_value self) {
  return mrb_toplevel_dir(mrb);
}

void
mrb_require_load_init(mrb_state* mrb) {
  mrb_define_singleton_method(mrb, mrb->top_self, "__dir__", mrb_toplevel___dir__, MRB_ARGS_NONE());
}

#ifdef CONTROLS
mrb_value
mrb_require_controls_load(mrb_state* mrb, mrb_value self) {
  mrb_value path_str;

  mrb_get_args(mrb, "S", &path_str);

  return mrb_require_load(mrb, path_str);
}
#endif /* CONTROLS */
