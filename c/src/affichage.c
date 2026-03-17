#include <stdio.h>
#include <math.h>
#include <time.h>

/* GL/glew.h must be included before SDL2/SDL.h */
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "affichage.h"
#include "particule.h"
#include "contexte.h"
#include "logo.h"

/* ------------------------------------------------------------------ */
/* Low-level CPU surface drawing (particles still rendered on CPU)      */
/* ------------------------------------------------------------------ */

void putPixel(int x, int y, int couleur, bool force, SDL_Surface *surface)
{
    if (x < 1 || y < 1 || x >= surface->w - 1 || y >= surface->h - 1)
        return;
    if (couleur < 0)   couleur = 0;
    if (couleur > 255) couleur = 255;

    uint8_t *offscreen = (uint8_t *)surface->pixels;
    int offset = y * surface->pitch + x;
    if (force || offscreen[offset] < couleur)
        offscreen[offset] = couleur;
}

void blur(int x1, int y1, int x2, int y2, SDL_Surface *surface)
{
    int pitch = surface->pitch;
    int offsetY = y1 * pitch;
    uint8_t *VScreen = (uint8_t *)surface->pixels;

    for (int y = y1; y < y2; y++, offsetY += pitch)
    {
        for (int x = x1; x < x2; x++)
        {
            int resultat =
                VScreen[offsetY - pitch + x] +
                VScreen[offsetY + pitch + x] +
                VScreen[offsetY + x + 1]     +
                VScreen[offsetY + x - 1];
            VScreen[offsetY + x] = resultat >> 2;
        }
    }
}

void drawParticule(SDL_Surface *VScreen, bool *Sprites[MAX_TAILLE], PARTICULE *particule)
{
    for (PARTICULE *p = particule; p != NULL; p = p->suivant)
        p->Drawer(p, VScreen, Sprites);
}

/* ------------------------------------------------------------------ */
/* Logo drawing                                                         */
/* ------------------------------------------------------------------ */

void afficherLogoTopRight(SDL_Surface *VScreen)
{
    afficherLogo(VScreen->w - LOGO_W, 0, VScreen);
}

void afficherLogoTile(SDL_Surface *VScreen)
{
    int nbX = VScreen->w / LOGO_W + 1;
    int nbY = VScreen->h / LOGO_H + 1;

    SDL_FillRect(VScreen, NULL, 0);
    for (int incX = 0; incX < nbX; incX++)
        for (int incY = 0; incY < nbY; incY++)
            afficherLogo(incX * LOGO_W + incX * 10,
                         incY * LOGO_H + incY * 10, VScreen);
}

void afficherLogoCenter(SDL_Surface *VScreen)
{
    SDL_FillRect(VScreen, NULL, 0);
    afficherLogo((VScreen->w / 2) - (LOGO_W / 2),
                 (VScreen->h / 2) - (LOGO_H / 2), VScreen);
}

void afficherLogo(int x, int y, SDL_Surface *VScreen)
{
    for (int incX = 0; incX < LOGO_W; incX++)
        for (int incY = 0; incY < LOGO_H; incY++)
            if (c_logo_map[incY * LOGO_W + incX] != 0x00)
                putPixel(incX + x, incY + y, 255, false, VScreen);
}

/* ------------------------------------------------------------------ */
/* HUD helpers (draw onto the RGBA32 hud surface)                       */
/* ------------------------------------------------------------------ */

void afficherPaletteHUD(CONTEXTE *contexte)
{
    SDL_LockSurface(contexte->hud);
    uint32_t *pixels  = (uint32_t *)contexte->hud->pixels;
    int       pitch_p = contexte->hud->pitch / 4; /* pitch in pixels */

    for (int incY = 0; incY < contexte->hauteur; incY++)
    {
        int idx = incY * 256 / contexte->hauteur;
        SDL_Color c = contexte->listeCouleur[idx];
        uint32_t px = SDL_MapRGBA(contexte->hud->format, c.r, c.g, c.b, 255);
        for (int incX = 0; incX < 40; incX++)
            pixels[incY * pitch_p + incX] = px;
    }
    SDL_UnlockSurface(contexte->hud);
}

static void afficherTexteHUD(const char *texte, CONTEXTE *contexte,
                              int x, int y)
{
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *ts = TTF_RenderUTF8_Blended(contexte->font, texte, white);
    if (!ts) return;

    SDL_Rect dst = {x, y, ts->w, ts->h};
    SDL_BlitSurface(ts, NULL, contexte->hud, &dst);
    SDL_FreeSurface(ts);
}

void afficherBoardHUD(CONTEXTE *contexte, clock_t t1, clock_t t2, long nbFrame)
{
    char texte[100];
    long clk_tck = CLOCKS_PER_SEC;

    static double cumul   = 0.0;
    static long   nbAcc   = 0;

    double tick = (double)(t2 - t1) / (double)clk_tck;
    double fps  = (t2 - t1) == 0 ? 1000.0
                                  : 1000.0 / ((double)(t2 - t1) * 1000.0 / (double)clk_tck);

    if (nbFrame == 1) { cumul = 0.0; nbAcc = 0; }
    cumul += fps;
    nbAcc++;
    double moyenne = cumul / nbAcc;

    snprintf(texte, sizeof(texte), "Particules : %d", contexte->nombreMeteor);
    afficherTexteHUD(texte, contexte, 100, 0);

    snprintf(texte, sizeof(texte), "FPS : %ld  moy: %.1lf", (long)fps, moyenne);
    afficherTexteHUD(texte, contexte, 100, 25);

    (void)tick;
}

/* ------------------------------------------------------------------ */
/* GPU render                                                           */
/* ------------------------------------------------------------------ */

void rendreGPU(CONTEXTE *contexte, int mouseX, int mouseY,
               clock_t t1, clock_t t2, long nbFrame)
{
    int w = contexte->largeur;
    int h = contexte->hauteur;

    /* --- Upload heightmap (bump) every frame --- */
    glBindTexture(GL_TEXTURE_2D, contexte->texHeightmap);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, contexte->bump->pitch);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h,
                    GL_RED, GL_UNSIGNED_BYTE, contexte->bump->pixels);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    /* --- Upload cache if mode switched --- */
    if (contexte->cacheTextureNeedsUpdate)
    {
        glBindTexture(GL_TEXTURE_2D, contexte->texCache);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, contexte->cache->pitch);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h,
                        GL_RED, GL_UNSIGNED_BYTE, contexte->cache->pixels);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        contexte->cacheTextureNeedsUpdate = false;
    }

    /* --- Choose program --- */
    int progIdx;
    switch (contexte->mode)
    {
        case LIGHT:  progIdx = 1; break;
        case SHADOW: progIdx = 2; break;
        case GLASS:  progIdx = 3; break;
        default:     progIdx = 0; break; /* FIREBALL */
    }
    GLuint prog = contexte->programs[progIdx];
    glUseProgram(prog);

    /* Bind heightmap to unit 0 */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, contexte->texHeightmap);
    glUniform1i(glGetUniformLocation(prog, "uHeightmap"), 0);

    /* Bind palette to unit 1 */
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, contexte->texPalette);
    glUniform1i(glGetUniformLocation(prog, "uPalette"), 1);

    /* Mode-specific uniforms */
    float tsX = 1.0f / (float)w;
    float tsY = 1.0f / (float)h;

    if (contexte->mode == LIGHT)
    {
        int offsetInt     = rand() % 20;
        float palOff      = (float)offsetInt / 255.0f;
        float lightRadius = (float)(w < h ? w : h) / 2.0f;

        glUniform2f(glGetUniformLocation(prog, "uTexelSize"),    tsX, tsY);
        glUniform2f(glGetUniformLocation(prog, "uLightPos"),     (float)mouseX, (float)mouseY);
        glUniform1f(glGetUniformLocation(prog, "uLightRadius"),  lightRadius);
        glUniform1f(glGetUniformLocation(prog, "uPaletteOffset"), palOff);
    }
    else if (contexte->mode == SHADOW)
    {
        int   offsetInt  = rand() % 20 + 1;  /* 1-20, never 0 */
        float palOff     = (float)offsetInt / 255.0f;
        float calcOff    = 1.0f / (float)offsetInt;
        float lightRadius = (float)(w < h ? w : h) / 2.0f;

        glUniform2f(glGetUniformLocation(prog, "uTexelSize"),    tsX, tsY);
        glUniform2f(glGetUniformLocation(prog, "uLightPos"),     (float)mouseX, (float)mouseY);
        glUniform1f(glGetUniformLocation(prog, "uLightRadius"),  lightRadius);
        glUniform1f(glGetUniformLocation(prog, "uPaletteOffset"), palOff);
        glUniform1f(glGetUniformLocation(prog, "uCalcOffset"),   calcOff);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, contexte->texCache);
        glUniform1i(glGetUniformLocation(prog, "uCache"), 2);
    }
    else if (contexte->mode == GLASS)
    {
        glUniform2f(glGetUniformLocation(prog, "uTexelSize"), tsX, tsY);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, contexte->texCache);
        glUniform1i(glGetUniformLocation(prog, "uCache"), 2);
    }

    /* Draw fullscreen quad */
    glBindVertexArray(contexte->quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    /* --- HUD pass (text / palette bar) --- */
    if (contexte->drawBoard || contexte->drawPalette)
    {
        SDL_FillRect(contexte->hud, NULL, 0);

        if (contexte->drawPalette) afficherPaletteHUD(contexte);
        if (contexte->drawBoard)   afficherBoardHUD(contexte, t1, t2, nbFrame);

        glBindTexture(GL_TEXTURE_2D, contexte->texHUD);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, contexte->hud->pitch / 4);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h,
                        GL_RGBA, GL_UNSIGNED_BYTE, contexte->hud->pixels);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

        GLuint hudProg = contexte->programs[4];
        glUseProgram(hudProg);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, contexte->texHUD);
        glUniform1i(glGetUniformLocation(hudProg, "uHUD"), 0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDisable(GL_BLEND);
    }
}
