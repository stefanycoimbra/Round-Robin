// Inclusão das bibliotecas
#include <stdio.h> 
#include <stdlib.h>

// Struct para guardar cada processo com sua criação, duração e prioridade
typedef struct {
    int criacao, duracao, prioridade, tempo_restante;
}processo;

// Vetor de processos
processo *vet;

// Struct para fazer a fila
typedef struct no {
    int elem;
    struct no *prox;
}fila;

fila *fila_prontos = NULL;

// Variáveis:
// duracao_total: guarda duração total do programa
// numero_processos: quantidade máxima de processos
// quantum: pedaço de tempo tomado pelo processo de cada vez
// tempo: tempo total do programa
// ordem_processo: guarda ordem de execução dos processos
// tempo_espera: guarda tempo de espera de cada processo
// final_processo: guarda o instante em que cada processa finaliza sua execução
int duracao_total = 0, numero_processos = 1, quantum = 3;
unsigned int tempo = 0;
int *ordem_processo, *tempo_espera, *final_processo;

/// Função para leitura da entrada
void dados() {
    // Leitura e atribuição dos dados de um arquivo "dados.txt" para um vetor dinamicamente alocado. Nesse arquivo,
    // as três colunas são separadas por um espaço e não pode existir linhas vazias.
    int i,j;
    char car;
    FILE *arq;
    arq = fopen("dados.txt", "r");
    if(arq == NULL)
        printf("Nao foi possivel abrir o arquivo");
    else {
        // Determina quantas linhas existe no arquivo
        while((car = getc(arq)) != EOF)
            if(car == '\n')
                numero_processos++;

        // Reserva espaço para guardar os valores do arquivo
        vet = (processo *) malloc(numero_processos*sizeof(processo));

        // Reserva espaço para guardar os valores do tempo total de cada processo
        // e do tempo de espera individual
        final_processo = (int *) malloc(numero_processos*sizeof(int));
        tempo_espera = (int *) malloc(numero_processos*sizeof(int));

        // Armazena os valores do arquivo no vet[i]
        fseek(arq, 0, SEEK_SET);
        for(i = 0; i < numero_processos; i++) {
            fscanf(arq, "%d %d %d\n", &vet[i].criacao, &vet[i].duracao, &vet[i].prioridade);
            vet[i].tempo_restante = vet[i].duracao; // Armazena a duracao de cada processo
            duracao_total += vet[i].duracao; // Armazena o tempo de todos os processos
        }
        fclose(arq);
    }
}

/// Essa função adiciona um elemento na fila
void adicionar_fila(int i) {
    fila *novo, *atual;
    novo = (fila*) malloc(sizeof(fila));
    // O novo processo é adicionado, apontando para NULL e salvando o índice i de cada processo
    novo->prox = NULL;
    novo->elem = i;
    if(fila_prontos == NULL) // Fila vazia
        fila_prontos = novo;
    else { // Fila com mais de um elemento
        atual = fila_prontos;
        while(atual->prox != NULL) // Chega no último elemento para adicionar o novo
            atual = atual->prox;
        atual->prox = novo;
    }
}

/// Essa função adiciona um elemento na fila exatamente no tempo de sua chegada
void adicionar_fila_chegados() {
    int i;
    // Percorre todos os processos até encontrar um com tempo de chegada 
    // igual ao instante do algoritmo
    for(i = 0; i < numero_processos; i++) {
        if(vet[i].criacao == tempo) // Verifica se existe um processo que começa nesse tem
            adicionar_fila(i);
    }
}

/// Essa função verifica e retorna o próximo processo da fila
int obter_proximo_processo() {
	int processo;
	if(fila_prontos != NULL) { // Se possuir um processo
		processo = fila_prontos->elem;  // processo recebe o elemento da fila
		fila_prontos = fila_prontos->prox; // É puxado o próximo processo da fila
		return processo; // Retorna o ID do processo
	}
    return -1; // Retorna -1 se não há nenhum processo na fila de espera
}

/// Essa função verica se ainda há processos a serem executados
unsigned int resta_execucao() {
	int i;
	unsigned int x = 0;
	for(i = 0; i < numero_processos; i++) // Para cada processo
		if(vet[i].tempo_restante > 0) // Caso haja um processo não finalizado
			x = 1;
	return x; // Retorna 1 se há processos não finalizados e 0 caso seja o fim do algoritmo
}

/// Função para escalonamento dos processos através do algoritmo Round-Robin
void roundrobin() {
    unsigned int i, prox_processo, aux_quantum = 0;

    ordem_processo = (int*) malloc(duracao_total*sizeof(int));

    adicionar_fila_chegados();

    while(resta_execucao()) { // Enquanto há processos em execução

        prox_processo = obter_proximo_processo(); // Obtém o próximo processo
        
        if(prox_processo == -1) { // Se não há processos na fila de espera
            ordem_processo[tempo++] = -1; // Não há próximo processo nesta fila
			adicionar_fila_chegados(); // Adiciona processo à fila de prontos
        }
        else {
			aux_quantum = quantum; // aux_quantum recebe o quantum
            
			if(vet[prox_processo].tempo_restante < aux_quantum)// Se o quantum for maior que o tempo restante
				aux_quantum = vet[prox_processo].tempo_restante; // aux_quantum recebe o tempo restante

			for(i = aux_quantum; i > 0; i--) { // Subtrai do tempo restante o quantum ou, caso o tempo restante seja menor que 
                                               // o quantum, zera a variável do tempo restante
                ordem_processo[tempo++] = prox_processo;
                vet[prox_processo].tempo_restante--;
				adicionar_fila_chegados();
			}
            // Processo ainda não terminado, sendo readicionado na fila
			if(vet[prox_processo].tempo_restante > 0)
				adicionar_fila(prox_processo);
		}
    }
}

/// Função que coloca o diagrama de execução, tempos de vida e de espera individuais e médios
/// em um arquivo saída.txt
void saida() {

    int i, j;
    unsigned t;
    
    // Variáveis para somar os tempos de vida e espera 
    // e para guardar os tempos médio de vida e espera
    int soma_tempo_vida = 0, soma_tempo_espera = 0;
    float media_tempo_vida = 0, media_tempo_espera = 0;
    
    FILE *arq; 
    
    // Calcula o tempo total de cada processo (desde que o programa iniciou) e o tempo de espera associado
    for(i = 0; i < numero_processos; i++) {
        for(t = tempo - 1; ordem_processo[t]!= i; t--); // Acha o instante anterior ao que o processo termina
	    final_processo[i] = t + 1; // Incrementa 1 para progedir ao instante em que processo termina 
        tempo_espera[i] = final_processo[i] - vet[i].criacao - vet[i].duracao; 
    }

    // Criação e atribuição de dados para um arquivo "saida.txt"
    arq = fopen("saida.txt", "w");
    if(arq == NULL)
        printf("Nao foi possivel abrir o arquivo");
    else {
        // Armazena os valores no arquivo de texto e também imprime na tela
        fprintf(arq, "Quantum: %d\n\n", quantum);

        fprintf(arq, "* * * * * * * * * *\n");
	    fprintf(arq, "*     Legenda     *\n");
	    fprintf(arq, "*                 *\n");
	    fprintf(arq, "*  ## : Execucao  *\n");
	    fprintf(arq, "*   -- : Espera   *\n");
	    fprintf(arq, "*                 *\n");
	    fprintf(arq, "* * * * * * * * * *\n\n");

        fprintf(arq, "Tempo\t ");
        for(i = 0; i < numero_processos; i++)
            fprintf(arq, "P%d \t",i+1);

        // Realiza a impressão do diagrama de execução
        fprintf(arq,"\n");
        for(t= 0; t < tempo; t++) {
            fprintf(arq, "%d-%d \t ", t, t+1); // Imprime os períodos de tempo
            for(j = 0; j < numero_processos; j++) {
                    if(ordem_processo[t] == j)  // Se caso o processo ocorre nesse instante, imprimimos (##)
                        fprintf(arq, "## \t");
                    else {
                            if(final_processo[j] > t && vet[j].criacao <= t)  // Senão, se o tempo está entre o instante de tempo em que 
                                                                              // o processo iniciou e o instante em que o processo terminou, 
                            fprintf(arq, "-- \t");                            // estamos em período de espera e imprimimos (--)
                        else
                            fprintf(arq, "   \t"); // Ainda senão, o processo ainda não começou 
                                                   // ou já terminou e, assim, imprimimos vazio ( )
                    }
                }
            fprintf(arq, "\n");
        }

        fprintf(arq, "\nID do processo \t\t Tempo de Duracao \t\t Tempo de Vida \t\t Tempo de Espera");
        fseek(arq, 2, SEEK_CUR);        
        
        for(i = 0; i < numero_processos; i++) {
            // Para cada processo imprime o seu número, os tempos de duração, de vida e de espera
            fprintf(arq, "\nProcesso [%d] \t\t\t\t %d \t\t\t\t %d \t\t\t\t %d", (i + 1), vet[i].duracao, final_processo[i] - vet[i].criacao, tempo_espera[i]);
            // Calcula a soma dos tempos de vida e espera de todos processos
            soma_tempo_vida += (final_processo[i] - vet[i].criacao);
            soma_tempo_espera += tempo_espera[i];
        }
        // Calcula a média dos tempos anteriores e posteriormente e realiza a impressão dos mesmos
        media_tempo_vida = soma_tempo_vida * 1.0/numero_processos;
        media_tempo_espera = soma_tempo_espera * 1.0/numero_processos;
        
        fseek (arq, numero_processos+2, SEEK_CUR);
        fprintf(arq, "\n\nTempo medio de vida:\t%.2f", media_tempo_vida);
        fprintf(arq, "\nTempo medio de espera:\t%.2f", media_tempo_espera);

        fclose(arq);
    }
}

/// Função principal
int main(){
    
    // Função para ler a entrada de um arquivo .txt
    dados();

    // Função para escalonamento dos processos através do algoritmo Round-Robin
    roundrobin();

    // Função para escrever saída em um arquivo .txt
    saida();

    // Limpa o vetor de processos após os procedimentos
    free(vet);

    return 0;
}
