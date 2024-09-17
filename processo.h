#ifndef PROCESSO_H
#define PROCESSO_H

#define MAX_LINE_LENGTH 256
#define MAX_PROCESSES 18393

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  long id;
  char numero[20];
  char data_ajuizamento[25];
  int id_classe;
  int id_assunto;
  int ano_eleicao;
} Process;

void readProcesses(char *fileName, Process processes[], int *numProcesses);
void ordenarPorId(Process process[], int n);
void countClassIds(char *fileName);

#endif