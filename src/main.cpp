#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "CAN_Input.h"
#include "CAN_Output.h"

std::mutex mtx;
std::condition_variable cv;
bool outputDone = false;

void runOutput(CANSignalSinglePointOutput& bpa) {
    std::cout << "Output" << std::endl;
    bpa.run();
    {
        std::lock_guard<std::mutex> lock(mtx);
        outputDone = true; // Set outputDone to true
    }
    cv.notify_one(); // Notify the waiting thread
}

void runInput(CANSignalSinglePointInput& apa) {
    std::cout << "Input" << std::endl;
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []{ return outputDone; }); // Wait until outputDone is true
    apa.run();
}

int main() {
    CANSignalSinglePointInput apa =  CANSignalSinglePointInput("CAN2", "NIXNET_example", "CAN_Cluster", {"CANEventSignal1", "CANEventSignal2"});
    CANSignalSinglePointOutput bpa = CANSignalSinglePointOutput("CAN1", "NIXNET_example", "CAN_Cluster", {"CANEventSignal1", "CANEventSignal2"});

    std::thread outputThread(runOutput, std::ref(bpa));
    std::thread inputThread(runInput, std::ref(apa));

    outputThread.join();
    inputThread.join();

    return 0;
}