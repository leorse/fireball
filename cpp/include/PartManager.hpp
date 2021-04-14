//
// Created by osboxes on 13/04/2021.
//

#ifndef CPP_PARTMANGER_H
#define CPP_PARTMANGER_H

#include <list>
#include <memory>

#include "Particule.hpp"

using namespace std;

class PartManager
{
public:
    PartManager();
    ~PartManager();

    list<unique_ptr<Particule>> particules;
    static const int NB_PARTICULE = 100;

    unique_ptr<Particule> factoryParticule();

    void growParticules();

    void drawParticules();

};


#endif //CPP_PARTMANGER_H
