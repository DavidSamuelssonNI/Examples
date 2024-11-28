#ifndef CAN_SIGNAL_SINGLE_POINT_OUTPUT_H
#define CAN_SIGNAL_SINGLE_POINT_OUTPUT_H

#include <nixnet.h>
#include <string>
#include <vector>


class CANSignalSinglePointOutput {
public:
    CANSignalSinglePointOutput(const std::string& interface, const std::string& database, const std::string& cluster, const std::vector<std::string>& signals);
    ~CANSignalSinglePointOutput();

    void run();
    void displayErrorAndExit(nxStatus_t status, const std::string& source);

private:
    nxSessionRef_t sessionRef;
    std::string selectedInterface;
    std::string selectedDatabase;
    std::string selectedCluster;
    std::vector<std::string> selectedSignalList;
    std::vector<f64> valueBuffer;
    int value_;

    void createOutputSession();
    void clearSession();
};

#endif // CAN_SIGNAL_SINGLE_POINT_OUTPUT_H