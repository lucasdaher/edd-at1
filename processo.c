#include "processo.h"

// Essa função é responsável por contar quantos processos existem dentro
// do arquivo de processos. Após contabilizar, ele recebe os dados que
// estão contidos dentro deste CSV e armazenam em um vetor de struct.
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

// void countClassIds(const char *fileName)
// {
//   FILE *file = fopen(fileName, "r");
//
//   if (file == NULL)
//   {
//     perror("O arquivo nao pode ser lido.\n");
//     exit(EXIT_FAILURE);
//   }
//
//   char line[MAX_LINE_LENGTH];
//   int classCounts[MAX_PROCESSES] = {0};
//
//   fgets(line, MAX_LINE_LENGTH, file);
//
//   while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
//   {
//     char *start = strchr(line, '{');
//     char *end = strchr(line, '}');
//     if (start != NULL && end != NULL)
//     {
//       *end = '\0';
//       int classId = atoi(start + 1);
//
//       if (classId >= 0 && classId < MAX_PROCESSES)
//       {
//         classCounts[classId]++;
//       }
//     }
//   }
//
//   fclose(file);
//
//   // Imprimir os resultados
//   for (int i = 0; i < MAX_PROCESSES; i++)
//   {
//     if (classCounts[i] > 0)
//     {
//       printf("id_classe %d: %d processos\n", i, classCounts[i]);
//     }
//   }
// }


void contarPorIdClasse(const char *fileName, int idClasse) {
  FILE *file = fopen(fileName, "r");

  if (file == NULL) {
    perror("O arquivo nao pode ser lido.\n");
    exit(EXIT_FAILURE);
  }

  char line[MAX_LINE_LENGTH];
  int count = 0;

  while (fgets(line, MAX_LINE_LENGTH, file)) {
    char *start = strchr(line, '{');
    char *end = strchr(line, '}');
    if (start != NULL && end != NULL) {
      *end = '\0';
      int classId = atoi(start + 1);

      if (classId == idClasse) {
        count++;
      }
    }
  }

  fclose(file);

  if (count > 0) {
    printf("id_classe %d: %d processos\n", idClasse, count);
  } else {
    printf("id_classe %d nao encontrado.\n", idClasse);
  }
}

void ordenarPorId(Process process[], int n)
{
  // Algoritmo de ordenação | Selection Sort
  int min;
  Process temp;
  for (int i = 0; i < n - 1; i++)
  {
    min = i;
    for (int j = i + 1; j < n; j++)
    {
      if (process[j].id < process[min].id)
      {
        min = j;
      }
    }

    temp.id = process[min].id;
    process[min].id = process[i].id;
    process[i].id = temp.id;
  }

  FILE *file = fopen("processos.csv", "w");

  if (file == NULL)
  {
    perror("O arquivo nao pode ser lido.\n");
    exit(EXIT_FAILURE);
  }

  // Adiciona o HEADER do arquivo CSV antes da implementação dos dados.
  fprintf(file, "\"id\",\"numero\",\"data_ajuizamento\",\"id_classe\",\"id_assunto\",\"ano_eleicao\"\n");

  for (int i = 0; i < n; i++)
  {
    fprintf(file, "%ld,\"%s\",%s,{%d},{%d},%d\n",
            process[i].id, process[i].numero, process[i].data_ajuizamento,
            process[i].id_classe, process[i].id_assunto, process[i].ano_eleicao);
    printf("%ld - Gravado com sucesso.\n", process[i].id);
  }
  system("cls");
  printf("Abrindo Excel com os processos ordenados de forma crescente pelo seu identificador.\n");

  system("start excel.exe processos.csv");

  fclose(file);
}
