#include <misc/util/abc_global.h>
#include <sat/bsat/satSolver.h>
#include "base/io/ioAbc.h"
////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

// procedures to start and stop the ABC framework
// (should be called before and after the ABC procedures are called)
extern void   Abc_Start();
extern void   Abc_Stop();

// procedures to get the ABC framework and execute commands in it
extern void * Abc_FrameGetGlobalFrame();
extern Abc_Ntk_t * Io_ReadVerilogExt( char * pFileName, int fCheck );
int main( int argc, char** argv ){
	Abc_Start();
	Abc_Ntk_t * pNtk= Io_ReadVerilogExt( argv[1], 1 );
	Abc_Stop();
}