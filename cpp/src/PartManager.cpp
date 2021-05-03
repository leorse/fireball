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
    cout << "rand type:" << type << endl;
    if (type == static_cast<int>(TypeParticule::COMET))
    {
        auto part = make_unique<Comet>(re);
        part.get()->setRefX(App::LARGEUR / 2);
        part.get()->setRefY(App::HAUTEUR / 2);
        return part;
    }
    if (type == static_cast<int>(TypeParticule::FIREBALL))
    {
        auto part = make_unique<Comet>(re);
        part.get()->setRefX(App::LARGEUR / 2);
        part.get()->setRefY(App::HAUTEUR / 2);
        return part;
    }
    auto part = make_unique<Comet>(re);
    part.get()->setRefX(App::LARGEUR / 2);
    part.get()->setRefY(App::HAUTEUR / 2);
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
        //Particule& particule = it;
        auto particule = it->get();
        particule->grow();
        if (!particule->isAlive())
        {
            particule->initLife(false);
        }
    }
}

void PartManager::drawParticules()
{
    for (auto it = this->particules.begin(); it != this->particules.end(); it++)
    {
        //Particule& particule = it;
        auto particule = it->get();
        this->drawer.afficheurTrainee(particule);
    }
}
