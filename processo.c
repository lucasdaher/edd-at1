#include "processo.h"

void selectionSortProcess(Processo process[], int n)
{
  int menor;
  for (int i = 0; i < n - 1; i++)
  {
    menor = i;
    for (int j = i + 1; j < n; j++)
    {
      if (process[j].id < process[i].id)
      {
        menor = j;
      }
    }
    Processo aux = process[menor];
    process[menor] = process[i];
    process[i] = aux;
  }
}
