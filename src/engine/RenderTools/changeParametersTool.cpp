#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/RenderTools/changeParametersTool.h"
#include "engine/RenderTools/soundTool.h"
#include "path_util.h"

/*
Mode 0 = inactive
Mode 1 = change weight
Mode 2 = change radius
Mode 3 = change position
Mode 4 = change name
Mode 5 = change speed
*/

ChangeParametersTool::ChangeParametersTool(RenderContext* renderContext) : RenderComponent(renderContext){
initMode1();
initMode2();
initMode3();
initMode4();
initMode5();
initGeneralUI();
generate_colors();
mode = 0;
font = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/RobotoB.ttf").c_str(), 20);
planeteNameFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/RobotoB.ttf").c_str(), 12);
planateSelectedFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("../assets/fonts/RobotoB.ttf").c_str(), 16);
}

// Desctructeur
ChangeParametersTool::~ChangeParametersTool() {
    /*remove all buttons and object in :
        std::vector<Labbel*> labbels,labbelsMode1,labbelsMode2,labbelsMode3,labbelsMode4,labbelsMode5;
    std::vector<Icon*> icons,iconsMode1,iconsMode2,iconsMode3,iconsMode4,iconsMode5;    
    std::vector<ImageButton*> imageButtons,imageButtonsMode1,imageButtonsMode2,imageButtonsMode3,imageButtonsMode4,imageButtonsMode5;
    std::vector<Button*> buttons,buttonsMode1,buttonsMode2,buttonsMode3,buttonsMode4,buttonsMode5;

    */
    for (auto& label : labbels) {
        delete label;
    }
    for (auto& label : labbelsMode1) {
        delete label;
    }
    for (auto& label : labbelsMode2) {
        delete label;
    }
    for (auto& label : labbelsMode3) {
        delete label;
    }
    for (auto& label : labbelsMode4) {
        delete label;
    }
    for (auto& label : labbelsMode5) {
        delete label;
    }
    for (auto& icon : icons) {
        delete icon;
    }
    for (auto& icon : iconsMode1) {
        delete icon;
    }
    for (auto& icon : iconsMode2) {
        delete icon;
    }
    for (auto& icon : iconsMode3) {
        delete icon;
    }
    for (auto& icon : iconsMode4) {
        delete icon;
    }
    for (auto& icon : iconsMode5) {
        delete icon;
    }
    for (auto& button : imageButtons) {
        delete button;
    }
    for (auto& button : imageButtonsMode1) {
        delete button;
    }
    for (auto& button : imageButtonsMode2) {
        delete button;
    }
    for (auto& button : imageButtonsMode3) {
        delete button;
    }
    for (auto& button : imageButtonsMode4) {
        delete button;
    }
    for (auto& button : imageButtonsMode5) {
        delete button;
    }
    for (auto& button : buttons) {
        delete button;
    }
    for (auto& button : buttonsMode1) {
        delete button;
    }
    for (auto& button : buttonsMode2) {
        delete button;
    }
    for (auto& button : buttonsMode3) {
        delete button;
    }
    for (auto& button : buttonsMode4) {
        delete button;
    }
    for (auto& button : buttonsMode5) {
        delete button;
    }
}

void ChangeParametersTool::initGeneralUI(){
           std::function<void(std::string)> playSoundFunc = 
    std::bind(&SoundTool::playSound, m_renderContext->soundTool, std::placeholders::_1);

    ImVec4 button_color = ImVec4(0.17f, 0.27f, 0.17f, 1.0f);
    float taille_x = 0.035f*0.8;
    float taille_y = 0.06f*0.8;
    ImageButton *closeButton = new ImageButton(playSoundFunc,0.5+(0.35/2)*0.925, 0.5-(0.35/2)*0.9, ImVec2(taille_x*0.5, taille_y*0.5),0.60,
                        button_color,button_color,
                        "../assets/button/close.png", 0,
                            std::bind(&ChangeParametersTool::closeButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    imageButtons.push_back(closeButton);

   Button *SaveButton = new Button(playSoundFunc,0.53, 0.5+(0.35/2)*0.85, ImVec2(taille_x*1.6, taille_y*0.65),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               "Save", 0.2f,20.0f,
                            std::bind(&ChangeParametersTool::Save, this),5);   
    Button *Cancel = new Button(playSoundFunc,0.47, 0.5+(0.35/2)*0.85, ImVec2(taille_x*1.6, taille_y*0.65),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                            "Cancel", 0.05f,19.0f,
                        std::bind(&ChangeParametersTool::closeButton, this),5); 
    buttons.push_back(SaveButton);
    buttons.push_back(Cancel);
}

void ChangeParametersTool::Update(CelestialObject* otherObj){
    if(otherObj != m_object){
        mode=0;
        return;
    }
}

void ChangeParametersTool::Draw() {
    if(mode == 0){
        return;
    }
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    drawMode();

    if (ImGui::IsKeyReleased(ImGuiKey_Escape)) {
    Close();}
}

void ChangeParametersTool::drawUI(){
    float longueur = winWidth* 0.375; // Exemple de taille
    float hauteur = winHeight * 0.375; // Exemple de taille

    ImVec2 centerPos = ImVec2(winWidth * 0.5f, winHeight * 0.5f);
    ImVec2 topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = winWidth*0.02;

    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(0,0,0, 255), // Couleur
                            cornerRadius);
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(20, 25, 30, 200), // Couleur
                            cornerRadius);


    for (auto& button : imageButtons) {
        button->Draw();
    }
    for (auto& label : labbels) {
        label->Draw();
    }
    for (auto& button : buttons) {
        button->Draw();
    }
}


void ChangeParametersTool::setMode(int mode, CelestialObject* object){
        miniMapWasShown = false;
        changeConfirmed = false;
        showConfirmChangeBox = false;
        this->mode = mode;
        m_object = object;
        changeMode();
        if(m_renderContext->showMinimap && mode != 0){
            closeMinimapFunction();
            miniMapWasShown = true;
        }
}
int ChangeParametersTool::getMode(){
    return mode;
}


void ChangeParametersTool::drawMode(){
    switch (mode) {
        case 0:
            return;
        case 1:
            drawUI();
            drawMode1();
            break;
        case 2:
            drawUI();
            drawMode2();
            break;
        case 3:
            drawMode3();
            break;
        case 4:
            drawMode4();
            break;
        case 5:
            drawMode5();
            break;
        default:
            break;
    }
}

void ChangeParametersTool::changeMode(){
    switch (mode) {
        case 0:
            break;
        case 1:{
            double weightObject = m_object->getWeight();
            closestWeightObject = nullptr;
            double minWeightDifference = std::numeric_limits<double>::max(); // Initialise avec la plus grande valeur possible
            for (auto& otherObj : m_renderContext->systemeSolaire->objects) {
                double weightDifference = std::abs(otherObj->getWeight() - weightObject); // Calcule la différence de masse
                if (weightDifference < minWeightDifference) {
                    minWeightDifference = weightDifference; // Mise à jour de la plus petite différence
                    closestWeightObject = otherObj; // Mise à jour de l'objet le plus proche
                }
            }
            massRatio = weightObject/closestWeightObject->getWeight();
            currentItemName = closestWeightObject->getName();
            mass = weightObject;
            lastMass = 0;
            lastMassRatio = 0;
            lastClosestWeightObject = nullptr;
            initialMassRatio = massRatio;
            initialMass = mass;
            initialClosestWeightObject = closestWeightObject;

            break;}
        case 2:{
            double radiusObject = m_object->real_radius;
            closestRadiusObject = nullptr;
            double minRadiusDifference = std::numeric_limits<double>::max(); // Initialise avec la plus grande valeur possible
            for (auto& otherObj : m_renderContext->systemeSolaire->objects) {
                double radiusDifference = std::abs(otherObj->real_radius - radiusObject); // Calcule la différence de masse
                if (radiusDifference < minRadiusDifference) {
                    minRadiusDifference = radiusDifference; // Mise à jour de la plus petite différence
                    closestRadiusObject = otherObj; // Mise à jour de l'objet le plus proche
                }
            }
            radiusRatio = radiusObject/closestRadiusObject->real_radius;
            currentItemName = closestRadiusObject->getName();
            radi = radiusObject;
            lastRadi = 0;
            lastRadi = 0;
            lastClosestRadiusObject = nullptr;
            initialRadiusRatio = radiusRatio;
            initialRadi = radi;
            initialClosestRadiusObject = closestRadiusObject;

            break;}
        case 3:
            editPositionFunction(m_object);
            if(*(m_renderContext->showCameraOptions)){*(m_renderContext->showCameraOptions) = false;}
            mode = 0;
            break;
        case 4:
            planeteName = m_object->getName();
            labbelsMode4[2]->isHidden = true;
            break;
        case 5:
            break;
        default:
            break;
    }

}

void ChangeParametersTool::initMode1(){
    std::function<void(std::string)> playSoundFunc = 
    std::bind(&SoundTool::playSound, m_renderContext->soundTool, std::placeholders::_1);
    Labbel* changeWeight = new Labbel(0.5,0.355,ImVec4(255,255,255,255),
                "Change Mass",28,0.85f);
    Labbel* weight = new Labbel(0.6,0.45,ImVec4(255,255,255,255),
                "Select mass:",23,0.85f);
    labbelsMode1.push_back(changeWeight);
    labbelsMode1.push_back(weight);

    float taille_y = 0.06f*0.8;
    Icon* weightI = new Icon(0.5-(0.35/2)*0.925, 0.5-(0.35/2)*0.9, ImVec2(taille_y*0.5, taille_y*0.5),0.6f,"../assets/button/weight.png",0.5);
    icons.push_back(weightI);

    float xPos = 0.45;
    float spacingY = 0.11;
    float topY = 0.5-spacingY;
    float bottomY = 0.5+spacingY;
    Icon* feather = new Icon(xPos, topY, ImVec2(taille_y*0.3, taille_y*0.3),0.6f,"../assets/button/feather.png",0.5);
    Icon* heavy = new Icon(xPos, bottomY, ImVec2(taille_y*0.3, taille_y*0.3),0.6f,"../assets/button/heavy.png",0.5);
    iconsMode1.push_back(feather);
    iconsMode1.push_back(heavy);

    ImageButton* reset = new ImageButton(playSoundFunc,0.6, 0.6, ImVec2(taille_y, taille_y),0.6f,
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f),ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                        "../assets/button/reset2.png", 0,
                            std::bind(&ChangeParametersTool::resetWeight, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    imageButtonsMode1.push_back(reset);
}
void ChangeParametersTool::initMode2(){
    std::function<void(std::string)> playSoundFunc = 
    std::bind(&SoundTool::playSound, m_renderContext->soundTool, std::placeholders::_1);
    Labbel* changeRadius = new Labbel(0.5,0.355,ImVec4(255,255,255,255),
                "Change Radius",28,0.85f);
    Labbel* radius = new Labbel(0.6,0.45,ImVec4(255,255,255,255),
                "Radius:",23,0.85f);
    labbelsMode2.push_back(changeRadius);
    labbelsMode2.push_back(radius);

    float taille_y = 0.06f*0.8;
    Icon* radiusI = new Icon(0.5-(0.35/2)*0.925, 0.5-(0.35/2)*0.9, ImVec2(taille_y*0.5, taille_y*0.5),0.6f,"../assets/button/changeRadius.png",0.5);
    iconsMode2.push_back(radiusI);

    ImageButton* reset = new ImageButton(playSoundFunc,0.6, 0.6, ImVec2(taille_y, taille_y),0.6f,
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f),ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                        "../assets/button/reset2.png", 0,
                            std::bind(&ChangeParametersTool::resetRadius, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    imageButtonsMode2.push_back(reset);
}
void ChangeParametersTool::initMode3(){

}
void ChangeParametersTool::initMode4(){
    std::function<void(std::string)> playSoundFunc = 
    std::bind(&SoundTool::playSound, m_renderContext->soundTool, std::placeholders::_1);
    Labbel* changeName = new Labbel(0.5,0.44,ImVec4(255,255,255,255),
                "Change Name",28,0.85f);
    Labbel* name =new Labbel(0.45,0.5,ImVec4(255,255,255,200),
                "Name: ",20,0.85f);
    Labbel* nameTaken =new Labbel(0.5,0.53,ImVec4(255,0,0,150),
                "This name is taken",17,0.85f);
    labbelsMode4.push_back(changeName);
    labbelsMode4.push_back(name);
    labbelsMode4.push_back(nameTaken);
    ImVec4 button_color = ImVec4(0.17f, 0.27f, 0.17f, 1.0f);
    float taille_x = 0.035f*0.8;
    float taille_y = 0.06f*0.8;
    ImageButton *closeButton = new ImageButton(playSoundFunc,0.5+(0.25/2)*0.915, 0.5-(0.2/2)*0.8, ImVec2(taille_x*0.5, taille_y*0.5),0.60,
                        button_color,button_color,
                        "../assets/button/close.png", 0,
                            std::bind(&ChangeParametersTool::closeButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    imageButtonsMode4.push_back(closeButton);
    ImageButton* reset = new ImageButton(playSoundFunc,0.56, 0.505, ImVec2(taille_y*0.9, taille_y*0.9),0.6f,
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f),ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                        "../assets/button/reset2.png", 0,
                            std::bind(&ChangeParametersTool::resetName, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    imageButtonsMode4.push_back(reset);

   Button *SaveButton = new Button(playSoundFunc,0.53, 0.5+(0.2/2)*0.75, ImVec2(taille_x*1.6, taille_y*0.65),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               "Save", 0.2f,20.0f,
                            std::bind(&ChangeParametersTool::Save, this),5);   
    Button *Cancel = new Button(playSoundFunc,0.47, 0.5+(0.2/2)*0.75, ImVec2(taille_x*1.6, taille_y*0.65),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                            "Cancel", 0.05f,19.0f,
                        std::bind(&ChangeParametersTool::closeButton, this),5); 
    buttonsMode4.push_back(SaveButton);
    buttonsMode4.push_back(Cancel);


}
void ChangeParametersTool::initMode5(){

}


void ChangeParametersTool::drawMode1(){ // Change Weight
if (lastMass == 0) lastMass = mass;
if (lastMassRatio == 0) lastMassRatio = 1.0; // Assuming default ratio is 1.0
if(lastClosestWeightObject == nullptr) lastClosestWeightObject = closestWeightObject;
    for (auto& label : labbelsMode1) {
        label->Draw();
    }
    for (auto& icon : icons) {
        icon->Draw();
    }
    for (auto& button : imageButtonsMode1) {
        button->Draw();
    }
    for(auto& button : iconsMode1){
        button->Draw();
    }
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImGui::PushFont(font);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95, 1, 0.95, 0.0)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.95, 1, 0.95, 0.2)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.8f, 0.8f, 0.8f, 0.4f)); // Couleur de sélection du texte
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // Elément sélectionné
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.7f)); // Survol
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 1.0f, 0.6f, 1.0f)); // Sélectionné
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.8f)); // Texte
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 2)); // Ajustez ces valeurs selon vos besoins

    ImVec2 centerPos = ImVec2(winWidth * 0.61f, winHeight * 0.475f);


    const char* buffer = "1000.0x";
    // Utilisez CalcTextSize pour calculer la longueur du texte
    float textSize2 = ImGui::CalcTextSize(buffer, NULL, false, 0.0f).x*1.12;
    float space = winWidth*0.01;
    ImGui::PushItemWidth(textSize2);
    ImVec2 cursorPos = ImVec2(centerPos.x-space/2-textSize2, centerPos.y);
    ImGui::SetCursorPos(cursorPos);

    char ratio[64]; // Assurez-vous que ce buffer est suffisamment grand
    snprintf(ratio, sizeof(massRatio), " %.2f x", mass); // Format de la masse comme dans ImGui::InputDouble
    float sizeRatio = ImGui::CalcTextSize(ratio, NULL, false, 0.0f).x;

    ImGui::InputFloat("##Planete Weight", &massRatio, 0.0f, 0.0f, "%.2f x");
    float sizeForPosition2 = ImGui::CalcTextSize(closestWeightObject->getName().c_str(), NULL, false, 0.0f).x;
    ImGui::PushItemWidth(sizeForPosition2*1.3);
    cursorPos = ImVec2(centerPos.x, centerPos.y);
    ImGui::SetCursorPos(cursorPos);
    //Draw a circle at both cursorPos
    //Get drawList
    
    if (ImGui::BeginCombo("##Choose mass", closestWeightObject->getName().c_str(), ImGuiComboFlags_NoArrowButton)) {
        for (auto& otherObj : m_renderContext->systemeSolaire->objects) {
            std::string name = otherObj->getName();
            
            std::string item = name;

            bool isSelected = (closestWeightObject->getName() == item);
            if (ImGui::Selectable(item.c_str(), isSelected)) {
                closestWeightObject = otherObj;
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }

        }
        ImGui::EndCombo();
    }

    // Check if massRatio was changed by the user
    if (massRatio != lastMassRatio || closestWeightObject != lastClosestWeightObject) {
        mass = massRatio * closestWeightObject->getWeight(); // Adjust the mass based on the new ratio
        lastMassRatio = massRatio; // Update lastMassRatio
        lastMass = mass; // Update lastMass to reflect the new mass
        lastClosestWeightObject = closestWeightObject ; // Update lastClosestWeightObject
    }




    buffer = "10000000000.0x";
    // Utilisez CalcTextSize pour calculer la longueur du texte
    textSize2 = ImGui::CalcTextSize(buffer, NULL, false, 0.0f).x*1.12;
    char massString[64]; // Assurez-vous que ce buffer est suffisamment grand
    snprintf(massString, sizeof(massString), " %.3e kg", mass); // Format de la masse comme dans ImGui::InputDouble
    float sizeForPosition = ImGui::CalcTextSize(massString, NULL, false, 0.0f).x;
    cursorPos = ImVec2(winWidth * 0.6f-sizeForPosition/2, winHeight * 0.52f);
    ImGui::SetCursorPos(cursorPos);
    ImGui::PushItemWidth(textSize2);
    
    ImGui::InputDouble("##Planete Weight total", &mass, 0.0f, 0.0f, " %.3e kg");
    const double minMass = 0.0;  // Remplacez par votre valeur minimale
    const double maxMass = 10e50;  // Remplacez par votre valeur maximale

    // Clamper la valeur entre min et max
    if (mass < minMass) {
        mass = minMass;
    } else if (mass > maxMass) {
        mass = maxMass;
    }
    if (mass != lastMass) {
            double weightObject = mass;
            CelestialObject* closest = nullptr;
            double minWeightDifference = std::numeric_limits<double>::max(); // Initialise avec la plus grande valeur possible
            for (auto& otherObj : m_renderContext->systemeSolaire->objects) {
                double weightDifference = std::abs(otherObj->getWeight() - weightObject); // Calcule la différence de masse
                if (weightDifference < minWeightDifference) {
                    minWeightDifference = weightDifference; // Mise à jour de la plus petite différence
                    closest = otherObj; // Mise à jour de l'objet le plus proche
                }
            }
            massRatio = weightObject/closest->getWeight();
            lastMassRatio = massRatio; // Update lastMassRatio to reflect the new ratio
            lastClosestWeightObject = closest; // Update lastClosestWeightObject to reflect the new closest object
            closestWeightObject = closest;
            lastMass = mass; // Update lastMass to reflect the new mass
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(7);
    ImGui::PopFont();
    drawPlanets();

    ImVec2 topleft = ImVec2(winWidth*0.55,winHeight*0.45) ;
    ImVec2 bottomRight = ImVec2(winWidth*0.65,winHeight*0.6);

    if(mass!=initialMass){
        imageButtonsMode1[0]->hidden = false;
    }else{
        imageButtonsMode1[0]->hidden = true;
    }

}

void ChangeParametersTool::drawPlanets(){
    ImGui::PushFont(planeteNameFont);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float radius = winWidth*0.005;
    float space = 0;
    bool right = true;
    bool hover = false;
    bool isThereSun = false;

    ImVec2 center = ImVec2(winWidth*0.45,winHeight*0.5);
    float spacingX = winWidth*0.025;
    float spacingY = winHeight*0.1;
    float rightX = center.x+spacingX;
    float leftX = center.x-spacingX;
    float topY = center.y-spacingY;
    float bottomY = center.y+spacingY;

    float largeurRect = spacingX/5;
    ImVec2 leftCornerRec = ImVec2(center.x-largeurRect/2, topY);
    ImVec2 rightCornerRec = ImVec2(center.x+largeurRect/2, bottomY);
    drawList->AddRectFilled(leftCornerRec, 
            rightCornerRec,
            IM_COL32(255,255,255, 50),20);


    std::vector<decltype(m_renderContext->systemeSolaire->objects)::value_type> sortedObjects(m_renderContext->systemeSolaire->objects.begin(), m_renderContext->systemeSolaire->objects.end());

    // Triez le nouveau vecteur par masse
    std::sort(sortedObjects.begin(), sortedObjects.end(),
        [](CelestialObject* a, CelestialObject* b) {
            return a->getWeight() < b->getWeight();
        }
    );
    // Calculer les logarithmes des masses
    float logMinMass = std::numeric_limits<float>::max();
    float logMaxMass = std::numeric_limits<float>::lowest();
    for(const auto& Obj : sortedObjects){
            if(Obj->typeName == "Sun"){
            isThereSun = true;
        }
            float logMass = std::log(Obj->getWeight());
            logMinMass = std::min(logMinMass, logMass);
            logMaxMass = std::max(logMaxMass, logMass);
    }

    float logRange = logMaxMass - logMinMass;
    logMinMass -= logRange * 0.15; // Diminue la masse minimale de 15%
    logMaxMass += logRange * 0.20; // Augmente la masse maximale de 15%

    bool positionDroite = true;
    for(const auto& Obj : sortedObjects){
            ImVec4 color = typeDictColor[Obj->typeName];
        if(Obj != m_object){
            float logMass = std::log(Obj->getWeight());
            float massRatio = (logMass - logMinMass) / (logMaxMass - logMinMass);
            float posY = topY + massRatio * (bottomY - topY);
            posY = std::max(posY, topY);
            posY = std::min(posY, bottomY);
            float posX = positionDroite ? rightX : leftX;

            // Calculez la taille du texte
            std::string objName = Obj->getName(); // Stockez le nom dans un std::string
            const char* name = objName.c_str();   // Utilisez c_str() sur le std::string stocké

            ImVec2 textSize = ImGui::CalcTextSize(name);

            // Positionnez le texte à droite ou à gauche de la planète
            float textX = positionDroite ? (posX + radius + 5) : (posX - textSize.x - radius - 5);
            float textY = posY - textSize.y / 2; // Centre le texte verticalement par rapport à la planète
            drawList->AddCircleFilled(ImVec2(posX, posY), radius, IM_COL32(color.x, color.y, color.z, 200), 100);
            drawList->AddLine(ImVec2(posX, posY), ImVec2(center.x, posY), IM_COL32(255,255,255, 50), winWidth*0.00075);
            drawList->AddText(ImVec2(textX, textY), IM_COL32(255, 255, 255, 150), name);
            drawList->AddCircle(ImVec2(posX, posY), radius*1.3, IM_COL32(255,255,255, 20), 100, winWidth*0.00075);
            if(Obj->typeName == "Sun"){
                drawSunEffect(ImVec2(posX, posY), radius);
                sunPos = ImVec2(posX, posY);
            }else if(isThereSun){
                drawPlanetLight(ImVec2(posX, posY), ImVec2(sunPos.x,sunPos.y), radius);
            }
            positionDroite = !positionDroite;
        }
    else {
            float logMass = std::log(mass);
            float massRatio = (logMass - logMinMass) / (logMaxMass - logMinMass);
            float posY = topY + massRatio * (bottomY - topY);
            posY = std::max(posY, topY);
            posY = std::min(posY, bottomY);
            ImVec2 mousePos = ImGui::GetMousePos();
            float distance = std::sqrt(std::pow(mousePos.x - center.x, 2) + std::pow(mousePos.y - posY, 2));
            if(distance <= 5*radius){
                hover = true;
    if (ImGui::IsMouseDragging(0, 0.0f)) {
        ImVec2 delta = ImGui::GetMouseDragDelta(0, 0.0f);
             // Seuil de détection défini par le rayon
                posY += delta.y;
                posY = std::max(posY, topY); // Clampez posY pour qu'elle ne dépasse pas topY ou bottomY
                posY = std::min(posY, bottomY);

                // Calculer la nouvelle masse en fonction de posY
                float newMassRatio = (posY - topY) / (bottomY - topY);
                float newLogMass = newMassRatio * (logMaxMass - logMinMass) + logMinMass;
                float newMass = std::exp(newLogMass);

                // Mettre à jour la masse de m_object (si nécessaire)
                mass = newMass;
        ImGui::ResetMouseDragDelta(0);
    }}
            ImGui::PopFont();
            ImGui::PushFont(planateSelectedFont);
            // Positionnement et dessin des autres planètes
            float posX = positionDroite ? center.x + spacingX : center.x - spacingX;

            // Calculez la taille du texte
            std::string objName = Obj->getName(); // Stockez le nom dans un std::string
            const char* name = objName.c_str();   // Utilisez c_str() sur le std::string stocké

            ImVec2 textSize = ImGui::CalcTextSize(name);

            // Positionnez le texte à droite ou à gauche de la planète
            float textX = positionDroite ? (posX + radius + 5) : (posX - textSize.x - radius - 5);
            float textY = posY - textSize.y / 2; // Centre le texte verticalement par rapport à la planète
            int alpha = hover ? 255 : 200;
            int alphaCercle = hover ? 255 : 100;
            drawList->AddCircleFilled(ImVec2(center.x, posY), radius, IM_COL32(color.x, color.y, color.z, 255),100);
            drawList->AddText(ImVec2(center.x-textSize.x/2, textY-radius*3), IM_COL32(200, 255, 200, alpha), name);

            positionDroite = !positionDroite;
            if(Obj->getTypeName() == "Sun"){
                drawSunEffect(ImVec2(center.x, posY), radius);
                sunPos = ImVec2(center.x, posY);
            }else if (isThereSun){
                drawPlanetLight(ImVec2(center.x, posY), ImVec2(sunPos.x,sunPos.y), radius);
            }
            drawList->AddCircle(ImVec2(center.x, posY), radius, IM_COL32(255,255,255, alphaCercle), 100, winWidth*0.0015);
    }}
    ImGui::PopFont();
}

void ChangeParametersTool::drawSunEffect(ImVec2 planetPos, float radius){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec4 colorCenterDot = ImVec4(255,215,80,255);
    float numBlurCircles = 40;
    float blurIncrease = radius*0.04; // How much larger each successive blur circle is
    float initialAlpha = 20; // Starting alpha value for the outermost blur circle
    float alphaDecrease = initialAlpha / numBlurCircles; // How much alpha decreases per circle
    for (int i = 0; i < numBlurCircles; ++i) {
        float blurRadius = radius + blurIncrease * (i + 1);
        float alpha = initialAlpha - alphaDecrease * i;
        drawList->AddCircleFilled(planetPos, blurRadius, IM_COL32(colorCenterDot.x,colorCenterDot.y,colorCenterDot.z, alpha), 100);}
}


void ChangeParametersTool::drawPlanetLight(ImVec2 planetPos, ImVec2 sunPos, float radius){
        ImVec2 directionToSun = ImVec2(planetPos.x - sunPos.x, planetPos.y - sunPos.y);
        float shadowAngle = atan2(directionToSun.y, directionToSun.x) + IM_PI; // Ajouter PI pour que l'ombre soit opposée au Soleil
        // Dessiner l'ombre comme un demi-cercle
        draw_half_circle_shadow(planetPos, radius, IM_COL32(0, 0, 0, 100), shadowAngle, 100);
}
void ChangeParametersTool::draw_half_circle_shadow(ImVec2 center, float radius, ImU32 color, float angle, int num_segments) {
    // Calcule les angles pour le demi-cercle d'ombre
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    radius*=1.1;
    float start_angle = angle - IM_PI / 2-M_PI; // Commence à 90 degrés par rapport à l'angle d'ombre
    float end_angle = angle + IM_PI / 2-M_PI;   // Termine à 270 degrés par rapport à l'angle d'ombre

    // Début du chemin par le centre
    drawList->PathLineTo(center);

    // Ajout de l'arc pour le demi-cercle
    for (int i = 0; i <= num_segments; ++i) {
        float segment_angle = start_angle + (float(i) / float(num_segments)) * (end_angle - start_angle);
        ImVec2 vertex = ImVec2(center.x + cos(segment_angle) * radius, center.y + sin(segment_angle) * radius);
        drawList->PathLineTo(vertex);
    }

    // Fermeture du chemin en revenant au centre
    drawList->PathLineTo(center);

    // Remplissage du demi-cercle
    drawList->PathFillConvex(color);
}


void ChangeParametersTool::drawMode2(){ // Change Radius
    for (auto& label : labbelsMode2) {
        label->Draw();
    }
    for (auto& icon : icons) {
        icon->Draw();
    }
    for (auto& button : imageButtonsMode2) {
        button->Draw();
    }
    if(radi != initialRadi){
        imageButtonsMode2[0]->hidden = false;
    }else{
        imageButtonsMode2[0]->hidden = true;
    }

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImGui::PushFont(font);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95, 1, 0.95, 0.0)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.95, 1, 0.95, 0.2)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.8f, 0.8f, 0.8f, 0.4f)); // Couleur de sélection du texte
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // Elément sélectionné
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.7f)); // Survol
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 1.0f, 0.6f, 1.0f)); // Sélectionné
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.8f)); // Texte
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 2)); // Ajustez ces valeurs selon vos besoins

    ImVec2 centerPos = ImVec2(winWidth * 0.61f, winHeight * 0.475f);

    const char* buffer = "1000.0x";
    // Utilisez CalcTextSize pour calculer la longueur du texte
    float textSize2 = ImGui::CalcTextSize(buffer, NULL, false, 0.0f).x*1.12;
    float space = winWidth*0.01;
    ImGui::PushItemWidth(textSize2);
    ImVec2 cursorPos = ImVec2(centerPos.x-space/2-textSize2, centerPos.y);
    ImGui::SetCursorPos(cursorPos);

    char ratio[64]; // Assurez-vous que ce buffer est suffisamment grand
    snprintf(ratio, sizeof(radiusRatio), " %.2f x", radi); // Format de la masse comme dans ImGui::InputDouble
    float sizeRatio = ImGui::CalcTextSize(ratio, NULL, false, 0.0f).x;

    ImGui::InputFloat("##Planete Weight", &radiusRatio, 0.0f, 0.0f, "%.2f x");
    float sizeForPosition2 = ImGui::CalcTextSize(closestRadiusObject->getName().c_str(), NULL, false, 0.0f).x;
    ImGui::PushItemWidth(sizeForPosition2*1.3);
    cursorPos = ImVec2(centerPos.x, centerPos.y);
    ImGui::SetCursorPos(cursorPos);
    //Draw a circle at both cursorPos
    //Get drawList
    
    if (ImGui::BeginCombo("##Choose mass", closestRadiusObject->getName().c_str(), ImGuiComboFlags_NoArrowButton)) {
        for (auto& otherObj : m_renderContext->systemeSolaire->objects) {
            std::string name = otherObj->getName();

            std::string item = name;

            bool isSelected = (closestRadiusObject->getName() == item);
            if (ImGui::Selectable(item.c_str(), isSelected)) {
                closestRadiusObject = otherObj;
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }

        }
        ImGui::EndCombo();
    }

    // Check if massRatio was changed by the user
    if (radiusRatio != lastRadiusRatio || closestRadiusObject != lastClosestRadiusObject) {
        radi = radiusRatio * closestRadiusObject->real_radius; // Adjust the mass based on the new ratio
        lastRadiusRatio = radiusRatio; // Update lastMassRatio
        lastRadi = radi; // Update lastMass to reflect the new mass
        lastClosestRadiusObject = closestRadiusObject ; // Update lastClosestWeightObject
    }




    buffer = "10000000000.0x";
    // Utilisez CalcTextSize pour calculer la longueur du texte
    textSize2 = ImGui::CalcTextSize(buffer, NULL, false, 0.0f).x*1.12;
    char radiusString[64]; // Assurez-vous que ce buffer est suffisamment grand
    snprintf(radiusString, sizeof(radiusString), " %.3e km", radi); // Format de la masse comme dans ImGui::InputDouble
    float sizeForPosition = ImGui::CalcTextSize(radiusString, NULL, false, 0.0f).x;
    cursorPos = ImVec2(winWidth * 0.6f-sizeForPosition/2, winHeight * 0.52f);
    ImGui::SetCursorPos(cursorPos);
    ImGui::PushItemWidth(textSize2);
    
    ImGui::InputDouble("##Planete Weight total", &radi, 0.0f, 0.0f, " %.3e km");
    const double minRadi = 0.0;  // Remplacez par votre valeur minimale
    const double maxRadi = 1e10;  // Remplacez par votre valeur maximale

    // Clamper la valeur entre min et max
    if (radi < minRadi) {
        radi = minRadi;
    } else if (radi > maxRadi) {
        radi = maxRadi;
    }
    if (radi != lastRadi) {
            double radiusObject = radi;
            CelestialObject* closest = nullptr;
            double minRadiusDifference = std::numeric_limits<double>::max(); // Initialise avec la plus grande valeur possible
            for (auto& otherObj : m_renderContext->systemeSolaire->objects) {
                double radiusDifference = std::abs(otherObj->real_radius - radiusObject); // Calcule la différence de masse
                if (radiusDifference < minRadiusDifference) {
                    minRadiusDifference = radiusDifference; // Mise à jour de la plus petite différence
                    closest = otherObj; // Mise à jour de l'objet le plus proche
                }
            }
            radiusRatio = radiusObject/closest->real_radius;
            lastRadiusRatio = radiusRatio; // Update lastMassRatio to reflect the new ratio
            lastClosestRadiusObject = closest; // Update lastClosestWeightObject to reflect the new closest object
            closestRadiusObject = closest;
            lastRadi = radi; // Update lastMass to reflect the new mass
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(7);
    ImGui::PopFont();
    drawPlanetsMode2();

    ImVec2 topleft = ImVec2(winWidth*0.55,winHeight*0.45) ;
    ImVec2 bottomRight = ImVec2(winWidth*0.65,winHeight*0.6);

}



void ChangeParametersTool::drawPlanetsMode2(){
    ImGui::PushFont(planeteNameFont);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float radius = winWidth*0.005;
    float radiusMin = radius*0.25;
    float radiusMax = radius*1.75;
    float space = 0;
    bool right = true;
    bool hover = false;
    bool isThereSun = false;

    ImVec2 center = ImVec2(winWidth*0.45,winHeight*0.5);
    float spacingX = winWidth*0.025;
    float spacingY = winHeight*0.1;
    float rightX = center.x+spacingX;
    float leftX = center.x-spacingX;
    float topY = center.y-spacingY;
    float bottomY = center.y+spacingY;

    float largeurRect = spacingX/5;
    ImVec2 leftCornerRec = ImVec2(center.x-largeurRect/2, topY);
    ImVec2 rightCornerRec = ImVec2(center.x+largeurRect/2, bottomY);
    drawList->AddRectFilled(leftCornerRec, 
            rightCornerRec,
            IM_COL32(255,255,255, 50),20);


    std::vector<decltype(m_renderContext->systemeSolaire->objects)::value_type> sortedObjects(m_renderContext->systemeSolaire->objects.begin(), m_renderContext->systemeSolaire->objects.end());

    // Triez le nouveau vecteur par masse
    std::sort(sortedObjects.begin(), sortedObjects.end(),
        [](CelestialObject* a, CelestialObject* b) {
            return a->real_radius < b->real_radius;
        }
    );
    // Calculer les logarithmes des masses
    float logMinRadius = std::numeric_limits<float>::max();
    float logMaxRadius = std::numeric_limits<float>::lowest();
    for(const auto& Obj : sortedObjects){
            if(Obj->typeName == "Sun"){
            isThereSun = true;
        }
            float logRadius = std::log(Obj->real_radius);
            logMinRadius = std::min(logMinRadius, logRadius);
            logMaxRadius = std::max(logMaxRadius, logRadius);
    }

    float logRange = logMaxRadius - logMinRadius;
    logMinRadius -= logRange * 0.15; // Diminue la masse minimale de 15%
    logMaxRadius += logRange * 0.20; // Augmente la masse maximale de 15%

    bool positionDroite = true;
    for(const auto& Obj : sortedObjects){
            ImVec4 color = typeDictColor[Obj->typeName];
        if(Obj != m_object){
            float logRadius = std::log(Obj->real_radius);
            float radiusRatio = (logRadius - logMinRadius) / (logMaxRadius - logMinRadius);
            float posY = topY + radiusRatio * (bottomY - topY);
            float radius = radiusMin + radiusRatio * (radiusMax - radiusMin);
            posY = std::max(posY, topY);
            posY = std::min(posY, bottomY);
            float posX = positionDroite ? rightX : leftX;

            // Calculez la taille du texte
            std::string objName = Obj->getName(); // Stockez le nom dans un std::string
            const char* name = objName.c_str();   // Utilisez c_str() sur le std::string stocké

            ImVec2 textSize = ImGui::CalcTextSize(name);

            // Positionnez le texte à droite ou à gauche de la planète
            float textX = positionDroite ? (posX + 2*radius) : (posX - textSize.x - 2*radius);
            float textY = posY - textSize.y / 2; // Centre le texte verticalement par rapport à la planète
            drawList->AddCircleFilled(ImVec2(posX, posY), radius, IM_COL32(color.x, color.y, color.z, 200), 100);
            drawList->AddLine(ImVec2(posX, posY), ImVec2(center.x, posY), IM_COL32(255,255,255, 50), winWidth*0.00075);
            drawList->AddText(ImVec2(textX, textY), IM_COL32(255, 255, 255, 150), name);
            drawList->AddCircle(ImVec2(posX, posY), radius*1.3, IM_COL32(255,255,255, 20), 100, winWidth*0.00075);
            if(Obj->typeName == "Sun"){
                drawSunEffect(ImVec2(posX, posY), radius);
                sunPos = ImVec2(posX, posY);
            }else if(isThereSun){
                drawPlanetLight(ImVec2(posX, posY), ImVec2(sunPos.x,sunPos.y), radius);
            }
            positionDroite = !positionDroite;
        }
    else {
            float logRadius = std::log(radi);
            float radiusRatio = (logRadius - logMinRadius) / (logMaxRadius - logMinRadius);
            float posY = topY + radiusRatio * (bottomY - topY);
            float radius = radiusMin + radiusRatio * (radiusMax - radiusMin);
            posY = std::max(posY, topY);
            posY = std::min(posY, bottomY);
            ImVec2 mousePos = ImGui::GetMousePos();
            float distance = std::sqrt(std::pow(mousePos.x - center.x, 2) + std::pow(mousePos.y - posY, 2));
            if(distance <= winWidth*0.025){
                hover = true;
    if (ImGui::IsMouseDragging(0, 0.0f)) {
        ImVec2 delta = ImGui::GetMouseDragDelta(0, 0.0f);
             // Seuil de détection défini par le rayon
                posY += delta.y;
                posY = std::max(posY, topY); // Clampez posY pour qu'elle ne dépasse pas topY ou bottomY
                posY = std::min(posY, bottomY);

                // Calculer la nouvelle masse en fonction de posY
                float newRadiusRatio = (posY - topY) / (bottomY - topY);
                float newLogRadius = newRadiusRatio * (logMaxRadius - logMinRadius) + logMinRadius;
                float newRadius = std::exp(newLogRadius);

                // Mettre à jour la masse de m_object (si nécessaire)
                radi = newRadius;
        ImGui::ResetMouseDragDelta(0);
    }}
            ImGui::PopFont();
            ImGui::PushFont(planateSelectedFont);
            // Positionnement et dessin des autres planètes
            float posX = positionDroite ? center.x + spacingX : center.x - spacingX;

            // Calculez la taille du texte
            std::string objName = Obj->getName(); // Stockez le nom dans un std::string
            const char* name = objName.c_str();   // Utilisez c_str() sur le std::string stocké

            ImVec2 textSize = ImGui::CalcTextSize(name);

            // Positionnez le texte à droite ou à gauche de la planète
            float textX = positionDroite ? (posX + radius + 5) : (posX - textSize.x - radius - 5);
            float textY = posY - textSize.y / 2; // Centre le texte verticalement par rapport à la planète
            int alpha = hover ? 255 : 200;
            int alphaCercle = hover ? 255 : 100;
            drawList->AddCircleFilled(ImVec2(center.x, posY), radius, IM_COL32(color.x, color.y, color.z, 255),100);
            drawList->AddText(ImVec2(center.x-textSize.x/2, textY-radius*3), IM_COL32(200, 255, 200, alpha), name);

            positionDroite = !positionDroite;
            if(Obj->getTypeName() == "Sun"){
                drawSunEffect(ImVec2(center.x, posY), radius);
                sunPos = ImVec2(center.x, posY);
            }else if (isThereSun){
                drawPlanetLight(ImVec2(center.x, posY), ImVec2(sunPos.x,sunPos.y), radius);
            }
            drawList->AddCircle(ImVec2(center.x, posY), radius, IM_COL32(255,255,255, alphaCercle), 100, winWidth*0.0015);
    }}
    ImGui::PopFont();
}
void ChangeParametersTool::drawMode3(){}
void ChangeParametersTool::drawMode4(){ // Change Name
    float longueur = winWidth* 0.25; // Exemple de taille
    float hauteur = winHeight * 0.2; // Exemple de taille

    ImVec2 centerPos = ImVec2(winWidth * 0.5f, winHeight * 0.5f);
    ImVec2 topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = winWidth*0.01;

    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(0,0,0, 255), // Couleur
                            cornerRadius);
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(20, 25, 30, 200), // Couleur
                            cornerRadius);


    for (auto& label : labbelsMode4) {
        label->Draw();
    }
    for (auto& icon : iconsMode4) {
        icon->Draw();
    }
    for (auto& button : imageButtonsMode4) {
        button->Draw();
    }
    for(auto& button : buttonsMode4){
        button->Draw();
    }

    ImGui::PushFont(font);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95, 1, 0.95, 0.3)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.95, 1, 0.95, 0.2)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.8f, 0.8f, 0.8f, 0.4f)); // Couleur de sélection du texte
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // Elément sélectionné
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.7f)); // Survol
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 1.0f, 0.6f, 1.0f)); // Sélectionné
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.8f)); // Texte

    char charBuffer[14];
    const char* buffer = "mercuremercure";
    ImVec2 textSize = ImGui::CalcTextSize(buffer, NULL, false, 0.0f);
    ImVec2 cursorPos = ImVec2(0.47*winWidth, 0.5*winHeight-textSize.y/2);
    ImGui::PushItemWidth(textSize.x*1.12);
    ImGui::SetCursorPos(cursorPos);
std::strncpy(charBuffer, planeteName.c_str(), sizeof(charBuffer));
// Ensure null termination
charBuffer[sizeof(charBuffer) - 1] = 0;

// Use the buffer with ImGui::InputText
if (ImGui::InputText("##Planete Name", charBuffer, sizeof(charBuffer))) {
    // If InputText returns true, the buffer was modified.
    // Update the std::string with the new content of the buffer.
    planeteName = std::string(charBuffer);
}
bool nameAlreadyExist = false;
for (auto& object : m_renderContext->systemeSolaire->objects){
    if(object == m_object){
        continue;
    }
    if(object->getName() == planeteName){
        nameAlreadyExist = true;
    }}
if(nameAlreadyExist){
    buttonsMode4[0]->enabled = false;
    labbelsMode4[2]->isHidden = false;
}else{
    buttonsMode4[0]->enabled = true;
    labbelsMode4[2]->isHidden = true;
}
    ImGui::PopStyleColor(7);
    ImGui::PopFont();

if(planeteName!=m_object->getName()){
    imageButtonsMode4[1]->hidden = false;
}else{
    imageButtonsMode4[1]->hidden = true;
}
}



void ChangeParametersTool::drawMode5(){}

void ChangeParametersTool::closeButton(){
    Close();
}

void ChangeParametersTool::Close(){
    mode = 0;
    if(miniMapWasShown){
        closeMinimapFunction();
    }
}
void ChangeParametersTool::Open(){
}

void ChangeParametersTool::resetWeight(){
    if(mass!=initialMass){
        mass = initialMass;
    }
}

void ChangeParametersTool::resetRadius(){
    if(radi != initialRadi){
        radi = initialRadi;
    }
}

void ChangeParametersTool::resetName(){
    if(planeteName != m_object->getName()){
        planeteName =  m_object->getName();
    }
}


void ChangeParametersTool::Save(){
    switch(mode){
        case 1:{
            m_object->setWeight(mass);
            break;}
        case 2:
            m_object->real_radius = radi;
            m_renderContext->systemeSolaire->setRayon(m_object);
            m_renderContext->currentCamera->newFollowObject(m_object);
        case 3:
            break;
        case 4:
            m_object->setName(planeteName);
            updateNamesFunction();
            break;
        case 5:
            break;
        default:
            break;
    }
    changeConfirmed = true;
    showConfirmChangeBox = false;
    Close();
}



void ChangeParametersTool::generate_colors() {
    // Modifier le dictionnaire pour utiliser des chaînes de caractères comme clés
    typeDictColor["Black Hole"] = ImVec4(0, 0, 0, 1);
    typeDictColor["Sun"] = ImVec4(250, 250, 120, 1); // Jaune pour une étoile par exemple
    typeDictColor["Mercury"] = ImVec4(100, 100, 100, 1);
    typeDictColor["Venus"] = ImVec4(238, 147, 17, 1); // Orange pour Venus
    typeDictColor["Earth"] = ImVec4(50, 100, 255, 1); // Bleu pour la Terre
    typeDictColor["Mars"] = ImVec4(197, 84, 62, 1); // Rougeâtre pour Mars
    typeDictColor["Jupiter"] = ImVec4(234, 206, 180, 1);
    typeDictColor["Saturn"] = ImVec4(217, 186, 140, 1); // Jaune pour Saturne
    typeDictColor["Uranus"] = ImVec4(155, 221, 227, 1);
    typeDictColor["Neptune"] = ImVec4(39, 114, 210, 1); // Bleu pour Neptune
    typeDictColor["Ice"] = ImVec4(122, 137, 150, 1); // Bleu clair pour les objets glacés
    typeDictColor["Rock"] = ImVec4(137, 80, 77, 1); // Gris pour les objets rocheux
    typeDictColor["Volcanic"] = ImVec4(62, 95, 86, 1); // Gris foncé pour les objets volcaniques
    typeDictColor["Moon"] = ImVec4(200, 200, 200, 1); // Gris pour les lunes
}

void ChangeParametersTool::setupdateNamesFunction(const std::function<void()>& func){
    updateNamesFunction = func;
}

void ChangeParametersTool::setCloseMinimapFunction(const std::function<void()>& func){
    closeMinimapFunction = func;
}

void ChangeParametersTool::setEditPositionFunction(const std::function<void(CelestialObject* object)>& func){
    editPositionFunction = func;
}
