#include <stdio.h>
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
  char *p;
#define TEST2_NTOK 10
  struct token buff[TEST2_NTOK + 1];
  int err;
  struct token *tok;

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

  if (err)
    printf("Tokenizer error: %s\n", pretty_tokenizer_err(err));
  else if (*p)
    printf("W: token buffer overran\n");

  del_tokens(buff, TEST2_NTOK);
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
