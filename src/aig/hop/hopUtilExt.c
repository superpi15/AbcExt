/**CFile****************************************************************

  FileName    [hopUtil.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [And-Inverter Graph package.]

  Synopsis    [Various procedures.]

  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - May 11, 2006.]

  Revision    [$Id: hopUtil.c,v 1.00 2006/05/11 00:00:00 alanmi Exp $]

***********************************************************************/

#include "aig/hop/hop.h"

ABC_NAMESPACE_IMPL_START


////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [Prints Verilog formula for the AIG rooted at this node.]

  Description [The formula is in terms of PIs, which should have
  their names assigned in pObj->pData fields.]
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Hop_ObjPrintVerilogExt( FILE * pFile, Hop_Obj_t * pObj, Vec_Vec_t * vLevels, int Level )
{
    Vec_Ptr_t * vSuper;
    Hop_Obj_t * pFanin, * pFanin0, * pFanin1, * pFaninC;
    int fCompl, i;
    // store the complemented attribute
    fCompl = Hop_IsComplement(pObj);
    pObj = Hop_Regular(pObj);
    // constant case
    if ( Hop_ObjIsConst1(pObj) )
    {
        fprintf( pFile, "1\'b%d", !fCompl );
        return;
    }
    // PI case
    if ( Hop_ObjIsPi(pObj) )
    {
        fprintf( pFile, "%s%s", fCompl? "~" : "", (char*)pObj->pData );
        return;
    }
    // EXOR case
    if ( Hop_ObjIsExor(pObj) )
    {
        Vec_VecExpand( vLevels, Level );
        vSuper = Vec_VecEntry( vLevels, Level );
        Hop_ObjCollectMulti( pObj, vSuper );
        fprintf( pFile, "%s", (Level==0? "" : "(") );
        Vec_PtrForEachEntry( Hop_Obj_t *, vSuper, pFanin, i )
        {
            Hop_ObjPrintVerilog( pFile, Hop_NotCond(pFanin, (fCompl && i==0)), vLevels, Level+1 );
            if ( i < Vec_PtrSize(vSuper) - 1 )
                fprintf( pFile, " ^ " );
        }
        fprintf( pFile, "%s", (Level==0? "" : ")") );
        return;
    }
    // MUX case
    if ( Hop_ObjIsMuxType(pObj) )
    {
        if ( Hop_ObjRecognizeExor( pObj, &pFanin0, &pFanin1 ) )
        {
            fprintf( pFile, "%s", (Level==0? "" : "(") );
            Hop_ObjPrintVerilog( pFile, Hop_NotCond(pFanin0, fCompl), vLevels, Level+1 );
            fprintf( pFile, " ^ " );
            Hop_ObjPrintVerilog( pFile, pFanin1, vLevels, Level+1 );
            fprintf( pFile, "%s", (Level==0? "" : ")") );
        }
        else 
        {
            pFaninC = Hop_ObjRecognizeMux( pObj, &pFanin1, &pFanin0 );
            fprintf( pFile, "%s", (Level==0? "" : "(") );
            Hop_ObjPrintVerilog( pFile, pFaninC, vLevels, Level+1 );
            fprintf( pFile, " ? " );
            Hop_ObjPrintVerilog( pFile, Hop_NotCond(pFanin1, fCompl), vLevels, Level+1 );
            fprintf( pFile, " : " );
            Hop_ObjPrintVerilog( pFile, Hop_NotCond(pFanin0, fCompl), vLevels, Level+1 );
            fprintf( pFile, "%s", (Level==0? "" : ")") );
        }
        return;
    }
    // AND case
    Vec_VecExpand( vLevels, Level );
    vSuper = Vec_VecEntry(vLevels, Level);
    Hop_ObjCollectMulti( pObj, vSuper );
    fprintf( pFile, "%s", (Level==0? "" : "(") );
    Vec_PtrForEachEntry( Hop_Obj_t *, vSuper, pFanin, i )
    {
        Hop_ObjPrintVerilog( pFile, Hop_NotCond(pFanin, fCompl), vLevels, Level+1 );
        if ( i < Vec_PtrSize(vSuper) - 1 )
            fprintf( pFile, " %s ", fCompl? "|" : "&" );
    }
    fprintf( pFile, "%s", (Level==0? "" : ")") );
    
    return;
}



////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


ABC_NAMESPACE_IMPL_END

