# Fireball
A fireball in different language based on my very old code (1998) first done in C on DOS.
Still in 256 colors in SDL2.

## Class Diagram

```mermaid
classDiagram
    METEOR<|--FireBall
    METEOR<|--Comet
    METEOR*--Contexte
    class METEOR {
        METEOR* suivant;
        METEOR* precedent
        int x
        int y
        double dY
        double dX
        double vie;
        double tps;

        InitMeteor()
        Drawer()
        Mover()
    }

    class FireBall {

    }
    class Comet{
        
    }

    class Contexte {
        METEOR* premier
        METEOR* dernier
        SDL_Color palette
    }
```


## C
Basiqualiy the same code that I've done in 1998.

### Compile on windows
Use the make file
```
mingw32-make.exe
```

