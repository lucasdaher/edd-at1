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
  int opcao;

  do {
    enviarTitulo();
    printf("\n1. Ordenar em ordem crescente a partir do atributo ID.");
    printf("\n2. Ordenar em ordem decrescente a partir do atributo DATA_AJUIZAMENTO.");
    printf("\n3. Contar quantos processos estao vinculados a um determinado ID_CLASSE.");
    printf("\n4. Identificar quantos ID_ASSUNTOS constam nos processos presentes na base de dados.");
    printf("\n5. Indicar a quantos dias um processo esta em tramitacao na justica.\n\n");
    scanf("%d", &opcao);

    switch(opcao) {
      case 1:
        lerProcessos("../processos.csv", processes, &numProcesses);
        ordenarId(processes, numProcesses);
        break;
      case 3:
        int idClasse;

        printf("Informe o numero do 'ID_CLASSE' desejado: \n");
        scanf("%d", &idClasse);
        system("cls");

        contarIdClasse("../processos.csv", idClasse);
        break;
      case 4:
        enviarMenuAssunto();
        break;
      default:
        enviarTitulo();
        printf("A opcao escolhida nao existe.\n");
        printf("Pressione qualquer tecla para tentar novamente...\n");
        getch();
    }
  } while(opcao <= 0 || opcao > 5);
}

void enviarMenuAssunto() {
  int opcao;
  do {
    printf(" -------------------------------------------------\n");
    printf("|             MANIPULACAO DE PROCESSOS            |\n");
    printf("|     Escolha uma opcao do menu para continuar    |\n");
    printf(" -------------------------------------------------\n");
    printf("\n");
    printf("1. Visualizar quantos assuntos constam especificando um 'ID'.\n");
    printf("2. Visualizar quantos assuntos constam em cada 'ID' de forma geral.\n\n");
    scanf("%d", &opcao);

    switch (opcao) {
      case 1:
        int idAssunto;
        printf("Informe o 'ID_ASSUNTO' que deseja visualizar a quantidade:\n");
        scanf("%d", &idAssunto);
        listarIdAssunto("../processos.csv", idAssunto);
      break;
      case 2:
        listarTodosIdAssunto("../processos.csv");
      break;
      default:
    }
  } while(opcao <= 0 || opcao > 2);
}

void lerProcessos(const char *nomeArquivo, Process processes[], int *numProcesses)
{
  FILE *arquivo = fopen(nomeArquivo, "r");
  if (arquivo == NULL)
  {
    perror("Erro ao realizar leitura do arquivo.\n");
    exit(EXIT_FAILURE);
  }

  char line[MAX_LINE_LENGTH];
  int position = 0;

  while (fgets(line, MAX_LINE_LENGTH, arquivo))
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

  fclose(arquivo);
}

void contarIdClasse(const char *nomeArquivo, int idClasse) {
  FILE *arquivo = fopen(nomeArquivo, "r");

  if (arquivo == NULL) {
    perror("O arquivo nao pode ser lido.\n");
    exit(EXIT_FAILURE);
  }

  char line[MAX_LINE_LENGTH];
  int count = 0;

  while (fgets(line, MAX_LINE_LENGTH, arquivo)) {
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

  fclose(arquivo);

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

void listarIdAssunto(const char *nomeArquivo, int idAssunto) {
  FILE *arquivo = fopen(nomeArquivo, "r");

  if (arquivo == NULL) {
    perror("O arquivo nao pode ser lido.\n");
    exit(EXIT_FAILURE);
  }

  char line[MAX_LINE_LENGTH];
  int count = 0;

  while (fgets(line, MAX_LINE_LENGTH, arquivo)) {
    char *token = strtok(line, ",");
    char *idAssuntoStr = NULL;

    for (int i = 0; i < 5; i++) {
      token = strtok(NULL, ",");
      if (i == 4) {
        idAssuntoStr = token;
        break;
      }
    }

    if (idAssuntoStr != NULL) {
      int idAssuntoCsv = atoi(idAssuntoStr + 1);
      if (idAssuntoCsv == idAssunto) {
        count++;
      }
    }
  }

  fclose(arquivo);

  if (count > 0) {
    printf("Existem %d para o assunto %d.\n", count, idAssunto);
  } else {
    printf("Nao encontramos nada relacionado ao ID_ASSUNTO %d\n", idAssunto);
  }
}

void listarTodosIdAssunto(const char *nomeArquivo) {
  FILE *arquivo = fopen(nomeArquivo, "r");

  if (arquivo == NULL) {
    perror("O arquivo nao pode ser lido.\n");
    exit(EXIT_FAILURE);
  }

  char linha[1024];
  int id_assuntos[MAX_PROCESSES];
  int count = 0;

  for (int i = 0; i < MAX_PROCESSES; i++) {
    id_assuntos[i] = -1;
  }

  while (fgets(linha, 1024, arquivo)) {
    char *token = strtok(linha, ",");
    token = strtok(NULL, ",");
    token = strtok(NULL, ",");
    token = strtok(NULL, ",");
    char *id_assunto_str = token;

    int id_assunto = atoi(id_assunto_str);
    int encontrado = 0;
    for (int i = 0; i < count; i++) {
      if (id_assuntos[i] == id_assunto) {
        encontrado = 1;
        break;
      }
    }
    if (!encontrado) {
      id_assuntos[count++] = id_assunto;
    }
  }

  fclose(arquivo);

  printf("Quantidade de id_assuntos unicos: %d\n", count);
}

void ordenarId(Process process[], int n)
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

  FILE *arquivo = fopen("processos.csv", "w");

  if (arquivo == NULL)
  {
    perror("O arquivo nao pode ser lido.\n");
    exit(EXIT_FAILURE);
  }

  // Adiciona o HEADER do arquivo CSV antes da implementação dos dados.
  fprintf(arquivo, "\"id\",\"numero\",\"data_ajuizamento\",\"id_classe\",\"id_assunto\",\"ano_eleicao\"\n");

  for (int i = 0; i < n; i++)
  {
    fprintf(arquivo, "%ld,\"%s\",%s,{%d},{%d},%d\n",
            process[i].id, process[i].numero, process[i].data_ajuizamento,
            process[i].id_classe, process[i].id_assunto, process[i].ano_eleicao);
    printf("%ld - Gravado com sucesso.\n", process[i].id);
  }
  system("cls");
  printf("Abrindo Excel com os processos ordenados de forma crescente pelo seu identificador.\n");

  system("start excel.exe processos.csv");

  fclose(arquivo);
}
