#include <mruby.h>
#include <mruby/string.h>

static inline const char*
get_file_extension_cstr(mrb_state* mrb, const char* path, size_t path_length) {
  for(mrb_int path_index = path_length; path_index > 0; path_index--) {
    if(path[path_index] == '.') {
      if(path[path_index + 1] == '\0') {
        break;
      }

      return &path[path_index];

    } else if(path[path_index] == '/' || path[path_index] == '\\') {
      break;
    }
  }

  return NULL;
}

static inline const char*
get_file_extension(mrb_state* mrb, mrb_value path_str) {
  const char* path = RSTRING_CSTR(mrb, path_str);
  size_t path_length = RSTRING_LEN(path_str);

  return get_file_extension_cstr(mrb, path, path_length);
}
