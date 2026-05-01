struct token;

struct line_info
{
  const char *line;
  int line_no;
  int nth;
};

void
decode_line_info(char *s, int n, struct line_info *li);

void
print_full_line_info(struct line_info *li);

int
get_pos_safe(struct token *, struct token *);
