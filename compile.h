struct func
{
  const char *exported_name;
};

struct program
{
  struct func *funcs;
};

enum
{
  DUPLICATE_FUNCTION,
};

struct duplicate_function
{
  const char *name;
  int pos;
};

struct compiler_err
{
  int kind;
  union
  {
    struct duplicate_function *duplicate_function;
  };
};

void
del_compiler_err (struct compiler_err *cerr);
