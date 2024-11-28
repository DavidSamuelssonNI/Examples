#include "CAN_Output.h"
#include "getch.h"
#include <iostream>
#include <ctype.h>

CANSignalSinglePointOutput::CANSignalSinglePointOutput(const std::string& interface, const std::string& database, const std::string& cluster, const std::vector<std::string>& signals)
    : sessionRef(0), selectedInterface(interface), selectedDatabase(database), selectedCluster(cluster), selectedSignalList(signals), valueBuffer(signals.size()) {
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
        std::cout << "Session created successfully." << std::endl;
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
    //unsigned int i = 0;
    std::cout << "Press any key to transmit new signal values or q to quit" << std::endl;
    value_ = 0;
    while (1) {
        valueBuffer[0] = static_cast<f64>(value_);
        valueBuffer[1] = static_cast<f64>(value_ * 10);

        nxStatus_t status = nxWriteSignalSinglePoint(sessionRef, valueBuffer.data(), valueBuffer.size() * sizeof(f64));
        if (status == nxSuccess) {
            std::cout << "Signals sent:" << std::endl;
            for (size_t j = 0; j < valueBuffer.size(); ++j) {
                std::cout << "Signal " << j + 1 << ": " << valueBuffer[j] << std::endl;
            }
            std::cout << std::endl;
            if (++value_ > 10) {
                value_ = 0;
            }
        } else {
            displayErrorAndExit(status, "nxWriteSignalSinglePoint");
        }
    }
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