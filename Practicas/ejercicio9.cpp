#include <string>
#include <iostream>
#include <thread>
#include <atomic>
#include <stdio.h>
#include <vector>
#include <semaphore.h>

// COMPILAR: g++ -pthread ejercicio2.cpp -o ejercicio2
// CORRER 100 VECES: for i in {1..100}; do ./ejercicio2; done

using namespace std;

sem_t sem;
sem_t iniciar;
int orden;

void fn(int *j, int* i, int* N){

    if(*j == *i){
        //sem_wait(&iniciar);
        cout << "Soy el proceso inicial: " << *j << endl;
        if (orden == (*N) - 1){
            orden = 0;
        }
        else{
            orden++;
        }
        //sem_post(&sem);
    }

    else if (*j != *i){

        //sem_wait(&sem);
        cout << "Soy el Proceso: " << *j << endl;

        if (orden == *j){
            cout << "entre" << endl;
            if (orden == (*N) - 1){
                orden = 0;
            }
            else{
                orden++;
            }
            sem_post(&sem);
        }
    }
}

int main(int argc, char const *argv[])
{
    int total = atoi(argv[1]);
    int i = atoi(argv[2]);


    //sem_init(&sem, 0, 0);
    //sem_init(&iniciar, 0, 1);

    orden = i;

    thread threads[total];

    for (int j = 0; j < total; j++){
        threads[j] = thread(fn, &j, &i, &total);
    }
    
    for (int j = 0; j < total; j++){
        threads[j].join();
    }

    //sem_destroy(&sem);
    
    return 0;
}
