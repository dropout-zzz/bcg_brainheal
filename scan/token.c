#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "token.h"
#include "gap.h"

#include "keyword.h"
#include "identifier.h"

#define EXPORT __attribute__((__visibility__("default")))

EXPORT int
get_tokens(char **cursor, struct token *arr, int n)
{
  int err;

  char *s;
  char *p;
  char tmp;

  const char *raw_identifier;

  char *old_p;
  const char *raw_keyword;
  int keyword_kind;

  err = NO_ERROR;

  for (p = s = *cursor; n; n--)
  {
    while (is_gap(tmp = *p))
      p++;

    if (!tmp)
      break;

    old_p = p;

    // dispatchable

    raw_identifier = get_raw_identifier(&p);
    if (raw_identifier)
    {
      arr->kind = IDENTIFIER;
      arr->identifier = malloc(sizeof(struct identifier));
      arr->identifier->s = raw_identifier;
      arr->pos = old_p - s;
      arr->len = p - old_p;
      arr++;
      continue;
    }

    // try everything

    raw_keyword = get_raw_keyword(&p);
    if (raw_keyword)
    {
      if (!strcmp("impl", raw_keyword))
        keyword_kind = IMPL;
      else if (!strcmp("bringit", raw_keyword))
        keyword_kind = BRINGIT;
      else if (!strcmp("thatsit", raw_keyword))
        keyword_kind = THATSIT;
      else
      {
        p = old_p;
        goto after_kw;
      }

      arr->kind = KEYWORD;
      arr->keyword = malloc(sizeof(struct keyword));
      arr->keyword->kind = keyword_kind;
      arr->pos = old_p - s;
      arr->len = p - old_p - 1;
      arr++;
      continue;
    }

after_kw:

    err = SYNTAX_ERROR;
    break;
  }

  *cursor = p;
  arr->kind = 0;
  return err;
}

EXPORT void
del_tokens(struct token *arr, int n)
{
  for (; n--; arr++)
  {
    switch (arr->kind)
    {
      case 0:
        return;
      case KEYWORD:
        free(arr->keyword);
        break;
      case IDENTIFIER:
        free(arr->identifier);
        break;
      default:
        printf("Fixme: may not be properly freeing unhandled token %d\n", arr->kind);
        break;
    }
  }
}

EXPORT const char *
pretty_tokenizer_err(int err)
{
  switch (err)
  {
    case NO_ERROR:
      return "No error";
    case SYNTAX_ERROR:
      return "Syntax error";
    default:
      return "Unknown error";
  }
}
