#include <cstdlib>
#include <ctime>
#include <iostream>

#include "App.hpp"

App::App()
{
    this->programAlive = true;
    re.seed(time(0));
    this->partManager.setSeed(re);
}

App::~App()
{
    cout << "app détruit" << endl;
}

void App::growParticules()
{
    this->partManager.growParticules();
    this->partManager.drawParticules();
}

bool App::isProgramAlive()
{
    return this->programAlive;
}

void App::stopProgram()
{
    this->programAlive = false;
}

void App::drawParticules()
{
}