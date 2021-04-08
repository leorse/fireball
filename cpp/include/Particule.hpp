#pragma once

enum class TypeParticule
{
    FIREBALL = 0,
    COMET = 1
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

public:
    static const int MAX_VITESSE = 45;
    static const int MAX_TAILLE = 6;
    static const int MAX_DUREE = 25;

    float
    GetX() const;
    void SetX(float x);

    TypeParticule getType() const;
    void setType(TypeParticule);

    float GetY() const;
    void SetY(float y);

    float GetTps() const;
    void SetTps(float tps);

    float GetDir() const;
    void SetDir(float dir);

    float GetPoids() const;
    void SetPoids(float poids);

    int GetVitesse() const;
    void SetVitesse(int vitesse);

    int GetTaille() const;
    void SetTaille(int taille);

    int GetVie() const;
    void SetVie(int vie);

    double GetDX() const;
    void SetDX(double dX);

    double GetDY() const;
    void SetDY(double dY);

    bool GetEphemere() const;
    void SetEphemere(bool ephemere);

    float GetRefX() const;
    void SetRefX(float refX);

    bool isAlive();

    void initLife(bool);

    virtual void grow(void){};
    virtual void move(void){};

    Particule();
    ~Particule(){};
};

class Comet : public Particule
{
public:
    Comet();
    ~Comet();

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