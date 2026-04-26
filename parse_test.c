#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "token.h"
#include "diagnosis.h"
#include "parse.h"

int
main()
{
  char test_code[] = {
    "impl /foo/ bringit\n"
    "thatsit\n"
    "\n"
    "impl /bar/ bringit\n"
    "thatsit\n"
  };
  char *orig;
  char *p;
#define NTOK 20
  struct token buff[NTOK + 1];
  int err;
  struct line_info li;

  orig = strdup(test_code);
  p = test_code;

  err = get_tokens(&p, buff, NTOK);
  if (err)
  {
    printf("tokenization error: %s.\n", pretty_tokenizer_err(err));
    decode_line_info(orig, p - test_code, &li);
    print_full_line_info(&li);
    goto out;
  }
  else if (*p != '\0')
  {
    printf("cannot fully tokenize. token buffer overran.\n");
    goto out;
  }

out:
  del_tokens(buff, NTOK);
  free(orig);
}
