/**CFile****************************************************************

  FileName    [ver_.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [Verilog parser.]

  Synopsis    [Parses several flavors of structural Verilog.]

  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - August 19, 2006.]

  Revision    [$Id: ver_.c,v 1.00 2006/08/19 00:00:00 alanmi Exp $]

***********************************************************************/

#include "verExt.h"
#include "base/abc/abc.h"

ABC_NAMESPACE_IMPL_START


////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

VerExt_Des_t * VerExt_DesAlloc()
{
  VerExt_Des_t * pNew = ABC_ALLOC( VerExt_Des_t, 1 );
  pNew->pNtk = NULL;
  pNew->pNtkLogic = NULL;
  pNew->pNtkStrash = NULL;
  return pNew;
}

/**Function*************************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

void VerExt_DesDelete( VerExt_Des_t * pDes )
{
  if( pDes->pNtk )
    Abc_NtkDelete( pDes->pNtk );
  if( pDes->pNtkLogic )
    Abc_NtkDelete( pDes->pNtkLogic );
  if( pDes->pNtkStrash )
    Abc_NtkDelete( pDes->pNtkStrash );
  ABC_FREE( pDes );
}
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


ABC_NAMESPACE_IMPL_END

