#include <misc/util/abc_global.h>
#include <sat/bsat/satSolver.h>
#include "base/io/ioAbc.h"
#include "base/ver/verExt.h"
////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

// procedures to start and stop the ABC framework
// (should be called before and after the ABC procedures are called)
extern void   Abc_Start();
extern void   Abc_Stop();

// procedures to get the ABC framework and execute commands in it
extern void * Abc_FrameGetGlobalFrame();
extern Abc_Ntk_t * Io_ReadVerilog( char * pFileName, int fCheck );
extern Abc_Ntk_t * Io_ReadVerilogExt( char * pFileName, int fCheck );
extern void Io_WriteVerilog( Abc_Ntk_t * pNtk, char * pFileName );
extern void Io_WriteVerilogExt( Abc_Ntk_t * pNtk, char * pFileName );
extern Abc_Ntk_t * Abc_NtkStrash( Abc_Ntk_t *, int, int, int );
extern void Abc_NtkDelete( Abc_Ntk_t * pNtk );

extern Abc_Ntk_t * Io_ReadNetlist( char * pFileName, Io_FileType_t FileType, int fCheck );
extern Abc_Ntk_t * Io_Read( char * pFileName, Io_FileType_t FileType, int fCheck, int fBarBufs );
extern Abc_Ntk_t * Abc_NtkToLogic( Abc_Ntk_t * pNtk );
extern Abc_Ntk_t * Abc_NtkToNetlist( Abc_Ntk_t * pNtk );
int main( int argc, char** argv ){
	Abc_Start();
	//Abc_Ntk_t * pNtk= Io_Read( argv[1], IO_FILE_VERILOG, 1 ,0);
	//Abc_Ntk_t * pNtk= Io_ReadNetlist( argv[1], IO_FILE_VERILOG, 1 );
	VerExt_Des_t * pDes = Io_ReadVerilogExt( argv[1], 0 );
	//Abc_Ntk_t * pNtk= Abc_NtkDup( pDes->pNtk );
	Abc_Ntk_t * pNtk= pDes->pNtk;
	//Abc_Ntk_t * pNtk= Io_ReadVerilog( argv[1], 1 );
	//pNtk = Abc_NtkToLogic( pNtk );
	//Abc_Ntk_t * pNtkRes = Abc_NtkStrash( pNtk, 0, 1, 0 );
	/**
    if ( Abc_NtkWhiteboxNum(pNtk) > 0 )
    {
        Abc_Ntk_t * pTemp;
        pNtk = Abc_NtkFlattenLogicHierarchy( pTemp = pNtk );
        Abc_NtkDelete( pTemp );
        if ( pNtk == NULL )
        {
            fprintf( stdout, "Flattening logic hierarchy has failed.\n" );
            return NULL;
        }
    }
    /**/
	//pNtk= Abc_NtkToLogic( pNtk );
	//pNtk= Abc_NtkToNetlist( pNtk );
	printf("%d %d\n", Abc_NtkHasAig(pNtk) , Abc_NtkHasMapping(pNtk) );
	printf("%d %d\n", Abc_NtkIsAigNetlist(pNtk) , Abc_NtkIsMappedNetlist(pNtk) );

	if( argc>=2 ) Io_WriteVerilogExt( pNtk, argv[2] );
	
	//Abc_NtkDelete( pNtk );
	VerExt_DesDelete( pDes );
	Abc_Stop();
}