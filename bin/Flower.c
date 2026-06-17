#include <stdio.h>
#include <string.h>
typedef struct { char* data; int length; } flower_string;
static flower_string flower_string_from_cstr(char* s) {
  flower_string out;
  out.data = s;
  out.length = (int)strlen(s);
  return out;
}
static int flower_string_eq(flower_string a, flower_string b) {
  if (a.length != b.length) return 0;
  return !strncmp(a.data, b.data, a.length);
}
static void flower_print_string(flower_string s) {
  printf("%.*s", s.length, s.data);
}
;
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
char project_root[512];
char* current_file;


void mod_src_globals_flo_set_current_file(char* f) {
current_file = f;
}
char* RED = "\033[0;31m";
char* YELLOW = "\033[0;33m";
char* GREEN = "\033[0;32m";
char* RESET = "\033[0m";
char* BLUE = "\033[0;34m";
char* BOLD = "\033[1m";


char* mod_src_globals_flo_resolve_path(char* importing_file, char* import_path) {
if (import_path[0] == '<') {
return NULL;
}
char resolved[1024];
if (import_path[0] == '.'  &&  import_path[1] == '/'  ||  import_path[0] == '.'  &&  import_path[1] == '.'  &&  import_path[2] == '/') {
char dir[1024];
strncpy(dir, importing_file, sizeof(dir) - 1);
dir[sizeof(dir) - 1] = '\0';
char* last_slash = strrchr(dir, '/');
if (last_slash) {
last_slash = last_slash + 1;
*last_slash = '\0';
}
else {
strcpy(dir, "./");
}
if (strlen(dir) + strlen(import_path) + 1 > sizeof(resolved)) {
printf("%sPath too long: %s%s%s\n", RED, dir, import_path, RESET);
return NULL;
}
snprintf(resolved, sizeof(resolved), "%s%s", dir, import_path + 2);
}
else {
snprintf(resolved, sizeof(resolved), "%s/%s", project_root, import_path);
}
return strdup(resolved);
}
int TOKEN_INT = 1;
int TOKEN_FLOAT = 2;
int TOKEN_DOUBLE = 3;
int TOKEN_BOOL = 4;
int TOKEN_CHAR = 5;
int TOKEN_STRING = 6;
int TOKEN_STRUCT = 7;
int TOKEN_UNION = 8;
int TOKEN_NULL = 9;
int TOKEN_TILDE = 10;
int TOKEN_IDENTIFIER = 11;
int TOKEN_NUMBER = 12;
int TOKEN_FLOAT_LIT = 13;
int TOKEN_STRING_LIT = 14;
int TOKEN_CHAR_LIT = 15;
int TOKEN_VOID = 16;
int TOKEN_TRUE = 17;
int TOKEN_FALSE = 18;
int TOKEN_RETURN = 19;
int TOKEN_CONTINUE = 20;
int TOKEN_BREAK = 21;
int TOKEN_WHILE = 22;
int TOKEN_FUNC = 23;
int TOKEN_FOR = 24;
int TOKEN_IN = 25;
int TOKEN_END = 26;
int TOKEN_IF = 27;
int TOKEN_ELSE = 28;
int TOKEN_NOT = 29;
int TOKEN_AND = 30;
int TOKEN_OR = 31;
int TOKEN_IMPORT = 32;
int TOKEN_FORWARD = 33;
int TOKEN_AS = 34;
int TOKEN_PROP = 35;
int TOKEN_HIDDEN = 36;
int TOKEN_READONLY = 37;
int TOKEN_FROZEN = 38;
int TOKEN_NEW = 39;
int TOKEN_PRUNE = 40;
int TOKEN_SIZEOF = 41;
int TOKEN_PRINT = 42;
int TOKEN_DOT = 43;
int TOKEN_DOTDOT = 44;
int TOKEN_DOTDOTEQ = 45;
int TOKEN_PLUS = 46;
int TOKEN_MINUS = 47;
int TOKEN_STAR = 48;
int TOKEN_CARET = 49;
int TOKEN_SLASH = 50;
int TOKEN_AMPERSAND = 51;
int TOKEN_AT = 52;
int TOKEN_ASSIGN = 53;
int TOKEN_LT = 54;
int TOKEN_GT = 55;
int TOKEN_COMP = 56;
int TOKEN_NEQ = 57;
int TOKEN_GEQ = 58;
int TOKEN_LEQ = 59;
int TOKEN_LPAREN = 60;
int TOKEN_RPAREN = 61;
int TOKEN_LBRACK = 62;
int TOKEN_RBRACK = 63;
int TOKEN_LBRACE = 64;
int TOKEN_RBRACE = 65;
int TOKEN_COLON = 66;
int TOKEN_COMMA = 67;
int TOKEN_SEMI = 68;
int TOKEN_NEWLINE = 69;
int TOKEN_EOF = 70;


typedef struct Token {
int kind;
int start;
int length;
} Token;


typedef struct TokenStream {
Token* data;
int count;
int capacity;
} TokenStream;


int mod_include_lexer_h_flo_get_line(char* src, int pos) {
int line = 1;
for (int i = 0; ((0) > (pos)) ? i > (pos) : i < (pos); ((0) > (pos)) ? i-- : i++) {
if (src[i] == '\n') {
line = line + 1;
}
}
return line;
}


int mod_include_lexer_h_flo_get_col(char* src, int pos) {
int col = 1;
while (pos > 0  &&  src[pos - 1] != '\n') {
col = col + 1;
pos = pos - 1;
}
return col;
}


void mod_src_lexer_flo_init_token_stream(TokenStream* ts) {
ts->data = NULL;
ts->count = 0;
ts->capacity = 0;
}


void mod_src_lexer_flo_add_token(TokenStream* ts, int kind, int start, int length) {
if (ts->count >= ts->capacity) {
if (ts->capacity == 0) {
ts->capacity = 64;
}
else {
ts->capacity = ts->capacity * 2;
}
ts->data = realloc(ts->data, ts->capacity * sizeof(Token));
if (ts->data == NULL) {
flower_print_string(((flower_string){ "Memory allocation failed\n", (int)(sizeof("Memory allocation failed\n") - 1) }));
return;}
}
Token* t = ts->data + ts->count;
t->kind = kind;
t->start = start;
t->length = length;
ts->count = ts->count + 1;
}


void mod_src_lexer_flo_lex(TokenStream* ts, char* src) {
int i = 0;
while (src[i] != '\0') {
if (src[i] == ' '  ||  src[i] == '\r') {
i = i + 1;
continue;
}
if (src[i] == '/'  &&  src[i + 1] == '/') {
while (src[i] != '\n'  &&  src[i] != '\0') {
i = i + 1;
}
continue;
}
if (src[i] == '\n') {
mod_src_lexer_flo_add_token(ts, TOKEN_NEWLINE, i, 1);
i = i + 1;
continue;
}
if (src[i] == '"') {
int start = i;
i = i + 1;
while (src[i] != '"'  &&  src[i] != '\0') {
if (src[i] == '\\') {
i = i + 1;
}
i = i + 1;
}
i = i + 1;
mod_src_lexer_flo_add_token(ts, TOKEN_STRING_LIT, start, i - start);
continue;
}
if (src[i] == '\'') {
int start = i;
i = i + 1;
while (src[i] != '\''  &&  src[i] != '\0') {
if (src[i] == '\\') {
i = i + 1;
}
i = i + 1;
}
i = i + 1;
mod_src_lexer_flo_add_token(ts, TOKEN_CHAR_LIT, start, i - start);
continue;
}
if (src[i] == ';') {
mod_src_lexer_flo_add_token(ts, TOKEN_SEMI, i, 1);
i = i + 1;
continue;
}
if (isdigit(src[i])) {
int start = i;
while (isdigit(src[i])) {
i = i + 1;
}
if (src[i] == '.'  &&  isdigit(src[i + 1])) {
i = i + 1;
while (isdigit(src[i])) {
i = i + 1;
}
mod_src_lexer_flo_add_token(ts, TOKEN_FLOAT_LIT, start, i - start);
}
else {
mod_src_lexer_flo_add_token(ts, TOKEN_NUMBER, start, i - start);
}
continue;
}
if (isalpha(src[i])  ||  src[i] == '_') {
int start = i;
while (isalnum(src[i])  ||  src[i] == '_') {
i = i + 1;
}
int length = i - start;
if (length == 2  &&  !(strncmp(src + start, "if", 2))) {
mod_src_lexer_flo_add_token(ts, TOKEN_IF, start, length);
}
else if (length == 2  &&  !(strncmp(src + start, "as", 2))) {
mod_src_lexer_flo_add_token(ts, TOKEN_AS, start, length);
}
else if (length == 2  &&  !(strncmp(src + start, "in", 2))) {
mod_src_lexer_flo_add_token(ts, TOKEN_IN, start, length);
}
else if (length == 2  &&  !(strncmp(src + start, "or", 2))) {
mod_src_lexer_flo_add_token(ts, TOKEN_OR, start, length);
}
else if (length == 3  &&  !(strncmp(src + start, "int", 3))) {
mod_src_lexer_flo_add_token(ts, TOKEN_INT, start, length);
}
else if (length == 3  &&  !(strncmp(src + start, "end", 3))) {
mod_src_lexer_flo_add_token(ts, TOKEN_END, start, length);
}
else if (length == 3  &&  !(strncmp(src + start, "new", 3))) {
mod_src_lexer_flo_add_token(ts, TOKEN_NEW, start, length);
}
else if (length == 3  &&  !(strncmp(src + start, "not", 3))) {
mod_src_lexer_flo_add_token(ts, TOKEN_NOT, start, length);
}
else if (length == 3  &&  !(strncmp(src + start, "and", 3))) {
mod_src_lexer_flo_add_token(ts, TOKEN_AND, start, length);
}
else if (length == 3  &&  !(strncmp(src + start, "for", 3))) {
mod_src_lexer_flo_add_token(ts, TOKEN_FOR, start, length);
}
else if (length == 4  &&  !(strncmp(src + start, "null", 4))) {
mod_src_lexer_flo_add_token(ts, TOKEN_NULL, start, length);
}
else if (length == 4  &&  !(strncmp(src + start, "char", 4))) {
mod_src_lexer_flo_add_token(ts, TOKEN_CHAR, start, length);
}
else if (length == 4  &&  !(strncmp(src + start, "bool", 4))) {
mod_src_lexer_flo_add_token(ts, TOKEN_BOOL, start, length);
}
else if (length == 4  &&  !(strncmp(src + start, "else", 4))) {
mod_src_lexer_flo_add_token(ts, TOKEN_ELSE, start, length);
}
else if (length == 4  &&  !(strncmp(src + start, "void", 4))) {
mod_src_lexer_flo_add_token(ts, TOKEN_VOID, start, length);
}
else if (length == 4  &&  !(strncmp(src + start, "prop", 4))) {
mod_src_lexer_flo_add_token(ts, TOKEN_PROP, start, length);
}
else if (length == 4  &&  !(strncmp(src + start, "func", 4))) {
mod_src_lexer_flo_add_token(ts, TOKEN_FUNC, start, length);
}
else if (length == 4  &&  !(strncmp(src + start, "true", 4))) {
mod_src_lexer_flo_add_token(ts, TOKEN_TRUE, start, length);
}
else if (length == 5  &&  !(strncmp(src + start, "union", 5))) {
mod_src_lexer_flo_add_token(ts, TOKEN_UNION, start, length);
}
else if (length == 5  &&  !(strncmp(src + start, "while", 5))) {
mod_src_lexer_flo_add_token(ts, TOKEN_WHILE, start, length);
}
else if (length == 5  &&  !(strncmp(src + start, "prune", 5))) {
mod_src_lexer_flo_add_token(ts, TOKEN_PRUNE, start, length);
}
else if (length == 5  &&  !(strncmp(src + start, "print", 5))) {
mod_src_lexer_flo_add_token(ts, TOKEN_PRINT, start, length);
}
else if (length == 5  &&  !(strncmp(src + start, "float", 5))) {
mod_src_lexer_flo_add_token(ts, TOKEN_FLOAT, start, length);
}
else if (length == 5  &&  !(strncmp(src + start, "break", 5))) {
mod_src_lexer_flo_add_token(ts, TOKEN_BREAK, start, length);
}
else if (length == 5  &&  !(strncmp(src + start, "false", 5))) {
mod_src_lexer_flo_add_token(ts, TOKEN_FALSE, start, length);
}
else if (length == 6  &&  !(strncmp(src + start, "return", 6))) {
mod_src_lexer_flo_add_token(ts, TOKEN_RETURN, start, length);
}
else if (length == 6  &&  !(strncmp(src + start, "string", 6))) {
mod_src_lexer_flo_add_token(ts, TOKEN_STRING, start, length);
}
else if (length == 6  &&  !(strncmp(src + start, "sizeof", 6))) {
mod_src_lexer_flo_add_token(ts, TOKEN_SIZEOF, start, length);
}
else if (length == 6  &&  !(strncmp(src + start, "struct", 6))) {
mod_src_lexer_flo_add_token(ts, TOKEN_STRUCT, start, length);
}
else if (length == 6  &&  !(strncmp(src + start, "import", 6))) {
mod_src_lexer_flo_add_token(ts, TOKEN_IMPORT, start, length);
}
else if (length == 6  &&  !(strncmp(src + start, "double", 6))) {
mod_src_lexer_flo_add_token(ts, TOKEN_DOUBLE, start, length);
}
else if (length == 6  &&  !(strncmp(src + start, "hidden", 6))) {
mod_src_lexer_flo_add_token(ts, TOKEN_HIDDEN, start, length);
}
else if (length == 6  &&  !(strncmp(src + start, "frozen", 6))) {
mod_src_lexer_flo_add_token(ts, TOKEN_FROZEN, start, length);
}
else if (length == 7  &&  !(strncmp(src + start, "forward", 7))) {
mod_src_lexer_flo_add_token(ts, TOKEN_FORWARD, start, length);
}
else if (length == 8  &&  !(strncmp(src + start, "continue", 8))) {
mod_src_lexer_flo_add_token(ts, TOKEN_CONTINUE, start, length);
}
else if (length == 8  &&  !(strncmp(src + start, "readonly", 8))) {
mod_src_lexer_flo_add_token(ts, TOKEN_READONLY, start, length);
}
else {
mod_src_lexer_flo_add_token(ts, TOKEN_IDENTIFIER, start, length);
}
continue;
}
if (src[i] == '.') {
if (src[i + 1] == '.') {
if (src[i + 2] == '=') {
mod_src_lexer_flo_add_token(ts, TOKEN_DOTDOTEQ, i, 3);
i = i + 3;
continue;
}
else {
mod_src_lexer_flo_add_token(ts, TOKEN_DOTDOT, i, 2);
i = i + 2;
continue;
}
}
else {
mod_src_lexer_flo_add_token(ts, TOKEN_DOT, i, 1);
i = i + 1;
continue;
}
}
else if (src[i] == '+') {
mod_src_lexer_flo_add_token(ts, TOKEN_PLUS, i, 1);
i = i + 1;
continue;
}
else if (src[i] == '-') {
mod_src_lexer_flo_add_token(ts, TOKEN_MINUS, i, 1);
i = i + 1;
continue;
}
else if (src[i] == '~') {
mod_src_lexer_flo_add_token(ts, TOKEN_TILDE, i, 1);
i = i + 1;
continue;
}
else if (src[i] == '*') {
mod_src_lexer_flo_add_token(ts, TOKEN_STAR, i, 1);
i = i + 1;
continue;
}
else if (src[i] == '/') {
mod_src_lexer_flo_add_token(ts, TOKEN_SLASH, i, 1);
i = i + 1;
continue;
}
else if (src[i] == '^') {
mod_src_lexer_flo_add_token(ts, TOKEN_CARET, i, 1);
i = i + 1;
continue;
}
else if (src[i] == '&') {
mod_src_lexer_flo_add_token(ts, TOKEN_AMPERSAND, i, 1);
i = i + 1;
continue;
}
else if (src[i] == '@') {
mod_src_lexer_flo_add_token(ts, TOKEN_AT, i, 1);
i = i + 1;
continue;
}
else if (src[i] == '=') {
if (src[i + 1] == '=') {
mod_src_lexer_flo_add_token(ts, TOKEN_COMP, i, 2);
i = i + 2;
continue;
}
else {
mod_src_lexer_flo_add_token(ts, TOKEN_ASSIGN, i, 1);
i = i + 1;
continue;
}
}
else if (src[i] == '<') {
if (src[i + 1] == '=') {
mod_src_lexer_flo_add_token(ts, TOKEN_LEQ, i, 2);
i = i + 2;
continue;
}
else {
mod_src_lexer_flo_add_token(ts, TOKEN_LT, i, 1);
i = i + 1;
continue;
}
}
else if (src[i] == '>') {
if (src[i + 1] == '=') {
mod_src_lexer_flo_add_token(ts, TOKEN_GEQ, i, 2);
i = i + 2;
continue;
}
else {
mod_src_lexer_flo_add_token(ts, TOKEN_GT, i, 1);
i = i + 1;
continue;
}
}
else if (src[i] == '!') {
if (src[i + 1] == '=') {
mod_src_lexer_flo_add_token(ts, TOKEN_NEQ, i, 2);
i = i + 2;
continue;
}
else {
mod_src_lexer_flo_add_token(ts, TOKEN_NOT, i, 1);
i = i + 1;
continue;
}
}
else if (src[i] == '(') {
mod_src_lexer_flo_add_token(ts, TOKEN_LPAREN, i, 1);
i = i + 1;
continue;
}
else if (src[i] == ')') {
mod_src_lexer_flo_add_token(ts, TOKEN_RPAREN, i, 1);
i = i + 1;
continue;
}
else if (src[i] == '[') {
mod_src_lexer_flo_add_token(ts, TOKEN_LBRACK, i, 1);
i = i + 1;
continue;
}
else if (src[i] == ']') {
mod_src_lexer_flo_add_token(ts, TOKEN_RBRACK, i, 1);
i = i + 1;
continue;
}
else if (src[i] == '{') {
mod_src_lexer_flo_add_token(ts, TOKEN_LBRACE, i, 1);
i = i + 1;
continue;
}
else if (src[i] == '}') {
mod_src_lexer_flo_add_token(ts, TOKEN_RBRACE, i, 1);
i = i + 1;
continue;
}
else if (src[i] == ':') {
mod_src_lexer_flo_add_token(ts, TOKEN_COLON, i, 1);
i = i + 1;
continue;
}
else if (src[i] == ',') {
mod_src_lexer_flo_add_token(ts, TOKEN_COMMA, i, 1);
i = i + 1;
continue;
}
else {
printf("%s%s:%d:%d: error:%s Unknown char '%c' (%d)\n", RED, current_file, mod_include_lexer_h_flo_get_line(src, i), mod_include_lexer_h_flo_get_col(src, i), RESET, src[i], src[i]);
exit(1);
}
i = i + 1;
}
mod_src_lexer_flo_add_token(ts, TOKEN_EOF, i, 0);
}


char* mod_src_lexer_flo_token_kind_name(int kind) {
if (kind == TOKEN_INT) {
return "TOKEN_INT";
}
else if (kind == TOKEN_FLOAT) {
return "TOKEN_FLOAT";
}
else if (kind == TOKEN_DOUBLE) {
return "TOKEN_DOUBLE";
}
else if (kind == TOKEN_VOID) {
return "TOKEN_VOID";
}
else if (kind == TOKEN_NULL) {
return "TOKEN_NULL";
}
else if (kind == TOKEN_BOOL) {
return "TOKEN_BOOL";
}
else if (kind == TOKEN_CHAR) {
return "TOKEN_CHAR";
}
else if (kind == TOKEN_STRING) {
return "TOKEN_STRING";
}
else if (kind == TOKEN_STRING_LIT) {
return "TOKEN_STRING_LIT";
}
else if (kind == TOKEN_CHAR_LIT) {
return "TOKEN_CHAR_LIT";
}
else if (kind == TOKEN_TRUE) {
return "TOKEN_TRUE";
}
else if (kind == TOKEN_FALSE) {
return "TOKEN_FALSE";
}
else if (kind == TOKEN_STRUCT) {
return "TOKEN_STRUCT";
}
else if (kind == TOKEN_UNION) {
return "TOKEN_UNION";
}
else if (kind == TOKEN_IDENTIFIER) {
return "TOKEN_IDENTIFIER";
}
else if (kind == TOKEN_NUMBER) {
return "TOKEN_NUMBER";
}
else if (kind == TOKEN_RETURN) {
return "TOKEN_RETURN";
}
else if (kind == TOKEN_CONTINUE) {
return "TOKEN_CONTINUE";
}
else if (kind == TOKEN_BREAK) {
return "TOKEN_BREAK";
}
else if (kind == TOKEN_WHILE) {
return "TOKEN_WHILE";
}
else if (kind == TOKEN_FUNC) {
return "TOKEN_FUNC";
}
else if (kind == TOKEN_IF) {
return "TOKEN_IF";
}
else if (kind == TOKEN_ELSE) {
return "TOKEN_ELSE";
}
else if (kind == TOKEN_END) {
return "TOKEN_END";
}
else if (kind == TOKEN_NOT) {
return "TOKEN_NOT";
}
else if (kind == TOKEN_FOR) {
return "TOKEN_FOR";
}
else if (kind == TOKEN_AND) {
return "TOKEN_AND";
}
else if (kind == TOKEN_IN) {
return "TOKEN_IN";
}
else if (kind == TOKEN_OR) {
return "TOKEN_OR";
}
else if (kind == TOKEN_IMPORT) {
return "TOKEN_IMPORT";
}
else if (kind == TOKEN_PROP) {
return "TOKEN_PROP";
}
else if (kind == TOKEN_HIDDEN) {
return "TOKEN_HIDDEN";
}
else if (kind == TOKEN_FROZEN) {
return "TOKEN_FROZEN";
}
else if (kind == TOKEN_READONLY) {
return "TOKEN_READONLY";
}
else if (kind == TOKEN_AS) {
return "TOKEN_AS";
}
else if (kind == TOKEN_NEW) {
return "TOKEN_NEW";
}
else if (kind == TOKEN_PRUNE) {
return "TOKEN_PRUNE";
}
else if (kind == TOKEN_SIZEOF) {
return "TOKEN_SIZEOF";
}
else if (kind == TOKEN_PRINT) {
return "TOKEN_PRINT";
}
else if (kind == TOKEN_DOT) {
return "TOKEN_DOT";
}
else if (kind == TOKEN_DOTDOT) {
return "TOKEN_DOTDOT";
}
else if (kind == TOKEN_DOTDOTEQ) {
return "TOKEN_DOTDOTEQ";
}
else if (kind == TOKEN_PLUS) {
return "TOKEN_PLUS";
}
else if (kind == TOKEN_MINUS) {
return "TOKEN_MINUS";
}
else if (kind == TOKEN_TILDE) {
return "TOKEN_TILDE";
}
else if (kind == TOKEN_STAR) {
return "TOKEN_STAR";
}
else if (kind == TOKEN_SLASH) {
return "TOKEN_SLASH";
}
else if (kind == TOKEN_CARET) {
return "TOKEN_CARET";
}
else if (kind == TOKEN_AMPERSAND) {
return "TOKEN_AMPERSAND";
}
else if (kind == TOKEN_AT) {
return "TOKEN_AT";
}
else if (kind == TOKEN_ASSIGN) {
return "TOKEN_ASSIGN";
}
else if (kind == TOKEN_LT) {
return "TOKEN_LT";
}
else if (kind == TOKEN_GT) {
return "TOKEN_GT";
}
else if (kind == TOKEN_NEQ) {
return "TOKEN_NEQ";
}
else if (kind == TOKEN_GEQ) {
return "TOKEN_GEQ";
}
else if (kind == TOKEN_LEQ) {
return "TOKEN_LEQ";
}
else if (kind == TOKEN_COMP) {
return "TOKEN_COMP";
}
else if (kind == TOKEN_LPAREN) {
return "TOKEN_LPAREN";
}
else if (kind == TOKEN_RPAREN) {
return "TOKEN_RPAREN";
}
else if (kind == TOKEN_LBRACK) {
return "TOKEN_LBRACK";
}
else if (kind == TOKEN_RBRACK) {
return "TOKEN_RBRACK";
}
else if (kind == TOKEN_LBRACE) {
return "TOKEN_LBRACE";
}
else if (kind == TOKEN_RBRACE) {
return "TOKEN_RBRACE";
}
else if (kind == TOKEN_COLON) {
return "TOKEN_COLON";
}
else if (kind == TOKEN_COMMA) {
return "TOKEN_COMMA";
}
else if (kind == TOKEN_SEMI) {
return "TOKEN_SEMI";
}
else if (kind == TOKEN_NEWLINE) {
return "TOKEN_NEWLINE";
}
else if (kind == TOKEN_EOF) {
return "TOKEN_EOF";
}
else {
return "UNKNOWN";
}
}


void mod_src_lexer_flo_print_token(Token* token, char* src) {
printf("%-18s\t", mod_src_lexer_flo_token_kind_name(token->kind));
if (token->kind == TOKEN_IDENTIFIER  ||  token->kind == TOKEN_NUMBER) {
printf("\"%.*s\"", token->length, src + token->start);
}
flower_print_string(((flower_string){ "\n", (int)(sizeof("\n") - 1) }));
}


void mod_src_lexer_flo_print_all_tokens(TokenStream* ts, char* src) {
for (int i = 0; ((0) > (ts->count)) ? i > (ts->count) : i < (ts->count); ((0) > (ts->count)) ? i-- : i++) {
mod_src_lexer_flo_print_token(&(ts->data[i]), src);
}
}


Token* mod_src_lexer_flo_peek(TokenStream* ts, int index) {
if (index >= ts->count) {
return &(ts->data[ts->count - 1]);
}
return &(ts->data[index]);
}


void mod_src_lexer_flo_free_token_stream(TokenStream* ts) {
free(ts->data);
ts->data = NULL;
ts->count = 0;
ts->capacity = 0;
}
int AST_FLOW_CONTROL = 0;
int AST_LITERAL = 1;
int AST_FLOAT_LIT = 2;
int AST_BOOL_LIT = 3;
int AST_CAST = 4;
int AST_NULL = 5;
int AST_STRING_LIT = 6;
int AST_ARRAY_LIT = 7;
int AST_SUBSCRIPT = 8;
int AST_STRUCT_LIT = 9;
int AST_CHAR_LIT = 10;
int AST_STRUCT_DEF = 11;
int AST_UNION_DEF = 12;
int AST_STRUCT_FIELD = 13;
int AST_DOT_ACCESS = 14;
int AST_BINARY_OP = 15;
int AST_UNARY_NOT = 16;
int AST_UNARY_NEG = 17;
int AST_NEW = 18;
int AST_PRUNE = 19;
int AST_SIZEOF = 20;
int AST_DEREF_ASS = 21;
int AST_DEREF = 22;
int AST_GET_ADDR = 23;
int AST_IMPORT = 24;
int AST_PROP = 25;
int AST_FORWARD = 26;
int AST_ALIAS_CALL = 27;
int AST_VAR_DECL = 28;
int AST_VAR_ASS = 29;
int AST_VAR_REF = 30;
int AST_FUNC_DEF = 31;
int AST_FUNC_CALL = 32;
int AST_WHILE = 33;
int AST_FOR = 34;
int AST_IF = 35;
int AST_PARAM = 36;
int AST_PRINT = 37;
int FIELD_FALSE = 0;
int FIELD_TRUE = 1;
int STRING_LOWER_STRING = 0;
int STRING_LOWER_CSTR = 1;
typedef struct AST AST;


typedef struct TypeInfo {
int base;
int pointer_depth;
int array_size;
AST* arr_size_expr;
int name_start;
int name_length;
char* name_src;
} TypeInfo;


typedef struct cast {
AST* value;
TypeInfo type;
TypeInfo value_type;
} cast;


typedef struct float_lit {
int start;
int length;
} float_lit;


typedef struct bool_lit {
int value;
} bool_lit;


typedef struct char_lit {
int start;
int length;
} char_lit;


typedef struct strng {
int str_start;
int str_length;
int lower_kind;
} strng;


typedef struct array {
AST* elements;
int arr_length;
} array;


typedef struct subscript {
AST* array;
AST* index;
AST* value;
} subscript;


typedef struct unary {
AST* operand;
} unary;


typedef struct new_alloc {
TypeInfo type;
} new_alloc;


typedef struct prune_free {
AST* ptr;
} prune_free;


typedef struct size_of {
TypeInfo type;
} size_of;


typedef struct deref_ass {
int name_start;
int name_length;
AST* value;
} deref_ass;


typedef struct deref {
AST* operand;
} deref;


typedef struct get_addr {
AST* operand;
} get_addr;


typedef struct prnt {
AST* value;
TypeInfo value_type;
} prnt;


typedef struct imprt {
int path_start;
int path_length;
int alias_start;
int alias_length;
int is_system;
int has_alias;
} imprt;


typedef struct prp {
AST* decl;
} prp;


typedef struct forwrd {
int name_start;
int name_length;
int is_func;
TypeInfo return_type;
AST* params;
} forwrd;


typedef struct alias_call {
int alias_start;
int alias_length;
int func_start;
int func_length;
AST* args;
} alias_call;


typedef struct struct_def {
int name_start;
int name_length;
AST* fields;
} struct_def;


typedef struct union_def {
int name_start;
int name_length;
AST* fields;
} union_def;


typedef struct struct_field {
int name_start;
int name_length;
TypeInfo type;
int is_hidden;
int is_frozen;
int is_readonly;
} struct_field;


typedef struct struct_lit {
AST* elements;
} struct_lit;
int ACCESS_UNKNOWN = 0;
int ACCESS_DOT = 1;
int ACCESS_ARROW = 2;


typedef struct dot_access {
AST* object;
int field_start;
int field_length;
AST* value;
int access_kind;
TypeInfo resolved_type;
int is_hidden;
int is_frozen;
int is_readonly;
char* owner_src;
} dot_access;


typedef struct binary {
AST* left;
AST* right;
int op;
int is_string_compare;
} binary;


typedef struct var_decl {
int name_start;
int name_length;
TypeInfo type;
AST* value;
} var_decl;


typedef struct var_ass {
int name_start;
int name_length;
AST* value;
} var_ass;


typedef struct var_ref {
int name_start;
int name_length;
} var_ref;


typedef struct func_def {
int name_start;
int name_length;
AST* params;
AST* body;
TypeInfo return_type;
} func_def;


typedef struct func_params {
int name_start;
int name_length;
TypeInfo type;
} func_params;


typedef struct func_call {
int name_start;
int name_length;
AST* args;
} func_call;


typedef struct flow_ctrl {
Token* base;
AST* value;
} flow_ctrl;


typedef struct while_loop {
AST* condition;
AST* body;
} while_loop;


typedef struct for_loop {
int var_start;
int var_length;
AST* from;
AST* to;
int inclusive;
AST* body;
} for_loop;


typedef struct if_condition {
AST* condition;
AST* body;
AST* else_branch;
} if_condition;


typedef union NodeData {
cast _cast;
float_lit _float_lit;
bool_lit _bool_lit;
char_lit _char_lit;
strng _string;
array _array;
subscript _subscript;
unary _unary;
new_alloc _new_alloc;
prune_free _prune_free;
size_of _size_of;
deref_ass _deref_ass;
deref _deref;
get_addr _get_addr;
prnt _print;
imprt _import;
prp _prop;
forwrd _forward;
alias_call _alias_call;
struct_def _struct_def;
union_def _union_def;
struct_field _struct_field;
struct_lit _struct_lit;
dot_access _dot_access;
binary _binary;
var_decl _var_decl;
var_ass _var_ass;
var_ref _var_ref;
func_def _func_def;
func_params _func_params;
func_call _func_call;
flow_ctrl _flow_ctrl;
while_loop _while_loop;
for_loop _for_loop;
if_condition _if_condition;
} NodeData;


typedef struct AST {
int kind;
AST* next;
int value;
char* src;
char* filename;
int start;
NodeData data;
} AST;


typedef struct Parser {
TokenStream* ts;
char* src;
int pos;
int alias_start[32];
int alias_lengths[32];
int alias_count;
char* filename;
int error_count;
} Parser;
Token* mod_src_parser_flo_parser_peek(Parser* ps);


AST* mod_src_parser_flo_make_node(Parser* ps, int kind) {
AST* node = calloc(1, sizeof(AST));
node->kind = kind;
node->value = 0;
node->src = ps->src;
node->start = mod_src_parser_flo_parser_peek(ps)->start;
node->filename = ps->filename;
return node;
}


void mod_src_parser_flo_init_parser(Parser* ps, TokenStream* ts, char* src, char* filename) {
ps->ts = ts;
ps->src = src;
ps->pos = 0;
ps->alias_count = 0;
ps->filename = filename;
ps->error_count = 0;
}
void mod_src_parser_flo_parser_error(Parser* ps, char* message);
void mod_src_parser_flo_parser_sync_until(Parser* ps, int stop_a, int stop_b);
AST* mod_src_parser_flo_parse_var_decl(Parser* ps);
AST* mod_src_parser_flo_parse_var_ass(Parser* ps);
AST* mod_src_parser_flo_parse_subscript_ass(Parser* ps);
AST* mod_src_parser_flo_parse_return(Parser* ps);
AST* mod_src_parser_flo_parse_continue(Parser* ps);
AST* mod_src_parser_flo_parse_break(Parser* ps);
AST* mod_src_parser_flo_parse_while(Parser* ps);
AST* mod_src_parser_flo_parse_if(Parser* ps, int is_elf_if);
AST* mod_src_parser_flo_parse_for(Parser* ps);
AST* mod_src_parser_flo_parse_expr(Parser* ps, int min_prec);
AST* mod_src_parser_flo_parse_statement(Parser* ps);
AST* mod_src_parser_flo_parse_primary(Parser* ps);
AST* mod_src_parser_flo_parse_func_def(Parser* ps);
AST* mod_src_parser_flo_parse_func_call(Parser* ps);
AST* mod_src_parser_flo_parse_alias_call(Parser* ps);
AST* mod_src_parser_flo_parse_param(Parser* ps);
AST* mod_src_parser_flo_parse_struct(Parser* ps);
AST* mod_src_parser_flo_parse_dot_ass(Parser* ps);
AST* mod_src_parser_flo_parse_forward(Parser* ps);
AST* mod_src_parser_flo_parse_import(Parser* ps);
int mod_src_parser_flo_is_field_flag(int kind);
void mod_src_parser_flo_parse_field_flags(Parser* ps, AST* field);


Token* mod_src_parser_flo_parser_peek(Parser* ps) {
return mod_src_lexer_flo_peek(ps->ts, ps->pos);
}


Token* mod_src_parser_flo_parser_advance(Parser* ps) {
Token* token = mod_src_parser_flo_parser_peek(ps);
if (token->kind != TOKEN_EOF) {
ps->pos = ps->pos + 1;
}
return token;
}


void mod_src_parser_flo_parser_skip_newline(Parser* ps) {
while (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_NEWLINE  ||  mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_SEMI) {
mod_src_parser_flo_parser_advance(ps);
}
}


int mod_src_parser_flo_parser_expect(Parser* ps, int kind) {
Token* curr_tok = mod_src_parser_flo_parser_peek(ps);
if (curr_tok->kind != kind) {
char message[128];
snprintf(message, sizeof(message), "Expected token: %s but got %s", mod_src_lexer_flo_token_kind_name(kind), mod_src_lexer_flo_token_kind_name(curr_tok->kind));
mod_src_parser_flo_parser_error(ps, message);
mod_src_parser_flo_parser_advance(ps);
return 0;
}
mod_src_parser_flo_parser_advance(ps);
return 1;
}


TypeInfo* mod_src_parser_flo_parse_type(Parser* ps) {
TypeInfo* type = malloc(sizeof(TypeInfo));
type->pointer_depth = 0;
type->array_size = 0;
type->arr_size_expr = NULL;
type->name_src = ps->src;
while (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_AT) {
type->pointer_depth = type->pointer_depth + 1;
mod_src_parser_flo_parser_advance(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in type");
return type;
}
}
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file while parsing type");
return type;
}
Token* base = mod_src_parser_flo_parser_advance(ps);
type->base = base->kind;
if (base->kind == TOKEN_IDENTIFIER) {
type->name_start = base->start;
type->name_length = base->length;
}
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_LBRACK) {
mod_src_parser_flo_parser_advance(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in array type");
return type;
}
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_RBRACK) {
type->array_size = -1;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_NUMBER) {
type->array_size = atoi(ps->src + mod_src_parser_flo_parser_peek(ps)->start);
mod_src_parser_flo_parser_advance(ps);
}
else {
type->arr_size_expr = mod_src_parser_flo_parse_expr(ps, 0);
}
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_RBRACK))) {
return type;
}
}
return type;
}


int mod_src_parser_flo_get_precedence(int kind) {
if (kind == TOKEN_STAR  ||  kind == TOKEN_SLASH) {
return 5;
}
else if (kind == TOKEN_PLUS  ||  kind == TOKEN_MINUS) {
return 4;
}
else if (kind == TOKEN_GT  ||  kind == TOKEN_LT  ||  kind == TOKEN_NEQ  ||  kind == TOKEN_LEQ  ||  kind == TOKEN_GEQ  ||  kind == TOKEN_COMP) {
return 3;
}
else if (kind == TOKEN_AND) {
return 2;
}
else if (kind == TOKEN_OR) {
return 1;
}
else {
return 0;
}
}


int mod_src_parser_flo_token_stream_contains(TokenStream* ts, int kind) {
for (int i = 0; ((0) > (ts->count)) ? i > (ts->count) : i < (ts->count); ((0) > (ts->count)) ? i-- : i++) {
Token* t = ts->data + i;
if (t->kind == kind) {
return 1;
}
}
return 0;
}


int mod_src_parser_flo_is_alias(Parser* ps, Token* tok) {
for (int i = 0; ((0) > (ps->alias_count)) ? i > (ps->alias_count) : i < (ps->alias_count); ((0) > (ps->alias_count)) ? i-- : i++) {
if (ps->alias_lengths[i] == tok->length  &&  !(strncmp(ps->src + ps->alias_start[i], ps->src + tok->start, tok->length))) {
return 1;
}
}
return 0;
}


void mod_src_parser_flo_parser_error(Parser* ps, char* message) {
Token* token = mod_src_parser_flo_parser_peek(ps);
int line = mod_include_lexer_h_flo_get_line(ps->src, token->start);
int col = mod_include_lexer_h_flo_get_col(ps->src, token->start);
printf("%s%s:%d:%d: error:%s %s\n", RED, ps->filename, line, col, RESET, message);
ps->error_count = ps->error_count + 1;
}


void mod_src_parser_flo_parser_sync_until(Parser* ps, int stop_a, int stop_b) {
while (mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_EOF  &&  mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_NEWLINE  &&  mod_src_parser_flo_parser_peek(ps)->kind != stop_a  &&  mod_src_parser_flo_parser_peek(ps)->kind != stop_b) {
mod_src_parser_flo_parser_advance(ps);
}
}


int mod_src_parser_flo_is_field_flag(int kind) {
if (kind == TOKEN_HIDDEN  ||  kind == TOKEN_READONLY  ||  kind == TOKEN_FROZEN) {
return 1;
}
return 0;
}


void mod_src_parser_flo_parse_field_flags(Parser* ps, AST* field) {
field->data._struct_field.is_hidden = 0;
field->data._struct_field.is_frozen = 0;
field->data._struct_field.is_readonly = 0;
while (mod_src_parser_flo_is_field_flag(mod_src_parser_flo_parser_peek(ps)->kind)) {
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_HIDDEN) {
field->data._struct_field.is_hidden = 1;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_FROZEN) {
field->data._struct_field.is_frozen = 1;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_READONLY) {
field->data._struct_field.is_readonly = 1;
}
mod_src_parser_flo_parser_advance(ps);
}
}


AST* mod_src_parser_flo_parse_var_decl(Parser* ps) {
AST* node = mod_src_parser_flo_make_node(ps, AST_VAR_DECL);
Token* tok = mod_src_parser_flo_parser_advance(ps);
if (tok->kind != TOKEN_IDENTIFIER) {
mod_src_parser_flo_parser_error(ps, "Expected variable name");
return node;
}
node->data._var_decl.name_start = tok->start;
node->data._var_decl.name_length = tok->length;
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_COLON))) {
return node;
}
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in variable declaration");
return node;
}
node->data._var_decl.type = *(mod_src_parser_flo_parse_type(ps));
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_ASSIGN) {
mod_src_parser_flo_parser_advance(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in variable initializer");
return node;
}
node->data._var_decl.value = mod_src_parser_flo_parse_expr(ps, 0);
}
return node;
}


AST* mod_src_parser_flo_parse_var_ass(Parser* ps) {
AST* node = mod_src_parser_flo_make_node(ps, AST_VAR_ASS);
Token* tok = mod_src_parser_flo_parser_advance(ps);
if (tok->kind != TOKEN_IDENTIFIER) {
mod_src_parser_flo_parser_error(ps, "Expected variable name");
return node;
}
node->data._var_ass.name_start = tok->start;
node->data._var_ass.name_length = tok->length;
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_ASSIGN))) {
return node;
}
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in variable assignment");
return node;
}
node->data._var_ass.value = mod_src_parser_flo_parse_expr(ps, 0);
return node;
}


AST* mod_src_parser_flo_parse_subscript_ass(Parser* ps) {
Token* name = mod_src_parser_flo_parser_advance(ps);
AST* arr_ref = mod_src_parser_flo_make_node(ps, AST_VAR_REF);
arr_ref->data._var_ref.name_start = name->start;
arr_ref->data._var_ref.name_length = name->length;
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_LBRACK))) {
return arr_ref;
}
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in subscript expression");
return arr_ref;
}
AST* idx = mod_src_parser_flo_parse_expr(ps, 0);
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_RBRACK))) {
return arr_ref;
}
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_ASSIGN))) {
return arr_ref;
}
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in subscript expression");
return arr_ref;
}
AST* val = mod_src_parser_flo_parse_expr(ps, 0);
AST* sub = mod_src_parser_flo_make_node(ps, AST_SUBSCRIPT);
sub->data._subscript.array = arr_ref;
sub->data._subscript.index = idx;
sub->data._subscript.value = val;
return sub;
}


AST* mod_src_parser_flo_parse_return(Parser* ps) {
AST* node = mod_src_parser_flo_make_node(ps, AST_FLOW_CONTROL);
node->data._flow_ctrl.base = mod_src_parser_flo_parser_peek(ps);
mod_src_parser_flo_parser_advance(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_NEWLINE  ||  mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_SEMI  ||  mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
node->data._flow_ctrl.value = NULL;
return node;
}
node->data._flow_ctrl.value = mod_src_parser_flo_parse_expr(ps, 0);
return node;
}


AST* mod_src_parser_flo_parse_continue(Parser* ps) {
AST* node = mod_src_parser_flo_make_node(ps, AST_FLOW_CONTROL);
node->data._flow_ctrl.value = NULL;
node->data._flow_ctrl.base = mod_src_parser_flo_parser_peek(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file after continue");
}
mod_src_parser_flo_parser_advance(ps);
return node;
}


AST* mod_src_parser_flo_parse_break(Parser* ps) {
AST* node = mod_src_parser_flo_make_node(ps, AST_FLOW_CONTROL);
node->data._flow_ctrl.value = NULL;
node->data._flow_ctrl.base = mod_src_parser_flo_parser_peek(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file after break");
}
mod_src_parser_flo_parser_advance(ps);
return node;
}


AST* mod_src_parser_flo_parse_while(Parser* ps) {
AST* node = mod_src_parser_flo_make_node(ps, AST_WHILE);
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_WHILE))) {
return node;
}
node->data._while_loop.condition = mod_src_parser_flo_parse_expr(ps, 0);
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_COLON))) {
return node;
}
AST* body_head = NULL;
AST* body_tail = NULL;
while (mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_END) {
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in if statement body");
return node;
}
mod_src_parser_flo_parser_skip_newline(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_END) {
break;
}
AST* statement = mod_src_parser_flo_parse_statement(ps);
if (body_head == NULL) {
body_head = statement;
}
else {
body_tail->next = statement;
}
body_tail = statement;
}
mod_src_parser_flo_parser_expect(ps, TOKEN_END);
node->data._while_loop.body = body_head;
return node;
}


AST* mod_src_parser_flo_parse_if(Parser* ps, int is_elf_if) {
AST* node = mod_src_parser_flo_make_node(ps, AST_IF);
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_IF))) {
return node;
}
node->data._if_condition.condition = mod_src_parser_flo_parse_expr(ps, 0);
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_COLON))) {
return node;
}
AST* body_head = NULL;
AST* body_tail = NULL;
while (mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_END  &&  mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_ELSE) {
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in if statement body");
return node;
}
mod_src_parser_flo_parser_skip_newline(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_END  ||  mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_ELSE) {
break;
}
AST* statement = mod_src_parser_flo_parse_statement(ps);
if (body_head == NULL) {
body_head = statement;
}
else {
body_tail->next = statement;
}
body_tail = statement;
}
node->data._if_condition.body = body_head;
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_ELSE) {
mod_src_parser_flo_parser_advance(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_IF) {
node->data._if_condition.else_branch = mod_src_parser_flo_parse_if(ps, 1);
}
else {
AST* else_node = mod_src_parser_flo_make_node(ps, AST_IF);
else_node->data._if_condition.condition = NULL;
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_COLON))) {
return node;
}
AST* else_body_head = NULL;
AST* else_body_tail = NULL;
while (mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_END  &&  mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_ELSE) {
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in if statement body");
return node;
}
mod_src_parser_flo_parser_skip_newline(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_END  ||  mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_ELSE) {
break;
}
AST* statement = mod_src_parser_flo_parse_statement(ps);
if (else_body_head == NULL) {
else_body_head = statement;
}
else {
else_body_tail->next = statement;
}
else_body_tail = statement;
}
else_node->data._if_condition.body = else_body_head;
node->data._if_condition.else_branch = else_node;
}
}
if (!(is_elf_if)) {
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_END))) {
return node;
}
}
return node;
}


AST* mod_src_parser_flo_parse_for(Parser* ps) {
AST* node = mod_src_parser_flo_make_node(ps, AST_FOR);
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_FOR))) {
return node;
}
Token* var = mod_src_parser_flo_parser_advance(ps);
if (var->kind != TOKEN_IDENTIFIER) {
char message[128];
snprintf(message, sizeof(message), "Expected loop variable after for, got %s\n", mod_src_lexer_flo_token_kind_name(var->kind));
mod_src_parser_flo_parser_error(ps, message);
return node;
}
node->data._for_loop.var_start = var->start;
node->data._for_loop.var_length = var->length;
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_IN))) {
return node;
}
AST* from = mod_src_parser_flo_parse_expr(ps, 0);
node->data._for_loop.from = from;
Token* op_range = mod_src_parser_flo_parser_advance(ps);
if (op_range->kind != TOKEN_DOTDOT  &&  op_range->kind != TOKEN_DOTDOTEQ) {
char message[128];
snprintf(message, sizeof(message), "Expected range operator `..` OR `..=`, got %s\n", mod_src_lexer_flo_token_kind_name(op_range->kind));
mod_src_parser_flo_parser_error(ps, message);
return node;
}
node->data._for_loop.inclusive = op_range->kind == TOKEN_DOTDOTEQ;
AST* to = mod_src_parser_flo_parse_expr(ps, 0);
node->data._for_loop.to = to;
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_COLON))) {
return node;
}
AST* body_head = NULL;
AST* body_tail = NULL;
while (mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_END) {
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in for loop body");
}
mod_src_parser_flo_parser_skip_newline(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_END) {
break;
}
AST* statement = mod_src_parser_flo_parse_statement(ps);
if (body_head == NULL) {
body_head = statement;
}
else {
body_tail->next = statement;
}
body_tail = statement;
}
mod_src_parser_flo_parser_expect(ps, TOKEN_END);
node->data._for_loop.body = body_head;
return node;
}


AST* mod_src_parser_flo_parse_expr(Parser* ps, int min_prec) {
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in expression");
return NULL;
}
AST* left = mod_src_parser_flo_parse_primary(ps);
if (left == NULL) {
return NULL;
}
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_AS) {
mod_src_parser_flo_parser_advance(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in cast expression");
return NULL;
}
AST* cast = mod_src_parser_flo_make_node(ps, AST_CAST);
cast->data._cast.value = left;
cast->data._cast.type = *(mod_src_parser_flo_parse_type(ps));
left = cast;
}
while (mod_src_parser_flo_get_precedence(mod_src_parser_flo_parser_peek(ps)->kind) > min_prec) {
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
break;
}
Token* op = mod_src_parser_flo_parser_advance(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file after binary operator");
return left;
}
AST* right = mod_src_parser_flo_parse_expr(ps, mod_src_parser_flo_get_precedence(op->kind));
if (right == NULL) {
return left;
}
AST* bin = mod_src_parser_flo_make_node(ps, AST_BINARY_OP);
bin->data._binary.left = left;
bin->data._binary.right = right;
bin->data._binary.op = op->kind;
left = bin;
}
return left;
}


AST* mod_src_parser_flo_parse_statement(Parser* ps) {
mod_src_parser_flo_parser_skip_newline(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file while parsing statement");
return NULL;
}
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_IDENTIFIER) {
if (mod_src_lexer_flo_peek(ps->ts, ps->pos + 1)->kind == TOKEN_COLON) {
return mod_src_parser_flo_parse_var_decl(ps);
}
else if (mod_src_lexer_flo_peek(ps->ts, ps->pos + 1)->kind == TOKEN_ASSIGN) {
return mod_src_parser_flo_parse_var_ass(ps);
}
else if (mod_src_lexer_flo_peek(ps->ts, ps->pos + 1)->kind == TOKEN_LPAREN) {
return mod_src_parser_flo_parse_func_call(ps);
}
else if (mod_src_lexer_flo_peek(ps->ts, ps->pos + 1)->kind == TOKEN_LBRACK) {
return mod_src_parser_flo_parse_dot_ass(ps);
}
else if (mod_src_lexer_flo_peek(ps->ts, ps->pos + 1)->kind == TOKEN_DOT) {
if (mod_src_parser_flo_is_alias(ps, mod_src_parser_flo_parser_peek(ps))) {
return mod_src_parser_flo_parse_alias_call(ps);
}
else {
return mod_src_parser_flo_parse_dot_ass(ps);
}
}
else {
char message[128];
snprintf(message, sizeof(message), "Unexpected token while parsing statement after identifier: %s", mod_src_lexer_flo_token_kind_name(mod_src_lexer_flo_peek(ps->ts, ps->pos + 1)->kind));
mod_src_parser_flo_parser_error(ps, message);
return NULL;
}
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_WHILE) {
return mod_src_parser_flo_parse_while(ps);
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_RETURN) {
return mod_src_parser_flo_parse_return(ps);
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_BREAK) {
return mod_src_parser_flo_parse_break(ps);
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_CONTINUE) {
return mod_src_parser_flo_parse_continue(ps);
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_IF) {
return mod_src_parser_flo_parse_if(ps, 0);
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_FOR) {
return mod_src_parser_flo_parse_for(ps);
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_PRINT) {
mod_src_parser_flo_parser_advance(ps);
AST* node = mod_src_parser_flo_make_node(ps, AST_PRINT);
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_LPAREN))) {
return node;
}
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in print statement");
return node;
}
node->data._print.value = mod_src_parser_flo_parse_expr(ps, 0);
mod_src_parser_flo_parser_expect(ps, TOKEN_RPAREN);
return node;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_PRUNE) {
mod_src_parser_flo_parser_advance(ps);
AST* node = mod_src_parser_flo_make_node(ps, AST_PRUNE);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in prune statement");
return node;
}
node->data._prune_free.ptr = mod_src_parser_flo_parse_expr(ps, 0);
return node;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_AT) {
mod_src_parser_flo_parser_advance(ps);
AST* node = mod_src_parser_flo_make_node(ps, AST_DEREF_ASS);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in dereference statement");
return node;
}
Token* name = mod_src_parser_flo_parser_advance(ps);
if (name->kind != TOKEN_IDENTIFIER) {
mod_src_parser_flo_parser_error(ps, "Expected identifier after '@'");
return node;
}
node->data._deref_ass.name_length = name->length;
node->data._deref_ass.name_start = name->start;
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_ASSIGN))) {
return node;
}
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in dereference assignment");
return node;
}
node->data._deref_ass.value = mod_src_parser_flo_parse_expr(ps, 0);
return node;
}
else {
char message[128];
snprintf(message, sizeof(message), "Unexpected token while parsing statement: %s", mod_src_lexer_flo_token_kind_name(mod_src_parser_flo_parser_peek(ps)->kind));
mod_src_parser_flo_parser_error(ps, message);
mod_src_parser_flo_parser_advance(ps);
return NULL;
}
}


AST* mod_src_parser_flo_parse_primary(Parser* ps) {
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file while parsing primary");
return NULL;
}
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_IDENTIFIER) {
if (mod_src_lexer_flo_peek(ps->ts, ps->pos + 1)->kind == TOKEN_LPAREN) {
AST* node = mod_src_parser_flo_parse_func_call(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_DOT) {
mod_src_parser_flo_parser_advance(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file after '.'");
return NULL;
}
Token* field = mod_src_parser_flo_parser_advance(ps);
if (field->kind != TOKEN_IDENTIFIER) {
mod_src_parser_flo_parser_error(ps, "Expected identifier name after '.'");
return node;
}
AST* dot = mod_src_parser_flo_make_node(ps, AST_DOT_ACCESS);
dot->data._dot_access.object = node;
dot->data._dot_access.field_length = field->length;
dot->data._dot_access.field_start = field->start;
return dot;
}
return node;
}
if (mod_src_lexer_flo_peek(ps->ts, ps->pos + 1)->kind == TOKEN_DOT  &&  mod_src_parser_flo_is_alias(ps, mod_src_parser_flo_parser_peek(ps))) {
return mod_src_parser_flo_parse_alias_call(ps);
}
Token* tok = mod_src_parser_flo_parser_peek(ps);
mod_src_parser_flo_parser_advance(ps);
AST* node = mod_src_parser_flo_make_node(ps, AST_VAR_REF);
node->data._var_ref.name_start = tok->start;
node->data._var_ref.name_length = tok->length;
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_AT) {
mod_src_parser_flo_parser_advance(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file after '@'");
return node;
}
AST* operand = mod_src_parser_flo_parse_primary(ps);
AST* drf = mod_src_parser_flo_make_node(ps, AST_DEREF);
drf->data._unary.operand = operand;
return drf;
}
while (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_DOT  ||  mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_LBRACK) {
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_DOT) {
mod_src_parser_flo_parser_advance(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file after '.'");
return node;
}
Token* field = mod_src_parser_flo_parser_advance(ps);
if (field->kind != TOKEN_IDENTIFIER) {
mod_src_parser_flo_parser_error(ps, "Expected identifier name after '.'");
return node;
}
AST* dot = mod_src_parser_flo_make_node(ps, AST_DOT_ACCESS);
dot->data._dot_access.object = node;
dot->data._dot_access.field_start = field->start;
dot->data._dot_access.field_length = field->length;
node = dot;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_LBRACK) {
mod_src_parser_flo_parser_advance(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in subscript");
return node;
}
AST* idx = mod_src_parser_flo_parse_expr(ps, 0);
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_RBRACK))) {
return node;
}
AST* sub = mod_src_parser_flo_make_node(ps, AST_SUBSCRIPT);
sub->data._subscript.array = node;
sub->data._subscript.index = idx;
node = sub;
}
else {
break;
}
}
return node;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_NUMBER) {
AST* lit = mod_src_parser_flo_make_node(ps, AST_LITERAL);
lit->value = atoi(ps->src + mod_src_parser_flo_parser_peek(ps)->start);
mod_src_parser_flo_parser_advance(ps);
return lit;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_FLOAT_LIT) {
AST* lit = mod_src_parser_flo_make_node(ps, AST_FLOAT_LIT);
lit->data._float_lit.start = mod_src_parser_flo_parser_peek(ps)->start;
lit->data._float_lit.length = mod_src_parser_flo_parser_peek(ps)->length;
mod_src_parser_flo_parser_advance(ps);
return lit;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_TRUE  ||  mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_FALSE) {
AST* lit = mod_src_parser_flo_make_node(ps, AST_BOOL_LIT);
int val = 0;
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_TRUE) {
val = 1;
}
lit->data._bool_lit.value = val;
mod_src_parser_flo_parser_advance(ps);
return lit;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_NULL) {
AST* nll = mod_src_parser_flo_make_node(ps, AST_NULL);
mod_src_parser_flo_parser_advance(ps);
return nll;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_STRING_LIT) {
AST* str = mod_src_parser_flo_make_node(ps, AST_STRING_LIT);
str->data._string.str_start = mod_src_parser_flo_parser_peek(ps)->start;
str->data._string.str_length = mod_src_parser_flo_parser_peek(ps)->length;
str->data._string.lower_kind = STRING_LOWER_STRING;
mod_src_parser_flo_parser_advance(ps);
return str;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_LBRACK) {
mod_src_parser_flo_parser_advance(ps);
AST* arr = mod_src_parser_flo_make_node(ps, AST_ARRAY_LIT);
AST* elem_head = NULL;
AST* elem_tail = NULL;
while (mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_RBRACK) {
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in array literal");
return arr;
}
AST* elem = mod_src_parser_flo_parse_expr(ps, 0);
if (elem == NULL) {
return arr;
}
if (elem_head == NULL) {
elem_head = elem;
}
else {
elem_tail->next = elem;
}
elem_tail = elem;
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_COMMA) {
mod_src_parser_flo_parser_advance(ps);
}
else if (mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_RBRACK) {
mod_src_parser_flo_parser_error(ps, "Expected ',' or ']' in array literal");
return arr;
}
}
mod_src_parser_flo_parser_expect(ps, TOKEN_RBRACK);
arr->data._array.elements = elem_head;
return arr;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_LBRACE) {
mod_src_parser_flo_parser_advance(ps);
AST* strct_lit = mod_src_parser_flo_make_node(ps, AST_STRUCT_LIT);
AST* struct_head = NULL;
AST* struct_tail = NULL;
while (mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_RBRACE) {
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in struct literal");
return strct_lit;
}
AST* elem = mod_src_parser_flo_parse_expr(ps, 0);
if (struct_head == NULL) {
struct_head = elem;
}
else {
struct_tail->next = elem;
}
struct_tail = elem;
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_COMMA) {
mod_src_parser_flo_parser_advance(ps);
}
}
mod_src_parser_flo_parser_expect(ps, TOKEN_RBRACE);
strct_lit->data._struct_lit.elements = struct_head;
return strct_lit;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_LPAREN) {
mod_src_parser_flo_parser_advance(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in parenthesized expression");
return NULL;
}
AST* expr = mod_src_parser_flo_parse_expr(ps, 0);
mod_src_parser_flo_parser_expect(ps, TOKEN_RPAREN);
return expr;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_NEW) {
mod_src_parser_flo_parser_advance(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file after 'new'");
return NULL;
}
AST* new_node = mod_src_parser_flo_make_node(ps, AST_NEW);
new_node->data._new_alloc.type = *(mod_src_parser_flo_parse_type(ps));
return new_node;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_NOT) {
mod_src_parser_flo_parser_advance(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file after 'not'");
return NULL;
}
AST* operand = mod_src_parser_flo_parse_primary(ps);
AST* unry = mod_src_parser_flo_make_node(ps, AST_UNARY_NOT);
unry->data._unary.operand = operand;
return unry;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_MINUS) {
mod_src_parser_flo_parser_advance(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file after unary '-' operator");
return NULL;
}
AST* operand = mod_src_parser_flo_parse_primary(ps);
AST* unry = mod_src_parser_flo_make_node(ps, AST_UNARY_NEG);
unry->data._unary.operand = operand;
return unry;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_AT) {
mod_src_parser_flo_parser_advance(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file after '@'");
return NULL;
}
AST* operand = mod_src_parser_flo_parse_primary(ps);
AST* deref = mod_src_parser_flo_make_node(ps, AST_DEREF);
deref->data._deref.operand = operand;
return deref;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_AMPERSAND) {
mod_src_parser_flo_parser_advance(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file after '&'");
return NULL;
}
AST* operand = mod_src_parser_flo_parse_primary(ps);
AST* get = mod_src_parser_flo_make_node(ps, AST_GET_ADDR);
get->data._get_addr.operand = operand;
return get;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_SIZEOF) {
mod_src_parser_flo_parser_advance(ps);
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_LPAREN))) {
return NULL;
}
AST* node = mod_src_parser_flo_make_node(ps, AST_SIZEOF);
node->data._size_of.type = *(mod_src_parser_flo_parse_type(ps));
mod_src_parser_flo_parser_expect(ps, TOKEN_RPAREN);
return node;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_CHAR_LIT) {
AST* node = mod_src_parser_flo_make_node(ps, AST_CHAR_LIT);
node->data._char_lit.start = mod_src_parser_flo_parser_peek(ps)->start;
node->data._char_lit.length = mod_src_parser_flo_parser_peek(ps)->length;
mod_src_parser_flo_parser_advance(ps);
return node;
}
else {
char message[128];
snprintf(message, sizeof(message), "parse_primary: unexpected token kind: %s", mod_src_lexer_flo_token_kind_name(mod_src_parser_flo_parser_peek(ps)->kind));
return NULL;
}
}


AST* mod_src_parser_flo_parse_func_def(Parser* ps) {
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_FUNC))) {
return NULL;
}
AST* node = mod_src_parser_flo_make_node(ps, AST_FUNC_DEF);
Token* name = mod_src_parser_flo_parser_advance(ps);
if (name->kind != TOKEN_IDENTIFIER) {
mod_src_parser_flo_parser_error(ps, "Expected function name");
return node;
}
node->data._func_def.name_start = name->start;
node->data._func_def.name_length = name->length;
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_LPAREN))) {
return node;
}
AST* param_head = NULL;
AST* param_tail = NULL;
while (mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_RPAREN) {
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in function definition parameters");
break;
}
AST* param = mod_src_parser_flo_parse_param(ps);
if (param_head == NULL) {
param_head = param;
}
else {
param_tail->next = param;
}
param_tail = param;
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_COMMA) {
mod_src_parser_flo_parser_advance(ps);
}
}
mod_src_parser_flo_parser_expect(ps, TOKEN_RPAREN);
node->data._func_def.params = param_head;
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_COLON))) {
return node;
}
node->data._func_def.return_type = *(mod_src_parser_flo_parse_type(ps));
AST* body_head = NULL;
AST* body_tail = NULL;
while (mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_END) {
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in function body");
break;
}
mod_src_parser_flo_parser_skip_newline(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_END) {
break;
}
AST* statement = mod_src_parser_flo_parse_statement(ps);
if (body_head == NULL) {
body_head = statement;
}
else {
body_tail->next = statement;
}
body_tail = statement;
}
mod_src_parser_flo_parser_expect(ps, TOKEN_END);
node->data._func_def.body = body_head;
return node;
}


AST* mod_src_parser_flo_parse_func_call(Parser* ps) {
AST* node = mod_src_parser_flo_make_node(ps, AST_FUNC_CALL);
Token* name = mod_src_parser_flo_parser_advance(ps);
node->data._func_call.name_start = name->start;
node->data._func_call.name_length = name->length;
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_LPAREN))) {
return node;
}
AST* arg_head = NULL;
AST* arg_tail = NULL;
while (mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_RPAREN) {
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in function call arguments");
return node;
}
AST* param = mod_src_parser_flo_parse_expr(ps, 0);
if (param == NULL) {
mod_src_parser_flo_parser_sync_until(ps, TOKEN_COMMA, TOKEN_RPAREN);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_COMMA) {
mod_src_parser_flo_parser_advance(ps);
continue;
}
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_RPAREN) {
break;
}
return node;
}
if (arg_head == NULL) {
arg_head = param;
}
else {
arg_tail->next = param;
}
arg_tail = param;
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_COMMA) {
mod_src_parser_flo_parser_advance(ps);
}
else if (mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_RPAREN) {
mod_src_parser_flo_parser_error(ps, "Expected ',' or ')' in function call arguments");
mod_src_parser_flo_parser_sync_until(ps, TOKEN_COMMA, TOKEN_RPAREN);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_COMMA) {
mod_src_parser_flo_parser_advance(ps);
}
else if (mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_RPAREN) {
return node;
}
}
}
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_RPAREN))) {
return node;
}
node->data._func_call.args = arg_head;
return node;
}


AST* mod_src_parser_flo_parse_alias_call(Parser* ps) {
AST* node = mod_src_parser_flo_make_node(ps, AST_ALIAS_CALL);
Token* alias = mod_src_parser_flo_parser_advance(ps);
node->data._alias_call.alias_start = alias->start;
node->data._alias_call.alias_length = alias->length;
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_DOT))) {
return node;
}
Token* fnc = mod_src_parser_flo_parser_advance(ps);
node->data._alias_call.func_start = fnc->start;
node->data._alias_call.func_length = fnc->length;
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_LPAREN))) {
return node;
}
AST* arg_head = NULL;
AST* arg_tail = NULL;
while (mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_RPAREN) {
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in function alias arguments");
return node;
}
AST* arg = mod_src_parser_flo_parse_expr(ps, 0);
if (arg == NULL) {
mod_src_parser_flo_parser_sync_until(ps, TOKEN_COMMA, TOKEN_RPAREN);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_COMMA) {
mod_src_parser_flo_parser_advance(ps);
continue;
}
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_RPAREN) {
break;
}
return node;
}
if (arg_head == NULL) {
arg_head = arg;
}
else {
arg_tail->next = arg;
}
arg_tail = arg;
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_COMMA) {
mod_src_parser_flo_parser_advance(ps);
}
else if (mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_RPAREN) {
mod_src_parser_flo_parser_error(ps, "Expected ',' or ')' in function alias arguments");
mod_src_parser_flo_parser_sync_until(ps, TOKEN_COMMA, TOKEN_RPAREN);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_COMMA) {
mod_src_parser_flo_parser_advance(ps);
}
else if (mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_RPAREN) {
return node;
}
}
}
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_RPAREN))) {
return node;
}
node->data._alias_call.args = arg_head;
return node;
}


AST* mod_src_parser_flo_parse_param(Parser* ps) {
AST* node = mod_src_parser_flo_make_node(ps, AST_PARAM);
Token* name = mod_src_parser_flo_parser_advance(ps);
node->data._func_params.name_start = name->start;
node->data._func_params.name_length = name->length;
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_COLON))) {
return node;
}
node->data._func_params.type = *(mod_src_parser_flo_parse_type(ps));
return node;
}


AST* mod_src_parser_flo_parse_struct(Parser* ps) {
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_STRUCT))) {
return NULL;
}
AST* node = mod_src_parser_flo_make_node(ps, AST_STRUCT_DEF);
Token* name = mod_src_parser_flo_parser_advance(ps);
if (name->kind != TOKEN_IDENTIFIER) {
mod_src_parser_flo_parser_error(ps, "Expected struct name");
return NULL;
}
node->data._struct_def.name_start = name->start;
node->data._struct_def.name_length = name->length;
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_LBRACE))) {
return NULL;
}
AST* field_head = NULL;
AST* field_tail = NULL;
while (mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_RBRACE) {
while (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_NEWLINE  ||  mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_COMMA  ||  mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_SEMI) {
mod_src_parser_flo_parser_advance(ps);
}
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_RBRACE) {
break;
}
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in struct body");
return node;
}
AST* field = mod_src_parser_flo_make_node(ps, AST_STRUCT_FIELD);
mod_src_parser_flo_parse_field_flags(ps, field);
Token* fname = mod_src_parser_flo_parser_advance(ps);
if (fname->kind != TOKEN_IDENTIFIER) {
mod_src_parser_flo_parser_error(ps, "Expected field name");
continue;
}
field->data._struct_field.name_start = fname->start;
field->data._struct_field.name_length = fname->length;
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_COLON))) {
continue;
}
field->data._struct_field.type = *(mod_src_parser_flo_parse_type(ps));
if (field_head == NULL) {
field_head = field;
}
else {
field_tail->next = field;
}
field_tail = field;
}
mod_src_parser_flo_parser_expect(ps, TOKEN_RBRACE);
node->data._struct_def.fields = field_head;
return node;
}


AST* mod_src_parser_flo_parse_union(Parser* ps) {
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_UNION))) {
return NULL;
}
AST* node = mod_src_parser_flo_make_node(ps, AST_UNION_DEF);
Token* name = mod_src_parser_flo_parser_advance(ps);
if (name->kind != TOKEN_IDENTIFIER) {
mod_src_parser_flo_parser_error(ps, "Expected union name");
return NULL;
}
node->data._union_def.name_start = name->start;
node->data._union_def.name_length = name->length;
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_LBRACE))) {
return NULL;
}
AST* field_head = NULL;
AST* field_tail = NULL;
while (mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_RBRACE) {
while (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_NEWLINE  ||  mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_COMMA  ||  mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_SEMI) {
mod_src_parser_flo_parser_advance(ps);
}
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_RBRACE) {
break;
}
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_RBRACE) {
break;
}
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
mod_src_parser_flo_parser_error(ps, "Unexpected end of file in union body");
return node;
}
AST* field = mod_src_parser_flo_make_node(ps, AST_STRUCT_FIELD);
mod_src_parser_flo_parse_field_flags(ps, field);
Token* fname = mod_src_parser_flo_parser_advance(ps);
if (fname->kind != TOKEN_IDENTIFIER) {
mod_src_parser_flo_parser_error(ps, "Expected field name");
continue;
}
field->data._struct_field.name_start = fname->start;
field->data._struct_field.name_length = fname->length;
if (!(mod_src_parser_flo_parser_expect(ps, TOKEN_COLON))) {
continue;
}
field->data._struct_field.type = *(mod_src_parser_flo_parse_type(ps));
if (field_head == NULL) {
field_head = field;
}
else {
field_tail->next = field;
}
field_tail = field;
}
mod_src_parser_flo_parser_expect(ps, TOKEN_RBRACE);
node->data._struct_def.fields = field_head;
return node;
}


AST* mod_src_parser_flo_parse_dot_ass(Parser* ps) {
AST* expr = mod_src_parser_flo_parse_expr(ps, 0);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_ASSIGN) {
mod_src_parser_flo_parser_advance(ps);
AST* val = mod_src_parser_flo_parse_expr(ps, 0);
if (expr->kind == AST_DOT_ACCESS) {
expr->data._dot_access.value = val;
}
else if (expr->kind == AST_SUBSCRIPT) {
expr->data._subscript.value = val;
}
else {
mod_src_parser_flo_parser_error(ps, "Invalid assignment target");
}
}
return expr;
}


AST* mod_src_parser_flo_parse_import(Parser* ps) {
mod_src_parser_flo_parser_expect(ps, TOKEN_IMPORT);
AST* node = mod_src_parser_flo_make_node(ps, AST_IMPORT);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_LT) {
mod_src_parser_flo_parser_advance(ps);
Token* path = mod_src_parser_flo_parser_advance(ps);
node->data._import.path_start = path->start;
node->data._import.path_length = path->length;
mod_src_parser_flo_parser_expect(ps, TOKEN_GT);
node->data._import.is_system = 1;
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_AS) {
mod_src_parser_flo_parser_advance(ps);
Token* alias = mod_src_parser_flo_parser_advance(ps);
node->data._import.alias_start = alias->start;
node->data._import.alias_length = alias->length;
node->data._import.has_alias = 1;
ps->alias_start[ps->alias_count] = alias->start;
ps->alias_lengths[ps->alias_count] = alias->length;
ps->alias_count = ps->alias_count + 1;
}
else {
node->data._import.has_alias = 0;
}
}
else {
Token* path = mod_src_parser_flo_parser_advance(ps);
node->data._import.path_start = path->start;
node->data._import.path_length = path->length;
node->data._import.is_system = 0;
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_AS) {
mod_src_parser_flo_parser_advance(ps);
Token* alias = mod_src_parser_flo_parser_advance(ps);
node->data._import.alias_start = alias->start;
node->data._import.alias_length = alias->length;
node->data._import.has_alias = 1;
ps->alias_start[ps->alias_count] = alias->start;
ps->alias_lengths[ps->alias_count] = alias->length;
ps->alias_count = ps->alias_count + 1;
}
else {
node->data._import.has_alias = 0;
}
}
return node;
}


AST* mod_src_parser_flo_parse_forward(Parser* ps) {
mod_src_parser_flo_parser_expect(ps, TOKEN_FORWARD);
AST* node = mod_src_parser_flo_make_node(ps, AST_FORWARD);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_STRUCT) {
mod_src_parser_flo_parser_advance(ps);
Token* name = mod_src_parser_flo_parser_advance(ps);
node->data._forward.name_start = name->start;
node->data._forward.name_length = name->length;
node->data._forward.is_func = 0;
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_FUNC) {
mod_src_parser_flo_parser_advance(ps);
Token* name = mod_src_parser_flo_parser_advance(ps);
node->data._forward.name_start = name->start;
node->data._forward.name_length = name->length;
mod_src_parser_flo_parser_expect(ps, TOKEN_LPAREN);
AST* param_head = NULL;
AST* param_tail = NULL;
while (mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_RPAREN) {
AST* param = mod_src_parser_flo_parse_param(ps);
if (param_head == NULL) {
param_head = param;
}
else {
param_tail->next = param;
}
param_tail = param;
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_COMMA) {
mod_src_parser_flo_parser_advance(ps);
}
}
mod_src_parser_flo_parser_expect(ps, TOKEN_RPAREN);
node->data._forward.params = param_head;
mod_src_parser_flo_parser_expect(ps, TOKEN_COLON);
node->data._forward.return_type = *(mod_src_parser_flo_parse_type(ps));
node->data._forward.is_func = 1;
}
else {
mod_src_parser_flo_parser_error(ps, "Expected either struct or function declaration");
return node;
}
return node;
}


AST* mod_src_parser_flo_parse(Parser* ps) {
AST* head = NULL;
AST* tail = NULL;
while (mod_src_parser_flo_parser_peek(ps)->kind != TOKEN_EOF) {
if (ps->error_count >= 10) {
printf("%sToo many errors, exiting.%s\n", RED, RESET);
break;
}
mod_src_parser_flo_parser_skip_newline(ps);
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_EOF) {
break;
}
AST* node = NULL;
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_IMPORT) {
node = mod_src_parser_flo_parse_import(ps);
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_STRUCT) {
node = mod_src_parser_flo_parse_struct(ps);
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_UNION) {
node = mod_src_parser_flo_parse_union(ps);
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_PROP) {
mod_src_parser_flo_parser_advance(ps);
AST* decl = NULL;
if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_FUNC) {
decl = mod_src_parser_flo_parse_func_def(ps);
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_STRUCT) {
decl = mod_src_parser_flo_parse_struct(ps);
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_UNION) {
decl = mod_src_parser_flo_parse_union(ps);
}
else {
mod_src_parser_flo_parser_error(ps, "Expected declaration after prop");
}
if (decl != NULL) {
AST* prop_node = mod_src_parser_flo_make_node(ps, AST_PROP);
prop_node->data._prop.decl = decl;
node = prop_node;
}
else {
node = NULL;
}
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_FUNC) {
node = mod_src_parser_flo_parse_func_def(ps);
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_FORWARD) {
node = mod_src_parser_flo_parse_forward(ps);
}
else if (mod_src_parser_flo_parser_peek(ps)->kind == TOKEN_IDENTIFIER  &&  mod_src_lexer_flo_peek(ps->ts, ps->pos + 1)->kind == TOKEN_COLON) {
node = mod_src_parser_flo_parse_var_decl(ps);
}
else {
mod_src_parser_flo_parser_error(ps, "Unexpected top-level statement or declaration");
mod_src_parser_flo_parser_sync_until(ps, TOKEN_NEWLINE, TOKEN_EOF);
}
if (node != NULL) {
if (head == NULL) {
head = node;
}
else {
tail->next = node;
}
tail = node;
}
}
return head;
}


char* mod_src_file_flo_read_file(char* path) {
FILE* f = fopen(path, "r");
if (f == NULL) {
return NULL;
}
fseek(f, 0, 2);
int length = ftell(f);
fseek(f, 0, 0);
char* buffer = malloc(sizeof(char) * (length + 1));
fread(buffer, 1, length, f);
buffer[length] = '\0';
fclose(f);
return buffer;
}
typedef struct Module Module;
int EXPORT_FUNC = 1;
int EXPORT_STRUCT = 2;
int EXPORT_UNION = 3;


typedef struct ExportInfo {
int kind;
int name_start;
int name_length;
char* name_src;
} ExportInfo;


typedef struct Module {
char* path;
char* src;
TokenStream* tokens;
AST* ast;
char* symbol_prefix;
ExportInfo exports[256];
int export_count;
} Module;


typedef struct ModuleSet {
Module modules[128];
int count;
} ModuleSet;


void mod_src_module_flo_init_modules(ModuleSet* set) {
set->count = 0;
}
Module* mod_src_module_flo_find_module(ModuleSet* set, char* path);
ExportInfo* mod_src_module_flo_find_export(Module* module, char* src, int start, int length, int kind);
char* mod_src_module_flo_import_path_from_ast(AST* import_ast, char* src, char* current_path);
void mod_src_module_flo_add_export(Module* module, int kind, int name_start, int name_length, char* name_src);
void mod_src_module_flo_collect_exports(Module* module);
void mod_src_module_flo_load_imports(ModuleSet* set, Module* module);
Module* mod_src_module_flo_load_module(ModuleSet* set, char* path);
char* mod_src_module_flo_build_symbol_prefix(char* path);
Module* mod_src_module_flo_find_imported_module(ModuleSet* set, Module* module, char* src, int start, int length);
int mod_src_module_flo_module_defines_func(Module* module, char* src, int start, int length);
int mod_src_module_flo_module_exports_func(Module* module, char* src, int start, int length);
Module* mod_src_module_flo_find_function_module(ModuleSet* set, char* src, int start, int length);


Module* mod_src_module_flo_find_module(ModuleSet* set, char* path) {
for (int i = 0; ((0) > (set->count)) ? i > (set->count) : i < (set->count); ((0) > (set->count)) ? i-- : i++) {
Module* m = set->modules + i;
if (!(strcmp(m->path, path))) {
return m;
}
}
return NULL;
}


ExportInfo* mod_src_module_flo_find_export(Module* module, char* src, int start, int length, int kind) {
for (int i = 0; ((0) > (module->export_count)) ? i > (module->export_count) : i < (module->export_count); ((0) > (module->export_count)) ? i-- : i++) {
ExportInfo* info = module->exports + i;
if (kind != 0  &&  info->kind != kind) {
continue;
}
if (info->name_length == length  &&  !(strncmp(info->name_src + info->name_start, src + start, length))) {
return info;
}
}
return NULL;
}


char* mod_src_module_flo_build_symbol_prefix(char* path) {
char* rel = path;
int root_len = strlen(project_root);
if (root_len > 0  &&  !(strncmp(path, project_root, root_len))) {
rel = path + root_len;
if (rel[0] == '/') {
rel = rel + 1;
}
}
else if (path[0] == '.'  &&  path[1] == '/') {
rel = path + 2;
}
else {
char* slash = strrchr(path, '/');
if (slash != NULL) {
rel = slash + 1;
}
}
char out[512];
strcpy(out, "mod_");
int i = 0;
int j = 4;
while (rel[i] != '\0'  &&  j < sizeof(out) - 1) {
char c = rel[i];
if (isalnum(c)) {
out[j] = c;
}
else {
out[j] = '_';
}
i = i + 1;
j = j + 1;
}
out[j] = '\0';
return strdup(out);
}


Module* mod_src_module_flo_find_imported_module(ModuleSet* set, Module* module, char* src, int start, int length) {
AST* curr = module->ast;
while (curr != NULL) {
if (curr->kind == AST_IMPORT  &&  curr->data._import.has_alias) {
if (curr->data._import.alias_length == length  &&  !(strncmp(module->src + curr->data._import.alias_start, src + start, length))) {
char* path = mod_src_module_flo_import_path_from_ast(curr, module->src, module->path);
if (path == NULL) {
return NULL;
}
Module* imported = mod_src_module_flo_find_module(set, path);
free(path);
return imported;
}
}
curr = curr->next;
}
return NULL;
}


int mod_src_module_flo_module_defines_func(Module* module, char* src, int start, int length) {
AST* curr = module->ast;
while (curr != NULL) {
if (curr->kind == AST_FUNC_DEF) {
if (curr->data._func_def.name_length == length  &&  !(strncmp(module->src + curr->data._func_def.name_start, src + start, length))) {
return 1;
}
}
else if (curr->kind == AST_PROP  &&  curr->data._prop.decl != NULL) {
if (curr->data._prop.decl->kind == AST_FUNC_DEF) {
if (curr->data._prop.decl->data._func_def.name_length == length  &&  !(strncmp(module->src + curr->data._prop.decl->data._func_def.name_start, src + start, length))) {
return 1;
}
}
}
curr = curr->next;
}
return 0;
}


int mod_src_module_flo_module_exports_func(Module* module, char* src, int start, int length) {
return mod_src_module_flo_find_export(module, src, start, length, EXPORT_FUNC) != NULL;
}


Module* mod_src_module_flo_find_function_module(ModuleSet* set, char* src, int start, int length) {
for (int i = 0; ((0) > (set->count)) ? i > (set->count) : i < (set->count); ((0) > (set->count)) ? i-- : i++) {
Module* m = set->modules + i;
if (mod_src_module_flo_module_defines_func(m, src, start, length)) {
return m;
}
}
return NULL;
}


void mod_src_module_flo_add_export(Module* module, int kind, int name_start, int name_length, char* name_src) {
if (module->export_count >= 256) {
printf("%sToo many exports in module: %s%s", RED, RESET, module->path);
return;}
ExportInfo* info = module->exports + module->export_count;
info->kind = kind;
info->name_start = name_start;
info->name_length = name_length;
info->name_src = name_src;
module->export_count = module->export_count + 1;
}


void mod_src_module_flo_collect_exports(Module* module) {
module->export_count = 0;
AST* curr = module->ast;
while (curr != NULL) {
if (curr->kind == AST_PROP  &&  curr->data._prop.decl != NULL) {
AST* decl = curr->data._prop.decl;
if (decl->kind == AST_FUNC_DEF) {
mod_src_module_flo_add_export(module, EXPORT_FUNC, decl->data._func_def.name_start, decl->data._func_def.name_length, module->src);
}
else if (decl->kind == AST_STRUCT_DEF) {
mod_src_module_flo_add_export(module, EXPORT_STRUCT, decl->data._struct_def.name_start, decl->data._struct_def.name_length, module->src);
}
else if (decl->kind == AST_UNION_DEF) {
mod_src_module_flo_add_export(module, EXPORT_UNION, decl->data._union_def.name_start, decl->data._union_def.name_length, module->src);
}
}
curr = curr->next;
}
}


char* mod_src_module_flo_import_path_from_ast(AST* import_ast, char* src, char* current_path) {
if (import_ast->data._import.is_system) {
return NULL;
}
char raw_path[256];
snprintf(raw_path, sizeof(raw_path), "%.*s", import_ast->data._import.path_length - 2, src + import_ast->data._import.path_start + 1);
return mod_src_globals_flo_resolve_path(current_path, raw_path);
}


void mod_src_module_flo_load_imports(ModuleSet* set, Module* module) {
AST* curr = module->ast;
while (curr != NULL) {
if (curr->kind == AST_IMPORT  &&  !(curr->data._import.is_system)) {
char* path = mod_src_module_flo_import_path_from_ast(curr, module->src, module->path);
if (path != NULL) {
mod_src_module_flo_load_module(set, path);
free(path);
}
}
curr = curr->next;
}
}


Module* mod_src_module_flo_load_module(ModuleSet* set, char* path) {
Module* existing = mod_src_module_flo_find_module(set, path);
if (existing != NULL) {
return existing;
}
char* src = mod_src_file_flo_read_file(path);
if (src == NULL) {
printf("%sCould not import file: %s%s\n", RED, path, RESET);
return NULL;
}
Module* m = set->modules + set->count;
m->path = strdup(path);
m->src = src;
m->symbol_prefix = mod_src_module_flo_build_symbol_prefix(path);
m->export_count = 0;
m->tokens = malloc(sizeof(TokenStream));
mod_src_lexer_flo_init_token_stream(m->tokens);
mod_src_lexer_flo_lex(m->tokens, src);
Parser p;
mod_src_parser_flo_init_parser(&(p), m->tokens, src, m->path);
m->ast = mod_src_parser_flo_parse(&(p));
mod_src_module_flo_collect_exports(m);
set->count = set->count + 1;
mod_src_module_flo_load_imports(set, m);
return m;
}
typedef struct StructInfo StructInfo;
typedef struct FieldInfo FieldInfo;
typedef struct VarInfo VarInfo;
typedef struct FuncInfo FuncInfo;


typedef struct FieldInfo {
char* src;
int name_start;
int name_length;
TypeInfo type;
int is_hidden;
int is_frozen;
int is_readonly;
} FieldInfo;


typedef struct StructInfo {
char* src;
int name_start;
int name_length;
FieldInfo fields[128];
int field_count;
} StructInfo;


typedef struct VarInfo {
char* src;
int name_start;
int name_length;
TypeInfo type;
} VarInfo;


typedef struct FuncInfo {
char* src;
int name_start;
int name_length;
AST* params;
TypeInfo return_type;
} FuncInfo;


typedef struct TypeEnv {
char* src;
StructInfo structs[1024];
int struct_count;
VarInfo vars[2048];
int var_count;
FuncInfo funcs[1024];
int func_count;
int error_count;
} TypeEnv;
ModuleSet* active_type_modules;
Module* current_type_module;
TypeInfo current_return_type;
int inside_function = 0;


int mod_src_typecheck_flo_same_name(char* a_src, int a_start, int a_len, char* b_src, int b_start, int b_len) {
if (a_len != b_len) {
return 0;
}
if (!(strncmp(a_src + a_start, b_src + b_start, a_len))) {
return 1;
}
return 0;
}


void mod_src_typecheck_flo_copy_type(TypeInfo* dst, TypeInfo* src_type) {
dst->base = src_type->base;
dst->pointer_depth = src_type->pointer_depth;
dst->array_size = src_type->array_size;
dst->arr_size_expr = src_type->arr_size_expr;
dst->name_start = src_type->name_start;
dst->name_length = src_type->name_length;
dst->name_src = src_type->name_src;
}
int mod_src_typecheck_flo_types_match(TypeInfo* expected, TypeInfo* actual);


int mod_src_typecheck_flo_is_bool_type(TypeInfo* type) {
if (type->base == TOKEN_BOOL  &&  type->pointer_depth == 0  &&  type->array_size == 0) {
return 1;
}
return 0;
}


int mod_src_typecheck_flo_is_void_type(TypeInfo* type) {
return type->base == TOKEN_VOID  &&  type->pointer_depth == 0  &&  type->array_size == 0;
}


int mod_src_typecheck_flo_is_null_type(TypeInfo* type) {
return type->base == TOKEN_VOID  &&  type->pointer_depth == 1  &&  type->array_size == 0;
}


int mod_src_typecheck_flo_is_integer_type(TypeInfo* type) {
return type->base == TOKEN_INT  ||  type->base == TOKEN_CHAR  &&  type->pointer_depth == 0  &&  type->array_size == 0;
}


int mod_src_typecheck_flo_is_decimal_type(TypeInfo* type) {
return type->base == TOKEN_FLOAT  ||  type->base == TOKEN_DOUBLE  &&  type->pointer_depth == 0  &&  type->array_size == 0;
}


int mod_src_typecheck_flo_is_pointer_like_type(TypeInfo* type) {
return type->pointer_depth > 0  ||  type->array_size != 0;
}


int mod_src_typecheck_flo_is_condition_type(TypeInfo* type) {
return mod_src_typecheck_flo_is_bool_type(type)  ||  mod_src_typecheck_flo_is_integer_type(type)  ||  mod_src_typecheck_flo_is_pointer_like_type(type);
}


int mod_src_typecheck_flo_is_string_type(TypeInfo* type) {
return type->base == TOKEN_STRING  &&  type->pointer_depth == 0  &&  type->array_size == 0;
}


int mod_src_typecheck_flo_is_cstr_type(TypeInfo* type) {
return type->base == TOKEN_CHAR  &&  type->pointer_depth == 1  &&  type->array_size == 0;
}


int mod_src_typecheck_flo_is_numeric_type(TypeInfo* type) {
return type->base == TOKEN_INT  ||  type->base == TOKEN_FLOAT  ||  type->base == TOKEN_DOUBLE  &&  type->pointer_depth == 0  &&  type->array_size == 0;
}


int mod_src_typecheck_flo_numeric_rank(TypeInfo* type) {
if (type->base == TOKEN_INT) {
return 1;
}
if (type->base == TOKEN_FLOAT) {
return 2;
}
if (type->base == TOKEN_DOUBLE) {
return 3;
}
return 0;
}


int mod_src_typecheck_flo_can_widen_numeric(TypeInfo* expected, TypeInfo* actual) {
if (!(mod_src_typecheck_flo_is_numeric_type(expected))  ||  !(mod_src_typecheck_flo_is_numeric_type(actual))) {
return 0;
}
return mod_src_typecheck_flo_numeric_rank(actual) <= mod_src_typecheck_flo_numeric_rank(expected);
}


int mod_src_typecheck_flo_can_implicitly_convert(TypeInfo* expected, TypeInfo* actual) {
if (mod_src_typecheck_flo_types_match(expected, actual)) {
return 1;
}
if (mod_src_typecheck_flo_can_widen_numeric(expected, actual)) {
return 1;
}
return 0;
}


int mod_src_typecheck_flo_can_explicitly_convert(TypeInfo* target, TypeInfo* value) {
if (mod_src_typecheck_flo_types_match(target, value)) {
return 1;
}
if (mod_src_typecheck_flo_is_numeric_type(target)  &&  mod_src_typecheck_flo_is_numeric_type(value)) {
return 1;
}
if (mod_src_typecheck_flo_is_bool_type(target)  &&  value->base == TOKEN_INT  &&  value->pointer_depth == 0  &&  value->array_size == 0) {
return 1;
}
if (mod_src_typecheck_flo_is_bool_type(value)  &&  target->base == TOKEN_INT  &&  target->pointer_depth == 0  &&  target->array_size == 0) {
return 1;
}
if (mod_src_typecheck_flo_is_string_type(target)  &&  mod_src_typecheck_flo_is_cstr_type(value)) {
return 1;
}
if (mod_src_typecheck_flo_is_cstr_type(target)  &&  mod_src_typecheck_flo_is_string_type(value)) {
return 1;
}
return 0;
}


int mod_src_typecheck_flo_can_convert_either_way(TypeInfo* left, TypeInfo* right) {
return mod_src_typecheck_flo_can_implicitly_convert(left, right)  ||  mod_src_typecheck_flo_can_implicitly_convert(right, left);
}


int mod_src_typecheck_flo_allow_string_literal_for_target(AST* expr, TypeInfo* expected) {
if (expr == NULL  ||  expr->kind != AST_STRING_LIT) {
return 0;
}
if (mod_src_typecheck_flo_is_cstr_type(expected)) {
expr->data._string.lower_kind = STRING_LOWER_CSTR;
return 1;
}
if (mod_src_typecheck_flo_is_string_type(expected)) {
expr->data._string.lower_kind = STRING_LOWER_STRING;
return 1;
}
return 0;
}


int mod_src_typecheck_flo_builtin_expects_cstr_arg(char* src, int start, int length, int index) {
if (length == 6  &&  !(strncmp(src + start, "printf", 6))) {
return index == 0;
}
else if (length == 7  &&  !(strncmp(src + start, "fprintf", 7))) {
return index == 1;
}
else if (length == 8  &&  !(strncmp(src + start, "snprintf", 8))) {
return index == 2;
}
else if (length == 6  &&  !(strncmp(src + start, "strcmp", 6))) {
return index == 0  ||  index == 1;
}
else if (length == 7  &&  !(strncmp(src + start, "strncmp", 7))) {
return index == 0  ||  index == 1;
}
else if (length == 6  &&  !(strncmp(src + start, "strcpy", 6))) {
return index == 0  ||  index == 1;
}
else if (length == 7  &&  !(strncmp(src + start, "strncpy", 7))) {
return index == 0  ||  index == 1;
}
else if (length == 6  &&  !(strncmp(src + start, "strdup", 6))) {
return index == 0;
}
else if (length == 5  &&  !(strncmp(src + start, "fopen", 5))) {
return index == 0  ||  index == 1;
}
else if (length == 6  &&  !(strncmp(src + start, "system", 6))) {
return index == 0;
}
else if (length == 8  &&  !(strncmp(src + start, "realpath", 8))) {
return index == 0;
}
return 0;
}


void mod_src_typecheck_flo_annotate_builtin_cstr_args(AST* args, char* src, int start, int length) {
AST* arg = args;
int index = 0;
while (arg != NULL) {
if (mod_src_typecheck_flo_builtin_expects_cstr_arg(src, start, length, index)  &&  arg->kind == AST_STRING_LIT) {
arg->data._string.lower_kind = STRING_LOWER_CSTR;
}
arg = arg->next;
index = index + 1;
}
}


int mod_src_typecheck_flo_wider_numeric_base(TypeInfo* left, TypeInfo* right) {
if (left->base == TOKEN_DOUBLE  ||  right->base == TOKEN_DOUBLE) {
return TOKEN_DOUBLE;
}
if (left->base == TOKEN_FLOAT  ||  right->base == TOKEN_FLOAT) {
return TOKEN_FLOAT;
}
return TOKEN_INT;
}


void mod_src_typecheck_flo_set_plain_type(TypeInfo* out, int base) {
out->base = base;
out->pointer_depth = 0;
out->array_size = 0;
out->arr_size_expr = NULL;
out->name_start = 0;
out->name_length = 0;
out->name_src = NULL;
}


void mod_src_typecheck_flo_set_cstr_type(TypeInfo* out) {
out->base = TOKEN_CHAR;
out->pointer_depth = 1;
out->array_size = 0;
out->arr_size_expr = NULL;
out->name_start = 0;
out->name_length = 0;
out->name_src = NULL;
}


int mod_src_typecheck_flo_is_comparison_op(int kind) {
return kind == TOKEN_GT  ||  kind == TOKEN_LT  ||  kind == TOKEN_NEQ  ||  kind == TOKEN_LEQ  ||  kind == TOKEN_GEQ  ||  kind == TOKEN_COMP;
}


int mod_src_typecheck_flo_is_logical_op(int kind) {
return kind == TOKEN_AND  ||  kind == TOKEN_OR;
}


int mod_src_typecheck_flo_can_decay_array_to_pointer(TypeInfo* expected, TypeInfo* actual) {
if (actual->array_size == 0  ||  expected->array_size != 0) {
return 0;
}
if (expected->base != actual->base) {
return 0;
}
if (expected->pointer_depth != actual->pointer_depth + 1) {
return 0;
}
if (expected->base == TOKEN_IDENTIFIER) {
if (!(mod_src_typecheck_flo_same_name(expected->name_src, expected->name_start, expected->name_length, actual->name_src, actual->name_start, actual->name_length))) {
return 0;
}
}
return 1;
}


int mod_src_typecheck_flo_can_pass_arg(TypeInfo* expected, TypeInfo* actual) {
if (mod_src_typecheck_flo_can_implicitly_convert(expected, actual)) {
return 1;
}
if (mod_src_typecheck_flo_can_decay_array_to_pointer(expected, actual)) {
return 1;
}
return 0;
}


int mod_src_typecheck_flo_types_match(TypeInfo* expected, TypeInfo* actual) {
if (mod_src_typecheck_flo_is_null_type(expected)  &&  mod_src_typecheck_flo_is_pointer_like_type(actual)) {
return 1;
}
if (mod_src_typecheck_flo_is_null_type(actual)  &&  mod_src_typecheck_flo_is_pointer_like_type(expected)) {
return 1;
}
if (mod_src_typecheck_flo_is_decimal_type(expected)  &&  mod_src_typecheck_flo_is_decimal_type(actual)) {
return 1;
}
if (expected->base != actual->base) {
return 0;
}
if (expected->pointer_depth != actual->pointer_depth) {
return 0;
}
if (expected->base == TOKEN_IDENTIFIER) {
if (mod_src_typecheck_flo_same_name(expected->name_src, expected->name_start, expected->name_length, actual->name_src, actual->name_start, actual->name_length)) {
return 1;
}
return 0;
}
return 1;
}


void mod_src_typecheck_flo_type_error(TypeEnv* env, AST* ast, char* message) {
if (ast != NULL  &&  ast->src != NULL  &&  ast->filename != NULL) {
int line = mod_include_lexer_h_flo_get_line(ast->src, ast->start);
int col = mod_include_lexer_h_flo_get_col(ast->src, ast->start);
printf("%s%s:%d:%d: error:%s %s\n", RED, ast->filename, line, col, RESET, message);
}
else {
printf("%serror:%s %s\n", RED, RESET, message);
}
env->error_count = env->error_count + 1;
}
void mod_src_typecheck_flo_register_struct(TypeEnv* env, AST* ast, char* src);
void mod_src_typecheck_flo_register_union(TypeEnv* env, AST* ast, char* src);
void mod_src_typecheck_flo_register_func(TypeEnv* env, AST* ast, char* src);
void mod_src_typecheck_flo_register_var(TypeEnv* env, AST* ast, char* src);
void mod_src_typecheck_flo_register_params(TypeEnv* env, AST* ast, char* src);
void mod_src_typecheck_flo_walk_statement_list(TypeEnv* env, AST* list, char* src);
void mod_src_typecheck_flo_typecheck_statement(TypeEnv* env, AST* ast, char* src);
StructInfo* mod_src_typecheck_flo_lookup_struct(TypeEnv* env, char* src, int start, int length);
FieldInfo* mod_src_typecheck_flo_lookup_field(TypeEnv* env, StructInfo* info, char* src, int start, int length);
int mod_src_typecheck_flo_lookup_func_return_type_in_module(TypeEnv* env, char* module_src, char* call_src, int start, int length, TypeInfo* out);
void mod_src_typecheck_flo_resolve_expr_list(TypeEnv* env, AST* list, char* src);
int mod_src_typecheck_flo_resolve_expr(TypeEnv* env, AST* expr, TypeInfo* out, char* src);
int mod_src_typecheck_flo_resolve_dot(TypeEnv* env, AST* expr, TypeInfo* out, char* src);
int mod_src_typecheck_flo_resolve_alias_call(TypeEnv* env, AST* expr, TypeInfo* out, char* src);
void mod_src_typecheck_flo_type_error(TypeEnv* env, AST* ast, char* message);


void mod_src_typecheck_flo_typecheck_collect(TypeEnv* env, AST* ast, char* src) {
AST* curr = ast;
while (curr != NULL) {
if (curr->kind == AST_STRUCT_DEF) {
mod_src_typecheck_flo_register_struct(env, curr, src);
}
else if (curr->kind == AST_UNION_DEF) {
mod_src_typecheck_flo_register_union(env, curr, src);
}
else if (curr->kind == AST_FUNC_DEF) {
mod_src_typecheck_flo_register_func(env, curr, src);
}
else if (curr->kind == AST_PROP  &&  curr->data._prop.decl != NULL) {
AST* decl = curr->data._prop.decl;
if (decl->kind == AST_FUNC_DEF) {
mod_src_typecheck_flo_register_func(env, decl, src);
}
else if (decl->kind == AST_STRUCT_DEF) {
mod_src_typecheck_flo_register_struct(env, decl, src);
}
else if (decl->kind == AST_UNION_DEF) {
mod_src_typecheck_flo_register_union(env, decl, src);
}
}
else if (curr->kind == AST_VAR_DECL) {
mod_src_typecheck_flo_register_var(env, curr, src);
}
curr = curr->next;
}
}


void mod_src_typecheck_flo_register_struct(TypeEnv* env, AST* ast, char* src) {
if (env->struct_count >= 1024) {
mod_src_typecheck_flo_type_error(env, ast, "type environment struct table overflow");
return;}
StructInfo* info = env->structs + env->struct_count;
info->src = src;
info->name_start = ast->data._struct_def.name_start;
info->name_length = ast->data._struct_def.name_length;
info->field_count = 0;
AST* field = ast->data._struct_def.fields;
while (field != NULL) {
FieldInfo* finfo = info->fields + info->field_count;
finfo->src = src;
finfo->name_start = field->data._struct_field.name_start;
finfo->name_length = field->data._struct_field.name_length;
mod_src_typecheck_flo_copy_type(&(finfo->type), &(field->data._struct_field.type));
finfo->is_hidden = field->data._struct_field.is_hidden;
finfo->is_frozen = field->data._struct_field.is_frozen;
finfo->is_readonly = field->data._struct_field.is_readonly;
info->field_count = info->field_count + 1;
field = field->next;
}
env->struct_count = env->struct_count + 1;
}


void mod_src_typecheck_flo_register_union(TypeEnv* env, AST* ast, char* src) {
if (env->struct_count >= 1024) {
mod_src_typecheck_flo_type_error(env, ast, "type environment struct table overflow");
return;}
StructInfo* info = env->structs + env->struct_count;
info->src = src;
info->name_start = ast->data._union_def.name_start;
info->name_length = ast->data._union_def.name_length;
info->field_count = 0;
AST* field = ast->data._union_def.fields;
while (field != NULL) {
FieldInfo* finfo = info->fields + info->field_count;
finfo->src = src;
finfo->name_start = field->data._struct_field.name_start;
finfo->name_length = field->data._struct_field.name_length;
mod_src_typecheck_flo_copy_type(&(finfo->type), &(field->data._struct_field.type));
finfo->is_hidden = field->data._struct_field.is_hidden;
finfo->is_frozen = field->data._struct_field.is_frozen;
finfo->is_readonly = field->data._struct_field.is_readonly;
info->field_count = info->field_count + 1;
field = field->next;
}
env->struct_count = env->struct_count + 1;
}


void mod_src_typecheck_flo_register_func(TypeEnv* env, AST* ast, char* src) {
if (env->func_count >= 1024) {
mod_src_typecheck_flo_type_error(env, ast, "type environment struct table overflow");
return;}
FuncInfo* info = env->funcs + env->func_count;
info->src = src;
info->name_start = ast->data._func_def.name_start;
info->name_length = ast->data._func_def.name_length;
info->params = ast->data._func_def.params;
mod_src_typecheck_flo_copy_type(&(info->return_type), &(ast->data._func_def.return_type));
env->func_count = env->func_count + 1;
}


void mod_src_typecheck_flo_register_var(TypeEnv* env, AST* ast, char* src) {
if (ast == NULL) {
return;}
if (env->var_count >= 2048) {
mod_src_typecheck_flo_type_error(env, ast, "type environment struct table overflow");
return;}
VarInfo* info = env->vars + env->var_count;
info->src = src;
info->name_start = ast->data._var_decl.name_start;
info->name_length = ast->data._var_decl.name_length;
mod_src_typecheck_flo_copy_type(&(info->type), &(ast->data._var_decl.type));
env->var_count = env->var_count + 1;
}


void mod_src_typecheck_flo_register_params(TypeEnv* env, AST* fnc, char* src) {
AST* param = fnc->data._func_def.params;
while (param != NULL) {
if (env->var_count >= 2048) {
mod_src_typecheck_flo_type_error(env, fnc, "type environment struct table overflow");
return;}
VarInfo* info = env->vars + env->var_count;
info->src = src;
info->name_start = param->data._func_params.name_start;
info->name_length = param->data._func_params.name_length;
mod_src_typecheck_flo_copy_type(&(info->type), &(param->data._func_params.type));
env->var_count = env->var_count + 1;
param = param->next;
}
}
int mod_src_typecheck_flo_lookup_var_type(TypeEnv* env, char* src, int start, int length, TypeInfo* out);
int mod_src_typecheck_flo_lookup_func_return_type(TypeEnv* env, char* src, int start, int length, TypeInfo* out);
FuncInfo* mod_src_typecheck_flo_lookup_func_info(TypeEnv* env, char* src, int start, int length);
FuncInfo* mod_src_typecheck_flo_lookup_func_in_module(TypeEnv* env, char* module_src, char* call_src, int start, int length);
int mod_src_typecheck_flo_check_call_args(TypeEnv* env, AST* call_ast, AST* args, AST* params, char* src);


void mod_src_typecheck_flo_resolve_expr_list(TypeEnv* env, AST* list, char* src) {
AST* curr = list;
while (curr != NULL) {
TypeInfo* tmp = malloc(sizeof(TypeInfo));
mod_src_typecheck_flo_resolve_expr(env, curr, tmp, src);
free(tmp);
curr = curr->next;
}
}


int mod_src_typecheck_flo_resolve_builtin_c_call(TypeEnv* env, AST* expr, TypeInfo* out, char* src) {
int start = expr->data._func_call.name_start;
int length = expr->data._func_call.name_length;
int return_kind = 0;
if (length == 6  &&  !(strncmp(src + start, "printf", 6))) {
return_kind = 1;
}
else if (length == 7  &&  !(strncmp(src + start, "fprintf", 7))) {
return_kind = 1;
}
else if (length == 8  &&  !(strncmp(src + start, "snprintf", 8))) {
return_kind = 1;
}
else if (length == 6  &&  !(strncmp(src + start, "strcmp", 6))) {
return_kind = 1;
}
else if (length == 7  &&  !(strncmp(src + start, "strncmp", 7))) {
return_kind = 1;
}
else if (length == 6  &&  !(strncmp(src + start, "strlen", 6))) {
return_kind = 1;
}
else if (length == 7  &&  !(strncmp(src + start, "tolower", 7))) {
return_kind = 1;
}
else if (length == 6  &&  !(strncmp(src + start, "system", 6))) {
return_kind = 1;
}
else if (length == 6  &&  !(strncmp(src + start, "fflush", 6))) {
return_kind = 1;
}
else if (length == 6  &&  !(strncmp(src + start, "fclose", 6))) {
return_kind = 1;
}
else if (length == 6  &&  !(strncmp(src + start, "strcpy", 6))) {
return_kind = 2;
}
else if (length == 7  &&  !(strncmp(src + start, "strncpy", 7))) {
return_kind = 2;
}
else if (length == 6  &&  !(strncmp(src + start, "strdup", 6))) {
return_kind = 2;
}
else if (length == 7  &&  !(strncmp(src + start, "strrchr", 7))) {
return_kind = 2;
}
else if (length == 8  &&  !(strncmp(src + start, "realpath", 8))) {
return_kind = 2;
}
else if (length == 6  &&  !(strncmp(src + start, "getcwd", 6))) {
return_kind = 2;
}
else {
return 0;
}
mod_src_typecheck_flo_annotate_builtin_cstr_args(expr->data._func_call.args, src, start, length);
mod_src_typecheck_flo_resolve_expr_list(env, expr->data._func_call.args, src);
if (return_kind == 1) {
mod_src_typecheck_flo_set_plain_type(out, TOKEN_INT);
}
else {
mod_src_typecheck_flo_set_cstr_type(out);
}
return 1;
}


int mod_src_typecheck_flo_resolve_expr(TypeEnv* env, AST* expr, TypeInfo* out, char* src) {
if (expr == NULL) {
return 0;
}
if (expr->kind == AST_VAR_REF) {
return mod_src_typecheck_flo_lookup_var_type(env, src, expr->data._var_ref.name_start, expr->data._var_ref.name_length, out);
}
else if (expr->kind == AST_NEW) {
mod_src_typecheck_flo_copy_type(out, &(expr->data._new_alloc.type));
out->pointer_depth = out->pointer_depth + 1;
return 1;
}
else if (expr->kind == AST_DEREF) {
if (!(mod_src_typecheck_flo_resolve_expr(env, expr->data._deref.operand, out, src))) {
mod_src_typecheck_flo_type_error(env, expr, "could not resolve object type for field access");
return 0;
}
if (out->pointer_depth <= 0) {
return 0;
}
out->pointer_depth = out->pointer_depth - 1;
return 1;
}
else if (expr->kind == AST_GET_ADDR) {
if (!(mod_src_typecheck_flo_resolve_expr(env, expr->data._get_addr.operand, out, src))) {
return 0;
}
out->pointer_depth = out->pointer_depth + 1;
return 1;
}
else if (expr->kind == AST_DOT_ACCESS) {
return mod_src_typecheck_flo_resolve_dot(env, expr, out, src);
}
else if (expr->kind == AST_FUNC_CALL) {
if (mod_src_typecheck_flo_resolve_builtin_c_call(env, expr, out, src)) {
return 1;
}
mod_src_typecheck_flo_annotate_builtin_cstr_args(expr->data._func_call.args, src, expr->data._func_call.name_start, expr->data._func_call.name_length);
FuncInfo* info = mod_src_typecheck_flo_lookup_func_info(env, src, expr->data._func_call.name_start, expr->data._func_call.name_length);
if (info == NULL) {
mod_src_typecheck_flo_resolve_expr_list(env, expr->data._func_call.args, src);
return 0;
}
if (!(mod_src_typecheck_flo_check_call_args(env, expr, expr->data._func_call.args, info->params, src))) {
return 0;
}
mod_src_typecheck_flo_copy_type(out, &(info->return_type));
return 1;
}
else if (expr->kind == AST_ALIAS_CALL) {
return mod_src_typecheck_flo_resolve_alias_call(env, expr, out, src);
}
else if (expr->kind == AST_SUBSCRIPT) {
if (!(mod_src_typecheck_flo_resolve_expr(env, expr->data._subscript.array, out, src))) {
return 0;
}
TypeInfo* idx_type = malloc(sizeof(TypeInfo));
if (!(mod_src_typecheck_flo_resolve_expr(env, expr->data._subscript.index, idx_type, src))) {
free(idx_type);
return 0;
}
free(idx_type);
if (out->array_size != 0) {
out->array_size = 0;
out->arr_size_expr = NULL;
return 1;
}
if (out->pointer_depth > 0) {
out->pointer_depth = out->pointer_depth - 1;
return 1;
}
return 1;
}
else if (expr->kind == AST_BINARY_OP) {
TypeInfo* left_type = malloc(sizeof(TypeInfo));
TypeInfo* right_type = malloc(sizeof(TypeInfo));
if (!(mod_src_typecheck_flo_resolve_expr(env, expr->data._binary.left, left_type, src))) {
free(left_type);
free(right_type);
return 0;
}
if (!(mod_src_typecheck_flo_resolve_expr(env, expr->data._binary.right, right_type, src))) {
free(left_type);
free(right_type);
return 0;
}
if (mod_src_typecheck_flo_is_logical_op(expr->data._binary.op)) {
if (!(mod_src_typecheck_flo_is_condition_type(left_type))  ||  !(mod_src_typecheck_flo_is_condition_type(right_type))) {
mod_src_typecheck_flo_type_error(env, expr, "logical operators require condition-compatible operands");
free(left_type);
free(right_type);
return 0;
}
out->base = TOKEN_BOOL;
out->pointer_depth = 0;
out->array_size = 0;
out->arr_size_expr = NULL;
}
else if (mod_src_typecheck_flo_is_comparison_op(expr->data._binary.op)) {
expr->data._binary.is_string_compare = 0;
if (mod_src_typecheck_flo_is_string_type(left_type)  ||  mod_src_typecheck_flo_is_string_type(right_type)) {
if (mod_src_typecheck_flo_is_string_type(left_type)  &&  mod_src_typecheck_flo_is_string_type(right_type)) {
if (expr->data._binary.op == TOKEN_COMP  ||  expr->data._binary.op == TOKEN_NEQ) {
mod_src_typecheck_flo_set_plain_type(out, TOKEN_BOOL);
expr->data._binary.is_string_compare = 1;
}
else {
mod_src_typecheck_flo_type_error(env, expr, "string comparisons only support == and !=");
free(left_type);
free(right_type);
return 0;
}
}
else {
mod_src_typecheck_flo_type_error(env, expr, "string comparisons require both operands to be string");
free(left_type);
free(right_type);
return 0;
}
}
else if (!(mod_src_typecheck_flo_can_convert_either_way(left_type, right_type))) {
mod_src_typecheck_flo_type_error(env, expr, "comparison operands must have matching types");
free(left_type);
free(right_type);
return 0;
}
else {
mod_src_typecheck_flo_set_plain_type(out, TOKEN_BOOL);
}
out->base = TOKEN_BOOL;
out->pointer_depth = 0;
out->array_size = 0;
out->arr_size_expr = NULL;
}
else {
if (mod_src_typecheck_flo_is_bool_type(left_type)  ||  mod_src_typecheck_flo_is_bool_type(right_type)) {
mod_src_typecheck_flo_type_error(env, expr, "arithmetic operators do not accept bool operands");
free(left_type);
free(right_type);
return 0;
}
if (expr->data._binary.op == TOKEN_PLUS  ||  expr->data._binary.op == TOKEN_MINUS  &&  mod_src_typecheck_flo_is_pointer_like_type(left_type)  &&  mod_src_typecheck_flo_is_integer_type(right_type)) {
mod_src_typecheck_flo_copy_type(out, left_type);
if (out->array_size != 0) {
out->array_size = 0;
out->arr_size_expr = NULL;
out->pointer_depth = out->pointer_depth + 1;
}
free(left_type);
free(right_type);
return 1;
}
if (expr->data._binary.op == TOKEN_PLUS  &&  mod_src_typecheck_flo_is_integer_type(left_type)  &&  mod_src_typecheck_flo_is_pointer_like_type(right_type)) {
mod_src_typecheck_flo_copy_type(out, right_type);
if (out->array_size != 0) {
out->array_size = 0;
out->arr_size_expr = NULL;
out->pointer_depth = out->pointer_depth + 1;
}
free(left_type);
free(right_type);
return 0;
}
if (!(mod_src_typecheck_flo_can_convert_either_way(left_type, right_type))) {
mod_src_typecheck_flo_type_error(env, expr, "arithmetic operands must have matching or compatible types");
free(left_type);
free(right_type);
return 0;
}
if (mod_src_typecheck_flo_is_numeric_type(left_type)  &&  mod_src_typecheck_flo_is_numeric_type(right_type)) {
mod_src_typecheck_flo_set_plain_type(out, mod_src_typecheck_flo_wider_numeric_base(left_type, right_type));
}
else {
mod_src_typecheck_flo_copy_type(out, left_type);
}
}
free(left_type);
free(right_type);
return 1;
}
else if (expr->kind == AST_UNARY_NOT) {
TypeInfo* operand_type = malloc(sizeof(TypeInfo));
if (!(mod_src_typecheck_flo_resolve_expr(env, expr->data._unary.operand, operand_type, src))) {
free(operand_type);
return 0;
}
if (!(mod_src_typecheck_flo_is_condition_type(operand_type))) {
mod_src_typecheck_flo_type_error(env, expr, "not requires a condition-compatible operand");
free(operand_type);
return 0;
}
free(operand_type);
out->base = TOKEN_BOOL;
out->pointer_depth = 0;
out->array_size = 0;
out->arr_size_expr = NULL;
return 1;
}
else if (expr->kind == AST_UNARY_NEG) {
TypeInfo* operand_type = malloc(sizeof(TypeInfo));
if (!(mod_src_typecheck_flo_resolve_expr(env, expr->data._unary.operand, operand_type, src))) {
free(operand_type);
return 0;
}
if (mod_src_typecheck_flo_is_bool_type(operand_type)) {
mod_src_typecheck_flo_type_error(env, expr, "unary minus does not accept bool");
free(operand_type);
return 0;
}
mod_src_typecheck_flo_copy_type(out, operand_type);
free(operand_type);
return 1;
}
else if (expr->kind == AST_CAST) {
TypeInfo* value_type = malloc(sizeof(TypeInfo));
if (!(mod_src_typecheck_flo_resolve_expr(env, expr->data._cast.value, value_type, src))) {
free(value_type);
return 0;
}
if (!(mod_src_typecheck_flo_can_explicitly_convert(&(expr->data._cast.type), value_type))) {
mod_src_typecheck_flo_type_error(env, expr, "cast is not valid for these types");
free(value_type);
return 0;
}
if (!(mod_src_typecheck_flo_can_explicitly_convert(&(expr->data._cast.type), value_type))) {
mod_src_typecheck_flo_type_error(env, expr, "cast is not valid for these types");
free(value_type);
return 0;
}
mod_src_typecheck_flo_copy_type(&(expr->data._cast.value_type), value_type);
free(value_type);
mod_src_typecheck_flo_copy_type(out, &(expr->data._cast.type));
return 1;
}
else if (expr->kind == AST_SIZEOF) {
out->base = TOKEN_INT;
out->pointer_depth = 0;
out->array_size = 0;
out->arr_size_expr = NULL;
return 1;
}
else if (expr->kind == AST_LITERAL) {
out->base = TOKEN_INT;
out->pointer_depth = 0;
out->array_size = 0;
out->arr_size_expr = NULL;
return 1;
}
else if (expr->kind == AST_FLOAT_LIT) {
out->base = TOKEN_FLOAT;
out->pointer_depth = 0;
out->array_size = 0;
out->arr_size_expr = NULL;
return 1;
}
else if (expr->kind == AST_BOOL_LIT) {
out->base = TOKEN_BOOL;
out->pointer_depth = 0;
out->array_size = 0;
out->arr_size_expr = NULL;
return 1;
}
else if (expr->kind == AST_CHAR_LIT) {
out->base = TOKEN_CHAR;
out->pointer_depth = 0;
out->array_size = 0;
out->arr_size_expr = NULL;
return 1;
}
else if (expr->kind == AST_STRING_LIT) {
mod_src_typecheck_flo_set_plain_type(out, TOKEN_STRING);
return 1;
}
else if (expr->kind == AST_NULL) {
out->base = TOKEN_VOID;
out->pointer_depth = 1;
out->array_size = 0;
out->arr_size_expr = NULL;
return 1;
}
return 0;
}


int mod_src_typecheck_flo_resolve_dot(TypeEnv* env, AST* expr, TypeInfo* out, char* src) {
TypeInfo* object_type = malloc(sizeof(TypeInfo));
if (!(mod_src_typecheck_flo_resolve_expr(env, expr->data._dot_access.object, object_type, src))) {
mod_src_typecheck_flo_type_error(env, expr, "could not resolve object type for field access");
free(object_type);
return 0;
}
if (object_type->base == TOKEN_STRING) {
if (object_type->pointer_depth == 0) {
expr->data._dot_access.access_kind = ACCESS_DOT;
}
else if (object_type->pointer_depth == 1) {
expr->data._dot_access.access_kind = ACCESS_ARROW;
}
else {
mod_src_typecheck_flo_type_error(env, expr, "cannot directly access field through multi-pointer type");
free(object_type);
return 0;
}
if (expr->data._dot_access.field_length == 6  &&  !(strncmp(src + expr->data._dot_access.field_start, "length", 6))) {
expr->data._dot_access.is_hidden = 0;
expr->data._dot_access.is_frozen = 1;
expr->data._dot_access.is_readonly = 1;
expr->data._dot_access.owner_src = NULL;
mod_src_typecheck_flo_set_plain_type(out, TOKEN_INT);
mod_src_typecheck_flo_copy_type(&(expr->data._dot_access.resolved_type), out);
free(object_type);
return 1;
}
mod_src_typecheck_flo_type_error(env, expr, "unknown field on string");
expr->data._dot_access.access_kind = ACCESS_UNKNOWN;
free(object_type);
return 0;
}
if (object_type->base != TOKEN_IDENTIFIER) {
mod_src_typecheck_flo_type_error(env, expr, "cannot access field on non-struct type");
expr->data._dot_access.access_kind = ACCESS_UNKNOWN;
free(object_type);
return 0;
}
if (object_type->pointer_depth == 0) {
expr->data._dot_access.access_kind = ACCESS_DOT;
}
else if (object_type->pointer_depth == 1) {
expr->data._dot_access.access_kind = ACCESS_ARROW;
}
else {
mod_src_typecheck_flo_type_error(env, expr, "cannot directly access field through multi-pointer type");
free(object_type);
return 0;
}
StructInfo* base_struct = mod_src_typecheck_flo_lookup_struct(env, object_type->name_src, object_type->name_start, object_type->name_length);
if (base_struct == NULL) {
mod_src_typecheck_flo_type_error(env, expr, "unknown struct type in field access");
expr->data._dot_access.access_kind = ACCESS_UNKNOWN;
free(object_type);
return 0;
}
FieldInfo* field = mod_src_typecheck_flo_lookup_field(env, base_struct, src, expr->data._dot_access.field_start, expr->data._dot_access.field_length);
if (field == NULL) {
mod_src_typecheck_flo_type_error(env, expr, "unknown field on struct");
expr->data._dot_access.access_kind = ACCESS_UNKNOWN;
free(object_type);
return 0;
}
expr->data._dot_access.is_hidden = field->is_hidden;
expr->data._dot_access.is_frozen = field->is_frozen;
expr->data._dot_access.is_readonly = field->is_readonly;
expr->data._dot_access.owner_src = base_struct->src;
if (current_type_module != NULL  &&  base_struct->src != current_type_module->src) {
if (field->is_hidden) {
mod_src_typecheck_flo_type_error(env, expr, "field does not exist in this scope");
expr->data._dot_access.access_kind = ACCESS_UNKNOWN;
free(object_type);
return 0;
}
}
mod_src_typecheck_flo_copy_type(out, &(field->type));
mod_src_typecheck_flo_copy_type(&(expr->data._dot_access.resolved_type), &(field->type));
free(object_type);
return 1;
}


int mod_src_typecheck_flo_resolve_alias_call(TypeEnv* env, AST* expr, TypeInfo* out, char* src) {
if (current_type_module == NULL) {
return 0;
}
Module* imported = mod_src_module_flo_find_imported_module(active_type_modules, current_type_module, src, expr->data._alias_call.alias_start, expr->data._alias_call.alias_length);
if (imported == NULL) {
mod_src_typecheck_flo_resolve_expr_list(env, expr->data._alias_call.args, src);
mod_src_typecheck_flo_type_error(env, expr, "unknown module alias in function call");
return 0;
}
if (!(mod_src_module_flo_module_exports_func(imported, src, expr->data._alias_call.func_start, expr->data._alias_call.func_length))) {
mod_src_typecheck_flo_resolve_expr_list(env, expr->data._alias_call.args, src);
mod_src_typecheck_flo_type_error(env, expr, "function is not exported by imported module");
return 0;
}
FuncInfo* info = mod_src_typecheck_flo_lookup_func_in_module(env, imported->src, src, expr->data._alias_call.func_start, expr->data._alias_call.func_length);
if (info == NULL) {
mod_src_typecheck_flo_resolve_expr_list(env, expr->data._alias_call.args, src);
mod_src_typecheck_flo_type_error(env, expr, "could not resolve imported function return type");
return 0;
}
if (!(mod_src_typecheck_flo_check_call_args(env, expr, expr->data._alias_call.args, info->params, src))) {
return 0;
}
mod_src_typecheck_flo_copy_type(out, &(info->return_type));
return 1;
}


void mod_src_typecheck_flo_typecheck_statement(TypeEnv* env, AST* ast, char* src) {
if (ast->kind == AST_VAR_DECL) {
mod_src_typecheck_flo_register_var(env, ast, src);
if (ast->data._var_decl.value != NULL) {
TypeInfo* value_type = malloc(sizeof(TypeInfo));
if (mod_src_typecheck_flo_resolve_expr(env, ast->data._var_decl.value, value_type, src)) {
if (!(mod_src_typecheck_flo_allow_string_literal_for_target(ast->data._var_decl.value, &(ast->data._var_decl.type)))  &&  !(mod_src_typecheck_flo_can_implicitly_convert(&(ast->data._var_decl.type), value_type))) {
mod_src_typecheck_flo_type_error(env, ast, "variable initializer does not match declared type");
free(value_type);
return;}
}
free(value_type);
}
}
else if (ast->kind == AST_VAR_ASS) {
TypeInfo* target_type = malloc(sizeof(TypeInfo));
TypeInfo* value_type = malloc(sizeof(TypeInfo));
if (!(mod_src_typecheck_flo_lookup_var_type(env, src, ast->data._var_ass.name_start, ast->data._var_ass.name_length, target_type))) {
mod_src_typecheck_flo_type_error(env, ast, "unknown variable in assignment");
free(target_type);
free(value_type);
return;}
if (mod_src_typecheck_flo_resolve_expr(env, ast->data._var_ass.value, value_type, src)) {
if (!(mod_src_typecheck_flo_allow_string_literal_for_target(ast->data._var_ass.value, target_type))  &&  !(mod_src_typecheck_flo_can_implicitly_convert(target_type, value_type))) {
mod_src_typecheck_flo_type_error(env, ast, "assigned value does not match variable type");
}
}
free(target_type);
free(value_type);
}
else if (ast->kind == AST_DOT_ACCESS) {
TypeInfo* tmp = malloc(sizeof(TypeInfo));
mod_src_typecheck_flo_resolve_expr(env, ast, tmp, src);
free(tmp);
if (ast->data._dot_access.value != NULL) {
TypeInfo* object_type = malloc(sizeof(TypeInfo));
if (mod_src_typecheck_flo_resolve_expr(env, ast->data._dot_access.object, object_type, src)) {
if (object_type->base == TOKEN_STRING) {
mod_src_typecheck_flo_type_error(env, ast, "string.length is readonly");
free(object_type);
return;}
}
free(object_type);
if (current_type_module != NULL  &&  ast->data._dot_access.owner_src != NULL  &&  ast->data._dot_access.owner_src != current_type_module->src) {
if (ast->data._dot_access.is_readonly) {
mod_src_typecheck_flo_type_error(env, ast, "field is readonly from this scope");
return;}
}
TypeInfo* value_type = malloc(sizeof(TypeInfo));
mod_src_typecheck_flo_resolve_expr(env, ast->data._dot_access.value, value_type, src);
free(value_type);
}
}
else if (ast->kind == AST_IF) {
if (ast->data._if_condition.condition != NULL) {
TypeInfo* tmp = malloc(sizeof(TypeInfo));
if (mod_src_typecheck_flo_resolve_expr(env, ast->data._if_condition.condition, tmp, src)) {
if (!(mod_src_typecheck_flo_is_condition_type(tmp))) {
mod_src_typecheck_flo_type_error(env, ast, "if condition must be a condition-compatible type");
}
}
free(tmp);
}
mod_src_typecheck_flo_walk_statement_list(env, ast->data._if_condition.body, src);
mod_src_typecheck_flo_walk_statement_list(env, ast->data._if_condition.else_branch, src);
}
else if (ast->kind == AST_WHILE) {
TypeInfo* tmp = malloc(sizeof(TypeInfo));
if (mod_src_typecheck_flo_resolve_expr(env, ast->data._while_loop.condition, tmp, src)) {
if (!(mod_src_typecheck_flo_is_condition_type(tmp))) {
mod_src_typecheck_flo_type_error(env, ast, "while condition must be condition-compatible type");
}
}
mod_src_typecheck_flo_walk_statement_list(env, ast->data._while_loop.body, src);
free(tmp);
}
else if (ast->kind == AST_FOR) {
int saved_var_count = env->var_count;
if (env->var_count >= 2048) {
mod_src_typecheck_flo_type_error(env, ast, "type environment struct table overflow");
return;}
VarInfo* loop_var = env->vars + env->var_count;
loop_var->src = src;
loop_var->name_start = ast->data._for_loop.var_start;
loop_var->name_length = ast->data._for_loop.var_length;
loop_var->type.base = TOKEN_INT;
loop_var->type.pointer_depth = 0;
loop_var->type.array_size = 0;
loop_var->type.arr_size_expr = NULL;
loop_var->type.name_start = 0;
loop_var->type.name_length = 0;
loop_var->type.name_src = NULL;
env->var_count = env->var_count + 1;
TypeInfo* from_type = malloc(sizeof(TypeInfo));
TypeInfo* to_type = malloc(sizeof(TypeInfo));
mod_src_typecheck_flo_resolve_expr(env, ast->data._for_loop.from, from_type, src);
mod_src_typecheck_flo_resolve_expr(env, ast->data._for_loop.to, to_type, src);
mod_src_typecheck_flo_walk_statement_list(env, ast->data._for_loop.body, src);
free(from_type);
free(to_type);
env->var_count = saved_var_count;
}
else if (ast->kind == AST_FLOW_CONTROL) {
if (ast->data._flow_ctrl.base != NULL  &&  ast->data._flow_ctrl.base->kind == TOKEN_RETURN  &&  inside_function) {
if (ast->data._flow_ctrl.value == NULL) {
if (!(mod_src_typecheck_flo_is_void_type(&(current_return_type)))) {
mod_src_typecheck_flo_type_error(env, ast, "return value does not match function return type");
}
}
else {
TypeInfo* tmp = malloc(sizeof(TypeInfo));
if (mod_src_typecheck_flo_resolve_expr(env, ast->data._flow_ctrl.value, tmp, src)) {
if (!(mod_src_typecheck_flo_allow_string_literal_for_target(ast->data._flow_ctrl.value, &(current_return_type)))  &&  !(mod_src_typecheck_flo_can_implicitly_convert(&(current_return_type), tmp))) {
mod_src_typecheck_flo_type_error(env, ast, "return value does not match function return type");
}
}
free(tmp);
}
}
else if (ast->data._flow_ctrl.value != NULL) {
TypeInfo* tmp = malloc(sizeof(TypeInfo));
mod_src_typecheck_flo_resolve_expr(env, ast->data._flow_ctrl.value, tmp, src);
free(tmp);
}
}
else if (ast->kind == AST_PRINT) {
TypeInfo* tmp = malloc(sizeof(TypeInfo));
if (mod_src_typecheck_flo_resolve_expr(env, ast->data._print.value, tmp, src)) {
mod_src_typecheck_flo_copy_type(&(ast->data._print.value_type), tmp);
}
free(tmp);
}
else if (ast->kind == AST_FUNC_CALL  ||  ast->kind == AST_ALIAS_CALL) {
TypeInfo* tmp = malloc(sizeof(TypeInfo));
mod_src_typecheck_flo_resolve_expr(env, ast, tmp, src);
free(tmp);
}
}


void mod_src_typecheck_flo_typecheck_ast(TypeEnv* env, AST* ast, char* src) {
AST* curr = ast;
while (curr != NULL) {
if (curr->kind == AST_FUNC_DEF) {
int saved_var_count = env->var_count;
inside_function = 1;
mod_src_typecheck_flo_copy_type(&(current_return_type), &(curr->data._func_def.return_type));
mod_src_typecheck_flo_register_params(env, curr, src);
mod_src_typecheck_flo_walk_statement_list(env, curr->data._func_def.body, src);
env->var_count = saved_var_count;
inside_function = 0;
}
else if (curr->kind == AST_PROP  &&  curr->data._prop.decl != NULL) {
AST* decl = curr->data._prop.decl;
if (decl->kind == AST_FUNC_DEF) {
int saved_var_count = env->var_count;
inside_function = 1;
mod_src_typecheck_flo_copy_type(&(current_return_type), &(decl->data._func_def.return_type));
mod_src_typecheck_flo_register_params(env, decl, src);
mod_src_typecheck_flo_walk_statement_list(env, decl->data._func_def.body, src);
env->var_count = saved_var_count;
inside_function = 0;
}
}
else if (curr->kind == AST_VAR_DECL) {
if (curr->data._var_decl.value != NULL) {
TypeInfo* value_type = malloc(sizeof(TypeInfo));
if (mod_src_typecheck_flo_resolve_expr(env, curr->data._var_decl.value, value_type, src)) {
if (!(mod_src_typecheck_flo_allow_string_literal_for_target(curr->data._var_decl.value, &(curr->data._var_decl.type)))  &&  !(mod_src_typecheck_flo_can_implicitly_convert(&(curr->data._var_decl.type), value_type))) {
mod_src_typecheck_flo_type_error(env, curr, "variable initializer does not match declared type");
}
}
free(value_type);
}
}
curr = curr->next;
}
}


StructInfo* mod_src_typecheck_flo_lookup_struct(TypeEnv* env, char* src, int start, int length) {
for (int i = 0; ((0) > (env->struct_count)) ? i > (env->struct_count) : i < (env->struct_count); ((0) > (env->struct_count)) ? i-- : i++) {
StructInfo* info = env->structs + i;
if (mod_src_typecheck_flo_same_name(info->src, info->name_start, info->name_length, src, start, length)) {
return info;
}
}
return NULL;
}


FieldInfo* mod_src_typecheck_flo_lookup_field(TypeEnv* env, StructInfo* info, char* src, int start, int length) {
for (int i = 0; ((0) > (info->field_count)) ? i > (info->field_count) : i < (info->field_count); ((0) > (info->field_count)) ? i-- : i++) {
FieldInfo* field = info->fields + i;
if (mod_src_typecheck_flo_same_name(field->src, field->name_start, field->name_length, src, start, length)) {
return field;
}
}
return NULL;
}


int mod_src_typecheck_flo_lookup_var_type(TypeEnv* env, char* src, int start, int length, TypeInfo* out) {
int i = env->var_count - 1;
while (i >= 0) {
VarInfo* info = env->vars + i;
if (mod_src_typecheck_flo_same_name(info->src, info->name_start, info->name_length, src, start, length)) {
mod_src_typecheck_flo_copy_type(out, &(info->type));
return 1;
}
i = i - 1;
}
return 0;
}


int mod_src_typecheck_flo_lookup_func_return_type(TypeEnv* env, char* src, int start, int length, TypeInfo* out) {
for (int i = 0; ((0) > (env->func_count)) ? i > (env->func_count) : i < (env->func_count); ((0) > (env->func_count)) ? i-- : i++) {
FuncInfo* info = env->funcs + i;
if (mod_src_typecheck_flo_same_name(info->src, info->name_start, info->name_length, src, start, length)) {
mod_src_typecheck_flo_copy_type(out, &(info->return_type));
return 1;
}
}
return 0;
}


int mod_src_typecheck_flo_lookup_func_return_type_in_module(TypeEnv* env, char* module_src, char* call_src, int start, int length, TypeInfo* out) {
for (int i = 0; ((0) > (env->func_count)) ? i > (env->func_count) : i < (env->func_count); ((0) > (env->func_count)) ? i-- : i++) {
FuncInfo* info = env->funcs + i;
if (info->src == module_src  &&  mod_src_typecheck_flo_same_name(info->src, info->name_start, info->name_length, call_src, start, length)) {
mod_src_typecheck_flo_copy_type(out, &(info->return_type));
return 1;
}
}
return 0;
}


FuncInfo* mod_src_typecheck_flo_lookup_func_info(TypeEnv* env, char* src, int start, int length) {
int i = env->func_count - 1;
while (i >= 0) {
FuncInfo* info = env->funcs + i;
if (mod_src_typecheck_flo_same_name(info->src, info->name_start, info->name_length, src, start, length)) {
return info;
}
i = i - 1;
}
return NULL;
}


FuncInfo* mod_src_typecheck_flo_lookup_func_in_module(TypeEnv* env, char* module_src, char* call_src, int start, int length) {
int i = env->func_count - 1;
while (i >= 0) {
FuncInfo* info = env->funcs + i;
if (info->src == module_src  &&  mod_src_typecheck_flo_same_name(info->src, info->name_start, info->name_length, call_src, start, length)) {
return info;
}
i = i - 1;
}
return NULL;
}


int mod_src_typecheck_flo_check_call_args(TypeEnv* env, AST* call_ast, AST* args, AST* params, char* src) {
AST* arg = args;
AST* param = params;
while (arg != NULL  &&  param != NULL) {
TypeInfo* arg_type = malloc(sizeof(TypeInfo));
if (!(mod_src_typecheck_flo_resolve_expr(env, arg, arg_type, src))) {
free(arg_type);
return 0;
}
if (!(mod_src_typecheck_flo_allow_string_literal_for_target(arg, &(param->data._func_params.type)))  &&  !(mod_src_typecheck_flo_can_pass_arg(&(param->data._func_params.type), arg_type))) {
mod_src_typecheck_flo_type_error(env, arg, "function argument does not match parameter type");
free(arg_type);
return 0;
}
free(arg_type);
arg = arg->next;
param = param->next;
}
if (arg != NULL  ||  param != NULL) {
mod_src_typecheck_flo_type_error(env, call_ast, "function call argument count does not match parameter count");
return 0;
}
return 1;
}


void mod_src_typecheck_flo_walk_statement_list(TypeEnv* env, AST* list, char* src) {
AST* curr = list;
while (curr != NULL) {
mod_src_typecheck_flo_typecheck_statement(env, curr, src);
curr = curr->next;
}
}


void mod_src_typecheck_flo_init_type_env(TypeEnv* env) {
env->struct_count = 0;
env->var_count = 0;
env->func_count = 0;
env->error_count = 0;
}


int mod_src_typecheck_flo_check_modules(ModuleSet* mods) {
TypeEnv* env = malloc(sizeof(TypeEnv));
mod_src_typecheck_flo_init_type_env(env);
active_type_modules = mods;
for (int i = 0; ((0) > (mods->count)) ? i > (mods->count) : i < (mods->count); ((0) > (mods->count)) ? i-- : i++) {
Module* m = mods->modules + i;
current_type_module = m;
mod_src_typecheck_flo_typecheck_collect(env, m->ast, m->src);
}
for (int i = 0; ((0) > (mods->count)) ? i > (mods->count) : i < (mods->count); ((0) > (mods->count)) ? i-- : i++) {
Module* m = mods->modules + i;
current_type_module = m;
mod_src_typecheck_flo_typecheck_ast(env, m->ast, m->src);
}
current_type_module = NULL;
int count = env->error_count;
free(env);
return count;
}
int has_stdlib = 0;
int has_stdio = 0;


void mod_src_codegen_flo_emit_string_runtime(FILE* out) {
fprintf(out, "#include <stdio.h>\n");
fprintf(out, "#include <string.h>\n");
fprintf(out, "typedef struct { char* data; int length; } flower_string;\n");
fprintf(out, "static flower_string flower_string_from_cstr(char* s) {\n");
fprintf(out, "  flower_string out;\n");
fprintf(out, "  out.data = s;\n");
fprintf(out, "  out.length = (int)strlen(s);\n");
fprintf(out, "  return out;\n");
fprintf(out, "}\n");
fprintf(out, "static int flower_string_eq(flower_string a, flower_string b) {\n");
fprintf(out, "  if (a.length != b.length) return 0;\n");
fprintf(out, "  return !strncmp(a.data, b.data, a.length);\n");
fprintf(out, "}\n");
fprintf(out, "static void flower_print_string(flower_string s) {\n");
fprintf(out, "  printf(\"%%.*s\", s.length, s.data);\n");
fprintf(out, "}\n;\n");
}


void mod_src_codegen_flo_emit_includes(ModuleSet* mods, FILE* out) {
int need_stdlib = 0;
int need_stdio = 0;
int need_string = 0;
int need_ctype = 0;
int need_unistd = 0;
for (int i = 0; ((0) > (mods->count)) ? i > (mods->count) : i < (mods->count); ((0) > (mods->count)) ? i-- : i++) {
Module* m = mods->modules + i;
if (mod_src_parser_flo_token_stream_contains(m->tokens, TOKEN_NULL)  ||  mod_src_parser_flo_token_stream_contains(m->tokens, TOKEN_NEW)) {
has_stdlib = 1;
}
if (mod_src_parser_flo_token_stream_contains(m->tokens, TOKEN_PRINT)) {
has_stdio = 1;
}
if (mod_src_parser_flo_token_stream_contains(m->tokens, TOKEN_STRING)) {
need_string = 1;
}
AST* curr = m->ast;
while (curr != NULL) {
if (curr->kind == AST_IMPORT  &&  curr->data._import.is_system) {
char name[64];
snprintf(name, sizeof(name), "%.*s", curr->data._import.path_length, m->src + curr->data._import.path_start);
if (!(strcmp(name, "stdio"))) {
need_stdio = 1;
}
else if (!(strcmp(name, "stdlib"))) {
need_stdlib = 1;
}
else if (!(strcmp(name, "string"))) {
need_stdio = 1;
need_string = 1;
}
else if (!(strcmp(name, "ctype"))) {
need_ctype = 1;
}
else if (!(strcmp(name, "unistd"))) {
need_unistd = 1;
}
}
curr = curr->next;
}
}
if (need_stdlib) {
fprintf(out, "#include <stdlib.h>\n");
}
if (need_stdio) {
fprintf(out, "#include <stdio.h>\n");
}
if (need_string) {
fprintf(out, "#include <string.h>\n");
}
if (need_ctype) {
fprintf(out, "#include <ctype.h>\n");
}
if (need_unistd) {
fprintf(out, "#include <unistd.h>\n");
}
}


char* mod_src_codegen_flo_token_to_string(int kind) {
if (kind == TOKEN_INT) {
return "int";
}
else if (kind == TOKEN_BOOL) {
return "int";
}
else if (kind == TOKEN_FLOAT) {
return "float";
}
else if (kind == TOKEN_DOUBLE) {
return "double";
}
else if (kind == TOKEN_CHAR) {
return "char";
}
else if (kind == TOKEN_STRING) {
return "string";
}
else if (kind == TOKEN_VOID) {
return "void";
}
else if (kind == TOKEN_RETURN) {
return "return";
}
else if (kind == TOKEN_WHILE) {
return "while";
}
else if (kind == TOKEN_IF) {
return "if";
}
else if (kind == TOKEN_ELSE) {
return "else";
}
else if (kind == TOKEN_END) {
return "}";
}
else if (kind == TOKEN_OR) {
return " || ";
}
else if (kind == TOKEN_AND) {
return " && ";
}
else if (kind == TOKEN_DOT) {
return ".";
}
else if (kind == TOKEN_PLUS) {
return "+";
}
else if (kind == TOKEN_MINUS) {
return "-";
}
else if (kind == TOKEN_STAR) {
return "*";
}
else if (kind == TOKEN_SLASH) {
return "/";
}
else if (kind == TOKEN_CARET) {
return "^";
}
else if (kind == TOKEN_AT) {
return "@";
}
else if (kind == TOKEN_AMPERSAND) {
return "&";
}
else if (kind == TOKEN_ASSIGN) {
return "=";
}
else if (kind == TOKEN_LT) {
return "<";
}
else if (kind == TOKEN_GT) {
return ">";
}
else if (kind == TOKEN_GEQ) {
return ">=";
}
else if (kind == TOKEN_LEQ) {
return "<=";
}
else if (kind == TOKEN_NEQ) {
return "!=";
}
else if (kind == TOKEN_COMP) {
return "==";
}
else if (kind == TOKEN_LPAREN) {
return "(";
}
else if (kind == TOKEN_RPAREN) {
return ")";
}
else if (kind == TOKEN_LBRACE) {
return "{";
}
else if (kind == TOKEN_RBRACE) {
return "}";
}
else if (kind == TOKEN_COLON) {
return ":";
}
else if (kind == TOKEN_COMMA) {
return ",";
}
else if (kind == TOKEN_SEMI) {
return "";
}
else if (kind == TOKEN_NEWLINE) {
return "\n";
}
else {
return "ERROR";
}
}


void mod_src_codegen_flo_typeinfo_to_string(TypeInfo* type, FILE* out, char* src) {
if (type->base == TOKEN_IDENTIFIER) {
fprintf(out, "%.*s", type->name_length, src + type->name_start);
}
else if (type->base == TOKEN_STRING) {
fprintf(out, "flower_string");
}
else {
fprintf(out, "%s", mod_src_codegen_flo_token_to_string(type->base));
}
int i = 0;
while (i < type->pointer_depth) {
i = i + 1;
fprintf(out, "*");
}
}
void mod_src_codegen_flo_gen_expr(AST* ast, FILE* out, char* src);
void mod_src_codegen_flo_gen_statement(AST* ast, FILE* out, char* src);


void mod_src_codegen_flo_gen_if(AST* ast, FILE* out, char* src, int is_else_if) {
if (is_else_if) {
if (ast->data._if_condition.condition == NULL) {
fprintf(out, "else {\n");
}
else {
fprintf(out, "else if (");
mod_src_codegen_flo_gen_expr(ast->data._if_condition.condition, out, src);
fprintf(out, ") {\n");
}
}
else {
fprintf(out, "if (");
mod_src_codegen_flo_gen_expr(ast->data._if_condition.condition, out, src);
fprintf(out, ") {\n");
}
AST* statement = ast->data._if_condition.body;
while (statement != NULL) {
mod_src_codegen_flo_gen_statement(statement, out, src);
statement = statement->next;
}
fprintf(out, "}\n");
if (ast->data._if_condition.else_branch != NULL) {
mod_src_codegen_flo_gen_if(ast->data._if_condition.else_branch, out, src, 1);
}
}


void mod_src_codegen_flo_gen_while(AST* ast, FILE* out, char* src) {
fprintf(out, "while (");
mod_src_codegen_flo_gen_expr(ast->data._while_loop.condition, out, src);
fprintf(out, ") {\n");
AST* statement = ast->data._while_loop.body;
while (statement != NULL) {
mod_src_codegen_flo_gen_statement(statement, out, src);
statement = statement->next;
}
fprintf(out, "}\n");
}


void mod_src_codegen_flo_gen_for(AST* ast, FILE* out, char* src) {
char* desc_op;
char* asc_op;
if (ast->data._for_loop.inclusive) {
desc_op = ">=";
asc_op = "<=";
}
else {
desc_op = ">";
asc_op = "<";
}
char* add_op = "++";
char* sub_op = "--";
AST* from = ast->data._for_loop.from;
AST* to = ast->data._for_loop.to;
if (from->kind == AST_LITERAL  &&  to->kind == AST_LITERAL) {
int reverse;
if (from->value > to->value) {
reverse = 1;
}
else {
reverse = 0;
}
char* op;
char* math_op;
if (reverse) {
op = desc_op;
math_op = sub_op;
}
else {
op = asc_op;
math_op = add_op;
}
fprintf(out, "for (int %.*s = ", ast->data._for_loop.var_length, src + ast->data._for_loop.var_start);
mod_src_codegen_flo_gen_expr(ast->data._for_loop.from, out, src);
fprintf(out, "; %.*s %s ", ast->data._for_loop.var_length, src + ast->data._for_loop.var_start, op);
mod_src_codegen_flo_gen_expr(ast->data._for_loop.to, out, src);
fprintf(out, "; %.*s%s) {\n", ast->data._for_loop.var_length, src + ast->data._for_loop.var_start, math_op);
}
else {
fprintf(out, "for (int %.*s = ", ast->data._for_loop.var_length, src + ast->data._for_loop.var_start);
mod_src_codegen_flo_gen_expr(ast->data._for_loop.from, out, src);
fprintf(out, "; ((");
mod_src_codegen_flo_gen_expr(ast->data._for_loop.from, out, src);
fprintf(out, ") > (");
mod_src_codegen_flo_gen_expr(ast->data._for_loop.to, out, src);
fprintf(out, ")) ? %.*s %s (", ast->data._for_loop.var_length, src + ast->data._for_loop.var_start, desc_op);
mod_src_codegen_flo_gen_expr(ast->data._for_loop.to, out, src);
fprintf(out, ") : %.*s %s (", ast->data._for_loop.var_length, src + ast->data._for_loop.var_start, asc_op);
mod_src_codegen_flo_gen_expr(ast->data._for_loop.to, out, src);
fprintf(out, "); ((");
mod_src_codegen_flo_gen_expr(ast->data._for_loop.from, out, src);
fprintf(out, ") > (");
mod_src_codegen_flo_gen_expr(ast->data._for_loop.to, out, src);
fprintf(out, ")) ? %.*s-- : %.*s++) {\n", ast->data._for_loop.var_length, src + ast->data._for_loop.var_start, ast->data._for_loop.var_length, src + ast->data._for_loop.var_start);
}
AST* statement = ast->data._for_loop.body;
while (statement != NULL) {
mod_src_codegen_flo_gen_statement(statement, out, src);
statement = statement->next;
}
fprintf(out, "}\n");
}


void mod_src_codegen_flo_gen_return(AST* ast, FILE* out, char* src) {
if (ast->data._flow_ctrl.value == NULL) {
fprintf(out, "return;");
}
else {
fprintf(out, "return ");
mod_src_codegen_flo_gen_expr(ast->data._flow_ctrl.value, out, src);
fprintf(out, ";\n");
}
}


void mod_src_codegen_flo_gen_continue(AST* ast, FILE* out, char* src) {
fprintf(out, "continue;\n");
}


void mod_src_codegen_flo_gen_break(AST* ast, FILE* out, char* src) {
fprintf(out, "break;\n");
}


typedef struct defined_struct {
char* name;
int length;
} defined_struct;
defined_struct defined_structs[256];
int defined_count = 0;


void mod_src_codegen_flo_gen_struct(AST* ast, FILE* out, char* src) {
for (int i = 0; ((0) > (defined_count)) ? i > (defined_count) : i < (defined_count); ((0) > (defined_count)) ? i-- : i++) {
if (defined_structs[i].length == ast->data._struct_def.name_length  &&  !(strncmp(defined_structs[i].name, ast->data._struct_def.name_start + src, ast->data._struct_def.name_length))) {
return;}
}
fprintf(out, "\n\ntypedef struct %.*s {\n", ast->data._struct_def.name_length, src + ast->data._struct_def.name_start);
AST* field = ast->data._struct_def.fields;
while (field != NULL) {
mod_src_codegen_flo_typeinfo_to_string(&(field->data._struct_field.type), out, src);
fprintf(out, " %.*s", field->data._struct_field.name_length, src + field->data._struct_field.name_start);
if (field->data._struct_field.type.array_size == -1) {
fprintf(out, "[]");
}
else if (field->data._struct_field.type.array_size > 0) {
fprintf(out, "[%d]", field->data._struct_field.type.array_size);
}
else if (field->data._struct_field.type.arr_size_expr != NULL) {
fprintf(out, "[");
mod_src_codegen_flo_gen_expr(field->data._struct_field.type.arr_size_expr, out, src);
fprintf(out, "]");
}
fprintf(out, ";\n");
field = field->next;
}
fprintf(out, "} %.*s;\n", ast->data._struct_def.name_length, src + ast->data._struct_def.name_start);
defined_structs[defined_count].name = strndup(src + ast->data._struct_def.name_start, ast->data._struct_def.name_length);
defined_structs[defined_count].length = ast->data._struct_def.name_length;
defined_count = defined_count + 1;
}


typedef struct defined_union {
char* name;
int length;
} defined_union;
defined_union defined_unions[256];
int defined_union_count = 0;


void mod_src_codegen_flo_gen_union(AST* ast, FILE* out, char* src) {
for (int i = 0; ((0) > (defined_union_count)) ? i > (defined_union_count) : i < (defined_union_count); ((0) > (defined_union_count)) ? i-- : i++) {
if (defined_unions[i].length == ast->data._union_def.name_length  &&  !(strncmp(defined_unions[i].name, ast->data._union_def.name_start + src, ast->data._union_def.name_length))) {
return;}
}
fprintf(out, "\n\ntypedef union %.*s {\n", ast->data._union_def.name_length, src + ast->data._union_def.name_start);
AST* field = ast->data._union_def.fields;
while (field != NULL) {
mod_src_codegen_flo_typeinfo_to_string(&(field->data._struct_field.type), out, src);
fprintf(out, " %.*s", field->data._union_def.name_length, src + field->data._union_def.name_start);
if (field->data._struct_field.type.array_size == -1) {
fprintf(out, "[]");
}
else if (field->data._struct_field.type.array_size > 0) {
fprintf(out, "[%d]", ast->data._struct_field.type.array_size);
}
else if (field->data._struct_field.type.arr_size_expr != NULL) {
fprintf(out, "[");
mod_src_codegen_flo_gen_expr(field->data._struct_field.type.arr_size_expr, out, src);
fprintf(out, "]");
}
fprintf(out, ";\n");
field = field->next;
}
fprintf(out, "} %.*s;\n", ast->data._union_def.name_length, src + ast->data._union_def.name_start);
defined_unions[defined_union_count].name = strndup(src + ast->data._union_def.name_start, ast->data._union_def.name_length);
defined_unions[defined_union_count].length = ast->data._union_def.name_length;
defined_union_count = defined_union_count + 1;
}


typedef struct EmittedImport {
char* path;
char* alias;
} EmittedImport;
EmittedImport emitted_imports[256];
int emitted_count = 0;
ModuleSet* active_modules;
Module* current_codegen_module;
void mod_src_codegen_flo_emit_lowered_func_name(Module* mod, FILE* out, char* src, int start, int length);
Module* mod_src_codegen_flo_resolve_called_function_module(char* src, int start, int length);


void mod_src_codegen_flo_gen_param(AST* param, FILE* out, char* src) {
mod_src_codegen_flo_typeinfo_to_string(&(param->data._func_params.type), out, src);
fprintf(out, " %.*s", param->data._func_params.name_length, src + param->data._func_params.name_start);
if (param->data._func_params.type.array_size == -1) {
fprintf(out, "[]");
}
else if (param->data._func_params.type.array_size != 0) {
fprintf(out, "[%d]", param->data._func_params.type.array_size);
}
else if (param->data._func_params.type.arr_size_expr != NULL) {
fprintf(out, "[");
mod_src_codegen_flo_gen_expr(param->data._func_params.type.arr_size_expr, out, src);
fprintf(out, "]");
}
}


void mod_src_codegen_flo_gen_func_def(AST* ast, FILE* out, char* src) {
fprintf(out, "\n\n");
mod_src_codegen_flo_typeinfo_to_string(&(ast->data._func_def.return_type), out, src);
fprintf(out, " ");
mod_src_codegen_flo_emit_lowered_func_name(current_codegen_module, out, src, ast->data._func_def.name_start, ast->data._func_def.name_length);
fprintf(out, "(");
AST* param = ast->data._func_def.params;
while (param != NULL) {
mod_src_codegen_flo_gen_param(param, out, src);
if (param->next != NULL) {
fprintf(out, ", ");
}
param = param->next;
}
fprintf(out, ") {\n");
AST* statement = ast->data._func_def.body;
while (statement != NULL) {
mod_src_codegen_flo_gen_statement(statement, out, src);
statement = statement->next;
}
fprintf(out, "}\n");
}


void mod_src_codegen_flo_emit_lowered_func_name(Module* mod, FILE* out, char* src, int start, int length) {
if (mod != NULL) {
Module* root = active_modules->modules + 0;
if (!(strcmp(mod->path, root->path))  &&  length == 4  &&  !(strncmp(src + start, "main", 4))) {
fprintf(out, "main");
return;}
fprintf(out, "%s_%.*s", mod->symbol_prefix, length, src + start);
return;}
fprintf(out, "%.*s", length, src + start);
}


Module* mod_src_codegen_flo_resolve_called_function_module(char* src, int start, int length) {
if (current_codegen_module != NULL  &&  mod_src_module_flo_module_defines_func(current_codegen_module, src, start, length)) {
return current_codegen_module;
}
return mod_src_module_flo_find_function_module(active_modules, src, start, length);
}


void mod_src_codegen_flo_gen_func_def_aliased(AST* ast, FILE* out, char* src, char* alias, int alias_len) {
mod_src_codegen_flo_gen_func_def(ast, out, src);
}


void mod_src_codegen_flo_gen_func_call(AST* ast, FILE* out, char* src) {
Module* target_mod = mod_src_codegen_flo_resolve_called_function_module(src, ast->data._func_call.name_start, ast->data._func_call.name_length);
if (target_mod != NULL) {
mod_src_codegen_flo_emit_lowered_func_name(target_mod, out, src, ast->data._func_call.name_start, ast->data._func_call.name_length);
}
else {
fprintf(out, "%.*s", ast->data._func_call.name_length, src + ast->data._func_call.name_start);
}
fprintf(out, "(");
AST* arg = ast->data._func_call.args;
while (arg != NULL) {
mod_src_codegen_flo_gen_expr(arg, out, src);
if (arg->next != NULL) {
fprintf(out, ", ");
}
arg = arg->next;
}
fprintf(out, ")");
}


void mod_src_codegen_flo_gen_var_decl(AST* ast, FILE* out, char* src) {
mod_src_codegen_flo_typeinfo_to_string(&(ast->data._var_decl.type), out, src);
fprintf(out, " %.*s", ast->data._var_decl.name_length, src + ast->data._var_decl.name_start);
if (ast->data._var_decl.type.array_size == -1) {
fprintf(out, "[]");
}
else if (ast->data._var_decl.type.array_size != 0) {
fprintf(out, "[%d]", ast->data._var_decl.type.array_size);
}
else if (ast->data._var_decl.type.arr_size_expr != NULL) {
fprintf(out, "[");
mod_src_codegen_flo_gen_expr(ast->data._var_decl.type.arr_size_expr, out, src);
fprintf(out, "]");
}
if (ast->data._var_decl.value != NULL) {
fprintf(out, " = ");
mod_src_codegen_flo_gen_expr(ast->data._var_decl.value, out, src);
}
fprintf(out, ";\n");
}


void mod_src_codegen_flo_gen_var_ass(AST* ast, FILE* out, char* src) {
fprintf(out, "%.*s = ", ast->data._var_ass.name_length, src + ast->data._var_ass.name_start);
mod_src_codegen_flo_gen_expr(ast->data._var_ass.value, out, src);
fprintf(out, ";\n");
}


void mod_src_codegen_flo_gen_forward(AST* ast, FILE* out, char* src) {
if (ast->data._forward.is_func) {
mod_src_codegen_flo_typeinfo_to_string(&(ast->data._forward.return_type), out, src);
fprintf(out, " ");
mod_src_codegen_flo_emit_lowered_func_name(current_codegen_module, out, src, ast->data._forward.name_start, ast->data._forward.name_length);
fprintf(out, "(");
AST* param = ast->data._forward.params;
while (param != NULL) {
mod_src_codegen_flo_gen_param(param, out, src);
if (param->next != NULL) {
fprintf(out, ", ");
}
param = param->next;
}
fprintf(out, ");\n");
}
else {
fprintf(out, "typedef struct %.*s %.*s;\n", ast->data._forward.name_length, src + ast->data._forward.name_start, ast->data._forward.name_length, src + ast->data._forward.name_start);
}
}
void mod_src_codegen_flo_gen_import(AST* ast, FILE* out, char* src);
void mod_src_codegen_flo_emit_module(Module* mod, FILE* out, char* alias, int alias_len, int has_alias);


void mod_src_codegen_flo_emit_module(Module* mod, FILE* out, char* alias, int alias_len, int has_alias) {
char* original = current_file;
current_file = mod->path;
Module* original_module = current_codegen_module;
current_codegen_module = mod;
AST* curr = mod->ast;
while (curr != NULL) {
if (curr->kind == AST_IMPORT) {
mod_src_codegen_flo_gen_import(curr, out, mod->src);
}
else if (curr->kind == AST_STRUCT_DEF) {
mod_src_codegen_flo_gen_struct(curr, out, mod->src);
}
else if (curr->kind == AST_UNION_DEF) {
mod_src_codegen_flo_gen_union(curr, out, mod->src);
}
else if (curr->kind == AST_PROP) {
AST* decl = curr->data._prop.decl;
if (decl->kind == AST_FUNC_DEF) {
mod_src_codegen_flo_gen_func_def(decl, out, mod->src);
}
else if (decl->kind == AST_STRUCT_DEF) {
mod_src_codegen_flo_gen_struct(decl, out, mod->src);
}
else if (decl->kind == AST_UNION_DEF) {
mod_src_codegen_flo_gen_union(decl, out, mod->src);
}
}
else if (curr->kind == AST_FORWARD) {
mod_src_codegen_flo_gen_forward(curr, out, mod->src);
}
else if (curr->kind == AST_FUNC_DEF) {
mod_src_codegen_flo_gen_func_def(curr, out, mod->src);
}
else if (curr->kind == AST_VAR_DECL) {
mod_src_codegen_flo_gen_var_decl(curr, out, mod->src);
}
curr = curr->next;
}
current_file = original;
current_codegen_module = original_module;
}


void mod_src_codegen_flo_gen_import(AST* ast, FILE* out, char* src) {
if (ast->data._import.is_system) {
return;}
char raw_path[256];
snprintf(raw_path, sizeof(raw_path), "%.*s", ast->data._import.path_length - 2, src + ast->data._import.path_start + 1);
char* path = mod_src_globals_flo_resolve_path(current_file, raw_path);
char alias[64];
alias[0] = '\0';
if (ast->data._import.has_alias) {
snprintf(alias, sizeof(alias), "%.*s", ast->data._import.alias_length, src + ast->data._import.alias_start);
}
for (int i = 0; ((0) > (emitted_count)) ? i > (emitted_count) : i < (emitted_count); ((0) > (emitted_count)) ? i-- : i++) {
if (!(strcmp(emitted_imports[i].path, path))) {
free(path);
return;}
}
Module* imported = mod_src_module_flo_find_module(active_modules, path);
if (imported == NULL) {
printf("%sCould not find loaded import module: %s%s", RED, RESET, path);
free(path);
return;}
emitted_imports[emitted_count].path = strdup(path);
emitted_imports[emitted_count].alias = strdup(alias);
emitted_count = emitted_count + 1;
mod_src_codegen_flo_emit_module(imported, out, "", 0, 0);
free(path);
}


void mod_src_codegen_flo_gen_expr(AST* ast, FILE* out, char* src) {
if (ast->kind == AST_VAR_REF) {
fprintf(out, "%.*s", ast->data._var_ref.name_length, src + ast->data._var_ref.name_start);
}
else if (ast->kind == AST_LITERAL) {
fprintf(out, "%d", ast->value);
}
else if (ast->kind == AST_FLOAT_LIT) {
fprintf(out, "%.*s", ast->data._float_lit.length, src + ast->data._float_lit.start);
}
else if (ast->kind == AST_BOOL_LIT) {
fprintf(out, "%d", ast->data._bool_lit.value);
}
else if (ast->kind == AST_BOOL_LIT) {
fprintf(out, "%d", ast->data._bool_lit.value);
}
else if (ast->kind == AST_NULL) {
fprintf(out, "NULL");
}
else if (ast->kind == AST_BINARY_OP) {
if (ast->data._binary.is_string_compare) {
if (ast->data._binary.op == TOKEN_NEQ) {
fprintf(out, "(!flower_string_eq(");
}
else {
fprintf(out, "flower_string_eq(");
}
mod_src_codegen_flo_gen_expr(ast->data._binary.left, out, src);
fprintf(out, ", ");
mod_src_codegen_flo_gen_expr(ast->data._binary.right, out, src);
if (ast->data._binary.op == TOKEN_NEQ) {
fprintf(out, "))");
}
else {
fprintf(out, ")");
}
}
else {
mod_src_codegen_flo_gen_expr(ast->data._binary.left, out, src);
fprintf(out, " %s ", mod_src_codegen_flo_token_to_string(ast->data._binary.op));
mod_src_codegen_flo_gen_expr(ast->data._binary.right, out, src);
}
}
else if (ast->kind == AST_FUNC_CALL) {
mod_src_codegen_flo_gen_func_call(ast, out, src);
}
else if (ast->kind == AST_STRING_LIT) {
if (ast->data._string.lower_kind == STRING_LOWER_CSTR) {
fprintf(out, "%.*s", ast->data._string.str_length, src + ast->data._string.str_start);
}
else {
fprintf(out, "((flower_string){ ");
fprintf(out, "%.*s", ast->data._string.str_length, src + ast->data._string.str_start);
fprintf(out, ", (int)(sizeof(");
fprintf(out, "%.*s", ast->data._string.str_length, src + ast->data._string.str_start);
fprintf(out, ") - 1) })");
}
}
else if (ast->kind == AST_ARRAY_LIT) {
fprintf(out, "{");
AST* elem = ast->data._array.elements;
while (elem != NULL) {
mod_src_codegen_flo_gen_expr(elem, out, src);
if (elem->next != NULL) {
fprintf(out, ", ");
}
elem = elem->next;
}
fprintf(out, "}");
}
else if (ast->kind == AST_STRUCT_LIT) {
fprintf(out, "{");
AST* struct_elem = ast->data._struct_lit.elements;
while (struct_elem != NULL) {
mod_src_codegen_flo_gen_expr(struct_elem, out, src);
if (struct_elem->next != NULL) {
fprintf(out, ", ");
struct_elem = struct_elem->next;
}
}
fprintf(out, "}");
}
else if (ast->kind == AST_CAST) {
if (ast->data._cast.type.base == TOKEN_CHAR  &&  ast->data._cast.type.pointer_depth == 1  &&  ast->data._cast.type.array_size == 0  &&  ast->data._cast.value_type.base == TOKEN_STRING  &&  ast->data._cast.value_type.pointer_depth == 0  &&  ast->data._cast.value_type.array_size == 0) {
fprintf(out, "(");
mod_src_codegen_flo_gen_expr(ast->data._cast.value, out, src);
fprintf(out, ").data");
}
else if (ast->data._cast.type.base == TOKEN_STRING  &&  ast->data._cast.type.pointer_depth == 0  &&  ast->data._cast.type.array_size == 0  &&  ast->data._cast.value_type.base == TOKEN_CHAR  &&  ast->data._cast.value_type.pointer_depth == 1  &&  ast->data._cast.value_type.array_size == 0) {
fprintf(out, "flower_string_from_cstr(");
mod_src_codegen_flo_gen_expr(ast->data._cast.value, out, src);
fprintf(out, ")");
}
else if (ast->data._cast.type.base == TOKEN_BOOL  &&  ast->data._cast.type.pointer_depth == 0  &&  ast->data._cast.type.array_size == 0) {
fprintf(out, "((");
mod_src_codegen_flo_gen_expr(ast->data._cast.value, out, src);
fprintf(out, ") != 0");
}
else {
fprintf(out, "(");
mod_src_codegen_flo_typeinfo_to_string(&(ast->data._cast.type), out, src);
fprintf(out, ")(");
mod_src_codegen_flo_gen_expr(ast->data._cast.value, out, src);
fprintf(out, ")");
}
}
else if (ast->kind == AST_DOT_ACCESS) {
AST* object = ast->data._dot_access.object;
mod_src_codegen_flo_gen_expr(object, out, src);
int obj_kind = ast->data._dot_access.access_kind;
if (obj_kind == ACCESS_ARROW) {
fprintf(out, "->%.*s", ast->data._dot_access.field_length, src + ast->data._dot_access.field_start);
}
else if (obj_kind == ACCESS_DOT) {
fprintf(out, ".%.*s", ast->data._dot_access.field_length, src + ast->data._dot_access.field_start);
}
else {
int obj_kind = object->kind;
if (obj_kind == AST_DOT_ACCESS) {
if (object->data._dot_access.resolved_type.pointer_depth > 0) {
fprintf(out, "->%.*s", ast->data._dot_access.field_length, src + ast->data._dot_access.field_start);
}
else {
fprintf(out, ".%.*s", ast->data._dot_access.field_length, src + ast->data._dot_access.field_start);
}
}
else if (obj_kind == AST_SUBSCRIPT) {
fprintf(out, ".%.*s", ast->data._dot_access.field_length, src + ast->data._dot_access.field_start);
}
else {
fprintf(out, "->%.*s", ast->data._dot_access.field_length, src + ast->data._dot_access.field_start);
}
}
}
else if (ast->kind == AST_NEW) {
fprintf(out, "malloc(sizeof(");
mod_src_codegen_flo_typeinfo_to_string(&(ast->data._new_alloc.type), out, src);
fprintf(out, ")");
if (ast->data._new_alloc.type.arr_size_expr != NULL) {
fprintf(out, " * (");
mod_src_codegen_flo_gen_expr(ast->data._new_alloc.type.arr_size_expr, out, src);
fprintf(out, ")");
}
else if (ast->data._new_alloc.type.array_size > 0) {
fprintf(out, " * %d", ast->data._new_alloc.type.array_size);
}
fprintf(out, ")");
}
else if (ast->kind == AST_ALIAS_CALL) {
Module* imported = mod_src_module_flo_find_imported_module(active_modules, current_codegen_module, src, ast->data._alias_call.alias_start, ast->data._alias_call.alias_length);
if (imported != NULL) {
mod_src_codegen_flo_emit_lowered_func_name(imported, out, src, ast->data._alias_call.func_start, ast->data._alias_call.func_length);
}
else {
fprintf(out, "%.*s_%.*s", ast->data._alias_call.alias_length, ast->data._alias_call.alias_start + src, ast->data._alias_call.func_length, src + ast->data._alias_call.func_start);
}
fprintf(out, "(");
AST* arg = ast->data._alias_call.args;
while (arg != NULL) {
mod_src_codegen_flo_gen_expr(arg, out, src);
if (arg->next != NULL) {
fprintf(out, ", ");
}
arg = arg->next;
}
fprintf(out, ")");
}
else if (ast->kind == AST_UNARY_NOT) {
fprintf(out, "!(");
mod_src_codegen_flo_gen_expr(ast->data._unary.operand, out, src);
fprintf(out, ")");
}
else if (ast->kind == AST_UNARY_NEG) {
fprintf(out, "-");
mod_src_codegen_flo_gen_expr(ast->data._unary.operand, out, src);
}
else if (ast->kind == AST_DEREF) {
fprintf(out, "*(");
mod_src_codegen_flo_gen_expr(ast->data._deref.operand, out, src);
fprintf(out, ")");
}
else if (ast->kind == AST_GET_ADDR) {
fprintf(out, "&(");
mod_src_codegen_flo_gen_expr(ast->data._get_addr.operand, out, src);
fprintf(out, ")");
}
else if (ast->kind == AST_SIZEOF) {
fprintf(out, "sizeof(");
mod_src_codegen_flo_typeinfo_to_string(&(ast->data._size_of.type), out, src);
fprintf(out, ")");
}
else if (ast->kind == AST_CHAR_LIT) {
fprintf(out, "%.*s", ast->data._char_lit.length, ast->data._char_lit.start + src);
}
else if (ast->kind == AST_SUBSCRIPT) {
mod_src_codegen_flo_gen_expr(ast->data._subscript.array, out, src);
fprintf(out, "[");
mod_src_codegen_flo_gen_expr(ast->data._subscript.index, out, src);
fprintf(out, "]");
}
else {
printf("gen_expr received node addr: %p, kind: %d\n", ast, ast->kind);
exit(1);
}
}


void mod_src_codegen_flo_gen_statement(AST* ast, FILE* out, char* src) {
if (ast->kind == AST_VAR_DECL) {
mod_src_codegen_flo_gen_var_decl(ast, out, src);
}
else if (ast->kind == AST_VAR_ASS) {
mod_src_codegen_flo_gen_var_ass(ast, out, src);
}
else if (ast->kind == AST_WHILE) {
mod_src_codegen_flo_gen_while(ast, out, src);
}
else if (ast->kind == AST_FOR) {
mod_src_codegen_flo_gen_for(ast, out, src);
}
else if (ast->kind == AST_FLOW_CONTROL) {
Token* base = ast->data._flow_ctrl.base;
if (base->kind == TOKEN_RETURN) {
mod_src_codegen_flo_gen_return(ast, out, src);
}
else if (base->kind == TOKEN_CONTINUE) {
mod_src_codegen_flo_gen_continue(ast, out, src);
}
else if (base->kind == TOKEN_BREAK) {
mod_src_codegen_flo_gen_break(ast, out, src);
}
else {
Token* base = ast->data._flow_ctrl.base;
printf("%s%s:%d:%d: error:%s Unexpected token: %s\n", RED, current_file, mod_include_lexer_h_flo_get_line(src, base->start), mod_include_lexer_h_flo_get_col(src, base->start), RESET, mod_src_lexer_flo_token_kind_name(base->kind));
exit(1);
}
}
else if (ast->kind == AST_FUNC_CALL) {
mod_src_codegen_flo_gen_func_call(ast, out, src);
fprintf(out, ";\n");
}
else if (ast->kind == AST_IF) {
mod_src_codegen_flo_gen_if(ast, out, src, 0);
}
else if (ast->kind == AST_DOT_ACCESS) {
AST* object = ast->data._dot_access.object;
mod_src_codegen_flo_gen_expr(object, out, src);
int obj_kind = ast->data._dot_access.access_kind;
if (obj_kind == ACCESS_ARROW) {
fprintf(out, "->%.*s", ast->data._dot_access.field_length, src + ast->data._dot_access.field_start);
}
else if (obj_kind == ACCESS_DOT) {
fprintf(out, ".%.*s", ast->data._dot_access.field_length, src + ast->data._dot_access.field_start);
}
else {
int obj_kind = object->kind;
if (obj_kind == AST_DOT_ACCESS) {
if (object->data._dot_access.resolved_type.pointer_depth > 0) {
fprintf(out, "->%.*s", ast->data._dot_access.field_length, src + ast->data._dot_access.field_start);
}
else {
fprintf(out, ".%.*s", ast->data._dot_access.field_length, src + ast->data._dot_access.field_start);
}
}
else if (obj_kind == AST_SUBSCRIPT) {
fprintf(out, ".%.*s", ast->data._dot_access.field_length, src + ast->data._dot_access.field_start);
}
else {
fprintf(out, "->%.*s", ast->data._dot_access.field_length, src + ast->data._dot_access.field_start);
}
}
if (ast->data._dot_access.value != NULL) {
fprintf(out, " = ");
mod_src_codegen_flo_gen_expr(ast->data._dot_access.value, out, src);
fprintf(out, ";\n");
}
}
else if (ast->kind == AST_PRINT) {
if (ast->data._print.value_type.base == TOKEN_STRING  &&  ast->data._print.value_type.pointer_depth == 0  &&  ast->data._print.value_type.array_size == 0) {
fprintf(out, "flower_print_string(");
mod_src_codegen_flo_gen_expr(ast->data._print.value, out, src);
fprintf(out, ");\n");
}
else if (ast->data._print.value_type.base == TOKEN_CHAR  &&  ast->data._print.value_type.pointer_depth == 1  &&  ast->data._print.value_type.array_size == 0) {
fprintf(out, "printf(\"%%s\", ");
mod_src_codegen_flo_gen_expr(ast->data._print.value, out, src);
fprintf(out, ");\n");
}
else if (ast->data._print.value_type.base == TOKEN_CHAR  &&  ast->data._print.value_type.pointer_depth == 0) {
fprintf(out, "printf(\"%%c\", ");
mod_src_codegen_flo_gen_expr(ast->data._print.value, out, src);
fprintf(out, ");\n");
}
else if (ast->data._print.value_type.base == TOKEN_FLOAT  ||  ast->data._print.value_type.base == TOKEN_DOUBLE) {
fprintf(out, "printf(\"%%f\", ");
mod_src_codegen_flo_gen_expr(ast->data._print.value, out, src);
fprintf(out, ");\n");
}
else {
fprintf(out, "printf(");
mod_src_codegen_flo_gen_expr(ast->data._print.value, out, src);
fprintf(out, ");\n");
}
}
else if (ast->kind == AST_PRUNE) {
fprintf(out, "free(");
mod_src_codegen_flo_gen_expr(ast->data._prune_free.ptr, out, src);
fprintf(out, ");\n");
}
else if (ast->kind == AST_DEREF_ASS) {
fprintf(out, "*%.*s = ", ast->data._deref_ass.name_length, src + ast->data._deref_ass.name_start);
mod_src_codegen_flo_gen_expr(ast->data._deref_ass.value, out, src);
fprintf(out, ";\n");
}
else if (ast->kind == AST_SUBSCRIPT) {
mod_src_codegen_flo_gen_expr(ast->data._subscript.array, out, src);
fprintf(out, "[");
mod_src_codegen_flo_gen_expr(ast->data._subscript.index, out, src);
fprintf(out, "]");
if (ast->data._subscript.value != NULL) {
fprintf(out, " = ");
mod_src_codegen_flo_gen_expr(ast->data._subscript.value, out, src);
}
fprintf(out, ";\n");
}
else if (ast->kind == AST_ALIAS_CALL) {
mod_src_codegen_flo_gen_expr(ast, out, src);
fprintf(out, ";\n");
}
else {
printf("Unknown statement kind: %d\n", ast->kind);
exit(1);
}
}


void mod_src_codegen_flo_codegen(ModuleSet* mods, FILE* out) {
active_modules = mods;
if (mods->count == 0) {
return;}
Module* main_mod = mods->modules + 0;
mod_src_codegen_flo_emit_module(main_mod, out, "", 0, 0);
}


int main(int argc, char* argv[]) {
getcwd(project_root, sizeof(project_root));
if (argc < 2) {
printf("%sMissing arguments! Use -help for usage instructions.%s\n", RED, RESET);
return -1;
}
for (int i = 1; ((1) > (argc)) ? i > (argc) : i < (argc); ((1) > (argc)) ? i-- : i++) {
char* arg = argv[i];
if (arg[0] == '-') {
while (arg[0] == '-') {
arg = arg + 1;
}
for (int i = 0; ((0) > (strlen(arg))) ? i > (strlen(arg)) : i < (strlen(arg)); ((0) > (strlen(arg))) ? i-- : i++) {
arg[i] = tolower((char)(arg[i]));
}
if (!(strcmp(arg, "help"))  ||  !(strcmp(arg, "h"))) {
printf("%s🌸 Welcome to Flower Compiler!\n\n%s", GREEN, RESET);
printf("%sUsage:\n%s", BLUE, RESET);
flower_print_string(((flower_string){ "\tflower\t[options] <filepath>\n\n", (int)(sizeof("\tflower\t[options] <filepath>\n\n") - 1) }));
printf("%sOptions:\n%s", BLUE, RESET);
flower_print_string(((flower_string){ "\t-help,    -h\tShow this help message\n", (int)(sizeof("\t-help,    -h\tShow this help message\n") - 1) }));
flower_print_string(((flower_string){ "\t-version, -v\tShow the current version of FloC\n", (int)(sizeof("\t-version, -v\tShow the current version of FloC\n") - 1) }));
flower_print_string(((flower_string){ "\t<filepath>\tSpecify the source code file to compile\n\n", (int)(sizeof("\t<filepath>\tSpecify the source code file to compile\n\n") - 1) }));
printf("%sExample:\n%s", BLUE, RESET);
flower_print_string(((flower_string){ "\tflower -help\n", (int)(sizeof("\tflower -help\n") - 1) }));
flower_print_string(((flower_string){ "\tflower main.flo\n\n", (int)(sizeof("\tflower main.flo\n\n") - 1) }));
printf("%sTips:\n%s", BLUE, RESET);
flower_print_string(((flower_string){ " - You can use any number of dashes before a flag, e.g., ---help\n", (int)(sizeof(" - You can use any number of dashes before a flag, e.g., ---help\n") - 1) }));
flower_print_string(((flower_string){ " - Flags are case-insensitive: -HELP works too!\n\n", (int)(sizeof(" - Flags are case-insensitive: -HELP works too!\n\n") - 1) }));
printf("%sHappy Compiling with Flower! 🌼\n%s", GREEN, RESET);
continue;
}
else if (!(strcmp(arg, "version"))  ||  !(strcmp(arg, "v"))) {
flower_print_string(((flower_string){ "Version: 0.0.4\n", (int)(sizeof("Version: 0.0.4\n") - 1) }));
continue;
}
else {
printf("%sUnrecognized flag argument! Use -help for more information.\n%s", RED, RESET);
}
}
else {
printf("%sCompiling file: %s%s\n", BLUE, argv[i], RESET);
char abs_path[512];
realpath(argv[i], abs_path);
mod_src_globals_flo_set_current_file(abs_path);
flower_print_string(((flower_string){ "Initializing modules..\n", (int)(sizeof("Initializing modules..\n") - 1) }));
ModuleSet* mods = malloc(sizeof(ModuleSet));
mod_src_module_flo_init_modules(mods);
Module* main_mod = mod_src_module_flo_load_module(mods, abs_path);
if (main_mod == NULL) {
return -1;
}
flower_print_string(((flower_string){ "Checking types..\n", (int)(sizeof("Checking types..\n") - 1) }));
int type_errors = mod_src_typecheck_flo_check_modules(mods);
if (type_errors > 0) {
printf("%sTypecheck failed with %d error(s)%s\n", RED, type_errors, RESET);
return -1;
}
char out_c[512];
if (i + 1 < argc  &&  argv[i + 1][0] != '-') {
snprintf(out_c, sizeof(out_c), "%s.c", argv[i + 1]);
}
else {
snprintf(out_c, sizeof(out_c), "output.c");
}
FILE* output = fopen(out_c, "w");
if (!(output)) {
printf("%sFailed to open output file\n%s", RED, RESET);
return -1;
}
int has_output = 0;
for (int j = 0; ((0) > (mods->count)) ? j > (mods->count) : j < (mods->count); ((0) > (mods->count)) ? j-- : j++) {
Module* m = mods->modules + j;
if (mod_src_parser_flo_token_stream_contains(m->tokens, TOKEN_PRINT)) {
has_output = 1;
}
}
flower_print_string(((flower_string){ "Codegen...\n", (int)(sizeof("Codegen...\n") - 1) }));
fflush(stdout);
mod_src_codegen_flo_emit_string_runtime(output);
mod_src_codegen_flo_emit_includes(mods, output);
mod_src_codegen_flo_codegen(mods, output);
free(mods);
fclose(output);
char bin_name[512];
snprintf(bin_name, sizeof(bin_name), "%.*s", (int)(strlen(out_c) - 2), out_c);
char build_cmd[1024];
snprintf(build_cmd, sizeof(build_cmd), "clang %s -o %s", out_c, bin_name);
system(build_cmd);
if (has_output) {
printf("%s\nOUTPUT:\n", YELLOW);
fflush(stdout);
printf("%s", RESET);
fflush(stdout);
snprintf(build_cmd, sizeof(build_cmd), "./%s", bin_name);
}
system(build_cmd);
flower_print_string(((flower_string){ "\n", (int)(sizeof("\n") - 1) }));
printf("%sCompiled %s → %s%s\n", GREEN, out_c, bin_name, RESET);
break;
}
}
}
