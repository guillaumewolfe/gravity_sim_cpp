#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/CreationTools/TextureCreator.h"
#include "engine/CreationTools/CreatorManager.h"


TextureCreator::TextureCreator(RenderContext* renderContext, CreatorManager* manager) : StateCreator(renderContext, manager) {
    generate_buttons();
    generate_labels();
    objects = initSystem();
    calculateObjectPositions();
    generatePlanetLabels();
}

void TextureCreator::Enter(){
    m_manager->isCreated=false;
    reset();
}

void TextureCreator::DrawOpenGL(){
    drawCelestialObjects();
    drawSelectionSphere();

}

void TextureCreator::Exit(){}

std::vector<CelestialObject*> TextureCreator::initSystem(){
    std::vector<CelestialObject*> celestialObjects;

    Sun* sun = new Sun();
    Mercure* mercure = new Mercure();
    Venus* venus = new Venus();
    Earth* earth = new Earth();
    Moon* moon = new Moon();
    Mars* mars = new Mars();
    Jupiter* jupiter = new Jupiter();
    Saturn* saturn = new Saturn();
    Uranus* uranus = new Uranus();
    Neptune* neptune = new Neptune();
    BlackHole*  blackHole = new BlackHole();
    Rock*  rock = new Rock();
    Volcanic*  volcanic= new Volcanic();
    Ice*  ice = new Ice();
    
    celestialObjects.push_back(sun);
    celestialObjects.push_back(blackHole);
    celestialObjects.push_back(rock);
    celestialObjects.push_back(volcanic);
    celestialObjects.push_back(ice);
    celestialObjects.push_back(mercure);
    celestialObjects.push_back(venus);
    celestialObjects.push_back(earth);
    celestialObjects.push_back(moon);
    celestialObjects.push_back(mars);
    celestialObjects.push_back(jupiter);
    celestialObjects.push_back(saturn);
    celestialObjects.push_back(uranus);
    celestialObjects.push_back(neptune);
    return celestialObjects;
}

void TextureCreator::Draw() {
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.3f, 0.4f, 0.3f)); // Exemple de couleur (bleu foncé)

    ImGui::Begin("MessageBox", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    if (ImGui::IsKeyReleased(ImGuiKey_Escape)) {CloseButtonPressed();}
    drawBackground();
    draw_buttons();
    draw_labels();
    updateHoverStatus();
    updateClickStatus();


    ImGui::End(); 
    ImGui::PopStyleColor();
}

void TextureCreator::drawBackground(){

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 10.0f;



    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(0, 0, 0, 255), // Couleur
                            cornerRadius);

    float cornerRadiusAdjustment = 10.0f;
    drawList->AddRect(topLeft,
                        ImVec2(topLeft.x + longueur, topLeft.y + hauteur),
                        IM_COL32(100, 100, 100, 150), // Couleur de remplissage
                        cornerRadius,0,3.0f); // Ajustez le rayon ici

}


void TextureCreator::generate_labels(){
    Labbel *MessageLabel = new Labbel(0.5f,0.20f,ImVec4(255,255,255,255),
                            "Choose type of object",30.0f,0.7f);
    labbels.push_back(MessageLabel);
}
void TextureCreator::updateClickStatus() {
    float detectionWidth = radius*2.3; // Largeur de la zone de détection en pixels
    float detectionHeight = radius*2.3; // Hauteur de la zone de détection en pixels
    for (size_t i = 0; i < objectPositions.size(); ++i) {
        auto& info = objectPositions[i];
        float posY = winHeight-info.posY;
        
        ImVec2 rectMin(info.posX - detectionWidth / 2.0f, posY - detectionHeight / 2.0f);
        ImVec2 rectMax(info.posX + detectionWidth / 2.0f, posY+ detectionHeight / 2.0f);

        // Utiliser ImGui::IsMouseHoveringRect pour vérifier si la souris survole la zone
        if (ImGui::IsMouseHoveringRect(rectMin, rectMax) && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            buttons[1]->hidden=false;
            buttons[0]->UpdatePosition(0.45f, 0.8225);
            selectedIndex = i;
            drawSphereClick= true;
            selectionSphereClick.posX = info.posX;
            selectionSphereClick.posY = info.posY;
            selectionSphereClick.radius = radius * 1.25f;
            selectionSphereClick.color = ImVec4(200,255,225,85);
            resetLabelColor();
            planeteNames[i]->UpdateColor(ImVec4(100,255,150,240)); 
        }
    }
}
void TextureCreator::resetLabelColor(){
    for (size_t i = 0; i < objectPositions.size(); ++i){
        planeteNames[i]->UpdateColor(ImVec4(255, 255, 255, 255));
    }
}
void TextureCreator::drawSelectionSphere(){
    draw2DSphereClicked();
    draw2DSphereHover();

}

void TextureCreator::draw2DSphereClicked() {
    if(!drawSphereClick){return;}
    float posX = selectionSphereClick.posX;
    float posY = selectionSphereClick.posY;
    float initialRadius = selectionSphereClick.radius;
    ImVec4 color = selectionSphereClick.color;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    int winWidth, winHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    glOrtho(0, winWidth, 0, winHeight, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const int numSegments = 30; // Ajustez le nombre de segments pour obtenir la qualité souhaitée
    const int numSpheres = 25; // Nombre de sphères pour l'effet d'aura
    float radiusIncrement = initialRadius * 0.1/numSpheres; // Incrément de rayon pour chaque sphère
    float alphaDecrement = color.w / numSpheres; // Diminution de l'alpha pour chaque sphère

    for (int j = 0; j < numSpheres; ++j) {
        float radius = initialRadius + radiusIncrement * j;
        float alpha = color.w - alphaDecrement * j;
        // Utilisez la couleur spécifiée pour le dessin
        glColor4f(color.x / 255, color.y / 255, color.z / 255, alpha / 255);

        // Dessinez un cercle en utilisant une approche basée sur des triangles
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(posX, posY); // Centre du cercle
        for (int i = 0; i <= numSegments; ++i) {
            float angle = 2.0f * 3.14159265358979323846f * i / numSegments;
            float x = radius * cosf(angle);
            float y = radius * sinf(angle);
            glVertex2f(posX + x, posY + y);
        }
        glEnd();
    }

    glDisable(GL_BLEND);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}
void TextureCreator::updateHoverStatus() {
    float detectionWidth = radius*2.3; // Largeur de la zone de détection en pixels
    float detectionHeight = radius*2.3; // Hauteur de la zone de détection en pixels
    drawSphereHover = false;
    for (size_t i = 0; i < objectPositions.size(); ++i) {
        auto& info = objectPositions[i];
        float posY = winHeight-info.posY;
        
        ImVec2 rectMin(info.posX - detectionWidth / 2.0f, posY - detectionHeight / 2.0f);
        ImVec2 rectMax(info.posX + detectionWidth / 2.0f, posY+ detectionHeight / 2.0f);

        // Utiliser ImGui::IsMouseHoveringRect pour vérifier si la souris survole la zone
        if (ImGui::IsMouseHoveringRect(rectMin, rectMax)) {
            drawSphereHover = true;
            selectionSphereHover.posX = info.posX;
            selectionSphereHover.posY = info.posY;
            selectionSphereHover.radius = radius * 1.25f;
            selectionSphereHover.color = ImVec4(100,255,150,50);
        } else {
        }
    }
}

void TextureCreator::draw2DSphereHover() {
    if(!drawSphereHover){return;}
    float posX = selectionSphereHover.posX;
    float posY = selectionSphereHover.posY;
    float initialRadius = selectionSphereHover.radius;
    ImVec4 color = selectionSphereHover.color;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    int winWidth, winHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    glOrtho(0, winWidth, 0, winHeight, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const int numSegments = 30; // Ajustez le nombre de segments pour obtenir la qualité souhaitée
    const int numSpheres = 25; // Nombre de sphères pour l'effet d'aura
    float radiusIncrement = initialRadius * 0.1/numSpheres; // Incrément de rayon pour chaque sphère
    float alphaDecrement = color.w / numSpheres; // Diminution de l'alpha pour chaque sphère

    for (int j = 0; j < numSpheres; ++j) {
        float radius = initialRadius + radiusIncrement * j;
        float alpha = color.w - alphaDecrement * j;
        // Utilisez la couleur spécifiée pour le dessin
        glColor4f(color.x / 255, color.y / 255, color.z / 255, alpha / 255);

        // Dessinez un cercle en utilisant une approche basée sur des triangles
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(posX, posY); // Centre du cercle
        for (int i = 0; i <= numSegments; ++i) {
            float angle = 2.0f * 3.14159265358979323846f * i / numSegments;
            float x = radius * cosf(angle);
            float y = radius * sinf(angle);
            glVertex2f(posX + x, posY + y);
        }
        glEnd();
    }

    glDisable(GL_BLEND);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void TextureCreator::draw_labels(){
        for (Labbel* label : labbels) {
        label->Draw();
    }
        for (Labbel* label : planeteNames) {
        label->Draw();
    }
}

void TextureCreator::generate_buttons(){
   Button *ReturnButton = new Button(0.5f, 0.8225, ImVec2(0.05, 0.045),
                                ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                                ImVec4(0.0f, 0.0f, 0.0f, 0.0f),
                               
                               "Close", 0.00f,22.0f,
                               std::bind(&TextureCreator::CloseButtonPressed, this));  

   Button *NextButton = new Button(0.55f, 0.8225, ImVec2(0.05, 0.04),
                                ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                                ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                               "Select", 0.0f,20.0f,
                               std::bind(&TextureCreator::NextButtonPressed, this));  
    buttons.push_back(ReturnButton);
    buttons.push_back(NextButton);
    buttons[1]->hidden=true;
    buttons[1]->UpdateLabelColor(100,255,150,200);
    buttons[0]->UpdateLabelColor(255,125,100,200);
}
void TextureCreator::draw_buttons(){
        for (Button* btn : buttons) {
        btn->Draw();
    }
}

void TextureCreator::CloseButtonPressed(){
    m_manager->Exit();
}

void TextureCreator::NextButtonPressed(){
    CelestialObject* clickedObject = objects[selectedIndex];
    CelestialObject* clickedObjectCopy = new CelestialObject(*clickedObject); // Copie de l'objet
    m_manager->newCreatedObject = clickedObjectCopy;
    m_manager->ChangeState("PositionCreator");
}



void TextureCreator::generatePlanetLabels() {
    planeteNames.clear(); // Vider la liste existante

    float labelOffsetY = -(winHeight * 0.045f); // Décalage vertical pour les étiquettes
    float labelScale = 0.7f; // Échelle pour les étiquettes

    for (const auto& info : objectPositions) {
        // Calculer la position de l'étiquette
        float labelPosX = info.posX / winWidth; // Position X normalisée
        float labelPosY = 1-((info.posY + labelOffsetY) / winHeight); // Position Y normalisée avec décalage

        // Créer une nouvelle étiquette pour l'objet
        Labbel* planetLabel = new Labbel(labelPosX, labelPosY, ImVec4(255, 255, 255, 255),
                                         info.name.c_str(), 18.0f, labelScale);

        // Ajouter l'étiquette au vecteur
        planeteNames.push_back(planetLabel);
    }
}




void TextureCreator::drawCelestialObjects() {
    radius = winHeight * 0.025;

    for (const auto& info : objectPositions) {
        drawTexturedSphere(radius, 30, 30, info.posX, info.posY, info.texture);
    }
}


void TextureCreator::calculateObjectPositions() {
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    longueur = winWidth* 0.30; // Exemple de taille
    hauteur = winHeight * 0.75; // Exemple de taille

    ImVec2 centerPos = ImVec2(winWidth * 0.5f, winHeight * 0.5f);
    topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);
    objectPositions.clear(); // Assurez-vous de vider la liste avant de la remplir

    const int maxObjectsPerRow = 4;
    float bufferTop = winHeight * 0.10;
    float bufferBottom = winHeight * 0.15;
    float usableHeight = hauteur - (bufferTop + bufferBottom);
    float spacingX = longueur / maxObjectsPerRow;
    int numRows = (objects.size() + maxObjectsPerRow - 1) / maxObjectsPerRow;
    float spacingY = usableHeight / numRows;
    float radius = winHeight * 0.025; // Le rayon de la sphère
    float hoverRadius = radius * 1.2f; // Le hoverRadius, légèrement plus grand que le rayon de la sphère

    for (size_t i = 0; i < objects.size(); ++i) {
        CelestialObjectInfo info;
        info.texture = objects[i]->getTexture();
        info.name = objects[i]->getName(); // Ou tout autre attribut nécessaire

        int row = i / maxObjectsPerRow;
        int col = i % maxObjectsPerRow;
        info.posX = topLeft.x + spacingX * (col + 0.5f);
        info.posY = topLeft.y + bufferTop + usableHeight - spacingY * (row + 0.5f);
        objectPositions.push_back(info);
    }
}
void TextureCreator::drawTexturedSphere(float radius, int numSegments, int numSlices, float offsetX, float offsetY,GLuint texture) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    int winWidth, winHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    glOrtho(0, winWidth, 0, winHeight, -radius, radius);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();


    // Positionnement de la sphère en fonction de la distance calculée
    glTranslatef(offsetX,offsetY,0);
    glRotatef(90,1,0,0);
    glRotatef(Rotation,0,0,-1);
    Rotation+=0.02;
    if(Rotation==360){Rotation-=360;}

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glEnable(GL_TEXTURE_2D);


    const float PI = 3.14159265358979323846;
    const float PI_2 = PI * 2.0f;

    for (int i = 0; i < numSlices; ++i) {
        float phi1 = static_cast<float>(i) / static_cast<float>(numSlices) * PI;
        float phi2 = static_cast<float>(i + 1) / static_cast<float>(numSlices) * PI;

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= numSegments; ++j) {
            float theta = static_cast<float>(j) / static_cast<float>(numSegments) * PI_2;

            float x1 = radius * sinf(phi1) * cosf(theta);
            float y1 = radius * sinf(phi1) * sinf(theta);
            float z1 = radius * cosf(phi1);

            float x2 = radius * sinf(phi2) * cosf(theta);
            float y2 = radius * sinf(phi2) * sinf(theta);
            float z2 = radius * cosf(phi2);

            // Calcul des coordonnées de texture en fonction de l'angle theta et phi
            float u1 = static_cast<float>(j) / static_cast<float>(numSegments);
            float v1 = static_cast<float>(i) / static_cast<float>(numSlices);

            float u2 = static_cast<float>(j) / static_cast<float>(numSegments);
            float v2 = static_cast<float>(i + 1) / static_cast<float>(numSlices);

            glTexCoord2f(u1, v1);
            glVertex3f(x1, y1, z1);

            glTexCoord2f(u2, v2);
            glVertex3f(x2, y2, z2);
        }
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void TextureCreator::reset(){
    drawSphereHover = false;
    drawSphereClick = false;
    Rotation=0;
    buttons[1]->hidden=true;
    buttons[0]->UpdatePosition(0.5f, 0.8225);
    resetLabelColor();

}
