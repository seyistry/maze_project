#include "../inc/gfx.h"
#include <stdio.h>

/*========================================================
 * Library debug functions
 *========================================================
 */

/* Error string buffer */
static char errstr[256];

const char *gfxGetError() { return errstr; }

void gfxSetError(char *str, char showSdlErr) {
  if (showSdlErr)
    sprintf(errstr, "%s: %s", str, SDL_GetError());
  else
    sprintf(errstr, "%s", str);
}
