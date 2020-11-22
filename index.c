#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

tPaciente LerPaciente(FILE *dados);

int main()
{
  FILE *dados;
  tPaciente *paciente;
  paciente = malloc(202362 * sizeof(tPaciente));

  char cabecalho[201];

  dados = fopen("covid19ES.csv", "r");
  if(dados == NULL)
  {
    printf("DEU MERDA");
    exit(1);
  }

  fgets(cabecalho, 201, dados);
  printf("%s", cabecalho);

  for(int  i = 0; i < 202362; i++){
    paciente[i] = LerPaciente(dados);
  }

  fclose(dados);
  free(paciente);
  return 0;
}

tPaciente LerPaciente(FILE *dados)
{

  tPaciente paciente;
  char dadosPaciente[151];
  unsigned int contador_virgulas = 0, indice = 0, contador_aspas = 0, i = 0;
  fgets(dadosPaciente, 150, dados);

  while(dadosPaciente[i] != '\n'){
    if (dadosPaciente[i] == ',' && contador_aspas == 0){
      contador_virgulas++;
      indice = 0;
      i++;
      continue;
    }

    if(dadosPaciente[i] == '"'){
      if(contador_aspas == 0){
        contador_aspas++;
      }else{
        contador_aspas = 0;
      }
    }

    if(contador_virgulas == 0){
      paciente.dataCadastro[indice] = dadosPaciente[i];
      paciente.dataCadastro[indice+1] = '\0';
    }
    else if(contador_virgulas == 1){
      paciente.dataObito[indice] = dadosPaciente[i];
      paciente.dataObito[indice+1] = '\0';
    }
    else if(contador_virgulas == 2){
      paciente.classificacao[indice] = dadosPaciente[i];
      paciente.classificacao[indice+1] = '\0';
    }
    else if(contador_virgulas == 3){
      paciente.municipio[indice] = dadosPaciente[i];
      paciente.municipio[indice+1] = '\0';
    }
    else if(contador_virgulas == 4){
      paciente.idadeNaDataNotificacao[indice] = dadosPaciente[i];
      paciente.idadeNaDataNotificacao[indice+1] = '\0';
    }
    else if(contador_virgulas == 5){
      paciente.comorbidadePulmao[indice] = dadosPaciente[i];
      paciente.comorbidadePulmao[indice+1] = '\0';
    }
    else if(contador_virgulas == 6){
      paciente.comorbidadeCardio[indice] = dadosPaciente[i];
      paciente.comorbidadeCardio[indice+1] = '\0';
    }
    else if(contador_virgulas == 7){
      paciente.comorbidadeRenal[indice] = dadosPaciente[i];
      paciente.comorbidadeRenal[indice+1] = '\0';
    }
    else if(contador_virgulas == 8){
      paciente.comorbidadeDiabetes[indice] = dadosPaciente[i];
      paciente.comorbidadeDiabetes[indice+1] = '\0';
    }
    else if(contador_virgulas == 9){
      paciente.comorbidadeTabagismo[indice] = dadosPaciente[i];
      paciente.comorbidadeTabagismo[indice+1] = '\0';
    }
    else if(contador_virgulas == 10){
      paciente.comorbidadeObesidade[indice] = dadosPaciente[i];
      paciente.comorbidadeObesidade[indice+1] = '\0';
    }
    else if(contador_virgulas == 11){
      paciente.ficouInternado[indice] = dadosPaciente[i];
      paciente.ficouInternado[indice+1] = '\0';
    }
    i++;
    indice++;
  }

  // printf("%s,", paciente.dataCadastro);
  // printf("%s,", paciente.dataObito);
  // printf("%s,", paciente.classificacao);
  // printf("%s,", paciente.municipio);
  // printf("%s,", paciente.idadeNaDataNotificacao);
  // printf("%s,", paciente.comorbidadePulmao);
  // printf("%s,", paciente.comorbidadeCardio);
  // printf("%s,", paciente.comorbidadeRenal);
  // printf("%s,", paciente.comorbidadeDiabetes);
  // printf("%s,", paciente.comorbidadeTabagismo);
  // printf("%s,", paciente.comorbidadeObesidade);
  // printf("%s\n", paciente.ficouInternado);
}