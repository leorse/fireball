#include <iostream>

#include "Drawer.hpp"
#include "SDLManagment.hpp"
#include "App.hpp"
#include "cpp_logo.hpp"
#include "Particule.hpp"

Drawer::Drawer()
{
    this->initSprites();
}

void Drawer::initSprites(void)
{
    vector<bool> vec1 = {true};
    this->sprites.push_back(vec1);

    vector<bool> vec2 = {true, false, false, true};
    this->sprites.push_back(vec2);

    vector<bool> vec3 = {false, true, false, true, true, true, false, true, false};
    this->sprites.push_back(vec3);

    vector<bool> vec4 = {false, true, false, false, false, true, true, true, true, true, true, false, false, false,
                         true, false};
    this->sprites.push_back(vec4);

    vector<bool> vec5 = {false, false, true, false, false, false, true, true, true, false, true, true, true, true, true,
                         false, true, true, true, false, false, false, true, false, false};
    this->sprites.push_back(vec5);

    vector<bool> vec6 = {false, false, true, false, false, false, false, true, true, true, false, false, false, true,
                         true, true, true, true, true, true, true, true, true, false, false, true, true, true, true,
                         false, false, false, false, true, false, false};
    this->sprites.push_back(vec6);

    cout << "fin init sprites" << endl;
}

void Drawer::putPixel(int x, int y, int couleur)
{
    SDL_Surface *surface = SDLManagment::surface;
    if (x < 1 || y < 1 || x >= App::LARGEUR - 1 || y >= App::HAUTEUR - 1)
    {
        return;
    }
    uint8_t *offscreen = (uint8_t *) surface->pixels;
    offscreen[y * App::LARGEUR + x] = couleur;
}

void Drawer::blur(int x1, int y1, int x2, int y2)
{
    int x, y;
    int resultat;

    SDL_Surface *surface = SDLManagment::surface;

    uint8_t *VScreen = (uint8_t *) surface->pixels;
    for (x = x1; x < x2; x++)
    {
        for (y = y1; y < y2; y++)
        {
            resultat = VScreen[(y - 1) * App::LARGEUR + x] + VScreen[(y + 1) * App::LARGEUR + x] +
                       VScreen[y * App::LARGEUR + (x + 1)] + VScreen[y * App::LARGEUR + (x - 1)];
            resultat = resultat / 4;
            VScreen[y * App::LARGEUR + x] = resultat;
        }
    }
}

void Drawer::afficherLogoTopRight()
{
    afficherLogo(App::LARGEUR - Logo::WIDTH, 0);
}

void Drawer::afficherLogo(int x, int y)
{
    for (int incX = 0; incX < Logo::WIDTH; incX++)
    {
        for (int incY = 0; incY < Logo::HEIGHT; incY++)
        {
            if (Logo::logo_map[incY * Logo::WIDTH + incX] != 0x00)
            {
                putPixel(x + incX, y + incY, 255);
            }
        }
    }
}

void Drawer::afficheurSimple(Particule *part)
{
    int posX = part->getX() + part->getRefX();
    int posY = part->getY() + part->getRefY();
    int taille = part->getTaille();
    int vie = part->getVie();
    int couleur = part->getTps() >= vie ? 0 : 256 - (part->getTps() * 256 / vie);

    if (taille == 0)
    {
        putPixel(posX, posY, couleur);
    }
    else
    {
        vector<bool> sprite = this->sprites.at(taille);
        for (int incX = 0; incX < taille; incX++)
        {
            for (int incY = 0; incY < taille; incY++)
            {
                if (sprite.at((incX * taille) + incY) == true)
                {
                    putPixel(posX + incX, posY + incY, couleur);
                    //drawline(posX + incX, posY + incY, part->getDX() + part->getRefX() + incX, part->getDY() + part->getRefY() + incY, 255);
                }
            }
        }
    }
    drawline(posX, posY, part->getDX() + part->getRefX(), part->getDY() + part->getRefY(), 255);
}

void Drawer::afficheurTrainee(Particule *part)
{
    int posX = part->getX() + part->getRefX();
    int posY = part->getY() + part->getRefY();
    int taille = part->getTaille();
    int vie = part->getVie();
    int couleur = part->getTps() >= vie ? 0 : 256 - (part->getTps() * 256 / vie);

    if (taille == 0)
    {
        putPixel(posX, posY, couleur);
        drawline(posX, posY, posX - part->getDX(), posY - part->getDY(), 255);
    }
    else
    {
        vector<bool> sprite = this->sprites.at(taille);
        for (int incX = 0; incX < taille; incX++)
        {
            for (int incY = 0; incY < taille; incY++)
            {
                if (sprite.at((incX * taille) + incY) == true)
                {
                    putPixel(posX + incX, posY + incY, couleur);
                    drawline(posX + incX, posY + incY, posX - part->getDX() + incX, posY - part->getDY() + incY,
                             couleur);
                }
            }
        }
    }
}

void Drawer::drawline(int x0, int y0, int x1, int y1, int couleur)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    for (;;)
    { /* loop */
        this->putPixel(x0, y0, couleur);
        if (x0 == x1 && y0 == y1)
        {
            break;
        }
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        } /* e_xy+e_x > 0 */
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        } /* e_xy+e_y < 0 */
    }
}
/*
void Drawer::afficherBoard(CONTEXTE *contexte, clock_t t1, clock_t t2, long nbFrame)
{
    char texte[100];
    SDL_Rect dstRect = {100, 0 ,50, 200};

SDL_Surface *surface = SDLManagment::surface;
    sprintf(texte, "Nombre Particule:%d", contexte->nombreMeteor);
    
    afficherTexte(texte, contexte, surface, dstRect);
    //calculerInfosTimer(texte, t1, t2, nbFrame);
    dstRect.y = 25;
    afficherTexte(texte, contexte, surface, dstRect);
}*/
/*
void Drawer::afficherTexte(char* texte, CONTEXTE* contexte, SDL_Rect rectDest)
{
    SDL_Color color = {255, 255, 255};
    SDL_Surface *text_surface;

SDL_Surface *surface = SDLManagment::surface;
    int w,h;
    TTF_SizeUTF8(contexte->font,texte,&w,&h);
    rectDest.w = w;
    rectDest.h = h;
    if (!(text_surface = TTF_RenderUTF8_Solid(contexte->font, texte, color)))
    {
        //handle error here, perhaps print TTF_GetError at least
    }
    else
    {
        SDL_FillRect( surface, &rectDest, 0);//SDL_MapRGB( surface->format, 0, 0, 0 ) );
        SDL_BlitSurface(text_surface, NULL, surface, &rectDest);
        //perhaps we can reuse it, but I assume not for simplicity.
        SDL_FreeSurface(text_surface);
    }
}*/