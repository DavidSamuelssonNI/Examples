#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>  // For tolower
#include "session.h"

pthread_mutex_t mutex;
pthread_cond_t cond;
int condition_met = 0;

void* CanSingleSignalOut(void* arg) {
    nxSessionRef_t* can_single_out_session = CreateSession(nxMode_SignalOutSinglePoint, "CAN1");
    nxStatus_t l_Status = 0;
    printf("Press any key to transmit new signal values or q to quit\n");
    unsigned int i = 0;
    f64 l_ValueBuffer[NUM_SIGNALS];

    while ('q' != tolower(_getch())) {
        l_ValueBuffer[0] = (f64)i;
        l_ValueBuffer[1] = (f64)(i * 10);

        // Update the signal data
        l_Status = nxWriteSignalSinglePoint(*can_single_out_session, l_ValueBuffer, sizeof(l_ValueBuffer));
        if (nxSuccess == l_Status) {
            printf("Signal values updated successfully.\n");
            printf("Signals sent:\n");
            printf("Signal 1: %f\n", l_ValueBuffer[0]);
            printf("Signal 2: %f\n\n", l_ValueBuffer[1]);
            if (++i > 10) {
                i = 0;
            }

            // Signal the read thread to read the updated values
            pthread_mutex_lock(&mutex);
            pthread_cond_signal(&cond);  // Signal the condition variable
            pthread_mutex_unlock(&mutex);
        } else {
            DisplayErrorAndExit(l_Status, "nxWriteSignalSinglePoint", *can_single_out_session);
        }
        i++;
    }

    // Clear the XNET session
    l_Status = nxClear(*can_single_out_session);
    if (nxSuccess == l_Status) {
        printf("Session cleared successfully!\n");
    } else {
        DisplayErrorAndExit(l_Status, "nxClear", *can_single_out_session);
    }

    // Signal main thread to exit
    pthread_mutex_lock(&mutex);
    condition_met++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* CanSingleSignalIn(void* arg) {
    nxSessionRef_t* can_single_in_session = CreateSession(nxMode_SignalInSinglePoint, "CAN2");
    // nxSessionRef_t can_single_in_session;
    nxStatus_t l_Status = 0;
    nxTimestamp_t l_TimestampBuffer[NUM_SIGNALS];
    f64 l_ValueBuffer[NUM_SIGNALS];

    if (nxSuccess == l_Status)
    {
        printf("Session created successfully.\n");
    }
    else
    {
        DisplayErrorAndExit(l_Status, "nxCreateSession", *can_single_in_session);
    }


    while (1) {
        // pthread_mutex_lock(&mutex);
        // pthread_cond_wait(&cond, &mutex);  // Wait for signal from writer
        // pthread_mutex_unlock(&mutex);

        // Attempt to read signals
        l_Status = nxReadSignalSinglePoint(*can_single_in_session, l_ValueBuffer,
         sizeof(l_ValueBuffer), l_TimestampBuffer, sizeof(l_TimestampBuffer));
        if (nxSuccess == l_Status) {
            printf("Signals received:\n");
            printf("Signal 1: %f\n", l_ValueBuffer[0]);
            printf("Signal 2: %f\n\n", l_ValueBuffer[1]);
        } else {
            DisplayErrorAndExit(l_Status, "nxReadSignalSinglePoint", *can_single_in_session);
        }
        usleep(1000000);
    }

    // Clear the XNET session
    l_Status = nxClear(*can_single_in_session);
    if (nxSuccess == l_Status) {
        printf("Session cleared successfully!\n");
    } else {
        DisplayErrorAndExit(l_Status, "nxClear", *can_single_in_session);
    }

    pthread_mutex_lock(&mutex);
    condition_met++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    // PrintSessionInfo();
    
    pthread_t can_out_thread, can_in_thread;

    // Initialize the mutex and condition variable
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // Create the reader thread
    if (pthread_create(&can_in_thread, NULL, CanSingleSignalIn, NULL) != 0) {
        perror("Failed to create read thread");
        return 1;
    }

    // // Create the writer thread
    // if (pthread_create(&can_out_thread, NULL, CanSingleSignalOut, NULL) != 0) {
    //     perror("Failed to create write thread");
    //     return 1;
    // }

    // Wait for both threads to signal completion
    pthread_mutex_lock(&mutex);
    while (condition_met < 2) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    // Clean up resources
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    printf("Both threads have finished execution, exiting main thread\n");
    return 0;
}
