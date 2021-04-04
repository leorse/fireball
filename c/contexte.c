#include "linkedList.h"
#include "contexte.h"
#include "particule.h"
#include "initSDL.h"
#include "affichage.h"
#include "ctools.h"
#include "linkedList.h"

void initContexte(CONTEXTE *contexte)
{
    contexte->nombreMeteor = 0;
    DoPalette(contexte->listeCouleur);
    contexte->dernier = initElmt(&contexte->premier);
    InitSprite(contexte->Sprites);
    drawSprite(contexte->Sprites);

    contexte->drawPalette = false;
}

void addParticule(CONTEXTE* contexte, int x, int y)
{
    PARTICULE *ptr = NULL;
    ptr = (PARTICULE *)__CT_creerElement(sizeof(PARTICULE), contexte->dernier);
    ParticuleFactory(ptr);
    ptr->refX = x;
    ptr->refY = y;
    ptr->effemere = true;
    contexte->dernier = ptr;
}

PARTICULE* removeParticule(CONTEXTE* contexte, PARTICULE* particule)
{
    PARTICULE* retour = NULL;
    if(particule == contexte->premier)
    {
        contexte->premier = contexte->premier->suivant;
        retour = contexte->premier;
    }
    else if(particule == contexte->dernier)
    {
        contexte->dernier = contexte->dernier->precedent;
        retour = contexte->dernier;
    }
    else{
        retour = particule->suivant;
    }
    __CT_libererUnElement(particule);
    return retour;
}

PARTICULE *initElmt(PARTICULE **lst)
{
    PARTICULE *ptr = NULL;

    srand(time(NULL));
    *lst = (PARTICULE *)__CT_creerElement(sizeof(PARTICULE), NULL);
    ptr = *lst;
    ParticuleFactory(ptr);
    ptr->refX = PTX;
    ptr->refY = PTY;
    for (int i = 0; i < NB_BOULETTE-1; i++)
    {
        ptr = (PARTICULE *)__CT_creerElement(sizeof(PARTICULE), ptr);
        ParticuleFactory(ptr);
        ptr->refX = PTX;
        ptr->refY = PTY;
    }

    return ptr;
}

void detruireContexte(CONTEXTE *contexte)
{
    __CT_libererElements(contexte->premier);
}

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

void switchPalette(CONTEXTE* contexte)
{
    contexte->drawPalette = !contexte->drawPalette;
}