#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
// Se estivermos no windows, vamos incluir usar uma saída de console com padrão adequado para utf-8.
#ifdef _WIN32
#include <Windows.h>
#endif

// Código da Ilha – Edição Free Fire
// Nível: Aventureiro
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.

// Definições de constantes.

/// @brief Tamanho reservado para o nome.
#define TAM_NOME 30
/// @brief Tamanho reservado para o tipo.
#define TAM_TIPO 20
/// @brief Tamanho reservado para a quantidade. Uso exclusivo em cabeçalhos.
#define TAM_QUANTIDADE 10
/// @brief Tamanho reservado para o máximo de itens.
#define TAM_MAX_ITEMS 10

// Declarações de estruturas.

/// @brief Define uma struct chamada Item com os campos char nome[30], char tipo[20] e int quantidade.
typedef struct {
	char nome[TAM_NOME];
	char tipo[TAM_TIPO];
	int quantidade;
} Item;

/// @brief Define uma struct para um vetor estático com capacidade para até TAM_MAX_ITEMS.
typedef struct {
	Item dados[TAM_MAX_ITEMS];
	int quantidade;
} VetorEstatico;

/// @brief Define uma struct com os campos Item dados e struct No* proximo, para a lista encadeada.
struct No {
	Item dados;
	struct No* proximo;
};

/// @brief Define a struct com um alias No, para uso com a lista encadeada.
typedef struct No No;

/// @brief Define um apontamento(além de um Alias) para a struct No* proximo, representando uma lista encadeada.
typedef No* ListaEncadeada;

// **** Declarações das funções. ****

// **** Funções de Interface de Usuário ****

/// @brief Exibe o menu inicial, com instruções sobre o 
/// tipo de estrutura a ser usada.
/// @returns Verdadeiro(true), se um vetor será usado. Caso contrário Falso(false). Ou seja, uma lista encadeada. 
bool exibirMenuInicial();
/// @brief Exibe o menu principal.
/// @param Ponteiro via referência para a opção.
/// @param Flag indicando se a estrutura a ser usada é um vetor ou lista encadeada.
/// @param Quantidade de itens existentes atualmente na mochila.
void exibirMenuPrincipal(int* opcao, bool usarVetor, int qtdeItensMochila);
/// @brief Exibe o menu de inserção de itens.
/// @param Flag indicando se a estrutura a ser usada é um vetor ou lista encadeada.
/// @param Ponteiro via referência para o vetor.
/// @param Ponteiro via referência para a lista encadeada.
void menuInserirItem(bool usarVetor, VetorEstatico* vetor, ListaEncadeada* lista);
/// @brief Exibe o menu de remoção de itens.
/// @param Flag indicando se a estrutura a ser usada é um vetor ou lista encadeada.
/// @param Ponteiro via referência para o vetor.
/// @param Ponteiro via referência para a lista encadeada.
void menuRemoverItem(bool usarVetor, VetorEstatico* vetor, ListaEncadeada* lista);
/// @brief Exibe o menu de listagem de itens.
/// @param Flag indicando se a estrutura a ser usada é um vetor ou lista encadeada.
/// @param Ponteiro via referência para o vetor. Somente leitura.
/// @param Ponteiro via referência para a lista encadeada. Somente leitura.
void menuListarItens(bool usarVetor, const VetorEstatico* vetor, const ListaEncadeada* lista);
/// @brief Exibe o menu de listagem de itens.
/// @param Flag indicando se a estrutura a ser usada é um vetor ou lista encadeada.
/// @param Ponteiro via referência para o vetor.
/// @param Ponteiro via referência para a lista encadeada.
void menuBuscarItem(bool usarVetor, VetorEstatico* vetor, ListaEncadeada* lista);
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
/// @brief Entrega uma estrutura inicializada para uma lista encadeada.
/// @returns Estrutura conforme especificação.
ListaEncadeada inicializarLista();
/// @brief Entrega o número de itens atualmente existentes no vetor.
/// @param Ponteiro para o vetor. Somente leitura.
/// @returns Número conforme especificação.
int contarItensVetor(const VetorEstatico* vetor);
/// @brief Entrega o número de itens atualmente existentes na lista encadeada.
/// @param Ponteiro para a lista. Somente leitura.
/// @returns Número conforme especificação.
int contarItensLista(const ListaEncadeada* lista);
/// @brief Insere um novo item no vetor.
/// @param Ponteiro via referência para o vetor.
/// @param Item a ser inserido.
void inserirItemVetor(VetorEstatico* vetor, Item item);
/// @brief Insere um novo item na lista encadeada.
/// @param Ponteiro via referência para a lista encadeada.
/// @param Item a ser inserido.
void inserirItemLista(ListaEncadeada* lista, Item item);
/// @brief Remove um item existente do vetor.
/// @param Ponteiro via referência para o vetor.
/// @param Nome do item a ser usado na remoção. Somente leitura.
void removerItemVetor(VetorEstatico* vetor, const char* nome);
/// @brief Remove um item existente da lista encadeada.
/// @param Ponteiro via referência para a lista encadeada.
/// @param Nome do item a ser usado na remoção. Somente leitura.
void removerItemLista(ListaEncadeada* lista, const char* nome);
/// @brief Efetua uma troca dos ponteiros.
/// @param Ponteiro via referência do item, para o primeiro operando.
/// @param Ponteiro via referência do item, para o segundo operando.
void trocarItemVetor(Item* a, Item* b);
/// @brief Efetua a ordenação dos itens do vetor da estrutura, via estratégia de bolha.
/// @param Ponteiro via referência para o vetor.
void bubbleSortNomeVetor(VetorEstatico* vetor);
/// @brief Efetua uma troca dos ponteiros do nó atual e de sua referência ao próximo.
/// @param Ponteiro via referência do nó atual.
/// @returns O nó referenciando o próximo, com conteúdo e endereçamento do atual
/// que também via referência, recebeu o conteúdo e endereçamento do próximo.
No* trocarNoLista(No* atual);
/// @brief Efetua a ordenação dos itens da lista encadeada, via estratégia de bolha.
/// @param Ponteiro via referência para a lista encadeada.
void bubbleSortNomeLista(ListaEncadeada* lista);
/// @brief Efetua a busca sequencial do item no vetor, usando o nome.
/// @param Ponteiro para o vetor. Somente leitura.
/// @param Nome a ser usado na busca. Somente leitura.
void buscarItemVetorSequencial(const VetorEstatico* vetor, const char* nome);
/// @brief Efetua a busca binária do item no vetor, usando o nome.
/// @param Ponteiro para o vetor previamente ordenado. Somente leitura.
/// @param Nome a ser usado na busca. Somente leitura.
void buscarItemVetorBinario(const VetorEstatico* vetorOrdenado, const char* nome);
/// @brief Efetua a busca sequencial do item na lista, usando o nome.
/// @param Ponteiro para a lista encadeada. Somente leitura.
/// @param Nome a ser usado na busca. Somente leitura.
void buscarItemListaSequencial(const ListaEncadeada* lista, const char* nome);
/// @brief Efetua a busca binária do item na lista, usando o nome.
/// @param Ponteiro para a lista encadeada previamente ordenada. Somente leitura.
/// @param Nome a ser usado na busca. Somente leitura.
void buscarItemListaBinario(const ListaEncadeada* listaOrdenada, const char* nome);
/// @brief Efetua a listagem dos itens existentes no vetor.
/// @param Ponteiro para o vetor. Somente leitura.
void listarItensVetor(const VetorEstatico* vetor);
/// @brief Efetua listagem dos itens existentes na lista encadeada.
/// @param Ponteiro para a lista encadeada. Somente leitura.
void listarItensLista(const ListaEncadeada* lista);
/// @brief Libera a memória alocada para a lista encadeada e seus nós existentes.
/// @param Ponteiro para a lista encadeada.
void liberarMemoriaLista(ListaEncadeada* lista);

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
	printf(" ================= Nível Aventureiro ================\n");
	printf("======================================================\n");

	bool usarVetor = exibirMenuInicial();

	int opcao = 0;

	VetorEstatico vetor; ListaEncadeada lista;

	if (usarVetor)
	{
		vetor = inicializarVetor();
	}
	else
	{
		lista = inicializarLista();
	}

	do
	{
		int quantidade = usarVetor ? contarItensVetor(&vetor) : contarItensLista(&lista);

		exibirMenuPrincipal(&opcao, usarVetor, quantidade);

		switch (opcao)
		{
		case 1:
			// Inserir item.			
			menuInserirItem(usarVetor, &vetor, &lista);
			break;
		case 2:
			// Remoção de item.
			menuRemoverItem(usarVetor, &vetor, &lista);
			break;
		case 3:
			// Listagem de itens.			
			menuListarItens(usarVetor, &vetor, &lista);
			break;
		case 4:
			// Busca de item.
			menuBuscarItem(usarVetor, &vetor, &lista);
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

	if (!usarVetor)
	{
		liberarMemoriaLista(&lista);
	}

	printf("\n==== Operação encerrada. ====\n");

	return EXIT_SUCCESS;
}

// **** Implementação das funções. ****

// **** Funções de Interface de Usuário ****

bool exibirMenuInicial()
{
	printf("\n  ❔  Antes de qualquer ação, é necessário decidir como a");
	printf("\nperformance de execução da missão será impactada, usando:\n");
	printf("\n1 - Um Vetor.\n");
	printf("2 - Uma Lista Encadeada.\n");

	int escolha = 0, invalido = 0;

	do
	{
		printf("Escolha uma opção: ");
		invalido = (scanf("%d", &escolha) != 1) || (escolha != 1 && escolha != 2);
		limparBufferEntrada();
		if (invalido)
		{
			printf("\n==== ⚠️  Entrada inválida. Escolha entre 1 ou 2. ====\n");
		}
	} while (invalido);

	return escolha == 1;
}

void exibirMenuPrincipal(int* opcao, bool usarVetor, int qtdeItensMochila)
{
	printf("\n Itens na Mochila: %d/%d (%s)\n", qtdeItensMochila, TAM_MAX_ITEMS, 
		usarVetor ? "Vetor em uso" : "Lista Encadeada em uso");
	printf("\n1 - Adicionar Item (Loot). \n");
	printf("2 - Remover Item pelo nome. \n");
	printf("3 - Listar todos os Itens. \n");
	printf("4 - Buscar Item pelo nome. \n");
	printf("0 - Sair. \n");

	int invalido = 0;

	do
	{
		printf("Escolha uma opção: ");
		invalido = (scanf("%d", opcao) != 1); // Sem o operador '&'. Já temos um ponteiro.
		limparBufferEntrada();
		if (invalido)
		{
			printf("\n==== ⚠️  Entrada inválida. Tente novamente. ====\n");
		}
	} while (invalido);
}

void menuInserirItem(bool usarVetor, VetorEstatico* vetor, ListaEncadeada* lista)
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


	if (usarVetor)
	{
		inserirItemVetor(vetor, item);
	}
	else
	{
		inserirItemLista(lista, item);
	}
}

void menuRemoverItem(bool usarVetor, VetorEstatico* vetor, ListaEncadeada* lista)
{
	printf("\n==== Remover Item ====\n");

	printf("\nDigite o Nome: ");
	char nome[TAM_NOME];
	fgets(nome, TAM_NOME, stdin);
	limparEnter(nome);

	if (usarVetor)
	{
		removerItemVetor(vetor, nome);
	}
	else
	{
		removerItemLista(lista, nome);
	}
}

void menuListarItens(bool usarVetor, const VetorEstatico* vetor, const ListaEncadeada* lista)
{
	printf("\n==== Listar Itens ====\n");

	if (usarVetor)
	{
		listarItensVetor(vetor);
	}
	else
	{
		listarItensLista(lista);
	}
}

void menuBuscarItem(bool usarVetor, VetorEstatico* vetor, ListaEncadeada* lista)
{
	printf("\n==== Buscar Item ====\n");

	bool invalido;
	char tipoDeBusca;

	do
	{
		printf("\nBusca Sequencial ou Binária? (s/b):");
		tipoDeBusca = getchar();
		limparBufferEntrada();
		invalido = tipoDeBusca != 's' && tipoDeBusca != 'S' && tipoDeBusca != 'b' && tipoDeBusca != 'B';
		if (invalido)
		{
			printf("\n==== ⚠️  Entrada inválida. Esperado: s ou b. ====\n");
		}
	} while (invalido);

	bool usarSequencial = tipoDeBusca == 's' || tipoDeBusca == 'S';

	printf("\nDigite o Nome: ");
	char nome[TAM_NOME];
	fgets(nome, TAM_NOME, stdin);
	limparEnter(nome);

	if (usarVetor)
	{
		if (usarSequencial)
		{
			buscarItemVetorSequencial(vetor, nome);
		}
		else
		{
			printf("\n ℹ️  Ordenando o vetor por nome do item, usando bubbleSort por padrão, para o nível de desafio atual.\n");
			bubbleSortNomeVetor(vetor);
			buscarItemVetorBinario(vetor, nome);
		}
	}
	else
	{
		if (usarSequencial)
		{
			buscarItemListaSequencial(lista, nome);
		}
		else
		{
			printf("\n ℹ️  Ordenando os nós da lista por nome do item, usando bubbleSort por padrão, para o nível de desafio atual.\n");
			bubbleSortNomeLista(lista);
			buscarItemListaBinario(lista, nome);
		}
	}
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

ListaEncadeada inicializarLista()
{
	ListaEncadeada lista = NULL; // // Valor padrão de inicialização.	
	return lista;
}

int contarItensVetor(const VetorEstatico* vetor)
{
	return vetor->quantidade;
}

int contarItensLista(const ListaEncadeada* lista)
{
	int contador = 0;
	No* atual = *lista; // Desreferenciando para recuperar o primeiro nó.

	while (atual != NULL)
	{
		contador++; // Adicionando ao contador enquanto nós validos forem recuperados.
		atual = atual->proximo;
	}

	return contador;
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

void inserirItemLista(ListaEncadeada* lista, Item item)
{
	int quantidade = contarItensLista(lista);

	if (quantidade == TAM_MAX_ITEMS)
	{
		printf("\nAviso ⚠️  O número de itens da lista atingiu o limite.\n");
		return;
	}

	No* novoNo = (No*)malloc(sizeof(No));

	if (novoNo == NULL)
	{
		printf(" ❌  Erro ao alocar memória para novo item de Lista Encadeada.\n");
		return;
	}

	novoNo->dados = item;
	novoNo->proximo = NULL;

	if (*lista == NULL)
	{
		// Lista vazia — o novo nó será o primeiro.
		*lista = novoNo;
	}
	else
	{
		No* atual = *lista;
		// Percorre até o último nó.
		while (atual->proximo != NULL)
		{
			atual = atual->proximo;
		}
		// Anexa o novo nó ao final da lista.
		atual->proximo = novoNo;
	}

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
				// Não temos ponteiros na struct de Item. Podemos passar o item para cópia completa.
				// O acesso não precisa ser específico(membro a membro) nesse cenário.
				vetor->dados[j] = vetor->dados[j + 1];
			}

			vetor->quantidade--; // Atualizando a quantidade de itens.

			printf("\n ✅  Item Nome \"%s\" removido com sucesso.\n", nome);
			return;
		}
	}

	printf("\n ⚠️  Item Nome \"%s\" não encontrado no vetor.\n", nome);
}

void removerItemLista(ListaEncadeada* lista, const char* nome)
{
	No* atual = *lista; // Desreferenciando para o primeiro nó da lista.
	No* anterior = NULL;

	while (atual != NULL)
	{
		if (strcmp(atual->dados.nome, nome) == 0)
		{
			// Efetuando o novo apontamento entre os nós, evitando uma lacuna, deixada pelo nó a ser removido.
			if (anterior == NULL)
			{
				// Encontrado logo no início da lista.
				*lista = atual->proximo;
			}
			else
			{
				// Encontrado de acordo com o deslocamento efetuado.
				anterior->proximo = atual->proximo;
			}

			free(atual); // Liberando a memória alocada para o nó em questão.
			printf("\n ✅  Item Nome \"%s\" removido com sucesso.\n", nome);

			return;
		}

		anterior = atual;
		atual = atual->proximo;
	}
	// Item não encontrado.
	printf("\n ⚠️  Item Nome \"%s\" não encontrado na lista.\n", nome);
}

void trocarItemVetor(Item* a, Item* b) 
{
	Item temp = *a;
	*a = *b;
	*b = temp;
}

void bubbleSortNomeVetor(VetorEstatico* vetor)
{
	int tamanho = vetor->quantidade;

	for (int i = 0; i < tamanho - 1; i++)
	{
		for (int j = 0; j < tamanho - 1 - i; j++)
		{
			if (strcmp(vetor->dados[j].nome, vetor->dados[j + 1].nome) > 0)
			{
				trocarItemVetor(&vetor->dados[j], &vetor->dados[j + 1]);
			}
		}
	}
}

No* trocarNoLista(No* atual)
{
	No* proximo = atual->proximo;
	atual->proximo = proximo->proximo;
	proximo->proximo = atual;
	return proximo;
}

void bubbleSortNomeLista(ListaEncadeada* lista)
{
	if (lista == NULL || *lista == NULL) return;

	int tamanho = contarItensLista(lista);
	// Vamos usar a contagem de nós apenas para poder iterar o número correto de vezes.
	// Por não se tratar de um vetor em si, não há necessidade de uso de índices(nem baseados em zero).
	for (int i = 1; i <= tamanho; i++)
	{
		No* anterior = NULL; // Mantendo o cache do nó anterior, para troca e navegação.
		No* atual = *lista; // Desreferenciando para o primeiro nó da lista.
		bool trocou = false;

		for (int j = 1; j <= tamanho - i; j++)
		{
			if (atual == NULL || atual->proximo == NULL)
			{
				break;
			}
			// Se encontrarmos a correspondência, efetuaremos a troca de nós.
			if (strcmp(atual->dados.nome, atual->proximo->dados.nome) > 0)
			{
				// Neste ponto, o nó atual será atribuido como o próximo, aonde faríamos atual = atual->proximo.
				No* proximo = trocarNoLista(atual);
				trocou = true;
				// Se não há anterior, estamos no início da lista. Para a troca, vamos atribuir o primeiro nó.
				if (anterior == NULL)
				{
					*lista = proximo;
				}
				else
				{
					// O nó anterior ainda aponta para o atual. Com a troca, deverá apontar para o próximo.
					anterior->proximo = proximo;
				}
				// Atribui anterior(usado na navegação) para o novo nó, que agora está no lugar do atual,
				// aonde faríamos anterior = atual.
				anterior = proximo;
			}
			else
			{
				// Sem troca, apenas avança os ponteiros para continuar a navegação.
				anterior = atual;
				atual = atual->proximo;
			}
		}
		// Lista já ordenada — saindo mais cedo, para evitar iterações improdutivas.
		if (!trocou)
		{
			break;
		}
	}
}

void buscarItemVetorSequencial(const VetorEstatico* vetor, const char* nome)
{
	// Busca sequencial.
	int contadorComparacoes = 0;
	for (int i = 0; i < vetor->quantidade; i++)
	{
		// Vamos efetuar a contagem de eventos de comparação via busca sequencial.
		contadorComparacoes++;

		if (strcmp(vetor->dados[i].nome, nome) == 0)
		{
			// Recupera o endereço do item, se encontrado. E exibe os resultados.
			printf("\n========================= Resultados da Busca ==============================");
			exibirCabecalhos();
			exibirItem(&vetor->dados[i]);
			printf("\n============================================================================\n");
			printf("\n ℹ️  Item encontrado via busca sequencial. Comparações efetuadas: %d.\n", contadorComparacoes);
			return;
		}
	}
	// Fim do vetor. Item não encontrado.
	printf("\n ⚠️  Item Nome \"%s\" não encontrado no vetor. \n", nome);
	printf("\n ℹ️  Item não encontrado via busca sequencial. Comparações efetuadas: %d.\n", contadorComparacoes);
}

void buscarItemVetorBinario(const VetorEstatico* vetorOrdenado, const char* nome)
{
	int inicio = 0, fim = vetorOrdenado->quantidade - 1, contadorComparacoes = 0;

	while (inicio <= fim)
	{
		int meio = (inicio + fim) / 2;
		int cmp = strcmp(vetorOrdenado->dados[meio].nome, nome);
		// Vamos efetuar a contagem de eventos de comparação via busca binária.
		contadorComparacoes++;

		if (cmp == 0)
		{
			// Recupera o endereço do item, se encontrado. E exibe os resultados.
			printf("\n========================= Resultados da Busca ==============================");
			exibirCabecalhos();
			exibirItem(&vetorOrdenado->dados[meio]);
			printf("\n============================================================================\n");
			printf("\n ℹ️  Item encontrado via busca binária. Comparações efetuadas: %d.\n", contadorComparacoes);
			return;
		}
		else if (cmp < 0)
		{
			inicio = meio + 1;
		}
		else
		{
			fim = meio - 1;
		}
	}
	// valor não encontrado.
	printf("\n ⚠️  Item Nome \"%s\" não encontrado no vetor. \n", nome);
	printf("\n ℹ️  Item não encontrado via busca binária. Comparações efetuadas: %d.\n", contadorComparacoes);
}

void buscarItemListaSequencial(const ListaEncadeada* lista, const char* nome)
{
	No* atual = *lista;	// Desreferenciando para o primeiro item da lista encadeada.

	int contadorComparacoes = 0;

	while (atual != NULL)
	{
		// Vamos efetuar a contagem de eventos de comparação via busca sequencial.
		contadorComparacoes++;

		if (strcmp(atual->dados.nome, nome) == 0) {
			// Recupera o endereço do item, se encontrado. E exibe os resultados.
			printf("\n========================= Resultados da Busca ==============================");
			exibirCabecalhos();
			exibirItem(&atual->dados);
			printf("\n============================================================================\n");
			printf("\n ℹ️  Item encontrado via busca sequencial. Comparações efetuadas: %d.\n", contadorComparacoes);
			return;
		}
		// Vamos navegando através da lista, procurando itens correspondentes.
		atual = atual->proximo;
	}
	// Fim da lista. Item não encontrado.
	printf("\n ⚠️  Item Nome \"%s\" não encontrado na lista. \n", nome);
	printf("\n ℹ️  Item não encontrado via busca sequencial. Comparações efetuadas: %d.\n", contadorComparacoes);
}

void buscarItemListaBinario(const ListaEncadeada* listaOrdenada, const char* nome)
{
	int tamanhoLista = contarItensLista(listaOrdenada);

	int inicio = 0, fim = tamanhoLista - 1, contadorComparacoes = 0;

	while (inicio <= fim)
	{
		int meio = (inicio + fim) / 2;

		No* atual = *listaOrdenada;
		int posicao = 0;

		while (atual->proximo != NULL)
		{
			atual = atual->proximo;
			posicao++;
			if (posicao == meio)
			{
				break;
			}
		}

		int cmp = strcmp(atual->dados.nome, nome);
		// Vamos efetuar a contagem de eventos de comparação via busca binária.
		contadorComparacoes++;

		if (cmp == 0)
		{
			// Recupera o endereço do item, se encontrado. E exibe os resultados.
			printf("\n========================= Resultados da Busca ==============================");
			exibirCabecalhos();
			exibirItem(&atual->dados);
			printf("\n============================================================================\n");
			printf("\n ℹ️  Item encontrado via busca binária. Comparações efetuadas: %d.\n", contadorComparacoes);
			return;
		}
		else if (cmp < 0)
		{
			inicio = meio + 1;
		}
		else
		{
			fim = meio - 1;
		}
	}
	// valor não encontrado.
	printf("\n ⚠️  Item Nome \"%s\" não encontrado na lista. \n", nome);
	printf("\n ℹ️  Item não encontrado via busca binária. Comparações efetuadas: %d.\n", contadorComparacoes);
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

void listarItensLista(const ListaEncadeada* lista)
{
	No* atual = *lista; // Desreferenciando para recuperar o primeiro nó da lista.

	if (atual == NULL)
	{
		printf("\n ⚠️  A lista encadeada está vazia! \n");
		return;
	}

	exibirCabecalhos();

	// Exibindo os itens, um de cada vez.
	while (atual != NULL)
	{
		exibirItem(&atual->dados);
		atual = atual->proximo;
	}

	printf("\n============================================================================\n");
}

void liberarMemoriaLista(ListaEncadeada* lista)
{
	No* atual = *lista; // Desreferenciando para recuperar o primeiro nó da lista.

	// Liberando a memória alocada para os nós, um de cada vez.
	while (atual != NULL)
	{
		No* temp = atual;
		atual = atual->proximo;
		free(temp);
	}

	*lista = NULL; // Terminando a limpeza.

	printf("\n ✅  Memória alocada para a Lista Encadeada Liberada com sucesso.\n");
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