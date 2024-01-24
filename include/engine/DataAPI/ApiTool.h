#ifndef APITOOL_H
#define APITOOL_H

#include "iostream"
#include <curl/curl.h>
#include <string>
#include <regex>
#include "engine/Vec3.h"
#include <map>
#include <utility>

class ApiTool {
public:
    // Constructor
    ApiTool();

    // Destructor
    ~ApiTool();
    bool connectionSuccess = false;

    // Function to get Mars data
    std::pair<Vec3, Vec3> extractBodyData(const std::string& data, const std::string& bodyName);
    std::string getBodyData(const std::string& bodyName);
    void printCurrentDataForManualEntry();
    bool testConnection();
private:
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *s);
    CURL *curl = nullptr;
    std::string getCurrentDateUTC();
    std::string encodeURL(const std::string &value);
    std::map<std::string, std::pair<Vec3, Vec3>> defaultData; // Données par défaut
    void initializeDefaultData(); //En cas de manque de connection internet
    std::pair<Vec3, Vec3> getDefaultBodyData(const std::string& bodyName);







    std::map<std::string, std::string> bodyIds = {
    {"Mercury", "199"},
    {"Venus", "299"},
    {"Earth", "399"},
    {"Mars", "499"},
    {"Jupiter", "599"},
    {"Saturn", "699"},
    {"Uranus", "799"},
    {"Neptune", "899"},
    {"Sun", "10"}, // Remplacement de Pluto par le Soleil
    {"Moon", "301"},
    // Ajouter d'autres corps ici si nécessaire
};

};

#endif // APITOOL_H