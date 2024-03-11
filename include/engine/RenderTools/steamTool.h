
#ifndef SteamTool_H
#define SteamTool_H

#include "RenderComponent.h"
#include <chrono>


class Success;
class SteamTool : public RenderComponent {
public:

    explicit SteamTool(RenderContext* renderContext);
    ~SteamTool() override;
    void Draw() override;

    std::string getUserName();
    void newSuccess(int id);
    void removeSuccess(int id);
    void getTotalPlayTime();
    void syncSuccess(std::vector<Success *> successList);

private:
    bool isConnected;
    void updatePlayTime();
    std::chrono::steady_clock::time_point lastUpdateTime;
    const long long updateInterval = 10;



};

#endif // SteamTool_H