#include "util.h"

enum token_type {
  TOK_NUM,
  TOK_ADD,
  TOK_SUB,
  TOK_MUL,
  TOK_DIV,
  TOK_LBRACK,
  TOK_RBRACK,
  TOK_END
};

struct token {
  enum token_type type;
  double          value;
};

struct lexer {
  const char* src;
  int         pos;
};

struct lexer* lexer_new(const char* src);
struct token  lexer_next(struct lexer* lex);
