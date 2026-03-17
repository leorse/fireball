#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shader.h"
#include "contexte.h"

/* ------------------------------------------------------------------ */
/* Internal helpers                                                     */
/* ------------------------------------------------------------------ */

static char *lireFichier(const char *chemin)
{
    FILE *f = fopen(chemin, "rb");
    if (!f)
    {
        fprintf(stderr, "shader: impossible d'ouvrir '%s'\n", chemin);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long taille = ftell(f);
    rewind(f);
    char *buf = (char *)malloc(taille + 1);
    if (!buf) { fclose(f); return NULL; }
    fread(buf, 1, taille, f);
    buf[taille] = '\0';
    fclose(f);
    return buf;
}

static GLuint compiler(GLenum type, const char *chemin)
{
    char *src = lireFichier(chemin);
    if (!src) return 0;

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const char **)&src, NULL);
    free(src);
    glCompileShader(shader);

    GLint ok;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok)
    {
        char log[1024];
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        fprintf(stderr, "shader '%s' erreur de compilation:\n%s\n", chemin, log);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

static GLuint lierProgram(GLuint vert, GLuint frag)
{
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog);

    GLint ok;
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok)
    {
        char log[1024];
        glGetProgramInfoLog(prog, sizeof(log), NULL, log);
        fprintf(stderr, "program erreur de link:\n%s\n", log);
        glDeleteProgram(prog);
        return 0;
    }
    return prog;
}

static void creerTexture(GLuint *tex, GLint internalFormat,
                         int w, int h,
                         GLenum format, GLenum type)
{
    glGenTextures(1, tex);
    glBindTexture(GL_TEXTURE_2D, *tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, type, NULL);
}

/* ------------------------------------------------------------------ */
/* Public API                                                           */
/* ------------------------------------------------------------------ */

void mettreAJourPalette(CONTEXTE *contexte)
{
    uint8_t data[256 * 4];
    for (int i = 0; i < 256; i++)
    {
        data[i * 4 + 0] = contexte->listeCouleur[i].r;
        data[i * 4 + 1] = contexte->listeCouleur[i].g;
        data[i * 4 + 2] = contexte->listeCouleur[i].b;
        data[i * 4 + 3] = 255;
    }
    glBindTexture(GL_TEXTURE_2D, contexte->texPalette);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 1,
                    GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void redimensionnerGL(CONTEXTE *contexte, int nouvLargeur, int nouvHauteur)
{
    glViewport(0, 0, nouvLargeur, nouvHauteur);

    /* Re-allocate size-dependent textures */
    glBindTexture(GL_TEXTURE_2D, contexte->texHeightmap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, nouvLargeur, nouvHauteur, 0,
                 GL_RED, GL_UNSIGNED_BYTE, NULL);

    glBindTexture(GL_TEXTURE_2D, contexte->texCache);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, nouvLargeur, nouvHauteur, 0,
                 GL_RED, GL_UNSIGNED_BYTE, NULL);

    glBindTexture(GL_TEXTURE_2D, contexte->texHUD);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nouvLargeur, nouvHauteur, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}

bool initGL(CONTEXTE *contexte)
{
    /* Initialise GLEW */
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        fprintf(stderr, "glewInit: %s\n", glewGetErrorString(err));
        return false;
    }

    /* Fullscreen quad (NDC triangle strip, 2D positions) */
    float quadVertices[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f,
    };

    glGenVertexArrays(1, &contexte->quadVAO);
    glGenBuffers(1, &contexte->quadVBO);

    glBindVertexArray(contexte->quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, contexte->quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    /* Compile shared vertex shader */
    GLuint vert = compiler(GL_VERTEX_SHADER, "shaders/quad.vert");
    if (!vert) return false;

    /* Compile fragment shaders */
    GLuint fragFireball = compiler(GL_FRAGMENT_SHADER, "shaders/fireball.frag");
    GLuint fragLight    = compiler(GL_FRAGMENT_SHADER, "shaders/light.frag");
    GLuint fragShadow   = compiler(GL_FRAGMENT_SHADER, "shaders/shadow.frag");
    GLuint fragGlass    = compiler(GL_FRAGMENT_SHADER, "shaders/glass.frag");
    GLuint fragHUD      = compiler(GL_FRAGMENT_SHADER, "shaders/hud.frag");

    if (!fragFireball || !fragLight || !fragShadow || !fragGlass || !fragHUD)
    {
        glDeleteShader(vert);
        return false;
    }

    /* Link programs: [0]=fireball [1]=light [2]=shadow [3]=glass [4]=hud */
    contexte->programs[0] = lierProgram(vert, fragFireball);
    contexte->programs[1] = lierProgram(vert, fragLight);
    contexte->programs[2] = lierProgram(vert, fragShadow);
    contexte->programs[3] = lierProgram(vert, fragGlass);
    contexte->programs[4] = lierProgram(vert, fragHUD);

    glDeleteShader(vert);
    glDeleteShader(fragFireball);
    glDeleteShader(fragLight);
    glDeleteShader(fragShadow);
    glDeleteShader(fragGlass);
    glDeleteShader(fragHUD);

    for (int i = 0; i < 5; i++)
    {
        if (!contexte->programs[i]) return false;
    }

    /* Create textures */
    int w = contexte->largeur;
    int h = contexte->hauteur;

    creerTexture(&contexte->texHeightmap, GL_R8,    w,   h,   GL_RED,  GL_UNSIGNED_BYTE);
    creerTexture(&contexte->texCache,     GL_R8,    w,   h,   GL_RED,  GL_UNSIGNED_BYTE);
    creerTexture(&contexte->texHUD,       GL_RGBA8, w,   h,   GL_RGBA, GL_UNSIGNED_BYTE);
    creerTexture(&contexte->texPalette,   GL_RGBA8, 256, 1,   GL_RGBA, GL_UNSIGNED_BYTE);

    /* Upload initial palette */
    mettreAJourPalette(contexte);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    return true;
}

void detruireGL(CONTEXTE *contexte)
{
    glDeleteTextures(1, &contexte->texHeightmap);
    glDeleteTextures(1, &contexte->texCache);
    glDeleteTextures(1, &contexte->texHUD);
    glDeleteTextures(1, &contexte->texPalette);

    for (int i = 0; i < 5; i++)
    {
        glDeleteProgram(contexte->programs[i]);
    }

    glDeleteBuffers(1, &contexte->quadVBO);
    glDeleteVertexArrays(1, &contexte->quadVAO);

    SDL_GL_DeleteContext(contexte->glContext);
}
