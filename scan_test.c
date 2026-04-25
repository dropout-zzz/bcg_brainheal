#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "token.h"

const char *
get_raw_keyword(char **);

const char *
get_raw_identifier(char **);

#define TEST_CODE_01 "impl /main/ bringit\nthatsit\n"

static void
test01(void)
{
  char test_code[] = TEST_CODE_01;
  char *p;

  p = test_code;

  printf("first keyword: '%s'\n", get_raw_keyword(&p));
  printf("identifier: '%s'\n", get_raw_identifier(&p));
}

struct line_info
{
  const char *line;
  int line_no;
  int nth;
};

static void
decode_line_info(char *s, int n, struct line_info *li)
{
  int line_no;
  char *line;
  char *p;
  char c;

  line_no = 1;

  for (line = p = s; n; n--)
  {
    c = *p++;

    switch (c)
    {
      case '\0':
        goto stop_loop;
      case '\n':
        line_no++;
        line = p;
        break;
    }
  }

stop_loop:

  li->nth = p - line + 1;

  for (; (c = *p); p++)
  {
    if (c == '\n')
    {
      *p = '\0';
      break;
    }
  }

  li->line_no = line_no;
  li->line = line;
}

int
get_tokens(char **, struct token *, int);

const char *
pretty_tokenizer_err(int);

void
del_tokens(struct token *, int);

static void
test02(void)
{
  char test_code[] = TEST_CODE_01;
  char *orig;
  char *p;
#define TEST2_NTOK 10
  struct token buff[TEST2_NTOK + 1];
  int err;
  struct token *tok;
  struct line_info li;

  orig = strdup(test_code);

  p = test_code;
  err = get_tokens(&p, buff, TEST2_NTOK);

  for (tok = buff; tok->kind; tok++)
  {
    switch (tok->kind)
    {
      case KEYWORD:
        printf("Keyword ");
        switch (tok->keyword->kind)
        {
          case IMPL:
            printf("Impl\n");
            break;
          case BRINGIT:
            printf("Bringit\n");
            break;
          case THATSIT:
            printf("Thatsit\n");
            break;
          default:
            printf("<Fixme: %d not handled>\n", tok->keyword->kind);
            break;
        }
        break;
      case IDENTIFIER:
        printf("Identifier '%s'\n", tok->identifier->s);
        break;
      default:
        printf("Fixme: %d token not handled\n", tok->kind);
        break;
    }
  }

  printf("Token count: %lu\n", tok - buff);

  if (err)
  {
    printf("Tokenizer error: %s\n", pretty_tokenizer_err(err));
    decode_line_info(orig, p - test_code, &li);
    printf("At line #%d, char #%d:\n", li.line_no, li.nth);
    puts(li.line);
    printf("%*s^\n", li.nth - 1, "");
  }
  else if (*p)
    printf("W: token buffer overran\n");

  del_tokens(buff, TEST2_NTOK);
  free(orig);
}

int
main()
{
#define RUN_TEST(fn) \
  printf("--- " #fn " ---\n"); \
  fn()

  RUN_TEST(test01);
  RUN_TEST(test02);
}
