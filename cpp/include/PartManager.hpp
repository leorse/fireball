//
// Created by osboxes on 13/04/2021.
//

#ifndef CPP_PARTMANGER_H
#define CPP_PARTMANGER_H

#include <list>

#include "Particule.hpp"


class PartManager
{
public:
    PartManager();
    ~PartManager();
    PartManager(const std::list<Particule> &particules);

    std::list<Particule> particules;
    static const int NB_PARTICULE = 1;

    Particule factoryParticule();

    void growParticules();

    void drawParticules();

};


#endif //CPP_PARTMANGER_H
