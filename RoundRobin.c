#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int criacao, duracao, prioridade;
}processo;

processo *vet;
int n;

void dados(){
    /// Leitura e atribuição dos dados de um arquivo "dados.txt" para um vetor dinamicamente alocado. Nesse arquivo,  
    /// as três colunas são separadas por um espaço e não pode existir linhas vazias.
    int i,j;
    char car;
    char c[20];
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

        for(i = 0; i < n; i++){
            printf("%d %d %d", vet[i].criacao, vet[i].duracao, vet[i].prioridade);
            printf("\n");
        }
    }
}

void main(){
    int i;
    dados();
    free(vet);
}