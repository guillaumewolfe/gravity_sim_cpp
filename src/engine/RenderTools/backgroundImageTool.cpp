#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/RenderTools/BackgroundImageTool.h"
#include "path_util.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <GLFW/glfw3.h>


BackgroundImageTool::BackgroundImageTool(RenderContext* renderContext) : RenderComponent(renderContext){
    loadImage();
    m_camera = renderContext->currentCamera;
}

// Desctructeur
BackgroundImageTool::~BackgroundImageTool() {
}



void BackgroundImageTool::Draw() {
glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    // Get the camera position
    Vec3 position_cam = m_camera->getPosition();

    // Ensure the nebula texture is bound
    GLuint nebulaTextureID = textureID;
    glBindTexture(GL_TEXTURE_2D, nebulaTextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Changed to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Changed to GL_REPEAT
    glEnable(GL_TEXTURE_2D);

    // No need to adjust for window size since we're making the sphere surround the camera
    float scaledRadius = 1500; // Adjust as necessary for correct size

    // Setup transformations
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    m_camera->lookAt();

    // Translate the sphere to be centered around the camera position
    glTranslatef(position_cam.x, position_cam.y, position_cam.z);

    // Optional: Adjust these rotations based on camera orientation if needed
    glRotatef(70, 0, 1, 0); // Adjust as necessary for correct orientation
    glRotatef(90, 1, 0, 0); // Adjust as necessary for correct orientation
    // Sphere drawing logic (unchanged, ensure it uses scaledRadius for size)
    const float PI = 3.14159265358979323846f;
    const float PI_2 = PI * 2.0f;
    int numSlices = 40;
    int numSegments = 40;

for (int i = 0; i < numSlices; ++i) {
    float phi1 = (float)i / (float)numSlices * PI;
    float phi2 = (float)(i + 1) / (float)numSlices * PI;

    glBegin(GL_QUAD_STRIP);
    for (int j = 0; j <= numSegments; ++j) {
        float theta = (float)j / (float)numSegments * PI_2;

        float x1 = scaledRadius * sinf(phi1) * cosf(theta);
        float y1 = scaledRadius * sinf(phi1) * sinf(theta);
        float z1 = scaledRadius * cosf(phi1);

        float x2 = scaledRadius * sinf(phi2) * cosf(theta);
        float y2 = scaledRadius * sinf(phi2) * sinf(theta);
        float z2 = scaledRadius * cosf(phi2);

        float u1 = (float)j / (float)numSegments;
        float v1 = (float)i / (float)numSlices;
        float u2 = (float)j / (float)numSegments;
        float v2 = (float)(i + 1) / (float)numSlices;

        float textureRepeatFactor = 2.0f; // Adjust based on desired repetition
        float verticalShift = 0.25f; // Adjust this value to shift the texture vertically

        // Apply the vertical shift before repeating the texture
        v1 = (v1 + verticalShift) * textureRepeatFactor;
        v2 = (v2 + verticalShift) * textureRepeatFactor;

        glTexCoord2f(u1 * textureRepeatFactor, v1);
        glVertex3f(x1, y1, z1);
        glTexCoord2f(u2 * textureRepeatFactor, v2);
        glVertex3f(x2, y2, z2);
    }
    glEnd();
}


    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void BackgroundImageTool::loadImage(){
    const char* filename = "assets/textures/background.jpg";
    std::string fileNameTransformed = getFullPath(filename);
    cv::Mat image = cv::imread(fileNameTransformed);
    if (image.empty()) {
        std::cerr << "Erreur: Image non trouvée " << filename << std::endl;
        return;
    }

    // Redimensionnement de l'image
    cv::Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(), 4.0, 4.0, cv::INTER_LINEAR);

    cv::flip(resizedImage, resizedImage, 0); // Inversion verticale de l'image

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resizedImage.cols, resizedImage.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, resizedImage.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}