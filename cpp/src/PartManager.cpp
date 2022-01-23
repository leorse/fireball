#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

#include "PartManager.hpp"
#include "App.hpp"

using namespace std;

PartManager::PartManager()
{
    //création des particules
    for (int inc = 0; inc < PartManager::NB_PARTICULE; inc++)
    {
        this->particules.push_back(this->factoryParticule());
    }
}

PartManager::~PartManager()
{
    cout << "partmanager détruit, taille:" << this->particules.size() << endl;
}

unique_ptr<Particule> PartManager::factoryParticule()
{

    std::uniform_int_distribution<int> uit{0, 2};

    int type = uit(re);
    if (type == static_cast<int>(TypeParticule::COMET))
    {
        auto part = make_unique<Comet>(re);
        part->initLife(false);
        part->setRefX(App::LARGEUR / 2);
        part->setRefY(App::HAUTEUR / 2);
        return part;
    }
    if (type == static_cast<int>(TypeParticule::FIREBALL))
    {
        auto part = make_unique<Comet>(re);
        part->setRefX(App::LARGEUR / 2);
        part->setRefY(App::HAUTEUR / 2);
        return part;
    }
    auto part = make_unique<Comet>(re);
    part->initLife(false);
    part->setRefX(App::LARGEUR / 2);
    part->setRefY(App::HAUTEUR / 2);
    return part;
}

void PartManager::setSeed(std::default_random_engine re)
{
    this->re.seed(re());
}

void PartManager::growParticules()
{
    for (auto it = this->particules.begin(); it != this->particules.end(); it++)
    {
        auto particule = it->get();
        particule->grow();
        if (!particule->isAlive())
        {
            if (particule->isEphemere())
            {
                it = this->particules.erase(it);
            }
            else if( particule->isExplosive() )
            {
                this->addParticules(20,particule->getX()+particule->getRefX(), particule->getY()+particule->getRefY());
                particule->initLife(false);
            }
            else
            {
                particule->initLife(false);
            }
        }
    }
}

void PartManager::addParticules(int nombre, int x, int y)
{
    //création des particules
    for (int inc = 0; inc < nombre; inc++)
    {
        unique_ptr<Particule> part = this->factoryParticule();
        part->setRefX(x);
        part->setRefY(y);
        part->setEphemere(true);
        this->particules.push_back(std::move(part));
    }
}

void PartManager::drawParticules()
{
    for (auto &it:  this->particules)
    {
        auto particule = it.get();
        this->drawer.afficheurTrainee(particule);
    }
}
