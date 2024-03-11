#ifndef CELESTIALOBJECT_H
#define CELESTIALOBJECT_H


#include <string>
#include <vector>
#include "engine/Vec3.h"
#include <GLFW/glfw3.h>

class GlowTool;
class AthmosphereTool;
class SaturnRingTool;
class UranusRingTool;

class CelestialObject{
protected:
    CelestialObject();
public:
    std::string name = "Default";
    std::string typeName = "Default";
    std::string texture_path = "Default";
    double real_radius = 5000;
    double weight = 1;
    double defaultWeight= 1;
    double defaultRadius= 1;
    std::string defaultName= "Default";
    double distance_initiale = 1;
    double rayon_simulation = 1;
    float inclinaison= 0;
    double rotationSid= 0;
    double rotationSidSpeed= 0;
    Vec3 rotationSidDirection = {0,0,0};
    bool shouldBeDrawn = true;
    std::string orbitalString=  "Default";
    std::string story= "Default";
    std::string temperature= "Default";
    std::string composition= "Default";
    Vec3 initialVelocity= {0,0,0};
    double distanceFromPlanet; //For satelite
    bool isCreated = false;
    double totalDistance =0; //Distance pour le calcul de la grandeur du Path
    double orbitCircumference=0;
    bool showOrbit = false;
    bool showPath = true;
    bool showName = true;
    bool isDeleted = false;
    bool isColliding = false;
    CelestialObject* mostInfluentialObject = nullptr;
    void setMostInfluentialObject(CelestialObject* newMostInfluentialObject);
    CelestialObject* getMostInfluentialObject();
    std::vector<CelestialObject*> planetsEaten;

    double initialWeight = 1;
    double initialRadius = 1;   
    
    const int MAX_HISTORY_SIZE = 500;
    std::vector<Vec3> positionHistory;

    int type= 0;
    GLuint textureID= 0;
    GLuint textureID2= 0;

    //SCALE
    double radiusScale = 1;
    double distanceScale = 1;

    GlowTool* glowTool = nullptr;
    AthmosphereTool* athmosphereTool = nullptr;
    SaturnRingTool* saturnRingTool = nullptr;
    UranusRingTool* uranusRingTool = nullptr;
    

    Vec3 position_real = {0,0,0};
    Vec3 velocity = {0,0,0};
    Vec3 force = {0,0,0};
    Vec3 accel = {0,0,0};
    Vec3 position_simulation = {0,0,0};
    Vec3 nasaPosition = {0,0,0};
    Vec3 nasaVelocity = {0,0,0};

    Vec3 defaultPosition = {0,0,0};
    Vec3 defaultVelocity = {0,0,0};
    
    GLuint vao = 0;
    GLuint vboVertices, vboNormals, vboTexCoords = 0;
    int vertexCount = 0;
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
    std::string getTypeName();

    virtual void setPath(std::string);
    virtual std::string getPath();

    virtual void setRayonSim(double scale);
    virtual double getRayon();
    virtual void realRadiusToSimRadius();

    void addPositionHistory(const Vec3& point);
    const std::vector<Vec3>& getPositionHistory();
    void clearPositionHistory();
    void updatePositionSimulation(const Vec3& newPosSimulation);
    Vec3 getRealPosition();
    double getDistanceFromOrigin();
    double getWeight();
    void setWeight(double newWeight);
    void setInitialSettings();
    void updateToInitialSettings();
    void addPlanetEaten(CelestialObject* planet);
    std::vector<CelestialObject*> getPlanetsEaten();
    void resetPlanetEaten();
    double getDefaultWeight();
    double getDefaultRadius();
    std::string getDefaultName();
    void setDefaultValues();
    void resetToDefaultValues();
    Vec3 getVelocity();
    


};



#endif


