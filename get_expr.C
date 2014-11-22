// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/RCS/get_expr.C,v 1.1 2000/09/15 10:15:33 mhoffman Exp $

#include "string.h"
#include "get_expr.h"

const char * get_expr(const char * pos, char * expr, char upto)
{   const char * old = pos;

  if ( (pos = strchr(old,upto)) == NULL ) {	// end of string reached
    return (NULL);
  }
  while ( old != pos ) {			// copy the expression
    *expr++ = *old++;				// without the 'upto'
  }
  *expr = '\0';					// termination
  return (pos+1);				// skip the 'upto'
}

const char * get_expr(const char * pos, char * expr, char * upto)
{   const char * old = pos;

  if ( (pos = strpbrk(old,upto)) == NULL ) {	// end of string reached
    return (NULL);
  }
  while ( old != pos ) {			// copy the expression
    *expr++ = *old++;				// without the 'upto'
  }
  *expr = '\0';					// termination
  return (pos+1);				// skip the 'upto'
}

// eof
