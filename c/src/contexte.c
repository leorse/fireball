#include "linkedList.h"
#include "contexte.h"
#include "particule.h"
#include "initSDL.h"
#include "affichage.h"

void initContexte(CONTEXTE *contexte)
{
    contexte->nombreMeteor = 0;
    doPalette(contexte->listeCouleur);
    contexte->dernier = initElmt(contexte);
    initSprite(contexte->Sprites);
    //� d�commenter pour afficher les sprites
    //drawSprite(contexte->Sprites);

    contexte->drawPalette = false;
    contexte->drawBoard = false;
    contexte->drawBlur = true;
    contexte->drawLogo = true;
    contexte->mode = FIREBALL;
}

void addParticule(CONTEXTE *contexte, int nombre, int x, int y)
{
    PARTICULE *ptr = NULL;
    //printf("creation particule:x:%d, y:%d\n", x, y);
    for (int i = 0; i < nombre; i++)
    {
        ptr = (PARTICULE *)__CT_creerElement(sizeof(PARTICULE), contexte->dernier);
        ParticuleFactory(ptr);
        ptr->refX = (float)x;
        ptr->refY = (float)y;
        ptr->ephemere = true;
        ptr->explosive = false;
        contexte->dernier = ptr;
        contexte->nombreMeteor++;
    }
}

PARTICULE *removeParticule(CONTEXTE *contexte, PARTICULE *particule)
{
    PARTICULE *retour = NULL;
    if (particule == contexte->premier)
    {
        contexte->premier = contexte->premier->suivant;
        retour = contexte->premier;
    }
    else if (particule == contexte->dernier)
    {
        contexte->dernier = contexte->dernier->precedent;
        retour = contexte->dernier;
    }
    else
    {
        retour = particule->suivant;
    }
    __CT_libererUnElement(particule);
    contexte->nombreMeteor--;
    return retour;
}

PARTICULE *initElmt(CONTEXTE *contexte)
{
    PARTICULE *ptr = NULL;
    PARTICULE **lst = &(contexte->premier);
    srand(time(NULL));
    *lst = (PARTICULE *)__CT_creerElement(sizeof(PARTICULE), NULL);
    ptr = *lst;
    ParticuleFactory(ptr);
    ptr->refX = PTX;
    ptr->refY = PTY;

    contexte->nombreMeteor++;

    for (int i = 0; i < NB_BOULETTE - 1; i++)
    {
        ptr = (PARTICULE *)__CT_creerElement(sizeof(PARTICULE), ptr);
        ParticuleFactory(ptr);
        ptr->refX = PTX;
        ptr->refY = PTY;
        contexte->nombreMeteor++;
    }

    return ptr;
}

void detruireContexte(CONTEXTE *contexte)
{
    __CT_libererElements(contexte->premier);
    TTF_CloseFont(contexte->font);
    SDL_FreeSurface(contexte->surface);
    SDL_FreeSurface(contexte->bump);
    SDL_FreeSurface(contexte->phongmap);
    SDL_FreeSurface(contexte->cache);
}

void doPalette(SDL_Color *Palette)
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
}

void doPaletteOffset(SDL_Color *Palette, int offset)
{
    doPalette(Palette);
    for (int i = 0; i < 256; i++)
    {
        Palette[i].r = Palette[i].r - offset < 0 ? 0 : Palette[i].r - offset;
        Palette[i].g = Palette[i].g - offset < 0 ? 0 : Palette[i].g - offset;
        Palette[i].b = Palette[i].b - offset < 0 ? 0 : Palette[i].b - offset;
    }
}

void initSprite(bool *Sprites[MAX_TAILLE])
{
    for (int i = 0; i < MAX_TAILLE; i++)
    {
        int taille = (i + 1) * (i + 1);
        Sprites[i] = (bool *)malloc(taille * sizeof(bool));
        bool *sprite = Sprites[i];
        //|1|
        if (i == 0)
        {
            sprite[0] = true;
        }
        //|11|
        if (i == 1)
        {
            bool tabSprite[2][2] =
                {{true, false},
                 {false, true}};
            memcpy(sprite, tabSprite, sizeof(tabSprite));
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

void doModeLight(CONTEXTE *contexte, int x, int y)
{
    doPaletteOffset(contexte->listeCouleur, rand() % 20 );
    SDL_SetPaletteColors((contexte->surface)->format->palette, contexte->listeCouleur, 0, 256);
    drawBumpMapping(contexte, x, y);
}

void doModeShadow(CONTEXTE *contexte, int x, int y)
{
    int offset = rand() % 20 +1;
    doPaletteOffset(contexte->listeCouleur, offset);
    SDL_SetPaletteColors((contexte->surface)->format->palette, contexte->listeCouleur, 0, 256);
    drawBumpMapping(contexte, x, y);
    drawShadow(contexte, x, y, offset);
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

void switchPalette(CONTEXTE *contexte)
{
    contexte->drawPalette = !contexte->drawPalette;
}

void switchBoard(CONTEXTE *contexte)
{
    contexte->drawBoard = !contexte->drawBoard;
}

void switchBlur(CONTEXTE *contexte)
{
    contexte->drawBlur = !contexte->drawBlur;
}

void switchLogo(CONTEXTE *contexte)
{
    contexte->drawLogo = !contexte->drawLogo;
}

void switchMode(CONTEXTE *contexte)
{
    if (contexte->mode == FIREBALL)
    {
        initialiserLumiere(contexte);
        SDL_BlitSurface(contexte->surface, NULL, contexte->bump, NULL);
        contexte->mode = LIGHT;
        //drawBumpMapping(contexte);
    }
    else if (contexte->mode == LIGHT)
    {
        //copier le cache
        afficherLogoCenter(contexte->cache);
        contexte->mode = SHADOW;
    }
    else if (contexte->mode == SHADOW)
    {

        contexte->mode = FIREBALL;
    }
}