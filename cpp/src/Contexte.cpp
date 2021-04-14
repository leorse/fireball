#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Contexte.hpp"

Contexte::Contexte()
{
    this->programAlive = true;

}

Contexte::~Contexte()
{
}

void Contexte::growParticules()
{
    this->partManager.growParticules();
    this->partManager.drawParticules();
}

bool Contexte::isProgramAlive()
{
    return this->programAlive;
}

void Contexte::stopProgram()
{
    this->programAlive = false;
}