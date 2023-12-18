#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/CreationTools/ParametersCreator.h"
#include "engine/CreationTools/CreatorManager.h"
#include "engine/Vec3.h"
#include <string>       // Pour std::string
#include <fstream>      // Pour std::ifstream
#include <sstream>      // Pour std::stringstream
#include <iostream> 
#include "engine/RenderTools/GlowTool.h"
#include "engine/RenderTools/athmosphereTool.h"
#include "engine/RenderTools/saturnRingTool.h"


ParametersCreator::ParametersCreator(RenderContext* renderContext, CreatorManager* manager) : StateCreator(renderContext, manager) {
    generate_buttons();
    generate_labels();
    initDictionnaries();
    font = ImGui::GetIO().Fonts->AddFontFromFileTTF("../assets/fonts/RobotoB.ttf", 14);
}

void ParametersCreator::initDictionnaries(){
    //Dict for mass
    itemsWeightDictionary["Earth Mass"] = 5.972e24;   
    itemsWeightDictionary["Sun Mass"] = 1.989e30;     
    itemsWeightDictionary["Mercury Mass"] = 3.285e23; 
    itemsWeightDictionary["Venus Mass"] = 4.867e24;   
    itemsWeightDictionary["Mars Mass"] = 6.39e23;    
    itemsWeightDictionary["Jupiter Mass"] = 1.898e27; 
    itemsWeightDictionary["Saturn Mass"] = 5.683e26;  
    itemsWeightDictionary["Uranus Mass"] = 8.681e25;  
    itemsWeightDictionary["Neptune Mass"] = 1.024e26; 
    itemsWeightDictionary["Moon Mass"] = 7.34767309e22; 
    for (const auto& item : itemsWeightDictionary) {
        itemNamesWeight.insert(itemNamesWeight.begin(), item.first);
    }

    //Dict for radius
    itemsRadiusDictionary["Earth Radius"] = 6371e3;    // Rayon de la Terre en m
    itemsRadiusDictionary["Sun Radius"] = 696340e3;    // Rayon du Soleil en m
    itemsRadiusDictionary["Mercury Radius"] = 2439.7e3; // Rayon de Mercure en m
    itemsRadiusDictionary["Venus Radius"] = 6051.8e3;   // Rayon de Vénus en m
    itemsRadiusDictionary["Mars Radius"] = 3389.5e3;    // Rayon de Mars en m
    itemsRadiusDictionary["Jupiter Radius"] = 69911e3;  // Rayon de Jupiter en m
    itemsRadiusDictionary["Saturn Radius"] = 58232e3;   // Rayon de Saturne en m
    itemsRadiusDictionary["Uranus Radius"] = 25362e3;   // Rayon d'Uranus en m
    itemsRadiusDictionary["Neptune Radius"] = 24622e3;  // Rayon de Neptune en m
    itemsRadiusDictionary["Moon Radius"] = 1737.1e3;    // Rayon de la Lune en m
    for (const auto& item : itemsRadiusDictionary) {
        itemNamesRadius.insert(itemNamesRadius.begin(), item.first);
    }
}


void ParametersCreator::getItemInitialParameters(){
// Obtenir le nom de l'objet créé et chercher dans le dictionnaire
std::string objectName = m_manager->newCreatedObject->getName();
std::string searchKeyWeight = objectName + " Mass";
std::string searchKeyRadius = objectName + " Radius";

// Vérifier si la clé existe dans le dictionnaire
if (itemsWeightDictionary.find(searchKeyWeight) != itemsWeightDictionary.end()) {
    currentItemWeight = searchKeyWeight;
    currentItemRadius = searchKeyRadius;
} else if(m_manager->newCreatedObject->type==0){
    currentItemWeight = "Sun Mass";
    currentItemRadius = "Sun Radius";
}else {
    currentItemWeight = "Earth Mass";
    currentItemRadius = "Earth Radius";
}

massMultiplicator = 1.0f;
radiusMultiplicator = 1.0f;
sideralMultiplicator = m_manager->newCreatedObject->rotationSidSpeed * 60*60*24;

}




void ParametersCreator::Enter(){
    m_renderContext->currentCamera->resetPosition();
    m_renderContext->currentCamera->newFollowObject(m_manager->newCreatedObject);
    *(m_renderContext->showInfo)=false;
    strcpy(planetName, m_manager->newCreatedObject->getName().c_str());
    getItemInitialParameters();
} 


void ParametersCreator::Draw(){
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));

    ImGui::Begin("MessageBox", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    if (ImGui::IsKeyReleased(ImGuiKey_Escape)) {previous_state();}
    m_renderContext->currentCamera->orbitAroundObject(0.0020,0);
    drawBackground();
    draw_buttons();
    draw_labels();
    draw_input_name();
    draw_input_weight();
    draw_input_radius();
    draw_input_sideral();

    ImGui::End(); 
} 
void ParametersCreator::DrawOpenGL(){} 


void ParametersCreator::generate_buttons(){
   Button *ReturnButton = new Button(0.045f, 0.57, ImVec2(0.045, 0.03),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               "Previous", 0.1,19.0f,
                               std::bind(&ParametersCreator::previous_state, this));  

   Button *NextButton = new Button(0.105f, 0.57, ImVec2(0.04, 0.03),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               "Select", 0.4,19.0f,
                               std::bind(&ParametersCreator::next_state, this));  


    buttons.push_back(ReturnButton);
    buttons.push_back(NextButton);




    ImVec4 button_color = ImVec4(0.17f, 0.27f, 0.17f, 1.0f);
    ImageButton* closeButton = new ImageButton((0.07+0.075)*0.95,(0.5-0.27)*0.95,ImVec2(0.025f,0.025f),0.6f,
                        button_color, button_color,
                        "../assets/button/close.png", 0,
                            std::bind(&ParametersCreator::forceClose, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
        
    icon = new Icon((0.075-0.06)*0.95,(0.5-0.27)*0.95,ImVec2(0.025f,0.025f),0.35f,"../assets/button/parameters.png",0.85);
    imageButtons.push_back(closeButton);
}

void ParametersCreator::generate_sliders(){
}



void ParametersCreator::generate_labels(){
    float space = ImGui::GetTextLineHeightWithSpacing();
    Labbel *MessageLabel = new Labbel(0.075f,0.25f,ImVec4(255,255,255,255),
                            "Parameters",25.0f,0.9f);
    float position_x = 0.010f;
    float position_y = 0.35f;
    float diff_y = 0.05f;
    Labbel *NameLabel = new Labbel(position_x, position_y,ImVec4(255,255,255,255),
                            "Name",18.0f,0.7f);
    Labbel *MassLabel = new Labbel(position_x, position_y + diff_y,ImVec4(255,255,255,255),
                            "Mass",18.0f,0.7f);    
    Labbel *RadiusLabel = new Labbel(position_x, position_y + 2*diff_y,ImVec4(255,255,255,255),
                            "Radius",18.0f,0.7f);    
    Labbel *SideralLabel = new Labbel(position_x, position_y + 3*diff_y,ImVec4(255,255,255,255),
                            "Rotation",18.0f,0.7f);     


    labbels.push_back(MessageLabel);    
    labbels.push_back(NameLabel);
    labbels.push_back(MassLabel);
    labbels.push_back(RadiusLabel);
    labbels.push_back(SideralLabel);

    //Faire une boucle sur les labbels sauf le premier
    for (int i = 1; i < labbels.size(); i++) {
        labbels[i]->alignLeft = true;
    }

}
void ParametersCreator::drawBackground(){

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 10.0f;


    float longueur = winWidth* 0.14; // Exemple de taille
    float hauteur = winHeight * 0.40; // Exemple de taille

    centerPos = ImVec2(winWidth * 0.075f, winHeight * 0.4f);
    topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);
    bottomRight = ImVec2(topLeft.x + longueur, topLeft.y + hauteur);

    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(0,0,0,255), // Couleur
                            cornerRadius);

    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(20, 25, 30, 200), // Couleur
                            cornerRadius);
    drawList->AddRect(topLeft, 
                        ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                        IM_COL32(255,255,255,40), // Couleur
                        cornerRadius,0,0.2f);

}

void ParametersCreator::draw_buttons(){
        for (Button* btn : buttons) {
        btn->Draw();
    }
    for(ImageButton* btn : imageButtons){
        btn->Draw();
    }
    icon->Draw();
}

void ParametersCreator::draw_sliders(){

}


void ParametersCreator::draw_labels(){
        for (Labbel* label : labbels) {
        label->Draw();
    }
}


void ParametersCreator::next_state(){
    setParameters();
    *(m_renderContext->showInfo)=true;
    m_manager->newCreatedObject->isCreated=true;
    m_manager->creationConfirmed=true;
    m_manager->Exit();
}
void ParametersCreator::previous_state(){
    m_manager->ChangeState("VelocityCreator");
}

void ParametersCreator::Exit(){

} 

void ParametersCreator::setParameters(){

    //Mass
    double objectMass = itemsWeightDictionary[currentItemWeight] * massMultiplicator;
    m_manager->newCreatedObject->setWeight(objectMass);

    //Radius
    double objectRadius = itemsRadiusDictionary[currentItemRadius] * radiusMultiplicator;
    m_manager->newCreatedObject->real_radius=objectRadius;
    m_renderContext->systemeSolaire->setRayon(m_manager->newCreatedObject); //Change effect if needed:
    updateEffects();
    m_renderContext->currentCamera->newFollowObject(m_manager->newCreatedObject); //Reset cam with new object.
    
    //Name
    if(planetName[0] != '\0')
        {m_manager->newCreatedObject->setName(planetName);}
    
    //Sideral speed
    m_manager->newCreatedObject->rotationSidSpeed = sideralMultiplicator/(24*60*60);
}

void ParametersCreator::updateEffects(){
    CelestialObject* newObj = m_manager->newCreatedObject;
    //remove previus effect
    if(newObj->getName()=="Saturn"){
        delete newObj->saturnRingTool;
        newObj->saturnRingTool = new SaturnRingTool(newObj,m_renderContext);
    }else if(newObj->getName()=="Earth"){
        delete newObj->athmosphereTool;
        newObj->athmosphereTool = new AthmosphereTool(newObj,m_renderContext);}

    else if(newObj->getName()=="Sun"){
        delete newObj->glowTool;
        newObj->glowTool = new GlowTool(newObj,m_renderContext);
    }
}




void ParametersCreator::draw_input_name() {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImGui::PushFont(font);
    //calclation size of font
    ImVec2 textSize = ImGui::CalcTextSize(planetName, NULL, false, 0.0f);
    float offsetX = winWidth * 0.14;
    float offsetY = winHeight * 0.35 - textSize.y/2;
    float pourcentageX = 0.8;
    ImVec2 cursorPos = ImVec2(centerPos.x*pourcentageX, offsetY);
    // Calculate the width of the input background based on the text length
    float textWidth = ImGui::CalcTextSize(planetName).x;
    float inputWidth = winWidth * 0.14/2*0.95+centerPos.x*(1-pourcentageX); // Add some padding

    ImGui::SetCursorPos(cursorPos);
    // Draw the editable text
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95, 1, 0.95, 0.2)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(1,0,0,1.0)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.8f, 0.8f, 0.8f, 0.4f)); // Couleur de sélection du texte
    ImGui::PushItemWidth(inputWidth);
    ImGui::InputText("##PlanetNameInput", planetName, sizeof(planetName));
    ImGui::PopItemWidth();
    ImGui::PopStyleColor(3);
    ImGui::PopFont();

    if (planetName[0] == '\0') {
        strcpy(planetName, m_manager->newCreatedObject->getName().c_str());
    }
}



void ParametersCreator::draw_input_weight(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImGui::PushFont(font);
    ImVec2 textSize = ImGui::CalcTextSize(planetName, NULL, false, 0.0f);
    float offsetX = winWidth * 0.14;
    float offsetY = winHeight * (0.35+0.05) - textSize.y/2;
    float pourcentageX = 0.8;
    ImVec2 cursorPos = ImVec2(centerPos.x*pourcentageX, offsetY);
    float inputWidth = winWidth * 0.14/2*0.95+centerPos.x*(1-pourcentageX); // Add some padding
    ImGui::SetCursorPos(cursorPos);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95, 1, 0.95, 0.2)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.95, 1, 0.95, 0.2)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.8f, 0.8f, 0.8f, 0.4f)); // Couleur de sélection du texte
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // Elément sélectionné
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.7f)); // Survol
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 1.0f, 0.6f, 1.0f)); // Sélectionné
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Texte
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Ajustez ces valeurs selon vos besoins
    const char* buffer = "1000.0x";
    // Utilisez CalcTextSize pour calculer la longueur du texte
    float textSize2 = ImGui::CalcTextSize(buffer, NULL, false, 0.0f).x*1.12;
    ImGui::PushItemWidth(textSize2);
    ImGui::InputFloat("##Planete Weight", &massMultiplicator, 0.0f, 0.0f, "%.1fx");
    float space = winWidth*0.001;
    cursorPos = ImVec2(centerPos.x*pourcentageX+textSize2+space, offsetY);
    ImGui::SetCursorPos(cursorPos);
    ImGui::PushItemWidth(winWidth * 0.14/2*0.95+centerPos.x*(1-pourcentageX)-textSize2-space);
    if (ImGui::BeginCombo("##Choose mass", currentItemWeight.c_str(),ImGuiComboFlags_NoArrowButton)) { // currentItemWeight est une std::string pour stocker l'élément sélectionné
        for (int i = 0; i < itemNamesWeight.size(); i++) {
            bool isSelected = (currentItemWeight == itemNamesWeight[i]);
            if (ImGui::Selectable(itemNamesWeight[i].c_str(), isSelected)) {
                currentItemWeight = itemNamesWeight[i];
                // Vous pouvez ici mettre à jour d'autres variables ou états en fonction de l'élément sélectionné
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(7);
    ImGui::PopFont();
}

void ParametersCreator::draw_input_radius(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImGui::PushFont(font);
    ImVec2 textSize = ImGui::CalcTextSize(planetName, NULL, false, 0.0f);
    float offsetX = winWidth * 0.14;
    float offsetY = winHeight * (0.35+0.1) - textSize.y/2;
    float pourcentageX = 0.8;
    ImVec2 cursorPos = ImVec2(centerPos.x*pourcentageX, offsetY);
    float inputWidth = winWidth * 0.14/2*0.95+centerPos.x*(1-pourcentageX); // Add some padding
    ImGui::SetCursorPos(cursorPos);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95, 1, 0.95, 0.2)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.95, 1, 0.95, 0.2)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.8f, 0.8f, 0.8f, 0.4f)); // Couleur de sélection du texte
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // Elément sélectionné
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.7f)); // Survol
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 1.0f, 0.6f, 1.0f)); // Sélectionné
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Texte
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Ajustez ces valeurs selon vos besoins
    const char* buffer = "1000.0x";
    // Utilisez CalcTextSize pour calculer la longueur du texte
    float textSize2 = ImGui::CalcTextSize(buffer, NULL, false, 0.0f).x*1.12;
    ImGui::PushItemWidth(textSize2);
    ImGui::InputFloat("##Planete Radius", &radiusMultiplicator, 0.0f, 0.0f, "%.2f  x");
    float space = winWidth*0.001;
    cursorPos = ImVec2(centerPos.x*pourcentageX+textSize2+space, offsetY);
    ImGui::SetCursorPos(cursorPos);
    ImGui::PushItemWidth(winWidth * 0.14/2*0.95+centerPos.x*(1-pourcentageX)-textSize2-space);

    if (ImGui::BeginCombo("##Choose radius", currentItemRadius.c_str(),ImGuiComboFlags_NoArrowButton)) { // currentItemRadius est une std::string pour stocker l'élément sélectionné
        for (int i = 0; i < itemNamesRadius.size(); i++) {
            bool isSelected = (currentItemRadius == itemNamesRadius[i]);
            if (ImGui::Selectable(itemNamesRadius[i].c_str(), isSelected)) {
                currentItemRadius = itemNamesRadius[i];
                // Vous pouvez ici mettre à jour d'autres variables ou états en fonction de l'élément sélectionné
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(7);
    ImGui::PopFont();
}
void ParametersCreator::draw_input_sideral(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImGui::PushFont(font);
    //calclation size of font
    ImVec2 textSize = ImGui::CalcTextSize(planetName, NULL, false, 0.0f);
    float offsetX = winWidth * 0.14;
    float offsetY = winHeight * 0.5 - textSize.y/2;
    float pourcentageX = 0.8;
    ImVec2 cursorPos = ImVec2(centerPos.x*pourcentageX, offsetY);
    // Calculate the width of the input background based on the text length
    float textWidth = ImGui::CalcTextSize(planetName).x;
    float inputWidth = winWidth * 0.14/2*0.95+centerPos.x*(1-pourcentageX); // Add some padding

    ImGui::SetCursorPos(cursorPos);
    // Draw the editable text
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95, 1, 0.95, 0.2)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(1,0,0,1.0)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.8f, 0.8f, 0.8f, 0.4f)); // Couleur de sélection du texte
    ImGui::PushItemWidth(inputWidth);
    ImGui::InputFloat("##Rotation", &sideralMultiplicator, 0.0f, 0.0f, "%.2f Per day");

    ImGui::PopStyleColor(3);
    ImGui::PopFont();

}


void ParametersCreator::forceClose(){
    m_manager->Exit();
}