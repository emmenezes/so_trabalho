# Trabalho de SO - 2022/2


## Snippets
Lista de todos os snippets. Existem 3 categorias de snippets:

* *sl* -  exemplos do slide *202001_so5_unix*, e o número se refere à página do slide.
* *ex* - exemplos próprios
* *cd* - códigos isolados do trabalho

## Comandos para análise

|Comando    |Argumentos |Explicação |
|-          |-          |-          |
|ps -l      |-          |Lista de processos com vários campos |
||||


## execprocd (background)

O que faz:

- Cria processos filhos 
- Espera a morte dos processos filhos
    - Id
    - Tempo total de execução
    - Número de trocas de contexto
- Determina quantum (parar o filho)

- Reformular as filas de prioridade
    - Determinar nova prioridade do processo recém-executado
    - Coloca no fim da fila 
    - Determina novo processo a ser executado
- Espera msg da fila de novos processos ({nome do arquivo, prioridade})
    - Imprimir o id

- Espera msg da fila de fim de processos (id ou -1) 
    - Finalizar 1 processo
        - Id
        - Tempo total de execução
        - Número de trocas de contexto
    - Finalizar tudo
        - Número total de processos executados
        - Número de processos cancelados
        - Número total de trocas de contexto ocorridas

## Coisas para fazer

1. execproc
    - fazer uso da fila e executar programa
2. cancela_proc e termina_execprocessod
    - passar inteiros por fila
3. execprocd 1
    - cria processos filhos (impressões)
    - espera a morte dos filhos (impressões)
        - id
        - tempo total de execução
        - número de trocas de contexto
    - para filho no quantum
4. execprocd 2 **
    - fila de arquivos
    - recebe arquivos do exeproc
    - recebe cancela_proc e termina (impressões)
5. execprocd 3
    - fila de prioridades
6. execprocd final ** 



