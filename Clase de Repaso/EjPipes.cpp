//Este ejercicio hecho en clase tiene muchos problemas!

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

using namespace std;

#define MIN_RANGE 2
#define MAX_RANGE 1000000001

int contarPares(int start, int end){
    int cantidadPares = 0;
    for (int i = start; i < end; i++){
        if(i%2 == 0)
            cantidadPares++;
    }
    return cantidadPares;
}

void cerrarPipes(int pipes[][2], int i, int n){
    for (int j = 0; j < n; j++){
        if (j!=i){
            //cierro el pipe
            close(pipes[j][0]);
            close(pipes[j][1]);
        }
    }
}

//Seria mejor pasarle cantidad por parametro y te ahorras un read y write pero bueno asi lo hicieron en clase.
void ejecutarHijo(int pipes[][2], int i){
    
    //Cierro los otros pipes
    //Codigo del hijo
    int start;
    read(pipes[i][0], &start, sizeof(start));
    
    int end;
    read(pipes[i][0], &end, sizeof(end));

    int total = contarPares(start, end);

    write(pipes[i][1], & total, sizeof(total));

}


int main(int argc, char const *argv[]){
    
    //Me fijo que tengo mas de una N
    if (argc < 2){
        cout << "Debe ejecutar con N como parametro" << endl;
        return 1;
    }

    //Agarro la N
    int N = atoi(argv[1]);

    //Creo el array de pipes
    int pipes[N][2];

    //Creo los pipes
    for (int i = 0; i < N; i++){
        if (pipe(pipes[i]) == -1){
            cout << "Pipe Error" << endl;
            return 1;
        }
    }

    int pid;

    //Doy a luz
    for (int i = 0; i < N; i++){
        
        pid = fork();
        
        //Soy el hijo
        if (pid == 0){
            ejecutarHijo(pipes, i);
            return 0;
        }
    }

    //Soy el padre

    int cantidad = ((MAX_RANGE - MIN_RANGE) + (N - 1))/N;

    int rango_inicial = MIN_RANGE;
    for (int i = 0; i < N; i++){
        int rango_final = rango_inicial + cantidad;
        write(pipes[i][1], &rango_inicial, sizeof(rango_inicial));
        write(pipes[i][1], &rango_final, sizeof(rango_final));
        rango_inicial += cantidad;
    }

    wait(NULL);

    int total = 0;
    for (int i = 0; i < N; i++){
        int pares = 0;
        read(pipes[i][0], &pares, sizeof(total));
        total+= pares;        
    }
    
    cout << "Total: " << total << endl;

    return 0;
}
