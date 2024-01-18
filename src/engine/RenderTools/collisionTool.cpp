#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/RenderTools/collisionTool.h"
#include <random> // Pour std::rand et std::srand
#include <chrono>



CollisionTool::CollisionTool(RenderContext* renderContext) : RenderComponent(renderContext){
    shouldGenerateParticle = false;
}

void CollisionTool::newCollision(CelestialObject* obj1_, CelestialObject* obj2_){
    if(!shouldGenerateParticle){
        this->obj1 = obj1_;
        this->obj2 = obj2_;
        shouldGenerateParticle = true;
        computeFactor();
    }
}
void CollisionTool::endCollision(CelestialObject* obj1_, CelestialObject* obj2_){
    shouldGenerateParticle = false;
}

void CollisionTool::Draw() {
    if (obj1 == nullptr || obj2 == nullptr) {
        return; // Aucune collision à dessiner
    }
    //Si on arrête les collisions et la liste de particule est vide, on mets les pointeurs nuls
    if (obj1 != nullptr && obj2 != nullptr && !shouldGenerateParticle && particles.size() == 0) {
        obj1 = nullptr; 
        obj2 = nullptr;
        return;
    }

// Vos déclarations initiales restent inchangées
Vec3 positionObj1 = obj1->getPositionSimulation();
Vec3 positionObj2 = obj2->getPositionSimulation();
float rayon1 = obj1->getRayon();
float rayon2 = obj2->getRayon();

// Calcul du vecteur directeur et normalisation
Vec3 direction = positionObj2 - positionObj1;
float distance = direction.norm();
direction = direction.normalize(); // Assurez-vous que la méthode normalize existe dans votre classe Vec3

distanceMiddle = (pow(rayon1, 2) - pow(rayon2, 2)+ pow(distance, 2)) / (2 * distance);

midpoint = positionObj1 + direction*distanceMiddle;

normal = (positionObj2 - positionObj1).normalize();

    if(!*(m_renderContext->isPaused)){
        if(shouldGenerateParticle){
            GenerateParticle();
        }
    }
    if(shouldGenerateParticle){
        drawCircleIntersection();
    }
    updateParticles();
    // Dessiner les particules
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
glLineWidth(1.0); // Largeur des lignes pour les chemins
glBegin(GL_LINES);
for (const auto& particle : particles) {
    if (!particle.previousPositions.empty()) {
        // Utiliser le premier et le dernier point pour dessiner la ligne
        Vec3 start = particle.previousPositions.front();
        Vec3 end = particle.previousPositions.back();

        glColor4f(particle.color.x, particle.color.y, particle.color.z, 0.4); // Couleur des lignes
        glVertex3f(start.x, start.y, start.z);
        glVertex3f(end.x, end.y, end.z);
    }
}
glEnd();
glLineWidth(1.0); // Restaurer la largeur de ligne par défaut
glDisable(GL_BLEND);
}

Vec3 RotateVector(const Vec3& v, const Vec3& axis, float angleDeg) {
    float angleRad = angleDeg * M_PI / 180.0f ; // Convertir l'angle en radians
    float cosAngle = cos(angleRad);
    float sinAngle = sin(angleRad);

    Vec3 vecteurFinal;
    vecteurFinal.x = v.x * cosAngle + axis.cross(v).x * sinAngle + axis.x * axis.dot(v) * (1 - cosAngle);
    vecteurFinal.y = v.y * cosAngle + axis.cross(v).y * sinAngle + axis.y * axis.dot(v) * (1 - cosAngle);
    vecteurFinal.z = v.z * cosAngle + axis.cross(v).z * sinAngle + axis.z * axis.dot(v) * (1 - cosAngle);
    // Formule de Rodrigues pour la rotation de vecteur
    return vecteurFinal;
}

#include <cmath>

Vec3 ApproachVectorTowardsNormal(const Vec3& v, const Vec3& normal, float angleDeg) {
    // Convertir l'angle en radians
    float angleRad = angleDeg * M_PI / 180.0f;

    // Calculer l'axe de rotation (perpendiculaire à la fois au vecteur initial et à la normale)
    Vec3 rotationAxis = v.cross(normal).normalize();
    if (rotationAxis.norm() == 0) {
        // Si le vecteur et la normale sont déjà alignés, aucune rotation n'est nécessaire
        return v;
    }

    // Calculer cos et sin de l'angle
    float cosAngle = cos(angleRad);
    float sinAngle = sin(angleRad);

    // Appliquer la formule de Rodrigues pour la rotation de vecteur
    Vec3 rotatedVector;
    rotatedVector.x = v.x * cosAngle + rotationAxis.cross(v).x * sinAngle + rotationAxis.x * rotationAxis.dot(v) * (1 - cosAngle);
    rotatedVector.y = v.y * cosAngle + rotationAxis.cross(v).y * sinAngle + rotationAxis.y * rotationAxis.dot(v) * (1 - cosAngle);
    rotatedVector.z = v.z * cosAngle + rotationAxis.cross(v).z * sinAngle + rotationAxis.z * rotationAxis.dot(v) * (1 - cosAngle);

    return rotatedVector;
}



void CollisionTool::GenerateParticle() {
    particleSpeedFactor = 0.50f;
    float particulesLoop = 10;

    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_real_distribution<> disAngle(-180.0, 180.0); // Pour l'angle de dérive
    std::uniform_real_distribution<> disSpeed(1.0, 3.0); // Pour la vitesse
    float angle1 = -60 * facteur1;
    float angle2 = 60 * facteur2;
    std::uniform_real_distribution<> disDeriveAngle(angle1, angle2); 

    Vec3 arbitraryVec = Vec3(1.0, 0.0, 0.0);
    if (abs(normal.dot(arbitraryVec)) > 0.9) {
        arbitraryVec = Vec3(0.0, 1.0, 0.0);
    }
    Vec3 baseVec1 = normal.cross(arbitraryVec).normalize();
    Vec3 baseVec2 = normal.cross(baseVec1).normalize();


    for (int i = 0; i < particulesLoop; ++i) {
        ParticleCollision particle;
        particle.position = midpoint;
        Vec3 selectedColor = explosionColors[rand() % explosionColors.size()];
        particle.color = selectedColor;

        Vec3 randomVec = (baseVec1 * cos(2 * M_PI) + baseVec2 * sin(2 * M_PI)).normalize();

        // Appliquer une rotation basée sur la normale et l'angle aléatoire
        float angle = disAngle(gen);
        Vec3 rotatedVec = RotateVector(randomVec, normal, angle).normalize();

        float deriveAngle = disDeriveAngle(gen);
        Vec3 finalVec = ApproachVectorTowardsNormal(rotatedVec, normal, deriveAngle).normalize();

        particle.velocity = finalVec * disSpeed(gen) * particleSpeedFactor;
        float smalestRadius = obj1->getRayon() < obj2->getRayon() ? obj1->getRayon() : obj2->getRayon();
        particle.distanceDestruction = (obj1->getRayon()+obj2->getRayon()) * disSpeed(gen);

        particles.push_back(particle);
    }
}



void CollisionTool::updateParticles(){
    // Use std::chrono::steady_clock to get the current time
    static auto lastUpdateTime = std::chrono::steady_clock::now();
    auto currentTime = std::chrono::steady_clock::now();

    // Calculate deltaTime in seconds
    float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - lastUpdateTime).count();
    lastUpdateTime = currentTime;

    if (*(m_renderContext->isPaused)){return;}
    for (size_t i = 0; i < particles.size(); ) {
        // Mise à jour de la position
        //Ajouter une particule 
        particles[i].previousPositions.push_back(particles[i].position);
        if (particles[i].previousPositions.size() > 10) {
            particles[i].previousPositions.erase(particles[i].previousPositions.begin());
        }
        particles[i].position.x += particles[i].velocity.x * deltaTime;
        particles[i].position.y += particles[i].velocity.y * deltaTime;
        particles[i].position.z += particles[i].velocity.z * deltaTime;

        // Vérifier la distance par rapport au point médian
        if ((particles[i].position - midpoint).norm() > particles[i].distanceDestruction) {
            particles.erase(particles.begin() + i);
        } else {
            ++i;
        }
    }
}

void CollisionTool::reset(){
    particles.clear();
    obj1 = nullptr;
    obj2 = nullptr;
}

void CollisionTool::computeFactor(){
    float fraction = obj1->getRayon() / obj2->getRayon();
    float angle1,angle2;
    float minFacteur = 0.3f;
    float maxFacteur = 1.0f;
    if (fraction > 1.2f) {
    // obj1 est plus grand que obj2
    facteur1 = minFacteur;
    facteur2 = maxFacteur * fraction; // Diminue lorsque la taille de obj1 augmente relativement à obj2
    } else if (fraction < 1.2f) {
        // obj2 est plus grand que obj1
        facteur1 = maxFacteur / fraction; // Diminue lorsque la taille de obj2 augmente relativement à obj1
        facteur2 = minFacteur;
    } else{
        // objets de taille égale
        facteur1 = facteur2 = maxFacteur;
    }
}

void CollisionTool::drawCircleIntersection(){
    int numSlices = 40; // Plus de tranches pour une sphère plus lisse
    int numSegments = 40; // Plus de segments pour une sphère plus lisse
    int numberOrCircle = 20;
    float rayonMax = obj1->getRayon() > obj2->getRayon() ? obj1->getRayon() : obj2->getRayon();
    CelestialObject* ObjectrayonMin = obj1->getRayon() < obj2->getRayon() ? obj1:obj2;
    float distance = (obj1->getPositionSimulation() - obj2->getPositionSimulation()).norm();
    float radius;
    Vec3 newMid = midpoint;
    float subRad = pow(obj1->getRayon(),2)-pow(distanceMiddle,2);
    radius = pow(subRad,0.5)*1.01; // Rayon du cercle, à ajuster selon l'échelle de votre scène

    
    Vec3 axisZ(0, 0, 1); // Axe Z comme référence
    Vec3 rotationAxis = axisZ.cross(normal); // Produit vectoriel pour l'axe de rotation
    float angle = acos(axisZ.dot(normal)) / (axisZ.norm() * normal.norm()); // Angle en radians
    angle = angle * 180.0 / M_PI; // Convertir en degrés

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // Translation pour positionner la sphère au point de collision
    glTranslatef(newMid.x, newMid.y, newMid.z);
    glRotatef(angle, rotationAxis.x, rotationAxis.y, rotationAxis.z); // Rotation pour aligner avec la normale
    // Configuration de la sphère (similaire à drawTexturedSphere)
    const float PI = 3.14159265358979323846f;
    const float PI_2 = PI * 2.0f;

    for (int circle = 0; circle < numberOrCircle; ++circle) {
        // Ajuster le rayon et l'alpha pour chaque cercle
        float collisionRadius = radius * (1.0f + 0.01f * circle);
        float tubeRadius = 0.1f * collisionRadius*(0.2*circle); 
        float alpha = 0.4f - 0.025f * circle; // Plus transparent pour les plus grands cercles

    for (int i = 0; i < numSlices; ++i) {
        float phi1 = static_cast<float>(i) / static_cast<float>(numSlices) * PI_2;
        float phi2 = static_cast<float>(i + 1) / static_cast<float>(numSlices) * PI_2;

        glBegin(GL_QUAD_STRIP);
        if(obj1->getTypeName() == "BlackHole" || obj2->getTypeName() == "BlackHole"){
            glColor4f(0.0f, 0.0f, 0.0f, alpha); // Noir pour les cercles
        }else{
        glColor4f(1.0f, 0.537f, 0.2f, alpha);} // Rouge pour les cercles
        for (int j = 0; j <= numSegments; ++j) {
            float theta = static_cast<float>(j) / static_cast<float>(numSegments) * PI_2;

            float x1 = (radius + tubeRadius * cosf(phi1)) * cosf(theta);
            float y1 = (radius + tubeRadius * cosf(phi1)) * sinf(theta);
            float z1 = tubeRadius * sinf(phi1);

            float x2 = (radius + tubeRadius * cosf(phi2)) * cosf(theta);
            float y2 = (radius + tubeRadius * cosf(phi2)) * sinf(theta);
            float z2 = tubeRadius * sinf(phi2);

            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y2, z2);
        }
        glEnd();
    }
    }
    glDisable(GL_BLEND);
    glPopMatrix();
}