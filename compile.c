#include <stdlib.h>
#include <stdio.h>
#include "compile.h"
#include "parse.h"

void
del_compiled_prog(struct program *cprog)
{
  free(cprog->funcs);
}

void
del_compiler_err(struct compiler_err *cerr)
{
  switch (cerr->kind)
  {
    case DUPLICATE_FUNCTION:
      free(cerr->duplicate_function);
      break;
    default:
      printf("Fixme: may not be properly freeing unhandled compiler err %d\n", cerr->kind);
      break;
  }
}
