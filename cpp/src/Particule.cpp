#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Particule.hpp"

using namespace std;

Comet::Comet()
{
    this->setType(TypeParticule::COMET);
}

Comet::~Comet()
{
    std::cout<<"particule détruite"<<std::endl;
}

void Comet::move(void)
{
    float oldX = this->x;
    float oldY = this->y;
    cout<<"je bouge!!"<<endl;
    if(this->tps==0)
    {
        oldX = 0;
        oldY = 0;
    }
    this->tps += 0.1f;
    float temps = this->tps;
    
    this->x = (double)(this->vitesse * temps * cos((this->dir + temps * 10) * M_PI / 180));
    this->y = (double)(this->vitesse * temps * sin((this->dir - temps * 10) * M_PI / 180) - (this->poids / 2) * (temps * temps));

    this->dX = (this->x-oldX)*5;
    this->dY = (this->y-oldY)*5;
    std::cout<<"x:"<<this->getX()<<",y:"<<this->getY()<<",tps:"<<this->getTps()<<",vie:"<<this->getVie()<<std::endl;
}

void Comet::grow(void)
{cout<<"je bouge ici aussi"<<endl;
    this->move();
}

//////// Particule

Particule::Particule()
{
    this->initLife(false);
}

void Particule::initLife(bool ephemere)
{
    std::srand(std::time(nullptr));
    //une direction entre 1 et 360°
    this->dir = std::rand() % 360;
    this->poids = -5;
    this->vitesse = 25 + (std::rand() % Particule::MAX_VITESSE);
    this->taille = 1 + (std::rand() % Particule::MAX_TAILLE);
    this->tps = 0;
    this->vie = 1 + (std::rand() % Particule::MAX_DUREE);
    this->x = 0;
    this->y = 0;
    this->dX = 0;
    this->dY = 0;
    this->ephemere = false;
}

bool Particule::isAlive()
{
    return this->tps<this->vie;
}

float Particule::getX() const
{
    return this->x;
}

void Particule::setX(float x)
{
    this->x = x;
}

float Particule::getY() const
{
    return this->y;
}

void Particule::setY(float y)
{
    this->y = y;
}

float Particule::getTps() const
{
    return this->tps;
}

void Particule::setTps(float tps)
{
    this->tps = tps;
}

float Particule::getDir() const
{
    return this->dir;
}

void Particule::setDir(float dir)
{
    this->dir = dir;
}

float Particule::getPoids() const
{
    return this->poids;
}

void Particule::setPoids(float poids)
{
    this->poids = poids;
}

int Particule::getVitesse() const
{
    return this->vitesse;
}

void Particule::setVitesse(int vitesse)
{
    this->vitesse = vitesse;
}

int Particule::getTaille() const
{
    return this->taille;
}

void Particule::setTaille(int taille)
{
    this->taille = taille;
}

int Particule::getVie() const
{
    return this->vie;
}

void Particule::setVie(int vie)
{
    this->vie = vie;
}

double Particule::getDX() const
{
    return this->dX;
}

void Particule::setDX(double dX)
{
    this->dX = dX;
}

double Particule::getDY() const
{
    return this->dY;
}

void Particule::setDY(double dY)
{
    this->dY = dY;
}

bool Particule::getEphemere() const
{
    return this->ephemere;
}

void Particule::setEphemere(bool ephemere)
{
    this->ephemere = ephemere;
}

float Particule::getRefX() const
{
    return this->refX;
}

void Particule::setRefX(float refX)
{
    this->refX = refX;
}

TypeParticule Particule::getType() const
{
    return this->type;
}

void Particule::setType(TypeParticule type)
{
    this->type = type;
}