#include "session.h"


    unsigned int i = 0;
    char *l_pSelectedInterface = "CAN1";
    char *l_pSelectedDatabase = "NIXNET_example";
    char *l_pSelectedCluster = "CAN_Cluster";
    char *l_pSelectedSignalList = "CANEventSignal1,CANEventSignal2";
    nxStatus_t l_Status = 0;
    static nxSessionRef_t m_SessionRef = 0;

    //=============================================================================
    // Global functions declarations
    //=============================================================================

    void PrintSessionInfo(){
        printf("Interface: %s\nDatabase: %s\nSignal List: %s\n",
        l_pSelectedInterface, l_pSelectedDatabase, l_pSelectedSignalList);
    }

    void DisplayErrorAndExit(nxStatus_t Status, char *Source)
    {
    char l_StatusString[1024];
    nxStatusToString(Status, sizeof(l_StatusString), l_StatusString);

    printf("\n\nERROR at %s!\n%s\n", Source, l_StatusString);
    printf("\nExecution stopped.\nPress any key to quit\n");

    nxClear(m_SessionRef);

    _getch();
    exit(1);
    }


    nxSessionRef_t* CreateSession(nxMode_t nxMode){
        nxStatus_t l_Status = 0;
        // Create an XNET session in SignalOutSinglePoint mode
        l_Status = nxCreateSession(l_pSelectedDatabase, l_pSelectedCluster,
        l_pSelectedSignalList, l_pSelectedInterface, nxMode,
        &m_SessionRef);
        if (nxSuccess == l_Status)
        {
        printf("Session created successfully.\n");
        }
        else
        {
        DisplayErrorAndExit(l_Status, "nxCreateSession");
        }
        return &m_SessionRef;
    }