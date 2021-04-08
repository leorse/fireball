#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Contexte.hpp"

Contexte::Contexte()
{
    this->programAlive = true;

    //création des particules
    for (int inc = 0; inc < Contexte::NB_PARTICULE; inc++)
    {
        this->particules.push_back(this->factoryParticule());
    }
}

Contexte::~Contexte()
{
    for(auto it = this->particules.begin();it!=this->particules.end(); it++)
    {
        Particule *particule = *it;
        delete *it;
    }
}

Particule *Contexte::factoryParticule()
{

    std::srand(std::time(nullptr));
    int type = std::rand() % 2;
    if (type == static_cast<int>(TypeParticule::COMET))
    {
        return new Comet();
    }
    if (type == static_cast<int>(TypeParticule::FIREBALL))
    {
        return new Comet();
    }
}

void Contexte::growParticules()
{
    int inc =0;
    for(auto it = this->particules.begin();it!=this->particules.end(); it++, inc++)
    {
        Particule *particule = *it;
        particule->grow();
        if(!particule->isAlive())
        {
            particule->initLife(false);
            std::cout<<"elle creve!!:"<<particule->GetX()<<std::endl;

        }
    }
}

bool Contexte::isProgramAlive()
{
    return this->programAlive;
}

void Contexte::stopProgram()
{
    this->programAlive = false;
}