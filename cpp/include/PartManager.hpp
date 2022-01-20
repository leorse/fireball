//
// Created by osboxes on 13/04/2021.
//

#ifndef CPP_PARTMANGER_H
#define CPP_PARTMANGER_H

#include <list>
#include <memory>
#include <random>

#include "Particule.hpp"
#include "Drawer.hpp"

using namespace std;

class PartManager
{
    private:
    std::default_random_engine re;
    Drawer drawer;
public:
    PartManager();
    ~PartManager();

    list<unique_ptr<Particule>> particules;
    
    static const int NB_PARTICULE = 100;

    unique_ptr<Particule> factoryParticule();

    void growParticules();

    void drawParticules();

    void setSeed(std::default_random_engine);

};


#endif //CPP_PARTMANGER_H
