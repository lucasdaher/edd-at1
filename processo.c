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
    sscanf(line, "%ld,\"%[^\"]\",%[^,],{%d},{%d},%d", &p.id, p.numero, p.data_ajuizamento, &p.id_classe, &p.id_assunto, &p.ano_eleicao);

    processes[*numProcesses] = p;
    (*numProcesses)++;
  }

  fclose(file);
}

void printProcesses(Process processes[], int numProcesses)
{
  FILE *file = fopen("processos.csv", "w");

  if (file == NULL)
  {
    perror("O arquivo nao pode ser lido.\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    fprintf(file, "\"id\",\"numero\",\"data_ajuizamento\",\"id_classe\",\"id_assunto\",\"ano_eleicao\"\n");
    for (int i = 0; i < numProcesses; i++)
    {
      fprintf(file, "%ld,\"%lld\",\"%s\",{%d},{%d},%d\n",
              processes[i].id, processes[i].numero, processes[i].data_ajuizamento,
              processes[i].id_classe, processes[i].id_assunto, processes[i].ano_eleicao);
    }

    fclose(file);
  }
}

void selectionSortById(Process processes[], int n)
{
  int min;
  for (int i = 0; i < n - 1; i++)
  {
    min = i;
    for (int j = i + 1; j < n; j++)
    {
      if (processes[j].id < processes[i].id)
      {
        min = j;
      }
    }
    Process aux;
    aux.id = processes[min].id;
    processes[min].id = processes[i].id;
    processes[i].id = aux.id;
  }

  FILE *file = fopen("processos.csv", "w");

  if (file == NULL)
  {
    perror("O arquivo nao pode ser lido.\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    fprintf(file, "\"id\",\"numero\",\"data_ajuizamento\",\"id_classe\",\"id_assunto\",\"ano_eleicao\"\n");
    for (int i = 0; i < n; i++)
    {
      fprintf(file, "%ld,\"%s\",\"%s\",{%d},{%d},%d\n",
              processes[i].id, processes[i].numero, processes[i].data_ajuizamento,
              processes[i].id_classe, processes[i].id_assunto, processes[i].ano_eleicao);
      printf("%ld - Gravado com sucesso.\n", processes[i].id);
    }

    fclose(file);
  }
}
