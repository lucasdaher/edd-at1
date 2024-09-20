#ifndef PROCESSO_H
#define PROCESSO_H

#define MAX_LINE_LENGTH 256
#define MAX_PROCESSES 18392

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

typedef struct
{
  long id;
  char numero[20];
  char data_ajuizamento[20];
  int id_classe;
  int id_assunto;
  int ano_eleicao;
} Process;

void enviarTitulo();
void enviarMenu();
void enviarMenuAssunto();

void lerProcessos(const char *nomeArquivo, Process processos[], int *numProcessos);
void contarIdClasse(const char *nomeArquivo, int idClasse);
void listarAssunto(const char *nomeArquivo, int idAssunto);
void listarTodosAssuntos(const char *nomeArquivo);
int compararDatas(const void *a, const void *b);
void ordenarPorData(const char *nomeArquivo, Process processo[], int tamanho);
void ordenarPorId(Process processos[], int n);

#endif