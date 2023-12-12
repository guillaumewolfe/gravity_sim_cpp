#include "engine/Physics/Physics.h"

Physics::Physics(RenderContext* renderContext):m_renderContext(renderContext){
}

void Physics::Update(double dt){
    for (auto& object : m_renderContext->systemeSolaire->objects){
        updateAccel(object, dt);
        updateVelocity(object, dt);
        updatePosition(object, dt);
        updateRotation(object, dt);
    }
}

void Physics::updateAccel(CelestialObject* obj, double dt){
    Vec3 total_accel(0.0, 0.0, 0.0);
    for (auto& other_obj : m_renderContext->systemeSolaire->objects){
        if(obj != other_obj){
        Vec3 vector_direction = Vec3(other_obj->position_real - obj->position_real);
        double distance = vector_direction.norm();
        //if(obj->getName()=="Terre"&&other_obj->getName()=="Sun"){std::cout<<"Distance Terre et "<<other_obj->getName()<<": "<<distance<<std::endl;}
        Vec3 unit_vector = vector_direction.normalize();
        double accel_magnitude = G * other_obj->weight / (distance * distance);
        Vec3 accel_vector = unit_vector * accel_magnitude;
        total_accel = total_accel + accel_vector;
    //if(obj->getName()=="Terre"){std::cout<<"Terre et "<<other_obj->getName()<<": "<<accel_vector.print()<<std::endl;}
    }}
obj->updateAccel(total_accel);
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