#include <stdio.h>
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
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
    return 1;
  }
}
