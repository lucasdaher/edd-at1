#ifndef PROCESSO_H
#define PROCESSO_H

#define MAX_LINE_LENGTH 256
#define MAX_PROCESSES 18392

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef struct {
  int ano;
  int mes;
  int dia;
  int hora;
  int minuto;
  int segundo;
} DataAjuizamento;

typedef struct
{
  long id;
  char numero[20];
  char data_ajuizamento[25];
  int id_classe;
  int id_assunto;
  int ano_eleicao;
} Process;

void enviarTitulo();
void enviarMenu();
void enviarMenuAssunto();

void lerProcessos(const char *fileName, Process processes[], int *numProcesses);
void contarIdClasse(const char *fileName, int idClasse);
void listarIdAssunto(const char *fileName, int idAssunto);
void listarTodosIdAssunto(const char *fileName);
void ordenarId(Process process[], int n);

#endif