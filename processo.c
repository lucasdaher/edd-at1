#include "processo.h"

void enviarTitulo() {
  printf(" --------------------------------------------------\n");
  printf("|             MANIPULACAO DE PROCESSOS             |\n");
  printf("|     Escolha uma opcao do menu para prosseguir    |\n");
  printf(" --------------------------------------------------\n");
}

void enviarMenu() {
  int option;

  do {
    const char *inputFile = "../processos.csv";
    Process processes[MAX_PROCESSES];
    int numProcesses = 0;

    enviarTitulo();
    printf("\n1. Ordenar em ordem crescente a partir do atributo ID.");
    printf("\n2. Ordenar em ordem decrescente a partir do atributo DATA_AJUIZAMENTO.");
    printf("\n3. Contar quantos processos estao vinculados a um determinado ID_CLASSE.");
    printf("\n4. Identificar quantos ID_ASSUNTOS constam nos processos presentes na base de dados.");
    printf("\n5. Indicar a quantos dias um processo esta em tramitacao na justica.\n\n");
    scanf("%d", &option);

    switch(option) {
      case 1:
        lerProcessos(inputFile, processes, &numProcesses);
        ordenarPorId(processes, numProcesses);
        break;

      case 2:
        ordenarPorData(inputFile, processes, numProcesses);
        break;
      case 3:
        int idClasse;

        printf("Informe o numero do 'ID_CLASSE' desejado: \n");
        scanf("%d", &idClasse);
        system("cls");

        contarIdClasse(inputFile, idClasse);
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
  } while(option <= 0 || option > 5);
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
        listarAssunto("../processos.csv", idAssunto);
      break;
      case 2:
        listarTodosAssuntos("../processos.csv");
      break;
      default:
    }
  } while(opcao <= 0 || opcao > 2);
}

// Comparar duas datas em formato de string para ordenação decrescente.
int compararDatas(const void *a, const void *b) {
  Process *pA = (Process *)a;
  Process *pB = (Process *)b;
  return strcmp(pB->data_ajuizamento, pA->data_ajuizamento);
}

// Ordena o arquivo de processos em ordem decrescente pelo seu atributo data_ajuizamento.
void ordenarPorData(const char *nomeArquivo, Process processo[], int tamanho) {
  lerProcessos(nomeArquivo, processo, &tamanho);
  qsort(processo, tamanho, sizeof(Process), compararDatas);

  FILE *arquivo = fopen("processos.csv", "w");

  if (arquivo == NULL)
  {
    perror("O arquivo nao pode ser lido.\n");
    exit(EXIT_FAILURE);
  }

  fprintf(arquivo, "\"id\",\"numero\",\"data_ajuizamento\",\"id_classe\",\"id_assunto\",\"ano_eleicao\"\n");

  for (int i = 0; i < tamanho; i++)
  {
    fprintf(arquivo, "%d,\"%s\",\"%s\",{%d},{%d},%d\n",
            processo[i].id, processo[i].numero, processo[i].data_ajuizamento,
            processo[i].id_classe, processo[i].id_assunto, processo[i].ano_eleicao);
    printf("\"%s\" - Data gravada com sucesso.\n", processo[i].data_ajuizamento);
  }
  system("cls");
  printf("Abrindo Excel com os processos ordenados de forma decrescente pelo atributo data_ajuizamento.\n");

  system("start excel.exe processos.csv");

  fclose(arquivo);
}

// Realiza a leitura do arquivo de processos e escaneia todas as informações presentes nele.
void lerProcessos(const char *nomeArquivo, Process processos[], int *numProcessos)
{
  FILE *arquivo = fopen(nomeArquivo, "r");
  if (arquivo == NULL)
  {
    perror("Erro ao realizar leitura do arquivo.\n");
    exit(EXIT_FAILURE);
  }

  char linha[MAX_LINE_LENGTH];
  int posicao = 0;

  while (fgets(linha, MAX_LINE_LENGTH, arquivo))
  {
    if (posicao == 0)
    {
      posicao++;
      continue;
    }

    Process p;
    sscanf(linha, "%d,\"%[^\"]\",%[^,],{%d},{%d},%d", &p.id, p.numero, p.data_ajuizamento, &p.id_classe, &p.id_assunto, &p.ano_eleicao);

    processos[*numProcessos] = p;
    (*numProcessos)++;
  }

  fclose(arquivo);
}

// Contar a quantidade de processos que estão vinculados a um determinado id classe.
void contarIdClasse(const char *nomeArquivo, int idClasse) {
  FILE *arquivo = fopen(nomeArquivo, "r");

  if (arquivo == NULL) {
    perror("O arquivo nao pode ser lido.\n");
    exit(EXIT_FAILURE);
  }

  char linha[MAX_LINE_LENGTH];
  int contador = 0;

  while (fgets(linha, MAX_LINE_LENGTH, arquivo)) {
    char *start = strchr(linha, '{');
    char *end = strchr(linha, '}');
    if (start != NULL && end != NULL) {
      *end = '\0';
      int classId = atoi(start + 1);

      if (classId == idClasse) {
        contador++;
      }
    }
  }

  fclose(arquivo);

  if (contador > 0) {
    if (contador == 1) {
      printf("O identificador da classe %d possui %d processo\n", idClasse, contador);
    } else {
      printf("O identificador da classe %d possui %d processos\n", idClasse, contador);
    }
  } else {
    printf("O identificador da classe %d nao foi encontrado.\n", idClasse);
  }
}

// Listar os processos atrelados ao atributo id assunto especificado pelo usuário.
void listarAssunto(const char *nomeArquivo, int idAssunto) {
  FILE *arquivo = fopen(nomeArquivo, "r");

  if (arquivo == NULL) {
    perror("O arquivo nao pode ser lido.\n");
    exit(EXIT_FAILURE);
  }

  char linha[MAX_LINE_LENGTH];
  int contador = 0;

  while (fgets(linha, MAX_LINE_LENGTH, arquivo)) {
    char *token = strtok(linha, ",");
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
        contador++;
      }
    }
  }

  fclose(arquivo);

  if (contador > 0) {
    printf("Existem %d para o assunto %d.\n", contador, idAssunto);
  } else {
    printf("Nao encontramos nada relacionado ao ID_ASSUNTO %d\n", idAssunto);
  }
}

// Listar a quantidade de processos atrelados a cada atributo id assunto.
void listarTodosAssuntos(const char *nomeArquivo) {
  FILE *arquivo = fopen(nomeArquivo, "r");

  if (arquivo == NULL) {
    perror("O arquivo nao pode ser lido.\n");
    exit(EXIT_FAILURE);
  }

  char linha[1024];
  int id_assuntos[MAX_PROCESSES];
  int contador = 0;

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
    for (int i = 0; i < contador; i++) {
      if (id_assuntos[i] == id_assunto) {
        encontrado = 1;
        break;
      }
    }
    if (!encontrado) {
      id_assuntos[contador++] = id_assunto;
    }
  }

  fclose(arquivo);

  printf("Quantidade de id_assuntos unicos: %d\n", contador);
}

// Função que ordena os processos pelo atributo id.vv
void ordenarPorId(Process processos[], int n)
{
  int min;
  Process temp;
  for (int i = 0; i < n - 1; i++)
  {
    min = i;
    for (int j = i + 1; j < n; j++)
    {
      if (processos[j].id < processos[min].id)
      {
        min = j;
      }
    }

    temp.id = processos[min].id;
    processos[min].id = processos[i].id;
    processos[i].id = temp.id;
  }

  FILE *arquivo = fopen("processos.csv", "w");

  if (arquivo == NULL)
  {
    perror("O arquivo nao pode ser lido.\n");
    exit(EXIT_FAILURE);
  }

  fprintf(arquivo, "\"id\",\"numero\",\"data_ajuizamento\",\"id_classe\",\"id_assunto\",\"ano_eleicao\"\n");

  for (int i = 0; i < n; i++)
  {
    fprintf(arquivo, "%d,\"%s\",%s,{%d},{%d},%d\n",
            processos[i].id, processos[i].numero, processos[i].data_ajuizamento,
            processos[i].id_classe, processos[i].id_assunto, processos[i].ano_eleicao);
    printf("%d - Gravado com sucesso.\n", processos[i].id);
  }
  system("cls");
  printf("Abrindo Excel com os processos ordenados de forma crescente pelo atributo id.\n");

  system("start excel.exe processos.csv");

  fclose(arquivo);
}
