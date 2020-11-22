#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

typedef struct
{
  char dataCadastro[11];
  char dataObito[11];
  char classificacao[13];
  char municipio[32];
  char idadeNaDataNotificacao[32];
  char comorbidadePulmao[5];
  char comorbidadeCardio[5];
  char comorbidadeRenal[5];
  char comorbidadeDiabetes[5];
  char comorbidadeTabagismo[5];
  char comorbidadeObesidade[5];
  char ficouInternado[16];
} tPaciente;

typedef struct
{
  char municipio[32];
  int num_casos;
} tCasosPorCidade;

tPaciente LerPaciente(FILE *dados);
int FiltrarData(char *data1);
int DiasEmUmIntervalo(char *data1, char *data2);
int StringsSaoIguais(char *str1, char *str2);

int main()
{
  FILE *dados;
  tPaciente *paciente;
  tCasosPorCidade *todasCidades;
  char cabecalhoInutil[201];

  paciente = malloc(202362 * sizeof(tPaciente));
  todasCidades = malloc(78 * sizeof(tCasosPorCidade));

  dados = fopen("covid19ES.csv", "r");
  if (dados == NULL)
  {
    printf("Deu ruim");
    exit(1);
  }

  fgets(cabecalhoInutil, 201, dados);

  int tamanho = 0;

  for (int i = 0; i < 202362; i++)
  {
    unsigned int ehIgual = 0;
    paciente[i] = LerPaciente(dados);
    for (int j = 0; j < tamanho; j++)
    {
      if (StringsSaoIguais(paciente[i].municipio, todasCidades[j].municipio) == TRUE)
      {
        ehIgual = 1;
        break;
      }
    }
    if (ehIgual == 0)
    {
      char *str;
      memcpy(todasCidades[tamanho].municipio, str, strlen(str));
      // mcpy(paciente[i].municipio, todasCidades[tamanho].municipio, 4);
      printf("%s\n", todasCidades[tamanho].municipio);
      tamanho++;
      free(str);
    }
  }

  fclose(dados);

  free(paciente);
  free(todasCidades);

  return 0;
}

tPaciente LerPaciente(FILE *dados)
{

  tPaciente paciente;
  char dadosPaciente[151];
  unsigned int contador_virgulas = 0, indice = 0, contador_aspas = 0, i = 0;
  fgets(dadosPaciente, 150, dados);

  while (dadosPaciente[i] != '\n')
  {
    if (dadosPaciente[i] == ',' && contador_aspas == 0)
    {
      contador_virgulas++;
      indice = 0;
      i++;
      continue;
    }

    if (dadosPaciente[i] == '"')
    {
      if (contador_aspas == 0)
      {
        contador_aspas++;
      }
      else
      {
        contador_aspas = 0;
      }
    }

    if (contador_virgulas == 0)
    {
      paciente.dataCadastro[indice] = dadosPaciente[i];
      paciente.dataCadastro[indice + 1] = '\0';
    }
    else if (contador_virgulas == 1)
    {
      paciente.dataObito[indice] = dadosPaciente[i];
      paciente.dataObito[indice + 1] = '\0';
    }
    else if (contador_virgulas == 2)
    {
      paciente.classificacao[indice] = dadosPaciente[i];
      paciente.classificacao[indice + 1] = '\0';
    }
    else if (contador_virgulas == 3)
    {
      paciente.municipio[indice] = dadosPaciente[i];
      paciente.municipio[indice + 1] = '\0';
    }
    else if (contador_virgulas == 4)
    {
      paciente.idadeNaDataNotificacao[indice] = dadosPaciente[i];
      paciente.idadeNaDataNotificacao[indice + 1] = '\0';
    }
    else if (contador_virgulas == 5)
    {
      paciente.comorbidadePulmao[indice] = dadosPaciente[i];
      paciente.comorbidadePulmao[indice + 1] = '\0';
    }
    else if (contador_virgulas == 6)
    {
      paciente.comorbidadeCardio[indice] = dadosPaciente[i];
      paciente.comorbidadeCardio[indice + 1] = '\0';
    }
    else if (contador_virgulas == 7)
    {
      paciente.comorbidadeRenal[indice] = dadosPaciente[i];
      paciente.comorbidadeRenal[indice + 1] = '\0';
    }
    else if (contador_virgulas == 8)
    {
      paciente.comorbidadeDiabetes[indice] = dadosPaciente[i];
      paciente.comorbidadeDiabetes[indice + 1] = '\0';
    }
    else if (contador_virgulas == 9)
    {
      paciente.comorbidadeTabagismo[indice] = dadosPaciente[i];
      paciente.comorbidadeTabagismo[indice + 1] = '\0';
    }
    else if (contador_virgulas == 10)
    {
      paciente.comorbidadeObesidade[indice] = dadosPaciente[i];
      paciente.comorbidadeObesidade[indice + 1] = '\0';
    }
    else if (contador_virgulas == 11)
    {
      paciente.ficouInternado[indice] = dadosPaciente[i];
      paciente.ficouInternado[indice + 1] = '\0';
    }
    i++;
    indice++;
  }
  return paciente;
}

int StringsSaoIguais(char *str1, char *str2)
{
  if (memcmp(str1, str2, 8) == 0)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

int DiasEmUmIntervalo(char *data1, char *data2)
{
  unsigned int dia1, dia2, mes1, mes2, ano1, ano2;
  char *quebrar_data1, *quebrar_data2;

  quebrar_data1 = strtok(data1, "-");
  ano1 = atoi(quebrar_data1);

  quebrar_data1 = strtok(NULL, "-");
  mes1 = atoi(quebrar_data1);

  quebrar_data1 = strtok(NULL, "-");
  dia1 = atoi(quebrar_data1);

  quebrar_data2 = strtok(data2, "-");
  ano2 = atoi(quebrar_data2);

  quebrar_data2 = strtok(NULL, "-");
  mes2 = atoi(quebrar_data2);

  quebrar_data2 = strtok(NULL, "-");
  dia2 = atoi(quebrar_data2);

  free(quebrar_data1);
  free(quebrar_data2);
}

int FiltrarData(char *data1)
{
  unsigned int dia, mes, ano;
  char *quebrar_data;

  quebrar_data = strtok(data1, "-");
  ano = atoi(quebrar_data);

  quebrar_data = strtok(NULL, "-");
  mes = atoi(quebrar_data);

  quebrar_data = strtok(NULL, "-");
  dia = atoi(quebrar_data);

  free(quebrar_data);
}