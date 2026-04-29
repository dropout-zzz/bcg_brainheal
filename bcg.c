#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
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

int
main(int argc, char *argv[])
{
  long ps;
  char *ext;
  AUTOCLO int fd;
  struct stat file;
  int real_size;
  int map_size;
  void *view;
  AUTOUNMAP struct map_info mi;

  fd = -1;
  mi.view = MAP_FAILED;

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
  view = mmap(0, map_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

  if (view == MAP_FAILED)
  {
    perror("cant map input file");
    return 1;
  }

  mi.size = map_size;
  mi.view = view;

  if (real_size % ps == 0)
  {
    if (mmap(view + real_size, 1, PROT_READ, MAP_ANON | MAP_PRIVATE | MAP_FIXED, -1, 0) == MAP_FAILED)
    {
      perror("cant ensure padding after input file");
      return 1;
    }
  }
}
