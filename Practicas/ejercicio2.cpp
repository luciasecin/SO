#include <string>
#include <iostream>
#include <thread>
#include <atomic>

// COMPILAR: g++ -pthread ejercicio2.cpp -o ejercicio2
// CORRER 100 VECES: for i in {1..100}; do ./ejercicio2; done

using namespace std;

void A(int *x, int *y){
    while (*x < 4){
        *y = 0;
        printf("%i ", *x);
        *y = 1;
        *x = *x + 1;
    }
    
}

void B(int *x, int *y){
    while (*x < 4){
        if (*y == 1)
            printf("a");
    }
}

int main(int argc, char const *argv[])
{
    int x = 0;
    int y = 0;
    thread t_a = thread(A, &x, &y);
    thread t_b = thread(B, &x, &y);

    t_a.join();
    t_b.join();
    return 0;
}
