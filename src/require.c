#include <mruby.h>
#include <mruby/array.h>
#include <mruby/string.h>
#include <mruby/variable.h>

#include <string.h>

#include "require.h"
#include "expand_path.h"
#include "file_extension.h"
#include "load_error.h"
#include "require_relative.h"

#ifndef MRUBY_REQUIRE_DEFAULT_LOAD_PATHS
#define MRUBY_REQUIRE_DEFAULT_LOAD_PATHS ""
#endif /* MRUBY_REQUIRE_DEFAULT_LOAD_PATH */

static const char* const mruby_require_default_load_paths = MRUBY_REQUIRE_DEFAULT_LOAD_PATHS;

static mrb_value
parse_load_paths(mrb_state* mrb, const char* load_paths) {
  mrb_value load_paths_ary = mrb_ary_new(mrb);

  size_t load_paths_length = strlen(load_paths);

  if(load_paths_length == 0) {
    return load_paths_ary;
  }

  const char* load_path = load_paths;

  for(mrb_int load_paths_index = 0; load_paths_index <= load_paths_length; load_paths_index++) {
    char chr = load_paths[load_paths_index];

    if(chr == '\0' || chr == ',') {
      size_t load_path_length = load_paths_index - (load_path - load_paths);
      mrb_value load_path_str = mrb_str_new_frozen(mrb, load_path, load_path_length);
      mrb_ary_push(mrb, load_paths_ary, load_path_str);

      if(chr == ',') {
        load_path = &load_paths[load_paths_index + 1];
      }
    }
  }

  return load_paths_ary;
}

static mrb_value
get_default_load_paths(mrb_state* mrb) {
  return parse_load_paths(mrb, mruby_require_default_load_paths);
}

static inline mrb_value
get_load_path(mrb_state* mrb) {
  return mrb_gv_get(mrb, mrb_intern_lit(mrb, "$LOAD_PATH"));
}

static mrb_value
get_dynamic_library_str(mrb_state* mrb, mrb_value feature_str) {
  const char* feature = RSTRING_CSTR(mrb, feature_str);
  size_t feature_length = RSTRING_LEN(feature_str);

  mrb_value dynamic_library_str = mrb_str_new_capa(mrb, feature_length);

  for(mrb_int feature_index = 0; feature_index < feature_length; feature_index++) {
    char chr = feature[feature_index];

    if(chr == '/' || chr == '\\') {
      mrb_str_cat_lit(mrb, dynamic_library_str, "-");
    } else {
      mrb_str_cat(mrb, dynamic_library_str, &chr, 1);
    }
  }

  return dynamic_library_str;
}

static mrb_value
require(mrb_state* mrb, mrb_value feature_str) {
  const char* feature = RSTRING_CSTR(mrb, feature_str);
  if(strncmp(feature, "..", sizeof("..") - 1) == 0) {
    if(feature[2] == '/' || feature[2] == '\\') {
      mrb_raise_load_error(mrb, feature_str);
    }
  }

  mrb_value load_paths = get_load_path(mrb);

  for(mrb_int index = 0; index < RARRAY_LEN(load_paths); index++) {
    mrb_value load_path = mrb_ary_entry(load_paths, index);

    mrb_value result = mrb_require_relative(mrb, feature_str, load_path);

    if(mrb_nil_p(result)) {
      continue;
    }

    return result;
  }

  return mrb_nil_value();
}

mrb_bool
mrb_require(mrb_state* mrb, mrb_value path) {
  mrb_value feature = mrb_require_expand_relative_path(mrb, path);

  mrb_value result;

  if(get_file_extension(mrb, path) == NULL) {
    mrb_value dynamic_library_str = get_dynamic_library_str(mrb, feature);

    if(!mrb_str_equal(mrb, feature, dynamic_library_str)) {
      result = require(mrb, dynamic_library_str);

      if(!mrb_nil_p(result)) {
        return mrb_bool(result);
      }
    }
  }

  result = require(mrb, feature);

  if(mrb_nil_p(result)) {
    mrb_raise_load_error(mrb, path);
  }

  return mrb_bool(result);
}

static mrb_value
mrb_kernel_require(mrb_state* mrb, mrb_value self) {
  mrb_value feature;

  mrb_get_args(mrb, "S", &feature);

  mrb_bool required = mrb_require(mrb, feature);

  return mrb_bool_value(required);
}

static inline void
set_load_path(mrb_state* mrb, mrb_value load_path) {
  mrb_gv_set(mrb, mrb_intern_lit(mrb, "$:"), load_path);
  mrb_gv_set(mrb, mrb_intern_lit(mrb, "$LOAD_PATH"), load_path);

  mrb_define_method(mrb, mrb->kernel_module, "require", mrb_kernel_require, MRB_ARGS_REQ(1));
}

void
mrb_require_init(mrb_state* mrb) {
  set_load_path(mrb, get_default_load_paths(mrb));
}

#ifdef CONTROLS
mrb_value
mrb_require_controls_parse_load_paths(mrb_state* mrb, mrb_value self) {
  const char* load_paths;

  mrb_get_args(mrb, "z", &load_paths);

  return parse_load_paths(mrb, load_paths);
}
#endif /* CONTROLS */
