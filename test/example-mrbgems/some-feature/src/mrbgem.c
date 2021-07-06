#include <mruby.h>
#include <mruby/variable.h>

void
mrb_some_feature_gem_init(mrb_state* mrb) {
  mrb_gv_set(mrb, mrb_intern_lit(mrb, "$some_feature_native_initialized"), mrb_true_value());
}

void
mrb_some_feature_gem_final(mrb_state* mrb) {
  mrb_gv_set(mrb, mrb_intern_lit(mrb, "$some_feature_native_finalized"), mrb_true_value());
}
