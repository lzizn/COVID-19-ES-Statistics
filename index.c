#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// teste

typedef struct
{
    char dataCadastro[10];
    char dataObito[10];
    char classificacao[12];
    char municipio[31];
    char idadeNaDataNotificacao[31];
    char comorbidadePulmao[3];
    char comorbidadeRenal[3];
    char comorbidadeDiabetes[3];
    char comorbidadeTabagismo[3];
    char comorbidadeObesidade[3];
    char ficouInternado[13];
} tPaciente;

tPaciente LerPaciente(FILE *dados);

int main()
{
    FILE *dados;
    tPaciente paciente;
    dados = fopen("covid19ES.csv", "r");

    if(dados == NULL)
    {
        printf("DEU MERDA");
        exit(1);
    }

    paciente = LerPaciente(dados);
    for(int i = 0; i < 10; i++){
      printf("%c", paciente.dataCadastro[i]);
    }
    printf("\n");
    for(int i = 0; i < 10; i++){
      printf("%c", paciente.dataObito[i]);
    }
    printf("\n");
    int tamanho = strlen(paciente.classificacao);
    printf("----%d\n", tamanho);
    for(int i = 0; i < tamanho; i++){
      printf("%c", paciente.classificacao[i]);
    }
    printf("\n");
    fclose(dados);
    return 0;
}

tPaciente LerPaciente(FILE *dados)
{
  tPaciente paciente;
  char charDescartavel;
  int i;
  for(int i = 0; i < 10; i++)
  {
    paciente.dataCadastro[i] = fgetc(dados);
  }
  charDescartavel = fgetc(dados);

  for(int i = 0; i < 10; i++)
  {
    paciente.dataObito[i] = fgetc(dados);
  }
  charDescartavel = fgetc(dados);

  for(int i = 0; i < 30; i++)
  {
    charDescartavel = fgetc(dados);
    if(charDescartavel != ',')
    {
      paciente.classificacao[i] = charDescartavel;
    }
    else
    {
      break;
    }
  }
  for(int i = 0; i < 40; i++)
  {
    charDescartavel = fgetc(dados);
    if(charDescartavel != ',')
    {
      paciente.municipio[i] = charDescartavel;
    }
    else
    {
      break;
    }
  }

  for(int i = 0; i < 3; i++)
  {
    paciente.comorbidadePulmao[i] = fgetc(dados);
  }
  
  charDescartavel = fgetc(dados);
  for(int i = 0; i < 3; i++)
  {
    paciente.comorbidadeRenal[i] = fgetc(dados);
  }

  charDescartavel = fgetc(dados);
  for(int i = 0; i < 3; i++)
  {
    paciente.comorbidadeDiabetes[i] = fgetc(dados);
  }

  charDescartavel = fgetc(dados);
  for(int i = 0; i < 3; i++)
  {
    paciente.comorbidadeTabagismo[i] = fgetc(dados);
  }

  charDescartavel = fgetc(dados);
  for(int i = 0; i < 3; i++)
  {
    paciente.comorbidadeObesidade[i] = fgetc(dados);
  }

  charDescartavel = fgetc(dados);
  for(int i = 0; i < 3; i++)
  {
    paciente.ficouInternado[i] = fgetc(dados);
  }

  charDescartavel = fgetc(dados);
  return paciente;
}