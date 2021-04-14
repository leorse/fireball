//
// Created by osboxes on 13/04/2021.
//

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "PartManager.hpp"

using namespace std;

PartManager::PartManager()
{

    //création des particules
    for (int inc = 0; inc < PartManager::NB_PARTICULE; inc++)
    {
        cout<<"j'aoute"<<endl;
        this->particules.push_back(this->factoryParticule());
    }
}

PartManager::~PartManager()
{
    cout<<"partmanager détruit"<<endl;
    /*
    for(auto it = this->particules.begin();it!=this->particules.end(); it++)
    {
        Particule *particule = *it;
        delete *it;
    }*/
}

unique_ptr<Particule> PartManager::factoryParticule()
{

    std::srand(std::time(nullptr));
    int type = std::rand() % 2;
    if (type == static_cast<int>(TypeParticule::COMET))
    {
        return make_unique<Comet>() ;
    }
    if (type == static_cast<int>(TypeParticule::FIREBALL))
    {
        return make_unique<Comet>() ;
    }
    return make_unique<Comet>() ;
}


void PartManager::growParticules()
{
    int inc =0;
    for(auto it = this->particules.begin();it!=this->particules.end(); it++, inc++)
    {
        cout<<"je fais grandir"<<endl;
        //Particule& particule = it;
        auto particule = it->get();
        cout<<"de type:"<<static_cast<int>(particule->getType())<<" et x:"<<particule->getX()<< endl;
        particule->grow();
        if(!particule->isAlive())
        {
            particule->initLife(false);
            std::cout<<"elle creve!!:"<<particule->getX()<<std::endl;
        }
    }
}

void PartManager::drawParticules()
{
}

