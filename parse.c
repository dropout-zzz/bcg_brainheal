#include <stdio.h>
#include <stdlib.h>
#include "parse.h"
#include "token.h"

static int
is_keyword_(struct token *tok, int what)
{
  if (tok->kind == KEYWORD
      && what == tok->keyword->kind)
    return 1;
  return 0;
}

static int
parse_func_impl_(struct token **cursor, struct impl_func *fi, int *err)
{
  struct token *p;

  p = *cursor;

  if (!is_keyword_(p, IMPL))
    return 0;

  p++;

  if (p->kind != IDENTIFIER)
  {
    *err = EXPECT_IDENTIFIER;
    goto out;
  }

  fi->name = p->identifier->s;

  p++;

  if (!is_keyword_(p, BRINGIT))
  {
    *err = EXPECT_BRINGIT;
    goto out;
  }

  p++;

  if (!is_keyword_(p, THATSIT))
  {
    *err = EXPECT_THATSIT;
    goto out;
  }

  p++;

  *err = 0;

out:
  *cursor = p;
  return 1;
}

const char *
pretty_parser_err(int err)
{
  switch (err)
  {
    case 0:
      return "Succeeded";
    case EXPECT_IDENTIFIER:
      return "Expects identifier";
    case EXPECT_BRINGIT:
      return "Expects \"bringit\"";
    case EXPECT_THATSIT:
      return "Expects \"thatsit\"";
    case UNEXPECTED_TOKEN:
      return "Unexpected token";
    default:
      return "Unknown";
  }
}

struct program_node *
get_program_tree(struct token **cursor, int *err)
{
  struct program_node top;
  struct program_node *node;
  struct token *p;

  struct impl_func fi;
  int e;

  node = &top;
  *err = 0;

  for (p = *cursor; p->kind != 0; )
  {
    if (parse_func_impl_(&p, &fi, &e))
    {
      if (e)
      {
        *err = e;
        break;
      }

      node = node->next = malloc(sizeof(struct program_node));
      node->kind = IMPL_FUNC;
      node->impl_func = malloc(sizeof(struct impl_func));
      node->impl_func->name = fi.name;
      continue;
    }

    *err = UNEXPECTED_TOKEN;
    break;
  }

  node->next = 0;
  *cursor = p;

  return top.next;
}

void
del_program_tree(struct program_node *tree)
{
  struct program_node *last;

  for (last = 0; tree; tree = tree->next)
  {
    switch (tree->kind)
    {
      case IMPL_FUNC:
        free(tree->impl_func);
        break;
      default:
        printf("Fixme: may not be freeing unhandled prog node %d proper\n", tree->kind);
        break;
    }

    free(last);
    last = tree;
  }

  free(last);
}
