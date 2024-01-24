#ifndef PATH_UTIL_H
#define PATH_UTIL_H

#include <iostream>
#include <string>
#include <algorithm>
#include <filesystem>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #include <windows.h>
    #define PATH_SEPARATOR '\\'
#else
    #define PATH_SEPARATOR '/'
#endif

namespace fs = std::filesystem;

inline std::string getExecutablePath() {
    char buffer[4096] = {0}; 

    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        if (GetModuleFileName(NULL, buffer, sizeof(buffer)) == 0) {
            std::cerr << "Erreur GetModuleFileName: " << GetLastError() << std::endl;
            buffer[0] = '\0';
        }
    #elif __APPLE__
        uint32_t size = sizeof(buffer);
        if (_NSGetExecutablePath(buffer, &size) != 0) {
            std::cerr << "Erreur _NSGetExecutablePath: buffer trop petit" << std::endl;
            buffer[0] = '\0';
        } else {
            char realPath[PATH_MAX];
            if (!realpath(buffer, realPath)) {
                std::cerr << "Erreur realpath" << std::endl;
                strcpy(buffer, realPath);
            }
        }
    #elif __linux__
        ssize_t count = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (count == -1) {
            std::cerr << "Erreur readlink: " << errno << std::endl;
            buffer[0] = '\0';
        } else {
            buffer[count] = '\0';
        }
    #endif

    std::string path(buffer);
    std::replace(path.begin(), path.end(), '/', PATH_SEPARATOR);
    std::replace(path.begin(), path.end(), '\\', PATH_SEPARATOR);

    return path;
}

inline std::string getFullPath(const std::string& relativePath) {
    std::string executablePath = getExecutablePath();

    fs::path execPath(executablePath);

    // Remonte de deux niveaux dans l'arborescence des dossiers
    execPath = execPath.parent_path().parent_path();

    // Normalisation du chemin relatif
    std::string normalizedRelativePath = relativePath;
    if (relativePath.find("../") == 0) {
        normalizedRelativePath = relativePath.substr(3);
    } else if (relativePath.find("/") == 0 || relativePath.find("\\") == 0) {
        normalizedRelativePath = relativePath.substr(1);
    }

    // Construction du chemin complet
    fs::path fullPath = execPath / normalizedRelativePath;

    std::string fullPathStr = fullPath.string();
    std::replace(fullPathStr.begin(), fullPathStr.end(), '/', PATH_SEPARATOR);
    std::replace(fullPathStr.begin(), fullPathStr.end(), '\\', PATH_SEPARATOR);

    return fullPathStr;
}


#endif // PATH_UTIL_H