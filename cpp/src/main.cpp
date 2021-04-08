#include <iostream>
#include <string>
#include "SDLManagment.hpp"
#include "Contexte.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    SDL_Event event;
    try
    {

        SDLManagment sdlManagment;
        Contexte contexte;

        while (contexte.isProgramAlive())
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    contexte.stopProgram();
                    break;
                }

                if (event.type == SDL_MOUSEBUTTONDOWN)
                {
                    //mousePress(&event.button, &contexte);
                }
                if (event.type == SDL_KEYUP)
                {
                    //keyPress(&event.key, &contexte);
                }
            }
            contexte.growParticules();
        }
    }
    catch (string ex)
    {
        std::cerr << ex << endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "oui plantage" << '\n';
        std::cerr << e.what() << '\n';
    }

    return 0;
}