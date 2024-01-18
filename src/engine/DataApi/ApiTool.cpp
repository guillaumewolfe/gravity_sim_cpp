#include "engine/DataAPI/ApiTool.h"
#include <chrono>
#include <sstream>


ApiTool::ApiTool() {
    curl = curl_easy_init();
    initializeDefaultData();

}

ApiTool::~ApiTool() {
    curl_easy_cleanup(curl);
}

size_t ApiTool::WriteCallback(void *contents, size_t size, size_t nmemb, std::string *s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    } catch(std::bad_alloc &e) {
        // handle memory problem
        return 0;
    }
    return newLength;
}

std::string ApiTool::getCurrentDateUTC() {
    auto now = std::chrono::system_clock::now();
    auto nowPlusOneHour = now + std::chrono::hours(1); // Ajouter une heure

    auto timeNow = std::chrono::system_clock::to_time_t(now);
    auto timeLater = std::chrono::system_clock::to_time_t(nowPlusOneHour);

    std::tm tmNow = *std::gmtime(&timeNow);
    std::tm tmLater = *std::gmtime(&timeLater);

    char bufferNow[20], bufferLater[20];
    // Modifier le format pour correspondre à l'attente de l'API Horizons
    std::strftime(bufferNow, sizeof(bufferNow), "%Y-%b-%d %H:%M", &tmNow);
    std::strftime(bufferLater, sizeof(bufferLater), "%Y-%b-%d %H:%M", &tmLater);

    return std::string(bufferNow) + "|" + std::string(bufferLater); // Retourner les deux dates séparées par une barre verticale
}

std::string ApiTool::encodeURL(const std::string &value) {
    std::ostringstream encoded;
    for (char c: value) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' || c == '{' || c == '}') {
            encoded << c;
        } else {
            encoded << std::uppercase;
            encoded << '%' << std::hex << int((unsigned char)c);
            encoded << std::nouppercase;
        }
    }
    return encoded.str();
}

std::string ApiTool::getBodyData(const std::string& bodyName) {
    // Vérifier si le corps céleste est dans la map
    auto it = bodyIds.find(bodyName);
    if (it == bodyIds.end()) {
        std::cerr << "Corps céleste non trouvé: " << bodyName << std::endl;
        return " ";
    }
    std::string bodyId = it->second;

    std::string datesUTC = getCurrentDateUTC();
    auto separatorPos = datesUTC.find('|');
    std::string currentDateUTC = encodeURL(datesUTC.substr(0, separatorPos));
    std::string nextDateUTC = encodeURL(datesUTC.substr(separatorPos + 1));
    std::string readBuffer;

    std::string url = "https://ssd.jpl.nasa.gov/api/horizons.api?format=text&COMMAND='" + bodyId + "'&OBJ_DATA='YES'&MAKE_EPHEM='YES'&EPHEM_TYPE='VECTORS'&CENTER='@0'&START_TIME='" + currentDateUTC + "'&STOP_TIME='" + nextDateUTC + "'&OUT_UNITS='KM-S'";
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            //std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return "connexionFailed";
        }
        return readBuffer;
    }
Vec3 rotateToXZPlane(const Vec3& original) {
    // Rotation de 90 degrés autour de l'axe X
    double newY = original.z;
    double newZ = -original.y;
    return Vec3(original.x, newY, newZ);
}
std::pair<Vec3, Vec3> ApiTool::extractBodyData(const std::string& data, const std::string& bodyName) {
    std::regex regex_pattern("\\$\\$SOE[\\s\\S]*?(-?\\d+\\.\\d+E[+-]\\d+)[\\s\\S]*?(-?\\d+\\.\\d+E[+-]\\d+)[\\s\\S]*?(-?\\d+\\.\\d+E[+-]\\d+)[\\s\\S]*?(-?\\d+\\.\\d+E[+-]\\d+)[\\s\\S]*?(-?\\d+\\.\\d+E[+-]\\d+)[\\s\\S]*?(-?\\d+\\.\\d+E[+-]\\d+)");
    std::smatch matches;

    if(data == "connexionFailed"){
        std::cerr << "Connexion échouée, utilisation des données par défaut" << std::endl;
        return getDefaultBodyData(bodyName);
    }   

    if (std::regex_search(data, matches, regex_pattern) && matches.size() == 7) {
        // Convertir les chaînes en doubles
        double x = std::stod(matches[1].str());
        double y = std::stod(matches[2].str());
        double z = std::stod(matches[3].str());
        Vec3 position(x, y, z);

        double vx = std::stod(matches[4].str());
        double vy = std::stod(matches[5].str());
        double vz = std::stod(matches[6].str());
        Vec3 velocity(vx, vy, vz);
        Vec3 positionRotated = rotateToXZPlane(position);
        Vec3 velocityRotated = rotateToXZPlane(velocity);
        return {positionRotated, velocityRotated};
    }

    // Retourner des vecteurs vides si les données ne sont pas trouvées
    return {Vec3(), Vec3()};

}

void ApiTool::printCurrentDataForManualEntry() {
    // Liste des corps célestes pour lesquels vous souhaitez imprimer les données
    std::vector<std::string> bodies = {"Sun", "Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune", "Moon"};

    for (const auto& bodyName : bodies) {
        std::string bodyData = getBodyData(bodyName);
        std::pair<Vec3, Vec3> positionAndVelocity = extractBodyData(bodyData, bodyName);

        // Imprimer les données
        std::cout << "defaultData[\"" << bodyName << "\"] = {Vec3(" 
                  << positionAndVelocity.first.x << ", " 
                  << positionAndVelocity.first.y << ", " 
                  << positionAndVelocity.first.z << "), Vec3(" 
                  << positionAndVelocity.second.x << ", " 
                  << positionAndVelocity.second.y << ", " 
                  << positionAndVelocity.second.z << ")};" << std::endl;
    }
}


void ApiTool::initializeDefaultData() {
    defaultData["Sun"] = {Vec3(-1.20097e+06, 31545, 421540), Vec3(0.00828328, -7.6754e-05, 0.0124155)};
    defaultData["Mercury"] = {Vec3(1.4502e+07, 2.12605e+06, -4.28335e+07), Vec3(-55.5344, 6.60501, -18.471)};
    defaultData["Venus"] = {Vec3(-1.05412e+08, 6.40214e+06, -2.56141e+07), Vec3(-8.65479, 0.030998, 34.1476)};
    defaultData["Earth"] = {Vec3(6.3667e+06, 23515.6, -1.46578e+08), Vec3(-30.2273, -0.000961813, -1.39557)};
    defaultData["Mars"] = {Vec3(-7.12028e+07, -2.70809e+06, 2.1308e+08), Vec3(23.9355, -0.702214, 5.51053)};
    defaultData["Jupiter"] = {Vec3(5.31452e+08, -1.40507e+07, -5.20804e+08), Vec3(-9.29053, 0.166541, -9.95056)};
    defaultData["Saturn"] = {Vec3(1.34118e+09, -4.35589e+07, 5.65915e+08), Vec3(3.21706, -0.282807, -8.88124)};
    defaultData["Uranus"] = {Vec3(1.84028e+09, -1.53575e+07, -2.28422e+09), Vec3(-5.35324, 0.0839541, -3.95511)};
    defaultData["Neptune"] = {Vec3(4.46294e+09, -9.7206e+07, 2.74211e+08), Vec3(0.29747, -0.119724, -5.45681)};
    defaultData["Moon"] = {Vec3(6.73529e+06, 8110.79, -1.46542e+08), Vec3(-30.0997, 0.080616, -2.44076)};
    }


std::pair<Vec3, Vec3> ApiTool::getDefaultBodyData(const std::string& bodyName) {
    auto it = defaultData.find(bodyName);
    if (it != defaultData.end()) {
        return it->second;
    }
    // Retourner une paire de Vec3 vides si le corps céleste n'est pas trouvé
    return {Vec3(), Vec3()};
}

bool ApiTool::testConnection() {
    CURL *curl = curl_easy_init();
    if(curl) {
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_URL, "http://google.com");
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1); // Nous n'avons pas besoin du corps de la réponse
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1); // Si la requête échoue (par exemple, si le serveur renvoie 404), curl_easy_perform renverra une erreur
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        return res == CURLE_OK;
    }
    return false;
}