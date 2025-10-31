#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
// Se estivermos no windows, vamos incluir usar uma saída de console com padrão adequado para utf-8.
#ifdef _WIN32
#include <Windows.h>
#endif

// Código da Ilha – Edição Free Fire
// Nível: Novato
// Este programa simula o gerenciamento básico de uma mochila com componentes coletados durante a fuga de uma ilha.

// **** Definições de constantes. ****

/// @brief Tamanho reservado para o nome.
#define TAM_NOME 30
/// @brief Tamanho reservado para o tipo.
#define TAM_TIPO 20
/// @brief Tamanho reservado para a quantidade. Uso exclusivo em cabeçalhos.
#define TAM_QUANTIDADE 10
/// @brief Tamanho reservado para o máximo de itens.
#define TAM_MAX_ITEMS 10

// **** Declarações de estruturas. ****

/// @brief Define uma struct chamada(alias) Item com os campos char nome[TAM_NOME], char tipo[TAM_TIPO] e int quantidade.
typedef struct {
	char nome[TAM_NOME];
	char tipo[TAM_TIPO];
	int quantidade;
} Item;

/// @brief Define uma struct para um vetor estático(com alias) com capacidade para até TAM_MAX_ITEMS.
typedef struct {
	Item dados[TAM_MAX_ITEMS];
	int quantidade;
} VetorEstatico;

// **** Declarações das funções. ****

// **** Interface de usuário. ****

/// @brief Exibe o menu principal.
/// @param Ponteiro via referência para a opção.
/// @param Ponteiro para o vetor. Somente leitura.
void exibirMenuPrincipal(int* opcao, const VetorEstatico* vetor);
/// @brief Exibe o menu de inserção de itens.
/// @param Ponteiro via referência para o vetor.
void menuInserirItem(VetorEstatico* vetor);
/// @brief Exibe o menu de remoção de itens.
/// @param Ponteiro via referência para o vetor.
void menuRemoverItem(VetorEstatico* vetor);
/// @brief Exibe o menu para listagem de itens.
/// @param Ponteiro para o vetor. Somente leitura.
void menuListarItens(const VetorEstatico* vetor);
/// @brief Exibe os cabeçalhos
/// para os dados tabulados.
void exibirCabecalhos();
/// @brief Exibe um item para informações tabuladas.
/// @param Item a ser exibido. Somente Leitura.
void exibirItem(const Item* item);

// **** Funções de Lógica Principal e Gerenciamento de Memória ****

/// @brief Entrega uma estrutura inicializada contendo um vetor.
/// @returns Estrutura conforme especificação.
VetorEstatico inicializarVetor();
/// @brief Insere um novo item no vetor.
/// @param Ponteiro via referência para o vetor.
/// @param Item a ser inserido.
void inserirItemVetor(VetorEstatico* vetor, Item item);
/// @brief Remove um item existente do vetor.
/// @param Ponteiro via referência para o vetor.
/// @param Nome do item a ser usado na remoção. Somente leitura.
void removerItemVetor(VetorEstatico* vetor, const char* nome);
/// @brief Lista os itens existentes no vetor.
/// @param Ponteiro para o vetor. Somente leitura.
void listarItensVetor(const VetorEstatico* vetor);

// **** Funções utilitárias ****

/// @brief Limpa o buffer de entrada do teclado (stdin), evitando problemas 
/// com leituras consecutivas de scanf e getchar.
void limparBufferEntrada();
/// @brief Função para limpar '\n' deixado pelo fgets.
/// @param str Conteúdo do texto a ser analisado e limpo.
void limparEnter(char* str);


/// @brief Ponto de entrada do programa.
/// @returns Inteiro. Zero, em caso de sucesso. Ex: EXIT_SUCCESS. 
/// Ou diferente de Zero, em caso de falha. Ex: EXIT_FAILURE.
int main() {

	// Configurações de caracteres para ajuste de ortografia para saída do console(UTF-8).
#ifdef _WIN32
	// Plataforma Windows.
	SetConsoleOutputCP(CP_UTF8);
#endif

	// Menu principal com opções:
	// 1. Adicionar um item
	// 2. Remover um item
	// 3. Listar todos os itens
	// 4. Ordenar os itens por critério (nome, tipo, prioridade)
	// 5. Realizar busca binária por nome
	// 0. Sair
	// A estrutura switch trata cada opção chamando a função correspondente.
	// A ordenação e busca binária exigem que os dados estejam bem organizados.

	printf("======================================================\n");
	printf(" ==== 🏝️  Código da Ilha – Edição Free Fire  🏝️ ====\n");
	printf(" =================== Nível Novato ===================\n");
	printf("======================================================\n");

	int opcao = 0;

	VetorEstatico lista = inicializarVetor();

	do
	{
		exibirMenuPrincipal(&opcao, &lista);

		switch (opcao)
		{
		case 1:
			// Inserir item.
			menuInserirItem(&lista);
			break;
		case 2:
			// Remoção de item.
			menuRemoverItem(&lista);
			break;
		case 3:
			// Listagem de itens.
			menuListarItens(&lista);
			break;
		case 0:
			// Sair.
			printf("\n==== Saindo do sistema... ====\n");
			break;
		default:
			// Continua.
			printf("\n==== Opção inválida. ====\n");
			break;
		}

	} while (opcao != 0);

	printf("\n==== Operação encerrada. ====\n");

	return EXIT_SUCCESS;
}

// **** Implementação das funções. ****

// **** Interface de usuário. ****

void exibirMenuPrincipal(int* opcao, const VetorEstatico* vetor) 
{
	printf("\n Itens na Mochila: %d/%d \n", vetor->quantidade, TAM_MAX_ITEMS);
	printf("\n1 - Adicionar Item (Loot). \n");
	printf("2 - Remover Item pelo nome. \n");
	printf("3 - Listar todos os Itens. \n");
	printf("0 - Sair. \n");

	int invalido;

	do
	{
		printf("Escolha uma opção: ");
		invalido = scanf("%d", opcao) != 1; // Sem o operador '&'. Já temos um ponteiro.
		limparBufferEntrada();
		if (invalido)
		{
			printf("\n==== ⚠️  Entrada inválida. Tente novamente. ====\n");
		}
	} while (invalido);
}

void menuInserirItem(VetorEstatico* vetor)
{
	printf("\n==== Adicionar Novo Item ====\n");
	// Tratando as entradas de usuário.
	Item item;

	printf("\nDigite o Nome: ");
	fgets(item.nome, TAM_NOME, stdin);
	limparEnter(item.nome);

	printf("Digite Tipo: ");
	fgets(item.tipo, TAM_TIPO, stdin);
	limparEnter(item.tipo);

	int invalido = 0;

	do
	{
		printf("Digite a quantidade: ");
		invalido = scanf("%d", &item.quantidade) != 1 || item.quantidade < 1;
		limparBufferEntrada();
		if (invalido)
		{
			printf("\n==== ⚠️  Entrada inválida. Um valor acima de zero é esperado. ====\n");
		}
	} while (invalido);

	// Já podemos inserir o item.
	inserirItemVetor(vetor, item);
}

void menuRemoverItem(VetorEstatico* vetor)
{
	printf("\n==== Remover Item ====\n");

	printf("\nDigite o Nome: ");
	char nome[TAM_NOME];
	fgets(nome, TAM_NOME, stdin);
	limparEnter(nome);

	removerItemVetor(vetor, nome);
}

void menuListarItens(const VetorEstatico* vetor) {
	printf("\n==== Listar Itens ====\n");

	listarItensVetor(vetor);
}

void exibirCabecalhos()
{
	printf("\n============================================================================");
	printf("\n%-*s | %-*s | %-*s", TAM_NOME, "NOME", TAM_TIPO, "TIPO", TAM_QUANTIDADE, "QUANTIDADE");
}

void exibirItem(const Item* item)
{
	// Exibe os resultados reservando espaços e tabulando de acordo com os valores do item atual.
	printf("\n----------------------------------------------------------------------------");
	printf("\n%-*s | %-*s | %-*d", TAM_NOME, item->nome, TAM_TIPO, item->tipo, TAM_QUANTIDADE, item->quantidade);
}

// **** Funções de Lógica Principal e Gerenciamento de Memória ****

VetorEstatico inicializarVetor()
{
	VetorEstatico vetor; // Valor padrão de inicialização.
	vetor.quantidade = 0; // Quantidade zerada.

	return vetor;
}

void inserirItemVetor(VetorEstatico* vetor, Item item)
{
	if (vetor->quantidade == TAM_MAX_ITEMS)
	{
		printf("\nAviso ⚠️  O número de itens do vetor atingiu o limite.\n");
		return;
	}
	// Atribuindo os valores correspondentes aos membros do item em questão,
	// de acordo com a posição de navegação atual da lista.
	strcpy(vetor->dados[vetor->quantidade].nome, item.nome);
	strcpy(vetor->dados[vetor->quantidade].tipo, item.tipo);
	vetor->dados[vetor->quantidade].quantidade = item.quantidade;

	vetor->quantidade++; // Atualizando a quantidade de itens da lista e preparando a próxima posição.

	printf("\n✅  Item de Nome \"%s\" inserido com sucesso.\n", item.nome);
}

void removerItemVetor(VetorEstatico* vetor, const char* nome)
{
	for (int i = 0; i < vetor->quantidade; i++)
	{
		if (strcmp(vetor->dados[i].nome, nome) == 0)
		{
			// Reorganizando a lista para a remoção.
			for (int j = i; j < vetor->quantidade - 1; j++)
			{
				// Não temos ponteiros na struct de Item. Podemos efetuar diretamente a cópia completa.
				// O acesso não precisa ser executado membro a membro, nesse cenário.
				vetor->dados[j] = vetor->dados[j + 1];
			}

			vetor->quantidade--; // Atualizando a quantidade de itens.

			printf("\n ✅  Item Nome \"%s\" removido com sucesso.\n", nome);
			return;
		}
	}

	printf("\n ⚠️  Item Nome \"%s\" não encontrado no vetor.\n", nome);
}

void listarItensVetor(const VetorEstatico* vetor)
{
	if (vetor->quantidade == 0)
	{
		printf("\n ⚠️  O vetor estático está vazio! \n");
		return;
	}

	exibirCabecalhos();

	for (int i = 0; i < vetor->quantidade; i++)
	{
		exibirItem(&vetor->dados[i]);
	}

	printf("\n============================================================================\n");
}

// **** Funções utilitárias ****

void limparBufferEntrada()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

void limparEnter(char* str)
{
	str[strcspn(str, "\n")] = '\0';
}