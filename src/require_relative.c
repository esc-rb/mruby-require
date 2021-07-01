#include <mruby.h>
#include <mruby/hash.h>
#include <mruby/variable.h>

#include "require_relative.h"
#include "expand_path.h"

static inline mrb_value
mrb_require_loading_files(mrb_state* mrb) {
  return mrb_gv_get(mrb, mrb_intern_lit(mrb, "REQUIRE_LOADING_FILES"));
}

static mrb_value
try_require(mrb_state* mrb, mrb_value path) {
  mrb_value loading_files = mrb_require_loading_files(mrb);

  if(mrb_hash_key_p(mrb, loading_files, path)) {
    return mrb_false_value();
  }

  return mrb_true_value();
}

mrb_value
mrb_require_relative(mrb_state* mrb, mrb_value relative_path, mrb_value origin) {
  mrb_value path = mrb_require_expand_path(mrb, relative_path, origin);

  return try_require(mrb, path);
}

void
mrb_require_relative_init(mrb_state* mrb) {
  mrb_value loading_files = mrb_hash_new(mrb);
  mrb_gv_set(mrb, mrb_intern_lit(mrb, "REQUIRE_LOADING_FILES"), loading_files);
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
