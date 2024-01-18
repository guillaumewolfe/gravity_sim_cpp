#ifndef PATH_UTIL_H
#define PATH_UTIL_H

#include <string>
#include <algorithm>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #include <windows.h>
#elif __APPLE__
    #include <mach-o/dyld.h>
#elif __linux__
    #include <unistd.h>
    #include <limits.h>
#endif

inline std::string getExecutablePath() {
    char buffer[1024] = {0};

    // Obtenez le chemin complet de l'exécutable
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        GetModuleFileName(NULL, buffer, sizeof(buffer));
    #elif __APPLE__
        uint32_t size = sizeof(buffer);
        if (_NSGetExecutablePath(buffer, &size) != 0) {
            buffer[0] = '\0'; // Buffer trop petit
        } else {
            // Résolvez le lien symbolique de l'exécutable
            char realPath[PATH_MAX];
            if (realpath(buffer, realPath)) {
                strcpy(buffer, realPath); // Copiez le vrai chemin dans buffer
            }
        }
    #elif __linux__
        ssize_t count = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (count != -1) {
            buffer[count] = '\0';
        }
    #endif

    std::string path(buffer);

    // Modifier le chemin pour remonter de deux niveaux dans la hiérarchie des dossiers
    size_t found = path.rfind("/build");
    if (found != std::string::npos) {
        path = path.substr(0, found);
    }

    return path;
}

inline std::string getFullPath(const std::string& relativePath) {
    std::string executablePath = getExecutablePath();
    std::string modifiedPath = relativePath;

    // Check if the relativePath starts with "../" and replace it with "/"
    if (modifiedPath.rfind("../", 0) == 0) {
        modifiedPath.replace(0, 3, "/");
    }

    return executablePath + modifiedPath;
}


#endif // PATH_UTIL_H
