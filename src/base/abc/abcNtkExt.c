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
    //Abc_NtkFixNonDrivenNets( pNtk );  //Not in ECO

    // reorder the CI/COs to PI/POs first
    Abc_NtkOrderCisCos( pNtk );
}
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


ABC_NAMESPACE_IMPL_END

