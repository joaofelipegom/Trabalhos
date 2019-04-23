#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/wait.h>

int maior_global = -1, prc, menor_global;

sem_t trava;

struct dados_thread {
    int* vetor;
    int tamanho_vetor;
    int numero_da_thread;
};

void* funcao_da_thread(void* argumento_funcao){
	struct dados_thread* dados = (struct dados_thread*)argumento_funcao;

	int i = 0;
  int inicio_vetor = (dados->tamanho_vetor / prc) * dados->numero_da_thread;
  int final_vetor = (dados->tamanho_vetor / prc) * (dados->numero_da_thread + 1);

  int maior = -1;

  for (i = inicio_vetor; i < final_vetor; i++) {
      if (maior < dados->vetor[i]) {
          maior = dados->vetor[i];
      }
  }

  sem_wait(&trava);

  if (maior_global < maior) {
      usleep(rand() % 500);
      maior_global = maior;
  }
  sem_post(&trava);

	int j = 0;
	int auxiliar = dados->vetor[0];
	for(j = 1; j < dados->tamanho_vetor; j++){
		if(dados->vetor[j] < auxiliar){
			auxiliar = dados->vetor[j];
		}
	}
	menor_global = auxiliar;
}

int main(void)
{
    int i = 0, num = 0, a = 0, j = 0, valor, prc_n, sts = 0, val, aux_int, aux_prc;
    float aux;

    printf("CRIANDO UM VETOR\n----------------\n");
    printf("INSIRA O TAMANHO DO VETOR: ");
    scanf("%i", &num);
    int v[num];
    pthread_t vetor_threads[num];
    //system("clear");

    for (a = 0; a < num; a++) {
        printf("VALOR %d : ", a + 1);
        scanf("%f", &aux);
				aux_int = aux;
				while((aux_int!=aux)){
					printf("VALOR INVALIDO! INSIRA NOVAMENTE!");
					printf("VALOR %d : ", a + 1);
	        scanf("%f", &aux);
					aux_int = aux;
				}
				v[a] = aux_int;
    }

    system("clear");

    processadores:
    printf("QUANTIDADE DE PROCESSOS? ");
    scanf("%i", &prc);

    int get_prc = get_nprocs();

    system("clear");

    if (prc > 0) {
        if (prc <= get_prc) {
            pid_t identificacao_processo;

            for (int i = 0; i < prc; i++) {
                identificacao_processo = fork();

                if (identificacao_processo == 0) {
                    struct dados_thread dados[prc];

                    sem_init(&trava, 0, 1);

                    for (i = 0; i < prc; i++) {
                        dados[i].vetor = v;
                        dados[i].tamanho_vetor = num;
                        dados[i].numero_da_thread = i;
                        pthread_create(&vetor_threads[i], NULL, funcao_da_thread, &dados[i]);
                    }

                    sem_destroy(&trava);

                    for (i = 0; i < prc; i++) {
                        pthread_join(vetor_threads[i], NULL);
                    }
                    printf("MAIOR VALOR DO VETOR: %d\n", maior_global);
										printf("MENOR VALOR DO VETOR: %d\n", menor_global);
                }
            }
        }
    }
		else{
			printf("VALORES NEGATIVOS NÃO SÃO PERMITIDOS!");
			goto processadores;
		}
    return 0;
}
