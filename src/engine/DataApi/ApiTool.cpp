#include "engine/DataAPI/ApiTool.h"
#include <chrono>
#include <sstream>


ApiTool::ApiTool() {
    curl = curl_easy_init();
    std::string bodyData = getBodyData("Sun");
    std::pair<Vec3, Vec3> marsPositionAndVelocity = extractBodyData(bodyData);
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
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        return " ";
    }
    return readBuffer;
}
Vec3 rotateToXZPlane(const Vec3& original) {
    // Rotation de 90 degrés autour de l'axe X
    double newY = original.z;
    double newZ = -original.y;
    return Vec3(original.x, newY, newZ);
}
std::pair<Vec3, Vec3> ApiTool::extractBodyData(const std::string& data) {
    std::regex regex_pattern("\\$\\$SOE[\\s\\S]*?(-?\\d+\\.\\d+E[+-]\\d+)[\\s\\S]*?(-?\\d+\\.\\d+E[+-]\\d+)[\\s\\S]*?(-?\\d+\\.\\d+E[+-]\\d+)[\\s\\S]*?(-?\\d+\\.\\d+E[+-]\\d+)[\\s\\S]*?(-?\\d+\\.\\d+E[+-]\\d+)[\\s\\S]*?(-?\\d+\\.\\d+E[+-]\\d+)");
    std::smatch matches;

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