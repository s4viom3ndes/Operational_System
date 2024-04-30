#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

// Implementar um algoritmo de espera circular -> detecção de deadlock

// verificar se existe ciclo de espera entre os procesos -> implementação de grafos 

// Implementa algoritmos que previne dead-lock, algoritmos do banqueiro: recebe (processsos e recursos) | retorna (processo pronto pra executar ou não)

// devo implementar uma estrutura que resume a disponibilidade de recursos


typedef struct{
    int IdREc; // id do recurso
    bool alocado; // retorna True ou False durante a verificação de alocçãod e recurso
}Recursos;


typedef struct{
    int Id;
    int tChegada;
    int duracao;
    int tEspera;
}Processos;

void VerificaCiclo(Processos *p, Recursos *r, int numproc, int numrec){
    int Adj[numproc][numrec];
    
}


void Baqueiro(){

}

void DetectaDeadLock(){

}

// Funcao que simula a execução de um processo
void executarProcesso(Processos *p) {
    printf("Executando processo %d por %d unidades de tempo.\n", p->Id, p->duracao);
    Sleep(p->duracao * 1000); // Espera o tempo de duração do processo em milissegundos
    printf("Processo %d concluído.\n", p->Id);
}

// vou usar essa implementação para realizar os processos
void fcfs(Processos *proc, int Numproc){
    int tempoAtual = 0;
    for(int i = 0; i < Numproc; i++){
        Processos *p = &proc[i];
        p->tEspera = tempoAtual - p->tChegada;
        tempoAtual += p->duracao;
        executarProcesso(p);
    }
}

int main(int argc, char *argv[]){

    if (argc < 2){
        printf("Uso: %s <numero_de_processos>\n", argv[0]);
        return 1;
    }

    int numproc = atoi(argv[1]);
    Processos *p = (Processos *)malloc(numproc * sizeof(Processos));
    Recursos *r = (Recursos *)malloc(10 * sizeof(Recursos));

    // iniciando recuros

    for(int j = 0 ; j < 10; j++){
        r[j].IdREc = j+1;
        r[j].alocado = false;
    }

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

    fcfs(p, numproc);
    return 0;
}

