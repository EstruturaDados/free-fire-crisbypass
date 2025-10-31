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
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.


// Definições de constantes.

/// @brief Tamanho reservado para o nome.
#define TAM_NOME 30
/// @brief Tamanho reservado para o tipo.
#define TAM_TIPO 20
/// @brief Tamanho reservado para a quantidade. Uso exclusivo em cabeçalhos.
#define TAM_QUANTIDADE 10
/// @brief Tamanho reservado para a prioridade. Uso exclusivo em cabeçalhos.
#define TAM_PRIORIDADE 10
/// @brief Tamanho reservado para o máximo de componentes.
#define TAM_MAX_COMPONENTES 20
/// @brief Tamanho reservado para o mínimo de prioridade.
#define TAM_MIN_PRIORIDADE 1
/// @brief Tamanho reservado para o máximo de prioridade.
#define TAM_MAX_PRIORIDADE 5

/// @brief Define uma struct chamada componente, com os campos char nome[30],
/// char tipo[20], int quantidade e int prioridade.
typedef struct {
	char nome[TAM_NOME];
	char tipo[TAM_TIPO];
	int quantidade;
	int prioridade;
} Componente;

/// @brief Define uma struct para conter um vetor estático com capacidade para até TAM_MAX_COMPONENTES.
typedef struct {
	Componente dados[TAM_MAX_COMPONENTES];
	int quantidade;
} VetorEstatico;

/// @brief Define uma struct com os campos Componente dados e struct No* proximo, para a lista encadeada.
struct No {
	Componente dados;
	struct No* proximo;
};

/// @brief Define a struct com um alias No, para uso com a lista encadeada.
typedef struct No No;

/// @brief Define o apontamento(e o alias) para a struct No* proximo, representando uma lista encadeada.
typedef No* ListaEncadeada;

/// @brief Define uma enumeração, para especificação da ordenação dos componentes por: Nome, Tipo, Prioridade.
typedef enum
{
	OrdenarMochilaPor_NENHUM = 0,
	OrdenarMochilaPor_NOME = 1,
	OrdenarMochilaPor_TIPO = 2,
	OrdenarMochilaPor_PRIORIDADE = 3
} OrdenarMochilaPor;

#pragma region MyRegion
/*
int buscaSequencial(int vetor[], int tamanho, int valor) {
	for (int i = 0; i < tamanho; i++) {
		if (vetor[i] == valor) {
			return i;
		}
	}
	return -1;
}

int buscaBinaria(int vetor[], int tamanho, int valor) {
	int inicio = 0, fim = tamanho - 1;

	while (inicio <= fim) {
		int meio = (inicio + fim) / 2;

		if (vetor[meio] == valor)
			return meio;
		else if (vetor[meio] < valor)
			inicio = meio + 1;
		else
			fim = meio - 1;
	}

	return -1; // valor não encontrado
}

int buscaBinariaRecursiva(int vetor[], int inicio, int fim, int valor) {
	if (inicio > fim)
		return -1;

	int meio = (inicio + fim) / 2;

	if (vetor[meio] == valor)
		return meio;
	else if (vetor[meio] < valor)
		return buscaBinariaRecursiva(vetor, meio + 1, fim, valor);
	else
		return buscaBinariaRecursiva(vetor, inicio, meio - 1, valor);
}

void bubbleSort(int vetor[], int tamanho) {
	for (int i = 0; i < tamanho - 1; i++) {
		for (int j = 0; j < tamanho - 1 - i; j++) {
			if (vetor[j] > vetor[j + 1]) {
				trocar(&vetor[j], &vetor[j + 1]);
			}
		}
	}
}

void insertionSort(int vetor[], int tamanho) {
	for (int i = 1; i < tamanho; i++) {
		int chave = vetor[i];
		int j = i - 1;

		// Move os elementos maiores que a chave uma posição à frente
		while (j >= 0 && vetor[j] > chave) {
			vetor[j + 1] = vetor[j];
			j--;
		}

		vetor[j + 1] = chave;
	}
}

void trocar(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

void selectionSort(int vetor[], int tamanho) {
	for (int i = 0; i < tamanho - 1; i++) {
		int indiceMenor = i;
		for (int j = i + 1; j < tamanho; j++) {
			if (vetor[j] < vetor[indiceMenor]) {
				indiceMenor = j;
			}
		}
		if (indiceMenor != i) {
			trocar(&vetor[i], &vetor[indiceMenor]);
		}
	}
}

void doFor() {
	// Código a ser medido (exemplo: laço com cálculos)
	for (long i = 0; i < 1000000000; i++) {
		// Apenas para gastar tempo
	}
}

void medirTempo(void* funcao(void)) {
	clock_t inicio, fim;
	double tempo_gasto;

	// Início da medição
	inicio = clock();

	// Código a ser medido (exemplo: laço com cálculos)
	//for (long i = 0; i < 1000000000; i++) {
	//    // Apenas para gastar tempo
	//}
	funcao();

	// Fim da medição
	fim = clock();

	// Calcular tempo gasto em segundos.
	tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

	printf(" ⏳  Tempo gasto: %.3f segundos.\n", tempo_gasto);
}

void medirTempo(void (*funcao)(int, double), int a, double b) {
	clock_t inicio, fim;
	double tempo_gasto;

	// Início da medição
	inicio = clock();

	// Código a ser medido (exemplo: laço com cálculos)
	//for (long i = 0; i < 1000000000; i++) {
	//    // Apenas para gastar tempo
	//}
	funcao(a, b);

	// Fim da medição
	fim = clock();

	// Calcular tempo gasto em segundos.
	tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

	printf(" ⏳  Tempo gasto: %.3f segundos.\n", tempo_gasto);
}
*/
#pragma endregion

// **** Declarações das funções. ****

// **** Funções de Interface de Usuário ****

/// @brief Exibe o menu inicial, com instruções sobre o tipo de estrutura a ser usada.
/// @returns Verdadeiro(true), se um vetor será usado. 
/// Caso contrário Falso(false). Ou seja, uma lista encadeada. 
bool exibirMenuInicial();
/// @brief Exibe o menu principal.
/// @param Ponteiro via referência para a opção.
/// @param Flag indicando se a estrutura a ser usada é um vetor ou lista encadeada.
/// @param Quantidade de itens existentes atualmente na mochila.
/// @param Valor da enumeração, representando o tipo de ordenação atual. Somente leitura.
void exibirMenuPrincipal(int* opcao, bool usarVetor, int qtdeComponentesMochila, const OrdenarMochilaPor enumOrdem);
/// @brief Exibe o menu de ordenação de componentes.
/// @param Flag indicando se a estrutura a ser usada é um vetor ou lista encadeada.
/// @param Ponteiro via referência para o vetor.
/// @param Ponteiro via referência para a lista encadeada.
/// @param Ponteiro(referência) para a enumeração, a conter o tipo de ordenação atual.
void menuOrdenacaoComponentes(bool usarVetor, VetorEstatico* vetor, ListaEncadeada* lista, OrdenarMochilaPor* ordem);
/// @brief Exibe o menu de inserção de componentes.
/// @param Flag indicando se a estrutura a ser usada é um vetor ou lista encadeada.
/// @param Ponteiro via referência para o vetor.
/// @param Ponteiro via referência para a lista encadeada.
void menuInserirComponente(bool usarVetor, VetorEstatico* vetor, ListaEncadeada* lista);
/// @brief Exibe o menu de remoção de componentes.
/// @param Flag indicando se a estrutura a ser usada é um vetor ou lista encadeada.
/// @param Ponteiro via referência para o vetor.
/// @param Ponteiro via referência para a lista encadeada.
void menuRemoverComponente(bool usarVetor, VetorEstatico* vetor, ListaEncadeada* lista);
/// @brief Exibe o menu de listagem de componentes.
/// @param Flag indicando se a estrutura a ser usada é um vetor ou lista encadeada.
/// @param Ponteiro via referência para o vetor. Somente leitura.
/// @param Ponteiro via referência para a lista encadeada. Somente leitura.
/// @param Quantidade atual de componentes existentes.
void menuListarComponentes(bool usarVetor, const VetorEstatico* vetor, const ListaEncadeada* lista, int quantidade);
/// @brief Exibe o menu de busca de componentes.
/// @param Flag indicando se a estrutura a ser usada é um vetor ou lista encadeada.
/// @param Ponteiro via referência para o vetor.
/// @param Ponteiro via referência para a lista encadeada.
/// @param Valor da enumeração, representando o tipo de ordenação atual. Somente leitura.
void menuBuscarComponente(bool usarVetor, VetorEstatico* vetor, ListaEncadeada* lista, const OrdenarMochilaPor ordem);
/// @brief Exibe os cabeçalhos
/// para os dados tabulados.
void exibirCabecalhos();
/// @brief Exibe um componente para informações tabuladas.
/// @param Componente a ser exibido. Somente Leitura.
void exibirComponente(const Componente* cpte);

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
int contarComponentesVetor(const VetorEstatico* vetor);
/// @brief Entrega o número de itens atualmente existentes na lista encadeada.
/// @param Ponteiro para a lista. Somente leitura.
/// @returns Número conforme especificação.
int contarComponentesLista(const ListaEncadeada* lista);
/// @brief Insere um novo componente no vetor.
/// @param Ponteiro via referência para o vetor.
/// @param Componente a ser inserido.
void inserirComponenteVetor(VetorEstatico* vetor, Componente cpte);
/// @brief Insere um novo componente na lista encadeada.
/// @param Ponteiro via referência para a lista encadeada.
/// @param Componente a ser inserido.
void inserirComponenteLista(ListaEncadeada* lista, Componente cpte);
/// @brief Remove um componente existente no vetor.
/// @param Ponteiro via referência para o vetor.
/// @param Nome do componente a ser usado na remoção. Somente leitura.
void removerComponenteVetor(VetorEstatico* vetor, const char* nome);
/// @brief Remove um componente existente na lista encadeada.
/// @param Ponteiro via referência para a lista encadeada.
/// @param Nome do componente a ser usado na remoção. Somente leitura.
void removerComponenteLista(ListaEncadeada* lista, const char* nome);
/// @brief Efetua uma troca dos ponteiros.
/// @param Ponteiro via referência do componente, para o primeiro operando.
/// @param Ponteiro via referência do componente, para o segundo operando.
void trocarComponenteVetor(Componente* a, Componente* b);
/// @brief Efetua a ordenação dos componentes do vetor da estrutura, via estratégia de bolha,
/// usando o nome do componente.
/// @param Ponteiro via referência para o vetor.
void bubbleSortNomeVetor(VetorEstatico* vetor);
/// @brief Efetua a ordenação dos componentes do vetor da estrutura, via estratégia de inserção,
/// usando o tipo do componente.
/// @param Ponteiro via referência para o vetor.
void insertionSortTipoVetor(VetorEstatico* vetor);
/// @brief Efetua a ordenação dos componentes do vetor da estrutura, via estratégia de seleção,
/// usando a prioridade do componente.
/// @param Ponteiro via referência para o vetor.
void selectionSortPrioridadeVetor(VetorEstatico* vetor);
/// @brief Efetua uma troca dos ponteiros do nó atual e de sua referência ao próximo.
/// @param Ponteiro via referência do nó atual.
/// @returns O nó referenciando o próximo, com conteúdo e endereçamento do atual
/// que também via referência, recebeu o conteúdo e endereçamento do próximo.
No* trocarNoLista(No* atual);
/// @brief Efetua a ordenação dos itens da lista encadeada, via estratégia de bolha,
/// usando o nome do componente.
/// @param Ponteiro via referência para a lista encadeada.
void bubbleSortNomeLista(ListaEncadeada* lista);
/// @brief Efetua a ordenação dos itens da lista encadeada, via estratégia de inserção,
/// usando o tipo do componente.
/// @param Ponteiro via referência para a lista encadeada.
void insertionSortTipoLista(ListaEncadeada* lista);
/// @brief Efetua a ordenação dos itens da lista encadeada, via estratégia de seleção,
/// usando a prioridade do componente.
/// @param Ponteiro via referência para a lista encadeada.
void selectionSortPrioridadeLista(ListaEncadeada* lista);
/// @brief Efetua a busca sequencial do componente no vetor, usando o nome.
/// @param Ponteiro para o vetor. Somente leitura.
/// @param Nome a ser usado na busca. Somente leitura.
void buscarComponenteVetorSequencial(const VetorEstatico* vetor, const char* nome);
/// @brief Efetua a busca binária do componente no vetor, usando o nome.
/// @param Ponteiro para o vetor previamente ordenado. Somente leitura.
/// @param Nome a ser usado na busca. Somente leitura.
void buscarComponenteVetorBinario(const VetorEstatico* vetorOrdenado, const char* nome);
/// @brief Efetua a busca sequencial do componente na lista, usando o nome.
/// @param Ponteiro para a lista encadeada. Somente leitura.
/// @param Nome a ser usado na busca. Somente leitura.
void buscarComponenteListaSequencial(const ListaEncadeada* lista, const char* nome);
/// @brief Efetua a busca binária do componente na lista, usando o nome.
/// @param Ponteiro para a lista encadeada previamente ordenada. Somente leitura.
/// @param Nome a ser usado na busca. Somente leitura.
void buscarComponenteListaBinario(const ListaEncadeada* listaOrdenada, const char* nome);
/// @brief Efetua a listagem dos componentes existentes no vetor.
/// @param Ponteiro para o vetor. Somente leitura.
void listarComponentesVetor(const VetorEstatico* vetor);
/// @brief Efetua listagem dos componentes existentes na lista encadeada.
/// @param Ponteiro para a lista encadeada. Somente leitura.
void listarComponentesLista(const ListaEncadeada* lista);
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
/// @brief Efetua a medição de desempenho, relativa ao tempo corrido usado para a busca no vetor.
/// @param Ponteiro para o método a ser invocado, com assinatura contendo argumentos de somente leitura.
/// @param Ponteiro para o vetor. Somente leitura.
/// @param Nome do componente a ser usado como parâmetro. Somente leitura.
void medirTempoBuscaVetor(void funcao(const VetorEstatico*, const char*),
	const VetorEstatico* vetor, const char* nome);
/// @brief Efetua a medição de desempenho, relativa ao tempo corrido usado para a busca na lista encadeada.
/// @param Ponteiro para o método a ser invocado, com assinatura contendo argumentos de somente leitura.
/// @param Ponteiro para a lista encadeada. Somente leitura.
/// @param Nome do componente a ser usado como parâmetro. Somente leitura.
void medirTempoBuscaLista(void funcao(const ListaEncadeada*, const char*),
	const ListaEncadeada* lista, const char* nome);
/// @brief Efetua a medição de desempenho, relativa ao tempo corrido usado para a ordenação do vetor.
/// @param Ponteiro para o método a ser invocado, com assinatura contendo argumentos via referência.
/// @param Ponteiro via referência para o vetor.
void medirTempoOrdenacaoVetor(void funcao(VetorEstatico*), VetorEstatico* vetor);
/// @brief Efetua a medição de desempenho, relativa ao tempo corrido usado para a ordenação da lista encadeada.
/// @param Ponteiro para o método a ser invocado, com assinatura contendo argumentos via referência.
/// @param Ponteiro via referência para a lista encadeada.
void medirTempoOrdenacaoLista(void funcao(ListaEncadeada*), ListaEncadeada* lista);

/// @brief Ponto de entrada do programa.
/// @returns Inteiro. Zero, em caso de sucesso. Ex: EXIT_SUCCESS. 
/// Ou diferente de Zero, em caso de falha. Ex: EXIT_FAILURE.
int main() {
	// Menu principal com opções:
	// 1. Adicionar um item
	// 2. Remover um item
	// 3. Listar todos os itens
	// 4. Ordenar os itens por critério (nome, tipo, prioridade)
	// 5. Realizar busca binária por nome
	// 0. Sair
	// A estrutura switch trata cada opção chamando a função correspondente.
	// A ordenação e busca binária exigem que os dados estejam bem organizados.

	// Configurações de caracteres para ajuste de ortografia para saída do console(UTF-8).
#ifdef _WIN32
	// Plataforma Windows.
	SetConsoleOutputCP(CP_UTF8);
#endif

	printf("======================================================\n");
	printf(" ==== 🏝️  Código da Ilha – Edição Free Fire  🏝️ ====\n");
	printf(" =================== Nível Mestre ===================\n");
	printf("======================================================\n");

	bool usarVetor = exibirMenuInicial();

	int opcao = 0;

	VetorEstatico vetor; ListaEncadeada lista;

	OrdenarMochilaPor ordem = OrdenarMochilaPor_NENHUM;

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
		int quantidade = usarVetor ? contarComponentesVetor(&vetor) : contarComponentesLista(&lista);

		exibirMenuPrincipal(&opcao, usarVetor, quantidade, ordem);

		switch (opcao)
		{
		case 1:
			// Inserir componente.			
			menuInserirComponente(usarVetor, &vetor, &lista);
			break;
		case 2:
			// Remoção de componente.
			menuRemoverComponente(usarVetor, &vetor, &lista);
			break;
		case 3:
			// Listagem de componentes.			
			menuListarComponentes(usarVetor, &vetor, &lista, quantidade);
			break;
		case 4:
			// Busca de componente.
			menuOrdenacaoComponentes(usarVetor, &vetor, &lista, &ordem);
			break;
		case 5:
			// Busca de componente.
			menuBuscarComponente(usarVetor, &vetor, &lista, ordem);
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

void exibirMenuPrincipal(int* opcao, bool usarVetor, int qtdeComponentesMochila, const OrdenarMochilaPor ordem) {

	bool ordenadoNome = ordem == OrdenarMochilaPor_NOME;

	printf("\n Componentes na Mochila: %d/%d (%s)\n", qtdeComponentesMochila, TAM_MAX_COMPONENTES,
		usarVetor ? "Vetor em uso" : "Lista Encadeada em uso");
	printf("\n Status da Ordenação por Nome: %s  \n", ordenadoNome ? "Ordenado" : "Não Ordenado");
	printf("\n1 - Adicionar Componente.\n");
	printf("2 - Descartar Componente.\n");
	printf("3 - Listar Componentes (Inventário).\n");
	printf("4 - Organizar Mochila (Ordenar Componentes).\n");
	printf("5 - Busca Binária(ou Sequencial) por Componente-Chave (por Nome).\n");
	printf("0 - Ativar Torre de Fuga (Sair). \n");

	int invalido = 0;

	do
	{
		printf("Escolha uma opção: ");
		invalido = scanf("%d", opcao) != 1;
		limparBufferEntrada();
		if (invalido)
		{
			printf("\n==== ⚠️  Entrada inválida. Tente novamente. ====\n");
		}
	} while (invalido);
}

void menuOrdenacaoComponentes(bool usarVetor, VetorEstatico* vetor, ListaEncadeada* lista, OrdenarMochilaPor* ordem)
{
	printf("\n==== Estratégia de Organização ====\n");
	printf("\n==== Como deseja ordenar os componentes? ====\n");
	printf("\n1 - Por Nome (Ordem Alfabética).\n");
	printf("2 - Por Tipo.\n");
	printf("3 - Por Prioridade de Montagem.\n");
	printf("0 - Cancelar. \n");	

	int invalido = 0;
	
	// Já temos um ponteiro para scanf(não precisa do &). Vamos efetuar um cast para int.
	do
	{
		printf("Escolha o critério: ");
		invalido = scanf("%d", (int*)ordem) != 1 || (*ordem != OrdenarMochilaPor_NOME &&
			*ordem != OrdenarMochilaPor_TIPO && *ordem != OrdenarMochilaPor_PRIORIDADE &&
			*ordem != OrdenarMochilaPor_NENHUM);

		limparBufferEntrada();

		if (invalido)
		{
			printf("\n==== ⚠️  Entrada inválida. Escolha entre 0 e 3. \n");
		}

	} while (invalido);

	// Vamos recuperar o valor de ordem diretamente do endereço.
	// E aproveitar para computar o tempo corrido para a execução das tarefas.
	switch (*ordem)
	{
	case OrdenarMochilaPor_NOME:
		if (usarVetor)
		{
			medirTempoOrdenacaoVetor(bubbleSortNomeVetor, vetor);
		}
		else
		{
			medirTempoOrdenacaoLista(bubbleSortNomeLista, lista);
		}
		break;
	case OrdenarMochilaPor_TIPO:
		if (usarVetor)
		{
			medirTempoOrdenacaoVetor(insertionSortTipoVetor, vetor);
		}
		else
		{
			medirTempoOrdenacaoLista(insertionSortTipoLista, lista);
		}
		break;
	case OrdenarMochilaPor_PRIORIDADE:
		if (usarVetor)
		{
			medirTempoOrdenacaoVetor(selectionSortPrioridadeVetor, vetor);
		}
		else
		{
			medirTempoOrdenacaoLista(selectionSortPrioridadeLista, lista);
		}
		break;
	default:
		printf("\n ℹ️  Ordenação cancelada.\n");
		break;
	}
}

void menuInserirComponente(bool usarVetor, VetorEstatico* vetor, ListaEncadeada* lista)
{
	printf("\n==== Coletar Novo Componente ====\n");
	// Tratando as entradas de usuário.
	Componente cpte;

	printf("\nDigite o Nome: ");
	fgets(cpte.nome, TAM_NOME, stdin);
	limparEnter(cpte.nome);

	printf("Digite Tipo: ");
	fgets(cpte.tipo, TAM_TIPO, stdin);
	limparEnter(cpte.tipo);

	int invalido = 0;

	do
	{
		printf("Digite a quantidade: ");
		invalido = scanf("%d", &cpte.quantidade) != 1 || cpte.quantidade < 1;
		limparBufferEntrada();
		if (invalido)
		{
			printf("\n==== ⚠️  Entrada inválida. Um valor acima de zero é esperado. ====\n");
		}
	} while (invalido);

	invalido = 0;

	do
	{
		printf("Digite a prioridade(%d a %d): ", TAM_MIN_PRIORIDADE, TAM_MAX_PRIORIDADE);
		invalido = scanf("%d", &cpte.prioridade) != 1 ||
			!(cpte.prioridade >= TAM_MIN_PRIORIDADE && cpte.prioridade <= TAM_MAX_PRIORIDADE);
		limparBufferEntrada();
		if (invalido)
		{
			printf("\n==== ⚠️  Entrada inválida. Um valor entre %d e %d é esperado. ====\n",
				TAM_MIN_PRIORIDADE, TAM_MAX_PRIORIDADE);
		}
	} while (invalido);


	if (usarVetor)
	{
		inserirComponenteVetor(vetor, cpte);
	}
	else
	{
		inserirComponenteLista(lista, cpte);
	}
}

void menuRemoverComponente(bool usarVetor, VetorEstatico* vetor, ListaEncadeada* lista)
{
	printf("\n==== Remover Componente ====\n");

	printf("\nDigite o Nome: ");
	char nome[TAM_NOME];
	fgets(nome, TAM_NOME, stdin);
	limparEnter(nome);

	if (usarVetor)
	{
		removerComponenteVetor(vetor, nome);
	}
	else
	{
		removerComponenteLista(lista, nome);
	}
}

void menuListarComponentes(bool usarVetor, const VetorEstatico* vetor, const ListaEncadeada* lista, int quantidade)
{
	printf("\n==== Listar Componentes(Inventário Atual) %d/%d ====\n", quantidade, TAM_MAX_COMPONENTES);

	if (usarVetor)
	{
		listarComponentesVetor(vetor);
	}
	else
	{
		listarComponentesLista(lista);
	}
}

void menuBuscarComponente(bool usarVetor, VetorEstatico* vetor, ListaEncadeada* lista, const OrdenarMochilaPor ordem)
{
	printf("\n==== Buscar Componente(Chave) ====\n");

	bool invalido;
	char tipoDeBusca;

	do
	{
		printf("\nBusca Binária ou Sequencial? (b/s):");
		tipoDeBusca = getchar();
		limparBufferEntrada();
		invalido = tipoDeBusca != 's' && tipoDeBusca != 'S' && tipoDeBusca != 'b' && tipoDeBusca != 'B';
		if (invalido)
		{
			printf("\n==== ⚠️  Entrada inválida. Esperado: b ou s. ====\n");
		}
	} while (invalido);

	bool usarSequencial = tipoDeBusca == 's' || tipoDeBusca == 'S';

	bool ordenadoPorNome = ordem == OrdenarMochilaPor_NOME;
	// Se a opção é pela busca binária, a ordenação é mandatória.
	if (!usarSequencial && !ordenadoPorNome)
	{
		printf("\n ⚠️  Para a busca binária, antes os componentes PRECISAM estar ordenados POR NOME.");
		printf("\n ℹ️  Organize a Mochila(Ordenar Componentes).\n");
		return;
	}

	printf("\nDigite o Nome: ");
	char nome[TAM_NOME];
	fgets(nome, TAM_NOME, stdin);
	limparEnter(nome);
		
	// Vamos aproveitar para além de verificar as opções, 
	// computar o tempo corrido para a execução das tarefas em questão.
	if (usarVetor)
	{
		if (usarSequencial)
		{
			medirTempoBuscaVetor(buscarComponenteVetorSequencial, vetor, nome);
		}
		else if (ordenadoPorNome)
		{			
			medirTempoBuscaVetor(buscarComponenteVetorBinario, vetor, nome);
		}		
	}
	else
	{
		if (usarSequencial)
		{
			medirTempoBuscaLista(buscarComponenteListaSequencial, lista, nome);
		}
		else if (ordenadoPorNome)
		{
			medirTempoBuscaLista(buscarComponenteListaBinario, lista, nome);
		}		
	}
}

void exibirCabecalhos()
{
	printf("\n===================================================================================");
	printf("\n%-*s | %-*s | %-*s | %-*s", TAM_NOME, "NOME", TAM_TIPO, "TIPO",
		TAM_QUANTIDADE, "QUANTIDADE", TAM_PRIORIDADE, "PRIORIDADE");
}

void exibirComponente(const Componente* cpte)
{
	// Exibe os resultados reservando espaços e tabulando de acordo com os valores do componente atual.
	printf("\n-----------------------------------------------------------------------------------");
	printf("\n%-*s | %-*s | %-*d | %-*d", TAM_NOME, cpte->nome, TAM_TIPO, cpte->tipo,
		TAM_QUANTIDADE, cpte->quantidade, TAM_PRIORIDADE, cpte->prioridade);
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

int contarComponentesVetor(const VetorEstatico* vetor)
{
	return vetor->quantidade;
}

int contarComponentesLista(const ListaEncadeada* lista)
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

void inserirComponenteVetor(VetorEstatico* vetor, Componente cpte)
{
	if (vetor->quantidade == TAM_MAX_COMPONENTES)
	{
		printf("\nAviso ⚠️  O número de componentes do vetor atingiu o limite.\n");
		return;
	}
	// Atribuindo os valores correspondentes aos membros do componente em questão,
	// de acordo com a posição de navegação atual da lista.
	strcpy(vetor->dados[vetor->quantidade].nome, cpte.nome);
	strcpy(vetor->dados[vetor->quantidade].tipo, cpte.tipo);
	vetor->dados[vetor->quantidade].quantidade = cpte.quantidade;
	vetor->dados[vetor->quantidade].prioridade = cpte.prioridade;

	vetor->quantidade++; // Atualizando a quantidade de componentes da lista e preparando a próxima posição.

	printf("\n✅  Componente de Nome \"%s\" inserido com sucesso.\n", cpte.nome);
}

void inserirComponenteLista(ListaEncadeada* lista, Componente cpte)
{
	int quantidade = contarComponentesLista(lista);

	if (quantidade == TAM_MAX_COMPONENTES)
	{
		printf("\nAviso ⚠️  O número de itens da lista atingiu o limite.\n");
		return;
	}

	No* novoNo = (No*)malloc(sizeof(No));

	if (novoNo == NULL)
	{
		printf(" ❌  Erro ao alocar memória para novo componente de Lista Encadeada.\n");
		return;
	}

	novoNo->dados = cpte;
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

	printf("\n✅  Componente de Nome \"%s\" inserido com sucesso.\n", cpte.nome);
}

void removerComponenteVetor(VetorEstatico* vetor, const char* nome)
{
	for (int i = 0; i < vetor->quantidade; i++)
	{
		if (strcmp(vetor->dados[i].nome, nome) == 0)
		{
			// Reorganizando a lista para a remoção.
			for (int j = i; j < vetor->quantidade - 1; j++)
			{
				// Não temos ponteiros na struct de Componente. Podemos passar o componente para cópia completa.
				// O acesso não precisa ser específico(membro a membro) nesse cenário.
				vetor->dados[j] = vetor->dados[j + 1];
			}

			vetor->quantidade--; // Atualizando a quantidade de componentes.

			printf("\n ✅  Componente Nome \"%s\" removido com sucesso.\n", nome);
			return;
		}
	}

	printf("\n ⚠️  Componente Nome \"%s\" não encontrado no vetor.\n", nome);
}

void removerComponenteLista(ListaEncadeada* lista, const char* nome)
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
			printf("\n ✅  Componente Nome \"%s\" removido com sucesso.\n", nome);

			return;
		}

		anterior = atual;
		atual = atual->proximo;
	}
	// Componente não encontrado.
	printf("\n ⚠️  Componente Nome \"%s\" não encontrado na lista.\n", nome);
}

void trocarComponenteVetor(Componente* a, Componente* b)
{
	Componente temp = *a;
	*a = *b;
	*b = temp;
}

void bubbleSortNomeVetor(VetorEstatico* vetor)
{
	int tamanho = vetor->quantidade;

	int contadorComparacoes = 0;

	for (int i = 0; i < tamanho - 1; i++)
	{
		for (int j = 0; j < tamanho - 1 - i; j++)
		{
			if (strcmp(vetor->dados[j].nome, vetor->dados[j + 1].nome) > 0)
			{
				// Vamos efetuar a contagem de eventos de comparação via ordenação por bolha.
				contadorComparacoes++;

				trocarComponenteVetor(&vetor->dados[j], &vetor->dados[j + 1]);
			}
		}
	}

	printf("\n ℹ️  Componentes da mochila ordenados por NOME(usando BubbleSort).");
	printf("\n ℹ️  Análise de desempenho. Comparações efetuadas: %d.\n", contadorComparacoes);
}

void insertionSortTipoVetor(VetorEstatico* vetor)
{
	int tamanho = vetor->quantidade;

	int contadorComparacoes = 0;

	for (int i = 1; i < tamanho; i++)
	{
		Componente chave = vetor->dados[i];
		int j = i - 1;
		// Move os elementos maiores que a chave uma posição à frente
		while (j >= 0 && strcmp(vetor->dados[j].tipo, chave.tipo) > 0)
		{
			// Vamos efetuar a contagem de eventos de comparação via ordenação por inserção.
			contadorComparacoes++;

			vetor->dados[j + 1] = vetor->dados[j];
			j--;
		}

		vetor->dados[j + 1] = chave;
	}

	printf("\n ℹ️  Componentes da mochila ordenados por TIPO(usando InsertionSort).");
	printf("\n ℹ️  Análise de desempenho => Comparações efetuadas: %d.\n", contadorComparacoes);
}

void selectionSortPrioridadeVetor(VetorEstatico* vetor)
{
	int tamanho = vetor->quantidade;

	int contadorComparacoes = 0;

	for (int i = 0; i < tamanho - 1; i++)
	{
		int indiceMenor = i;

		for (int j = i + 1; j < tamanho; j++)
		{
			if (vetor->dados[j].prioridade < vetor->dados[indiceMenor].prioridade)
			{
				// Vamos efetuar a contagem de eventos de comparação via ordenação por seleção.
				contadorComparacoes++;

				indiceMenor = j;
			}
		}

		if (indiceMenor != i)
		{
			trocarComponenteVetor(&vetor->dados[i], &vetor->dados[indiceMenor]);
		}
	}

	printf("\n ℹ️  Componentes da mochila ordenados por PRIORIDADE(usando SelectionSort).");
	printf("\n ℹ️  Análise de desempenho => Comparações efetuadas: %d.\n", contadorComparacoes);
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
	if (lista == NULL || *lista == NULL)
	{
		return;
	}

	int tamanho = contarComponentesLista(lista);

	int contadorComparacoes = 0;

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
				// Vamos efetuar a contagem de eventos de comparação via ordenação por bolha.
				contadorComparacoes++;
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

	printf("\n ℹ️  Componentes da mochila ordenados por NOME(usando BubbleSort).");
	printf("\n ℹ️  Análise de desempenho => Comparações efetuadas: %d.\n", contadorComparacoes);
}

void insertionSortTipoLista(ListaEncadeada* lista)
{
	if (lista == NULL || *lista == NULL || (*lista)->proximo == NULL)
	{
		return;
	}

	No* listaOrdenada = NULL;

	int contadorComparacoes = 0;

	while (*lista != NULL)
	{
		No* noAtual = *lista;
		*lista = (*lista)->proximo;

		if (listaOrdenada == NULL || strcmp(noAtual->dados.tipo, listaOrdenada->dados.tipo) < 0)
		{
			// Vamos efetuar a contagem de eventos de comparação via ordenação por inserção.
			contadorComparacoes++;

			noAtual->proximo = listaOrdenada;
			listaOrdenada = noAtual;
		}
		else
		{
			No* aux = listaOrdenada;
			while (aux->proximo != NULL && strcmp(aux->proximo->dados.tipo, noAtual->dados.tipo) < 0)
			{
				aux = aux->proximo;
			}
			noAtual->proximo = aux->proximo;
			aux->proximo = noAtual;
		}
	}

	*lista = listaOrdenada;

	printf("\n ℹ️  Componentes da mochila ordenados por TIPO(usando InsertionSort).");
	printf("\n ℹ️  Análise de desempenho => Comparações efetuadas: %d.\n", contadorComparacoes);
}

void selectionSortPrioridadeLista(ListaEncadeada* lista)
{
	if (lista == NULL || *lista == NULL || (*lista)->proximo == NULL)
	{
		return;
	}

	No** ponteiroLista = lista;

	int contadorComparacoes = 0;

	while (*ponteiroLista != NULL)
	{
		No* atual = *ponteiroLista;
		No* menor = atual;
		No* anterior = NULL;

		while (atual->proximo != NULL)
		{
			// Vamos efetuar a contagem de eventos de comparação via ordenação por seleção.
			contadorComparacoes++;

			if (atual->proximo->dados.prioridade < menor->dados.prioridade)
			{
				menor = atual->proximo;
			}
			atual = atual->proximo;
		}

		// Move o menor até o início da sublista usando trocas adjacentes.
		while (*ponteiroLista != menor)
		{
			No* trocado = trocarNoLista(*ponteiroLista);
			*ponteiroLista = trocado;
		}

		ponteiroLista = &((*ponteiroLista)->proximo);
	}

	printf("\n ℹ️  Componentes da mochila ordenados por PRIORIDADE(usando SelectionSort).");
	printf("\n ℹ️  Análise de desempenho => Comparações efetuadas: %d.\n", contadorComparacoes);
}

void buscarComponenteVetorSequencial(const VetorEstatico* vetor, const char* nome)
{	
	int contadorComparacoes = 0;

	for (int i = 0; i < vetor->quantidade; i++)
	{
		// Vamos efetuar a contagem de eventos de comparação via busca sequencial.
		contadorComparacoes++;

		if (strcmp(vetor->dados[i].nome, nome) == 0)
		{
			// Recupera o endereço do componente, se encontrado. E exibe os resultados.
			printf("\n============================= Resultados da Busca =================================");
			exibirCabecalhos();
			exibirComponente(&vetor->dados[i]);
			printf("\n===================================================================================\n");
			printf("\n ℹ️  Componente encontrado via busca sequencial. Comparações efetuadas: %d.\n", contadorComparacoes);
			return;
		}
	}
	// Fim do vetor. Componente não encontrado.
	printf("\n ⚠️  Componente Nome \"%s\" não encontrado no vetor.", nome);
	printf("\n ℹ️  Componente não encontrado via busca sequencial. Comparações efetuadas: %d.\n", contadorComparacoes);
}

void buscarComponenteVetorBinario(const VetorEstatico* vetorOrdenado, const char* nome)
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
			// Recupera o endereço do componente, se encontrado. E exibe os resultados.
			printf("\n============================= Resultados da Busca =================================");
			exibirCabecalhos();
			exibirComponente(&vetorOrdenado->dados[meio]);
			printf("\n===================================================================================\n");
			printf("\n ℹ️  Componente encontrado via busca binária. Comparações efetuadas: %d.\n", contadorComparacoes);
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
	// Valor não encontrado.
	printf("\n ⚠️  Componente Nome \"%s\" não encontrado no vetor.", nome);
	printf("\n ℹ️  Componente não encontrado via busca binária. Comparações efetuadas: %d.\n", contadorComparacoes);
}

void buscarComponenteListaSequencial(const ListaEncadeada* lista, const char* nome)
{
	No* atual = *lista;	// Desreferenciando para o primeiro nó da lista encadeada.

	int contadorComparacoes = 0;

	while (atual != NULL)
	{
		// Vamos efetuar a contagem de eventos de comparação via busca sequencial.
		contadorComparacoes++;

		if (strcmp(atual->dados.nome, nome) == 0) {
			// Recupera o endereço do componente, se encontrado. E exibe os resultados.
			printf("\n============================= Resultados da Busca =================================");
			exibirCabecalhos();
			exibirComponente(&atual->dados);
			printf("\n===================================================================================\n");
			printf("\n ℹ️  Componente encontrado via busca sequencial. Comparações efetuadas: %d.\n", contadorComparacoes);
			return;
		}
		// Vamos navegando através da lista, procurando componentes correspondentes.
		atual = atual->proximo;
	}
	// Fim da lista. Componente não encontrado.
	printf("\n ⚠️  Componente Nome \"%s\" não encontrado na lista.", nome);
	printf("\n ℹ️  Componente não encontrado via busca sequencial. Comparações efetuadas: %d.\n", contadorComparacoes);
}

void buscarComponenteListaBinario(const ListaEncadeada* listaOrdenada, const char* nome)
{
	int tamanhoLista = contarComponentesLista(listaOrdenada);

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
			// Recupera o endereço do componente, se encontrado. E exibe os resultados.
			printf("\n============================= Resultados da Busca =================================");
			exibirCabecalhos();
			exibirComponente(&atual->dados);
			printf("\n===================================================================================\n");
			printf("\n ℹ️  Componente encontrado via busca binária. Comparações efetuadas: %d.\n", contadorComparacoes);
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
	printf("\n ⚠️  Componente Nome \"%s\" não encontrado na lista.", nome);
	printf("\n ℹ️  Componente não encontrado via busca binária. Comparações efetuadas: %d.\n", contadorComparacoes);
}

void listarComponentesVetor(const VetorEstatico* vetor)
{
	if (vetor->quantidade == 0)
	{
		printf("\n ⚠️  O vetor estático está vazio! \n");
		return;
	}

	exibirCabecalhos();

	for (int i = 0; i < vetor->quantidade; i++)
	{
		exibirComponente(&vetor->dados[i]);
	}

	printf("\n===================================================================================\n");
}

void listarComponentesLista(const ListaEncadeada* lista)
{
	No* atual = *lista; // Desreferenciando para recuperar o primeiro nó da lista.

	if (atual == NULL)
	{
		printf("\n ⚠️  A lista encadeada está vazia! \n");
		return;
	}

	exibirCabecalhos();

	// Exibindo os componentes, um de cada vez.
	while (atual != NULL)
	{
		exibirComponente(&atual->dados);
		atual = atual->proximo;
	}

	printf("\n===================================================================================\n");
}

void liberarMemoriaLista(ListaEncadeada* lista)
{
	if (lista == NULL || *lista == NULL)
	{
		return;
	}

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

void medirTempoBuscaVetor(void funcao(const VetorEstatico*, const char*),
	const VetorEstatico* vetor, const char* nome)
{
	clock_t inicio, fim;
	double tempo_gasto;

	// Início da medição
	inicio = clock();

	funcao(vetor, nome); // Chamada principal.

	// Fim da medição
	fim = clock();

	// Calcular tempo gasto em segundos.
	tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

	printf(" ℹ️  Análise de desempenho => ⏳  Tempo gasto: %.3f segundos.\n", tempo_gasto);
}

void medirTempoBuscaLista(void funcao(const ListaEncadeada*, const char*),
	const ListaEncadeada* lista, const char* nome)
{
	clock_t inicio, fim;
	double tempo_gasto;

	// Início da medição
	inicio = clock();

	funcao(lista, nome); // Chamada principal.

	// Fim da medição
	fim = clock();

	// Calcular tempo gasto em segundos.
	tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

	printf(" ℹ️  Análise de desempenho => ⏳  Tempo gasto: %.3f segundos.\n", tempo_gasto);
}

void medirTempoOrdenacaoVetor(void funcao(VetorEstatico*), VetorEstatico* vetor)
{
	clock_t inicio, fim;
	double tempo_gasto;

	// Início da medição
	inicio = clock();

	funcao(vetor); // Chamada principal.

	// Fim da medição
	fim = clock();

	// Calcular tempo gasto em segundos.
	tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

	printf(" ℹ️  Análise de desempenho => ⏳  Tempo gasto: %.3f segundos.\n", tempo_gasto);
}

void medirTempoOrdenacaoLista(void funcao(ListaEncadeada*), ListaEncadeada* lista)
{
	clock_t inicio, fim;
	double tempo_gasto;

	// Início da medição
	inicio = clock();

	funcao(lista); // Chamada principal.

	// Fim da medição
	fim = clock();

	// Calcular tempo gasto em segundos.
	tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

	printf(" ℹ️  Análise de desempenho => ⏳  Tempo gasto: %.3f segundos.\n", tempo_gasto);
}