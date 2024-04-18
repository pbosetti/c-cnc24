/*
  ____                                             _
 |  _ \ _ __ ___   __ _ _ __ __ _ _ __ ___     ___| | __ _ ___ ___
 | |_) | '__/ _ \ / _` | '__/ _` | '_ ` _ \   / __| |/ _` / __/ __|
 |  __/| | | (_) | (_| | | | (_| | | | | | | | (__| | (_| \__ \__ \
 |_|   |_|  \___/ \__, |_|  \__,_|_| |_| |_|  \___|_|\__,_|___/___/
                  |___/
*/

#include "program.h"

/*
  ____        __ _       _ _   _
 |  _ \  ___ / _(_)_ __ (_) |_(_) ___  _ __  ___
 | | | |/ _ \ |_| | '_ \| | __| |/ _ \| '_ \/ __|
 | |_| |  __/  _| | | | | | |_| | (_) | | | \__ \
 |____/ \___|_| |_|_| |_|_|\__|_|\___/|_| |_|___/

*/

typedef struct program {
  char *filename; // G-code file path
  block_t *first; // First block
  block_t *current;
  block_t *last;
  size_t n; // total number of G-code blocks
} program_t;

/*
  _____                 _   _
 |  ___|   _ _ __   ___| |_(_) ___  _ __  ___
 | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|
 |  _|| |_| | | | | (__| |_| | (_) | | | \__ \
 |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

*/

/* Lifecycle ******************************************************************/
program_t *program_new(char const *filename) {
  assert(filename);
  program_t *p = (program_t *)malloc(sizeof(program_t));
  if (!p) {
    eprintf("Could not allocate memory for program\n");
    return NULL;
  }
  if (strlen(filename) == 0) {
    eprintf("Invalid file name\n");
    free(p);
    return NULL;
  }
  memset(p, 0, sizeof(*p));
  p->filename = strdup(filename);
  return p;
}

void program_free(program_t *p) {
  assert(p);
  block_t *b, *tmp;
  b = p->first;
  while (b) {
    tmp = b;
    b = block_next(b);
    block_free(tmp);
  }
  free(p->filename);
  free(p);
  p = NULL;
}

void program_print(program_t *p, FILE *output) {
  assert(p && output);
  block_t *b = p->first;
  while (b) {
    block_print(b, output);
    b = block_next(b);
  }
}

/* Accessors ******************************************************************/
#define program_getter(typ, par, name)                                         \
  typ program_##name(program_t const *p) {                                     \
    assert(p);                                                                 \
    return p->par;                                                             \
  }

program_getter(char *, filename, filename);
program_getter(size_t, n, length);
program_getter(block_t *, first, first);
program_getter(block_t *, current, current);
program_getter(block_t *, last, last);

/* Methods ********************************************************************/
ccnc_error_t program_parse(program_t *p, machine_t const *m) {
  assert(p && m);
  FILE *file = NULL;
  ssize_t line_len = 0;
  size_t n = 0;
  char *line = NULL;
  block_t *b = NULL;

  file = fopen(p->filename, "r");
  if (!file) {
    eprintf("Cannot open the file at %s\n", p->filename);
    return FILE_ERR;
  }

  // Parsing loop
  while ((line_len = getline(&line, &n, file)) >= 0) {
    if (line[line_len - 1] == '\n') {
      line[line_len - 1] = '\0';
    }
    if (!(b = block_new(line, p->last, m))) {
      eprintf("Error creating a block from line %s\n", line);
      return PARSE_ERR;
    }
    if (p->first == NULL) {
      p->first = b;
    }
    p->last = b;
    p->n++;
  }
  fclose(file);
  free(line);
  program_reset(p);
  return NO_ERR;
}

void program_reset(program_t *p) {
  assert(p);
  p->current = NULL;
}

block_t *program_next(program_t *p) {
  assert(p);
  if (p->current == NULL) { // first block
    p->current = p->first;
  } else {
    p->current = block_next(p->current);
  }
  return p->current;
}




/*
  ____                                        _            _   
 |  _ \ _ __ ___   __ _ _ __ __ _ _ __ ___   | |_ ___  ___| |_ 
 | |_) | '__/ _ \ / _` | '__/ _` | '_ ` _ \  | __/ _ \/ __| __|
 |  __/| | | (_) | (_| | | | (_| | | | | | | | ||  __/\__ \ |_ 
 |_|   |_|  \___/ \__, |_|  \__,_|_| |_| |_|  \__\___||___/\__|
                  |___/                                        
*/

#ifdef PROGRAM_MAIN
int main(int argc, char const **argv) {
  machine_t *m = NULL;
  program_t *p = NULL;
  block_t *curr_b = NULL;
  point_t *pos = NULL;
  data_t t = 0, tt = 0, dt = 0, tq = 0;
  data_t lambda = 0, v = 0;

  if (argc != 3) {
    eprintf("I need exactly two arguments: G-code file, and INI file\n");
    exit(EXIT_FAILURE);
  }
  m = machine_new(argv[2]);
  if (!m) {
    eprintf("Error in INI file\n");
    exit(EXIT_FAILURE);
  }
  p = program_new(argv[1]);
  if (!p) {
    eprintf("Error creating a program\n");
    exit(EXIT_FAILURE);
  }
  if (program_parse(p, m) != NO_ERR) {
    eprintf("Error parsing the program\n");
    exit(EXIT_FAILURE);
  }
  program_print(p, stderr);

  tq = machine_tq(m);
  // run the program
  program_reset(p);
  printf("N t tt lambda s v X Y Z\n");
  while((curr_b = program_next(p))) {
    if (block_type(curr_b) == RAPID) {
      continue;
    }
    dt = block_dt(curr_b);
    for (t = 0; t - dt < tq / 10.0; t += tq, tt += tq) {
      pos = block_interpolate_t(curr_b, t, &lambda, &v);
      if (!pos) {
        continue;
      }
      printf("%03lu %.3f %.3f %.6f %.3f %.3f %.3f %.3f %.3f\n", block_n(curr_b), t, tt, lambda, lambda * block_length(curr_b), v, point_x(pos), point_y(pos), point_z(pos));
    }
  }

  program_free(p);
  machine_free(m);
  return 0;
}



#endif // PROGRAM_MAIN