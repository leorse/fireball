#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#define L 800
#define H 600
#define RANDOM(n) (((float)rand() / (float)RAND_MAX) * (n))
#define NB_BOULETTE 25
#define MAX_VITESSE 45
#define MAX_TAILLE 6
#define MAX_DUREE 25
#define PTX (L / 2)
#define PTY (H / 2)
#define PI 3.14159

int compteur = 0;
int poids = -5;
int vit = 3;
int dire = 60;
typedef struct
{
    float x;
    float y;
    float tps;
    float dir;
    int vitesse;
    int taille;
    int vie;
} METEOR;

METEOR arma[NB_BOULETTE];

void DoPalette(SDL_Color *Palette)
{
    int i;
    for (i = 0; i < 64; i++)
    {
        Palette[i].r = i;
        Palette[i].g = 0;
        Palette[i].b = 0;
    }
    for (i = 64; i < 128; i++)
    {
        Palette[i].r = 255;
        Palette[i].g = i - 256;
        Palette[i].b = 0;
    }
    for (i = 128; i < 256; i++)
    {
        Palette[i].r = 255;
        Palette[i].g = 255;
        Palette[i].b = (i - 258) >> 1;
    }
}

void Init_boulette(int boulette)
{
    arma[boulette].dir = 1+(rand() % 360);
    arma[boulette].vitesse = 25+(rand() % MAX_VITESSE);
    arma[boulette].taille = 1+(rand() % MAX_TAILLE);
    arma[boulette].tps = 0;
    arma[boulette].vie++;
}

void Init_boulettes(void)
{
    int i;
    for (i = 0; i < NB_BOULETTE; i++)
    {
        arma[i].vie = -1;
        Init_boulette(i);
    }
}

void Move_Boulette(void)
{
    int i;
    float temps;
    for (i = 0; i < NB_BOULETTE; i++)
    {
        arma[i].tps += 0.1;
        temps = arma[i].tps;
        //printf("vie:%d dir:%f cos de %f=%f\n",arma[i].vie,arma[i].dir,(arma[i].dir) * (180 / PI), cos((arma[i].dir) * (180 / PI)));
        arma[i].x = (double)(arma[i].vitesse * temps * cos((arma[i].dir) * (180 / PI))) + PTX;
        arma[i].y = (double)(arma[i].vitesse * temps * sin((arma[i].dir) * (180 / PI)) - (poids / 2) * (temps * temps)) + PTY;

        if (arma[i].y >= H || arma[i].x >= L || arma[i].x < 0 || arma[i].tps>=MAX_DUREE)
        {
            Init_boulette(i);
        }
    }
}

void Mem_Pixel(int x, int y, int couleur, SDL_Surface *surface)
{
    if (x < 0 || y < 0 || x >= L || y >= H)
    {
        return;
    }
    uint8_t *offscreen = (uint8_t *)surface->pixels;
    offscreen[y * L + x] = couleur;
}

void flou(int x1, int y1, int x2, int y2, SDL_Surface *surface)
{
    int x, y;
    int resultat;

    uint8_t *VScreen = (uint8_t *)surface->pixels;
    for (x = x1; x < x2; x++)
    {
        for (y = y1; y < y2; y++)
        {
            resultat = //VScreen[y * L + x]+
                VScreen[(y - 1) * L + x] +
                VScreen[(y + 1) * L + x] +
                VScreen[y * L + (x + 1)] +
                VScreen[y * L + (x - 1)];
            resultat = resultat / 4;
            VScreen[y * L + x] = resultat;
        }
    }
}

void Affiche_boulette(SDL_Surface *VScreen)
{
    int i;
    for (i = 0; i < NB_BOULETTE; i++)
    {
        int posX = arma[i].x;
        int posY = arma[i].y;
        int taille = arma[i].taille;
        int couleur =arma[i].tps>=MAX_DUREE?0:256-(arma[i].tps*256/MAX_DUREE);
        Mem_Pixel(posX, posY, couleur, VScreen);
        for(int incX=0;incX<taille;incX++)
        {
            for(int incY=0;incY<taille-incX;incY++)
            {
                Mem_Pixel(posX, posY+incY, couleur, VScreen);
                Mem_Pixel(posX, posY-incY, couleur, VScreen);
                Mem_Pixel(posX+incX, posY-incY, couleur, VScreen);
                Mem_Pixel(posX-incX, posY+incY, couleur, VScreen);
                Mem_Pixel(posX+incX, posY+incY, couleur, VScreen);
                Mem_Pixel(posX-incX, posY-incY, couleur, VScreen);
            }
        }
        /*
          0
         000
        00000
         000
          0
        */
    }
}

int main(int argv, char *argc[])
{
    SDL_Window *sdlWindow;
    SDL_Texture *sdlTexture;
    SDL_Event event;
    SDL_Rect r;
    SDL_Palette *palette;
    r.w = 100;
    r.h = 50;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 0;
    }

    if ((sdlWindow = SDL_CreateWindow("ma fenêtre", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, L, H, SDL_WINDOW_OPENGL)) == NULL)
    {
        printf("non ça veut pas créer!");
        return 0;
    }
    palette = SDL_AllocPalette(256);

    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
    SDL_Color listeCouleur[256];
    DoPalette(listeCouleur);

    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, L, H, 8, 0, 0, 0, 0);
    SDL_SetPaletteColors(surface->format->palette, listeCouleur, 0, 256);
    sdlTexture = SDL_CreateTextureFromSurface(sdlRenderer, surface);

    Init_boulettes();
    while (1)
    {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
            break;

        Move_Boulette();
        Affiche_boulette(surface);
        flou(1, 1, L - 1, H - 1, surface);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(sdlRenderer, surface);
        SDL_RenderCopy(sdlRenderer, texture, NULL, NULL);
        SDL_RenderPresent(sdlRenderer);
        //SDL_FillRect( surface, NULL, SDL_MapRGB( surface->format, 0, 0, 0 ) );
        SDL_DestroyTexture(texture);
    }
    SDL_DestroyRenderer(sdlRenderer);
    SDL_FreePalette(palette);
    SDL_Quit();

    return 0;
}