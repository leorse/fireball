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
                    app.mousePress(event.button);
                }
                if (event.type == SDL_KEYUP)
                {
                    //keyPress(&event.key, &app);
                }
                if (event.type == SDL_MOUSEMOTION)
                {
                    app.setMousePosition(event.motion.x,event.motion.y);
                }
            }
            app.render();
            //if (contexte.drawLogo)
           // {
       //         afficherLogoTopRight(contexte.bump);
            //}

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