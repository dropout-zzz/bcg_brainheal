#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "diagnosis.h"
#include "parse.h"
#include "scan/token.h"

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
  struct token *tok;
  struct program_node *tree;
  struct program_node *node;

  orig = strdup(test_code);
  p = test_code;

  err = get_tokens(&p, buff, NTOK, 0);
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

  tok = buff;
  tree = get_program_tree(&tok, &err);

  for (node = tree; node; node = node->next)
  {
    switch (node->kind)
    {
      case IMPL_FUNC:
        printf("FuncImpl '%s'\n", node->impl_func->name);
        break;
      default:
        printf("Fixme prog node %d not handled\n", node->kind);
        break;
    }
  }

  if (err)
  {
    printf("parser error: %s.\n", pretty_parser_err(err));
    decode_line_info(orig, get_pos_safe(tok, buff), &li);
    print_full_line_info(&li);
  }

  del_program_tree(tree);

out:
  del_tokens(buff, NTOK);
  free(orig);
}
