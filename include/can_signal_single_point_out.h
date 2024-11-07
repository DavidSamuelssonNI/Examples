#ifndef BC6F93F8_2B09_48F6_ACFA_6BE95AA69C95
#define BC6F93F8_2B09_48F6_ACFA_6BE95AA69C95

#include "example_support.h"
#include <nixnet.h>                // Include file for NI-XNET functions and constants
#include <stdlib.h>                // Include file for various functions
#include <stdio.h>                 // Include file for printf
#include <ctype.h>     // Include file for Sleep, _getch
#include <nixnet.h>                // Include file for NI-XNET functions and constants
#include "session.h"


// Declare all functions

extern void DisplayErrorAndExit(nxStatus_t Status, char *Source);
extern void PrintSessionInfo();
extern nxSessionRef_t* CreateSession(nxMode_t nxMode);


#endif /* BC6F93F8_2B09_48F6_ACFA_6BE95AA69C95 */
