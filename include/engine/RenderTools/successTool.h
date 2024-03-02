
#ifndef SuccessTool_H
#define SuccessTool_H

#include "RenderComponent.h"
#include "engine/Vec3.h"
#include "UI/Labbel.h"
#include <functional>
#include <engine/RenderTools/notificationTool.h>   

class Success {
    public:
    Success(RenderContext* m_renderContext,int totalSteps, std::string title, std::string description, std::function<bool()> func)
    : totalSteps(totalSteps),  title(title), description(description), checkSuccessFunction(func), m_renderContext(m_renderContext) {}

    int totalSteps;
    int stepsDone = 0;
    bool isDone = false;
    std::string title;
    std::string description;
    std::function<bool()> checkSuccessFunction;
    RenderContext* m_renderContext;

    bool checkSuccess() {
        return checkSuccessFunction();
    }
};

class SuccessTool : public RenderComponent {
public:

    explicit SuccessTool(RenderContext* renderContext);
    ~SuccessTool() override;
    void Draw() override;
    void Open();
    void Close();
    void setConfirmBoxFunction(const std::function<void(std::function<void()> func, const std::string& message)>& confirmBoxFunc);

    std::vector<Success*> getSuccessList();
    int getSuccessCount();
    void setNotificationTool(NotificationTool* notificationTool);

private:

    std::vector<Success*> successList;
    NotificationTool* notificationTool;
    void checkSuccess();
    void initSuccess();



    //For success:
    std::vector<std::string> elementsToVerify;
};

#endif // SuccessTool_H