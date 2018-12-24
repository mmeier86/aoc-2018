/**
 * @file tokenizer.h
 * @brief A string tokenizer utility.
 */

struct tokintern;

typedef struct tokintern tok_t;

tok_t* get_tokenizer(char* s, char* delim);
char* n_tok(tok_t* tok);
void free_tok(tok_t* tok);
void reset_tok(tok_t* tok);
