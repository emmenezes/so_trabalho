# Trabalho de SO - 2022/2

O projeto consiste da confecção de um escalonador. Todos os arquivos finais estão na pasta **src**.

Alunas:
* Alícia Rita Oliveira Dos Reis - 17/0129306
* Cintia Leal Rodrigues - 17/0125696
* Emme Costa de Menezes - 17/0032353


## execprocd 
Processo de background que gerencia os processos. Em sua chamada, é preciso passar o modo de prioridade, sendo:
- 1: escolha randômica
- 2: prioridade estática
- 3: prioridade dinâmica

```
./execprocd <modo_prioridade>
```

## execproc
Processo que solicita a execução de outro processo. Em sua chamada, é preciso passar a prioridade (um número de 1 a 3, onde 1 é a mais alta prioridade) e o nome do arquivo executável.

```
./execproc <prioridade> <arquivo>
```
Retorna id do processo.

## cancela_proc
Processo que solicita cancelamento do processo com bas
```
./cancela_proc <id_procesoo>
```
Retorna id do processo, tempo de execução e quantidade trocas de contexto.

## termina_execprocd
Processo que solicita fim da execução.
```
./termina_execprocd
```
Retorna quantidade de processos concluído, quantidade de processos cancelados e quantidade de trocas de contexto.

## outros

* Há dois programas com duração longa, um de **14 segundos** e um de **37 segundos** que também estão na pasta **src**.
* Quando um processo é concluído, retorna id do processo, tempo de execução e quantidade trocas de contexto.
* Sempre que o modo de prioridade não é estático, há uma mensagem alertando sobre a nova prioridade do processo.
* À cada execução, é exibida uma mensagem informando qual processo está sendo executado e sua prioridade.
* As funções auxiliares *check_proc_table* e *check_queues* são utéis para checar a situação do background
* Na linha 334 de execprocd, está comentada a função *check_queues* que pode auxiliar no acompanhamento das filas de prioridade.