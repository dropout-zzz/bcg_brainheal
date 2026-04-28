#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include "diagnosis.h"
#include "parse.h"
#include "compile.h"

int
main(int argc, char *argv[])
{
  char *ext;

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
}
