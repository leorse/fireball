#include <iostream>
#include <string>
#include <sstream>
#include "SDLManagment.hpp"
#include "App.hpp"
using namespace std;

SDL_Renderer* SDLManagment::sdlRenderer;
SDL_Surface* SDLManagment::surface;

void SDLManagment::initPalette()
{
    int i;
    for (i = 0; i < 128; i++)
    {
        this->palette[i].r = i;
        this->palette[i].g = 0;
        this->palette[i].b = 0;
    }
    for (i = 128; i < 256; i++)
    {
        this->palette[i].r = i;
        this->palette[i].g = (i - 128) * 2;
        this->palette[i].b = 0;
    }
    this->palette[255].r = 255;
    this->palette[255].g = 255;
    this->palette[255].b = 255;
}

SDLManagment::SDLManagment()
{
    
    SDLManagment::sdlRenderer = nullptr;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        throw "Erreur d'initialisation de la SDL";
    }
    
    if (TTF_Init() < 0)
    {
        stringstream ss(TTF_GetError());
        throw "Erreur d'initialisation de TTF_Init : ",ss.str();
        exit(EXIT_FAILURE);
    }

    if ((this->sdlWindow = SDL_CreateWindow("ma fenêtre", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, App::LARGEUR, App::HAUTEUR, SDL_WINDOW_OPENGL)) == NULL)
    {
        throw "Erreur à la création dela fenêtre";
    }

    SDLManagment::sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
    SDLManagment::surface = SDL_CreateRGBSurface(SDL_SWSURFACE, App::LARGEUR, App::HAUTEUR, 8, 0, 0, 0, 0);
    this->initPalette();
    SDL_SetPaletteColors(SDLManagment::surface->format->palette, this->palette, 0, 256);
}

SDLManagment::~SDLManagment()
{
    SDL_DestroyWindow(this->sdlWindow);
    SDL_Quit();
}

void SDLManagment::render(void)
{
    SDL_Texture *texture = SDL_CreateTextureFromSurface(sdlRenderer, surface);
        SDL_RenderCopy(sdlRenderer, texture, NULL, NULL);

        SDL_RenderPresent(sdlRenderer);
        //pour effacer
        /*if (!contexte.drawBlur)
        {
            SDL_FillRect( surface, NULL, 0 );
        }*/
        SDL_FillRect( surface, NULL, 0 );
        SDL_DestroyTexture(texture);
}