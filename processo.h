#ifndef PROCESSO_H
#define PROCESSO_H

#include <string.h>
#include <time.h>

#define MAX_PROCESS 100

typedef struct
{
  int dia;
  int mes;
  int ano;
} Data;

typedef struct
{
  int id;
  char numero[50];
  Data data_ajuizamento;
  int id_classe;
  int id_assunto;
  int ano_eleicao;
} Processo;

Processo process[MAX_PROCESS];

int readValueProcess(char *fileName);
void selectionSortProcess(Processo process[], int n);

#endif