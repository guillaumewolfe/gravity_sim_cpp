#include "glad/glad.h"
#include "engine/CelestialObject.h"
#include <cmath>
#include <tuple>
#include <string>
#include "opencv2/opencv.hpp"



//Constructeur avec NAME + TEXTURE
CelestialObject::CelestialObject(){}

// Constructeur avec tout
CelestialObject::CelestialObject(std::string name, std::string texture_path, double real_radius, double weight, int object_type, int distance_initiale, double rayon_simulation,
    const Vec3& pos_real, const Vec3& velocity, const Vec3& force ,const Vec3& accel ,const Vec3& pos_simul)
: position_real(pos_real),velocity(velocity),force(force),accel(accel),position_simulation(pos_simul),
name(name),texture_path(texture_path),real_radius(real_radius),weight(weight),distance_initiale(distance_initiale),rayon_simulation(rayon_simulation){}


void CelestialObject::updatePositionReal(const Vec3& newPosReal){
    position_real = newPosReal;
};
void CelestialObject::updatePositionSimulation(){
    position_simulation.x = position_real.x * distanceScale;
    position_simulation.y = position_real.y * distanceScale;
    position_simulation.z = position_real.z * distanceScale;
};
void CelestialObject::updateVelocity(const Vec3& newVel){
    velocity = newVel;
};
void CelestialObject::updateAccel(const Vec3& newAcel){
    accel = newAcel;
};
void CelestialObject::updateForce(const Vec3& newForce){
    force = newForce;
};

//NAME
std::string CelestialObject::getName(){return name;};
void CelestialObject::setName(std::string nm){name = nm;};
//PATH
std::string CelestialObject::getPath(){return texture_path;};
void CelestialObject::setPath(std::string path){texture_path = path;};
//RAYON
void CelestialObject::setRayonSim(double scale){
    rayon_simulation = real_radius*scale;
    //rayon_simulation=3;
    }
double CelestialObject::getRayon(){return rayon_simulation;}
void CelestialObject::realRadiusToSimRadius(){
    rayon_simulation= radiusScale*real_radius;}

Vec3 CelestialObject::getPositionSimulation(){
    return Vec3(position_simulation.x,position_simulation.y,position_simulation.z);}

GLuint CelestialObject::getTexture(){return textureID;}









GLuint CelestialObject::loadTexture(const char* filename) {
    cv::Mat image = cv::imread(filename);
    if (image.empty()) {
        std::cerr << "Erreur: Image non trouvée " << filename << std::endl;
        return 0;
    }
    cv::flip(image, image, 0); // Inversion verticale de l'image

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}


void CelestialObject::addPositionHistory(const Vec3& point) {
    positionHistory.push_back(point);
    if (positionHistory.size() > MAX_HISTORY_SIZE) {
        positionHistory.erase(positionHistory.begin());
    }
}

const std::vector<Vec3>& CelestialObject::getPositionHistory(){
    return positionHistory;
}

void CelestialObject::clearPositionHistory() {
    positionHistory.clear();
}


void CelestialObject::updatePositionSimulation(const Vec3& newPosSimulation){
    position_simulation = newPosSimulation;

    if (distanceScale != 0) {
        position_real.x = position_simulation.x / distanceScale;
        position_real.y = position_simulation.y / distanceScale;
        position_real.z = position_simulation.z / distanceScale;
    }
}

Vec3 CelestialObject::getRealPosition(){
    return position_real;
}



double CelestialObject::getDistanceFromOrigin() {
    return position_real.norm();
}

double CelestialObject::getWeight(){
    return weight;
}



void CelestialObject::setWeight(double newWeight){
    weight = newWeight;
}