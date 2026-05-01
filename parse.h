struct token;

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
  struct token *name_tok;
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

struct program_node *
get_program_tree(struct token **, int *);

const char *
pretty_parser_err(int);

void
del_program_tree(struct program_node *);
