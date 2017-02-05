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
extern VerExt_Des_t * Io_ReadVerilogExt( char * pFileName, int fCheck );
extern void Io_WriteVerilog( Abc_Ntk_t * pNtk, char * pFileName );
extern void Io_WriteVerilogExt( VerExt_Des_t * pDes, char * pFileName );
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
	if ( pNtk->pDesign )
		printf("Multiple moddules using\n");
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
    Abc_Obj_t * pPo, * pPi, * pFanin, * pFanout, * pObj;
    int i,j,k;
	Abc_NtkForEachPi( pNtk, pPi, i ){
		Abc_ObjForEachFanout( pPi, pFanout, j ){
			printf("%d- %s %d\n",i,Abc_ObjName(pFanout),Abc_ObjFanoutNum(pFanout));
		}
	}
	Abc_NtkForEachPo( pNtk, pPo, i ){
		Abc_ObjForEachFanin( pPo, pFanin, j ){
			printf("%d- %s %d\n",i,Abc_ObjName(pFanin),Abc_ObjFaninNum(pFanin));
			//pObj= Abc_ObjFanin
			/**
			Abc_ObjForEachFanin( pFanin, pObj, k ){
				printf("%d- %s %d %s %s\n",k,Abc_ObjName(pObj),Abc_ObjFaninNum(pObj), Abc_ObjIsNode(pObj)?"y":"n",VerExt_GetGateName( pObj->GateType ) );

				Vec_Ptr_t * vNodes=Vec_PtrAlloc(100);
				Abc_NtkDfs_rec( pObj, vNodes );
			}/**/
		}/**/
		Vec_Ptr_t * vNodes = Vec_PtrAlloc(100);
		Abc_NtkDfs_rec( Abc_ObjFanin0(Abc_ObjFanin0(pPo)), vNodes );
		Vec_PtrForEachEntry( Abc_Obj_t* , vNodes, pObj, j ){
			if ( Abc_ObjIsNode(pObj) ){
				printf("\t %s %s",VerExt_GetGateName( pObj->GateType ),"");
	            fprintf( stdout, " ( %s", Abc_ObjName(Abc_ObjFanout0(pObj)) );
	            Abc_ObjForEachFanin( pObj, pFanin, k )
	                fprintf( stdout, ", %s(%s)", Abc_ObjName(pFanin), Abc_ObjIsNet(pFanin)?"Net":"?" );

            	fprintf( stdout, "); \n" );
			}
		}
		Vec_PtrFree( vNodes );
		printf("\n");/**/
	}

	if( argc>=2 ) Io_WriteVerilogExt( pDes, argv[2] );
	//if( argc>=2 ) Io_WriteVerilog( pNtk, argv[2] );

	//Abc_NtkDelete( pNtk );
	VerExt_DesDelete( pDes );
	Abc_Stop();
}