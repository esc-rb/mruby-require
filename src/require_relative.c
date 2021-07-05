#include <mruby.h>
#include <mruby/array.h>
#include <mruby/hash.h>
#include <mruby/string.h>
#include <mruby/variable.h>

#include <string.h>

#include "require_relative.h"
#include "expand_path.h"
#include "file_extension.h"
#include "load.h"
#include "load_error.h"

static inline mrb_value
mrb_require_loading_files(mrb_state* mrb) {
  return mrb_gv_get(mrb, mrb_intern_lit(mrb, "REQUIRE_LOADING_FILES"));
}

static inline mrb_value
get_loaded_features(mrb_state* mrb) {
  return mrb_gv_get(mrb, mrb_intern_lit(mrb, "$LOADED_FEATURES"));
}

static mrb_value
try_require(mrb_state* mrb, mrb_value path) {
  mrb_value loading_files = mrb_require_loading_files(mrb);
  mrb_value loaded_features = get_loaded_features(mrb);

  const char* extension = get_file_extension(mrb, path);
  if(extension == NULL) {
    mrb_value required;
    size_t original_path_length = RSTRING_LEN(path);

    mrb_str_cat_lit(mrb, path, ".so");
    required = try_require(mrb, path);
    if(!mrb_nil_p(required)) {
      return required;
    }
    mrb_str_resize(mrb, path, original_path_length);

    mrb_str_cat_lit(mrb, path, ".mrb");
    required = try_require(mrb, path);
    if(!mrb_nil_p(required)) {
      return required;
    }
    mrb_str_resize(mrb, path, original_path_length);

    mrb_str_cat_lit(mrb, path, ".rb");
    return try_require(mrb, path);
  }

  if(mrb_hash_key_p(mrb, loading_files, path)) {
    return mrb_false_value();
  }

  for(mrb_int index = 0; index < RARRAY_LEN(loaded_features); index++) {
    mrb_value loaded_feature = mrb_ary_entry(loaded_features, index);

    if(mrb_equal(mrb, loaded_feature, path)) {
      return mrb_false_value();
    }
  }

  mrb_hash_set(mrb, loading_files, path, mrb_true_value());

  mrb_value return_value = mrb_require_load(mrb, path);

  mrb_hash_delete_key(mrb, loading_files, path);

  if(mrb_obj_is_kind_of(mrb, return_value, mrb->eException_class)) {
    mrb_exc_raise(mrb, return_value);
  }

  if(mrb_true_p(return_value)) {
    mrb_ary_push(mrb, loaded_features, path);
    return mrb_true_value();
  } else {
    return mrb_nil_value();
  }
}

mrb_value
mrb_require_relative(mrb_state* mrb, mrb_value relative_path, mrb_value origin) {
  mrb_value path = mrb_require_expand_path(mrb, relative_path, origin);

  return try_require(mrb, path);
}

mrb_noreturn static mrb_value
mrb_kernel_require_relative(mrb_state* mrb, mrb_value self) {
  mrb_value relative_path;

  mrb_get_args(mrb, "S", &relative_path);

  mrb_raise_load_error_reason(mrb, relative_path, "require_relative cannot be called from a method");
}

static mrb_value
mrb_toplevel_require_relative(mrb_state* mrb, mrb_value self) {
  mrb_value relative_path;

  mrb_get_args(mrb, "S", &relative_path);

  mrb_value directory = mrb_toplevel_dir(mrb);
  if(mrb_nil_p(directory)) {
    mrb_raise_load_error_reason(mrb, relative_path, "__dir__ could not be determined");
  }

  mrb_value path = mrb_require_expand_path(mrb, relative_path, directory);

  mrb_value required = try_require(mrb, path);
  if(mrb_nil_p(required)) {
    mrb_raise_load_error(mrb, path);
  }

  return required;
}

void
mrb_require_relative_init(mrb_state* mrb) {
  mrb_value loading_files = mrb_hash_new(mrb);
  mrb_gv_set(mrb, mrb_intern_lit(mrb, "REQUIRE_LOADING_FILES"), loading_files);

  mrb_value loaded_features = mrb_ary_new(mrb);
  mrb_gv_set(mrb, mrb_intern_lit(mrb, "$\""), loaded_features);
  mrb_gv_set(mrb, mrb_intern_lit(mrb, "$LOADED_FEATURES"), loaded_features);

  mrb_define_method(mrb, mrb->kernel_module, "require_relative", mrb_kernel_require_relative, MRB_ARGS_REQ(1));
  mrb_define_singleton_method(mrb, mrb->top_self, "require_relative", mrb_toplevel_require_relative, MRB_ARGS_REQ(1));
}

#ifdef CONTROLS
mrb_value
mrb_require_controls_require_relative(mrb_state* mrb, mrb_value self) {
  mrb_value relative_path;
  mrb_value origin;

  mrb_get_args(mrb, "SS", &relative_path, &origin);

  return mrb_require_relative(mrb, relative_path, origin);
}

mrb_value
mrb_require_controls_loading_files(mrb_state* mrb, mrb_value self) {
  return mrb_require_loading_files(mrb);
}
#endif
