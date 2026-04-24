#include <stdio.h>

char *
get_raw_keyword(char **);

char *
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
main()
{
  test01();
}
