#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

FILE *LerDados(FILE *dados);
tPaciente *LerPacientes(FILE *dados, tPaciente *paciente);
tData FiltrarData(char *data);
int DataEstaNoIntervalo(tData data_menor, tData data_maior, tData dataAComparar);
void LerCidadesContarCasos(tPaciente *pacientes);
int TemComorbidade(tPaciente paciente);

void OrdemAlfabeticaQntMinimaCasos(int quantidade_min_casos);
void OrdemTopCasos(int top_n, tData data_menor, tData data_maior, tPaciente *pacientes);
void QuantidadeCasosEntreDatas(tData data_menor, tData data_maior, tPaciente *pacientes);
void Item_6(char nome_municipio[31], tPaciente *pacientes);
void Item_7(tData data_menor, tData data_maior, tPaciente *pacientes);
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

  // char destinoPasta[10];
  // fgets(destinoPasta, 10, stdin);

  int quantidadeMinCasos = 10;
  // scanf("%d", &quantidadeMinCasos);
  OrdemAlfabeticaQntMinimaCasos(quantidadeMinCasos);

  tData data_menor, data_maior;

  data_menor.mes = 7;
  data_menor.dias = 10;
  data_maior.mes = 8;
  data_maior.dias = 10;

  // int ano_inutil;
  // // scanf("%4d-%2d-%2d ", &ano_inutil, &data_menor.mes, &data_menor.dias);
  // // scanf("%4d-%2d-%2d", &ano_inutil, &data_maior.mes, &data_maior.dias);
  QuantidadeCasosEntreDatas(data_menor, data_maior, pacientes);

  // scanf("%*c");

  unsigned int top_n = 10;
  // scanf("%d %4d-%2d-%2d %4d-%2d-%2d ", &top_n, &ano_inutil, &data_menor.mes, &data_menor.dias, &ano_inutil, &data_maior.mes, &data_maior.dias);
  OrdemTopCasos(top_n, data_menor, data_maior, pacientes);

  // char municipio[31];
  // fgets(municipio, 31, stdin);
  char municipio[31] = {'c', 'o', 'l', 'a', 't', 'i', 'n', 'a'};
  Item_6(municipio, pacientes);

  // scanf("%4d-%2d-%2d ", &ano_inutil, &data_menor.mes, &data_menor.dias);
  // scanf("%4d-%2d-%2d", &ano_inutil, &data_maior.mes, &data_maior.dias);
  Item_7(data_menor, data_maior, pacientes);

  free(pacientes);
  pacientes = NULL;
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

  fclose(dados);
  dados = NULL;
  return paciente;
}

tData FiltrarData(char data[11])
{
  tData dataFiltrada;
  char *aux;
  aux = strtok(data, "-");
  aux = strtok(NULL, "-");
  dataFiltrada.mes = atoi(aux);
  aux = strtok(NULL, "-");
  dataFiltrada.dias = atoi(aux);

  free(aux);
  aux = NULL;
  return dataFiltrada;
}

int DataEstaNoIntervalo(tData data_menor, tData data_maior, tData dataAComparar)
{

  unsigned int media_entre_meses = (data_maior.mes + data_menor.mes) / 2;

  if (data_maior.mes == dataAComparar.mes && data_menor.mes == dataAComparar.mes)
  {
    if (data_maior.dias >= dataAComparar.dias && dataAComparar.dias >= data_menor.dias)
    {
      return 1;
    }
  }
  else if (data_menor.mes == dataAComparar.mes)
  {
    if (dataAComparar.dias >= data_menor.dias)
    {
      return 1;
    }
  }
  else if (data_maior.mes == dataAComparar.mes)
  {
    if (dataAComparar.dias <= data_maior.dias)
    {
      return 1;
    }
  }
  else if ((dataAComparar.mes >= (media_entre_meses)) && (dataAComparar.mes <= data_maior.mes))
  {
    return 1;
  }
  else if ((dataAComparar.mes <= (media_entre_meses)) && (dataAComparar.mes >= data_menor.mes))
  {
    return 1;
  }

  return 0;
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

void OrdemAlfabeticaQntMinimaCasos(int quantidade_min_casos)
{

  tCidadeseCasos auxiliar;

  for (int i = 0; i < 78; i++)
  {
    for (int j = i + 1; j < 78; j++)
    {
      if (strcmp(todasCidades[i].municipio, todasCidades[j].municipio) > 0)
      {
        auxiliar = todasCidades[i];
        todasCidades[i] = todasCidades[j];
        todasCidades[j] = auxiliar;
      }
    }
  }

  FILE *saida2;

  saida2 = fopen("item3.txt", "wb");

  for (int i = 0; i < 78; i++)
  {
    if (todasCidades[i].quantidade_casos > quantidade_min_casos)
    {
      fprintf(saida2, "- %s: %d casos\n", todasCidades[i].municipio, todasCidades[i].quantidade_casos);
    }
  }

  fclose(saida2);
}

void QuantidadeCasosEntreDatas(tData data_menor, tData data_maior, tPaciente *pacientes)
{

  int quantidade_casos_intervalo = 0;
  for (int i = 0; i < 202363; i++)
  {
    char auxDataPaciente[11];
    strcpy(auxDataPaciente, pacientes[i].dataCadastro);
    tData dataCadastro = FiltrarData(auxDataPaciente);
    if (DataEstaNoIntervalo(data_menor, data_maior, dataCadastro) == 1)
    {
      if (strcmp(pacientes[i].classificacao, "Confirmados") == 0)
      {
        quantidade_casos_intervalo++;
      }
    }
  }

  FILE *item4;
  item4 = fopen("item4.txt", "wb");
  fprintf(item4, "- Total de pessoas: %d", quantidade_casos_intervalo);
  fclose(item4);
  item4 = NULL;
}

void OrdemTopCasos(int top_n, tData data_menor, tData data_maior, tPaciente *pacientes)
{
  tCidadeseCasos auxiliar;
  tCidadeseCasos copiaDasCidades[78];

  for (int i = 0; i < 78; i++)
  {
    strcpy(copiaDasCidades[i].municipio, todasCidades[i].municipio);
    copiaDasCidades[i].quantidade_casos = 0;
  }

  for (int i = 0; i < 202363; i++)
  {
    char auxDataPaciente[11];
    strcpy(auxDataPaciente, pacientes[i].dataCadastro);
    tData dataCadastro = FiltrarData(auxDataPaciente);
    if (strcmp(pacientes[i].classificacao, "Confirmados") == 0)
    {
      if (DataEstaNoIntervalo(data_menor, data_maior, dataCadastro) == 1)
      {
        for (int j = 0; j < 78; j++)
        {
          if (strcmp(pacientes[i].municipio, copiaDasCidades[j].municipio) == 0)
          {
            copiaDasCidades[j].quantidade_casos++;
            break;
          }
        }
      }
    }
  }

  for (int i = 0; i < 78; i++)
  {
    for (int j = i + 1; j < 78; j++)
    {
      if (copiaDasCidades[i].quantidade_casos < copiaDasCidades[j].quantidade_casos)
      {
        auxiliar = copiaDasCidades[i];
        copiaDasCidades[i] = copiaDasCidades[j];
        copiaDasCidades[j] = auxiliar;
      }
    }
  }

  FILE *item5;

  item5 = fopen("item5.txt", "wb");

  for (int i = 0; i < top_n; i++)
  {
    fprintf(item5, "- %s: %d casos\n", copiaDasCidades[i].municipio, copiaDasCidades[i].quantidade_casos);
  }
  fclose(item5);
  item5 = NULL;
}

void Item_6(char nome_municipio[31], tPaciente *pacientes)
{

  int cont_confirmados = 0, quant_internadas = 0, quant_morreram = 0, quant_internadas_e_morreram = 0;

  for (int i = 0; i < 31; i++)
  {
    if (nome_municipio[i] == '\n')
    {
      nome_municipio[i] = '\0';
    }
    if (nome_municipio[i] != '\0')
    {
      nome_municipio[i] = toupper(nome_municipio[i]);
    }
  }

  if (strcmp(nome_municipio, "TODAS") == 0)
  {
    for (int i = 0; i < 202363; i++)
    {
      if (strcmp(pacientes[i].classificacao, "Confirmados") == 0)
      {

        cont_confirmados++;

        if (strcmp(pacientes[i].ficouInternado, "Sim") == 0)
        {
          quant_internadas++;
          if (strcmp(pacientes[i].dataObito, "0000-00-00") != 0)
          {
            quant_internadas_e_morreram++;
            quant_morreram++;
          }
        }
        else if (strcmp(pacientes[i].dataObito, "0000-00-00") != 0)
        {
          quant_morreram++;
        }
      }
    }
  }
  else
  {
    for (int i = 0; i < 202363; i++)
    {
      if (strcmp(pacientes[i].municipio, nome_municipio) == 0)
      {
        if (strcmp(pacientes[i].classificacao, "Confirmados") == 0)
        {

          cont_confirmados++;

          if (strcmp(pacientes[i].ficouInternado, "Sim") == 0)
          {
            quant_internadas++;
            if (strcmp(pacientes[i].dataObito, "0000-00-00") != 0)
            {
              quant_internadas_e_morreram++;
              quant_morreram++;
            }
          }
          else if (strcmp(pacientes[i].dataObito, "0000-00-00") != 0)
          {
            quant_morreram++;
          }
        }
      }
    }
  }

  float porcentagem_internados = 100.0 * (quant_internadas * 1.0 / cont_confirmados * 1.0);
  float porcentagem_morreram = (quant_morreram * 1.0 / cont_confirmados * 1.0) * 100.0;
  float porcentagem_internados_e_morreram = (quant_internadas_e_morreram * 1.0 / quant_morreram * 1.0) * 100.0;

  FILE *item6;
  item6 = fopen("item6.txt", "wb");

  fprintf(item6, "- Resultados para %s:\n", nome_municipio);
  fprintf(item6, "- A %% de pessoas com Covid-19 que ficaram internadas: %.3f%%\n", porcentagem_internados);
  fprintf(item6, "- A %% de pessoas com Covid-19 que morreram: %.3f%%\n", porcentagem_morreram);
  fprintf(item6, "- A %% de pessoas que ficaram internadas e morreram: %.3f%%", porcentagem_internados_e_morreram);

  fclose(item6);
  item6 = NULL;
}

void Item_7(tData data_menor, tData data_maior, tPaciente *pacientes)
{

  int quant_mortos = 0, total_idade = 0, morreram_sem_comorbidade = 0;
  // int *idade_pacientes_confirmados;

  for (int i = 0; i < 202363; i++)
  {
    if (strcmp(pacientes[i].dataObito, "0000-00-00") != 0)
    {

      quant_mortos++;

      int alg_1 = pacientes[i].idadeNaDataNotificacao[1];
      int alg_2 = pacientes[i].idadeNaDataNotificacao[2];
      int alg_3 = pacientes[i].idadeNaDataNotificacao[3];

      // total_idade += idade;

      if (TemComorbidade(pacientes[i]) == 0)
      {
        morreram_sem_comorbidade++;
      }
    }
  }

  float media_idades = total_idade * 1.0 / quant_mortos * 1.0;
  // float desvio_padrao;
  // for()

  FILE *item7;
  item7 = fopen("item7.txt", "wb");
  fprintf(item7, "- qnt mortos: %d\n", quant_mortos);
  fprintf(item7, "- totalidade: %d\n", total_idade);
  fprintf(item7, "- morreram sem comorbidade: %d\n", morreram_sem_comorbidade);
  fprintf(item7, "- medida_idades: %.3f%%\n", media_idades);
}

int TemComorbidade(tPaciente paciente)
{

  if (!(strcmp(paciente.comorbidadePulmao, "Sim") == 0))
  {
    if (!(strcmp(paciente.comorbidadeCardio, "Sim") == 0))
    {
      if (!(strcmp(paciente.comorbidadeObesidade, "Sim") == 0))
      {
        if (!(strcmp(paciente.comorbidadeRenal, "Sim") == 0))
        {
          if (!(strcmp(paciente.comorbidadeTabagismo, "Sim") == 0))
          {
            if (!(strcmp(paciente.comorbidadeDiabetes, "Sim") == 0))
            {
              return 0;
            }
          }
        }
      }
    }
  }

  return 1;
}