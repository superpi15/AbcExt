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
extern void Io_WriteVerilog( Abc_Ntk_t * pNtk, char * pFileName );
extern Abc_Ntk_t * Abc_NtkStrash( Abc_Ntk_t *, int, int, int );
extern void Abc_NtkDelete( Abc_Ntk_t * pNtk );
int main( int argc, char** argv ){
	Abc_Start();
	Abc_Ntk_t * pNtk= Io_ReadVerilogExt( argv[1], 1 );
	//Abc_Ntk_t * pNtkRes = Abc_NtkStrash( pNtk, 0, 1, 0 );
	if( argc>=2 )
	Io_WriteVerilog( pNtk, argv[2] );
	Abc_NtkDelete( pNtk );
	Abc_Stop();
}