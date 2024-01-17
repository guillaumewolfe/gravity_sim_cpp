#include "glad/glad.h"
#include "UI/ImageButton.h"
#include "opencv2/opencv.hpp"

ImageButton::ImageButton(float xPercent, float yPercent, ImVec2 sizePercent, float imageScalePercent, ImVec4 color, ImVec4 hoverColor, const std::string& imagePath, float alpha, std::function<void()> onClickAction, float cornerRadius, bool isRound, ImVec4 borderColor, bool isContinuedClick)
    : position(xPercent, yPercent), sizePercent(sizePercent),imageScale(imageScalePercent), color(color), hoverColor(hoverColor), alpha(alpha), onClick(onClickAction), cornerRadius(cornerRadius), isRound(isRound), isContinuedClick(isContinuedClick)  {
    if (!InitSoundEffects()) {
        std::cout<<"Failed to load sound at init"<<std::endl;
    }
    loadTexture(imagePath.c_str());
    if (!textureNormal && !textureHover && !textureClicked) {
        std::cerr << "Erreur lors du chargement de la texture." << std::endl;
    }
}

ImageButton::~ImageButton() {
    // Free sound effects if they were loaded
    if (hoverSound) {
        Mix_FreeChunk(hoverSound);
        hoverSound = nullptr;
    }
    if (clickSound) {
        Mix_FreeChunk(clickSound);
        clickSound = nullptr;
    }
}

void ImageButton::Draw() {
if(hidden){return;}
    // Get window dimensions
    int winWidth, winHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);

    // Calculate actual size based on window dimensions and percentage values
    ImVec2 actualSize = ImVec2(sizePercent.x * winWidth, sizePercent.y * winHeight);

    // Calculate the position based on the logical window size
    ImVec2 actualPos = ImVec2((position.x * winWidth) - (actualSize.x * 0.5),
                                (position.y * winHeight) - (actualSize.y * 0.5));

    // Set the cursor position for ImGui
    ImGui::SetCursorPos(actualPos);
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();

    // Get the draw list from ImGui
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Check if the button is hovered
    ImVec2 center = ImVec2(cursorPos.x + actualSize.x * 0.5, cursorPos.y + actualSize.y * 0.5);
    float radius = (actualSize.x < actualSize.y ? actualSize.x : actualSize.y) * 0.5;

    bool isHovered;
    if (isRound) {
        // Check hover for a circle
        ImVec2 mousePos = ImGui::GetMousePos();
        isHovered = (mousePos.x - center.x) * (mousePos.x - center.x) +
                    (mousePos.y - center.y) * (mousePos.y - center.y) <= radius * radius;
    } else {
        // Check hover for a rectangle
        isHovered = ImGui::IsMouseHoveringRect(cursorPos, ImVec2(cursorPos.x + actualSize.x, cursorPos.y + actualSize.y));
    }

    // Hover sound logic
    if (isHovered && !hoverSoundPlayed&&enabled) {
        Mix_PlayChannel(-1, hoverSound, 0);
        hoverSoundPlayed = true;
    } else if (!isHovered && hoverSoundPlayed) {
        hoverSoundPlayed = false;
    }

    // Determine button shape
    if (isRound) {
        // Draw a circle if isRound is true
        ImVec2 center = ImVec2(cursorPos.x + actualSize.x * 0.5, cursorPos.y + actualSize.y * 0.5);
        float radius = (actualSize.x < actualSize.y ? actualSize.x : actualSize.y) * 0.5;
        drawList->AddCircleFilled(center, radius,
                                    IM_COL32(color.x * 255, color.y * 255, color.z * 255, alpha * 255));
    } else {
        // Draw a rectangle if isRound is false
        drawList->AddRectFilled(cursorPos,
                                ImVec2(cursorPos.x + actualSize.x, cursorPos.y + actualSize.y),
                                IM_COL32(color.x * 255, color.y * 255, color.z * 255, alpha * 255), cornerRadius);

    }
        float minDimension = std::min(actualSize.x, actualSize.y);
        ImVec2 imageSize = ImVec2(minDimension * imageScale, minDimension * imageScale);

        // Centrez l'image dans le bouton
        ImVec2 imagePos = ImVec2(actualPos.x + (actualSize.x - imageSize.x) * 0.5f, 
                                 actualPos.y + (actualSize.y - imageSize.y) * 0.5f);


    // Button click detection
    if (isHovered && ImGui::IsMouseClicked(0)) {
        mouseButtonPressed = true;
    }
    if (!isHovered && ImGui::IsMouseReleased(0) && mouseButtonPressed) {
        mouseButtonPressed = false;
    }

    if (isContinuedClick) {
        if (isHovered && ImGui::IsMouseDown(0)) {
            // Si le bouton est survolé et que le bouton de la souris est enfoncé
            if (onClick && enabled) {
                onClick(); // Appel répété de la fonction onClick
            }
            // Ajoutez ici la logique d'affichage pour le clic maintenu, si nécessaire
        }
    } else {
        // Logique existante pour le mode non continu
        if (isHovered && ImGui::IsMouseReleased(0) && mouseButtonPressed) {
            if (onClick && enabled) {
                Mix_PlayChannel(-1, clickSound, 0);
                onClick(); // Invocation de l'action associée au bouton
            }
            mouseButtonPressed = false; // Réinitialiser le statut du bouton pressé
            
        }
    }
    if(isOn){
    float rectHeight = actualSize.y * 0.075; // 10% de la hauteur du bouton
    float rectWidth = actualSize.x * imageScale;// 10% de la largeur du bouton
    float offsetX = (actualSize.x - rectWidth) / 2; // Demi-différence entre les largeurs du bouton et du rectangle
    ImVec2 rectTopLeft = ImVec2(actualPos.x + offsetX, actualPos.y + actualSize.y - rectHeight);
    ImVec2 rectBottomRight = ImVec2(rectTopLeft.x + rectWidth, actualPos.y + actualSize.y);
    drawList->AddRectFilled(rectTopLeft, rectBottomRight, IM_COL32(255, 255, 255, 255),cornerRadius);
    drawList->AddRectFilled(cursorPos,
                            ImVec2(cursorPos.x + actualSize.x, cursorPos.y + actualSize.y),
                            IM_COL32(255, 255, 255, 10), cornerRadius);}
    GLuint texture;
    if(isOn){
        texture = textureClicked;}
    else if(isHovered && ImGui::IsMouseDown(0)){
        texture = textureClicked;
    }else if(isHovered){
        texture = textureHover;
    }else{
        texture = textureNormal;
    }
    // Set cursor position for image
    ImGui::SetCursorPos(imagePos);
    // Draw image with selected texture
    ImGui::Image((void*)(intptr_t)texture, imageSize);
}


void ImageButton::loadTexture(const char* filename) {
    cv::Mat image = cv::imread(filename, cv::IMREAD_UNCHANGED);
    if (image.empty()) {
        std::cerr << "Erreur: Image non trouvée " << filename << std::endl;
        return;
    }
    // Création des trois textures
    textureNormal = createTextureWithTint(image, 0.5); // Gris
    textureHover = createTextureWithTint(image, 0.75);  // Moyen Gris
    textureClicked = createTextureWithTint(image, 1.0); // Blanc
}

GLuint ImageButton::createTextureWithTint(const cv::Mat& originalImage, float tint) {
    cv::Mat tintedImage = originalImage.clone();

    if (tintedImage.channels() == 4) {
        for (int y = 0; y < tintedImage.rows; ++y) {
            for (int x = 0; x < tintedImage.cols; ++x) {
                cv::Vec4b& pixel = tintedImage.at<cv::Vec4b>(y, x);
                // Appliquer le niveau de gris tout en conservant l'alpha
                pixel[0] = pixel[1] = pixel[2] = static_cast<unsigned char>(255 * tint);
            }
        }
    }

    // Création et configuration de la texture OpenGL
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    GLenum format = tintedImage.channels() == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, tintedImage.cols, tintedImage.rows, 0, format, GL_UNSIGNED_BYTE, tintedImage.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}




void ImageButton::changeColor(ImVec4 newColor){
    color = newColor;
}

// Initialize sound effects
bool ImageButton::InitSoundEffects() {
    // Open audio with desired parameters
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        std::cerr << "SDL_mixer could not initialize audio: " << Mix_GetError() << std::endl;
        return false;
    }

    // Load sound effects (replace with your sound file paths)
    hoverSound = Mix_LoadWAV("../assets/sounds/select.wav");
    clickSound = Mix_LoadWAV("../assets/sounds/delete.wav");

    if (!hoverSound  || !clickSound ) {
        std::cerr << "Failed to load sound effects: " << Mix_GetError() << std::endl;
        Mix_CloseAudio(); // Close audio before returning
        return false;
    }

    return true; // Sound effects initialized successfully
}

void ImageButton::UpdatePosition(float xPerc, float yPerc){
    position = ImVec2(xPerc,yPerc);
}

void ImageButton::turnOff(){
    isOn = false;
}

void ImageButton::turnOn(){
    isOn = true;
}

ImVec2 ImageButton::getPosition(){
    return position;
}

