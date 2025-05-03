# Interpretador-REPL

Feito em C.

🛠️ **Como funciona**

O código é dividido em componentes principais:

Estrutura Variable: representa variáveis com nome e valor.

**Funções auxiliares:**

get_var_index() e set_variable() controlam armazenamento de variáveis.

substituir_variaveis() troca nomes de variáveis pelos seus valores em expressões.

avaliar_expressao() usa o interpretador bc do sistema para calcular o valor da expressão.

**Loop REPL:**

Lê comandos do usuário com fgets.

Identifica se é uma atribuição (x = ...) ou uma expressão simples (x + 3 ou x).

Avalia o resultado e exibe na tela.
