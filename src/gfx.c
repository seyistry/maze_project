/*
 * ===================================================
 * ===================================================
 *                SDL Graphics Wrapper
 *                     Revision 1
 *
 *            (C) 2015  Nick Stones-Havas
 *
 * ===================================================
 *
 *  C library for drawing to the screen.
 *
 * ===================================================
 * ===================================================
 */
#include "../inc/gfx.h"
#include <stdio.h>

/* Used to identify a texture structure in memory */
#define TEX_TAG 0x55AA

static unsigned int screenWidth = -1;
static unsigned int screenHeight = -1;

ManagedTexture_ *managedTextures = NULL;

/* SDL Stuff */
SDL_Window *window = NULL;
SDL_Renderer *renderer;

/*========================================================
 * GFX management functions
 *========================================================
 */

int initGFX(char *title, unsigned int width, unsigned int height) {
  if (window || renderer)
    return 0;

  screenWidth = width;
  screenHeight = height;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    gfxSetError("Error initializing SDL", 1);
    return 0;
  }

  window = SDL_CreateWindow(title, 50, 50, width, height, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

  if (!window || !renderer) {
    gfxSetError("Could not create SDL window", 1);
    return 0;
  }

  SDL_SetWindowTitle(window, title);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  return 1;
}

void *createTexture(unsigned int width, unsigned int height) {
  Uint32 *data;
  ManagedTexture_ *newmtex;
  if (!width || !height || !renderer) {
    gfxSetError("SDL window has not been initialized yet", 0);
    return NULL;
  }

  newmtex = malloc(sizeof(ManagedTexture_));
  if (!newmtex) {
    return NULL;
  }
  newmtex->pitch = width * sizeof(Uint32);
  newmtex->next = NULL;
  newmtex->prev = NULL;
  newmtex->magicTag = TEX_TAG;

  newmtex->texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888,
                        SDL_TEXTUREACCESS_STREAMING, width, height);
  if (!(newmtex->texture)) {
    free(newmtex);
    gfxSetError("Could not create texture", 1);
    return NULL;
  }

  data = malloc((sizeof(Uint32) * width * height) + sizeof(ManagedTexture_ *));
  if (!data) {
    free(newmtex);
    return NULL;
  }

  /*
   * Hide a pointer to the managed struct before the actual pixel data.
   *
   * This allows the caller to only have to worry about the data they
   * want, while still allowing this library to perform more complicated
   * management tasks given only a reference to the pixel data.
   */
  *((ManagedTexture_ **)data) = newmtex;
  newmtex->pixelData = ((ManagedTexture_ **)data) + 1;

  /* Insert the item into the linked list */
  if (managedTextures) {
    newmtex->next = managedTextures;
    managedTextures->prev = newmtex;
  }
  managedTextures = newmtex;

  return newmtex->pixelData;
}

void displayFullscreenTexture(void *texture) {
  ManagedTexture_ *mtex;

  if (!window || !renderer) {
    gfxSetError("SDL window has not been initialized yet", 0);
    return;
  }

  /* Recover the managed texture structure */
  mtex = *(((ManagedTexture_ **)texture) - 1);

  /* Don't do anything if it's not actually a managed texture */
  if (mtex->magicTag != TEX_TAG) {
    gfxSetError("Not a valid texture pointer", 0);
    return;
  }

  SDL_UpdateTexture(mtex->texture, NULL, mtex->pixelData, mtex->pitch);

  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, mtex->texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

int destroyTexture(void *ptr) {
  /* Recover the memory management structure before freeing anything */
  ManagedTexture_ *mtex = *(((ManagedTexture_ **)ptr) - 1);

  /* Don't do anything if it's not actually a managed structure */
  if (mtex->magicTag != TEX_TAG) {
    gfxSetError("Not a valid texture pointer", 0);
    return 0;
  }

  /* Prevent this structure from being somehow sent here again */
  mtex->magicTag = 0;

  /* Actual cleanup */
  free(((ManagedTexture_ **)ptr) - 1);

  SDL_DestroyTexture(mtex->texture);

  if (mtex->prev)
    mtex->prev->next = mtex->next;
  if (mtex->next)
    mtex->next->prev = mtex->prev;
  if (managedTextures == mtex)
    managedTextures = mtex->next;
  free(mtex);

  return 1;
}

void destroyGFX() {
  /* Destroy all allocated textures */
  while (managedTextures)
    destroyTexture(managedTextures->pixelData);

  /* Clean everything else up */
  if (window && renderer) {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
  }
}
