#include <stdio.h>
#include <stdlib.h>

#include "processo.h"
#include "processo.c"

int main()
{
  Process processes[MAX_PROCESSES];
  int numProcesses = 0;

  printf("Requisitando funcao que realiza a leitura dos processos.\n");
  readProcesses("processosOriginal.csv", processes, &numProcesses);
  printf("Requisitando funcao que ordena os processos pelo id.\n");
  selectionSortById(processes, numProcesses);
  return 0;
}