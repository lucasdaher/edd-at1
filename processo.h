#ifndef PROCESSO_H
#define PROCESSO_H

#include <string.h>
#include <time.h>

typedef struct
{
  int dia;
  int mes;
  int ano;
} Data;

typedef struct
{
  int id;
  char numero[50];
  Data data_ajuizamento;
  int id_classe;
  int id_assunto;
  int ano_eleicao;
} Processo;

#endif