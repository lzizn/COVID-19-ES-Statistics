/*
 *  Autores:
 *    - Laian Braum (2020101796)
 *    - Ryan Costa (2020101735)
 * 
 *  Repositório utilizado para versionamento
 *  (privado até finalizar as avaliações):
 *    https://github.com/laianbraum/Trabalho-pratico-Programacao-II
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>

/*
 * Tipos Definidos:
 *  - tCidadeseCasos -> guardar as Cidades e a quantidade de casos de cada uma
 *  - tPaciente -> guardar todas as informações de cada paciente (utilizamos somente strings)
 *  - tData -> usado para armazenar as Datas filtradas (de string para tData).
 */

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
  int ano;
  int mes;
  int dias;
} tData;

/*
 * As variáveis e funções estão com nomes intuitivos e descritivos, então consideramos,
 * de certa forma, que o próprio código já está bem autoexplicativo.
 * 
 * Criamos variáveis globais pois teríamos que repetir o mesmo parametro diversas
 * vezes para as funções, então olhando para as boas práticas de código, achamos mais ideal
 * criar três variáveis globais, afinal todos os itens giram em torno delas.
 * 
*/

#define tamanho 202362

tPaciente pacientes[tamanho];
tCidadeseCasos todasCidades[78];
char destinoPasta[10];

/*  O cabeçalho de funções está organizado na ordem:
 *    1 - entrada de dados,
 *    2 - utilitárias,
 *    3 - os itens em si (algoritmos e saídas de dados);
 *
 *  Explicaremos mais detalhadamente sobre as funções no bloco de código de cada uma;
 */

FILE *LerDados(FILE *dados); //Item 1
void LerPacientes(FILE *dados);
void LerCidadesContarCasos();

tData FiltrarData(char data[11]); // Item 2
int DataEstaNoIntervalo(tData data_menor, tData data_maior, tData dataAComparar);
int TemComorbidade(tPaciente paciente);

void Item_3(int quantidade_min_casos);
void Item_4(tData data_menor, tData data_maior);
void Item_5(int top_n, tData data_menor, tData data_maior);
void Item_6(char nome_municipio[31]);
void Item_7(tData data_menor, tData data_maior);

int main()
{

  FILE *dados;
  dados = LerDados(dados);

  /*  O cabeçalho inutil */

  char cabecalhoInutil[201];
  fgets(cabecalhoInutil, 201, dados);

  /*  "Preparando o terreno" para os itens posteriores, lendo
   * o arquivo csv e armazenando na variável global de pacientes (Item 1).
   * 
   * Após isso armazenando cada cidade e as informações de quantidade de casos (em todo o período).
  */

  LerPacientes(dados);

  LerCidadesContarCasos();

  /*  Onde a mágica começa:
   *  Lemos o destino do arquivo e assim a variável global destinoPasta está pronta para ser usada.
  */

  fgets(destinoPasta, 10, stdin);
  mkdir(destinoPasta, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  // Item 3

  int quantidadeMinCasos;
  scanf("%d", &quantidadeMinCasos);
  Item_3(quantidadeMinCasos);

  /* Preparando o terreno para os itens que precisam de data,
   * reutilizaremos essas variáveis em todo os itens necessários.
   */

  tData data_menor, data_maior;

  // Item 4

  scanf("%4d-%2d-%2d ", &data_menor.ano, &data_menor.mes, &data_menor.dias);
  scanf("%4d-%2d-%2d ", &data_menor.ano, &data_maior.mes, &data_maior.dias);
  Item_4(data_menor, data_maior);

  // Item 5

  unsigned int top_n;
  scanf("%d %4d-%2d-%2d %4d-%2d-%2d ", &top_n, &data_menor.ano, &data_menor.mes, &data_menor.dias, &data_maior.ano, &data_maior.mes, &data_maior.dias);
  Item_5(top_n, data_menor, data_maior);

  // Item 6

  char municipio[31];
  fgets(municipio, 31, stdin);
  Item_6(municipio);

  // Item 7

  scanf("%4d-%2d-%2d ", &data_maior.ano, &data_menor.mes, &data_menor.dias);
  scanf("%4d-%2d-%2d", &data_maior.ano, &data_maior.mes, &data_maior.dias);
  Item_7(data_menor, data_maior);

  /* E assim nós terminamos esse trabalho com muita fé no divino.
   * 
   * Pedimos perdão se algum pedaço de código não ficou claro, tentamos utilizar ao máximo
   * boas práticas e achamos que também fizemos um bom trabalho na componentização das funções.
   */

  fclose(dados);
  dados = NULL;

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
  dados = NULL;
}

void LerPacientes(FILE *dados)
{

  /*  Nossa forma de leitura de pacientes é via um contador de vírgulas.
   *  
   * Nós pegamos a linha inteira do arquivo com o fgets e armazenamos em um vetor de 161 chars, em seguida
   *  iteramos char por char e dependendo da vírgula armazenamos no campo do paciente.
   *  Para colocar isso em prática precisamos das seguintes variáveis:
   * 
   *    dadosPaciente -> é o array de char onde armazenaremos toda a linha
   * 
   *    countador_virgulas -> iterar a cada vírgula encontrada e assim mudar o campo de dado do paciente
   * 
   *    indice -> o índice de cada campo de chars dos dados do paciente (municipio, dataCadastro etc..)
   *    
   *    i -> o índice do vetor de dados do paciente que pegamos com o fgets
   * 
   *    j -> o índice para acessar cada paciente
   * 
   *    aspasEstaoAtivas -> verifica se achou aspas, usado para "ignorar" as vírgulas da idade (não aumentar o contador_vírgulas)
  */

  char dadosPaciente[161];
  int contador_virgulas = 0, indice = 0, i = 0, aspasEstaoAtivas = 0, j = 0;

  while (fgets(dadosPaciente, 160, dados) != NULL)
  {

    /*  Redefinindo as variáveis "controladoras" guardar o paciente anterior e pegar a nova linha de dados. */

    i = 0, indice = 0, contador_virgulas = 0, aspasEstaoAtivas = 0;

    while (dadosPaciente[i] != '\n')
    {

      /*  Controlador de aspas e vírgulas */

      if (dadosPaciente[i] == ',' && aspasEstaoAtivas == 0)
      {
        contador_virgulas++;
        indice = 0;
        i++;
        continue;
      }

      if (dadosPaciente[i] == '"')
      {
        if (aspasEstaoAtivas == 0)
        {
          aspasEstaoAtivas++;
        }
        else
        {
          aspasEstaoAtivas = 0;
        }
      }

      /* Com esses if else's nós pegamos todos os chars */

      if (contador_virgulas == 0)
      {
        pacientes[j].dataCadastro[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 1)
      {
        pacientes[j].dataObito[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 2)
      {
        pacientes[j].classificacao[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 3)
      {
        pacientes[j].municipio[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 4)
      {
        pacientes[j].idadeNaDataNotificacao[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 5)
      {
        pacientes[j].comorbidadePulmao[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 6)
      {
        pacientes[j].comorbidadeCardio[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 7)
      {
        pacientes[j].comorbidadeRenal[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 8)
      {
        pacientes[j].comorbidadeDiabetes[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 9)
      {
        pacientes[j].comorbidadeTabagismo[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 10)
      {
        pacientes[j].comorbidadeObesidade[indice] = dadosPaciente[i];
      }
      else if (contador_virgulas == 11)
      {
        pacientes[j].ficouInternado[indice] = dadosPaciente[i];
      }

      /*  Aqui o respectivo indice do campo do paciente já recebeu o dadosPaciente[i]
       *  e nós iteramos as duas variáveis
      */

      i++;
      indice++;
    }

    /* Terminando de ler o pacientes[j] ele itera o j e começa a ler pacientes[j+1] */

    j++;
  }

  fclose(dados);
  dados = NULL;
}

void LerCidadesContarCasos()
{

  /*    Essa função itera todo o vetor de pacientes e já guarda todos os casos confirmados
   *  e seus respectivos municípios.
   * 
   *    Primeiro criamos uma variável chamada "quantidadeCidadesGuardadas" que será responsável por verificar a quantidade
   *  de cidades que já foram cadastradas no vetor de cidades.
   * 
   *    Em seguida setamos como 0 a propriedade quantidade_casos de todo o  vetor "todasCidades[78]" a fim
   *  de utilizarmos operador unário posteriormente.
   * 
   *    Após isso iteramos o vetor e para cada paciente verificamos se o caso é confirmado
   *  usando a função strcmp, se não é confirmado pulamos para a próxima iteração,
   *  se é confirmado criamos uma variável chamada "ja_Esta_Cadastrado" com valor 0 ("não"),
   *  e iteramos um for j = 0; j < quantidadesCidadesGuardadas.
   * 
   *  Dentro desse for comparamos com strcmp se a cidade do paciente é igual à alguma
   *  já existente no vetor; 
   * 
   *  Se sim, setamos a variável já_Esta_Cadastrado para um ("sim"),
   *  iteramos a quantidade de casos desse muncípio e quebramos o for.
   * 
   *  Se não, fazemos uma cópia do municipio do paciente para a propriedade municipio do todasCidades e
   *  iteramos a quantidade_casos do município criado e também a quantidadeDeCidadesGuardadas.
   * 
   *  Dessa forma, ao final, teremos 78 municípios cadastrados com suas respectivas quantidade de casos.
  */

  int quantidadesCidadesGuardadas = 0;

  for (int i = 0; i < 78; i++)
  {
    todasCidades[i].quantidade_casos = 0;
  }

  for (int i = 0; i < 202363; i++)
  {
    if (strcmp(pacientes[i].classificacao, "Confirmados") == 0)
    {

      int ja_Esta_Cadastrado = 0;

      for (int j = 0; j < quantidadesCidadesGuardadas; j++)
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
        strcpy(todasCidades[quantidadesCidadesGuardadas].municipio, pacientes[i].municipio);
        todasCidades[quantidadesCidadesGuardadas].quantidade_casos++;
        quantidadesCidadesGuardadas++;
      }
    }
  }
}

tData FiltrarData(char data[11])
{

  /*  Essa função é utilizada somente para separar os meses e os dias de um array de char dados.

   *    Criamos uma string auxiliar que guarda os pedaços de string dados pela função
   *  strtok passando como parametro os traços('-') das strings de datas. Após isso
   *  transformamos a string em inteiro usando a função atoi e armazenamos nas propriedades
   *  "mes" e "dias" do nosso tipo definido tData.
   * 
   *  Ao final temos a Data filtrada e a retornamos.
  */

  char auxData[11];
  strcpy(auxData, data);

  tData dataFiltrada;

  char *aux = "\0";
  aux = strtok(auxData, "-");
  if (aux != NULL)
  {
    dataFiltrada.ano = atoi(aux);
  }

  aux = strtok(NULL, "-");
  if (aux != NULL)
  {
    dataFiltrada.mes = atoi(aux);
  }

  aux = strtok(NULL, "-");
  if (aux != NULL)
  {
    dataFiltrada.dias = atoi(aux);
  }

  return dataFiltrada;
}

int DataEstaNoIntervalo(tData data_menor, tData data_maior, tData dataAComparar)
{

  /*  
   *    Essa é uma função utilitária usada para verificar se uma data está dentro de um
   *  intervalo de datas. Se sim, retorna 1, se não, retorna 0.
   * 
   *  A ideia é bem simples, utilizamos somente condicionais.
   * 
   *  Primeiro fazemos verificações básicas como dataAComparar == dataMaior == dataMenor etc.
   * 
   *  A engenhosidade dessa função está na forma que verificamos se a Data está dentro de um
   *  intervalo de meses diferentes. Criamos uma média entre os meses e verificamos se a data
   *  está dentro do intervalo
   *  dataMenor <= data <= mediaEntreMeses ou mediaEntreMeses <=data <= dataMaior
   *  
   *  Dessa forma conseguimos verificar todas as datas.
  */

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

int TemComorbidade(tPaciente paciente)
{

  /*  Essa é uma função utilitária que verifica se o paciente tem qualquer comorbidade,
   * se tem, retorna 1, se não, retorna 0.
   * 
   * Criamos if's aninhados, nada de novo sob o sol.
   * 
  */

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

void Item_3(int quantidade_min_casos)
{

  /*  
  *     Nessa função implantamos um algoritmo de ordenação que itera cada municipio e compara ele a todos do vetor
  *   de cidades.
  *     Utilizando a função strcmp verificamos se o município1 tem o valor maior(soma dos valores de cada char em unicode)
  *   que outro município2, se sim, logo municipio2 tem valor menor, assim trocamos todo o struct de lugar (índice do vetor),
  *   utilizando um auxiliar. Assim ao final teremos o vetor organizado em ordem alfabética.
  * 
  *     Após organizar em ordem alfabética, só printamos no arquivo se a cidade tiver o número de casos estritamente maior
  *   do que o número lido no arquivo de entrada.
 */

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

  /* 
   *  Criamos um vetor de char auxiliar e copiamos a pasta de destino da entrada e concatenamos com
   * o nome de saída desse item 3 ("item3.txt"). Depois passamos esse vetor de char auxiliar para o fopen e criamos a
   * pasta e o arquivo.
   *
   * Esse mesmo padrão se repete para todos os itens.
  */

  FILE *item3;
  char auxiliarPasta[20];
  strcpy(auxiliarPasta, destinoPasta);
  strcat(auxiliarPasta, "item3.txt");
  item3 = fopen(auxiliarPasta, "w+");

  if (item3 == NULL)
  {
    exit(1);
  }

  for (int i = 0; i < 78; i++)
  {
    if (todasCidades[i].quantidade_casos > quantidade_min_casos)
    {
      fprintf(item3, "- %s: %d casos\n", todasCidades[i].municipio, todasCidades[i].quantidade_casos);
    }
  }

  fclose(item3);
  item3 = NULL;
}

void Item_4(tData data_menor, tData data_maior)
{

  /*  
   *    Nesse item 4 nós criamos uma variável para armazenar a quantidade de casos no intervalo e
   *  verificamos utilizando nossa função "DataEstaNoIntervalo" se a data do paciente está dentro
   *  do intervalo de datas que é dado como entrada , se ela retornar 1 (sim), verificamos com a função strcmp se o paciente
   *  tem classificação como Confirmado, se sim, aumentamos o a quant_casos_intervalo.
   * 
   *  Ao final dela printamos essa quantidade de casos do intervalo.
  */

  int quantidade_casos_intervalo = 0;
  for (int i = 0; i < 202363; i++)
  {
    tData dataCadastro = FiltrarData(pacientes[i].dataCadastro);
    if (DataEstaNoIntervalo(data_menor, data_maior, dataCadastro) == 1)
    {
      if (strcmp(pacientes[i].classificacao, "Confirmados") == 0)
      {
        quantidade_casos_intervalo++;
      }
    }
  }

  FILE *item4;
  char auxiliarPasta[20] = {'\0'};
  strcpy(auxiliarPasta, destinoPasta);
  strcat(auxiliarPasta, "item4.txt");
  item4 = fopen(auxiliarPasta, "w+");

  fprintf(item4, "- Total de pessoas: %d", quantidade_casos_intervalo);

  fclose(item4);
  item4 = NULL;
}

void Item_5(int top_n, tData data_menor, tData data_maior)
{

  /*  
   *  Primeiro nós resetamos a quantidade de casos do nosso vetor global todasCidades[78] pois precisamos
   * inserir novos dados nele.
   * 
   *  Fazemos um loop em todos os pacientes e para cada paciente verificamos se ele é um caso Confirmado,
   * se sim, verificamos se ele está dentro do intervalo de datas dado, caso esteja, fazemos um loop para iterarmos
   * as cidades e quando o municipio do paciente bate com o municipio iterado, aumentamos a propriedade quantidade_casos
   * desse mesmo município.
   *  Dessa forma conseguimos verificar a quantidade de casos entre as datas em cada municipio.
   *  
   *    Após isso utilizando uma variável auxiliar, usamos um algoritmo básico para ordenarmos o vetor de
   *  cidades de forma decrescente.
   */

  tData dataCadastro;

  for (int i = 0; i < 78; i++)
  {
    todasCidades[i].quantidade_casos = 0;
  }

  for (int i = 0; i < 202363; i++)
  {
    if (strcmp(pacientes[i].classificacao, "Confirmados") == 0)
    {
      dataCadastro = FiltrarData(pacientes[i].dataCadastro);
      if (DataEstaNoIntervalo(data_menor, data_maior, dataCadastro) == 1)
      {
        for (int j = 0; j < 78; j++)
        {
          if (strcmp(pacientes[i].municipio, todasCidades[j].municipio) == 0)
          {
            todasCidades[j].quantidade_casos++;
            break;
          }
        }
      }
    }
  }

  tCidadeseCasos auxiliar;

  for (int i = 0; i < 78; i++)
  {
    for (int j = i + 1; j < 78; j++)
    {
      if (todasCidades[i].quantidade_casos < todasCidades[j].quantidade_casos)
      {
        auxiliar = todasCidades[i];
        todasCidades[i] = todasCidades[j];
        todasCidades[j] = auxiliar;
      }
    }
  }

  FILE *item5;
  char auxiliarPasta[20];
  strcpy(auxiliarPasta, destinoPasta);
  strcat(auxiliarPasta, "item5.txt");
  item5 = fopen(auxiliarPasta, "wb");

  if (!(todasCidades[0].quantidade_casos == 0))
  {
    for (int i = 0; i < top_n; i++)
    {

      fprintf(item5, "- %s: %d casos\n", todasCidades[i].municipio, todasCidades[i].quantidade_casos);
    }
  }

  fclose(item5);
  item5 = NULL;
}

void Item_6(char nome_municipio[31])
{

  /*
   *  Primeiro setamos todos os caracteres do municipio de entrada como maiusculas e trocamos o \n por \0, pois
   * a forma que usamos para pegar esse dado estava vindo o \n. 
   * 
   *  Após isso verificamos se a entrada do municipio é "TODAS" ou se é um municipio específico e tomamos caminhos
   * diferentes a partir disso. A ideia dos 2 caminhos é a mesma. Caso a entrada for for "TODAS", nós pegamos os dados
   * necessários de todos os municípios. Por outro lado, se a entrada não for "TODAS", nós usamos um if a mais que verifica
   * se a cidade do paciente é igual à do município em questão e só pegamos os dados desse município. 
   * 
   *  A forma de tratamento desses dados foi via comparação de strings usando a strcmp, usando if's aninhados
   * nós verificamosse a classificação do paciente é Confirmada, se ficou internado, se veio à obito e ficou internado,
   * se veio à óbito sem ficar internado. 
   * 
   *  Ao final criamos as variáveis responsáveis por guardar essas porcentagens, fazemos os cálculos de porcentagens
   * e printamos de acordo com o padrão de saída.
   */

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
  char auxiliar[20];
  strcpy(auxiliar, destinoPasta);
  strcat(auxiliar, "item6.txt");
  item6 = fopen(auxiliar, "wb");

  fprintf(item6, "- Resultados para %s:\n", nome_municipio);
  fprintf(item6, "- A %% de pessoas com Covid-19 que ficaram internadas: %.3f%%\n", porcentagem_internados);
  fprintf(item6, "- A %% de pessoas com Covid-19 que morreram: %.3f%%\n", porcentagem_morreram);
  fprintf(item6, "- A %% de pessoas que ficaram internadas e morreram: %.3f%%", porcentagem_internados_e_morreram);

  fclose(item6);
  item6 = NULL;
}

void Item_7(tData data_menor, tData data_maior)
{

  /*
   *  Utilizando if's aninhados nós verificamos algumas condições necessárias.
   * Primeiro verificamos se a data de cadastro do paciente está dentro do intervalo dado,
   * se sim, verificamos se ele é um caso confirmado, daí, vemos se ele veio à óbito, assim,
   * conseguimos todos os pacientes dentro do intervalo que são confirmados e vieram à óbito.
   * 
   *  Com isso em mente, pegamos a idadeNaDataDeNotificacao de cada paciente e damos uma
   * filtradinha no primeiro elemento da idade, pois estava vindo as aspas do arquivo.
   * 
   *  Após isso, para conseguir a idade do paciente em inteiro, criamos um ponteiro de char
   * auxiliar que recebe o token retornado pela função strtok onde passamos como parametro um espaço, 
   * pois como a entrada de idade é no formato "27 anos, 6 meses, 1 dias", verifica-se o padrão
   * de espaço após o primeiro número, dessa forma só pegamos o número que representa a idade.
   * 
   * Agora temos o número e precisamos passar ele para inteiro, usamos a função atoi().
   * 
   * Verificamos que existem pacientes com idade -1 (???) e assim criamos um if que verifica
   * se a idade é maior que 0, se sim, somamos o total de idades e guardamos essa idade em um vetor
   * a fim de utilizar esses dados no cálculo da média de idade e o desvio padrão.
   * 
   * Iteramos a quantidade de mortos e verificamos se o paciente tem qualquer comorbidade usando
   * nossa função TemComorbidade(tPaciente paciente), se não tem nenhuma, iteramos a variável
   * morreram_sem_comorbidade.
   * 
   * Dessa forma temos todos os dados necessários para fazer os cálculos para esse item,
   * então tiramos a média dividindo o total_idades pela quantidade de mortos e usamos um for
   * que funciona como o somatório do desvio padrão e depois tiramos a raíz na tranquilidade de buda.
   * 
  */

  int quant_mortos = 0, total_idade = 0, morreram_sem_comorbidade = 0;
  int idade_pacientes_confirmados[10000];

  for (int i = 0; i < 202363; i++)
  {
    tData dataCadastro = FiltrarData(pacientes[i].dataCadastro);
    if (DataEstaNoIntervalo(data_menor, data_maior, dataCadastro) == 1)
    {
      if (strcmp(pacientes[i].classificacao, "Confirmados") == 0)
      {
        if (strcmp(pacientes[i].dataObito, "0000-00-00") != 0)
        {

          pacientes[i].idadeNaDataNotificacao[0] = '0';
          char *aux;
          aux = strtok(pacientes[i].idadeNaDataNotificacao, " ");
          int verificaIdade;
          verificaIdade = atoi(aux);
          if (verificaIdade > 0)
          {
            total_idade += verificaIdade;
            idade_pacientes_confirmados[quant_mortos] = verificaIdade;
          }

          quant_mortos++;

          if (TemComorbidade(pacientes[i]) == 0)
          {
            morreram_sem_comorbidade++;
          }
        }
      }
    }
  }

  FILE *item7;
  char auxiliarPasta[20];
  strcpy(auxiliarPasta, destinoPasta);
  strcat(auxiliarPasta, "item7.txt");
  item7 = fopen(auxiliarPasta, "wb");

  if (!(quant_mortos == 0))
  {
    double media_idades = total_idade * 1.0 / quant_mortos * 1.0;
    double porcentagem_morreram_sem_comorbidade = (morreram_sem_comorbidade * 100.0 / quant_mortos * 1.0);

    double somatorioDesvio = 0;

    for (int i = 0; i < quant_mortos; i++)
    {
      somatorioDesvio += pow((idade_pacientes_confirmados[i] - media_idades), 2);
    }

    double desvioPadrao = sqrt(somatorioDesvio / (quant_mortos - 1) * 1.0);

    fprintf(item7, "A media e desvio padrao da idade: %.3lf -- %.3lf\n", media_idades, desvioPadrao);
    fprintf(item7, "A %% de pessoas que morreram sem comorbidade: %.3lf%%", porcentagem_morreram_sem_comorbidade);
  }
  else
  {
    fprintf(item7, "A media e desvio padrao da idade: 0.000 -- 0.000\n");
    fprintf(item7, "A %% de pessoas que morreram sem comorbidade: 0.000%%");
  }

  fclose(item7);
}