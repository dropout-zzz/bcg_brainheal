#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "token.h"
#include "diagnosis.h"
#include "parse.h"

struct program_node *
get_program_tree(struct token **, int *);

const char *
pretty_parser_err(int);

void
del_program_tree(struct program_node *);

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
  int pos;
  struct token *last_tok;

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

    if (tok->kind || tok == buff)
    {
      pos = tok->pos;
    }
    else
    {
      last_tok = tok - 1;
      pos = last_tok->pos + last_tok->len;
    }

    decode_line_info(orig, pos, &li);
    print_full_line_info(&li);
  }

  del_program_tree(tree);

out:
  del_tokens(buff, NTOK);
  free(orig);
}
