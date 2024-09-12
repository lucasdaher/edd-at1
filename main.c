#include <stdio.h>
#include <stdlib.h>

#include "processo.h"
#include "processo.c"

int main()
{
  Process processes[MAX_PROCESSES];
  int numProcesses = 0;

  readProcesses("processosOriginal.csv", processes, &numProcesses);
  ordenarPorId(processes, numProcesses);
  return 0;
}