#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "CAN_Input.h"
#include "CAN_Output.h"

std::mutex mtx;
std::condition_variable cv;
bool outputDone = false;
bool inputDone = false;

void runOutput(CANSignalSinglePointOutput& bpa) {
    while (true) {
        {
            std::unique_lock<std::mutex> lock(mtx);
            outputDone = false;
            inputDone = false;
        }

        bpa.run(); // Perform output task

        {
            std::unique_lock<std::mutex> lock(mtx);
            outputDone = true;
            cv.notify_one(); // Notify input thread
        }

        // Wait for input to complete
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [] { return inputDone; });
        }
    }
}

void runInput(CANSignalSinglePointInput& apa) {
    while (true) {
        // Wait for output to complete
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [] { return outputDone; });
        }

        apa.run(); // Perform input task

        {
            std::unique_lock<std::mutex> lock(mtx);
            inputDone = true;
            cv.notify_one(); // Notify output thread
        }
    }
}


int main() {
    CANSignalSinglePointInput apa =  CANSignalSinglePointInput("CAN2", "NIXNET_example", "CAN_Cluster", {"CANEventSignal1", "CANEventSignal2"});
    CANSignalSinglePointOutput bpa = CANSignalSinglePointOutput("CAN1", "NIXNET_example", "CAN_Cluster", {"CANEventSignal1", "CANEventSignal2"});


    std::thread outputThread(runOutput, std::ref(bpa));
    std::thread inputThread(runInput, std::ref(apa));
    outputThread.join();
    inputThread.join();

    std::cin.get();

    return 0;
}