#ifndef TOGGLE_BUTTON_H
#define TOGGLE_BUTTON_H

#include <glm/glm.hpp>
#include <functional>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"

class ToggleButton {
public:
    // Constructeur
    ToggleButton(float xPercent, float yPercent, ImVec2 sizePercent, ImVec4 color, ImVec4 activeColor, bool* value, float cornerRadius);

    // Méthode pour dessiner le bouton
    void Draw();
    void updatePointer(bool* newPointer);
    bool isDeactivated = false;
    void setPosition(float xPercent, float yPercent);
    void setSize(float xPercent, float yPercent);
    bool isHidden = false;

private:
    ImVec2 position = ImVec2(0, 0);     
    ImVec2 sizePercent = ImVec2(1,1);     // Taille en pourcentage de la largeur et hauteur de la fenêtre
    ImVec4 color = ImVec4(255,255,255,255);           // Couleur du fond lorsque le bouton est désactivé  // Couleur du cercle et du bord lorsque le bouton est activé/hover
    ImVec4 activeColor = ImVec4(255,255,255,255);     // Couleur du fond lorsque le bouton est activé
    bool* value = nullptr;            // Pointeur vers la variable booléenne contrôlée par le bouton
    float cornerRadius = 1;     // Rayon des coins arrondis du rectangle
};

#endif // TOGGLE_BUTTON_H
