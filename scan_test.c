#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "scan/keyword.h"
#include "scan/identifier.h"

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

#include "diagnosis.h"
#include "scan/token.h"

static void
test02(void)
{
  char test_code[] = TEST_CODE_01;
  char *orig;
  char *p;
#define TEST2_NTOK 10
  struct token buff[TEST2_NTOK + 1];
  int count;
  int err;
  struct token *tok;
  struct line_info li;

  orig = strdup(test_code);

  p = test_code;
  err = get_tokens(&p, buff, TEST2_NTOK, &count);

  printf("Token count: %d\n", count);

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
  {
    printf("Tokenizer error: %s\n", pretty_tokenizer_err(err));
    decode_line_info(orig, p - test_code, &li);
    print_full_line_info(&li);
  }
  else if (*p)
    printf("W: token buffer overran\n");

  del_tokens(buff, count);
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
