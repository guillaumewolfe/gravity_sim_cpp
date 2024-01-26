// UITool.h
#ifndef BACKGROUNDTOOL_H
#define BACKGROUNDTOOL_H

#include "RenderComponent.h"
#include "engine/Vec3.h"

struct Star {
    GLfloat x, y, z;     // Position
    GLfloat size;        // Taille actuelle
    GLfloat r, g, b;     // Couleur
    GLfloat alpha;       // Transparence actuelle
    GLfloat originalSize;   // Taille originale
    GLfloat originalAlpha;  // Transparence originale
    int indice;
    GLfloat flickerOffset;
    bool isMoving = false;
    float angle = 0; // Angle autour de la sphère pour le mouvement
    float speedPhi = 0; // Vitesse de déplacement autour de la sphère
    float speedTheta = 0; // Vitesse de déplacement autour de la sphère
    float theta = 0;
    float phi = 0;
};



class BackgroundTool : public RenderComponent {
public:
    explicit BackgroundTool(RenderContext* renderContext);
    ~BackgroundTool() override;
    void Draw() override;


private:

    std::vector<Star> stars;  // Liste des étoiles
    int numberOfStars = 0;
    void initStars(int numberOfStars);
    int drawIterations;

    GLuint texture;
    GLuint loadTexture(const char* filename);
    void drawImage();

};

#endif // BACKGROUNDTOOL_H