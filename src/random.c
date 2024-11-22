/* Include headers */
#include "ni_modelframework.h"
#include "model.h"
#include <stddef.h>

/* User defined datatypes and constants */
#define rtDBL	0

/* Pull in the parameters structure from codegen.c */
extern Parameters rtParameter[2];
extern int32_t READSIDE;

/* !!!! IMPORTANT !!!!
Accessing parameters values must be done through rtParameter[READSIDE]
The macro readParam is defined for you as a simple way to access parameters
!!!! IMPORTANT !!!! */
#define readParam rtParameter[READSIDE]

/* INPUT: ptr, base address of where value should be set.
INPUT: subindex, offset into ptr where value should be set.
INPUT: value, the value to be set
INPUT: type, the user defined type of the parameter being set, as defined in
rtParamAttributes
RETURN: status, NI_ERROR on error, NI_OK otherwise */
int32_t USER_SetValueByDataType(void* ptr, int32_t subindex, double value, int32_t type)
{
	switch (type) {
	case rtDBL:
		((double *)ptr)[subindex] = (double)value;
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
	case rtDBL:
		return (double)(((double *)ptr)[subindex]);
	}
	{	/* return NaN, ok for vxworks and pharlap */
		uint32_t nan[2] = { 0xFFFFFFFF, 0xFFFFFFFF };
		return *(double*)nan;
	}
}

/*
// When a model has parameters of the form: "modelname/parameter", these model parameters are considered global parameters (target scoped) in VeriStand
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
int32_t ParameterSize DataSection(".NIVS.paramlistsize") = 2;
NI_Parameter rtParamAttribs[] DataSection(".NIVS.paramlist") = {
	{ 0, "random/Amplitude", offsetof(Parameters, Amp), rtDBL, 1, 2, 0, 0 },
	{ 0, "random/Bias", offsetof(Parameters, Bias), rtDBL, 1, 2, 2, 0 },
};
int32_t ParamDimList[] DataSection(".NIVS.paramdimlist") =
{
	1, 1,                                /* random/Amplitude */
	1, 1,                                /* random/Bias */
};

/* Initialize parameters */
Parameters initParams DataSection(".NIVS.defaultparams") = { 1.2, 0.8 };

/*
This data structure is used to retrieve the size, width, and datatype of the default parameters.

ParamSizeWidth Parameters_sizes[] DataSection(".NIVS.defaultparamsizes") = {
{ sizeof(initParams), 0, 0},  The first element in this array uses only the first field in the typedef.  It is used to specify the size of the default parameters structure.
{ sizeof(double), 1, 0 }, Subsequent elements in the array use all 3 fields, they are: the size (num of bytes per element), the width (num of elements) (2x2 array would have 4 elements), and the datatype of each parameter
};  */
ParamSizeWidth Parameters_sizes[] DataSection(".NIVS.defaultparamsizes") = {
	{ sizeof(initParams) },
	{ sizeof(double), 1, rtDBL },
	{ sizeof(double), 1, rtDBL },
};

/* Define signal attributes */
int32_t SignalSize DataSection(".NIVS.siglistsize") = 0;
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


/* Define IO attributes */
int32_t ExtIOSize DataSection(".NIVS.extlistsize") = 1;
int32_t InportSize = 0;
int32_t OutportSize = 1;
NI_ExternalIO rtIOAttribs[] DataSection(".NIVS.extlist") = {
	{ 0, "Out1", 0, 1, 1, 1, 1 },
	{ -1 },
};

/* Model name and build information */
const char * USER_ModelName DataSection(".NIVS.compiledmodelname") = "random";
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
	if (outData)
		outData[0] = rand() * readParam.Amp / RAND_MAX + readParam.Bias;
	
	return NI_OK;
}

/* RETURN: status, NI_ERROR on error, NI_OK otherwise */
int32_t USER_Finalize() {
	return NI_OK;
}

int32_t USER_ModelStart() {
	return NI_OK;
}