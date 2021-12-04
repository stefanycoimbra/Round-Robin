// Inclusão das bibliotecas
#include <stdio.h>
#include <stdlib.h>

// Struct para guardar cada processo com sua criação, duração e prioridade
typedef struct {
    int criacao, duracao, prioridade;
}processo;

processo *vet;

int n, cont_saida = 0, time = 0;
int conta_proc = 0, total_time = 0;
int *burst_time, *arrival_time, *total_tp, *order_process;
int *wait_tp, *life_tp;
float average_wait_time, average_life_time;

// Função para leitura da entrada
void dados() {

    // Leitura e atribuição dos dados de um arquivo "dados.txt" para um vetor dinamicamente alocado. Nesse arquivo,
    // as três colunas são separadas por um espaço e não pode existir linhas vazias.
    int i,j;
    char car;
    FILE *arq;
    arq = fopen("dados.txt", "r");
    n = 0;
    if(arq == NULL)
        printf("Nao foi possivel abrir o arquivo");
    else{
        // Determina quantas linhas existe no arquivo
        while((car = getc(arq)) != EOF){
            if(car == '\n')
                n++;
        }
        n++;

        // Reserva espaço para guardar os valores do arquivo
        vet = (processo *) malloc(n*sizeof(processo));

        // Armazena os valores
        fseek(arq, 0, SEEK_SET);
        for(i = 0; i < n; i++)
            fscanf(arq, "%d %d %d\n", &vet[i].criacao, &vet[i].duracao, &vet[i].prioridade);
        fclose(arq);

        // Reserva espaço para armazenas a duração de cada processo e seus tempos de chegada na fila
        // de processos, respectivamente
        burst_time = (int *) malloc(n*sizeof(int));
        arrival_time = (int *) malloc(n*sizeof(int));

        for(i = 0; i < n; i++){
            printf("%d %d %d", vet[i].criacao, vet[i].duracao, vet[i].prioridade);
            printf("\n");

            burst_time[i] = vet[i].duracao;
            arrival_time[i] = vet[i].criacao;
        }

        printf("\n");

        // Número total de processos
        conta_proc = n;
    }
}

void saida() {

    int i, j;
    FILE *arq;

    // Criação e atribuição de dados para um arquivo "saida.txt"
    arq = fopen("saida.txt", "w");
    if(arq == NULL)
        printf("Nao foi possivel abrir o arquivo");
    else{
        // Armazena os valores no arquivo de texto e também imprime na tela

        fprintf(arq,"Tempo \t");
        for (i = 0; i < conta_proc; i++)
        {
            fprintf(arq, " P%d \t",i+1);
        }

        fprintf(arq, "\n");

        for(i = 0; i < time; i++)
        {
            fprintf(arq, "%d-%d \t ",i,i+1);
            for (j = 0; j < conta_proc; j++)
            {
                if(order_process[i] == j)
                    fprintf(arq, "## \t");
                else
                    fprintf(arq, "-- \t");
            }
            fprintf(arq, "\n");
        }

        fprintf(arq, "\nID do processo \t\t Tempo de Duracao \t\t Tempo de Vida \t\t Tempo de Espera");
        printf("ID do processo \t\t Tempo de Duracao \t\t Tempo de Vida \t\t Tempo de Espera");
        fseek (arq, 2, SEEK_CUR);
        for (i = 0; i < conta_proc; i++)
        {
            fprintf(arq, "\nProcesso [%d] \t\t\t\t %d \t\t\t\t %d \t\t\t %d", (i + 1), (vet[i].duracao), (total_tp[i] - arrival_time[i]), (total_tp[i] - arrival_time[i] - vet[i].duracao));
            printf("\nProcesso [%d] \t\t\t\t %d \t\t\t\t %d \t\t\t %d", (i + 1), (vet[i].duracao), (total_tp[i] - arrival_time[i]), (total_tp[i] - arrival_time[i] - vet[i].duracao));
        }
        fseek (arq, conta_proc+2, SEEK_CUR);
        fprintf(arq, "\n\nTempo medio de vida:\t%f", average_life_time);
        fprintf(arq, "\nTempo medio de espera:\t%f", average_wait_time);

        printf("\n\nTempo medio de vida:\t%f", average_life_time);
        printf("\nTempo medio de espera:\t%f", average_wait_time);

        fclose(arq);
    }
}

// Função para escalonamento dos processos através do algoritmo Round-Robin
void roundrobin() {

    // Variáveis:
    // i & j -> para o loop for;
    // time -> instante de tempo;
    // max_proc -> guarda o número máximo de processos;
    // terminado -> flag que indica se o processo foi completamente executado;
    // quantum -> guarda o valor do quantum usado para a construção do algoritmo;
    // wait_time -> tempo de espera dos processos;
    // life_time -> tempo de vida dos processos;

    int i, j, max_proc, terminado = 0, quantum;
    int wait_time = 0, life_time = 0;
    int total_burst = 0;

    max_proc = conta_proc;
    quantum = 3;

    // Aloca espaço para guardar os tempos totais, de espera e de vida dos processos, respectivamente
    total_tp = (int *) malloc(n*sizeof(int));
    wait_tp = (int *) malloc(n*sizeof(int));
    life_tp = (int *) malloc(n*sizeof(int));

    //Aloca espaço para armazenar a ordem de exucução dos processos
    for(i = 0; i < conta_proc; i++)
	total_burst += burst_time[i];
    order_process = (int *) malloc(total_burst*sizeof(int));

    // Para cada processo da fila
    for(total_time = 0, i = 0; max_proc != 0;)
    {

        // Se a duração não for nula ou, por proteção da entrada, negativa,
        // e se a duração do processo for menor ou igual ao quantum determinado
        if(burst_time[i] <= quantum && burst_time[i] > 0)
        {
            // O tempo total para a execução do processo é igual ao tempo
            // que se passou de outros processos anteriores + sua própria duração
            
            //registra qual processo está sendo executado em cada instante
	    for(j = 0;  j < burst_time[i]; j++)
            	order_process[time++] = i;
            
            total_time = total_time + burst_time[i];
            burst_time[i] = 0;
            terminado = 1;
        }

        // Se a duração do processo for maior que o quantum
        else if(burst_time[i] > 0)
        {
            // A duração da execução será apenas do quantum e o resto será
            // executado depois, com o processo jogado para o final da fila
            burst_time[i] = burst_time[i] - quantum;
            total_time = total_time + quantum;
            //registra qual processo está sendo executado em cada instante
            for(j = 0;  j < quantum; j++)
                order_process[time++] = i;
        }

        // Se a duração para o processo chegou a zero e se o contador é igual a 1,
        // pela primeira verificação, ele foi completamente executado
        // e sua linha no diagrama de execução pode ser escrita na saída
        if(burst_time[i] == 0 && terminado == 1)
        {
            // Menos um processo para a continuação da divisão do processador
            max_proc--;

            // O tempo de espera conta o tempo total e tira dele
            // o tempo de chegada dele na fila e sua duração
            wait_time = wait_time + total_time - arrival_time[i] - vet[i].duracao;
            
            // O tempo de vida conta o tempo total e tira dele
            // o tempo de chegada dele na fila
            life_time = life_time + total_time - arrival_time[i];

            // Guarda os tempos totais, de espera e de vida totais
            total_tp[i] = total_time;
            wait_tp[i] = wait_time;
            life_tp[i] = life_time;

            // Contador é zerado para o próximo processo
            terminado = 0;
        }

        // Se chegou ao fim dos processos nessa passada pela fila
        // é o momento de verificar os processos que foram para o final
        // e precisam ser finalizados
        if(i == (conta_proc - 1))
        {
            i = 0;
        }

        // Se o tempo de chegada do próximo processo for menor que o tempo total,
        // então será aquele que o processador irá executar
        else if(arrival_time[i + 1] <= total_time)
        {
            i++;
        }

        // Em qualquer outra situação, a fila é rotacionada
        else
        {
            i = 0;
        }

    }

    // O tempo médio de espera é obtido a partir da razão da somatória dos tempos de espera
    // individuais pelo número de processos
    average_wait_time = wait_time * 1.0 / conta_proc;

    // O tempo médio de vida é obtido a partir da razão da somatória dos tempos de vida
    // individuais pelo número de processos
    average_life_time = life_time * 1.0 / conta_proc;

    //printf("\nTempo medio de vida:\t%f\n", average_life_time);
    //printf("\nTempo medio de espera:\t%f", average_wait_time);

}

int main(){
    int i;

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
