#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char **argv)
{	
	int status, n, start;
	int buffer[1];

	if (argc != 4){ printf("Uso: anillo <n> <c> <s> \n"); exit(0);}
    
  	//Leo los inputs
	n = atoi(argv[1]);
	buffer[0] = atoi(argv[2]);
	start = atoi(argv[3]);

    printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n", n, buffer[0], start);

	//Array de pids para luego matar a los procesos
	int pids[n+1];

	//Array de pipes y pipe para comunicacion padre/hijo principal
	int pipes[n+1][2];

	//Abro los pipes
	for (int i = 0; i <= n; i++){
		pipe(pipes[i]);
	}
	
	//escribo el numero inicial en el pipe anterior a mi hijo principal
	printf("Soy el padre, voy a empezar la cadena escribiendo %d\n", buffer[0]);
	write(pipes[start - 1][1], &buffer[0], sizeof(buffer[0]));

	//Empiezo a hacer los hijos
	for (int i = 1; i <= n; i++){

		//Doy a luz
		if((pids[i] = fork()) == -1){
			perror("fork");
			exit(1);
		}

		//Soy un hijo
		if(pids[i] == 0){

			//Cierro los pipes que no necesito
			for (int j = 0; j <= n; j++){
				
				//CASOS ESPACIALES
				//si soy el principal no cierro conexion con padre
				if (i == start && j == 0){
					continue;
				} 
				//si soy el primero no cierro conexion con el ultimo
				else if (i == 1 && j == n){
					continue;
				}
				
				//GENERICOS
				//no cierro el anterior, ni el mio
				else if ((j == i-1 && i != 1) || j == i){
					continue;
				} else {
					close(pipes[j][0]);
					close(pipes[j][1]);
				}

			}
			
			//Soy el hijo inicial
			if (i == start){
				
				//Hago el numero secreto
				srand(time(NULL));
				int numero_secreto = rand();
				numero_secreto = numero_secreto % 100;
				printf("El numero secreto es %d\n", numero_secreto);

				//Cierro la lectura con el padre
				close(pipes[0][0]);

				//cierro hermanos
				i == 1 ? close(pipes[n][1]) : close(pipes[i - 1][1]);
				close(pipes[i][0]);

				int numero;

				//espero que el numero sea mas grande que el secreto
				while(1){

					//Leo lo que esta en la pipe de mi anterior
					read(pipes[i-1][0], &numero, sizeof(numero));
					//printf("Soy el hijo %d, recibi el numero %d de mi hermano %d\n", i, numero, (i==1 ? n : i-1));

					//si el numero es mas grande o igual que el numero secreto, salgo del ciclo 
					if (numero >= numero_secreto){
						break;
					} 
					//sino, aumento el numero
					else {
						numero++;
						write(pipes[i][1], &numero, sizeof(numero));
					}
					
				}

				//Sali del ciclo, escribo en el pipe con el padre el numero resultante
				//printf("\nSupere el numero secreto, le mando %d al padre\n\n", numero);
				write(pipes[0][1], &numero, sizeof(numero));
				exit(0);			

			}

			//Soy un hijo normal
			else {

				// cierro hermanos
				close(pipes[i - 1][1]);
				close(pipes[i][0]);

				int numero;

				// voy aumentando el i
				while (1){

					// Leo lo que esta en la pipe de mi anterior
					if (i == 1){
						read(pipes[n][0], &numero, sizeof(numero));
					} else {					
						read(pipes[i - 1][0], &numero, sizeof(numero));
					}

					//printf("Soy el hijo %d, recibi el numero %d de mi hermano %d\n", i, numero, i - 1);

					numero++;
					write(pipes[i][1], &numero, sizeof(numero));
				}
			} 
		} 
	}

	// soy el padre
	// cierro todos los pipes menos la lectura del primero
	for (int j = 1; j < n; j++)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
	}

	close(pipes[0][1]);

	int pid = pids[start];

	waitpid(pid, &status, 0);

	//mato a los hijos inutiles
	for (int i = 1; i < n; i++){
		if (i != start){
			kill(pids[i], SIGKILL);
		}
	}

	printf("MASAAAAACRE\n");

	int result;
	read(pipes[0][0], &result, sizeof(result));

	printf("Fin del programa el numero fue: %d\n", result);

	return 0;
    
}
