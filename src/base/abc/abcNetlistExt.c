/**CFile****************************************************************

  FileName    [abcNetlist.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [Network and node package.]

  Synopsis    [Transforms netlist into a logic network and vice versa.]

  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - June 20, 2005.]

  Revision    [$Id: abcNetlist.c,v 1.00 2005/06/20 00:00:00 alanmi Exp $]

***********************************************************************/

#include "base/abc/abc.h"
#include "base/main/main.h"
//#include "seq.h"

ABC_NAMESPACE_IMPL_START

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

Abc_Ntk_t * Abc_NtkToLogicExt( Abc_Ntk_t * pNtk, int fDeFloating, int FloatingSig );
Abc_Ntk_t * Abc_NtkToLogicDefloating( Abc_Ntk_t * pNtk );

/**Function*************************************************************

  Synopsis    [Transform the netlist into a logic network.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
Abc_Ntk_t * Abc_NtkToLogicExt( Abc_Ntk_t * pNtk, int fDeFloating, int FloatingSig )
{
    Abc_Ntk_t * pNtkNew; 
    Abc_Obj_t * pObj, * pFanin;
    int i, k;
    // consider the case of the AIG
    if ( Abc_NtkIsStrash(pNtk) )
        return Abc_NtkToLogic( pNtk );
    assert( Abc_NtkIsNetlist(pNtk) );
    // consider simple case when there is hierarchy
//    assert( pNtk->pDesign == NULL );
    assert( Abc_NtkWhiteboxNum(pNtk) == 0 );
    assert( Abc_NtkBlackboxNum(pNtk) == 0 );
    assert( FloatingSig == 0 || FloatingSig == 1 );
    // start the network
    pNtkNew = Abc_NtkStartFrom( pNtk, ABC_NTK_LOGIC, pNtk->ntkFunc );
    // duplicate the nodes 
    Abc_NtkForEachNode( pNtk, pObj, i )
    {
        Abc_NtkDupObj(pNtkNew, pObj, 0);
        Abc_ObjAssignName( pObj->pCopy, Abc_ObjName(Abc_ObjFanout0(pObj)), NULL );
    }
    // reconnect the internal nodes in the new network
    Abc_NtkForEachNode( pNtk, pObj, i )
        Abc_ObjForEachFanin( pObj, pFanin, k ){
            if ( Abc_ObjFaninNum(pFanin)==0 && fDeFloating )
                Abc_ObjAddFanin( pObj->pCopy, (FloatingSig == 0)? Abc_NtkCreateNodeConst0(pNtkNew): Abc_NtkCreateNodeConst1(pNtkNew) );
            else
                Abc_ObjAddFanin( pObj->pCopy, Abc_ObjFanin0(pFanin)->pCopy );
        }
    // collect the CO nodes
    Abc_NtkFinalize( pNtk, pNtkNew );
    // fix the problem with CO pointing directly to CIs
    Abc_NtkLogicMakeSimpleCos( pNtkNew, 0 );
    // duplicate EXDC 
    if ( pNtk->pExdc )
        pNtkNew->pExdc = Abc_NtkToLogic( pNtk->pExdc );
    if ( !Abc_NtkCheck( pNtkNew ) )
        fprintf( stdout, "Abc_NtkToLogic(): Network check has failed.\n" );
    return pNtkNew;
}

/**Function*************************************************************

  Synopsis    [Transform the netlist into a logic network, and set floating signal to inputs]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
Abc_Ntk_t * Abc_NtkToLogicDefloating( Abc_Ntk_t * pNtk )
{
    Abc_Ntk_t * pNtkNew; 
    Abc_Obj_t * pObj, * pFanin;
    int i, k;
    // consider the case of the AIG
    if ( Abc_NtkIsStrash(pNtk) )
        return Abc_NtkToLogic( pNtk );
    assert( Abc_NtkIsNetlist(pNtk) );
    // consider simple case when there is hierarchy
//    assert( pNtk->pDesign == NULL );
    assert( Abc_NtkWhiteboxNum(pNtk) == 0 );
    assert( Abc_NtkBlackboxNum(pNtk) == 0 );
    // start the network
    pNtkNew = Abc_NtkStartFrom( pNtk, ABC_NTK_LOGIC, pNtk->ntkFunc );
    // duplicate the nodes 
    Abc_NtkForEachNode( pNtk, pObj, i )
    {
        Abc_NtkDupObj(pNtkNew, pObj, 0);
        Abc_ObjAssignName( pObj->pCopy, Abc_ObjName(Abc_ObjFanout0(pObj)), NULL );
    }

    // reconnect the internal nodes in the new network
    Abc_NtkForEachNode( pNtk, pObj, i )
        Abc_ObjForEachFanin( pObj, pFanin, k ){
            assert ( Abc_ObjIsNet(pFanin) );
            if ( Abc_ObjFaninNum(pFanin) != 0 )
                Abc_ObjAddFanin( pObj->pCopy, Abc_ObjFanin0(pFanin)->pCopy );
            else
                Abc_ObjAddFanin( pObj->pCopy, Abc_NtkCreatePi( pNtkNew ) );
        }
    // collect the CO nodes
    Abc_NtkFinalize( pNtk, pNtkNew );
    // fix the problem with CO pointing directly to CIs
    Abc_NtkLogicMakeSimpleCos( pNtkNew, 0 );
    // duplicate EXDC 
    if ( pNtk->pExdc )
        pNtkNew->pExdc = Abc_NtkToLogic( pNtk->pExdc );
    if ( !Abc_NtkCheck( pNtkNew ) )
        fprintf( stdout, "Abc_NtkToLogic(): Network check has failed.\n" );
    return pNtkNew;
}


////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


ABC_NAMESPACE_IMPL_END

