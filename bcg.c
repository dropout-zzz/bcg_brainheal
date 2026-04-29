#include <stdio.h>
#include <string.h>
#include <fcntl.h>
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

int
main(int argc, char *argv[])
{
  char *ext;
  AUTOCLO int fd;

  fd = -1;

  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
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
}
