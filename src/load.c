#include <mruby.h>
#include <mruby/string.h>
#include <mruby/variable.h>

#include <string.h>

#include "load.h"
#include "expand_path.h"
#include "file_extension.h"

static inline mrb_value
expand_directory(mrb_state* mrb, mrb_value path) {
  return mrb_require_expand_path(mrb, mrb_str_new_lit(mrb, ".."), path);
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
  size_t program_name_length = RSTRING_LEN(program_name_str);

  if(program_name[0] == '-') {
    if(strncmp(program_name, "-", program_name_length) == 0) {
      return mrb_nil_value();
    } else if(strncmp(program_name, "-e", program_name_length) == 0) {
      return mrb_nil_value();
    }
  }

  const char* program_name_extension = get_file_extension(mrb, program_name_str);

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
