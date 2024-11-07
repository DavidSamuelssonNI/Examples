
#include "can_signal_single_point_out.h"


int main(){
    PrintSessionInfo();
    nxSessionRef_t* can_single_out_session = CreateSession(nxMode_SignalOutSinglePoint);
    return 0;
}
