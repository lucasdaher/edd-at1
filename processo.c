#include "processo.h"

Process processes[MAX_PROCESSES];
int numProcesses = 0;

void enviarTitulo() {
  printf(" -------------------------------------------------\n");
  printf("|             MANIPULACAO DE PROCESSOS            |\n");
  printf("|     Escolha uma opcao do menu para continuar    |\n");
  printf(" -------------------------------------------------\n");
}

void enviarMenu() {
  int option;

  do {
    enviarTitulo();
    printf("\n1. Ordenar em ordem crescente a partir do atributo ID.\n");
    printf("\n2. Ordenar em ordem decrescente a partir do atributo DATA_AJUIZAMENTO.\n");
    printf("\n3. Contar quantos processos estão vinculados a um determinado ID_CLASSE.\n");
    printf("\n4. Identificar quantos ID_ASSUNTOS constam nos processos presentes na base de dados.\n");
    printf("\n5. Indicar a quantos dias um processo esta em tramitacao na justica.\n\n");
    scanf("%d", &option);

    switch(option) {
      case 1:
        readProcesses("../processosOriginal.csv", processes, &numProcesses);
        ordenarPorId(processes, numProcesses);
        break;
      case 3:
        int idClasse;

        printf("Informe o numero do ID_CLASSE desejado: \n");
        scanf("%d", &idClasse);
        system("cls");

        contarPorIdClasse("../processosOriginal.csv", idClasse);
        break;
      default:
        enviarTitulo();
        printf("A opcao escolhida nao existe.\n");
        printf("Pressione qualquer tecla para tentar novamente...\n");
        getch();
    }
  }while(option <= 0 || option > 5);

}

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
    if (count == 1) {
      printf("O identificador da classe %d possui %d processo\n", idClasse, count);
    } else {
      printf("O identificador da classe %d possui %d processos\n", idClasse, count);
    }
  } else {
    printf("O identificador da classe %d nao foi encontrado.\n", idClasse);
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
