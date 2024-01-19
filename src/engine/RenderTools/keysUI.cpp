
#include "glad/glad.h"
#include "engine/RenderTools/keysUI.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "UI/Buttons.h"
#include "UI/Labbel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>
#include "engine/RenderTools/soundTool.h"

KeysUI::KeysUI(RenderContext* renderContext) : RenderComponent(renderContext){
    initPosition();
    initUI();
    init_key_icons();
}

KeysUI::~KeysUI() {
    for (auto& key : keys) {
        delete key.icon; // Libérer la mémoire allouée pour l'icône
    }
}


void KeysUI::Draw() {
glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
ImGui::SetNextWindowPos(ImVec2(0, 0));
ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

draw_rect();
drawUI();
draw_key_icons();
updateAlpha();


ImGui::End(); 


}

void KeysUI::draw_rect(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 5.0f;

    // Dessinez le rectangle
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(0, 0, 0, 255), // Couleur
                            cornerRadius);
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(20, 25, 30, 100), // Couleur
                            cornerRadius);

    drawList->AddRect(topLeft, 
                        ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                        IM_COL32(255,255,255,20), // Couleur
                        cornerRadius,0,0.2f);
}

void KeysUI::initPosition(){
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    longueur = winWidth * 0.125; // Exemple de taille
    hauteur = winHeight * 0.44; // Exemple de taille

    topLeft = ImVec2(0.005*winWidth,0.15*winHeight);
    center = ImVec2(topLeft.x + longueur/2, topLeft.y + hauteur/2);
}

void KeysUI::initUI(){
    Labbel* menuLabbel = new Labbel(center.x/winWidth,(center.y-hauteur/2)*1.2/winHeight,ImVec4(255,255,255,255),
                                "Controls",25.0f,0.7f);

    labbels.push_back(menuLabbel);    

    std::function<void(std::string)> playSoundFunc = 
    std::bind(&SoundTool::playSound, m_renderContext->soundTool, std::placeholders::_1);
    ImageButton* hide = new ImageButton(playSoundFunc,(topLeft.x+longueur*0.90)/winWidth,topLeft.y*1.1/winHeight,ImVec2(0.030f,0.030f),0.7f,
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f), ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                        "../assets/button/hide.png", 0,
                            std::bind(&KeysUI::hideButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    imageButtons.push_back(hide);
}

void KeysUI::drawUI() {
    for (auto& labbel : labbels) {
        labbel->Draw();
    }
    for (auto& labbel : keyLabbels) {
        labbel->Draw();
    }

    for (auto& button : buttons) {
        button->Draw();
    }
    for (auto& imageButton : imageButtons) {
        imageButton->Draw();
    }
}

void KeysUI::init_key_icons(){
    float diff = 0.035;
    float hauteurk = (center.y-hauteur/3.25)/(winHeight);
    float longueurk = (center.x-longueur/3.2)/winWidth;
    float grandeur = 0.025;

    // Création et ajout des touches dans le vecteur 'keys'
    keys.push_back({"esc", "Options", new Icon(longueurk, hauteurk, ImVec2(grandeur, grandeur), 0.5f, "../assets/button/keys/esc.png", 0.5)});
    keys.push_back({"e", "Zoom In", new Icon(longueurk, hauteurk + diff, ImVec2(grandeur, grandeur), 0.5f, "../assets/button/keys/e.png", 0.5)});
    keys.push_back({"q", "Zoom Out", new Icon(longueurk, hauteurk + 2*diff, ImVec2(grandeur, grandeur), 0.5f, "../assets/button/keys/q.png", 0.5)});
    keys.push_back({"Space", "Play/Pause", new Icon(longueurk, hauteurk + 3*diff, ImVec2(grandeur, grandeur), 0.5f, "../assets/button/keys/space.png", 0.5)});
    keys.push_back({"R", "Reset Camera", new Icon(longueurk, hauteurk + 4*diff, ImVec2(grandeur, grandeur), 0.5f, "../assets/button/keys/r.png", 0.5)});
    keys.push_back({"Tab", "Next Object", new Icon(longueurk, hauteurk + 5*diff, ImVec2(grandeur, grandeur), 0.5f, "../assets/button/keys/tab.png", 0.5)});
    keys.push_back({"F", "Focus/Global view", new Icon(longueurk, hauteurk + 6*diff, ImVec2(grandeur, grandeur), 0.5f, "../assets/button/keys/f.png", 0.5)});
    keys.push_back({"I", "Show/Hide infos", new Icon(longueurk, hauteurk + 7*diff, ImVec2(grandeur, grandeur), 0.5f, "../assets/button/keys/i.png", 0.5)});
    keys.push_back({"wasd", "Move around", new Icon(longueurk, hauteurk + 8*diff, ImVec2(grandeur*1.5, 1.5*grandeur), 0.5f, "../assets/button/keys/wasd.png", 0.5)});
    keys.push_back({"mouse", "Select Object", new Icon(longueurk, hauteurk + 9*diff, ImVec2(grandeur, grandeur), 0.5f, "../assets/button/keys/mouse.png", 0.5)});
    for (const auto& key : keys) {
        ImVec4 labelColor = ImVec4(255, 255, 255, 255); // Couleur du label
        keyLabbels.push_back(new Labbel((center.x/winWidth)*1.25, hauteurk, labelColor, key.controlPhrase, 20.0f, 0.6f));
        hauteurk += diff; // Incrémenter la hauteur pour la prochaine clé
    }

    hauteurk = (center.y-hauteur/3.25)/(winHeight);
    for (const auto& key : keys) {
        // Création d'une nouvelle icône avec un alpha de 0.9
        // En supposant que la classe Icon a un constructeur qui prend ces paramètres
        float grandeurHigh = grandeur;
        if(key.name == "wasd"){grandeurHigh = 1.5*grandeur;} // Les touches WASD et la souris sont traitées séparément dans updateAlpha(
        Icon* hilitedIcon = new Icon(longueurk, hauteurk, ImVec2(grandeurHigh, grandeurHigh), 0.5f, key.icon->imagePath, 0.9);
        hilitedIcon->enabled = false; // Désactiver l'icône
        // Ajouter la clé avec la nouvelle icône à 'keysHilited'
        keysHighlight.push_back({key.name, key.controlPhrase, hilitedIcon});

        hauteurk += diff; // Incrémenter la hauteur pour la prochaine clé
    }
}

void KeysUI::updateAlpha() {
    // Parcourir toutes les touches dans 'keys'
    // Parcourir toutes les touches dans 'keys'
    for (size_t i = 0; i < keys.size(); ++i) {
        auto& key = keys[i];
        auto& highlightedKey = keysHighlight[i];
        bool isKeyDown = false;

        // Traitement spécial pour WASD et la souris
        if (key.name == "wasd") {
            isKeyDown = ImGui::IsKeyDown(ImGuiKey_W) || ImGui::IsKeyDown(ImGuiKey_A) || ImGui::IsKeyDown(ImGuiKey_S) || ImGui::IsKeyDown(ImGuiKey_D);
        } else if (key.name == "mouse") {
            isKeyDown = ImGui::IsMouseDown(ImGuiMouseButton_Left) || ImGui::IsMouseDown(ImGuiMouseButton_Right);
        } else {
            // Obtenir la valeur ImGuiKey correspondante à partir du nom de la touche
            ImGuiKey imguiKey = getKeyFromName(key.name);
            isKeyDown = imguiKey != ImGuiKey_None && ImGui::IsKeyDown(imguiKey);
        }

        // Mettre à jour l'alpha du label et l'état de l'icône surlignée
        if (isKeyDown) {
            if (i < keyLabbels.size()) {
                keyLabbels[i]->UpdateAlpha(0.9f);
            }
            highlightedKey.icon->enabled = true;
        } else {
            if (i < keyLabbels.size()) {
                keyLabbels[i]->UpdateAlpha(0.6f);
            }
            highlightedKey.icon->enabled = false;
        }
        drawRectForPressedKey(highlightedKey.icon, isKeyDown);
    }
}

ImGuiKey KeysUI::getKeyFromName(const std::string& keyName) {
    if (keyName == "esc") return ImGuiKey_Escape;
    else if (keyName == "e") return ImGuiKey_E;
    else if (keyName == "q") return ImGuiKey_Q;
    else if (keyName == "Space") return ImGuiKey_Space;
    else if (keyName == "R") return ImGuiKey_R;
    else if (keyName == "T") return ImGuiKey_T;
    else if (keyName == "G") return ImGuiKey_G;
    else if (keyName == "I") return ImGuiKey_I;
    else if(keyName == "Tab") return ImGuiKey_Tab;
    else if(keyName == "F") return ImGuiKey_F;
    // Les touches WASD sont traitées séparément dans updateAlpha
    // La souris est également traitée séparément dans updateAlpha
    else return ImGuiKey_None;
}

void KeysUI::drawRectForPressedKey(Icon* icon, bool isKeyDown) {
    if (!icon || !isKeyDown) return;

    float iconHalfHeight = (icon->getSize().y * winHeight) / 2; // La moitié de la hauteur de l'icône ajustée selon la hauteur de la fenêtre
    ImVec2 rectPos = ImVec2(
        (icon->getPosition().x * winWidth) - 0.02 * winWidth, 
        (icon->getPosition().y * winHeight) - iconHalfHeight
    ); // Ajustement pour aligner en haut
    ImVec2 rectSize = ImVec2(0.0020 * winWidth, (icon->getSize().y * winHeight)); // Taille du rectangle ajustée selon les dimensions de la fenêtre

    ImU32 rectColor = IM_COL32(255, 255, 255, 255*0.5); // Couleur du rectangle

    ImGui::GetWindowDrawList()->AddRectFilled(
        ImVec2(rectPos.x, rectPos.y),
        ImVec2(rectPos.x + rectSize.x, rectPos.y + rectSize.y),
        rectColor
    );
}


void KeysUI::draw_key_icons() {
    for (auto& key : keys) {
        if (key.icon != nullptr) {
            key.icon->Draw();
        }
    }
    for (auto& key : keysHighlight) {
        if (key.icon != nullptr) {
            key.icon->Draw();
        }
    }
}



void KeysUI::deactivateKeys() {
    for (auto& key : keys) {
        if (key.icon != nullptr) {
            key.icon->enabled = false;
        }
    }
}

void KeysUI::activateKey(const std::string& keyName) {
    for (auto& key : keys) {
        if (key.name == keyName && key.icon != nullptr) {
            key.icon->enabled = true;
            break; // Arrête la boucle une fois la clé trouvée et activée
        }
    }
}


void KeysUI::updateKeyPhrase(const std::string& keyName, const std::string& newControlPhrase) {
    for (auto& key : keys) {
        if (key.name == keyName) {
            key.controlPhrase = newControlPhrase;
            break; // Arrête la boucle une fois la clé trouvée et mise à jour
        }
    }
}



void KeysUI::hideButton(){
    m_renderContext->showControls = false;
}