// Incluindo bibliotecas básicas
#include <stdio.h>
#include <stdlib.h>

// Incluindo a bibioteca que contém todas as funções e lógicas
#include "processo.h"
#include "processo.c"

// Função principal do programa
int main()
{
  Process processes[MAX_PROCESSES];
  int numProcesses = 0;

  readProcesses("processos.csv", processes, &numProcesses);
  printProcesses(processes, numProcesses);
  return 0;
}