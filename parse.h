#include "lex.h"

enum node_type {
  ND_NUM,
  ND_ADD,
  ND_SUB,
  ND_MUL,
  ND_DIV
};

struct ast_node {
  enum node_type   type;
  double           value;
  struct ast_node *left;
  struct ast_node *right;
};

struct parser {
  struct lexer *lex;
  struct token  cur; /* for iterating through src */
};

struct parser   *parser_new(struct lexer *lex);
void             parser_free(struct parser *p);
struct ast_node *parser_run(struct parser *p);
void             ast_free(struct ast_node *node);
double           eval(const struct ast_node *node);
