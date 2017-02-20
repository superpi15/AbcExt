
#ifndef ABC__base__ver__verExt_h
#define ABC__base__ver__verExt_h

#include "base/abc/abc.h"
#include "base/ver/ver.h"


////////////////////////////////////////////////////////////////////////
///                         PARAMETERS                               ///
////////////////////////////////////////////////////////////////////////



ABC_NAMESPACE_HEADER_START

// types of verilog gates
typedef enum { 
	VER_GATE_UDF = 0,
    VER_GATE_AND,
    VER_GATE_OR,
    VER_GATE_XOR,
    VER_GATE_BUF,
    VER_GATE_NAND,
    VER_GATE_NOR,
    VER_GATE_XNOR,
    VER_GATE_NOT
} Ver_GateType_t;

static inline char * VerExt_GetGateName ( Ver_GateType_t GateType ){
	if( GateType==VER_GATE_AND ) return "and";
	if( GateType==VER_GATE_OR ) return "or";
	if( GateType==VER_GATE_XOR ) return "xor";
	if( GateType==VER_GATE_BUF ) return "buf";
	if( GateType==VER_GATE_NAND ) return "nand";
	if( GateType==VER_GATE_NOR ) return "nor";
	if( GateType==VER_GATE_XNOR ) return "xnor";
	if( GateType==VER_GATE_NOT ) return "not";
	return "(null)";
}

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////

typedef struct VerExt_Des_t_		VerExt_Des_t;

struct VerExt_Des_t_ {
	Abc_Ntk_t * pNtk;			//Verilog Design
	Abc_Ntk_t * pNtkLogic;		//Logic Network
	Abc_Ntk_t * pNtkStrash;		//Structural Hashed AIG
};

////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////

extern VerExt_Des_t * VerExt_DesAlloc();
extern void VerExt_DesDelete( VerExt_Des_t * pDes );
/*=== verParseExt.c ========================================================*/

extern char *         VerExt_ParseGetName( Ver_Man_t * p );

ABC_NAMESPACE_HEADER_END



#endif

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
