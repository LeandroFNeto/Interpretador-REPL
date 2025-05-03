#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_VARS 100
#define MAX_LEN 256

typedef struct {
    char name[32];
    double value;
} Variable;

Variable vars[MAX_VARS];
int var_count = 0;

// Busca variável
int get_var_index(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0)
            return i;
    }
    return -1;
}

// Armazena variável
void set_variable(const char *name, double value) {
    int idx = get_var_index(name);
    if (idx == -1) {
        strcpy(vars[var_count].name, name);
        vars[var_count].value = value;
        var_count++;
    } else {
        vars[idx].value = value;
    }
}

// Retorna valor da variável
double get_variable(const char *name, int *found) {
    int idx = get_var_index(name);
    if (idx != -1) {
        *found = 1;
        return vars[idx].value;
    }
    *found = 0;
    return 0.0;
}

// Expressão: parser recursivo
const char *expr_ptr;

void skip_spaces() {
    while (*expr_ptr == ' ') expr_ptr++;
}

// Forward declarations
double parse_expr();

double parse_number_or_variable() {
    skip_spaces();
    if (isdigit(*expr_ptr)) {
        double val = 0;
        while (isdigit(*expr_ptr)) {
            val = val * 10 + (*expr_ptr - '0');
            expr_ptr++;
        }
        if (*expr_ptr == '.') {
            expr_ptr++;
            double frac = 0, base = 0.1;
            while (isdigit(*expr_ptr)) {
                frac += (*expr_ptr - '0') * base;
                base *= 0.1;
                expr_ptr++;
            }
            val += frac;
        }
        return val;
    } else if (isalpha(*expr_ptr)) {
        char name[32];
        int i = 0;
        while (isalnum(*expr_ptr)) {
            name[i++] = *expr_ptr++;
        }
        name[i] = '\0';
        int found;
        double val = get_variable(name, &found);
        if (!found) {
            printf("Erro: variável '%s' não declarada.\n", name);
            exit(1);
        }
        return val;
    } else if (*expr_ptr == '(') {
        expr_ptr++;
        double val = parse_expr();
        skip_spaces();
        if (*expr_ptr != ')') {
            printf("Erro: esperado ')'\n");
            exit(1);
        }
        expr_ptr++;
        return val;
    } else {
        printf("Erro de sintaxe: caractere inesperado '%c'\n", *expr_ptr);
        exit(1);
    }
}

double parse_term() {
    double val = parse_number_or_variable();
    skip_spaces();
    while (*expr_ptr == '*' || *expr_ptr == '/') {
        char op = *expr_ptr++;
        double right = parse_number_or_variable();
        if (op == '*') val *= right;
        else if (op == '/') val /= right;
        skip_spaces();
    }
    return val;
}

double parse_expr() {
    double val = parse_term();
    skip_spaces();
    while (*expr_ptr == '+' || *expr_ptr == '-') {
        char op = *expr_ptr++;
        double right = parse_term();
        if (op == '+') val += right;
        else if (op == '-') val -= right;
        skip_spaces();
    }
    return val;
}

double avaliar_expressao(char *expr) {
    expr_ptr = expr;
    return parse_expr();
}

// REPL
int main() {
    char linha[MAX_LEN];
    printf("Interpretador REPL em C (sem bc) — Digite 'sair' para encerrar\n");

    while (1) {
        printf(">>> ");
        if (!fgets(linha, MAX_LEN, stdin)) break;
        linha[strcspn(linha, "\n")] = '\0';

        if (strcmp(linha, "sair") == 0) break;

        char nome[32], expr[MAX_LEN];

        char *igual = strchr(linha, '=');
        if (igual != NULL) {
            sscanf(linha, "%31[^=]=%255[^\n]", nome, expr);

            // remove espaços iniciais
            char *p_nome = nome;
            while (*p_nome == ' ') p_nome++;
            char *p_expr = expr;
            while (*p_expr == ' ') p_expr++;

            double valor = avaliar_expressao(p_expr);
            set_variable(p_nome, valor);
            printf("%s = %g\n", p_nome, valor);
        } else {
            int found;
            double val = get_variable(linha, &found);
            if (found) {
                printf("%s = %g\n", linha, val);
            } else {
                double resultado = avaliar_expressao(linha);
                printf("%g\n", resultado);
            }
        }
    }

    return 0;
}


