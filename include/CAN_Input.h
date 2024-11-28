#ifndef CBD34458_03C0_4007_92A3_9DFF17F855A8
#define CBD34458_03C0_4007_92A3_9DFF17F855A8

#include <nixnet.h>
#include <string>
#include <vector>


class CANSignalSinglePointInput {
public:
    CANSignalSinglePointInput(const std::string& interface, const std::string& database, const std::string& cluster, const std::vector<std::string>& signals);
    ~CANSignalSinglePointInput();

    void run();
    void displayErrorAndExit(nxStatus_t status, const std::string& source);

private:
    nxSessionRef_t sessionRef;
    std::string selectedInterface;
    std::string selectedDatabase;
    std::string selectedCluster;
    std::vector<std::string> selectedSignalList;
    std::vector<f64> valueBuffer;
    std::vector<nxTimestamp_t> timestampBuffer;

    void createInputSession();
    void clearSession();
};

#endif /* CBD34458_03C0_4007_92A3_9DFF17F855A8 */
