#include "states/LoadingState.h"
#include "game/game.h"
#include <random>

LoadingState::LoadingState(Game* gameObj) : BaseState(gameObj) {
    Enter();
    generate_colors();
    initializePlanetAngles();
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    generateRandomStars();
}

void LoadingState::Enter() {
    // Créez un contexte ImGui distinct pour la fenêtre secondair
}

void LoadingState::Update() {
}

void LoadingState::UpdatePhysics(double dt){};

void LoadingState::Draw() {
    // Get screen width and height
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);

    // Set up 2D orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, winWidth, 0, winHeight, -1, 1); // Left, Right, Bottom, Top, Near, Far
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Switch back to modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Clear the buffers
    glClearColor(0,0,0,0.5f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the sphere in 2D
    //drawFixedStars();

    // Draw the solar system
    drawSolarSystem();
    glDisable(GL_BLEND);
}

void LoadingState::drawSolarSystem(){
    float pourcentageX = 0.3f;
    float pourcentageY = 0.3f;
    float pourcentageSize = 0.45f;
    ImVec2 positionCentre = ImVec2(winWidth * 0.5f, winHeight * 0.5f);
    float maxSize = winHeight * pourcentageSize;
    float normalRadius = maxSize/(9*4);
    float spacing = maxSize / 9;
    static float globalAngle = 0.0f;

    // Calculez le delta de temps depuis la dernière mise à jour
    auto now = std::chrono::steady_clock::now();
    float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(now - lastUpdateTime).count();
    lastUpdateTime = now;

    // Mettez à jour l'angle global basé sur le temps écoulé
    globalAngle += deltaTime * 0.2f;

    float indice = 0;


    for(std::string planet : systemSolaire){
        ImVec4 color = typeDictColor[planet];
        float distance = indice * spacing;// Assurez-vous que les planètes ne sont pas sur le Soleil

        // La vitesse orbitale diffère pour chaque planète
        float orbitSpeed = 0.2f + (10-indice) * 0.75f;

        float angle = -(globalAngle * orbitSpeed + initialAngles[indice]); // Utilisez l'angle initial aléatoire stocké

        // Calculez la position de la planète sur son orbite
        ImVec2 position = ImVec2(positionCentre.x + cos(angle) * distance, positionCentre.y + sin(angle) * distance);

        if(planet == "Sun"){
            drawFullCircle(position.x, position.y, normalRadius*1.2, color.x, color.y, color.z, 0.8f);
            drawSunEffect(position, normalRadius);
        }else{
        drawFullCircle(position.x, position.y, normalRadius, color.x, color.y, color.z, 0.8f);
        drawPlanetLightNOSHADOW(position, normalRadius, color);}

        //drawCircle(positionCentre.x,positionCentre.y, distance,1.0,1.0,1.0,1.0,0.2); // Dessinez l'orbite
        indice++;
    }

}


void LoadingState::initializePlanetAngles() {
    std::mt19937 gen(std::random_device{}()); // Générateur de nombres aléatoires
    std::uniform_real_distribution<float> dist(0.0f, 2.0f * M_PI); // Distribution de 0 à 2π

    initialAngles.clear(); // Assurez-vous qu'il est vide avant de commencer
    for(size_t i = 0; i < systemSolaire.size(); ++i) {
        initialAngles.push_back(dist(gen)); // Générer et stocker un angle aléatoire pour chaque planète
    }
}

void LoadingState::drawFullCircle(float screenPosX, float screenPosY, float radius, float red, float green, float blue, float alpha) {
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);

    // Convert screen percentage positions to actual coordinates
    float posX = screenPosX;
    float posY = screenPosY ;

    int numSegments = 100; // Increase for a smoother circle

    // Set color and alpha
    glColor4f(red, green, blue, alpha);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(posX, posY); // Center of circle
    for (int i = 0; i <= numSegments; i++) {
        float theta = 2.0f * M_PI * (float)i / (float)numSegments;
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        glVertex2f(posX + x, posY + y);
    }
    glEnd();

    // Reset color
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void LoadingState::drawCircle(float screenPosX, float screenPosY, float radius, float lineWidth, float red, float green, float blue, float alpha) {
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);

    // Convert screen percentage positions to actual coordinates
    float posX = screenPosX;
    float posY = screenPosY;

    int numSegments = 100; // Increase for a smoother circle

    // Set color and alpha
    glColor4f(red, green, blue, alpha);

    // Set line width
    glLineWidth(lineWidth);

    glBegin(GL_LINES);
    for (int i = 0; i < numSegments; i++) {
        float theta1 = 2.0f * M_PI * (float)i / (float)numSegments;
        float theta2 = 2.0f * M_PI * (float)(i + 1) / (float)numSegments;
        float x1 = radius * cos(theta1);
        float y1 = radius * sin(theta1);
        float x2 = radius * cos(theta2);
        float y2 = radius * sin(theta2);
        glVertex2f(posX + x1, posY + y1);
        glVertex2f(posX + x2, posY + y2);
    }
    glEnd();

    // Reset line width
    glLineWidth(1.0f);

    // Reset color
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}




void LoadingState::drawPlanetLightNOSHADOW(ImVec2 planetPos, float radius, ImVec4 colorCenterDot){
    float numBlurCircles = 20;
    float blurIncrease = radius*0.04; // How much larger each successive blur circle is
    float initialAlpha = 40; // Starting alpha value for the outermost blur circle
    float alphaDecrease = initialAlpha / numBlurCircles; // How much alpha decreases per circle
    for (int i = 0; i < numBlurCircles; ++i) {
        float blurRadius = radius + blurIncrease * (i + 1);
        float alpha = initialAlpha - alphaDecrease * i;
        drawFullCircle(planetPos.x,planetPos.y, blurRadius, colorCenterDot.x,colorCenterDot.y,colorCenterDot.z, alpha/255.0f);}
}

void LoadingState::drawSunEffect(ImVec2 planetPos, float radius){
    ImVec4 colorCenterDot = ImVec4(255.0f/255.0f,215.0f/255.0f,80.0f/255.0f,255.0f/255.0f);
    float numBlurCircles = 40;
    float blurIncrease = radius*0.05; // How much larger each successive blur circle is
    float initialAlpha = 30; // Starting alpha value for the outermost blur circle
    float alphaDecrease = initialAlpha / numBlurCircles; // How much alpha decreases per circle
    for (int i = 0; i < numBlurCircles; ++i) {
        float blurRadius = radius + blurIncrease * (i + 1);
        float alpha = initialAlpha - alphaDecrease * i;
        drawFullCircle(planetPos.x,planetPos.y, blurRadius, colorCenterDot.x,colorCenterDot.y,colorCenterDot.z, alpha/255.0f);}
}



void LoadingState::Exit() {
    std::cout << "Exiting Loading State" << std::endl;
}

std::string LoadingState::getDescription() {
    return "Loading State";
}
void LoadingState::RestartState(){}


void LoadingState::setWindow(GLFWwindow* newWindow){
    window = newWindow;
}











void LoadingState::generate_colors() {
    // Modifier le dictionnaire pour utiliser des chaînes de caractères comme clés
    typeDictColor["Black Hole"] = ImVec4(0, 0, 0, 1);
    typeDictColor["Sun"] = ImVec4(250.0f/255.0f, 250.0f/255.0f, 120.0f/255.0f, 1); // Jaune pour une étoile par exemple
    typeDictColor["Mercury"] = ImVec4(100.0f/255.0f, 100.0f/255.0f, 100.0f/255.0f, 1);
    typeDictColor["Venus"] = ImVec4(238.0f/255.0f, 147.0f/255.0f, 17.0f/255.0f, 1); // Orange pour Venus
    typeDictColor["Earth"] = ImVec4(50.0f/255.0f, 100.0f/255.0f, 255.0f/255.0f, 1); // Bleu pour la Terre
    typeDictColor["Mars"] = ImVec4(197.0f/255.0f, 84.0f/255.0f, 62.0f/255.0f, 1); // Rougeâtre pour Mars
    typeDictColor["Jupiter"] = ImVec4(234.0f/255.0f, 206.0f/255.0f, 180.0f/255.0f, 1);
    typeDictColor["Saturn"] = ImVec4(217.0f/255.0f, 186.0f/255.0f, 140.0f/255.0f, 1); // Jaune pour Saturne
    typeDictColor["Uranus"] = ImVec4(155.0f/255.0f, 221.0f/255.0f, 227.0f/255.0f, 1);
    typeDictColor["Neptune"] = ImVec4(39.0f/255.0f, 114.0f/255.0f, 210.0f/255.0f, 1); // Bleu pour Neptune

}

void LoadingState::drawFixedStars() {

    for (const auto& star : stars) {
        drawPlanetLightNOSHADOW(ImVec2(star.first, star.second), 0.5f, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    }
}

void LoadingState::generateRandomStars() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> disX(0.0f, static_cast<float>(winWidth));
    std::uniform_real_distribution<float> disY(0.0f, static_cast<float>(winHeight));

    for (int i = 0; i < numStars; ++i) {
        float x = disX(gen);
        float y = disY(gen);
        stars.push_back(std::make_pair(x, y));
    }
}