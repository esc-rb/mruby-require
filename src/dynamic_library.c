#include <mruby.h>
#include <mruby/data.h>
#include <mruby/error.h>
#include <mruby/numeric.h>
#include <mruby/string.h>

#include <dlfcn.h>
#include <string.h>
#include <sys/stat.h>

#include "dynamic_library.h"
#include "file_extension.h"
#include "load_error.h"

#define gem_function_prefix "GENERATED_TMP_mrb_"
#define gem_init_function_suffix "_gem_init"
#define gem_final_function_suffix "_gem_final"

typedef void (*mrbgem_init_function_ptr)(mrb_state*);
typedef void (*mrbgem_final_function_ptr)(mrb_state*);

static inline mrb_bool
loaded_p(DynamicLibrary dynamic_library) {
  if(dynamic_library->resource == NULL) {
    return FALSE;
  } else {
    return TRUE;
  }
}

static void
mrb_dynamic_library_free(mrb_state* mrb, void* ptr) {
  DynamicLibrary_mut dynamic_library = ptr;

  mrb_gc_unregister(mrb, dynamic_library->path);

  mrb_free(mrb, dynamic_library);
}

static const struct mrb_data_type mrb_dynamic_library_type = { NULL, mrb_dynamic_library_free };

static DynamicLibrary_mut
allocate_dynamic_library(mrb_state* mrb, mrb_value path_str) {
  const char* path = RSTRING_CSTR(mrb, path_str);

  size_t path_length = RSTRING_LEN(path_str);
  const char* path_extension = get_file_extension_cstr(mrb, path, path_length);

  mrb_int path_index;
  for(path_index = path_extension - path; path_index >= 0; path_index--) {
    if(path[path_index] == '/' || path[path_index] == '\\') {
      path_index++;
      break;
    }
  }

  size_t name_segment_length = path_extension - &path[path_index];

  DynamicLibrary_mut dynamic_library = mrb_malloc(mrb, sizeof(struct DynamicLibrary) + name_segment_length + 1);

  dynamic_library->path = mrb_str_dup(mrb, path_str);
  mrb_gc_register(mrb, path_str);

  dynamic_library->name_segment_length = name_segment_length;
  dynamic_library->resource = NULL;

  for(mrb_int name_segment_index = 0; name_segment_index < name_segment_length; name_segment_index++) {
    char chr = path[path_index + name_segment_index];

    if(chr == '-') {
      chr = '_';
    }

    dynamic_library->name_segment[name_segment_index] = chr;
  }
  dynamic_library->name_segment[name_segment_length] = '\0';

  return dynamic_library;
}

static inline DynamicLibrary_mut
dynamic_library_get_mut_ptr(mrb_state* mrb, mrb_value dynamic_library_obj) {
  DynamicLibrary_mut dynamic_library = mrb_data_get_ptr(mrb, dynamic_library_obj, &mrb_dynamic_library_type);

  return dynamic_library;
}

static inline DynamicLibrary
dynamic_library_get_ptr(mrb_state* mrb, mrb_value dynamic_library_obj) {
  return dynamic_library_get_mut_ptr(mrb, dynamic_library_obj);
}

static mrb_value
gem_init_function_name(mrb_state* mrb, DynamicLibrary dynamic_library) {
  size_t capacity = sizeof(gem_function_prefix) + dynamic_library->name_segment_length + sizeof(gem_init_function_suffix);

  mrb_value str = mrb_str_new_capa(mrb, capacity);
  mrb_str_cat_lit(mrb, str, gem_function_prefix);
  mrb_str_cat(mrb, str, dynamic_library->name_segment, dynamic_library->name_segment_length);
  mrb_str_cat_lit(mrb, str, gem_init_function_suffix);

  return str;
}

static mrb_value
gem_final_function_name(mrb_state* mrb, DynamicLibrary dynamic_library) {
  size_t capacity = sizeof(gem_function_prefix) + dynamic_library->name_segment_length + sizeof(gem_final_function_suffix);

  mrb_value str = mrb_str_new_capa(mrb, capacity);
  mrb_str_cat_lit(mrb, str, gem_function_prefix);
  mrb_str_cat(mrb, str, dynamic_library->name_segment, dynamic_library->name_segment_length);
  mrb_str_cat_lit(mrb, str, gem_final_function_suffix);

  return str;
}

static mrb_value
call_gem_init_function(mrb_state* mrb, void* gem_init_function_ptr) {
  mrbgem_init_function_ptr gem_init_function = (mrbgem_init_function_ptr) gem_init_function_ptr;
  gem_init_function(mrb);
  return mrb_nil_value();
}

static mrb_bool
load_dynamic_library(mrb_state* mrb, DynamicLibrary_mut dynamic_library) {
  const char* path = RSTRING_CSTR(mrb, dynamic_library->path);

  struct stat stat_buf;
  if(stat(path, &stat_buf) < 0) {
    return FALSE;
  }

  void* resource = dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
  if(resource == NULL) {
    mrb_raise_load_error_reason(mrb, dynamic_library->path, dlerror());
  }
  dlerror();

  const char* init_function_name = RSTRING_CSTR(mrb, gem_init_function_name(mrb, dynamic_library));
  mrbgem_init_function_ptr gem_init_function = (mrbgem_init_function_ptr)dlsym(resource, init_function_name);

  if(gem_init_function == NULL) {
    dlclose(resource);

    mrb_value reason_str = mrb_str_new_lit(mrb, "missing gem init function: ");
    mrb_str_cat_cstr(mrb, reason_str, init_function_name);

    mrb_raise_load_error_reason(mrb, dynamic_library->path, RSTRING_CSTR(mrb, reason_str));
  }
  dlerror();

  mrb_bool error;
  mrb_value exception = mrb_protect_error(mrb, call_gem_init_function, gem_init_function, &error);

  if(error) {
    dlclose(resource);
    mrb_exc_raise(mrb, exception);
  }

  dynamic_library->resource = resource;

  return TRUE;
}

static mrb_value
call_gem_final_function(mrb_state* mrb, void* gem_final_function_ptr) {
  mrbgem_final_function_ptr gem_final_function = (mrbgem_final_function_ptr) gem_final_function_ptr;
  gem_final_function(mrb);
  return mrb_nil_value();
}

void
unload_dynamic_library(mrb_state* mrb, DynamicLibrary_mut dynamic_library) {
  const char* final_function_name = RSTRING_CSTR(mrb, gem_final_function_name(mrb, dynamic_library));
  mrbgem_final_function_ptr gem_final_function = (mrbgem_final_function_ptr)dlsym(dynamic_library->resource, final_function_name);

  mrb_assert(gem_final_function != NULL);

  void* resource = dynamic_library->resource;
  dynamic_library->resource = NULL;

  mrb_bool error = FALSE;
  mrb_value exception = mrb_protect_error(mrb, call_gem_final_function, gem_final_function, &error);

  dlclose(resource);

  if(error) {
    mrb_exc_raise(mrb, exception);
  }
}

#ifdef CONTROLS
mrb_value
mrb_require_controls_dynamic_library_initialize(mrb_state* mrb, mrb_value self) {
  mrb_value path;

  mrb_get_args(mrb, "S", &path);

  DynamicLibrary_mut dynamic_library = allocate_dynamic_library(mrb, path);

  mrb_data_init(self, dynamic_library, &mrb_dynamic_library_type);

  return self;
}

mrb_value
mrb_require_controls_dynamic_library_path(mrb_state* mrb, mrb_value self) {
  DynamicLibrary dynamic_library = dynamic_library_get_ptr(mrb, self);

  return dynamic_library->path;
}

mrb_value
mrb_require_controls_dynamic_library_init_function_name(mrb_state* mrb, mrb_value self) {
  DynamicLibrary dynamic_library = dynamic_library_get_ptr(mrb, self);

  return gem_init_function_name(mrb, dynamic_library);
}

mrb_value
mrb_require_controls_dynamic_library_final_function_name(mrb_state* mrb, mrb_value self) {
  DynamicLibrary dynamic_library = dynamic_library_get_ptr(mrb, self);

  return gem_final_function_name(mrb, dynamic_library);
}

mrb_value
mrb_require_controls_dynamic_library_inspect(mrb_state* mrb, mrb_value self) {
  DynamicLibrary dynamic_library = dynamic_library_get_ptr(mrb, self);

  mrb_value inspect_text = mrb_str_new_lit(mrb, "#<");
  mrb_str_cat_cstr(mrb, inspect_text, mrb_obj_classname(mrb, self));
  mrb_str_cat_lit(mrb, inspect_text, ":0x");
  mrb_str_cat_str(mrb, inspect_text, mrb_fixnum_to_str(mrb, mrb_int_value(mrb, mrb_obj_id(self)), 16));
  mrb_str_cat_lit(mrb, inspect_text, " path=\"");
  mrb_str_cat_str(mrb, inspect_text, dynamic_library->path);
  mrb_str_cat_lit(mrb, inspect_text, "\", init_function_name=\"");
  mrb_str_cat_str(mrb, inspect_text, gem_init_function_name(mrb, dynamic_library));
  mrb_str_cat_lit(mrb, inspect_text, "\", final_function_name=\"");
  mrb_str_cat_str(mrb, inspect_text, gem_final_function_name(mrb, dynamic_library));
  mrb_str_cat_lit(mrb, inspect_text, "\">");

  return inspect_text;
}

mrb_value
mrb_require_controls_dynamic_library_load(mrb_state* mrb, mrb_value self) {
  DynamicLibrary_mut dynamic_library = dynamic_library_get_mut_ptr(mrb, self);

  mrb_bool loaded = load_dynamic_library(mrb, dynamic_library);

  return mrb_bool_value(loaded);
}

mrb_value
mrb_require_controls_dynamic_library_unload(mrb_state* mrb, mrb_value self) {
  DynamicLibrary_mut dynamic_library = dynamic_library_get_mut_ptr(mrb, self);

  unload_dynamic_library(mrb, dynamic_library);

  return mrb_true_value();
}

mrb_value
mrb_require_controls_dynamic_library_loaded_p(mrb_state* mrb, mrb_value self) {
  DynamicLibrary dynamic_library = dynamic_library_get_ptr(mrb, self);

  mrb_bool loaded = loaded_p(dynamic_library);

  return mrb_bool_value(loaded);
}
#endif /* CONTROLS */
