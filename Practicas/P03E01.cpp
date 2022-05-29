#include <string>
#include <iostream>
#include <thread>
#include <atomic>

// COMPILAR: g++ -pthread ejercicio1.cpp -o ejercicio1
// CORRER 100 VECES: for i in {1..100}; do ./ejercicio1; done

using namespace std;

void A(int* x){
    *x = *x + 1;
    printf("%i ", *x);
}

void B(int* x){
    *x = *x+1;
}

int main(int argc, char const *argv[])
{
    int x = 0;
    thread t_a = thread(A, &x);
    thread t_b = thread(B, &x);
    
    t_a.join();
    t_b.join();
    return 0;
}


