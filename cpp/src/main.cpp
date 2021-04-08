#include <iostream>
#include <string>
#include "SDLManagment.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    try
    {
        SDLManagment sdlManagment;
        std::cout << "oui réussie" << endl;
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