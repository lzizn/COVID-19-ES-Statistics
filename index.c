#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  char municipio[31];
  int quantidade_casos;
} tCidadeseCasos;

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
  int mes;
  int dias;
} tData;

tCidadeseCasos todasCidades[78];
char destinoPasta[10];

FILE *LerDados(FILE *dados);
tPaciente *LerPacientes(FILE *dados, tPaciente *paciente);
void LerCidadesContarCasos(tPaciente *pacientes);
void OrdemTopCasos(int top_n);
void OrdemAlfabeticaQntMinimaCasos(int quantidade_min_casos);
void QuantidadeCasosEntreDatas(tData data_menor, tData data_maior, tPaciente *pacientes);
tData FiltrarData(char *data);
int stringsSaoIguais(char *str1, char *str2);

int main()
{
  FILE *dados;
  dados = LerDados(dados);

  tPaciente *pacientes;
  pacientes = malloc(202362 * sizeof(tPaciente));

  char cabecalhoInutil[201];
  fgets(cabecalhoInutil, 201, dados);

  pacientes = LerPacientes(dados, pacientes);
  LerCidadesContarCasos(pacientes);

  // fgets(destinoPasta, 10, stdin);
  // int quantidade_min_casos;
  // scanf("%d", &quantidade_min_casos);
  // OrdemAlfabeticaQntMinimaCasos(quantidade_min_casos);

  // tData data_menor, data_maior;
  // unsigned ano_inutil;
  // scanf("%4d-%2d-%2d ", &ano_inutil, &data_menor.mes, &data_menor.dias);
  // scanf("%4d-%2d-%2d", &ano_inutil, &data_maior.mes, &data_maior.dias);
  // QuantidadeCasosEntreDatas(data_menor, data_maior, pacientes);
  tData data_menor = {6, 5}, data_maior = {10, 1};
  QuantidadeCasosEntreDatas(data_menor, data_maior, pacientes);

  free(pacientes);
  fclose(dados);
  return 0;
}

FILE *LerDados(FILE *dados)
{
  dados = fopen("covid19ES.csv", "r");
  if (dados == NULL)
  {
    printf("Deu ruim");
    exit(1);
  }
  return dados;
}

tPaciente *LerPacientes(FILE *dados, tPaciente *paciente)
{

  char dadosPaciente[161];
  int contador_virgulas = 0, indice = 0, contador_aspas = 0, i = 0, j = 0, tamanho = 0;

  while (fgets(dadosPaciente, 160, dados) != NULL)
  {

    i = 0, indice = 0, contador_virgulas = 0, contador_aspas = 0;

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
        paciente[j].dataCadastro[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 1)
      {
        paciente[j].dataObito[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 2)
      {
        paciente[j].classificacao[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 3)
      {
        paciente[j].municipio[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 4)
      {
        paciente[j].idadeNaDataNotificacao[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 5)
      {
        paciente[j].comorbidadePulmao[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 6)
      {
        paciente[j].comorbidadeCardio[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 7)
      {
        paciente[j].comorbidadeRenal[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 8)
      {
        paciente[j].comorbidadeDiabetes[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 9)
      {
        paciente[j].comorbidadeTabagismo[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 10)
      {
        paciente[j].comorbidadeObesidade[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 11)
      {
        paciente[j].ficouInternado[indice] = dadosPaciente[i];
      }
      i++;
      indice++;
    }

    j++;
  }

  free(dados);
  return paciente;
}

void LerCidadesContarCasos(tPaciente *pacientes)
{
  int indice = 0;

  for (int i = 0; i < 78; i++)
  {
    todasCidades[i].quantidade_casos = 0;
  }

  for (int i = 0; i < 202363; i++)
  {

    if (strcmp(pacientes[i].classificacao, "Confirmados") == 0)
    {

      int ja_Esta_Cadastrado = 0;

      for (int j = 0; j < indice; j++)
      {
        if (strcmp(pacientes[i].municipio, todasCidades[j].municipio) == 0)
        {
          todasCidades[j].quantidade_casos++;
          ja_Esta_Cadastrado = 1;
          break;
        }
      }

      if (ja_Esta_Cadastrado == 0)
      {
        strcpy(todasCidades[indice].municipio, pacientes[i].municipio);
        todasCidades[indice].quantidade_casos++;
        indice++;
      }
    }
  }
}

int stringsSaoIguais(char *str1, char *str2)
{
  if (strcmp(str1, str2) == 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

// void OrdemTopCasos(int top_n)
// {
//   int aux;

//   for (int i = 0; i < 78; i++)
//   {
//     for (int j = i + 1; j < 78; j++)
//     {
//       if (todasCidades[i].quantidade_casos < todasCidades[j].quantidade_casos)
//       {
//         aux = todasCidades[i].quantidade_casos;
//         todasCidades[i].quantidade_casos = todasCidades[j].quantidade_casos;
//         todasCidades[j].quantidade_casos = aux;
//       }
//     }
//   }

//   FILE *saida1;

//   saida1 = fopen("saida1.txt", "wb");

//   for (int i = 0; i < top_n; i++)
//   {
//     fprintf(saida1, "- %s: %d casos\n", todasCidades[i].municipio, todasCidades[i].quantidade_casos);
//   }
//   fclose(saida1);
// }
void OrdemAlfabeticaQntMinimaCasos(int quantidade_min_casos)
{

  char aux[35];

  for (int i = 0; i < 78; i++)
  {
    for (int j = i + 1; j < 78; j++)
    {
      if (strcmp(todasCidades[i].municipio, todasCidades[j].municipio) > 0)
      {
        strcpy(aux, todasCidades[i].municipio);
        strcpy(todasCidades[i].municipio, todasCidades[j].municipio);
        strcpy(todasCidades[j].municipio, aux);
      }
    }
  }

  FILE *saida2;

  saida2 = fopen("saida2.txt", "wb");

  for (int i = 0; i < quantidade_min_casos; i++)
  {
    fprintf(saida2, "- %s: %d casos\n", todasCidades[i].municipio, todasCidades[i].quantidade_casos);
  }
  fclose(saida2);
}

void QuantidadeCasosEntreDatas(tData data_menor, tData data_maior, tPaciente *pacientes)
{
  unsigned int media_entre_meses = (data_maior.mes + data_menor.mes) / 2;

  int quantidade_casos_intervalo = 0;
  for (int i = 0; i < 202363; i++)
  {
    tData dataCadastro = FiltrarData(pacientes[i].dataCadastro);
    if (data_maior.mes == dataCadastro.mes && data_menor.mes == dataCadastro.mes)
    {
      if (data_maior.dias >= dataCadastro.dias && dataCadastro.dias >= data_menor.dias)
      {
        if (strcmp(pacientes[i].classificacao, "Confirmados") == 0)
        {
          quantidade_casos_intervalo++;
        }
      }
    }
    else if (data_menor.mes == dataCadastro.mes)
    {
      if (dataCadastro.dias >= data_menor.dias)
      {
        if (strcmp(pacientes[i].classificacao, "Confirmados") == 0)
        {
          quantidade_casos_intervalo++;
        }
      }
    }
    else if (data_maior.mes == dataCadastro.mes)
    {
      if (dataCadastro.dias <= data_maior.dias)
      {
        if (strcmp(pacientes[i].classificacao, "Confirmados") == 0)
        {
          quantidade_casos_intervalo++;
        }
      }
    }
    else if ((dataCadastro.mes >= (media_entre_meses)) && (dataCadastro.mes <= data_maior.mes))
    {
      if (strcmp(pacientes[i].classificacao, "Confirmados") == 0)
      {
        quantidade_casos_intervalo++;
      }
    }
    else if ((dataCadastro.mes <= (media_entre_meses)) && (dataCadastro.mes >= data_menor.mes))
    {
      if (strcmp(pacientes[i].classificacao, "Confirmados") == 0)
      {
        // printf("%d-%d\n", dataCadastro.mes, dataCadastro.dias);
        quantidade_casos_intervalo++;
      }
    }
    // printf("%d\n", quantidade_casos_intervalo);
  }
}

tData FiltrarData(char *data)
{
  tData dataFiltrada;
  char *aux;
  aux = strtok(data, "-");
  aux = strtok(NULL, "-");
  dataFiltrada.mes = atoi(aux);
  aux = strtok(NULL, "-");
  dataFiltrada.dias = atoi(aux);

  free(aux);
  return dataFiltrada;
}