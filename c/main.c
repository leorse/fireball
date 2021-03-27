#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#define L 1980
#define H 1024
#define RANDOM(n) (((float)rand() / (float)RAND_MAX) * (n))
#define NB_BOULETTE 200
#define MAX_VITESSE 50
#define PTX (L/2)
#define PTY (H/2)
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
    int dir;
    int vitesse;
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
    int i;
    arma[boulette].dir = (rand() %360)	;
    arma[boulette].vitesse = rand() % MAX_VITESSE;//random(MAX_VITESSE)	;
    arma[boulette].tps = 0;
}

void Init_boulettes(void)
{
    int i;
    for (i = 0; i < NB_BOULETTE; i++)
    {
        Init_boulette(i);
        /*arma[i].dir = dire;    //60+(random(59))	;
        arma[i].vitesse = vit; //random(MAX_VITESSE)	;
        arma[i].tps = 0;*/
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
        arma[i].x = (double)(arma[i].vitesse * temps * cos((arma[i].dir)*(180/PI))) + PTX;
        arma[i].y = (double)(arma[i].vitesse * temps * sin((arma[i].dir)*(180/PI)) - (poids / 2) * (temps * temps)) + PTY;
        //printf("boulette vie %f  %f %f\n",arma[i].tps, arma[i].x,arma[i].y );
        if (arma[i].y >= H || arma[i].x >= L || arma[i].x < 0)
        {
            //printf("boulette init %f %f\n",arma[i].x,arma[i].y );
            Init_boulette(i);
        }
    }
}

void Mem_Pixel(int x, int y, int couleur, SDL_Surface *surface)
{
    if(x<0 || y<0 || x>=L || y>=H)
    {
        return;
    }
    uint8_t *offscreen = (uint8_t *)surface->pixels;
    offscreen[y * L + x] = couleur;
}

void flou(int x1,int y1,int x2,int y2,SDL_Surface *surface)
	{
	int x,y;
	int resultat;

    uint8_t *VScreen = (uint8_t *)surface->pixels;
	for(x=x1;x<x2;x++)
		{
		for(y=y1;y<y2;y++)
			{
			//if(VScreen[(x-1)+(y<<8)+(y<<6)]!=0 || VScreen[(x+1)+(y<<8)+(y<<6)]!=0 || VScreen[(x-1)+((y+1)<<8)+((y+1)<<6)]!=0 || VScreen[x+((y+1)<<8)+((y+1)<<6)]!=0 || VScreen[(x+1)+((y+1)<<8)+((y+1)<<6)]!=0 || VScreen[(x-1)+((y-1)<<8)+((y-1)<<6)]!=0 || VScreen[x+((y-1)<<8)+((y-1)<<6)] !=0 || VScreen[(x+1)+((y-1)<<8)+((y-1)<<6)]!=0)
				//resultat=((VScreen[(x-1)+(y<<8)+(y<<6)]+VScreen[(x+1)+(y<<8)+(y<<6)]+VScreen[(x-1)+((y+1)<<8)+((y+1)<<6)]+VScreen[x+((y+1)<<8)+((y+1)<<6)]+VScreen[(x+1)+((y+1)<<8)+((y+1)<<6)]+VScreen[(x-1)+((y-1)<<8)+((y-1)<<6)]+VScreen[x+((y-1)<<8)+((y-1)<<6)]+VScreen[(x+1)+((y-1)<<8)+((y-1)<<6)]));
				//resultat = resultat>>3;
                resultat = //VScreen[y * L + x]+
                            VScreen[(y-1) * L + x]+
                            VScreen[(y+1) * L + x]+
                            VScreen[y * L + (x+1)]+
                            VScreen[y * L + (x-1)];
                            resultat=resultat/4;
				VScreen[y * L + x]=resultat;
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
        int couleur = 255;//(((int)arma[i].tps)*20)%256;
        Mem_Pixel(posX, posY, couleur, VScreen);
        Mem_Pixel((posX) - 1, posY, couleur, VScreen);
        Mem_Pixel((posX) + 1, posY, couleur, VScreen);
        Mem_Pixel(posX, (posY) - 1, couleur, VScreen);
        Mem_Pixel(posX, (posY) + 1, couleur, VScreen);
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
    /*sdlTexture = SDL_CreateTexture(sdlRenderer,
                                   SDL_PIXELFORMAT_INDEX8,
                                   SDL_TEXTUREACCESS_STREAMING,
                                   640, 480);*/
    SDL_Color listeCouleur[256];
    DoPalette(listeCouleur);
    /*for (int i = 0; i < 256; i++)
    {
        SDL_Color couleur;
        couleur.a = 255;
        couleur.r = 255;
        couleur.b = 0;
        couleur.g = i;
        listeCouleur[i] = couleur;
        //printf( "r:%d, v:%d, b:%d\n", listeCouleur[i].r, listeCouleur[i].g, listeCouleur[i].b);
    }*/

    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, L, H, 8, 0, 0, 0, 0);
    SDL_SetPaletteColors(surface->format->palette, listeCouleur, 0, 256);
    sdlTexture = SDL_CreateTextureFromSurface(sdlRenderer, surface);

    Init_boulettes();
    while (1)
    {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
            break;

        //r.y = rand() % 256;
        /*uint8_t *offscreen = (uint8_t *)surface->pixels;
        for (int x = 0; x < L; x++)
        {
            for (int y = 0; y < H; y++)
            {
                int couleur = rand() % 256;
                offscreen[y * L + x] = couleur;
                //offscreen[0] = couleur;
            }
        }*/
Move_Boulette();
Affiche_boulette(surface);
flou(1,1,L-1,H-1,surface);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(sdlRenderer, surface);
        //SDL_UpdateTexture(sdlTexture, NULL, surface->pixels, surface->pitch);
        SDL_RenderCopy(sdlRenderer, texture, NULL, NULL);
        SDL_RenderPresent(sdlRenderer);
        SDL_DestroyTexture(texture);
        /*
        SDL_SetRenderTarget(sdlRenderer, sdlTexture);
        SDL_SetRenderDrawColor(sdlRenderer, 0x00, 0x00, 0x00, 0x00);
        //SDL_RenderClear(sdlRenderer);
        SDL_RenderDrawRect(sdlRenderer, &r);
        SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0x00, 0x00, 0x00);
        SDL_RenderFillRect(sdlRenderer, &r);
        SDL_SetRenderTarget(sdlRenderer, NULL);
        SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
        SDL_RenderPresent(sdlRenderer);*/

        //dire= (dire+1)%360;
    }
    SDL_DestroyRenderer(sdlRenderer);
    SDL_FreePalette(palette);
    SDL_Quit();

    return 0;
}