#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/CreationTools/ParametersCreator.h"
#include "engine/CreationTools/CreatorManager.h"
#include "engine/Vec3.h"
#include <string>       // Pour std::string
#include <fstream>      // Pour std::ifstream
#include <sstream>      // Pour std::stringstream
#include <iostream> 



ParametersCreator::ParametersCreator(RenderContext* renderContext, CreatorManager* manager) : StateCreator(renderContext, manager) {
    generate_buttons();
    generate_labels();
    initDictionnaries();
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
    strcpy(planetName, m_manager->newCreatedObject->getName().c_str());
    getItemInitialParameters();
} 


void ParametersCreator::Draw(){
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    std::cout<<winWidth<<std::endl;
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
   Button *ReturnButton = new Button(0.0725f, 0.675, ImVec2(0.05, 0.045),
                                ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                                ImVec4(0.0f, 0.0f, 0.0f, 0.0f),
                               
                               "Previous", 0.00f,18.0f,
                               std::bind(&ParametersCreator::previous_state, this));  
   Button *NextButton = new Button(0.125f, 0.675, ImVec2(0.05, 0.04),
                                ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                                ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                               "Done", 0.0f,23.0f,
                               std::bind(&ParametersCreator::next_state, this));  


    buttons.push_back(ReturnButton);
    buttons.push_back(NextButton);
    buttons[1]->UpdateLabelColor(100,255,150,200);
    buttons[0]->UpdateLabelColor(255,125,100,200);

}

void ParametersCreator::generate_sliders(){
}



void ParametersCreator::generate_labels(){
    float space = ImGui::GetTextLineHeightWithSpacing();
    Labbel *MessageLabel = new Labbel(0.095f,0.135f,ImVec4(255,255,255,255),
                            "Choose Parameters",22.0f,0.9f);
    Labbel *NameLabel = new Labbel(0.075f-0.030, 0.20f,ImVec4(255,255,255,255),
                            "Name",18.0f,0.7f);
    Labbel *MassLabel = new Labbel(0.095f, 0.300f,ImVec4(255,255,255,255),
                            "Mass",18.0f,0.7f);    
    Labbel *RadiusLabel = new Labbel(0.095f, 0.425f,ImVec4(255,255,255,255),
                            "Radius",18.0f,0.7f);    
    Labbel *SideralLabel = new Labbel(0.095f, 0.545f,ImVec4(255,255,255,255),
                            "Rotation",18.0f,0.7f);    
    Labbel *PerDayLabel = new Labbel(0.125, 0.575f,ImVec4(255,255,255,255),
                            "per day",18.0f,0.7f);    



    labbels.push_back(MessageLabel);    
    labbels.push_back(NameLabel);
    labbels.push_back(MassLabel);
    labbels.push_back(RadiusLabel);
    labbels.push_back(SideralLabel);
    labbels.push_back(PerDayLabel);

}
void ParametersCreator::drawBackground(){

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 2.0f;


    float longueur = winWidth * 0.18; // Exemple de taille
    float hauteur = winHeight * 0.60; // Exemple de taille

    centerPos = ImVec2(winWidth * 0.095f, winHeight * 0.4f);
    topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);
    bottomRight = ImVec2(topLeft.x + longueur, topLeft.y + hauteur);



    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(7.5, 7.5, 7.5, 255), // Couleur
                            cornerRadius);

    float cornerRadiusAdjustment = 10.0f;
    drawList->AddRect(topLeft,
                        ImVec2(topLeft.x + longueur, topLeft.y + hauteur),
                        IM_COL32(50, 50, 50, 150), // Couleur de remplissage
                        cornerRadius,0,3.0f); // Ajustez le rayon ici

}

void ParametersCreator::draw_buttons(){
        for (Button* btn : buttons) {
        btn->Draw();
    }
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
    m_manager->Exit();
}
void ParametersCreator::previous_state(){
    m_manager->ChangeState("VelocityCreator");
}

void ParametersCreator::Exit(){

} 

void ParametersCreator::setParameters(){
    //Name
    if(planetName[0] != '\0')
        {m_manager->newCreatedObject->setName(planetName);}
    
    //Mass
    double objectMass = itemsWeightDictionary[currentItemWeight] * massMultiplicator;
    m_manager->newCreatedObject->setWeight(objectMass);

    //Radius
    double objectRadius = itemsRadiusDictionary[currentItemRadius] * radiusMultiplicator;
    m_manager->newCreatedObject->real_radius=objectRadius;
    m_renderContext->systemeSolaire->setRayon(m_manager->newCreatedObject);
    m_renderContext->currentCamera->newFollowObject(m_manager->newCreatedObject); //Reset cam with new object.

    //Sideral speed
    m_manager->newCreatedObject->rotationSidSpeed = sideralMultiplicator/(24*60*60);
}






void ParametersCreator::draw_input_name(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float longueurBox = winWidth*0.06;
    float offsetX = winWidth * 0.02 ;
    float offsetY = winHeight * -0.21;
    ImVec2 cursorPos = ImVec2(centerPos.x-longueurBox/2+offsetX, centerPos.y + offsetY);

    // Calculer la taille du rectangle
    float rectWidth = longueurBox * 1.2;
    float rectHeight = 1.5 * ImGui::GetTextLineHeightWithSpacing();

    // Calculer la position du rectangle pour le centrer
    ImVec2 rect_pos = ImVec2(cursorPos.x - (rectWidth - longueurBox) / 2, cursorPos.y - (rectHeight - ImGui::GetTextLineHeightWithSpacing()) / 2);

    // Taille du rectangle
    ImVec2 rect_size = ImVec2(rectWidth, rectHeight);

    //drawList->AddRect(rect_pos, ImVec2(rect_pos.x + rect_size.x, rect_pos.y + rect_size.y), IM_COL32(128, 128, 128, 255),3,0,2); // Modifier la couleur ici

    ImGui::SetCursorPos(cursorPos);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.3f, 0.3f, 0.3f, 0.0f)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.8f, 0.8f, 0.8f, 1.0f)); // Couleur de sélection du texte
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.0f, 1.0f));
    ImGui::SetNextItemWidth(longueurBox);
    ImGui::InputText("##Planete Name", planetName, sizeof(planetName));
    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar();

    if(planetName[0] == '\0'){strcpy(planetName, m_manager->newCreatedObject->getName().c_str());}//Si C'est vide, remet le nom de la planète
}

void ParametersCreator::draw_input_weight(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float longueurBox = winWidth*0.1;
    float offsetX = winWidth * 0.025 ;
    float offsetY = -winHeight * 0.085f;
    ImVec2 cursorPos = ImVec2(centerPos.x-longueurBox/2+offsetX, centerPos.y + offsetY);
    ImGui::SetCursorPos(cursorPos);
    ImGui::SetNextItemWidth(longueurBox);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.3f, 0.3f, 0.3f, 0.0f)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(1,1,1,0.5)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Fond
    ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.8f, 0.8f, 0.8f, 1.0f)); // Couleur de sélection du texte
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // Elément sélectionné
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.7f)); // Survol
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 1.0f, 0.6f, 1.0f)); // Sélectionné
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Texte
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Ajustez ces valeurs selon vos besoins
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
    ImGui::SetCursorPos(ImVec2(cursorPos.x-0.04*winWidth, cursorPos.y));
    ImGui::SetNextItemWidth(longueurBox/2);
    ImGui::InputFloat("##Planete Weight", &massMultiplicator, 0.0f, 0.0f, "%.1f  x");

    ImGui::PopStyleColor(8);
}

void ParametersCreator::draw_input_radius(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float longueurBox = winWidth*0.1;
    float offsetX = winWidth * 0.025 ;
    float offsetY = winHeight * 0.04f;
    ImVec2 cursorPos = ImVec2(centerPos.x-longueurBox/2+offsetX, centerPos.y + offsetY);
    ImGui::SetCursorPos(cursorPos);
    ImGui::SetNextItemWidth(longueurBox);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.3f, 0.3f, 0.3f, 0.0f)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(1,1,1,0.5)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Fond
    ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.8f, 0.8f, 0.8f, 1.0f)); // Couleur de sélection du texte
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // Elément sélectionné
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.7f)); // Survol
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 1.0f, 0.6f, 1.0f)); // Sélectionné
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Texte
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Ajustez ces valeurs selon vos besoins
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
    ImGui::SetCursorPos(ImVec2(cursorPos.x-0.04*winWidth, cursorPos.y));
    ImGui::SetNextItemWidth(longueurBox/2);
    ImGui::InputFloat("##Planete Radius", &radiusMultiplicator, 0.0f, 0.0f, "%.1f  x");
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(8);
}
void ParametersCreator::draw_input_sideral(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float longueurBox = winWidth*0.1;
    float offsetX = winWidth * 0.05 ;
    float offsetY = winHeight * 0.163f;
    ImVec2 cursorPos = ImVec2(centerPos.x-longueurBox/2+offsetX, centerPos.y + offsetY);
    ImGui::SetCursorPos(cursorPos);
    ImGui::SetNextItemWidth(longueurBox);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.3f, 0.3f, 0.3f, 0.0f)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(1,1,1,0.5)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Fond
    ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.8f, 0.8f, 0.8f, 1.0f)); // Couleur de sélection du texte
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // Elément sélectionné
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.7f)); // Survol
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 1.0f, 0.6f, 1.0f)); // Sélectionné
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Texte
    ImGui::SetCursorPos(ImVec2(cursorPos.x-0.04*winWidth, cursorPos.y));
    ImGui::InputFloat("##Rotation", &sideralMultiplicator, 0.0f, 0.0f, "%.2f");

    ImGui::PopStyleColor(8);


}


