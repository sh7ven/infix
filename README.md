# Infix calculator
Recursive-descent LL(1) parser for simple infix math. Uses ASTs to build expression nodes.

# Usage
```c
#include "parse.h"

/* create lexer from expression */
struct lexer* lex = lexer_new("4 + 5");
struct parser*  p = parser_new(lex);

/* parse the expression */
struct ast_node* n = parser_run(p);

/* get the result */
double result = eval(n);
```


# Known limitations
- Ideally `strtod()` does the work and does the overflow/underflow checks. I will either add the checks myself or clean out to use `strtod()`

- Token structs waste memory, I should probably use a union to fix this. But eh, I'm lazy

- AST nodes are allocated manually (ex: `malloc(sizeof* node)`). This would in theory cause fragmentation

- There could be concerns with recursion depth with an expression like `((((((((1))))))))`, but for simple infix expressions it is not catastrophic
