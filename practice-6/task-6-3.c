#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <stdbool.h>

// Типы выражений
typedef enum {
    EXPR_NUMBER,
    EXPR_VARIABLE,
    EXPR_ADD,
    EXPR_SUB,
    EXPR_MUL,
    EXPR_DIV,
    EXPR_NEG,    // Унарный минус
    EXPR_SQRT,
    EXPR_POW
} ExprType;

// Структура выражения (объявление)
typedef struct Expr Expr;

// Структура выражения (определение)
struct Expr {
    ExprType type;
    union {
        double number;              // Для EXPR_NUMBER
        char variable;              // Для EXPR_VARIABLE ('x', 'y', ...)
        struct {
            Expr *left;
            Expr *right;
        } binary;                   // Для бинарных операций
        Expr *operand;              // Для унарных операций
    } data;
};

// Контекст для переменных
typedef struct {
    char name;
    double value;
} Variable;

typedef struct {
    Variable vars[26];  // a-z
    int count;
} Context;

//Конструкторы 

Expr* expr_number(double value) {
    Expr *e = (Expr*)malloc(sizeof(Expr));
    if (!e) return NULL;
    e->type = EXPR_NUMBER;
    e->data.number = value;
    return e;
}

Expr* expr_variable(char name) {
    Expr *e = (Expr*)malloc(sizeof(Expr));
    if (!e) return NULL;
    // Проверка: имя переменной должно быть буквой a-z или A-Z
    if (!((name >= 'a' && name <= 'z') || (name >= 'A' && name <= 'Z'))) {
        free(e);
        return NULL;
    }
    e->type = EXPR_VARIABLE;
    e->data.variable = name;
    return e;
}

Expr* expr_add(Expr *left, Expr *right) {
    if (!left || !right) return NULL;
    Expr *e = (Expr*)malloc(sizeof(Expr));
    if (!e) return NULL;
    e->type = EXPR_ADD;
    e->data.binary.left = left;
    e->data.binary.right = right;
    return e;
}

Expr* expr_sub(Expr *left, Expr *right) {
    if (!left || !right) return NULL;
    Expr *e = (Expr*)malloc(sizeof(Expr));
    if (!e) return NULL;
    e->type = EXPR_SUB;
    e->data.binary.left = left;
    e->data.binary.right = right;
    return e;
}

Expr* expr_mul(Expr *left, Expr *right) {
    if (!left || !right) return NULL;
    Expr *e = (Expr*)malloc(sizeof(Expr));
    if (!e) return NULL;
    e->type = EXPR_MUL;
    e->data.binary.left = left;
    e->data.binary.right = right;
    return e;
}

Expr* expr_div(Expr *left, Expr *right) {
    if (!left || !right) return NULL;
    Expr *e = (Expr*)malloc(sizeof(Expr));
    if (!e) return NULL;
    e->type = EXPR_DIV;
    e->data.binary.left = left;
    e->data.binary.right = right;
    return e;
}

Expr* expr_neg(Expr *operand) {
    if (!operand) return NULL;
    Expr *e = (Expr*)malloc(sizeof(Expr));
    if (!e) return NULL;
    e->type = EXPR_NEG;
    e->data.operand = operand;
    return e;
}

Expr* expr_sqrt(Expr *operand) {
    if (!operand) return NULL;
    Expr *e = (Expr*)malloc(sizeof(Expr));
    if (!e) return NULL;
    e->type = EXPR_SQRT;
    e->data.operand = operand;
    return e;
}

Expr* expr_pow(Expr *base, Expr *exp) {
    if (!base || !exp) return NULL;
    Expr *e = (Expr*)malloc(sizeof(Expr));
    if (!e) return NULL;
    e->type = EXPR_POW;
    e->data.binary.left = base;
    e->data.binary.right = exp;
    return e;
}

// Контекст 

void context_init(Context *ctx);
void context_set(Context *ctx, char name, double value);
double context_get(const Context *ctx, char name);

// Освобождение памяти (объявление)
void expr_free(Expr *e);

void context_init(Context *ctx) {
    ctx->count = 0;
}

void context_set(Context *ctx, char name, double value) {
    // Ищем существующую переменную
    for (int i = 0; i < ctx->count; i++) {
        if (ctx->vars[i].name == name) {
            ctx->vars[i].value = value;
            return;
        }
    }
    // Добавляем новую
    if (ctx->count < 26) {
        ctx->vars[ctx->count].name = name;
        ctx->vars[ctx->count].value = value;
        ctx->count++;
    }
}

double context_get(const Context *ctx, char name) {
    for (int i = 0; i < ctx->count; i++) {
        if (ctx->vars[i].name == name) {
            return ctx->vars[i].value;
        }
    }
    return 0.0;  // Переменная не найдена
}

//  Вычисление выражения 

double expr_eval(const Expr *e, const Context *ctx) {
    if (!e || !ctx) return 0.0;
    
    switch (e->type) {
        case EXPR_NUMBER:
            return e->data.number;
        case EXPR_VARIABLE:
            return context_get(ctx, e->data.variable);
        case EXPR_ADD:
            return expr_eval(e->data.binary.left, ctx) + expr_eval(e->data.binary.right, ctx);
        case EXPR_SUB:
            return expr_eval(e->data.binary.left, ctx) - expr_eval(e->data.binary.right, ctx);
        case EXPR_MUL:
            return expr_eval(e->data.binary.left, ctx) * expr_eval(e->data.binary.right, ctx);
        case EXPR_DIV: {
            double denom = expr_eval(e->data.binary.right, ctx);
            if (denom == 0.0) {
                printf("Ошибка: деление на ноль!\n");
                return 0.0;
            }
            return expr_eval(e->data.binary.left, ctx) / denom;
        }
        case EXPR_NEG:
            return -expr_eval(e->data.operand, ctx);
        case EXPR_SQRT: {
            double val = expr_eval(e->data.operand, ctx);
            if (val < 0.0) {
                printf("Ошибка: квадратный корень из отрицательного числа!\n");
                return 0.0;
            }
            return sqrt(val);
        }
        case EXPR_POW:
            return pow(expr_eval(e->data.binary.left, ctx), expr_eval(e->data.binary.right, ctx));
        default:
            return 0.0;
    }
}

// Вывод выражения 

static void expr_print_recursive(const Expr *e, int precedence) {
    if (!e) return;
    
    int my_prec = 0;

    switch (e->type) {
        case EXPR_NUMBER:
            printf("%.2f", e->data.number);
            return;
        case EXPR_VARIABLE:
            printf("%c", e->data.variable);
            return;
        case EXPR_ADD:
        case EXPR_SUB:
            my_prec = 1;
            break;
        case EXPR_MUL:
        case EXPR_DIV:
            my_prec = 2;
            break;
        case EXPR_NEG:
        case EXPR_SQRT:
            my_prec = 3;
            break;
        case EXPR_POW:
            my_prec = 4;
            break;
        default:
            return;
    }

    int need_parens = (precedence > my_prec);

    if (need_parens) printf("(");

    switch (e->type) {
        case EXPR_ADD:
            expr_print_recursive(e->data.binary.left, my_prec);
            printf(" + ");
            expr_print_recursive(e->data.binary.right, my_prec + 1);
            break;
        case EXPR_SUB:
            expr_print_recursive(e->data.binary.left, my_prec);
            printf(" - ");
            expr_print_recursive(e->data.binary.right, my_prec + 1);
            break;
        case EXPR_MUL:
            expr_print_recursive(e->data.binary.left, my_prec);
            printf(" * ");
            expr_print_recursive(e->data.binary.right, my_prec + 1);
            break;
        case EXPR_DIV:
            expr_print_recursive(e->data.binary.left, my_prec);
            printf(" / ");
            expr_print_recursive(e->data.binary.right, my_prec + 1);
            break;
        case EXPR_NEG:
            printf("-");
            expr_print_recursive(e->data.operand, my_prec);
            break;
        case EXPR_SQRT:
            printf("sqrt(");
            expr_print_recursive(e->data.operand, 0);
            printf(")");
            break;
        case EXPR_POW:
            expr_print_recursive(e->data.binary.left, my_prec);
            printf(" ^ ");
            expr_print_recursive(e->data.binary.right, my_prec + 1);
            break;
        default:
            break;
    }

    if (need_parens) printf(")");
}

void expr_print(const Expr *e) {
    if (e) expr_print_recursive(e, 0);
}

// Дифференцирование 

Expr* expr_derivative(const Expr *e, char var) {
    if (!e) return NULL;
    
    switch (e->type) {
        case EXPR_NUMBER:
            // d(c)/dx = 0
            return expr_number(0.0);

        case EXPR_VARIABLE:
            // d(x)/dx = 1, d(y)/dx = 0
            return (e->data.variable == var) ? expr_number(1.0) : expr_number(0.0);

        case EXPR_ADD: {
            // d(f+g)/dx = df/dx + dg/dx
            Expr *dl = expr_derivative(e->data.binary.left, var);
            Expr *dr = expr_derivative(e->data.binary.right, var);
            return expr_add(dl, dr);
        }

        case EXPR_SUB: {
            // d(f-g)/dx = df/dx - dg/dx
            Expr *dl = expr_derivative(e->data.binary.left, var);
            Expr *dr = expr_derivative(e->data.binary.right, var);
            return expr_sub(dl, dr);
        }

        case EXPR_MUL: {
            // d(f*g)/dx = df/dx * g + f * dg/dx (правило произведения)
            Expr *dl = expr_derivative(e->data.binary.left, var);
            Expr *dr = expr_derivative(e->data.binary.right, var);
            Expr *left = expr_mul(dl, e->data.binary.right);
            Expr *right = expr_mul(e->data.binary.left, dr);
            return expr_add(left, right);
        }

        case EXPR_DIV: {
            // d(f/g)/dx = (df*g - f*dg) / g^2 (правило частного)
            Expr *dl = expr_derivative(e->data.binary.left, var);
            Expr *dr = expr_derivative(e->data.binary.right, var);
            Expr *num = expr_sub(expr_mul(dl, e->data.binary.right),
                                  expr_mul(e->data.binary.left, dr));
            Expr *den = expr_mul(e->data.binary.right, e->data.binary.right);
            return expr_div(num, den);
        }

        case EXPR_NEG: {
            // d(-f)/dx = -df/dx
            return expr_neg(expr_derivative(e->data.operand, var));
        }

        case EXPR_SQRT: {
            // d(sqrt(f))/dx = (1/(2*sqrt(f))) * df/dx
            Expr *inner_deriv = expr_derivative(e->data.operand, var);
            Expr *sqrt_f = expr_sqrt(e->data.operand);
            Expr *two = expr_number(2.0);
            Expr *denom = expr_mul(two, sqrt_f);
            return expr_mul(expr_div(expr_number(1.0), denom), inner_deriv);
        }

        case EXPR_POW: {
            // d(f^g)/dx = f^g * (g' * ln(f) + g * f'/f)
            // Для простоты считаем, что степень - константа
            Expr *base = e->data.binary.left;
            Expr *exp = e->data.binary.right;
            Expr *db = expr_derivative(base, var);
            Expr *de = expr_derivative(exp, var);

            // Проверяем, является ли степень константой
            if (exp->type == EXPR_NUMBER) {
                double n = exp->data.number;
                if (n == 0.0) {
                    // d(c^0)/dx = 0
                    return expr_number(0.0);
                }
                // d(f^n)/dx = n * f^(n-1) * f'
                Expr *new_exp = expr_number(n - 1.0);
                Expr *new_pow = expr_pow(base, new_exp);
                Expr *coef = expr_mul(expr_number(n), new_pow);
                return expr_mul(coef, db);
            } else {
                // Общий случай: d(f^g)/dx = f^g * (g' * ln(f) + g * f'/f)
                Expr *pow_val = expr_pow(base, exp);
                Expr *ln_f = expr_number(log(2.718281828));  
                Expr *term1 = expr_mul(de, ln_f);
                Expr *term2 = expr_mul(exp, expr_div(db, base));
                return expr_mul(pow_val, expr_add(term1, term2));
            }
        }

        default:
            return expr_number(0.0);
    }
}

//  Упрощение выражения 

Expr* expr_simplify(Expr *e) {
    if (!e) return NULL;

    // Сначала упрощаем (switch по тегу)
    switch (e->type) {
        case EXPR_ADD:
        case EXPR_SUB:
        case EXPR_MUL:
        case EXPR_DIV:
        case EXPR_POW:
            e->data.binary.left = expr_simplify(e->data.binary.left);
            e->data.binary.right = expr_simplify(e->data.binary.right);
            break;
        case EXPR_NEG:
        case EXPR_SQRT:
            e->data.operand = expr_simplify(e->data.operand);
            break;
        default:
            break;
    }

    // Применяем правила упрощения
    switch (e->type) {
        case EXPR_ADD: {
            // 0 + x = x, x + 0 = x
            if (e->data.binary.left->type == EXPR_NUMBER &&
                e->data.binary.left->data.number == 0.0) {
                Expr *right = e->data.binary.right;
                free(e);
                return right;
            }
            if (e->data.binary.right->type == EXPR_NUMBER &&
                e->data.binary.right->data.number == 0.0) {
                Expr *left = e->data.binary.left;
                free(e);
                return left;
            }
            // Числа: a + b = c
            if (e->data.binary.left->type == EXPR_NUMBER &&
                e->data.binary.right->type == EXPR_NUMBER) {
                double result = e->data.binary.left->data.number +
                               e->data.binary.right->data.number;
                expr_free(e->data.binary.left);
                expr_free(e->data.binary.right);
                e->type = EXPR_NUMBER;
                e->data.number = result;
            }
            break;
        }

        case EXPR_SUB: {
            // x - 0 = x
            if (e->data.binary.right->type == EXPR_NUMBER &&
                e->data.binary.right->data.number == 0.0) {
                Expr *left = e->data.binary.left;
                free(e);
                return left;
            }
            // 0 - x = -x
            if (e->data.binary.left->type == EXPR_NUMBER &&
                e->data.binary.left->data.number == 0.0) {
                Expr *right = e->data.binary.right;
                free(e);
                return expr_neg(right);
            }
            // Числа: a - b = c
            if (e->data.binary.left->type == EXPR_NUMBER &&
                e->data.binary.right->type == EXPR_NUMBER) {
                double result = e->data.binary.left->data.number -
                               e->data.binary.right->data.number;
                expr_free(e->data.binary.left);
                expr_free(e->data.binary.right);
                e->type = EXPR_NUMBER;
                e->data.number = result;
            }
            break;
        }

        case EXPR_MUL: {
            // 0 * x = 0, x * 0 = 0
            if ((e->data.binary.left->type == EXPR_NUMBER &&
                 e->data.binary.left->data.number == 0.0) ||
                (e->data.binary.right->type == EXPR_NUMBER &&
                 e->data.binary.right->data.number == 0.0)) {
                expr_free(e->data.binary.left);
                expr_free(e->data.binary.right);
                e->type = EXPR_NUMBER;
                e->data.number = 0.0;
                return e;
            }
            // 1 * x = x, x * 1 = x
            if (e->data.binary.left->type == EXPR_NUMBER &&
                e->data.binary.left->data.number == 1.0) {
                Expr *right = e->data.binary.right;
                free(e);
                return right;
            }
            if (e->data.binary.right->type == EXPR_NUMBER &&
                e->data.binary.right->data.number == 1.0) {
                Expr *left = e->data.binary.left;
                free(e);
                return left;
            }
            // Числа: a * b = c
            if (e->data.binary.left->type == EXPR_NUMBER &&
                e->data.binary.right->type == EXPR_NUMBER) {
                double result = e->data.binary.left->data.number *
                               e->data.binary.right->data.number;
                expr_free(e->data.binary.left);
                expr_free(e->data.binary.right);
                e->type = EXPR_NUMBER;
                e->data.number = result;
            }
            break;
        }

        case EXPR_DIV: {
            // 0 / x = 0 (x != 0)
            if (e->data.binary.left->type == EXPR_NUMBER &&
                e->data.binary.left->data.number == 0.0) {
                expr_free(e->data.binary.left);
                expr_free(e->data.binary.right);
                e->type = EXPR_NUMBER;
                e->data.number = 0.0;
                return e;
            }
            // x / 1 = x
            if (e->data.binary.right->type == EXPR_NUMBER &&
                e->data.binary.right->data.number == 1.0) {
                Expr *left = e->data.binary.left;
                free(e);
                return left;
            }
            // x / x = 1 (если одинаковые переменные)
            if (e->data.binary.left->type == EXPR_VARIABLE &&
                e->data.binary.right->type == EXPR_VARIABLE &&
                e->data.binary.left->data.variable == e->data.binary.right->data.variable) {
                expr_free(e->data.binary.left);
                expr_free(e->data.binary.right);
                e->type = EXPR_NUMBER;
                e->data.number = 1.0;
                return e;
            }
            // Числа: a / b = c
            if (e->data.binary.left->type == EXPR_NUMBER &&
                e->data.binary.right->type == EXPR_NUMBER) {
                if (e->data.binary.right->data.number != 0.0) {
                    double result = e->data.binary.left->data.number /
                                   e->data.binary.right->data.number;
                    expr_free(e->data.binary.left);
                    expr_free(e->data.binary.right);
                    e->type = EXPR_NUMBER;
                    e->data.number = result;
                }
            }
            break;
        }

        case EXPR_NEG: {
            // -(-x) = x
            if (e->data.operand->type == EXPR_NEG) {
                Expr *inner = e->data.operand->data.operand;
                free(e->data.operand);
                free(e);
                return inner;
            }
            // -(число) = новое число
            if (e->data.operand->type == EXPR_NUMBER) {
                double result = -e->data.operand->data.number;
                expr_free(e->data.operand);
                e->type = EXPR_NUMBER;
                e->data.number = result;
            }
            break;
        }

        case EXPR_SQRT: {
            // sqrt(число)
            if (e->data.operand->type == EXPR_NUMBER &&
                e->data.operand->data.number >= 0.0) {
                double result = sqrt(e->data.operand->data.number);
                expr_free(e->data.operand);
                e->type = EXPR_NUMBER;
                e->data.number = result;
            }
            break;
        }

        case EXPR_POW: {
            // x ^ 0 = 1
            if (e->data.binary.right->type == EXPR_NUMBER &&
                e->data.binary.right->data.number == 0.0) {
                expr_free(e->data.binary.left);
                expr_free(e->data.binary.right);
                e->type = EXPR_NUMBER;
                e->data.number = 1.0;
                return e;
            }
            // x ^ 1 = x
            if (e->data.binary.right->type == EXPR_NUMBER &&
                e->data.binary.right->data.number == 1.0) {
                Expr *left = e->data.binary.left;
                expr_free(e->data.binary.right);
                free(e);
                return left;
            }
            // 0 ^ x = 0 (x > 0)
            if (e->data.binary.left->type == EXPR_NUMBER &&
                e->data.binary.left->data.number == 0.0) {
                expr_free(e->data.binary.left);
                expr_free(e->data.binary.right);
                e->type = EXPR_NUMBER;
                e->data.number = 0.0;
                return e;
            }
            // 1 ^ x = 1
            if (e->data.binary.left->type == EXPR_NUMBER &&
                e->data.binary.left->data.number == 1.0) {
                expr_free(e->data.binary.left);
                expr_free(e->data.binary.right);
                e->type = EXPR_NUMBER;
                e->data.number = 1.0;
                return e;
            }
            // Числа: a ^ b = c
            if (e->data.binary.left->type == EXPR_NUMBER &&
                e->data.binary.right->type == EXPR_NUMBER) {
                double result = pow(e->data.binary.left->data.number,
                                   e->data.binary.right->data.number);
                expr_free(e->data.binary.left);
                expr_free(e->data.binary.right);
                e->type = EXPR_NUMBER;
                e->data.number = result;
            }
            break;
        }

        default:
            break;
    }

    return e;
}

//  Освобождение памяти 
void expr_free(Expr *e) {
    if (!e) return;
    
    switch (e->type) {
        case EXPR_NUMBER:
        case EXPR_VARIABLE:
            
            break;
        case EXPR_ADD:
        case EXPR_SUB:
        case EXPR_MUL:
        case EXPR_DIV:
        case EXPR_POW:
            expr_free(e->data.binary.left);
            expr_free(e->data.binary.right);
            break;
        case EXPR_NEG:
        case EXPR_SQRT:
            expr_free(e->data.operand);
            break;
    }
    
    free(e);
}

// Демонстрация 

int main() {
    SetConsoleOutputCP(CP_UTF8);
    
    printf("=== Калькулятор с AST ===\n\n");

    // Пример 1: (x + 3) * (x - 2) при x = 5
    printf("--- Пример 1: (x + 3) * (x - 2) ---\n");
    Expr *x = expr_variable('x');
    Expr *three = expr_number(3.0);
    Expr *two = expr_number(2.0);

    Expr *expr1 = expr_mul(expr_add(x, three), expr_sub(expr_variable('x'), two));
    
    printf("Выражение: ");
    expr_print(expr1);
    printf("\n");
    
    Context ctx;
    context_init(&ctx);
    context_set(&ctx, 'x', 5.0);
    
    double result = expr_eval(expr1, &ctx);
    printf("При x = 5: %.2f\n", result);
    printf("Ожидаемо: (5 + 3) * (5 - 2) = 8 * 3 = 24\n\n");
    
    // Пример 2: Производная
    printf("--- Пример 2: Производная ---\n");
    Expr *x2 = expr_variable('x');
    Expr *expr2 = expr_mul(expr_add(x2, expr_number(3.0)), 
                           expr_sub(expr_variable('x'), expr_number(2.0)));
    
    printf("Исходное выражение: ");
    expr_print(expr2);
    printf("\n");
    
    Expr *deriv = expr_derivative(expr2, 'x');
    printf("Производная по x: ");
    expr_print(deriv);
    printf("\n");
    
    Expr *deriv_simplified = expr_simplify(deriv);
    printf("Упрощённая производная: ");
    expr_print(deriv_simplified);
    printf("\n\n");
    
    // Пример 3: Упрощение
    printf("--- Пример 3: Упрощение выражений ---\n");
    
    // 0 + x = x
    Expr *simp1 = expr_add(expr_number(0.0), expr_variable('x'));
    printf("0 + x = ");
    expr_print(expr_simplify(simp1));
    printf("\n");
    
    // x * 1 = x
    Expr *simp2 = expr_mul(expr_variable('y'), expr_number(1.0));
    printf("y * 1 = ");
    expr_print(expr_simplify(simp2));
    printf("\n");
    
    // x / x = 1
    Expr *simp3 = expr_div(expr_variable('z'), expr_variable('z'));
    printf("z / z = ");
    expr_print(expr_simplify(simp3));
    printf("\n");
    
    // -(-x) = x
    Expr *simp4 = expr_neg(expr_neg(expr_variable('a')));
    printf("-(-a) = ");
    expr_print(expr_simplify(simp4));
    printf("\n");
    
    // sqrt(16) = 4
    Expr *simp5 = expr_sqrt(expr_number(16.0));
    printf("sqrt(16) = ");
    expr_print(expr_simplify(simp5));
    printf("\n\n");
    
    // Пример 4: Сложное выражение
    printf("--- Пример 4: Сложное выражение ---\n");
    Expr *complex = expr_add(
        expr_mul(expr_number(2.0), expr_pow(expr_variable('x'), expr_number(3.0))),
        expr_sub(expr_sqrt(expr_variable('x')), expr_number(5.0))
    );
    printf("Выражение: 2*x^3 + (sqrt(x) - 5)\n");
    printf("В AST: ");
    expr_print(complex);
    printf("\n");
    
    context_set(&ctx, 'x', 4.0);
    result = expr_eval(complex, &ctx);
    printf("При x = 4: %.2f\n", result);
    printf("Проверка: 2*64 + (2 - 5) = 128 - 3 = 125\n\n");
    
    // Очистка памяти
    expr_free(expr1);
    expr_free(deriv_simplified);
    expr_free(complex);
    
    return 0;
}
