/* Include headers */
#include "ni_modelframework.h"
#include "model.h"
#include <stddef.h>

/* User defined datatypes and constants */
#define rtINT	2
#define MAX_DELAY 10000

/* Pull in the parameters structure from codegen.c */
extern Parameters rtParameter[2];
extern int32_t READSIDE;

/* !!!! IMPORTANT !!!!
   Accessing parameters values must be done through rtParameter[READSIDE]
   The macro readParam is defined for you as a simple way to access parameters
   !!!! IMPORTANT !!!! */
#define readParam rtParameter[READSIDE]

/* Define IO and Signals structs */
typedef struct {
	double In1;                          
} Inports;

typedef struct {
  	double Out1;
} Outports;

/* Instantiate IO and states */
Inports rtInport;
Outports rtOutport;
double rtState[MAX_DELAY];

/* INPUT: ptr, base address of where value should be set.
   INPUT: subindex, offset into ptr where value should be set.
   INPUT: value, the value to be set
   INPUT: type, the user defined type of the parameter being set, as defined in
  		  rtParamAttributes
   RETURN: status, NI_ERROR on error, NI_OK otherwise */
int32_t USER_SetValueByDataType(void* ptr, int32_t subindex, double value, int32_t type)
{
	switch (type) {
	case rtINT: 
    	((int32_t *)ptr)[subindex] = (int32_t)value;
    	return NI_OK;
	}
  	return NI_ERROR;
}

/* INPUT: ptr, base address of value to be retrieved.
   INPUT: subindex, offset into ptr where value should be retrieved.
   INPUT: type, the user defined type of the parameter or signal being 
  		  retrieved, as defined in rtParamAttributes or rtSignalAttributes
   RETURN: value of user-defined type cast to a double */
double USER_GetValueByDataType(void* ptr, int32_t subindex, int32_t type)
{
	switch (type) {
	case rtINT:
		return (double)(((int32_t *)ptr)[subindex]);
  	}
  	{	/* return NaN, ok for vxworks and pharlap */
	  	uint32_t nan[2] = {0xFFFFFFFF, 0xFFFFFFFF};
		return *(double*)nan;
	}
}
/*
// When a model has parameters of the form: "modelname/parameter", these model parameters are considered global paramters (target scoped) in VeriStand
// When a model has parameters of the form: "modelname/block/paramter" these model parameters are NOT considered global parameters (model scoped) in VeriStand
typedef struct {
  int32_t idx;			// not used
  char* paramname;	// name of the parameter, e.g., "Amplitude"
  uintptr_t addr;// offset of the parameter in the Parameters struct
  int32_t datatype;		// integer describing a user defined datatype. must have a corresponding entry in GetValueByDataType and SetValueByDataType
  int32_t width;		// size of parameter
  int32_t numofdims; 	// number of dimensions
  int32_t dimListOffset;// offset into dimensions array
  int32_t IsComplex;	// not used
} NI_Parameter;

*/

/* Define parameter attributes */
int32_t ParameterSize DataSection(".NIVS.paramlistsize") = 1;
NI_Parameter rtParamAttribs[] DataSection(".NIVS.paramlist") = {
  	{ 0, "delay/z-block/Order", offsetof(Parameters, delay), rtINT, 1, 2, 0, 0},
};
int32_t ParamDimList[] DataSection(".NIVS.paramdimlist") =
{
  1, 1,
};

/* Initialize parameters */
Parameters initParams DataSection(".NIVS.defaultparams") = {100.0};

/*
   This data structure is used to retrieve the size, width, and datatype of the default parameters.
      
   ParamSizeWidth Parameters_sizes[] DataSection(".NIVS.defaultparamsizes") = {
    { sizeof(initParams), 0, 0},  The first element in this array uses only the first field in the typedef.  It is used to specify the size of the default parameters structure.
    { sizeof(double), 1, 0 }, Subsequent elements in the array use all 3 fields, they are: the size (num of bytes per element), the width (num of elements) (2x2 array would have 4 elements), and the datatype of each parameter
   };  */
ParamSizeWidth Parameters_sizes[] DataSection(".NIVS.defaultparamsizes") = {
  { sizeof(initParams)},
  { sizeof(int32_t), 1, rtINT },
};

/* Define signal attributes */
int32_t SignalSize DataSection(".NIVS.siglistsize") = 0;
/* we still have to create this entity, even though it is not used. */
NI_Signal rtSignalAttribs[1];
int32_t SigDimList[1];

/*
typedef struct {
  int32_t	idx;	// not used
  char*	name;	// name of the external IO, e.g., "In1"
  int32_t	TID;	// = 0
  int32_t   type; 	// Ext Input: 0, Ext Output: 1
  int32_t  width; 	// not used
  int32_t	dimX;	// 1st dimension size
  int32_t	dimY; 	// 2nd dimension size
} NI_ExternalIO;
*/

/* Define I/O attributes */
int32_t ExtIOSize DataSection(".NIVS.extlistsize") = 2;
int32_t InportSize = 1;
int32_t OutportSize = 1;
NI_ExternalIO rtIOAttribs[] DataSection(".NIVS.extlist") = {
	{ 0, "In1", 0, 0, 1, 1, 1},
	{ 0, "Out1", 0, 1, 1, 1, 1},
	{ -1 },
};


/* Model name and build information */
const char * USER_ModelName DataSection(".NIVS.compiledmodelname") = "delay";
const char * USER_Builder DataSection(".NIVS.builder") = "VeriStand Custom DLL";

/* baserate is the rate at which the model runs */
double USER_BaseRate = .01;

/*
typedef struct {
  int32_t    tid;		// = 0
  double tstep;		
  double offset;
  int32_t priority;
} NI_Task;
*/

NI_Task rtTaskAttribs DataSection(".NIVS.tasklist") = { 0 /* must be 0 */, .01 /* must be equal to baserate */, 0, 0 };

/* RETURN: status, NI_ERROR on error, NI_OK otherwise */
int32_t USER_Initialize() {
	return NI_OK;
}

/* INPUT: *inData, pointer to inport data at the current timestamp, to be 
  	      consumed by the function
   OUTPUT: *outData, pointer to outport data at current time + baserate, to be
  	       produced by the function
   INPUT: timestamp, current simulation time */
int32_t USER_TakeOneStep(double *inData, double *outData, double timestamp) 
{
	int32_t valid_delay, idx;
	
	for (idx = MAX_DELAY - 1; idx > 0; idx--)
		rtState[idx] = rtState[idx - 1];

	if (inData) {
		rtInport.In1 = inData[0];
		rtState[0] = rtInport.In1;	
	}
							
	if (outData) {
		if (readParam.delay >= MAX_DELAY)
			valid_delay = MAX_DELAY - 1;
		else
			valid_delay = readParam.delay;
			
		outData[0] = rtState[valid_delay];	
	}
	
	return NI_OK;
}

/* RETURN: status, NI_ERROR on error, NI_OK otherwise */
int32_t USER_Finalize() {
	return NI_OK;
}

/* Non-supported API */

extern struct { 
	int32_t stopExecutionFlag;
	const char *errmsg;
	void* flip;
	uint32_t inCriticalSection;
	int32_t SetParamTxStatus;
	double timestamp;
} NIRT_system;

DLL_EXPORT int32_t NIRT_GetSimState(int32_t* numContStates, char* contStatesNames, double* contStates, int32_t* numDiscStates, char* discStatesNames, double* discStates, int32_t* numClockTicks, char* clockTicksNames, int32_t* clockTicks) 
{
	if (numContStates && numDiscStates && numClockTicks) {
		if (*numContStates < 0 || *numDiscStates < 0 || *numClockTicks < 0) {
			*numContStates = 0;
			*numDiscStates = MAX_DELAY;
			*numClockTicks = 1;
			return NI_OK;
		}
	}
	
	if (discStates && discStatesNames) {
		int32_t idx = 0;
		while (idx < MAX_DELAY) {
			discStates[idx] = rtState[idx];
			strcpy(discStatesNames + (idx++ * 100), "RTSTATE");
		}		
	}
	
	if (clockTicks && clockTicksNames) {
		clockTicks[0] = NIRT_system.timestamp;
		strcpy(clockTicksNames, "clockTick0");
	}	
	return NI_OK;
}

DLL_EXPORT int32_t NIRT_SetSimState(double* contStates, double* discStates, int32_t* clockTicks)
{
	if (discStates) {
		int32_t idx = 0;
		while (idx < MAX_DELAY)
			rtState[idx] = discStates[idx++];
	}
	
	if (clockTicks) {
		NIRT_system.timestamp = clockTicks[0];
	}	
	return NI_OK;
}

int32_t USER_ModelStart() {
	return NI_OK;
}