#include <mruby.h>
#include <mruby/string.h>
#include <mruby/variable.h>

#include "load_error.h"

static mrb_value
new_load_error_message(mrb_state* mrb, mrb_value message) {
  return mrb_funcall(mrb, mrb_obj_value(E_LOAD_ERROR), "new", 1, message);
}

static mrb_value
new_load_error_reason(mrb_state* mrb, mrb_value path, const char* reason) {
  path = mrb_str_dup(mrb, path);
  mrb_obj_freeze(mrb, path);

  mrb_value message = mrb_str_new_lit(mrb, "cannot load such file");
  mrb_str_cat_lit(mrb, message, " -- ");
  mrb_str_cat_str(mrb, message, path);

  if(reason != NULL) {
    mrb_str_cat_lit(mrb, message, " (");
    mrb_str_cat_cstr(mrb, message, reason);
    mrb_str_cat_lit(mrb, message, ")");
  }

  mrb_obj_freeze(mrb, message);

  mrb_value load_error = new_load_error_message(mrb, message);

  mrb_iv_set(mrb, load_error, mrb_intern_lit(mrb, "path"), path);

  return load_error;
}

static mrb_value
new_load_error(mrb_state* mrb, mrb_value path) {
  return new_load_error_reason(mrb, path, NULL);
}

mrb_noreturn void
mrb_raise_load_error_message(mrb_state* mrb, mrb_value message) {
  mrb_value load_error = new_load_error_message(mrb, message);
  mrb_exc_raise(mrb, load_error);
}

mrb_noreturn void
mrb_raise_load_error_reason(mrb_state* mrb, mrb_value path, const char* reason) {
  mrb_value load_error = new_load_error_reason(mrb, path, reason);
  mrb_exc_raise(mrb, load_error);
}

mrb_noreturn void
mrb_raise_load_error(mrb_state* mrb, mrb_value path) {
  mrb_value load_error = new_load_error(mrb, path);
  mrb_exc_raise(mrb, load_error);
}

static mrb_value
mrb_load_error_path(mrb_state* mrb, mrb_value self) {
  return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "path"));
}

void
mrb_load_error_init(mrb_state* mrb) {
  struct RClass* load_error = mrb_define_class(mrb, "LoadError", mrb_class_get(mrb, "ScriptError"));

  mrb_define_method(mrb, load_error, "path", mrb_load_error_path, MRB_ARGS_NONE());
}

#ifdef CONTROLS
mrb_value
mrb_require_controls_load_error_raise(mrb_state* mrb, mrb_value self) {
  mrb_value path;

  mrb_get_args(mrb, "S", &path);

  mrb_raise_load_error(mrb, path);
}

mrb_value
mrb_require_controls_load_error_raise_reason(mrb_state* mrb, mrb_value self) {
  mrb_value path;
  const char* reason;

  mrb_get_args(mrb, "Sz", &path, &reason);

  mrb_raise_load_error_reason(mrb, path, reason);
}

mrb_value
mrb_require_controls_load_error_raise_message(mrb_state* mrb, mrb_value self) {
  mrb_value message;

  mrb_get_args(mrb, "S", &message);

  mrb_raise_load_error_message(mrb, message);
}
#endif /* CONTROLS */
