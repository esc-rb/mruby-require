#include <mruby.h>
#include <mruby/string.h>

#include <errno.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "expand_path.h"
#include "load_error.h"

#define DEFAULT_ALLOCATION_SIZE_BYTES -1

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

static mrb_int
join_home_directory_raw(mrb_state* mrb, mrb_value output_str, const char* user, size_t user_length, mrb_int allocation_size_bytes) {
  mrb_assert(strnlen(user, user_length) == user_length);

  if(allocation_size_bytes == DEFAULT_ALLOCATION_SIZE_BYTES) {
    allocation_size_bytes = PATH_MAX;
  }

  struct passwd pwd;
  struct passwd* pwd_ptr;

  mrb_int allocations = 1;
  char* getpwnam_r_buf = mrb_malloc(mrb, allocation_size_bytes);

  char* user_final_chr = (char*)&user[user_length];
  const char previous_final_chr = *user_final_chr;

  if(previous_final_chr != '\0') {
    *user_final_chr = '\0';
  }

  while(getpwnam_r(user, &pwd, getpwnam_r_buf, allocation_size_bytes * allocations, &pwd_ptr) > 0) {
    /* An allocation size of zero is used to test handling failures from getpwnam_r(3). */
    if(errno != ERANGE || allocation_size_bytes == 0) {
      break;
    }

    allocations += 1;

    getpwnam_r_buf = mrb_realloc(mrb, getpwnam_r_buf, allocations * allocation_size_bytes);
  }

  if(previous_final_chr != '\0') {
    *user_final_chr = previous_final_chr;
  }

  if(pwd_ptr == NULL) {
    mrb_value message_str = mrb_str_new_lit(mrb, "cannot get home directory -- ");

    mrb_str_cat(mrb, message_str, user, user_length);

    if(errno) {
      mrb_str_cat_lit(mrb, message_str, " (");
      mrb_str_cat_cstr(mrb, message_str, strerror(errno));
      mrb_str_cat_lit(mrb, message_str, ")");
    }

    mrb_raise_load_error_message(mrb, message_str);
  }

  const char* home_directory = pwd_ptr->pw_dir;
  size_t home_directory_length = strnlen(home_directory, allocations * allocation_size_bytes);

  mrb_str_cat(mrb, output_str, home_directory, home_directory_length);

  return allocations;
}

static inline mrb_int
join_home_directory(mrb_state* mrb, mrb_value output_str, const char* user, size_t user_length) {
  return join_home_directory_raw(mrb, output_str, user, user_length, DEFAULT_ALLOCATION_SIZE_BYTES);
}

static mrb_int
join_current_home_directory_raw(mrb_state* mrb, mrb_value output_str, const char* env_var, mrb_int allocation_size_bytes) {
  if(env_var == NULL) {
    env_var = "HOME";
  }

  const char* home_directory = getenv(env_var);
  if(home_directory != NULL) {
    size_t home_directory_length = strlen(home_directory);
    mrb_str_cat(mrb, output_str, home_directory, home_directory_length);
    return 0;
  }

  if(allocation_size_bytes == DEFAULT_ALLOCATION_SIZE_BYTES) {
    allocation_size_bytes = PATH_MAX;
  }

  mrb_int allocations = 1;
  char* username = mrb_malloc(mrb, allocation_size_bytes);

  while(getlogin_r(username, allocations * allocation_size_bytes) != 0) {
    /* An allocation size of zero is used to test handling failures from getlogin_r(3). */
    if(errno != ERANGE || allocation_size_bytes == 0) {
      mrb_value message_str = mrb_str_new_lit(mrb, "cannot get home directory");

      mrb_str_cat_lit(mrb, message_str, " (");
      mrb_str_cat_cstr(mrb, message_str, strerror(errno));
      mrb_str_cat_lit(mrb, message_str, ")");

      mrb_raise_load_error_message(mrb, message_str);
    }

    allocations += 1;

    username = mrb_realloc(mrb, username, allocations * allocation_size_bytes);
  }

  size_t username_length = strnlen(username, allocations * allocation_size_bytes);

  allocations += join_home_directory_raw(mrb, output_str, username, username_length, allocation_size_bytes);

  return allocations;
}

static inline mrb_int
join_current_home_directory(mrb_state* mrb, mrb_value output_str) {
  return join_current_home_directory_raw(mrb, output_str, NULL, DEFAULT_ALLOCATION_SIZE_BYTES);
}

static mrb_int
join_current_directory_raw(mrb_state* mrb, mrb_value output_str, mrb_int allocation_size_bytes) {
  if(allocation_size_bytes == DEFAULT_ALLOCATION_SIZE_BYTES) {
    allocation_size_bytes = PATH_MAX;
  }

  mrb_int allocations = 1;

  /* An allocation size of zero is used to test handling failures from getcwd(3).
   * mrb_malloc would return a NULL pointer if a zero sized region were requested,
   * which would cause getcwd(3) to fall back to its own unsafe internal allocation
   * strategy. Therefore, if allocation size is zero, 1 is supplied to mrb_malloc. */
  char* home_directory = mrb_malloc(mrb, allocation_size_bytes == 0 ? 1 : allocation_size_bytes);

  while(getcwd(home_directory, allocations * allocation_size_bytes) == NULL) {
    if(errno != ERANGE) {
      mrb_value message_str = mrb_str_new_lit(mrb, "cannot get current directory");

      mrb_str_cat_lit(mrb, message_str, " (");
      mrb_str_cat_cstr(mrb, message_str, strerror(errno));
      mrb_str_cat_lit(mrb, message_str, ")");

      mrb_raise_load_error_message(mrb, message_str);
    }

    allocations += 1;

    home_directory = mrb_realloc(mrb, home_directory, allocations * allocation_size_bytes);
  }

  size_t home_directory_length = strnlen(home_directory, allocations * allocation_size_bytes);

  mrb_str_cat(mrb, output_str, home_directory, home_directory_length);

  return allocations;
}

static inline mrb_int
join_current_directory(mrb_state* mrb, mrb_value output_str) {
  return join_current_directory_raw(mrb, output_str, DEFAULT_ALLOCATION_SIZE_BYTES);
}

static void
initial_path(mrb_state* mrb, mrb_value output_str, const char* path, size_t path_length) {
  mrb_int path_index;

  for(path_index = 0; path_index < path_length; path_index++) {
    if(path[path_index] != DIR_SEPARATOR) {
      break;
    }
  }

  /* Path starts with two leading directory separators, e.g. `//' */
  if(path_index == 2) {
    mrb_str_cat(mrb, output_str, path, 2);

  /* Path starts with either just one, or else three or more, directory separators, e.g. `/' or `///' */
  } else if(path_index > 0) {
    mrb_str_cat(mrb, output_str, path, 1);

  /* Home directory reference */
  } else if(path[0] == '~') {
    mrb_int path_index = 1;

    for(; path_index <= path_length; path_index++) {
      if(path[path_index] == DIR_SEPARATOR || path[path_index] == '\0') {
        break;
      }
    }

    if(path_index > 1) {
      join_home_directory(mrb, output_str, &path[1], path_index - 1);
    } else {
      join_current_home_directory(mrb, output_str);
    }

    path += path_index;
    path_length -= path_index;

  /* Relative path */
  } else {
    join_current_directory(mrb, output_str);
  }

  join_path(mrb, output_str, path, path_length);
}

mrb_value
mrb_require_expand_relative_path(mrb_state* mrb, mrb_value path_str) {
  const char* path = RSTRING_CSTR(mrb, path_str);
  size_t path_length = RSTRING_LEN(path_str);

  mrb_value output_str = mrb_str_new_capa(mrb, path_length);

  join_path(mrb, output_str, path, path_length);

  return output_str;
}

mrb_value
mrb_require_expand_path(mrb_state* mrb, mrb_value path_str, mrb_value origin_str) {
  const char* path = RSTRING_CSTR(mrb, path_str);
  size_t path_length = RSTRING_LEN(path_str);

  size_t origin_length;

  if(mrb_nil_p(origin_str)) {
    origin_length = 0;
  } else {
    origin_length = RSTRING_LEN(origin_str);
  }

  mrb_value output_str = mrb_str_new_capa(mrb, path_length + origin_length);

  mrb_bool relative_path;
  if(path_length > 0 && (path[0] == DIR_SEPARATOR || path[0] == '~')) {
    relative_path = FALSE;
  } else {
    relative_path = TRUE;
  }

  if(relative_path == TRUE && origin_length > 0) {
    const char* origin = RSTRING_CSTR(mrb, origin_str);
    initial_path(mrb, output_str, origin, origin_length);

    join_path(mrb, output_str, path, path_length);
  } else {
    initial_path(mrb, output_str, path, path_length);
  }

  return output_str;
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

mrb_value
mrb_require_controls_expand_path_initial(mrb_state* mrb, mrb_value self) {
  const char* path;
  size_t path_length;

  mrb_get_args(mrb, "s", &path, &path_length);

  mrb_value output_str = mrb_str_new_capa(mrb, PATH_MAX);

  initial_path(mrb, output_str, path, path_length);

  return output_str;
}

mrb_value
mrb_require_controls_expand_path_home_directory(mrb_state* mrb, mrb_value self) {
  mrb_value output_str;
  const char* user;
  size_t user_length;
  mrb_int allocation_size_bytes;
  mrb_bool allocation_size_bytes_given;

  mrb_get_args(mrb, "Ss|i?", &output_str, &user, &user_length, &allocation_size_bytes, &allocation_size_bytes_given);

  mrb_int allocations;

  if(allocation_size_bytes_given == FALSE) {
    allocations = join_home_directory(mrb, output_str, user, user_length);
  } else {
    allocations = join_home_directory_raw(mrb, output_str, user, user_length, allocation_size_bytes);
  }

  return mrb_fixnum_value(allocations);
}

mrb_value
mrb_require_controls_expand_path_current_home_directory(mrb_state* mrb, mrb_value self) {
  mrb_value output_str;
  const char* env_var;
  mrb_bool env_var_given;
  mrb_int allocation_size_bytes = -1;

  mrb_get_args(mrb, "S|z?i", &output_str, &env_var, &env_var_given, &allocation_size_bytes);

  mrb_int allocations;

  if(env_var_given == FALSE) {
    allocations = join_current_home_directory(mrb, output_str);
  } else {
    allocations = join_current_home_directory_raw(mrb, output_str, env_var, allocation_size_bytes);
  }

  return mrb_fixnum_value(allocations);
}

mrb_value
mrb_require_controls_expand_path_current_directory(mrb_state* mrb, mrb_value self) {
  mrb_value output_str;
  mrb_int allocation_size_bytes;
  mrb_bool allocation_size_bytes_given;

  mrb_get_args(mrb, "S|i?", &output_str, &allocation_size_bytes, &allocation_size_bytes_given);

  mrb_int allocations;

  if(allocation_size_bytes_given == FALSE) {
    allocations = join_current_directory(mrb, output_str);
  } else {
    allocations = join_current_directory_raw(mrb, output_str, allocation_size_bytes);
  }

  return mrb_fixnum_value(allocations);
}

mrb_value
mrb_require_controls_expand_path_relative(mrb_state* mrb, mrb_value self) {
  mrb_value path;

  mrb_get_args(mrb, "S", &path);

  return mrb_require_expand_relative_path(mrb, path);
}

mrb_value
mrb_require_controls_expand_path(mrb_state* mrb, mrb_value self) {
  mrb_value path;
  mrb_value origin = mrb_nil_value();

  mrb_get_args(mrb, "S|S!", &path, &origin);

  return mrb_require_expand_path(mrb, path, origin);
}
#endif /* CONTROLS */
