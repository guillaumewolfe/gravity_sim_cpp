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

private:
    ImVec2 position;        // Position en pourcentage de la largeur et hauteur de la fenêtre
    ImVec2 sizePercent;     // Taille en pourcentage de la largeur et hauteur de la fenêtre
    ImVec4 color;           // Couleur du fond lorsque le bouton est désactivé  // Couleur du cercle et du bord lorsque le bouton est activé/hover
    ImVec4 activeColor;     // Couleur du fond lorsque le bouton est activé
    bool* value;            // Pointeur vers la variable booléenne contrôlée par le bouton
    float cornerRadius;     // Rayon des coins arrondis du rectangle
};

#endif // TOGGLE_BUTTON_H
