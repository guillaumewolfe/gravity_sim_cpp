#ifndef CELESTIALOBJECT_H
#define CELESTIALOBJECT_H


#include <string>
#include <vector>
#include "engine/Vec3.h"
#include <GLFW/glfw3.h>


class CelestialObject{
protected:
    CelestialObject();
public:
    std::string name;
    std::string texture_path;
    double real_radius = 1;
    double weight = 1;
    double distance_initiale = 1;
    double rayon_simulation = 1;
    float inclinaison;
    double rotationSid;
    double rotationSidSpeed;
    Vec3 rotationSidDirection;
    
    const int MAX_HISTORY_SIZE = 500;
    std::vector<Vec3> positionHistory;

    int type;
    GLuint textureID;

    //SCALE
    double radiusScale = 1;
    double distanceScale = 1/(149597870e3/2);


    Vec3 position_real = {0,0,0};
    Vec3 velocity = {0,0,0};
    Vec3 force = {0,0,0};
    Vec3 accel = {0,0,0};
    Vec3 position_simulation = {0,0,0};
    
    GLuint vao;
    GLuint vboVertices, vboNormals, vboTexCoords;
    int vertexCount;
    // Autres données nécessaires...

    // Constructeur avec tout
    CelestialObject(std::string name, std::string texture_path, double real_radius, double weight, int object_type, int distance_initiale, double rayon_simulation,
    const Vec3& pos_real = Vec3(0,0,0), const Vec3& velocity = Vec3(0,0,0), const Vec3& force = Vec3(0,0,0),const Vec3& accel = Vec3(0,0,0),const Vec3& pos_simul = Vec3(0,0,0));


    //Update methodes to change their values
    virtual void updatePositionReal(const Vec3& newPosReal);
    virtual void updatePositionSimulation();
    virtual Vec3 getPositionSimulation();
    virtual void updateVelocity(const Vec3& newVel);
    virtual void updateAccel(const Vec3& newAcel);
    virtual void updateForce(const Vec3& newForce);
    GLuint loadTexture(const char* filename);
    GLuint getTexture();

    virtual void setName(std::string);
    virtual std::string getName();

    virtual void setPath(std::string);
    virtual std::string getPath();

    virtual void setRayonSim(double scale);
    virtual double getRayon();
    virtual void realRadiusToSimRadius();

    void addPositionHistory(const Vec3& point);
    const std::vector<Vec3>& getPositionHistory();
    void clearPositionHistory();



};

#endif


