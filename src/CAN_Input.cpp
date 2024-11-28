#include "CAN_Input.h"
#include "getch.h"
#include <iostream>
#include <ctype.h>

CANSignalSinglePointInput::CANSignalSinglePointInput(const std::string& interface, const std::string& database, const std::string& cluster, const std::vector<std::string>& signals)
    : sessionRef(0), selectedInterface(interface), selectedDatabase(database), selectedCluster(cluster), selectedSignalList(signals), valueBuffer(signals.size()), timestampBuffer(signals.size()) {
    createInputSession();
}

CANSignalSinglePointInput::~CANSignalSinglePointInput() {
    clearSession();
}

void CANSignalSinglePointInput::createInputSession() {
    std::string signalList = selectedSignalList[0];
    for (size_t i = 1; i < selectedSignalList.size(); ++i) {
        signalList += "," + selectedSignalList[i];
    }

    nxStatus_t status = nxCreateSession(selectedDatabase.c_str(), selectedCluster.c_str(), signalList.c_str(), selectedInterface.c_str(), nxMode_SignalInSinglePoint, &sessionRef);
    if (status == nxSuccess) {
        std::cout << "Session created successfully." << std::endl;
    } else {
        displayErrorAndExit(status, "nxCreateSession");
    }
}

void CANSignalSinglePointInput::clearSession() {
    nxStatus_t status = nxClear(sessionRef);
    if (status == nxSuccess) {
        std::cout << "Session cleared successfully!" << std::endl;
    } else {
        displayErrorAndExit(status, "nxClear");
    }
}

void CANSignalSinglePointInput::run() {
    // std::cout << "Press any key to read the latest signal values or q to quit" << std::endl;

    while (1) {
        nxStatus_t status = nxReadSignalSinglePoint(sessionRef, valueBuffer.data(), valueBuffer.size() * sizeof(f64), timestampBuffer.data(), timestampBuffer.size() * sizeof(nxTimestamp_t));
        if (status == nxSuccess) {
            std::cout << "Signals received:" << std::endl;
            for (size_t i = 0; i < valueBuffer.size(); ++i) {
                std::cout << "Signal " << i + 1 << ": " << valueBuffer[i] << std::endl;
            }
            std::cout << std::endl;
        } else {
            displayErrorAndExit(status, "nxReadSignalSinglePoint");
        }
    }
}

void CANSignalSinglePointInput::displayErrorAndExit(nxStatus_t status, const std::string& source) {
    char statusString[1024];
    nxStatusToString(status, sizeof(statusString), statusString);

    std::cerr << "\n\nERROR at " << source << "!\n" << statusString << std::endl;
    std::cerr << "\nExecution stopped.\nPress any key to quit" << std::endl;

    clearSession();

    _getch();
    exit(1);
}