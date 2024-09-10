// Incluindo bibliotecas básicas
#include <stdio.h>
#include <stdlib.h>

// Incluindo a bibioteca que contém todas as funções e lógicas
#include "processo.h"
#include "processo.c"

// Função principal do programa
int main()
{
  FILE *file = fopen("processos.csv", "r");

  if (file == NULL)
  {
    perror("O arquivo nao pode ser lido, tente novamente...\n");
    exit(1);
  }
  else
  {
    char header[256];
    fgets(header, sizeof(header), file);

    Processo processo[MAX_PROCESS];

    int id;
    char numero[50];
    char data_ajuizamento[25];
    int id_classe;
    int id_assunto;
    int ano_eleicao;

    int i = 0; // Contador

    while (fscanf(file, "%d,\"%[^\"]\",\"%[^\"]\",{%d},{%d},%d\n", &id, numero, data_ajuizamento, &id_classe, &id_assunto, &ano_eleicao) == 6)
    {
      int dia, mes, ano;
      sscanf(data_ajuizamento, "%d-%d-%d", &ano, &mes, &dia);

      processo[i].id = id;
      snprintf(processo[i].numero, sizeof(processo[i].numero), "%s", numero);
      processo[i].data_ajuizamento.dia = dia;
      processo[i].data_ajuizamento.mes = mes;
      processo[i].data_ajuizamento.ano = ano;
      processo[i].id_classe = id_classe;
      processo[i].id_assunto = id_assunto;
      processo[i].ano_eleicao = ano_eleicao;

      i++;
    };

    fclose(file);

    for (int j = 0; j < i; j++)
    {
      printf("ID: %d\n", processo[j].id);
      printf("Número: %s\n", processo[j].numero);
      printf("Data de Ajuizamento: %02d/%02d/%04d\n", processo[j].data_ajuizamento.dia, processo[j].data_ajuizamento.mes, processo[j].data_ajuizamento.ano);
      printf("ID Classe: %d\n", processo[j].id_classe);
      printf("ID Assunto: %d\n", processo[j].id_assunto);
      printf("Ano Eleição: %d\n\n", processo[j].ano_eleicao);
    }
  }

  return 0;
}