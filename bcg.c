#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include "diagnosis.h"
#include "parse.h"
#include "compile.h"

#define AUTOCLO __attribute__((__cleanup__(fd_exit_)))

static void
fd_exit_(int *ptr)
{
  close(*ptr);
}

struct map_info
{
  int size;
  void *view;
};

#define AUTOUNMAP __attribute__((__cleanup__(map_exit_)))

static void
map_exit_(struct map_info *mi)
{
  void *base;

  base = mi->view;
  if (base == MAP_FAILED)
    return;

  munmap(base, mi->size);
}

#define AUTOFREE __attribute__((__cleanup__(ptr_exit_)))

static void
ptr_exit_(void *ptr)
{
  free(*(void **)ptr);
}

struct tok_arr
{
  struct token *a;
  int n;
};

#define AUTO_DEL_TOKARR __attribute__((__cleanup__(tok_arr_exit_)))

static void
tok_arr_exit_(struct tok_arr *ta)
{
  del_tokens(ta->a, ta->n);
}

#define AUTO_DEL_PROG __attribute__((__cleanup__(prog_tree_exit_)))

static void
prog_tree_exit_(struct program_node **ptr)
{
  del_program_tree(*ptr);
}

int
main(int argc, char *argv[])
{
  long ps;
  char *ext;
  AUTOCLO int fd;
  struct stat file;
  int real_size;
  int map_size;
  AUTOFREE struct token *buff;
  void *view;
  AUTOUNMAP struct map_info mi;
  int need_pad;
  char *p;
  int count;
  int err;
  void *tmp;
  AUTO_DEL_TOKARR struct tok_arr ta;
  int pos;
  void *orig;
  struct line_info li;
  struct token *tok;
  AUTO_DEL_PROG struct program_node *tree;

  fd = -1;
  buff = 0;
  mi.view = MAP_FAILED;
  ta.n = 0;
  tree = 0;

  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
    return 1;
  }

  ps = sysconf(_SC_PAGE_SIZE);
  if (ps < 0)
  {
    perror("cant get system page size");
    return 1;
  }

  ext = strrchr(argv[1], '.');
  if (!ext)
  {
    fprintf(stderr, "input path did not include a dot\n");
    return 1;
  }

  ext++;
  if (strcmp(ext, "bg"))
  {
    fprintf(stderr, "input file name did not end with `.bg'\n");
    return 1;
  }

  fd = open(argv[1], O_RDONLY);
  if (fd < 0)
  {
    perror("cant open input file");
    return 1;
  }

  if (fstat(fd, &file) < 0)
  {
    perror("cant query size of input file");
    return 1;
  }

  real_size = file.st_size;
  map_size = real_size + 1;

  buff = reallocarray(0, map_size, sizeof(struct token));
  if (!buff)
  {
    perror("cant allocate token array");
    return 1;
  }

  view = mmap(0, map_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  if (view == MAP_FAILED)
  {
    perror("cant map input file");
    return 1;
  }

  mi.size = map_size;
  mi.view = view;
  need_pad = real_size % ps == 0;

  if (need_pad
      && mmap(view + real_size, 1, PROT_READ, MAP_ANON | MAP_PRIVATE | MAP_FIXED, -1, 0)
         == MAP_FAILED)
  {
    perror("cant ensure padding after input file");
    return 1;
  }

  p = view;
  err = get_tokens(&p, buff, real_size, &count);
  tmp = reallocarray(buff, count + 1, sizeof(struct token));

  if (tmp)
    buff = tmp;
  else
    perror("cant shrink token array");

  ta.a = buff;
  ta.n = count;

  if (err)
  {
    fprintf(stderr, "tokenize error: %s\n", pretty_tokenizer_err(err));
    pos = p - (char *)view;

print_line_and_quit:
    orig = mmap(0, map_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (orig == MAP_FAILED)
    {
      perror("cant map input file again for diagnosis");
    }
    else
    {
      if (need_pad
          && mmap(orig + real_size, 1, PROT_READ, MAP_ANON | MAP_PRIVATE | MAP_FIXED, -1, 0)
             == MAP_FAILED)
      {
        perror("cant ensure padding after input file again for diagnosis");
      }
      else
      {
        decode_line_info(orig, pos, &li);
        print_full_line_info(&li);
      }

      munmap(orig, map_size);
    }

    return 1;
  }
  else if (*p != '\0')
  {
    fprintf(stderr, "token array overflow. this is a bug.\n");
    return 1;
  }

  tok = buff;
  tree = get_program_tree(&tok, &err);

  if (err)
  {
    fprintf(stderr, "parse error: %s\n", pretty_parser_err(err));
    pos = get_pos_safe(tok, buff);
    goto print_line_and_quit;
  }
}
