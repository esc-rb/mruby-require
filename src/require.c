#include <mruby.h>
#include <mruby/array.h>
#include <mruby/variable.h>

#include <string.h>

#include "require.h"

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

static inline void
set_load_path(mrb_state* mrb, mrb_value load_path) {
  mrb_gv_set(mrb, mrb_intern_lit(mrb, "$:"), load_path);
  mrb_gv_set(mrb, mrb_intern_lit(mrb, "$LOAD_PATH"), load_path);
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
