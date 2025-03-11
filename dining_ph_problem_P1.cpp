#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore>
#include <chrono>

using namespace std;
using namespace std::this_thread;
using namespace std::chrono;

// Constanrs representing philosopher states
const int THINKING = 0;
const int HUNGRY = 1;
const int EATING = 2;

int N;  // Number of philosophers
int* state;  // Array to store the state of each philosopher
mutex mutex1;  // Mutex to ensure mutual exclusion when modifying shared resources
thread* threads;  // Array of philosopher threads
counting_semaphore<1>** S;  // Array of semaphores, one for each philosopher


// Function to determine the left and right neighbors of a philosopher
pair<int, int> left_right(int phnumber, int N) {
    return {(phnumber + N - 1) % N, (phnumber + 1) % N};
}


// Function to check if a philosopher can start eating
void test(int phnumber) {

    auto [LEFT, RIGHT] = left_right(phnumber, N);

    if (state[phnumber] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        state[phnumber] = EATING;
        cout << "Philosopher " << phnumber + 1 << " takes fork " << LEFT + 1 << " and " << phnumber + 1 << endl;
        cout << "Philosopher " << phnumber + 1 << " is EATING" << endl;
        sleep_for(seconds(2));
        S[phnumber]->release(); // Increment the semaphore of the philosopher - get permission
    }
}


// Function for a philosopher to take forks and try to eat
void take_fork(int phnumber) {

    mutex1.lock(); // Begin of CRITICAL SECTION 
    state[phnumber] = HUNGRY;
    cout << "Philosopher " << phnumber + 1 << " is HUNGRY" << endl;
    test(phnumber);
    mutex1.unlock(); // End of CRITICAL SECTION 
    S[phnumber]->acquire();  // Decrement the semaphore of the philosopher - wait for permission
}


// Function for a philosopher to put down forks and begin thinking
void put_fork(int phnumber) {

    mutex1.lock(); // Begin of CRITICAL SECTION 
    auto [LEFT, RIGHT] = left_right(phnumber, N);
    state[phnumber] = THINKING;
    cout << "Philosopher " << phnumber + 1 << " putting fork " << LEFT + 1 << " and " << phnumber + 1 << " down" << endl;
    cout << "Philosopher " << phnumber + 1 << " is THINKING" << endl;

    // Test neighbors to see if they can now eat
    test(LEFT);
    test(RIGHT);
    mutex1.unlock(); // End of CRITICAL SECTION 
}


// Function representing philosopher process
void philosopher(int phnumber) {

    while (true) {
        sleep_for(seconds(2));
        take_fork(phnumber);
        sleep_for(seconds(2));
        put_fork(phnumber);
    }
}

int main() {

    cout << "Input number of philosophers: ";
    cin >> N;

    // Memory allocation
    state = new int[N];
    S = new counting_semaphore<1>*[N];
    threads = new thread[N];

    // Initialize philosophers and semaphores
    for (int i = 0; i < N; i++) {
        state[i] = THINKING;
        S[i] = new counting_semaphore<1>(0);  
        threads[i] = thread(philosopher, i);
        cout << "Philosopher " << i + 1 << " is THINKING" << endl;
    }

    // Wait for all philosopher threads to finish (never happens)
    for (int i = 0; i < N; i++) {
        threads[i].join();
    }

    // Free allocate memory
    for (int i = 0; i < N; i++) {
        delete S[i];
    }
    delete[] S;
    delete[] state;
    delete[] threads;

    return 0;
}
