#pragma once

#include <random>

enum class TypeParticule
{
    FIREBALL = 0,
    COMET = 1,
    NONE = 2
};

class Particule
{
protected:
    float x;
    float y;
    float tps;
    float dir;
    float poids;
    int vitesse;
    int taille;
    int vie;
    double dX;
    double dY;

    TypeParticule type;

    bool ephemere;

    float refX, refY;

    std::default_random_engine re;

public:
    static const int MAX_VITESSE = 45;
    static const int MAX_TAILLE = 5;
    static const int MAX_DUREE = 25;

    void setSeed(std::default_random_engine);

    float getX() const;

    void setX(float x);

    TypeParticule getType() const;

    void setType(TypeParticule);

    float getY() const;

    void setY(float y);

    float getTps() const;

    void setTps(float tps);

    float getDir() const;

    void setDir(float dir);

    float getPoids() const;

    void setPoids(float poids);

    int getVitesse() const;

    void setVitesse(int vitesse);

    int getTaille() const;

    void setTaille(int taille);

    int getVie() const;

    void setVie(int vie);

    double getDX() const;

    void setDX(double dX);

    double getDY() const;

    void setDY(double dY);

    bool getEphemere() const;

    void setEphemere(bool ephemere);

    float getRefX() const;

    void setRefX(float refX);

    float getRefY() const;

    void setRefY(float refY);

    bool isAlive();

    void initLife(bool);

    virtual void grow(void)
    {
    };

    virtual void move(void)
    {
    };

    void quisuisJe(void);

    Particule();

    virtual ~Particule();
};

class Comet : public Particule
{
public:
    Comet(std::default_random_engine);

    ~Comet();

    void quisuisJe(void);

    void grow(void);

    void move(void);
};
/*
class FireBall : public Particule
{
public:
    FireBall();
    ~FireBall();

    void grow(void);
    void move(void);
};*/