#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


// Atributos de processo
typedef struct {
    int Id;
    int tChegada;
    int duracao;
    int tEspera;
} Processos;

// Funcao que simula a execução de um processo
void executarProcesso(Processos *p) {
    printf("Executando processo %d por %d unidades de tempo.\n", p->Id, p->duracao);
    Sleep(p->duracao * 1000); // Espera o tempo de duração do processo em milissegundos
    printf("Processo %d concluído.\n", p->Id);
}


// Funcão First come, First Served 

void fcfs(Processos *proc, int Numproc){
    int tempoAtual = 0;
    for(int i = 0; i < Numproc; i++){
        Processos *p = &proc[i];
        p->tEspera = tempoAtual - p->tChegada;
        tempoAtual += p->duracao;
        executarProcesso(p);
    }
}


// Funcao Round Robin
void rr(Processos *proc, int Numproc, int quantum) {
    int tempoAtual = 0;
    int *tempoRestante = (int*)malloc(Numproc * sizeof(int)); // Array para armazenar o tempo restante de cada processo
    if (tempoRestante == NULL) {
        printf("Erro ao alocar memória para o tempo restante dos processos\n");
        return;
    }
    for (int i = 0; i < Numproc; i++) {
        tempoRestante[i] = proc[i].duracao;
    }
    while (1) {
        int todosConcluidos = 1; // condiçao de parada do loop while
        for (int i = 0; i < Numproc; i++) {

            Processos *p = &proc[i];

            if (tempoRestante[i] > 0) {
                todosConcluidos = 0;
                int tempoExecutado = (tempoRestante[i] < quantum) ? tempoRestante[i] : quantum;// verificando se o temo é menor que o quantum definido
                tempoRestante[i] -= tempoExecutado;
                tempoAtual += tempoExecutado;
                if (tempoRestante[i] == 0) {
                    p->tEspera = tempoAtual - p->tChegada - p->duracao;
                    printf("Processo %d concluído. Tempo de espera: %d\n", p->Id, p->tEspera);
                } else {
                    printf("Executando processo %d por %d unidades de tempo.\n", p->Id, tempoExecutado);
                }
            }
        }
        if (todosConcluidos) {
            break;
        }
    }
    free(tempoRestante);
}


// Funçao de ordenação auxiliar p/ a sjf (Insertion Sort)
void ordena(int vet[], int tam) {
    for (int i = 1; i < tam; i++) {
        int chave = vet[i];
        int j = i - 1;
        while (j >= 0 && vet[j] > chave) {
            vet[j + 1] = vet[j];
            j--;
        }
        vet[j + 1] = chave;
    }
}


// Funcao Shortest jobd first

void sjf(Processos *proc, int Numproc) {
    int tempoAtual = 0;
    int *aux = (int*)malloc(Numproc * sizeof(int));
    if (aux == NULL) {
        printf("Erro ao alocar memória para o array auxiliar\n");
        return;
    }
    for (int i = 0; i < Numproc; i++){
        Processos *p = &proc[i];
        aux[i] = p->tEspera;
    }
    ordena(aux, Numproc);
    for (int i = 0; i < Numproc; i++) {
        Processos *p = &proc[i];
        for (int j = 0; j < Numproc; j++) {
            if (aux[i] == p->tEspera) {
                executarProcesso(p);
                break;
            }
        }
    }
    free(aux);
}

int main(int argc, char *argv[]){
    if (argc < 2){
        printf("Uso: %s <numero_de_processos>\n", argv[0]);
        return 1;
    }
    int numproc = atoi(argv[1]);
    Processos *p = (Processos *)malloc(numproc * sizeof(Processos));
    if(p == NULL){
        printf("Erro ao alocar memoria para os processos\n");
        return 1;
    }
    srand(GetTickCount());
    for(int i = 0; i < numproc; i++){
        p[i].Id = i + 1;
        p[i].tChegada = rand() % 100; // Tempo aleatório
        p[i].duracao = rand() % 20 + 1; // Algum tempo entre 1 e 20
    }
    printf("First-Come, First-Served (FCFS):\n");
    fcfs(p, numproc);
    printf("\nRound Robin (RR):\n");
    rr(p, numproc, 2); // Exemplo de quantum: 2 unidades de tempo
    printf("\nShortest Job First (SJF):\n");
    sjf(p, numproc);
    free(p);
    return 0;
}
