#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

int processadores = 0;

int maior_global = -1;

struct dados_thread {
    int *vet;
    int tamanho_vetor;
    int numero_da_thread;
};

void *funcao_da_thread(void *argumento_funcao) {

    struct dados_thread *dados = (struct dados_thread *)argumento_funcao;

    int i = 0;
    int inicio_vetor = (dados->tamanho_vetor/4)*dados->numero_da_thread;
    int final_vetor = (dados->tamanho_vetor/4)*(dados->numero_da_thread + 1);

    /* Variable local do maior numero do vetor */
    int maior = -1;

    printf("Sou a Thread %d -- Vetor inicia em %d e termina em %d\n", dados->numero_da_thread, inicio_vetor, final_vetor);

    for (i = inicio_vetor; i < final_vetor; i++) {
        if (maior <dados->vet[i]) {
            maior = dados->vet[i];
        }
    }

    printf("Sou a Thread %d -- Achei maior igual à %d\n", dados->numero_da_thread, maior);

    printf(" ----> Sou a Thread %d -- Vou atribuir meu valor maior.\n", dados->numero_da_thread);
    if (maior_global < maior) {
        usleep(rand() % 500);
        maior_global = maior;
    }
    printf(" ----> Sou a Thread %d -- Ja atribui meu valor maior.\n", dados->numero_da_thread);
}

///////////////////////////////////////////////////////////////////////////////

int main(void){
  int soma=0, tamanhoVetor, ind=0, temp;
  float vrf;

  printf("CRIANDO UM VETOR\n----------------\n");
  printf("QUANTIDADE DE NÚMEROS: ");
  scanf("%i",&tamanhoVetor);

  int vetor[tamanhoVetor];
  system("clear");

  for(int i=0;i<tamanhoVetor;i++){
    loop:
    printf("%i : ", i+1);
    scanf("%f",&vrf);
      if(vrf >= 0){
        if (vrf == (int)vrf){
          vetor[i] == vrf;
        }
        else{
          printf("NUMEROS DECIMAIS NAO SAO PERMITIDOS\nDIGITE OUTRO NUMERO!\n");
          goto loop;
        }
      }
      else{
        printf("NUMEROS NEGATIVOS NAO SAO PERMITIDOS\nDIGITE OUTRO NUMERO!\n");
        goto loop;
      }
    }
  system("clear");

  //Obtem o numero de processadores utilizados
  processadores:
  printf("QUANTOS PROCESSADORES DESEJA UTILIZAR? ");
  scanf("%i",&processadores);

  int prc = get_nprocs();
  if(processadores > 0){
    if(processadores <= prc){

      // UTILIZACAO DO FORK ///////////////////////////////////////////////////
      pid_t identificacao_processo;

      for (int i = 0; i < processadores; i++) {
              identificacao_processo = fork();

          if (identificacao_processo == 0) {
            // THREADS ////////////////////////////////////////////////////////
            pthread_t vetor_threads[processadores];
            struct dados_thread dados[processadores];

              /* Cria as Threads */
              for (i = 0; i < processadores; i++) {
                  /* Passa dados para a Thread */
                  dados[i].vet = vetor;
                  dados[i].tamanho_vetor = tamanhoVetor;
                  dados[i].numero_da_thread = i;
                  pthread_create(&vetor_threads[i], NULL, funcao_da_thread, &dados[i]);
              }

              /* Conclui as Threads */
              for (i = 0; i < processadores; i++) {
                  pthread_join(vetor_threads[i], NULL);
              }
                        printf("Maior valor do vetor: %d\n", maior_global);
            ///////////////////////////////////////////////////////////////////
          }
        }
    ///////////////////////////////////////////////////////////////////////////
    }
    else{
      printf("NAO FOI POSSIVEL COMPLETAR\n");
      goto processadores;
    }
  }
  else{
    printf("O NUMERO DE PROCESSADORES DEVE SER MAIOR QUE 2!\n");
    goto processadores;
  }

  return 0;
}
