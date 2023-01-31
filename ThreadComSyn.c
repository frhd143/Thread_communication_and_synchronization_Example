/*
    Written by Farhad Asadi, BTH student
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Shared Variables
pthread_mutex_t lock;
double bankAccountBalance = 0;

void deposit(double amount) {
    // Create Lock
    pthread_mutex_lock(&lock);
    bankAccountBalance += amount;
    pthread_mutex_unlock(&lock);
    // Release Lock
}

void withdraw(double amount) {
    // Create Lock
    pthread_mutex_lock(&lock);
    bankAccountBalance -= amount;
    // Release Lock
    pthread_mutex_unlock(&lock);
}

// utility function to identify even-odd numbers
unsigned odd(unsigned long num) {
    return num % 2;
}

// simulate id performing 1000 transactions
void do1000Transactions(unsigned long id) {
    //printf("The id is: %d\n", id);
    for (int i = 0; i < 1000; i++) {
        if (odd(id))
            deposit(100.0); // odd threads deposit
        else
            withdraw(100.0); // even threads withdraw
    }
}

void* child(void* buf) {
    unsigned long childID = (unsigned long)buf;
    do1000Transactions(childID);
    return NULL;
}

int main(int argc, char** argv) {
    pthread_t *children;
    unsigned long id = 0;
    unsigned long nThreads = 0;
    if (argc > 1)
        nThreads = atoi(argv[1]);
    children = malloc( nThreads * sizeof(pthread_t) );

    // Initialize the mutex
    pthread_mutex_init(&lock, NULL);
    for (id = 1; id < nThreads; id++)
        pthread_create(&(children[id-1]), NULL, child, (void*)id);

    do1000Transactions(0); // main thread work (id=0)
    for (id = 1; id < nThreads; id++)
        pthread_join(children[id-1], NULL);

    // Destroy the mutex
    pthread_mutex_destroy(&lock);
    printf("\nThe final account balance with %lu threads is $%.2f.\n\n", nThreads, bankAccountBalance);

    free(children);
    pthread_mutex_destroy(&lock);
    return 0;
}
