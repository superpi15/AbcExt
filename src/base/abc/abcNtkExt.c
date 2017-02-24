/**CFile****************************************************************

  FileName    [abcNtk.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [Network and node package.]

  Synopsis    [Network creation/duplication/deletion procedures.]

  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - June 20, 2005.]

  Revision    [$Id: abcNtk.c,v 1.00 2005/06/20 00:00:00 alanmi Exp $]

***********************************************************************/

#include "base/abc/abc.h"
#include "base/abc/abcInt.h"
#include "base/main/main.h"
#include "map/mio/mio.h"
#include "aig/gia/gia.h"

#ifdef ABC_USE_CUDD
#include "bdd/extrab/extraBdd.h"
#endif

ABC_NAMESPACE_IMPL_START


////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

static void Abc_NtkFixNonDrivenNetsExt( Abc_Ntk_t * pNtk );
////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [Finalizes the network using the existing network as a model.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Abc_NtkFinalizeReadExt( Abc_Ntk_t * pNtk )
{
    Abc_Obj_t * pBox, * pObj, * pTerm, * pNet;
    int i;
    if ( Abc_NtkHasBlackbox(pNtk) && Abc_NtkBoxNum(pNtk) == 0 )
    {
        pBox = Abc_NtkCreateBlackbox(pNtk);
        Abc_NtkForEachPi( pNtk, pObj, i )
        {
            pTerm = Abc_NtkCreateBi(pNtk);
            Abc_ObjAddFanin( pTerm, Abc_ObjFanout0(pObj) );
            Abc_ObjAddFanin( pBox, pTerm );
        }
        Abc_NtkForEachPo( pNtk, pObj, i )
        {
            pTerm = Abc_NtkCreateBo(pNtk);
            Abc_ObjAddFanin( pTerm, pBox );
            Abc_ObjAddFanin( Abc_ObjFanin0(pObj), pTerm );
        }
        return;
    }
    assert( Abc_NtkIsNetlist(pNtk) );

    // check if constant 0 net is used
    pNet = Abc_NtkFindNet( pNtk, "1\'b0" );
    if ( pNet )
    {
        if ( Abc_ObjFanoutNum(pNet) == 0 )
            Abc_NtkDeleteObj(pNet);
        else if ( Abc_ObjFaninNum(pNet) == 0 )
            Abc_ObjAddFanin( pNet, Abc_NtkCreateNodeConst0(pNtk) );
    }
    // check if constant 1 net is used
    pNet = Abc_NtkFindNet( pNtk, "1\'b1" );
    if ( pNet )
    {
        if ( Abc_ObjFanoutNum(pNet) == 0 )
            Abc_NtkDeleteObj(pNet);
        else if ( Abc_ObjFaninNum(pNet) == 0 )
            Abc_ObjAddFanin( pNet, Abc_NtkCreateNodeConst1(pNtk) );
    }
    // fix the net drivers
    //Abc_NtkFixNonDrivenNetsExt( pNtk );  //Not in ECO

    // reorder the CI/COs to PI/POs first
    Abc_NtkOrderCisCos( pNtk );
}

/**Function*************************************************************

  Synopsis    [Reads the verilog file.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Abc_NtkFixNonDrivenNetsExt( Abc_Ntk_t * pNtk )
{ 
    Vec_Ptr_t * vNets;
    Abc_Obj_t * pNet, * pNode;
    int i;

    if ( Abc_NtkNodeNum(pNtk) == 0 && Abc_NtkBoxNum(pNtk) == 0 )
        return;

    // special case
    pNet = Abc_NtkFindNet( pNtk, "[_c1_]" );
    if ( pNet != NULL )
    {
        pNode = Abc_NtkCreateNodeConst1( pNtk );
        Abc_ObjAddFanin( pNet, pNode );
    }

    // check for non-driven nets
    vNets = Vec_PtrAlloc( 100 );
    Abc_NtkForEachNet( pNtk, pNet, i )
    {
        if ( Abc_ObjFaninNum(pNet) > 0 )
            continue;
        // add the constant 0 driver
        pNode = Abc_NtkCreateNodeConst0( pNtk );
        // add the fanout net
        Abc_ObjAddFanin( pNet, pNode );
        // add the net to those for which the warning will be printed
        Vec_PtrPush( vNets, pNet );
    }

    // print the warning
    if ( vNets->nSize > 0 )
    {
        printf( "Warning: Constant-0 drivers added to %d non-driven nets in network \"%s\":\n", Vec_PtrSize(vNets), pNtk->pName );
        Vec_PtrForEachEntry( Abc_Obj_t *, vNets, pNet, i )
        {
            printf( "%s%s", (i? ", ": ""), Abc_ObjName(pNet) );
            if( Abc_ObjFanoutNum(pNet)==0 )
              printf("(zo)");
            //if ( i == 3 )\
            {\
                if ( Vec_PtrSize(vNets) > 3 )\
                    printf( " ..." );\
                break;\
            }
        }
        printf( "\n" );
    }
    Vec_PtrFree( vNets );
}

/**Function*************************************************************

  Synopsis    [Finalizes the network using the existing network as a model.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Abc_NtkFinalizeExt( Abc_Ntk_t * pNtk, Abc_Ntk_t * pNtkNew )
{
    Abc_Obj_t * pObj, * pDriver, * pDriverNew;
    int i;
    // set the COs of the strashed network
    Abc_NtkForEachCo( pNtk, pObj, i )
    {
        if ( Abc_ObjFaninNum(Abc_ObjFanin0(pObj)) != 0 ){
            pDriver    = Abc_ObjFanin0Ntk( Abc_ObjFanin0(pObj) );
            pDriverNew = Abc_ObjNotCond(pDriver->pCopy, Abc_ObjFaninC0(pObj));
            Abc_ObjAddFanin( pObj->pCopy, pDriverNew );
        } else {
            Abc_ObjAddFanin( pObj->pCopy, Abc_ObjFanin0(pObj)->pCopy );
        }
    }
    // duplicate timing manager
    if ( pNtk->pManTime )
        Abc_NtkTimeInitialize( pNtkNew, pNtk );
    if ( pNtk->vPhases )
        Abc_NtkTransferPhases( pNtkNew, pNtk );
    if ( pNtk->pWLoadUsed )
        pNtkNew->pWLoadUsed = Abc_UtilStrsav( pNtk->pWLoadUsed );
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


ABC_NAMESPACE_IMPL_END

