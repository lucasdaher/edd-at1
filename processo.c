#include "processo.h"

void readProcesses(const char *fileName, Process processes[], int *numProcesses)
{
  FILE *file = fopen(fileName, "r");
  if (file == NULL)
  {
    perror("Erro ao realizar leitura do arquivo.\n");
    exit(EXIT_FAILURE);
  }

  char line[MAX_LINE_LENGTH];
  int position = 0;

  while (fgets(line, MAX_LINE_LENGTH, file))
  {
    if (position == 0)
    {
      position++;
      continue;
    }

    Process p;
    sscanf(line, "%ld,\"%[^\"]\",\"%[^\"]\",{%d},{%d},%d", &p.id, p.numero, p.data_ajuizamento, &p.id_classe, &p.id_assunto, &p.ano_eleicao);

    processes[*numProcesses] = p;
    (*numProcesses)++;
  }

  fclose(file);
}

void printProcesses(Process processes[], int numProcesses)
{
  for (int i = 0; i < numProcesses; i++)
  {
    printf("ID: %ld\n", processes[i].id);
    printf("Numero: %s\n", processes[i].numero);
    printf("Data Ajuizamento: %s\n", processes[i].data_ajuizamento);
    printf("ID Classe: %d\n", processes[i].id_classe);
    printf("ID Assunto: %d\n", processes[i].id_assunto);
    printf("Ano Eleicao: %d\n", processes[i].ano_eleicao);
    printf("\n");
  }
}

void selectionSortProcess(Process process[], int n)
{
  int min;
  for (int i = 0; i < n - 1; i++)
  {
    min = i;
    for (int j = i + 1; j < n; j++)
    {
      if (process[j].id < process[i].id)
      {
        min = j;
      }
    }
    Process aux = process[min];
    process[min] = process[i];
    process[i] = aux;
  }
}
