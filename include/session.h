#ifndef F3DCA39F_BFA3_43E9_8D61_8F9582A30DFF
#define F3DCA39F_BFA3_43E9_8D61_8F9582A30DFF

#include "example_support.h"
#include <nixnet.h>                // Include file for NI-XNET functions and constants
#include <stdlib.h>                // Include file for various functions
#include <stdio.h>                 // Include file for printf
#include <ctype.h>     // Include file for Sleep, _getch

#define NUM_SIGNALS 2
typedef unsigned int nxMode_t;


// Declare all variables for the function
extern unsigned int i;
extern char *l_pSelectedInterface;
extern char *l_pSelectedDatabase;
extern char *l_pSelectedCluster;
extern char *l_pSelectedSignalList;
extern f64 l_ValueBuffer[NUM_SIGNALS];

extern void DisplayErrorAndExit(nxStatus_t Status, char *Source);
extern void PrintSessionInfo();
extern nxSessionRef_t* CreateSession(nxMode_t nxMode);

#endif /* F3DCA39F_BFA3_43E9_8D61_8F9582A30DFF */
