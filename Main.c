#include <stdio.h>      // Fornece funções de entrada e saída padrão, como printf, scanf, fgets, etc.
#include <stdlib.h>     // Contém funções utilitárias como malloc, free, atoi, atof, exit, etc.
#include <string.h>     // Permite manipulação de strings (strcmp, strlen, strcpy, etc.).
#include <locale.h>     // Usada para configurar a localização (locale), útil para suportar acentos e caracteres especiais.
#include <conio.h>      // Fornece funções de console como getch(), usada para capturar entrada do teclado sem pressionar Enter (somente no Windows).
#include <windows.h>    // Permite interações com a API do Windows, como controle de cores no terminal (via SetConsoleTextAttribute).

void mudarCor(int corTexto, int corFundo) {
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (corFundo << 4) | corTexto);
}

typedef struct {
  char marca[50];
  char modelo[50];
  int anoFabricacao;
  int capacidadeDeMemoria;
  float preco;
} Smartphone;

Smartphone smartphones[100];
int total = 0;

void inserirDadoOrdenado(Smartphone s) {
  int i = total - 1;
  while (i >= 0 && smartphones[i].preco > s.preco) {
    smartphones[i + 1] = smartphones[i];
    i--;
  }
  smartphones[i + 1] = s;
  total++;
}

void carregarArquivo() {
  FILE *file = fopen("smartphones.txt", "r");
  if (!file) {
    printf("Erro ao abrir arquivo\n");
    return;
  }

  Smartphone s;
  char linha[100];
  while (fgets(s.marca, sizeof(s.marca), file) != NULL) {
    int len = strlen(s.marca);
    if (len > 0 && s.marca[len - 1] == '\n') s.marca[len - 1] = '\0';

    if (fgets(s.modelo, sizeof(s.modelo), file) == NULL) break;
    len = strlen(s.modelo);
    if (len > 0 && s.modelo[len - 1] == '\n') s.modelo[len - 1] = '\0';

    if (fgets(linha, sizeof(linha), file) == NULL) break;
    s.anoFabricacao = atoi(linha);

    if (fgets(linha, sizeof(linha), file) == NULL) break;
    s.capacidadeDeMemoria = atoi(linha);

    if (fgets(linha, sizeof(linha), file) == NULL) break;
    s.preco = atof(linha);

    inserirDadoOrdenado(s);
  }

  fclose(file);
}

void salvarArquivo() {
  FILE *file = fopen("smartphones.txt", "w");
  if (!file) {
    printf("Erro ao abrir arquivo para escrita\n");
    return;
  }

  for (int i = 0; i < total; i++) {
    fprintf(file, "%s\n%s\n%d\n%d\n%.2f\n",
            smartphones[i].marca,
            smartphones[i].modelo,
            smartphones[i].anoFabricacao,
            smartphones[i].capacidadeDeMemoria,
            smartphones[i].preco);
  }

  fclose(file);
}

void exibirTodosSmartphones() {
  mudarCor(0, 10);
  printf("                Lista de Smartphones                \n");
  for (int i = 0; i < total; i++) {
    mudarCor(15, 0);
    printf("[%d] %s %s %d %dGB R$%.2f\n", i,
           smartphones[i].marca, smartphones[i].modelo,
           smartphones[i].anoFabricacao, smartphones[i].capacidadeDeMemoria,
           smartphones[i].preco);
  }
}

void exibirPorMarca() {
  mudarCor(0, 9);
  printf("                 Listar por marca                   \n");
  char marcaBusca[50];
  mudarCor(7, 0);
  printf("Digite a marca:");
  scanf(" %50[^\n]", marcaBusca);
  printf("Smartphones da marca %s:\n", marcaBusca);
  for (int i = 0; i < total; i++) {
    if (strcmp(smartphones[i].marca, marcaBusca) == 0) {
      printf("[%d] %s %s %d %dGB R$%.2f\n", i,
             smartphones[i].marca, smartphones[i].modelo,
             smartphones[i].anoFabricacao, smartphones[i].capacidadeDeMemoria,
             smartphones[i].preco);
    }
  }
}

void exibirPorPreco() {
  mudarCor(0, 13);
  printf("                 Listar por preço                   \n");
  mudarCor(7, 0);
  float precoMin, precoMax;
  char buffer[100];

  printf("Digite o preço mínimo e máximo (separados por espaço): ");
  fflush(stdin);
  fgets(buffer, sizeof(buffer), stdin);

  if (sscanf(buffer, "%f %f", &precoMin, &precoMax) != 2) {
    printf("Entrada inválida! Digite dois números separados por espaço.\n");
    return;
  }

  printf("\nSmartphones com preço entre R$%.2f e R$%.2f:\n", precoMin, precoMax);

  int encontrados = 0;
  for (int i = 0; i < total; i++) {
    if (smartphones[i].preco >= precoMin && smartphones[i].preco <= precoMax) {
      printf("[%d] %s %s %d %dGB R$%.2f\n", i,
             smartphones[i].marca, smartphones[i].modelo,
             smartphones[i].anoFabricacao, smartphones[i].capacidadeDeMemoria,
             smartphones[i].preco);
      encontrados++;
    }
  }

  if (encontrados == 0) {
    printf("Nenhum smartphone encontrado na faixa de preço informada.\n");
  }
}

void inserirSmartphoneNovo() {
  mudarCor(0, 14);
  printf("              Inserir novo Smartphone               \n");
  mudarCor(7, 0);
  Smartphone s;
  printf("Digite a marca:");
  scanf(" %50[^\n]", s.marca);
  printf("Digite o modelo:");
  scanf(" %50[^\n]", s.modelo);
  printf("Digite o ano:");
  scanf("%d", &s.anoFabricacao);
  printf("Digite a memória (GB):");
  scanf("%d", &s.capacidadeDeMemoria);
  printf("Digite o preço:");
  scanf("%f", &s.preco);
  inserirDadoOrdenado(s);
  salvarArquivo();
  printf("Smartphone inserido com sucesso!\n");
}

void removerMemoriaInferior() {
  mudarCor(0, 11);
  printf("     Remover por memória inferior à especificada    \n");
  mudarCor(7, 0);
  int minMemoria;
  printf("Digite a memória mínima (GB):");
  scanf("%d", &minMemoria);

  int removidos = 0;
  int i = 0;
  while (i < total) {
    if (smartphones[i].capacidadeDeMemoria < minMemoria) {
      for (int j = i; j < total - 1; j++) {
        smartphones[j] = smartphones[j + 1];
      }
      total--;
      removidos++;
    } else {
      i++;
    }
  }

  salvarArquivo();
  if (removidos > 0) {
    printf("%d smartphones removidos com memória inferior a %dGB.\n", removidos, minMemoria);
  } else {
    printf("Nenhum smartphone foi removido. Todos possuem memória igual ou superior a %dGB.\n", minMemoria);
  }
}

void sairPrograma() {
  mudarCor(0, 4);
  printf("Saindo do Programa...\n");
  exit(0);
}

void menu() {
  int opcao;
  char entrada[10];
  do {
    mudarCor(1, 15);
    printf("==================== MENU ====================\n");
    mudarCor(2, 15);
    printf("1 -> Exibir todos os smartphones\n");
    mudarCor(1, 15);
    printf("2 -> Exibir por marca\n");
    mudarCor(5, 15);
    printf("3 -> Exibir por faixa de preço\n");
    mudarCor(6, 15);
    printf("4 -> Inserir novo smartphone\n");
    mudarCor(3, 15);
    printf("5 -> Remover por memória inferior à especificada\n");
    mudarCor(0, 4);
    printf("6 -> Sair\n");
    mudarCor(7, 0);
    printf("Escolha uma opção: ");

    fflush(stdin);
    fgets(entrada, sizeof(entrada), stdin);
    if (sscanf(entrada, "%d", &opcao) != 1) {
      printf("Entrada inválida. Tente novamente.\n");
      continue;
    }

    switch(opcao) {
      case 1: exibirTodosSmartphones(); break;
      case 2: exibirPorMarca(); break;
      case 3: exibirPorPreco(); break;
      case 4: inserirSmartphoneNovo(); break;
      case 5: removerMemoriaInferior(); break;
      case 6: sairPrograma(); break;
      default: printf("Inválido, tente novamente.\n"); break;
    }
  } while (opcao != 6);
}

int main() {
  SetConsoleOutputCP(65001); // UTF-8
  setlocale(LC_ALL, "pt_BR.UTF-8");
  carregarArquivo();
  menu();
  printf("Pressione qualquer tecla para sair...");
  getch();
  return 0;
}
