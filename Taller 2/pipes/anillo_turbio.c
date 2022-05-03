#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char **argv)
{	
	int status, n, start;
	int buffer[1];

	if (argc != 4){ printf("Uso: anillo <n> <c> <s> \n"); exit(0);}
    
  	//Leo los inputs
	n = atoi(argv[1]);
	buffer[0] = atoi(argv[2]);
	start = atoi(argv[3]);

	//Array de pids
	int pids[n];

    printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n", n, buffer[0], start);

	//hago el pipe padre - hijo principal
	int resultado[2];

	//Abro el pipe
	pipe(resultado);
    
	//Hago pipeline de los hermanos
	int hermanos[2];
	int turnos[2];

	//Abro el pipe de los hermanos
	pipe(hermanos);
	pipe(turnos);

	for (int i = 1; i <= n; i++){

		int pipe_hijo[2];
		pids[i] = fork();

		if(pids[i] == -1){
			perror("fork");
			exit(1);
		}

		//Soy un hijo
		if(pids[i] == 0){

			//Soy el hijo inicial
			if (i == start){
				
				//Hago el numero secreto
				int numero_secreto = 8;
				printf("Soy el primer hijo! Hijo numero %d, el numero secreto es %d, mi pid es %d\n", i, numero_secreto, getpid());

				//Cierro la lectura con el padre
				close(resultado[0]);

				buffer[0]++;
				int turno = i;

				//escribo el primer numero en el pipe y empiezo el ciclo
				printf("Voy a empezar la cadena escribiendo %d\n", buffer[0]);
				write(hermanos[1], &buffer[0], sizeof(buffer[0]));
				write(turnos[1], &turno, sizeof(turno));

				//cierro los otros pipes
				close(hermanos[1]);
				close(turnos[1]);

				int numero;

				//espero que el numero sea mas grande que el secreto
				while(1){

					// Leo lo que esta en la pipe comunal
					read(hermanos[0], &numero, sizeof(numero));

					if (numero >= numero_secreto){
						break;
					}
					
				}

				//Sali del ciclo, escribo en el pipe con el padre el numero resultante
				printf("Supere el numero secreto, le mando %d al padre\n", numero);
				write(resultado[1], &numero, sizeof(numero));
				exit(0);			

			}
			//Soy un hijo normal
			else {

				//cierro pipe padre - hijo principal
				close(resultado[0]);
				close(resultado[1]);

				int numero;
				int turno;

				pipe(pipe_hijo);

				//cierro lectura de mi pipe
				close(pipe_hijo[0]);

				// cierro escritura del pipe comunal y apunto mi pipe a ese pipe
				dup2(hermanos[1], pipe_hijo[1]);

				while (1){
				
					read(turnos[0], &turno, sizeof(turno));

					if (turno == i-1){		

						read(hermanos[0], &numero, sizeof(numero));
						printf("El turno anterior fue %d\nSoy el hijo %d, lei el numero %d, mi pid es %d\n", turno, i, numero, getpid());

						//incremento el numero
						numero++;
						turno = i;

						//escribo el numero
						write(pipe_hijo[1], &numero, sizeof(numero));

						//escribo el turno
						write(turnos[1], &turno, sizeof(turno));

						printf("Soy el hijo %d, escribi el numero %d\n", i, numero);

					} 
				}

			}
		} 
		//Soy el padre
		else {
			//close(resultado[1]);
		}
	
	}
	
	int pid = pids[start];

	waitpid(pid,&status, 0);
	printf("El proceso inicial tiene de pid %d\n", pid);

	for (int i = 1; i < n; i++){
		if (i != start){
			kill(pids[i], SIGKILL);
		}
	}

	int result;
	read(resultado[0], &result, sizeof(result));

	printf("Fin del programa el numero fue: %d\n", result);

	return 0;
    
}
