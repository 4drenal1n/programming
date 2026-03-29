#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

//  Типы токенов 

typedef enum {
    TOK_NUMBER,
    TOK_IDENTIFIER,
    TOK_OPERATOR,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_EOF,
    TOK_ERROR
} TokType;

//  Токен 

typedef struct {
    TokType type;
    union {
        double number;
        char identifier[64];
        char op;
    } value;
    int line;
    int column;
} Token;

//  Состояния лексера 

typedef enum {
    STATE_START,
    STATE_IN_NUMBER,
    STATE_IN_IDENTIFIER,
    STATE_IN_OPERATOR,
    STATE_ERROR,
    STATE_DONE
} LexerState;

//  Лексер 

typedef struct {
    const char *input;
    size_t pos;
    int line;
    int column;
    LexerState state;
    size_t input_len;
} Lexer;

//  Вспомогательные функции 

static char current_char(const Lexer *lex) {
    if (lex->pos >= lex->input_len) return '\0';
    return lex->input[lex->pos];
}

static char peek_char(const Lexer *lex, int offset) {
    size_t pos = lex->pos + offset;
    if (pos >= lex->input_len) return '\0';
    return lex->input[pos];
}

static void advance(Lexer *lex) {
    if (lex->pos < lex->input_len) {
        if (lex->input[lex->pos] == '\n') {
            lex->line++;
            lex->column = 1;
        } else {
            lex->column++;
        }
        lex->pos++;
    }
}

static int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' ||
           c == '=' || c == '<' || c == '>' || c == '!' ||
           c == '&' || c == '|' || c == '^' || c == '%' ||
           c == '(' || c == ')' || c == '{' || c == '}' ||
           c == '[' || c == ']' || c == ',' || c == ';';
}

static int is_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

static int is_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

static int is_digit(char c) {
    return c >= '0' && c <= '9';
}

// Инициализация 

void lexer_init(Lexer *lex, const char *input) {
    if (!lex || !input) return;
    lex->input = input;
    lex->pos = 0;
    lex->line = 1;
    lex->column = 1;
    lex->state = STATE_START;
    lex->input_len = strlen(input);
}

//  Пропуск пробелов 

void lexer_skip_whitespace(Lexer *lex) {
    while (current_char(lex) != '\0' && isspace(current_char(lex))) {
        advance(lex);
    }
}

//  Пропуск комментариев 

static void lexer_skip_comment(Lexer *lex) {
    if (current_char(lex) == '/') {
        if (peek_char(lex, 1) == '/') {
            // Однострочный комментарий //
            while (current_char(lex) != '\0' && current_char(lex) != '\n') {
                advance(lex);
            }
        } else if (peek_char(lex, 1) == '*') {
            // Многострочный комментарий /* */
            advance(lex); // пропускаем '*'
            while (current_char(lex) != '\0') {
                if (current_char(lex) == '*' && peek_char(lex, 1) == '/') {
                    advance(lex); // '/'
                    advance(lex);
                    return;
                }
                advance(lex);
            }
        }
    }
}

// Создание токенов 

static Token make_token(TokType type, int line, int column) {
    Token tok;
    tok.type = type;
    tok.line = line;
    tok.column = column;
    return tok;
}

static Token make_number_token(double value, int line, int column) {
    Token tok = make_token(TOK_NUMBER, line, column);
    tok.value.number = value;
    return tok;
}

static Token make_identifier_token(const char *str, int line, int column) {
    Token tok = make_token(TOK_IDENTIFIER, line, column);
    strncpy(tok.value.identifier, str, sizeof(tok.value.identifier) - 1);
    tok.value.identifier[sizeof(tok.value.identifier) - 1] = '\0';
    return tok;
}

static Token make_operator_token(char op, int line, int column) {
    Token tok = make_token(TOK_OPERATOR, line, column);
    tok.value.op = op;
    return tok;
}

static Token make_error_token(const char *msg, int line, int column) {
    Token tok = make_token(TOK_ERROR, line, column);
    strncpy(tok.value.identifier, msg, sizeof(tok.value.identifier) - 1);
    tok.value.identifier[sizeof(tok.value.identifier) - 1] = '\0';
    return tok;
}

// Получение следующего токена 

Token lexer_next_token(Lexer *lex) {
    if (!lex) return make_error_token("NULL lexer", 0, 0);

    while (1) {
        // Пропускаем пробелы
        lexer_skip_whitespace(lex);

        // Пропускаем комментарии
        lexer_skip_comment(lex);

        // Если после пропуска снова пробелы - повторяем
        if (isspace(current_char(lex))) continue;

        break;
    }

    int start_line = lex->line;
    int start_col = lex->column;

    // Конец ввода
    if (current_char(lex) == '\0') {
        lex->state = STATE_DONE;
        return make_token(TOK_EOF, start_line, start_col);
    }

    char c = current_char(lex);

    // Числа (включая числа с плавающей точкой)
    if (is_digit(c) || (c == '.' && is_digit(peek_char(lex, 1)))) {
        char num_buf[64];
        int idx = 0;
        int has_dot = 0;

        while (current_char(lex) != '\0' &&
               (is_digit(current_char(lex)) ||
                (current_char(lex) == '.' && !has_dot))) {
            if (current_char(lex) == '.') has_dot = 1;
            if (idx < (int)sizeof(num_buf) - 1) {
                num_buf[idx++] = current_char(lex);
            }
            advance(lex);
        }
        num_buf[idx] = '\0';

        lex->state = STATE_IN_NUMBER;
        return make_number_token(atof(num_buf), start_line, start_col);
    }

    // Идентификаторы и ключевые слова
    if (is_identifier_start(c)) {
        char id_buf[64];
        int idx = 0;

        while (current_char(lex) != '\0' && is_identifier_char(current_char(lex))) {
            if (idx < (int)sizeof(id_buf) - 1) {
                id_buf[idx++] = current_char(lex);
            }
            advance(lex);
        }
        id_buf[idx] = '\0';

        lex->state = STATE_IN_IDENTIFIER;
        return make_identifier_token(id_buf, start_line, start_col);
    }

    // Операторы и скобки
    if (is_operator_char(c)) {
        advance(lex);

        // Проверка на двухсимвольные операторы
        char next = current_char(lex);
        char two_char[3] = {c, next, '\0'};

        int is_two_char_op = (strcmp(two_char, "==") == 0 ||
                              strcmp(two_char, "!=") == 0 ||
                              strcmp(two_char, "<=") == 0 ||
                              strcmp(two_char, ">=") == 0 ||
                              strcmp(two_char, "&&") == 0 ||
                              strcmp(two_char, "||") == 0 ||
                              strcmp(two_char, "++") == 0 ||
                              strcmp(two_char, "--") == 0);

        if (is_two_char_op && next != '\0') {
            advance(lex);
            
            return make_operator_token(c, start_line, start_col);
        }

        // Скобки
        if (c == '(') {
            return make_token(TOK_LPAREN, start_line, start_col);
        }
        if (c == ')') {
            return make_token(TOK_RPAREN, start_line, start_col);
        }

        lex->state = STATE_IN_OPERATOR;
        return make_operator_token(c, start_line, start_col);
    }

    // Нераспознанный символ
    lex->state = STATE_ERROR;
    return make_error_token("Неизвестный символ", start_line, start_col);
}

// Заглянуть в следующий токен 

Token lexer_peek(Lexer *lex) {
    if (!lex) return make_error_token("NULL lexer", 0, 0);

    // Сохраняем состояние
    Lexer saved = *lex;
    Token tok = lexer_next_token(lex);
    // Восстанавливаем состояние
    *lex = saved;

    return tok;
}

//  Вывод токена 

const char* token_type_name(TokType type) {
    switch (type) {
        case TOK_NUMBER:     return "NUMBER";
        case TOK_IDENTIFIER: return "IDENTIFIER";
        case TOK_OPERATOR:   return "OPERATOR";
        case TOK_LPAREN:     return "LPAREN";
        case TOK_RPAREN:     return "RPAREN";
        case TOK_EOF:        return "EOF";
        case TOK_ERROR:      return "ERROR";
        default:             return "UNKNOWN";
    }
}

void token_print(const Token *tok) {
    if (!tok) return;

    switch (tok->type) {
        case TOK_NUMBER:
            printf("[%s] %.6g (line %d, col %d)\n",
                   token_type_name(tok->type), tok->value.number,
                   tok->line, tok->column);
            break;
        case TOK_IDENTIFIER:
            printf("[%s] %s (line %d, col %d)\n",
                   token_type_name(tok->type), tok->value.identifier,
                   tok->line, tok->column);
            break;
        case TOK_OPERATOR:
            printf("[%s] %c (line %d, col %d)\n",
                   token_type_name(tok->type), tok->value.op,
                   tok->line, tok->column);
            break;
        case TOK_LPAREN:
            printf("[%s] ( (line %d, col %d)\n",
                   token_type_name(tok->type), tok->line, tok->column);
            break;
        case TOK_RPAREN:
            printf("[%s] ) (line %d, col %d)\n",
                   token_type_name(tok->type), tok->line, tok->column);
            break;
        case TOK_EOF:
            printf("[%s] (line %d, col %d)\n",
                   token_type_name(tok->type), tok->line, tok->column);
            break;
        case TOK_ERROR:
            printf("[%s] %s (line %d, col %d)\n",
                   token_type_name(tok->type), tok->value.identifier,
                   tok->line, tok->column);
            break;
    }
}

//  Токенизация всей строки 

size_t tokenize(const char *input, Token *tokens, size_t max_tokens) {
    if (!input || !tokens || max_tokens == 0) return 0;

    Lexer lex;
    lexer_init(&lex, input);

    size_t count = 0;
    while (count < max_tokens) {
        tokens[count] = lexer_next_token(&lex);
        count++;

        if (tokens[count - 1].type == TOK_EOF ||
            tokens[count - 1].type == TOK_ERROR) {
            break;
        }
    }

    return count;
}

//  Демонстрация 

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    printf("=== Конечный автомат (Lexer) ===\n\n");

    // Пример 1: простое выражение
    const char *input1 = "x = 3.14 * (y + 2)";
    printf("Вход: \"%s\"\n", input1);
    printf("Токены:\n");

    Lexer lex1;
    lexer_init(&lex1, input1);
    Token tok;
    do {
        tok = lexer_next_token(&lex1);
        token_print(&tok);
    } while (tok.type != TOK_EOF && tok.type != TOK_ERROR);

    // Пример 2: сложное выражение с комментариями
    printf("\n");
    const char *input2 = "result = (a + b) * 2 // это комментарий";
    printf("Вход: \"%s\"\n", input2);
    printf("Токены:\n");

    Lexer lex2;
    lexer_init(&lex2, input2);
    do {
        tok = lexer_next_token(&lex2);
        token_print(&tok);
    } while (tok.type != TOK_EOF && tok.type != TOK_ERROR);

    // Пример 3: многострочный комментарий
    printf("\n");
    const char *input3 = "x = 10 /* комментарий\nмногострочный */ + y";
    printf("Вход: \"%s\"\n", input3);
    printf("Токены:\n");

    Lexer lex3;
    lexer_init(&lex3, input3);
    do {
        tok = lexer_next_token(&lex3);
        token_print(&tok);
    } while (tok.type != TOK_EOF && tok.type != TOK_ERROR);

    // Пример 4: токенизация в массив
    printf("\n");
    const char *input4 = "a + b * c - d / e";
    printf("Вход: \"%s\"\n", input4);

    Token tokens[50];
    size_t count = tokenize(input4, tokens, sizeof(tokens) / sizeof(tokens[0]));

    printf("Всего токенов: %zu\n", count);
    printf("Токены:\n");
    for (size_t i = 0; i < count; i++) {
        token_print(&tokens[i]);
    }

    // Пример 5: lexer_peek
    printf("\n");
    const char *input5 = "x + y";
    printf("Вход: \"%s\"\n", input5);

    Lexer lex5;
    lexer_init(&lex5, input5);

    Token peeked = lexer_peek(&lex5);
    printf("lexer_peek: ");
    token_print(&peeked);

    Token first = lexer_next_token(&lex5);
    printf("lexer_next_token (1): ");
    token_print(&first);

    peeked = lexer_peek(&lex5);
    printf("lexer_peek: ");
    token_print(&peeked);

    // Пример 6: ошибки
    printf("\n");
    const char *input6 = "x = 5 @ y";
    printf("Вход: \"%s\"\n", input6);

    Lexer lex6;
    lexer_init(&lex6, input6);
    do {
        tok = lexer_next_token(&lex6);
        token_print(&tok);
    } while (tok.type != TOK_EOF && tok.type != TOK_ERROR);

    return 0;
}
