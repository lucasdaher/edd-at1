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
  int id_classe_procurado = 11541;
  int total; 

  total = countByIdClass(processes, numProcesses, id_classe_procurado);

  readProcesses("processos.csv", processes, &numProcesses);
  selectionSortById(processes, numProcesses);
  countByIdClass(processes, numProcesses, id_classe_procurado);

  printf("Total de processos com id_classe %d: %d", id_classe_procurado, total);

  return 0;
}