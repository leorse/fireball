#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Particule.hpp"


Comet::Comet()
{
    this->setType(TypeParticule::COMET);
}

void Comet::move(void)
{
    float oldX = this->x;
    float oldY = this->y;
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
    std::cout<<"x:"<<this->GetX()<<",y:"<<this->GetY()<<",tps:"<<this->GetTps()<<",vie:"<<this->GetVie()<<std::endl;
}

void Comet::grow(void)
{
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

float Particule::GetX() const
{
    return this->x;
}

void Particule::SetX(float x)
{
    this->x = x;
}

float Particule::GetY() const
{
    return this->y;
}

void Particule::SetY(float y)
{
    this->y = y;
}

float Particule::GetTps() const
{
    return this->tps;
}

void Particule::SetTps(float tps)
{
    this->tps = tps;
}

float Particule::GetDir() const
{
    return this->dir;
}

void Particule::SetDir(float dir)
{
    this->dir = dir;
}

float Particule::GetPoids() const
{
    return this->poids;
}

void Particule::SetPoids(float poids)
{
    this->poids = poids;
}

int Particule::GetVitesse() const
{
    return this->vitesse;
}

void Particule::SetVitesse(int vitesse)
{
    this->vitesse = vitesse;
}

int Particule::GetTaille() const
{
    return this->taille;
}

void Particule::SetTaille(int taille)
{
    this->taille = taille;
}

int Particule::GetVie() const
{
    return this->vie;
}

void Particule::SetVie(int vie)
{
    this->vie = vie;
}

double Particule::GetDX() const
{
    return this->dX;
}

void Particule::SetDX(double dX)
{
    this->dX = dX;
}

double Particule::GetDY() const
{
    return this->dY;
}

void Particule::SetDY(double dY)
{
    this->dY = dY;
}

bool Particule::GetEphemere() const
{
    return this->ephemere;
}

void Particule::SetEphemere(bool ephemere)
{
    this->ephemere = ephemere;
}

float Particule::GetRefX() const
{
    return this->refX;
}

void Particule::SetRefX(float refX)
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