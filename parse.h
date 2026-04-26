enum
{
  EXPECT_IDENTIFIER = 1,
  EXPECT_BRINGIT,
  EXPECT_THATSIT,
  UNEXPECTED_TOKEN,
};

enum
{
  IMPL_FUNC,
};

struct impl_func
{
  const char *name;
};

struct program_node
{
  int kind;
  union
  {
    struct impl_func *impl_func;
  };
  struct program_node *next;
};
