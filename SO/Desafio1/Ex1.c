#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define READ_END 0
#define WRITE_END 1

// Função para calcular a soma de um intervalo de números
int calcularSoma(int inicio, int fim) {
    int soma = 0;
    for (int i = inicio; i <= fim; i++) {
        soma += i;
    }
    return soma;
}
// função main que cham e calcula os processos

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Como usar: %s <numero_de_processos_filhos> <tamanho_da_lista>\n", argv[0]);
        return 1;
    }

    int numProcessos = atoi(argv[1]);
    int tamanhoLista = atoi(argv[2]);
    int numerosPorProcesso = tamanhoLista / numProcessos;
    int resto = tamanhoLista % numProcessos;
    int inicio = 1;
    int fim = 0;

    HANDLE pipeHandles[numProcessos][2];
    STARTUPINFO si[numProcessos];
    PROCESS_INFORMATION pi[numProcessos];

    // Cria pipes para comunicação entre processos
    for (int i = 0; i < numProcessos; i++) {

        // verificando se a pipe foi criada com sucesso 
        if (!CreatePipe(&pipeHandles[i][READ_END], &pipeHandles[i][WRITE_END], NULL, 0)) {
            fprintf(stderr, "Falha ao criar pipes.\n");
            return 1;
        }

        ZeroMemory(&si[i], sizeof(si[i]));
        si[i].cb = sizeof(si[i]);
        ZeroMemory(&pi[i], sizeof(pi[i]));

        // Calcula o fim do intervalo para este processo filho
        fim = inicio + numerosPorProcesso - 1;
        // Adiciona o resto ao último processo filho
        if (resto > 0 && i == numProcessos - 1) {
            fim += resto;
        }

        char parametros[50];
        sprintf(parametros, "%d %d %d %d", inicio, fim, pipeHandles[i][WRITE_END], numProcessos);
        
        if (!CreateProcess(NULL, argv[0], NULL, NULL, TRUE, 0, NULL, NULL, &si[i], &pi[i])) {
  
            fprintf(stderr, "Falha ao criar processo filho.\n");
            return 1;
        }

        // Fecha o handle de escrita do pipe no processo pai
        CloseHandle(pipeHandles[i][WRITE_END]);

        inicio = fim + 1;
    }

    // Processo pai lê os resultados parciais dos processos filhos e calcula a soma total
    int somaTotal = 0;
    for (int i = 0; i < numProcessos; i++) {
        int resultadoParcial;
        DWORD bytesRead;
        char buffer[50];

        // Lê do pipe no processo pai
        ReadFile(pipeHandles[i][READ_END], buffer, sizeof(buffer), &bytesRead, NULL);
        resultadoParcial = atoi(buffer);
        somaTotal += resultadoParcial;

        CloseHandle(pipeHandles[i][READ_END]);
    }

    printf("Soma total: %d\n", somaTotal);

    return 0;
}
