/**CFile****************************************************************

  FileName    [verParse.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [Verilog parser.]

  Synopsis    [Performs some Verilog parsing tasks.]

  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - August 19, 2006.]

  Revision    [$Id: verParse.c,v 1.00 2006/08/19 00:00:00 alanmi Exp $]

***********************************************************************/

#include "base/ver/ver.h"
#include "base/ver/verExt.h"

ABC_NAMESPACE_IMPL_START


////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [Parses a Verilog name that can be being with a slash.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
char * VerExt_ParseGetName( Ver_Man_t * pMan )
{
    Ver_Stream_t * p = pMan->pReader;
    char Symbol;
    char * pWord;
    pMan->fNameLast = 0;
    if ( !Ver_StreamIsOkey(p) )
        return NULL;
    if ( !Ver_ParseSkipComments( pMan ) )
        return NULL;
    Symbol = Ver_StreamScanChar( p );
    if ( Symbol == '\\' )
    {
        pMan->fNameLast = 1;
        //Ver_StreamPopChar( p );
        pWord = Ver_StreamGetWord( p, " \r\n" );
        Ver_StreamSkipChars( p, " \r\n" );
        if ( Ver_StreamScanChar(p) == '[' )
        {
            char This, * pEnd = pWord + strlen( pWord );
            while ( (This = Ver_StreamPopChar(p)) != ']' )
                *pEnd++ = This;
            *pEnd++ = This;
            *pEnd = 0;
        }
    }
    else
        pWord = Ver_StreamGetWord( p, " \t\n\r(),;" );
    if ( Ver_StreamIsOkey(p) && !Ver_ParseSkipComments( pMan ) )
        return NULL;
    return pWord;
}


////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


ABC_NAMESPACE_IMPL_END

