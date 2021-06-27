#include <mruby.h>
#include <mruby/string.h>

#include <string.h>

#include "expand_path.h"
#include "load_error.h"

#define DIR_SEPARATOR '/'
#define DIR_SEPARATOR_STR "/"

static void
join_segment(mrb_state* mrb, mrb_value output_str, const char* segment, size_t segment_length) {
  const char* output = RSTRING_CSTR(mrb, output_str);
  size_t output_length = RSTRING_LEN(output_str);

  mrb_bool root_path = FALSE;
  if(output_length > 0 && output[0] == DIR_SEPARATOR) {
    if(output_length == 1) {
      root_path = TRUE;
    } else if(output_length == 2) {
      root_path = output[1] == DIR_SEPARATOR;
    }
  }

  if(strncmp(segment, ".", segment_length) == 0) {
    return;

  } else if(strncmp(segment, "..", segment_length) == 0) {
    if(root_path == TRUE) {
      return;
    }

    if(output_length == 0) {
      mrb_str_cat(mrb, output_str, segment, segment_length);
      return;
    }

    for(mrb_int output_index = output_length; output_index >= 0; output_index--) {
      if(output[output_index] == DIR_SEPARATOR) {
        size_t new_size = output_index;

        /* Preserve the root directory separator(s) */
        if(output_index == 0 || (output_index == 1 && output[0] == DIR_SEPARATOR)) {
          new_size++;
        }

        mrb_str_resize(mrb, output_str, new_size);
        return;
      }
    }

    mrb_str_resize(mrb, output_str, 0);

  } else {
    if(!root_path && output_length > 0) {
      mrb_str_cat_lit(mrb, output_str, DIR_SEPARATOR_STR);
    }
    mrb_str_cat(mrb, output_str, segment, segment_length);
  }
}

static void
join_path(mrb_state* mrb, mrb_value output_str, const char* path, size_t path_length) {
  const char* segment = path;

  char previous_chr = '\0';

  for(mrb_int path_index = 0; path_index <= path_length; path_index++) {
    const char chr = path[path_index];

    if(chr == DIR_SEPARATOR || chr == '\0') {
      if(previous_chr != DIR_SEPARATOR) {
        join_segment(mrb, output_str, segment, &path[path_index] - segment);

        segment = NULL;
      }
    } else if(segment == NULL) {
      segment = &path[path_index];
    }

    previous_chr = path[path_index];
  }
}

#ifdef CONTROLS
mrb_value
mrb_require_controls_expand_path_join_segment(mrb_state* mrb, mrb_value self) {
  const char* segment;
  size_t segment_length;
  mrb_value output_str = mrb_nil_value();

  mrb_get_args(mrb, "s|S", &segment, &segment_length, &output_str);

  if(mrb_nil_p(output_str)) {
    output_str = mrb_str_new(mrb, NULL, 0);
  }

  join_segment(mrb, output_str, segment, segment_length);

  return output_str;
}

mrb_value
mrb_require_controls_expand_path_join_path(mrb_state* mrb, mrb_value self) {
  mrb_value output_str;
  const char* path;
  size_t path_length;

  mrb_get_args(mrb, "Ss", &output_str, &path, &path_length);

  join_path(mrb, output_str, path, path_length);

  return output_str;
}
#endif /* CONTROLS */
