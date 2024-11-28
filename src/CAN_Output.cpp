#include "CAN_Output.h"
#include "getch.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <ctype.h>

CANSignalSinglePointOutput::CANSignalSinglePointOutput(const std::string& interface, const std::string& database, const std::string& cluster, const std::vector<std::string>& signals)
    : sessionRef(0), selectedInterface(interface), selectedDatabase(database), selectedCluster(cluster), selectedSignalList(signals), valueBuffer(signals.size()) {
    value_ = 0;
    MyModel my_model = MyModel();
    frequency_ = 10;
    createOutputSession();
}

CANSignalSinglePointOutput::~CANSignalSinglePointOutput() {
    clearSession();
}

void CANSignalSinglePointOutput::createOutputSession() {
    std::string signalList = selectedSignalList[0];
    for (size_t i = 1; i < selectedSignalList.size(); ++i) {
        signalList += "," + selectedSignalList[i];
    }

    nxStatus_t status = nxCreateSession(selectedDatabase.c_str(), selectedCluster.c_str(), signalList.c_str(), selectedInterface.c_str(), nxMode_SignalOutSinglePoint, &sessionRef);
    if (status == nxSuccess) {
        //std::cout << "Session created successfully." << std::endl;
    } else {
        displayErrorAndExit(status, "nxCreateSession");
    }
}

void CANSignalSinglePointOutput::clearSession() {
    nxStatus_t status = nxClear(sessionRef);
    if (status == nxSuccess) {
        std::cout << "Session cleared successfully!" << std::endl;
    } else {
        displayErrorAndExit(status, "nxClear");
    }
    
}

void CANSignalSinglePointOutput::run() {

    // value_ = 0;
        
        auto now = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        int timeInMs = static_cast<int>(ms);
        valueBuffer[0] = static_cast<f64>(value_);
        //valueBuffer[1] = static_cast<f64>(value_ * 10);
        int model_value = my_model.output(timeInMs, frequency_); 
        valueBuffer[1] = static_cast<f64>(model_value);
        //std::cout << "Out: " << value_ << std::endl;
        nxStatus_t status = nxWriteSignalSinglePoint(sessionRef, valueBuffer.data(), valueBuffer.size() * sizeof(f64));
        if (status == nxSuccess) {

            std::cout <<  valueBuffer[0] << " "<< valueBuffer[1] <<std::endl;

            if (++value_ > 10) {
                value_ = 0;
            }
        } else {
            displayErrorAndExit(status, "nxWriteSignalSinglePoint");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void CANSignalSinglePointOutput::displayErrorAndExit(nxStatus_t status, const std::string& source) {
    char statusString[1024];
    nxStatusToString(status, sizeof(statusString), statusString);

    std::cerr << "\n\nERROR at " << source << "!\n" << statusString << std::endl;
    std::cerr << "\nExecution stopped.\nPress any key to quit" << std::endl;

    clearSession();

    _getch();
    exit(1);
}