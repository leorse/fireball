#include <iostream>
#include <string>
#include "SDLManagment.hpp"
#include "App.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    SDL_Event event;
    try
    {

        SDLManagment sdlManagment;
        App app;

        while (app.isProgramAlive())
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    app.stopProgram();
                    break;
                }

                if (event.type == SDL_MOUSEBUTTONDOWN)
                {
                    //mousePress(&event.button, &app);
                }
                if (event.type == SDL_KEYUP)
                {
                    //keyPress(&event.key, &app);
                }
            }
            app.growParticules();

            sdlManagment.render();
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