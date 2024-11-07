
// #include "can_signal_single_point_out.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "session.h"

pthread_mutex_t mutex;
pthread_cond_t cond;
int condition_met = 0;

void* threadFunction(void* arg) {
    int threadNumber = *((int*)arg);  // Cast argument to int pointer and dereference
    printf("Thread %d is running\n", threadNumber);
    sleep(1);  // Simulate some work with sleep
    printf("Thread %d is finishing\n", threadNumber);

        // Signal the main thread that the condition is met
    pthread_mutex_lock(&mutex);
    condition_met++;  // Increment the condition variable
    pthread_cond_signal(&cond);  // Signal the condition variable
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* CanSingleSignalOut(void* arg){

    nxSessionRef_t* can_single_out_session = CreateSession(nxMode_SignalOutSinglePoint);
    nxStatus_t l_Status = 0;
    printf("Press any key to transmit new signal values or q to quit\n");
    unsigned int i = 0;
    f64 l_ValueBuffer[NUM_SIGNALS];
    while ('q' != tolower(_getch()))
    {
        l_ValueBuffer[0] = (f64)i;
        l_ValueBuffer[1] = (f64)(i * 10);

        // Update the signal data
        l_Status = nxWriteSignalSinglePoint(*can_single_out_session, l_ValueBuffer,
            sizeof(l_ValueBuffer));
        if (nxSuccess == l_Status)
        {
            printf("Signal values updated successfully.\n");
                printf("Signals sent:\n");
                printf("Signal 1: %f\n", l_ValueBuffer[0]);
                printf("Signal 2: %f\n\n", l_ValueBuffer[1]);
                if (++i > 10)
                {
                    i = 0;
                }
        }
        else
        {
            DisplayErrorAndExit(l_Status, "nxWriteSignalSinglePoint");
        }
        i++;
    }

    // Clear the XNET session
   l_Status = nxClear(*can_single_out_session);
   if (nxSuccess == l_Status)
   {
      printf("Session cleared successfully!\n");
   }
   else
   {
      DisplayErrorAndExit(l_Status, "nxClear");
   }

    // Signal the main thread that the condition is met
    pthread_mutex_lock(&mutex);
    condition_met++;  // Increment the condition variable
    pthread_cond_signal(&cond);  // Signal the condition variable
    pthread_mutex_unlock(&mutex);

return NULL;

}

int main(){
    PrintSessionInfo();
    // nxSessionRef_t* can_single_out_session = CreateSession(nxMode_SignalOutSinglePoint);
    
    pthread_t can_out_thread, thread2;
    int can_out_thread_num = 1;
    int threadNum2 = 2;

  // Initialize the mutex and condition variable
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // Create the first thread in detached mode
    if (pthread_create(&can_out_thread, NULL, CanSingleSignalOut, &can_out_thread_num) != 0) {
        perror("Failed to create thread 1");
        return 1;
    }

    // Detach the first thread
    pthread_detach(can_out_thread);

    // Create the second thread in detached mode
    if (pthread_create(&thread2, NULL, threadFunction, &threadNum2) != 0) {
        perror("Failed to create thread 2");
        return 1;
    }

    // Detach the second thread
    pthread_detach(thread2);

    // Wait for the condition variable to be signaled before quitting
    pthread_mutex_lock(&mutex);
    while (condition_met < 2) {  // Wait for both threads to signal completion
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    // Clean up resources
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    printf("Both threads have finished execution, exiting main thread\n");
    return 0;
}
