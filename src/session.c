#include "session.h"


    unsigned int i = 0;
    // char *l_pSelectedInterface_out = "CAN1";
    // char *l_pSelectedInterface_in = "CAN2";

    char *l_pSelectedDatabase = "NIXNET_example";
    char *l_pSelectedCluster = "CAN_Cluster";
    char *l_pSelectedSignalList = "CANEventSignal1,CANEventSignal2";
    nxStatus_t l_Status = 0;
    // static nxSessionRef_t m_SessionRef = 0;



    //=============================================================================
    // Global functions declarations
    //=============================================================================

    // void PrintSessionInfo(){
    //     printf("Interface: %s\nDatabase: %s\nSignal List: %s\n",
    //     l_pSelectedInterface, l_pSelectedDatabase, l_pSelectedSignalList);
    // }

    void DisplayErrorAndExit(nxStatus_t Status, char *Source, nxSessionRef_t m_SessionRef)
    {
    char l_StatusString[1024];
    nxStatusToString(Status, sizeof(l_StatusString), l_StatusString);

    printf("\n\nERROR at %s!\n%s\n", Source, l_StatusString);
    printf("\nExecution stopped.\nPress any key to quit\n");

    nxClear(m_SessionRef);

    _getch();
    exit(1);
    }

nxSessionRef_t* CreateSession(nxMode_t nxMode, char *l_pSelectedInterface) {
    nxStatus_t l_Status = 0;
    nxSessionRef_t* m_SessionRef = (nxSessionRef_t*)malloc(sizeof(nxSessionRef_t));
    if (m_SessionRef == NULL) {
        printf("Failed to allocate memory for session reference.\n");
        return NULL;
    }
    
    // Create an XNET session in the specified mode
    l_Status = nxCreateSession(l_pSelectedDatabase, l_pSelectedCluster,
                               l_pSelectedSignalList, l_pSelectedInterface, nxMode,
                               m_SessionRef);
    if (nxSuccess == l_Status) {
        printf("Session created successfully. Session Ref: %ld %s\n", *m_SessionRef, l_pSelectedInterface);
    } else {
        DisplayErrorAndExit(l_Status, "nxCreateSession", *m_SessionRef);
        free(m_SessionRef);  // Free memory if session creation fails
        return NULL;
    }
    
    return m_SessionRef;
}