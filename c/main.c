#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include "ctools.h"
#include "linkedList.h"

#define L 1000
#define H 1000
#define RANDOM(n) (((float)rand() / (float)RAND_MAX) * (n))
#define NB_BOULETTE 5000
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

typedef struct _METEOR
{
    struct _METEOR *suivant;
    struct _METEOR *precedent;
    float x;
    float y;
    float tps;
    float dir;
    int vitesse;
    int taille;
    int vie;
    double dX;
    double dY;

} METEOR;

typedef struct
{
    METEOR *premier;
    METEOR *dernier;
    int nombreMeteor;
    bool *Sprites[MAX_TAILLE];
    SDL_Color listeCouleur[256];

} CONTEXTE;

METEOR arma[NB_BOULETTE];

void DoPalette(SDL_Color *Palette)
{
    int i;
    for (i = 0; i < 128; i++)
    {
        Palette[i].r = i;
        Palette[i].g = 0;
        Palette[i].b = 0;
    }
    for (i = 128; i < 256; i++)
    {
        Palette[i].r = i;
        Palette[i].g = (i - 128) * 2;
        Palette[i].b = 0;
    }
    Palette[255].r = 255;
    Palette[255].g = 255;
    Palette[255].b = 255;
    /*for (i = 0; i < 64; i++)
    {
        Palette[i].r = i;
        Palette[i].g = 0;
        Palette[i].b = 0;
    }
    for (i = 64; i < 128; i++)
    {
        Palette[i].r = i;
        Palette[i].g = i - 64;
        Palette[i].b = 0;
    }
    for (i = 128; i < 256; i++)
    {
        Palette[i].r = i;
        Palette[i].g = i-128;
        Palette[i].b = 0;//(i - 128) >> 1;
    }*/
}
void InitSprite(bool *Sprites[MAX_TAILLE])
{
    for (int i = 0; i < MAX_TAILLE; i++)
    {
        int taille = (i + 1) * (i + 1);
        Sprites[i] = (bool *)malloc(taille * sizeof(bool));
        //printf("ici, taille:%d, sp=%p", taille, Sprites[i] );
        bool *sprite = Sprites[i];
        /*char tabSprite[MAX_TAILLE][MAX_TAILLE] =
            {
                {0, 0, 1, 0, 0},
                {0, 1, 1, 1, 0},
                {1, 1, 1, 1, 1},
                {0, 1, 1, 1, 0},
                {0, 0, 1, 0, 0},
            };*/
        //|1|
        if (i == 0)
        {
            sprite[0] = true;
        }
        //|11|
        //|11|
        if (i == 1)
        {
            bool tabSprite[2][2] =
                {{true, false},
                 {false, true}};
            memcpy(sprite, tabSprite, sizeof(tabSprite));
            printf("ts\n");
            printf("%c,", tabSprite[0][0] + 'A');
            printf("%c\n", tabSprite[1][0] + 'A');
            printf("%c,", tabSprite[0][1] + 'A');
            printf("%c,", tabSprite[1][1] + 'A');
            printf("s\n");
            printf("%c,", sprite[0] + 'A');
            printf("%c\n", sprite[1] + 'A');
            printf("%c,", sprite[2] + 'A');
            printf("%c,", sprite[3] + 'A');
        }
        if (i == 2)
        {
            bool tabSprite[3][3] =
                {
                    {false, true, false},
                    {true, true, true},
                    {false, true, false},
                };
            memcpy(sprite, tabSprite, sizeof(tabSprite));
            printf("ts\n");
            printf("%c,", tabSprite[0][0] + 'A');
            printf("%c,", tabSprite[1][0] + 'A');
            printf("%c\n", tabSprite[2][0] + 'A');
            printf("%c,", tabSprite[0][1] + 'A');
            printf("%c,", tabSprite[1][1] + 'A');
            printf("%c\n", tabSprite[2][1] + 'A');
            printf("%c,", tabSprite[0][2] + 'A');
            printf("%c,", tabSprite[1][2] + 'A');
            printf("%c\n", tabSprite[2][2] + 'A');
            printf("s\n");
            printf("%c,", sprite[0] + 'A');
            printf("%c,", sprite[1] + 'A');
            printf("%c\n,", sprite[2] + 'A');
            printf("%c,", sprite[3] + 'A');
            printf("%c,", sprite[4] + 'A');
            printf("%c\n,", sprite[5] + 'A');
            printf("%c,", sprite[6] + 'A');
            printf("%c,", sprite[7] + 'A');
            printf("%c", sprite[8] + 'A');
            printf("S\n");
            bool *spr = Sprites[2];
            //printf("S %p s %p", &Sprites[2], &spr);
            printf("%c,", *(spr + 0) + 'A');
            printf("%c,", *(spr + 1) + 'A');
            printf("%c\n,", *(spr + 2) + 'A');
            printf("%c,", *(spr + 3) + 'A');
            printf("%c,", *(spr + 4) + 'A');
            printf("%c\n,", *(spr + 5) + 'A');
            printf("%c,", *(spr + 6) + 'A');
            printf("%c,", *(spr + 7) + 'A');
            printf("%c", *(spr + 8) + 'A');
        }
        if (i == 3)
        {
            bool tabSprite[4][4] =
                {{false, true, false, false},
                 {false, true, true, true},
                 {true, true, true, false},
                 {false, false, true, false}};
            memcpy(sprite, tabSprite, sizeof(tabSprite));
        }
        if (i == 4)
        {
            bool tabSprite[5][5] =
                {{false, false, true, false, false},
                 {false, true, true, true, false},
                 {true, true, true, true, true},
                 {false, true, true, true, false},
                 {false, false, true, false, false}};
            memcpy(sprite, tabSprite, sizeof(tabSprite));
        }
        if (i == 5)
        {
            bool tabSprite[6][6] =
                {{false, false, true, false, false, false},
                 {false, true, true, true, false, false},
                 {false, true, true, true, true, true},
                 {true, true, true, true, true, false},
                 {false, true, true, true, true, false},
                 {false, false, false, true, false, false}};
            memcpy(sprite, tabSprite, sizeof(tabSprite));
        }
    }
}

void drawSprite(bool *Sprites[MAX_TAILLE])
{
    for (int inc = 0; inc < MAX_TAILLE; inc++)
    {
        bool *sprite = Sprites[inc];
        printf("--- Sprite %2d:\n", inc + 1);
        int taille = inc + 1;
        for (int incX = 0; incX < taille; incX++)
        {
            for (int incY = 0; incY < taille; incY++)
            {
                char car = sprite[incX * taille + incY] == true ? '1' : '0';
                printf("%c,", car);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void Init_boulette(METEOR* meteor)
{
    meteor->dir = 1 + (rand() % 360);
    meteor->vitesse = 25 + (rand() % MAX_VITESSE);
    meteor->taille = 1 + (rand() % MAX_TAILLE);
    meteor->tps = 0;
    meteor->vie = 1 + (rand() % MAX_DUREE);
    meteor->dX = cos((meteor->dir) * PI / 180); // * (180 / PI));
    meteor->dY = sin((meteor->dir) * PI / 180); // * (180 / PI));
}

METEOR* initElmt(METEOR **lst)
{
    METEOR *ptr = NULL;

    srand(time(NULL));
    *lst = (METEOR *)__CT_creerElement(sizeof(METEOR), NULL);
    ptr = *lst;
    Init_boulette(ptr);
    for (int i = 0; i < NB_BOULETTE; i++)
    {
        ptr = (METEOR *)__CT_creerElement(sizeof(METEOR), ptr);
        Init_boulette(ptr);
    }

    return ptr;
}



void Move_Boulette(METEOR* meteor)
{
    int i;
    float temps;
    METEOR *ptrMeteor;

i=0;
    for (ptrMeteor=meteor;ptrMeteor!=NULL;ptrMeteor=ptrMeteor->suivant)
    {
        //printf("ici %d\n", i++);
        ptrMeteor->tps += 0.1;
        temps = ptrMeteor->tps;
        ptrMeteor->x = (double)(ptrMeteor->vitesse * temps * cos((ptrMeteor->dir + temps * 10) * PI / 180) /*ptrMeteor->dX*/) + PTX;
        ptrMeteor->y = (double)(ptrMeteor->vitesse * temps * sin((ptrMeteor->dir - temps * 10) * PI / 180) /*ptrMeteor->dY*/ - (poids / 2) * (temps * temps)) + PTY;

        /* if(i==0)
        {
            printf("dir:%f\n", arma[i].dir);
            //printf("tmp:%f, vitesse:%d dx:%lf, dy:%lf, poids:%d, x:%f, y:%f\n", temps, arma[i].vitesse, arma[i].dX, arma[i].dY, poids, arma[i].x,arma[i].y);
        }*/
        if (/*arma[i].y >= H || arma[i].x >= L || arma[i].x < 0 ||*/ ptrMeteor->tps >= ptrMeteor->vie)
        {
            Init_boulette(ptrMeteor);
        }
    }
}

void Mem_Pixel(int x, int y, int couleur, SDL_Surface *surface)
{
    if (x < 1 || y < 1 || x >= L - 1 || y >= H - 1)
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
            resultat =
                VScreen[(y - 1) * L + x] +
                VScreen[(y + 1) * L + x] +
                VScreen[y * L + (x + 1)] +
                VScreen[y * L + (x - 1)];
            resultat = resultat / 4;
            VScreen[y * L + x] = resultat;
        }
    }
}

void Affiche_boulette(SDL_Surface *VScreen, bool *Sprites[MAX_TAILLE], METEOR* meteor)
{
    int i;
    METEOR *ptrMeteor;
    char tabSprite[MAX_TAILLE][MAX_TAILLE] =
        {
            {0, 0, 1, 0, 0},
            {0, 1, 1, 1, 0},
            {1, 1, 1, 1, 1},
            {0, 1, 1, 1, 0},
            {0, 0, 1, 0, 0},
        };
    for (ptrMeteor=meteor;ptrMeteor!=NULL;ptrMeteor=ptrMeteor->suivant)
    {
        int posX = ptrMeteor->x;
        int posY = ptrMeteor->y;
        int taille = ptrMeteor->taille;
        int vie = ptrMeteor->vie;
        int couleur = ptrMeteor->tps >= vie ? 0 : 256 - (ptrMeteor->tps * 256 / vie);
        //méthode avec les sprites précalculés
        if (taille == 1)
        {
            Mem_Pixel(posX, posY, couleur, VScreen);
        }
        else
        {
            bool *sprite = Sprites[taille - 1];
            for (int incX = 0; incX < taille; incX++)
            {
                for (int incY = 0; incY < taille; incY++)
                {
                    if (*(sprite + ((incX * taille) + incY)) == true)
                    {
                        Mem_Pixel(posX + incX, posY + incY, couleur, VScreen);
                    }
                }
            }
        }
        //méthode avec le sprite de 5*5
        /*
        for (int incX = 0; incX < MAX_TAILLE; incX++)
        {
            for (int incY = 0; incY < MAX_TAILLE; incY++)
            {
                if (tabSprite[incX][incY])
                {
                    Mem_Pixel(posX + incX, posY + incY, couleur, VScreen);
                }
            }
        }*/
        //méthode avec tout le calcul à chaque fois
        /*
        Mem_Pixel(posX, posY, couleur, VScreen);
        for (int incX = 0; incX < taille; incX++)
        {
            for (int incY = 0; incY < taille - incX; incY++)
            {
                Mem_Pixel(posX, posY + incY, couleur, VScreen);
                Mem_Pixel(posX, posY - incY, couleur, VScreen);
                Mem_Pixel(posX + incX, posY - incY, couleur, VScreen);
                Mem_Pixel(posX - incX, posY + incY, couleur, VScreen);
                Mem_Pixel(posX + incX, posY + incY, couleur, VScreen);
                Mem_Pixel(posX - incX, posY - incY, couleur, VScreen);
            }
        }*/
        /*
          0
         000
        00000
         000
          0
        */
    }
}

void afficherPalette(SDL_Surface *VScreen)
{
    for (int incX = 0; incX < 40; incX++)
    {
        for (int incY = 0; incY < H; incY++)
        {
            Mem_Pixel(incX, incY, incY * 256 / H, VScreen);
        }
    }
}

void mousePress(SDL_MouseButtonEvent *bE)
{
    if (bE->button == SDL_BUTTON_LEFT)
    {
        //handle a left-click
        printf("BOUTON!!!!!!\n");
        // SDL_Cursor cur = SDL_GetCursor
    }
}

typedef struct LBL_STR
{
    struct LBL_STR *suivant;
    struct LBL_STR *precedent;
    int i;
    char une_chaine[25];
} TYP_STR;



void initContexte(CONTEXTE* contexte)
{
    contexte->nombreMeteor = 0;
    DoPalette(contexte->listeCouleur);
    contexte->dernier = initElmt(&contexte->premier);
    InitSprite(contexte->Sprites);
    drawSprite(contexte->Sprites);
}

void detruireContexte(CONTEXTE* contexte)
{
    __CT_libererElements(contexte->premier);
}

int main(int argv, char *argc[])
{
    SDL_Window *sdlWindow;
    SDL_Texture *sdlTexture;
    SDL_Event event;
    SDL_Rect r;

    CONTEXTE contexte;

    long clk_tck = CLOCKS_PER_SEC;
    long nbFrame = 0L;
    double cumul = 0;
    double moyenne = 0;
    double tickMin, tickMax = 0;
    clock_t t1, t2;
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

    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);

    initContexte(&contexte);

    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, L, H, 8, 0, 0, 0, 0);
    SDL_SetPaletteColors(surface->format->palette, contexte.listeCouleur, 0, 256);
    sdlTexture = SDL_CreateTextureFromSurface(sdlRenderer, surface);

    

    while (1)
    {
        nbFrame++;
        /* Recuperation du temps initial en "clock ticks" */
        t1 = clock();

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
            break;
        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            //do whatever you want to do after a mouse button was pressed,
            // e.g.:
            mousePress(&event.button);
        }

        Move_Boulette(contexte.premier);
        Affiche_boulette(surface, contexte.Sprites, contexte.premier);
        flou(1, 1, L - 1, H - 1, surface);
        afficherPalette(surface);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(sdlRenderer, surface);
        SDL_RenderCopy(sdlRenderer, texture, NULL, NULL);
        SDL_RenderPresent(sdlRenderer);
        //SDL_FillRect( surface, NULL, SDL_MapRGB( surface->format, 0, 0, 0 ) );
        SDL_DestroyTexture(texture);

        /* Recuperation du temps final en "clock ticks" */
        t2 = clock();

        double tick = (double)(t2 - t1) / (double)clk_tck;
        if (nbFrame == 100)
        {
            cumul = 0;
            nbFrame = 1;
            tickMax = tickMin = 0;
        }
        cumul += tick;
        moyenne = cumul / nbFrame;
        if (tick > tickMax || tickMax == 0)
        {
            tickMax = tick;
        }
        if (tick < tickMin || tickMin == 0)
        {
            tickMin = tick;
        }
        //(void)printf("Temps consomme (s) : %lf , moyenne : %lf, min: %lf, Max: %lf\n", tick, moyenne, tickMin, tickMax);
    }
    SDL_DestroyRenderer(sdlRenderer);
    SDL_Quit();

    return 0;
}