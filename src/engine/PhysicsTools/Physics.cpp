#include "engine/Physics/Physics.h"
#include "engine/RenderTools/notificationTool.h"
#include "engine/RenderTools/SuccessTool.h"

Physics::Physics(RenderContext* renderContext):m_renderContext(renderContext){
}

void Physics::Update(double dt){
    for (auto& object : m_renderContext->systemeSolaire->objects){

        updateAccel(object, dt);
        updateVelocity(object, dt);
        updatePosition(object, dt);
        updateRotation(object, dt);
        checkIfTooFar(object);
        if(*(m_renderContext->simulationTime)>0){checkCollision(m_renderContext->systemeSolaire->objects);}
    }
}


void Physics::updateAccel(CelestialObject* obj, double dt){
    Vec3 total_accel(0.0, 0.0, 0.0);
    CelestialObject* mostInfluentialObject = nullptr;
    double maxForce = 0.0;

    for (auto& other_obj : m_renderContext->systemeSolaire->objects){
        if(obj != other_obj){
            Vec3 vector_direction = Vec3(other_obj->position_real - obj->position_real);
            double distance = vector_direction.norm();
            Vec3 unit_vector = vector_direction.normalize();
            double forceMagnitude = G * other_obj->weight / (distance * distance);
            if (forceMagnitude > maxForce) {
                maxForce = forceMagnitude;
                mostInfluentialObject = other_obj;
            }
            Vec3 accel_vector = unit_vector * forceMagnitude;
            total_accel = total_accel + accel_vector;
        }
    }

    obj->updateAccel(total_accel);
    obj->setMostInfluentialObject(mostInfluentialObject); // Stocker l'objet le plus influent
}

void Physics::updateVelocity(CelestialObject* obj, double dt){
    //Vec3 updated_velocity = obj->velocity + (obj->accel * dt);
    double vel_x = obj->velocity.x + obj->accel.x * dt;
    double vel_y = obj->velocity.y+ obj->accel.y * dt;
    double vel_z = obj->velocity.z+ obj->accel.z * dt;
    //if(obj->getName()=="Terre"){std::cout<<vel_x<<", "<<vel_y<<", "<<vel_z<<std::endl;}
    obj->updateVelocity(Vec3(vel_x,vel_y,vel_z));
}

void Physics::updatePosition(CelestialObject* obj, double dt){
    double pos_x = obj->position_real.x + obj->velocity.x * dt;
    double pos_y = obj->position_real.y+ obj->velocity.y * dt;
    double pos_z = obj->position_real.z+ obj->velocity.z * dt;
    //Vec3 updated_real_position = obj->position_real + obj->velocity * dt;
    obj->updatePositionReal(Vec3(pos_x,pos_y,pos_z));
    obj->updatePositionSimulation();
    obj->addPositionHistory(obj->position_simulation);
}

void Physics::updateRotation(CelestialObject* obj, double dt){
    obj->rotationSid += (obj->rotationSidSpeed*dt*360);
    //if(obj->getName()=="Earth"){
    //std::cout<<"rorationSid: "<<obj->rotationSid<<" Speed: "<<obj->rotationSidSpeed<<" dt:"<<dt*60<<std::endl;}
} 


void Physics::checkIfTooFar(CelestialObject* obj){
    CelestialObject* sun = m_renderContext->systemeSolaire->getSun(obj);
    if(sun == nullptr){return;}
    double distanceFromSun = (obj->getPositionSimulation()-sun->getPositionSimulation()).norm();
    double distanceFromOrigin = obj->getPositionSimulation().norm();

    if(distanceFromSun > m_renderContext->systemeSolaire->maxSize*2 or distanceFromOrigin > m_renderContext->systemeSolaire->maxSize*2){
        if(*(m_renderContext->simulationTime)>100){
        m_renderContext->systemeSolaire->removeObject(obj);}
    }
}


void Physics::checkCollision(std::vector<CelestialObject*> objects){
    isColliding = false;
    float collisionThreshold = 0.30; // Seuil pour la fonction collision
    float collidingThreshold = 1.00; // Seuil pour activer isColliding
    for (int i = 0; i < objects.size(); i++){
        for (int j = i + 1; j < objects.size(); j++){
            Vec3 positionObj = objects[i]->getPositionSimulation();
            Vec3 positionOther = objects[j]->getPositionSimulation();

            float rayonObj = objects[i]->getRayon();
            float rayonOther = objects[j]->getRayon();

            Vec3 diff = positionObj - positionOther;
            float distance = diff.norm(); // Calcule la distance entre les centres

            // Détection de collision pour activer isColliding
            if (distance <= (rayonObj + rayonOther) * collidingThreshold) {
                collisionForToolFunction(objects[i], objects[j]);
                isColliding = true;
            }

            bool isCompletelyInside = distance + std::min(rayonObj, rayonOther) <= std::max(rayonObj, rayonOther);


            // Détection de collision pour la fonction collision
            if (distance <= (rayonObj + rayonOther) * collisionThreshold || isCompletelyInside) {
                collision(objects[i], objects[j]);
                isColliding = false;
            }
            if(!isColliding){
                endCollisionForToolFunction(objects[i], objects[j]);
            }
        }
    }
}



void Physics::collision(CelestialObject* obj1, CelestialObject* obj2){
    float m1 = obj1->getWeight();
    float m2 = obj2->getWeight();
    float r1 = obj1->real_radius;
    float r2 = obj2->real_radius;

    m_renderContext->successTool->checkCollision(obj1, obj2);


    Vec3 v1 = obj1->velocity;
    Vec3 v2 = obj2->velocity;
    Vec3 pos1 = obj1->position_real;
    Vec3 pos2 = obj2->position_real;

    // Calculer la nouvelle masse
    float new_mass = m1 + m2;
    float new_rayon = r1 + r2;

    // Calculer la nouvelle vitesse
    Vec3 new_velocity;
    new_velocity.x = (v1.x * m1 + v2.x * m2) / new_mass;
    new_velocity.y = (v1.y * m1 + v2.y * m2) / new_mass;
    new_velocity.z = (v1.z * m1 + v2.z * m2) / new_mass;

    CelestialObject* objectRestant;

    if(obj1->getTypeName() == "BlackHole"){
        if(!(obj2->getTypeName() == "BlackHole") && !(obj2->getRayon()>obj1->getRayon())){
        obj1->setWeight(new_mass);
        obj1->updateVelocity(new_velocity);
        obj1->addPlanetEaten(obj2);
        m_renderContext->systemeSolaire->removeObject(obj2);
        objectRestant = obj1;
        if(obj1->typeName != "Moon" && obj2->typeName != "Moon"){
        sendMessageCollsion(obj1, obj2, objectRestant);}
        return;}
    }if(obj2->getTypeName() == "BlackHole"){
        obj2->setWeight(new_mass);
        obj2->updateVelocity(new_velocity);
        obj2->addPlanetEaten(obj1);
        m_renderContext->systemeSolaire->removeObject(obj1);
        objectRestant = obj2;
        if(obj1->typeName != "Moon" && obj2->typeName != "Moon"){
        sendMessageCollsion(obj1, obj2, objectRestant);}
        return;
    }

    else{//Not black holes
    if(m1 > m2){
        obj1->setWeight(new_mass);
        obj1->updateVelocity(new_velocity);
        obj1->real_radius = new_rayon;
        obj1->addPlanetEaten(obj2);
        m_renderContext->systemeSolaire->setRayon(obj1);
        m_renderContext->systemeSolaire->removeObject(obj2);
        objectRestant = obj1;
        if(obj1->typeName != "Moon" && obj2->typeName != "Moon"){
        sendMessageCollsion(obj1, obj2, objectRestant);}
        return;
    } else if(m2 > m1){
        obj2->setWeight(new_mass);
        obj2->updateVelocity(new_velocity);
        obj2->real_radius = new_rayon;
        obj2->addPlanetEaten(obj1);
        m_renderContext->systemeSolaire->setRayon(obj2);
        m_renderContext->systemeSolaire->removeObject(obj1);
        objectRestant = obj2;
        if(obj1->typeName != "Moon" && obj2->typeName != "Moon"){
        sendMessageCollsion(obj1, obj2, objectRestant);}
        return;
    }
    else{
        obj1->setWeight(new_mass);
        obj1->updateVelocity(new_velocity);
        obj1->real_radius = new_rayon;
        obj1->addPlanetEaten(obj2);
        m_renderContext->systemeSolaire->setRayon(obj1);
        m_renderContext->systemeSolaire->removeObject(obj2);
        objectRestant = obj1;
        if(obj1->typeName != "Moon" && obj2->typeName != "Moon"){
        sendMessageCollsion(obj1, obj2, objectRestant);}
        return;
    }
    }

}

void Physics::sendMessageCollsion(CelestialObject* obj1, CelestialObject* obj2, CelestialObject* objectRestant){
    m_renderContext->NotificationTool->Open("Collision", "Collision between " + obj1->getName() + " and " + obj2->getName()+"!", "See", "Cancel", [this, objectRestant](){ m_renderContext->currentCamera->newFollowObject(objectRestant); });
}

void Physics::setCollisionFunction(const std::function<void(CelestialObject*, CelestialObject*)>& func) {
    collisionForToolFunction = func;
}

void Physics::setEndCollisionFunction(const std::function<void(CelestialObject*, CelestialObject*)>& func) {
    endCollisionForToolFunction = func;
}
