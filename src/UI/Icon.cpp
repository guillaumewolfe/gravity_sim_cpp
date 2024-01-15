#include "glad/glad.h"
#include "UI/Icon.h"
#include "opencv2/opencv.hpp"


Icon::Icon(float xPercent, float yPercent, ImVec2 sizePercent, float imageScalePercent, const std::string& imagePath, float alpha)
    : position(xPercent, yPercent), sizePercent(sizePercent), imageScale(imageScalePercent), alpha(alpha), imagePath(imagePath) {
    loadTexture(imagePath.c_str());
    if (!texture) {
        std::cerr << "Error loading texture." << std::endl;
    }
}

Icon::~Icon() {
}

void Icon::Draw() {
    if(!enabled) return;
    // Get window dimensions
    int winWidth, winHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);

    // Calculate actual size based on window dimensions and percentage values
    ImVec2 actualSize = ImVec2(sizePercent.x * winWidth, sizePercent.y * winHeight);

    // Ensure the icon is square by using the minimum dimension for both width and height
    float minDimension = std::min(actualSize.x, actualSize.y);
    ImVec2 squareSize = ImVec2(minDimension, minDimension);

    // Calculate the position based on the logical window size
    ImVec2 actualPos = ImVec2((position.x * winWidth) - (squareSize.x * 0.5),
                              (position.y * winHeight) - (squareSize.y * 0.5));

    // Set cursor position for image
    ImGui::SetCursorPos(actualPos);

    // Draw image with texture, ensuring it's square
    ImGui::Image((void*)(intptr_t)texture, squareSize);
}


ImVec2 Icon::getPosition() {
    return position;
}   
ImVec2 Icon::getSize() {
    return sizePercent;
}


void Icon::loadTexture(const char* filename) {
    cv::Mat image = cv::imread(filename, cv::IMREAD_UNCHANGED);
    if (image.empty()) {
        std::cerr << "Error: Image not found " << filename << std::endl;
        return;
    }

    texture = createTexture(image);
}
GLuint Icon::createTexture(const cv::Mat& originalImage) {
    // Check if the image has an alpha channel
    GLenum format;
    if (originalImage.channels() == 4) {
        format = GL_RGBA;
    } else if (originalImage.channels() == 3) {
        format = GL_RGB;
    } else {
        std::cerr << "Unsupported image format." << std::endl;
        return 0;
    }
    cv::Mat tintedImage = originalImage.clone();
    if (tintedImage.channels() == 4) {
        for (int y = 0; y < tintedImage.rows; ++y) {
            for (int x = 0; x < tintedImage.cols; ++x) {
                cv::Vec4b& pixel = tintedImage.at<cv::Vec4b>(y, x);
                // Appliquer le niveau de gris tout en conservant l'alpha
                pixel[0] = pixel[1] = pixel[2] = static_cast<unsigned char>(255 * alpha);
            }
        }
    }
    // Generate and configure OpenGL texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image data into the texture
    glTexImage2D(GL_TEXTURE_2D, 0, format, tintedImage.cols, tintedImage.rows, 0, format, GL_UNSIGNED_BYTE, tintedImage.data);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}


void Icon::UpdatePosition(float xPerc, float yPerc) {
    position = ImVec2(xPerc, yPerc);
}