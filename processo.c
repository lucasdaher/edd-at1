#include "processo.h"

// Enviar o título decorativo do menu.
void enviarTitulo() {
  printf(" --------------------------------------------------\n");
  printf("|             MANIPULACAO DE PROCESSOS             |\n");
  printf("|     Escolha uma opcao do menu para prosseguir    |\n");
  printf(" --------------------------------------------------\n");
}

// Enviar o menu principal com as opções de manipulação e consulta dos processos.
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

        printf("Informe o \"id_classe\" que deseja consultar: \n");
        scanf("%d", &idClasse);
        contarIdClasse(inputFile, idClasse);
        break;
      case 4:
        listarAssuntos(inputFile);
        break;
      default:
        enviarTitulo();
        printf("A opcao escolhida nao existe.\n");
        printf("Pressione qualquer tecla para tentar novamente...\n");
        getch();
    }
  } while(option <= 0 || option > 5);
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

  printf("Gerando o seu arquivo ordenado em ordem decrescente pelo atributo data_ajuizamento...\n");
  fprintf(arquivo, "\"id\",\"numero\",\"data_ajuizamento\",\"id_classe\",\"id_assunto\",\"ano_eleicao\"\n");

  for (int i = 0; i < tamanho; i++)
  {
    fprintf(arquivo, "%d,\"%s\",%s,{%d},{%d},%d\n",
            processo[i].id, processo[i].numero, processo[i].data_ajuizamento,
            processo[i].id_classe, processo[i].id_assunto, processo[i].ano_eleicao);
  }
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
    char *token = strtok(linha, ",");
    p.id = atoi(token);

    token = strtok(NULL, "\"\"");
    strcpy(p.numero, token + 1);
    p.numero[strlen(p.numero) - 1] = '\0';

    token = strtok(NULL, ",");
    strcpy(p.data_ajuizamento, token);

    token = strtok(NULL, "{");
    p.id_classe = atoi(token);

    token = strtok(NULL, "{");
    p.id_assunto = atoi(token);

    token = strtok(NULL, ",");
    p.ano_eleicao = atoi(token);

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
      char *idClasseStr = start + 1; // skip the '{' character
      char *token = strtok(idClasseStr, ",");
      while (token != NULL) {
        int classId = atoi(token);
        if (classId == idClasse) {
          contador++;
        }
        token = strtok(NULL, ",");
      }
    }
  }

  fclose(arquivo);

  if (contador > 0) {
    if (contador == 1) {
      printf("O atributo \"id_classe\" %d possui %d processo\n", idClasse, contador);
    } else {
      printf("O atributo \"id_classe\" %d possui %d processos\n", idClasse, contador);
    }
  } else {
    printf("O atributo \"id_classe\" %d nao possui nenhum processo.\n", idClasse);
  }
}

// Listar a quantidade de processos atrelados a cada atributo id assunto.
void listarAssuntos(const char *nomeArquivo) {
  FILE *arquivo = fopen(nomeArquivo, "r");

  if (arquivo == NULL) {
    perror("O arquivo nao pode ser lido.\n");
    exit(EXIT_FAILURE);
  }

  char linha[1024];
  int id_assuntos[MAX_PROCESSES];
  int contador = 0;
  int frequencia[MAX_PROCESSES];

  for (int i = 0; i < MAX_PROCESSES; i++) {
    id_assuntos[i] = -1;
    frequencia[i] = 0;
  }

  while (fgets(linha, 1024, arquivo)) {
    char *token = strtok(linha, ",");
    int campo = 0;
    while (token != NULL) {
      if (campo == 4) { // id_assunto is the 5th field
        char *start = strchr(token, '{');
        char *end = strchr(token, '}');
        if (start != NULL && end != NULL) {
          *end = '\0';
          char *id_assunto_str = start + 1; // skip the '{' character
          char *token_id_assunto = strtok(id_assunto_str, ",");
          while (token_id_assunto != NULL) {
            int id_assunto = atoi(token_id_assunto);
            int encontrado = 0;
            for (int i = 0; i < contador; i++) {
              if (id_assuntos[i] == id_assunto) {
                encontrado = 1;
                frequencia[i]++;
                break;
              }
            }
            if (!encontrado) {
              id_assuntos[contador++] = id_assunto;
              frequencia[contador - 1] = 1;
            }
            token_id_assunto = strtok(NULL, ",");
          }
        }
      }
      token = strtok(NULL, ",");
      campo++;
    }
  }

  fclose(arquivo);

  enviarTitulo();
  printf("Quantidade de \"id_assuntos\" na base de dados: %d\n", contador);
  int opcao;
  printf("1) Visualizar a quantidade vezes que um \"id_assunto\" aparece em cada processo.\n");
  printf("2) Voltar ao menu principal\n");
  scanf("%d", &opcao);
  system("cls");

  do {
    switch(opcao) {
      case 1:
        for (int i = 0; i < contador; i++) {
          printf("O \"id_assunto\" %d aparece %d vezes na base de dados.\n", id_assuntos[i], frequencia[i]);
        }
        system("cls");
      break;
      case 2:
        enviarMenu();
        system("cls");
        break;
      default:
        printf("A opcao informada esta invalida, saindo do programa...\n");
        system("cls");
        exit(EXIT_FAILURE);
    }

  } while(opcao <= 0 || opcao > 2);
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

  printf("Gerando o seu arquivo ordenado em ordem decrescente pelo atributo data_ajuizamento...\n");
  fprintf(arquivo, "\"id\",\"numero\",\"data_ajuizamento\",\"id_classe\",\"id_assunto\",\"ano_eleicao\"\n");

  for (int i = 0; i < n; i++)
  {
    fprintf(arquivo, "%d,\"%s\",%s,{%d},{%d},%d\n",
            processos[i].id, processos[i].numero, processos[i].data_ajuizamento,
            processos[i].id_classe, processos[i].id_assunto, processos[i].ano_eleicao);
  }
  system("start excel.exe processos.csv");

  fclose(arquivo);
}
