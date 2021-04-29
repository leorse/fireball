#include "particule.h"
#include "contexte.h"
#include "affichage.h"

/**
 * @brief génère un mouvement de tourbillon
 * 
 * @param particule 
 */
void moveTour(PARTICULE *particule)
{
    float oldX = particule->x;
    float oldY = particule->y;
    if(particule->tps==0)
    {
        oldX = 0;
        oldY = 0;
    }
    particule->tps += 0.1;
    float temps = particule->tps;
    
    particule->x = (double)(particule->vitesse * temps * cos((particule->dir + temps * 10) * PI / 180));
    particule->y = (double)(particule->vitesse * temps * sin((particule->dir - temps * 10) * PI / 180) - (particule->poids / 2) * (temps * temps));

    particule->dX = (particule->x-oldX)*5;
    particule->dY = (particule->y-oldY)*5;
}

/**
 * @brief génère un mouvement simple de lancé
 * 
 * @param particule 
 */
void moveClassic(PARTICULE *particule)
{
    float oldX = particule->x;
    float oldY = particule->y;
    if(particule->tps==0)
    {
        oldX = 0;
        oldY = 0;
    }

    particule->dX = oldX;
    particule->dY = oldY;
    particule->tps += 0.1;
    float temps = particule->tps;
    particule->x = (double)(particule->vitesse * temps * cos((particule->dir) * PI / 180));
    particule->y = (double)(particule->vitesse * temps * sin((particule->dir) * PI / 180) - (particule->poids / 2) * (temps * temps));
}

/**
 * @brief fait évoluer dans le temps la particule en la faisant mourir
 * 
 * @param particule 
 */
bool faireVivre(PARTICULE *particule)
{
    particule->Mover(particule);
    //if(particule->ephemere) printf("****");
    //printf("x:%f, y:%f\n", particule->x, particule->y);
    if (particule->tps >= particule->vie)
    {
        if(particule->ephemere || particule->explosive)
        {
            return true;
        }
        else
        {
            ParticuleFactory(particule);
        }
    }
    return false;
}

/**
 * @brief initialisation d'une particule
 * 
 * @param particule 
 */
void ParticuleFactory(PARTICULE *particule)
{
    //une direction entre 1 et 360°
    particule->dir = rand() % 360;
    particule->poids = -5;
    particule->vitesse = 25 + (rand() % MAX_VITESSE);
    particule->taille = 1 + (rand() % MAX_TAILLE);
    particule->tps = 0;
    particule->vie = 1 + (rand() % MAX_DUREE);
    particule->x = 0;
    particule->y = 0;
    particule->dX = 0;
    particule->dY = 0;
    particule->ephemere = false;
    particule->explosive = (rand() % 10) == 0;

    //déplacement simple ou en tour
    if (rand() % 2)
    {
        //mode Comete
        particule->Mover = moveTour;
        //initialisation du pointeur de fonction pour l'affichage
        particule->Drawer = afficheurTrainee;
    }
    else
    {
        //move Boule de feu
        particule->Mover = moveClassic;
        //initialisation du pointeur de fonction pour l'affichage
        particule->Drawer = afficheurSimple;
    }

    //pointeur de fonction pour faire vivre la particule
    particule->Grower = faireVivre;
}

/**
 * @brief simple affichage de la particule
 * 
 * @param this 
 * @param VScreen 
 * @param Sprites 
 */
void afficheurSimple(PARTICULE *this, SDL_Surface *VScreen, bool *Sprites[])
{
    int posX = this->x + this->refX;
    int posY = this->y + this->refY;
    int taille = this->taille;
    int vie = this->vie;
    int couleur = this->tps >= vie ? 0 : 256 - (this->tps * 256 / vie);

    if (taille == 1)
    {
        putPixel(posX, posY, couleur, VScreen);
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
                    putPixel(posX + incX, posY + incY, couleur, VScreen);
                    drawline(posX + incX, posY + incY, this->dX+ this->refX + incX, this->dY+ this->refY + incY, 255, VScreen);
                    
                }
            }
        }
    }
    drawline(posX, posY, this->dX+ this->refX, this->dY+ this->refY, 255, VScreen);
}

void afficheurTrainee(PARTICULE *this, SDL_Surface *VScreen, bool *Sprites[])
{
    int posX = this->x + this->refX;
    int posY = this->y + this->refY;
    int taille = this->taille;
    int vie = this->vie;
    int couleur = this->tps >= vie ? 0 : 256 - (this->tps * 256 / vie);

    if (taille == 1)
    {
        putPixel(posX, posY, couleur, VScreen);
        drawline(posX, posY, posX-this->dX, posY-this->dY, 255, VScreen);
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
                    putPixel(posX + incX, posY + incY, couleur, VScreen);
                    drawline(posX + incX, posY+ incY, posX-this->dX + incX, posY-this->dY+ incY, couleur, VScreen);
                }
            }
        }
    }
}

void drawline(int x0, int y0, int x1, int y1, int couleur, SDL_Surface *VScreen)
{
    int dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1; 
  int err = dx + dy, e2; /* error value e_xy */
  for (;;){  /* loop */
    putPixel (x0,y0, couleur, VScreen);
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
    if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
  }
    
}