#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/RenderTools/SuccessTool.h"
#include "path_util.h"
#include "opencv2/opencv.hpp"
#include "engine/RenderTools/soundTool.h"

//INIT

void SuccessTool::initUI(){

    std::function<void(std::string)> playSoundFunc = 
    std::bind(&SoundTool::playSound, m_renderContext->soundTool, std::placeholders::_1);


    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    centerPos = ImVec2(winWidth * 0.5, winHeight * 0.5);
    longueur = winWidth*0.5;
    hauteur = winHeight*0.7;

    //Badge & Quest
    longueurRects = 0.825*longueur;
    hauteurRectBadge = 0.15*hauteur;
    hauteurRectQuest = 0.45*hauteur;

    centerYBagde = winHeight*0.35;
    centerYQuest = winHeight*0.65;

    topLeftBadge = ImVec2(centerPos.x - longueurRects * 0.5, centerYBagde- hauteurRectBadge * 0.5);
    bottomRightBadge = ImVec2(centerPos.x + longueurRects * 0.5, centerYBagde + hauteurRectBadge * 0.5);

    topLeftQuest = ImVec2(centerPos.x - longueurRects * 0.5, centerYQuest - hauteurRectQuest * 0.5);
    bottomRightQuest = ImVec2(centerPos.x + longueurRects * 0.5, centerYQuest + hauteurRectQuest * 0.5);



    
    float titlesPosX = topLeftBadge.x/winWidth;
    float posYBadgeTitle = (topLeftBadge.y)/winHeight-0.01f;
    float posYQuestTitle = (topLeftQuest.y)/winHeight-0.02f;
    title = new Labbel(0.5f,0.20f,ImVec4(255,255,255,255),
                            "Success",28.0f,0.8f);

    badgeTitle = new Labbel(0.5,posYBadgeTitle,ImVec4(255,255,255,255),
                            "Badges",22.0f,0.7f);
    
    questTitle = new Labbel(titlesPosX,posYQuestTitle,ImVec4(255,255,255,255),
                            "Quests",22.0f,0.7f);
    
    //badgeTitle->alignLeft = true;
    questTitle->alignLeft = true;
    float posx = (centerPos.x+(longueurRects*0.30))/winWidth;
    ImVec2 posBoutton = ImVec2(posx, posYQuestTitle);
    viewAllButton = new Button(playSoundFunc,posBoutton.x-0.035, posBoutton.y, ImVec2(0.025, 0.03),
                               ImVec4(20.0/255.0, 25.0/255.0, 30.0/255.0, 1.0f),
                               ImVec4(20.0/255.0, 25.0/255.0, 30.0/255.0,  1.0f),
                               "All", 0.0f,16.0,
                               std::bind(&SuccessTool::setShowMode, this,1),1.0); 
    viewCompletedButton = new Button(playSoundFunc,posBoutton.x, posBoutton.y, ImVec2(0.04, 0.03),
                               ImVec4(20.0/255.0, 25.0/255.0, 30.0/255.0, 1.0f),
                               ImVec4(20.0/255.0, 25.0/255.0, 30.0/255.0,  1.0f),
                               "Completed", 0.0f,16.0,
                               std::bind(&SuccessTool::setShowMode, this,2),1.0); 
    viewInProgressButton = new Button(playSoundFunc,posBoutton.x+0.05, posBoutton.y, ImVec2(0.045, 0.03),
                               ImVec4(20.0/255.0, 25.0/255.0, 30.0/255.0, 1.0f),
                               ImVec4(20.0/255.0, 25.0/255.0, 30.0/255.0,  1.0f),
                               "In Progress", 0.0f,16.0,
                               std::bind(&SuccessTool::setShowMode, this,3),1.0); 
    closeBtn = new ImageButton(nullptr,0.728646, 0.18, ImVec2(0.025, 0.025),0.65,
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f),ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                        "../assets/button/close.png", 0,
                            std::bind(&SuccessTool::Close, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    nextBadge = new ImageButton(nullptr,bottomRightBadge.x/winWidth, centerYBagde/winHeight, ImVec2(0.035, 0.035),1.0f,
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f),ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                        "../assets/button/right.png", 0,
                            std::bind(&SuccessTool::changeScrollOffsetBadge, this,1),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    prevBadge = new ImageButton(nullptr,topLeftBadge.x/winWidth, centerYBagde/winHeight, ImVec2(0.035, 0.035),1.0,
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f),ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                        "../assets/button/left.png", 0,
                            std::bind(&SuccessTool::changeScrollOffsetBadge, this,-1),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    

    mainIcon = new Icon(0.271354, 0.185078,ImVec2(0.035, 0.035),0.65f,"../assets/button/trophy.png",0.8);

    //close button:0.73125, 0.181202
    float fontSizeScaledMain = 18.0f; // Taille de police
    float fontSizeScaledSub = 15.0f; // Taille de police
    float fontSizeScaledSubSub = 13.0f; // Taille de police
    float badgeFontScale = 13.0f;
    questTitleFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), fontSizeScaledMain*winWidth / 1920);
    questExplanationFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), fontSizeScaledSub*winWidth / 1920);
    questProgressFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), fontSizeScaledSubSub*winWidth / 1920);
    badgeTitleFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), badgeFontScale*winWidth / 1920);

    checkMark = loadTexture("/assets/button/planetSuccess.png", 0.8f);
    //if(checkMark == 0)std::cout<<"Error loading checkMark"<<std::endl;

}





void SuccessTool::drawUI(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = winWidth*0.020;

    topLeft = ImVec2(centerPos.x - longueur * 0.5, centerPos.y - hauteur * 0.5);
    bottomRight = ImVec2(centerPos.x + longueur * 0.5, centerPos.y + hauteur * 0.5);

    if(isTransiting){
        topLeft = ImVec2(centerPos.x - longueur * 0.5*transitionStep, centerPos.y - hauteur * 0.5*transitionStep);
        bottomRight = ImVec2(centerPos.x + longueur * 0.5*transitionStep, centerPos.y + hauteur * 0.5*transitionStep);
    }
    transitionStep += transitionSpeedBackground;
    if(transitionStep > 1){
        isTransiting = false;
        transitionStep = 0;
    }


    drawList->AddRectFilled(topLeft, 
                            bottomRight, 
                            IM_COL32(0,0,0,175), // Couleur
                            cornerRadius);    // Dessiner le premier rectangle avec coins arrondis

    drawList->AddRectFilled(topLeft, 
                            bottomRight, 
                            IM_COL32(20, 25, 30, 150), // Couleur
                            cornerRadius);

    //Draw Rec for Badges and rec for quests

/*
    drawList->AddRect(topLeftBadge, 
                            bottomRightBadge, 
                            IM_COL32(255,255,255,175));    // Dessiner le premier rectangle avec coins arrondis

    drawList->AddRect(topLeftQuest, 
                            bottomRightQuest, 
                            IM_COL32(255,0,255,175));    // Dessiner le premier rectangle avec coins arrondis
    */


    //COUT mouse click position
    if(ImGui::IsMouseClicked(0))std::cout<< "Mouse click position: " << ImGui::GetMousePos().x/winWidth << ", " << ImGui::GetMousePos().y/winHeight << std::endl;

    if(isTransiting)return;

    title->Draw();
    badgeTitle->Draw();
    questTitle->Draw();
    viewAllButton->Draw();
    viewCompletedButton->Draw();
    viewInProgressButton->Draw();
    closeBtn->Draw();
    mainIcon->Draw();

    drawScrollBarQuest();
    //drawScrollBarBadge();
    drawQuests();
    drawBadges();
                            
}

void SuccessTool::drawQuests(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    float spacing = winHeight*0.000;
    float questHeight = (hauteurRectQuest-(maxQuestShown-1)*spacing)/maxQuestShown;

    //Loop thouh max between maxQuestShown and the number of success in successToShow
    int max = std::min(maxQuestShown, static_cast<int>(successToShow.size()));
    //Draw a rec for each quests : 
    for(int i = 0; i < max; i++){
        ImVec2 topLeft = ImVec2(topLeftQuest.x, topLeftQuest.y + i * (questHeight + spacing));
        ImVec2 bottomRight = ImVec2(bottomRightQuest.x, topLeft.y + questHeight);


        //Hovering------------------------------------------------------------
        bool isHovering = ImGui::IsMouseHoveringRect(topLeft, bottomRight);
        bool mouseClicked = ImGui::IsMouseClicked(0);
        bool isSelected = selectedSuccess == successToShow[i+questOffset];
        ImVec4 color = ImVec4(255, 255, 255, 100);
        if(isHovering){
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if(!isSelected){
            float largeur = winWidth*0.002;
            ImVec2 topleftRectSelected = ImVec2(topLeft.x - winWidth*0.01-largeur/2, topLeft.y+questHeight*0.2);
            ImVec2 bottomRightRectSelected = ImVec2(topLeft.x - winWidth*0.01 + largeur/2, bottomRight.y-questHeight*0.2);
            drawList->AddRectFilled(topleftRectSelected, bottomRightRectSelected, IM_COL32(255, 255, 255, 100), winWidth*0.01);}
        }

        if(mouseClicked && isHovering){//If clicked, select the quest
            selectedSuccess = successToShow[i+questOffset];
        }

        //If this quest is selected, draw a rec
        if(selectedSuccess == successToShow[i+questOffset]){
            float largeur = winWidth*0.01;
            ImVec2 topleftRectSelected = ImVec2(topLeft.x - winWidth*0.01-largeur/2, topLeft.y+questHeight*0.1);
            ImVec2 bottomRightRectSelected = ImVec2(bottomRight.x - winWidth*0.01 + largeur/2, bottomRight.y-questHeight*0.1);
            drawList->AddRectFilled(topleftRectSelected, bottomRightRectSelected, IM_COL32(255, 255, 255, 20), winWidth*0.01);
        }



        //Add quest text------------------------------------------------------------
        float pourcentageTextFromMid = 0.75;
        ImVec2 topLeftText = ImVec2(centerPos.x-(longueurRects/2)*pourcentageTextFromMid, topLeft.y + (bottomRight.y - topLeft.y) * 0.30);
        ImGui::PushFont(questTitleFont);
        std::string text = successToShow[i+questOffset]->title;
        ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
        ImVec2 textPos = ImVec2(topLeftText.x, topLeftText.y);
        float alphaMAIN = isHovering ? 220 : 175;
        alphaMAIN = isSelected ? 255 : alphaMAIN;

        drawList->AddText(textPos, IM_COL32(color.x,color.y,color.z, alphaMAIN), text.c_str());
        ImGui::PopFont();

        //Add quest Description
        ImGui::PushFont(questExplanationFont);
        text = successToShow[i+questOffset]->description;
        float spacingText = + questHeight*0.3;
        textSize = ImGui::CalcTextSize(text.c_str());
        textPos = ImVec2(topLeftText.x, topLeftText.y +spacingText);
        float alphaSecond = isHovering ? 125 : 100;
        alphaSecond = isSelected ? 150 : alphaSecond;
        drawList->AddText(textPos, IM_COL32(color.x,color.y,color.z, alphaSecond), text.c_str());
        ImGui::PopFont();

        //Draw the badge, circle at the left of the text------------------------------------------------------------
        float posX = questTitle->getPosition().x*winWidth + questTitle->getSize().x/2;
        ImVec2 badgeCenter = ImVec2(posX, topLeft.y + questHeight*0.5);
        float badgeRadius = questTitle->getSize().x*0.3;
        drawBadgesForQuest(badgeCenter, badgeRadius, successToShow[i+questOffset]);


        //Draw the progress of the quest------------------------------------------------------------
        if(successToShow[i+questOffset]->getTotalSteps() == 0)continue;
        //If completed, write completed
        ImVec2 centerBar = ImVec2(centerPos.x+(longueurRects*0.30), topLeft.y + (bottomRight.y - topLeft.y) * 0.5);
        
        if(successToShow[i+questOffset]->isCompleted()){
            ImGui::PushFont(questTitleFont);
            std::string progressText = "Completed!";
            ImVec2 textSizeProgress = ImGui::CalcTextSize(progressText.c_str());
            ImVec2 textPosProgress = ImVec2(centerBar.x - textSizeProgress.x*0.5, centerBar.y - textSizeProgress.y*0.5);
            drawList->AddText(textPosProgress, IM_COL32(color.x,color.y,color.z, 255), progressText.c_str());
            ImGui::PopFont();
            //Add checkMark image next to completed! Text
            ImVec2 sizeCheckMark = ImVec2(questHeight*0.3, questHeight*0.3);
            ImVec2 topLeftCheckMark = ImVec2(centerBar.x + textSizeProgress.x*0.5 + sizeCheckMark.x/2, centerBar.y-sizeCheckMark.y/2);
            ImVec2 bottomRightCheckMark = ImVec2(topLeftCheckMark.x + sizeCheckMark.x, topLeftCheckMark.y + sizeCheckMark.y);
            //drawList->AddImage((void*)(intptr_t)checkMark, topLeftCheckMark, bottomRightCheckMark);
            //Square around the image to debug and circle in middle
            continue;
        }
        int stepDone = successToShow[i+questOffset]->getStepsDone();
        int stepTotal = successToShow[i+questOffset]->getTotalSteps();
        float progress = static_cast<float>(stepDone) / static_cast<float>(stepTotal);


        //Simple rec for progressBar
        float hauteureBar = questHeight*0.30;
        float longueurBar = longueurRects*0.25;
        ImVec2 topLeftBar = ImVec2(centerBar.x - longueurBar * 0.5, centerBar.y - hauteureBar * 0.5);
        ImVec2 bottomRightBar = ImVec2(centerBar.x + longueurBar * 0.5, centerBar.y + hauteureBar * 0.5);
        drawList->AddRectFilled(topLeftBar, bottomRightBar, IM_COL32(color.x,color.y,color.z, 50), winHeight*0.01);

        //Add rect for progress
        if(progress > 0){
        ImVec2 topLeftProgress = ImVec2(centerBar.x - longueurBar * 0.5, centerBar.y - hauteureBar * 0.5);
        ImVec2 bottomRightProgress = ImVec2(centerBar.x - longueurBar * 0.5 + longueurBar*progress, centerBar.y + hauteureBar * 0.5);
        drawList->AddRectFilled(topLeftProgress, bottomRightProgress, IM_COL32(color.x,color.y,color.z, 100), winHeight*0.01);
        }

        //Write the progress in the middle of rec
        ImGui::PushFont(questProgressFont);
        std::string progressText = std::to_string(stepDone) + "/" + std::to_string(stepTotal);
        ImVec2 textSizeProgress = ImGui::CalcTextSize(progressText.c_str());
        ImVec2 textPosProgress = ImVec2(centerBar.x - textSizeProgress.x*0.5, centerBar.y - textSizeProgress.y*0.5);
        drawList->AddText(textPosProgress, IM_COL32(color.x,color.y,color.z, 255), progressText.c_str());
        ImGui::PopFont();
        


    }

    //If click outside of the quest rec, unselect the quest
    if(ImGui::IsMouseClicked(0) && !ImGui::IsMouseHoveringRect(topLeftQuest, bottomRightQuest)){
        selectedSuccess = nullptr;
    }

}

void SuccessTool::drawBadgesForQuest(ImVec2 pos, float radius, Success* success){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    //Add checkMark at position and use radius to get the size
    ImVec2 topLeft = ImVec2(pos.x - radius, pos.y - radius);
    ImVec2 bottomRight = ImVec2(pos.x + radius, pos.y + radius);

    GLuint badge = success->isCompleted() ? success->badgeCOMPLETED : success->badgeNOTCOMPLTETED;

    drawList->AddImage((void*)(intptr_t)badge, topLeft, bottomRight);

    return;

}


void SuccessTool::drawScrollBarQuest(){
    if(successToShow.size() <= maxQuestShown)return;
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 mousePos = ImGui::GetMousePos();
        float scrollbarWidth = winWidth*0.15*0.15*0.05; // La largeur de la barre de défilement
        float numberOfQuests = successToShow.size();
        ImVec2 centerPosScrollBar =  ImVec2(bottomRightQuest.x+winWidth*0.015, centerYQuest);
        // Calcul de la hauteur de la barre de défilement en fonction du nombre d'éléments visibles
        float totalItemsHeight = hauteurRectQuest*0.8; // La hauteur totale disponible pour les éléments
        ImVec2 topLeftScrollBar = ImVec2(centerPosScrollBar.x - scrollbarWidth/2, centerPosScrollBar.y - totalItemsHeight/2);
        ImVec2 bottomRightScrollBar = ImVec2(centerPosScrollBar.x + scrollbarWidth/2, centerPosScrollBar.y + totalItemsHeight/2);
        float visibleItemsRatio = static_cast<float>(maxQuestShown) / static_cast<float>(numberOfQuests);
        float scrollbarHeight = totalItemsHeight * visibleItemsRatio;

        // Calcul de la position Y de la barre de défilement en fonction de questOffset
        float scrollbarPositionRatio = static_cast<float>(questOffset) / static_cast<float>(numberOfQuests - maxQuestShown);
        float scrollbarYPosition = topLeft.y + (totalItemsHeight - scrollbarHeight) * scrollbarPositionRatio;
        float scrollbarWidthActive = 3*scrollbarWidth;
        // Dessiner le fond de la barre de défilement
        drawList->AddRectFilled(topLeftScrollBar, bottomRightScrollBar, IM_COL32(255, 255, 255, 100));

                // Dessiner la barre de défilement
        float scrollbarActiveYPosition = topLeftScrollBar.y + (bottomRightScrollBar.y - topLeftScrollBar.y - scrollbarHeight) * scrollbarPositionRatio;


        ImVec2 topLeftScrollBarActive = ImVec2(centerPosScrollBar.x - scrollbarWidthActive / 2, scrollbarActiveYPosition);
        ImVec2 bottomRightScrollBarActive = ImVec2(centerPosScrollBar.x + scrollbarWidthActive / 2, scrollbarActiveYPosition + scrollbarHeight);

        // Utilisez une couleur distincte pour la barre de défilement active pour la différencier du fond
        float detectionWidth = scrollbarWidth * 15; // Increase the width for hover detection
        ImVec2 detectionTopLeft = ImVec2(centerPosScrollBar.x - detectionWidth / 2, topLeftScrollBar.y);
        ImVec2 detectionBottomRight = ImVec2(centerPosScrollBar.x + detectionWidth / 2, bottomRightScrollBar.y);

        // Check if mouse is hovering over the widened detection area
        bool isHoveringScrollBar = mousePos.x >= detectionTopLeft.x && mousePos.x <= detectionBottomRight.x && mousePos.y >= detectionTopLeft.y && mousePos.y <= detectionBottomRight.y;
        if (isHoveringScrollBar) {
            //Transform cursor to hand
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            // Increase alpha when hovering
            drawList->AddRectFilled(topLeftScrollBarActive, bottomRightScrollBarActive, IM_COL32(255, 255, 255, 255), winWidth*0.002); // Hover alpha = 255

            if (ImGui::IsMouseDown(0)) {
                // Adjust the offset based on the mouse position relative to the scrollbar's top and bottom
                float mouseRelativeToScrollbar = (mousePos.y - detectionTopLeft.y) / (detectionBottomRight.y - detectionTopLeft.y);
                int newOffset = static_cast<int>(mouseRelativeToScrollbar * (numberOfQuests - maxQuestShown + 1)); // +1 to ensure reachability of last item
                newOffset = std::max(0, std::min(newOffset, static_cast<int>(numberOfQuests) - maxQuestShown));
                
                if(newOffset != questOffset) {
                    // Calculate the direction of scroll based on newOffset vs. current questOffset
                    int scrollDirection = (newOffset > questOffset) ? 1 : -1;
                    while(questOffset != newOffset) {
                        changeScrollOffsetQuest(scrollDirection);
                    }
                }
            }
        } else {
            // Default scrollbar alpha = 200
            drawList->AddRectFilled(topLeftScrollBarActive, bottomRightScrollBarActive, IM_COL32(255, 255, 255, 200), winWidth*0.002);
        }

        //If mouse scrolling and over the quest rec, scroll the quest
        bool hoveringRectQuest = mousePos.x >= topLeft.x && mousePos.x <= bottomRight.x && mousePos.y >= topLeftQuest.y && mousePos.y <= bottomRightQuest.y;
        if(hoveringRectQuest && ImGui::GetIO().MouseWheel != 0){
            changeScrollOffsetQuest(-ImGui::GetIO().MouseWheel);
        }


}

void SuccessTool::drawScrollBarBadge(){
    std::vector<Success*> badgeToDraw;
    //Draw only completed badge
    for(int i = 0; i < successList.size(); i++){
        if(successList[i]->isCompleted()){
            badgeToDraw.push_back(successList[i]);
        }
    }
    if(badgeToDraw.size() <= maxBadgeShown)return;

    

    //Draw a rec for the scrollbar
    //Draw a rec for the scrollbar
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 mousePos = ImGui::GetMousePos();
    float scrollbarWidth = winWidth*0.15*0.15*0.05; // La largeur de la barre de défilement
    float numberOfBadges = badgeToDraw.size();
    ImVec2 centerPosScrollBar =  ImVec2(bottomRightBadge.x+winWidth*0.015, centerYBagde);
    // Calcul de la hauteur de la barre de défilement en fonction du nombre d'éléments visibles
    float totalItemsHeight = hauteurRectBadge*0.8; // La hauteur totale disponible pour les éléments
    ImVec2 topLeftScrollBar = ImVec2(centerPosScrollBar.x - scrollbarWidth/2, centerPosScrollBar.y - totalItemsHeight/2);
    ImVec2 bottomRightScrollBar = ImVec2(centerPosScrollBar.x + scrollbarWidth/2, centerPosScrollBar.y + totalItemsHeight/2);

    float visibleItemsRatio = static_cast<float>(maxBadgeShown) / static_cast<float>(numberOfBadges);
    float scrollbarHeight = totalItemsHeight * visibleItemsRatio;





    float scrollbarPositionRatio = static_cast<float>(badgeOffset) / static_cast<float>(numberOfBadges - maxBadgeShown);
    float scrollbarYPosition = topLeft.y + (totalItemsHeight - scrollbarHeight) * scrollbarPositionRatio;
    float scrollbarWidthActive = 3*scrollbarWidth;
    // Dessiner le fond de la barre de défilement
    drawList->AddRectFilled(topLeftScrollBar, bottomRightScrollBar, IM_COL32(255, 255, 255, 100));

               // Dessiner la barre de défilement
        float scrollbarActiveYPosition = topLeftScrollBar.y + (bottomRightScrollBar.y - topLeftScrollBar.y - scrollbarHeight) * scrollbarPositionRatio;


        ImVec2 topLeftScrollBarActive = ImVec2(centerPosScrollBar.x - scrollbarWidthActive / 2, scrollbarActiveYPosition);
        ImVec2 bottomRightScrollBarActive = ImVec2(centerPosScrollBar.x + scrollbarWidthActive / 2, scrollbarActiveYPosition + scrollbarHeight);

        // Utilisez une couleur distincte pour la barre de défilement active pour la différencier du fond
        float detectionWidth = scrollbarWidth * 15; // Increase the width for hover detection
        ImVec2 detectionTopLeft = ImVec2(centerPosScrollBar.x - detectionWidth / 2, topLeftScrollBar.y);
        ImVec2 detectionBottomRight = ImVec2(centerPosScrollBar.x + detectionWidth / 2, bottomRightScrollBar.y);

        // Check if mouse is hovering over the widened detection area
        bool isHoveringScrollBar = mousePos.x >= detectionTopLeft.x && mousePos.x <= detectionBottomRight.x && mousePos.y >= detectionTopLeft.y && mousePos.y <= detectionBottomRight.y;
        if (isHoveringScrollBar) {
            //Transform cursor to hand
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            // Increase alpha when hovering
            drawList->AddRectFilled(topLeftScrollBarActive, bottomRightScrollBarActive, IM_COL32(255, 255, 255, 255), winWidth*0.002); // Hover alpha = 255

            if (ImGui::IsMouseDown(0)) {
                // Adjust the offset based on the mouse position relative to the scrollbar's top and bottom
                float mouseRelativeToScrollbar = (mousePos.y - detectionTopLeft.y) / (detectionBottomRight.y - detectionTopLeft.y);
                int newOffset = static_cast<int>(mouseRelativeToScrollbar * (numberOfBadges - maxBadgeShown + 1)); // +1 to ensure reachability of last item
                newOffset = std::max(0, std::min(newOffset, static_cast<int>(numberOfBadges) - maxBadgeShown));
                
                if(newOffset != badgeOffset) {
                    // Calculate the direction of scroll based on newOffset vs. current questOffset
                    int scrollDirection = (newOffset > badgeOffset) ? 1 : -1;
                    while(badgeOffset != newOffset) {
                        changeScrollOffsetBadge(scrollDirection);
                    }
                }
            }
        } else {
            // Default scrollbar alpha = 200
            drawList->AddRectFilled(topLeftScrollBarActive, bottomRightScrollBarActive, IM_COL32(255, 255, 255, 200), winWidth*0.002);
        }

        bool hoveringRectBadge = mousePos.x >= topLeft.x && mousePos.x <= bottomRight.x && mousePos.y >= topLeftBadge.y && mousePos.y <= bottomRightBadge.y;
        if(hoveringRectBadge && ImGui::GetIO().MouseWheel != 0){
            changeScrollOffsetBadge(-ImGui::GetIO().MouseWheel);
        }
}


void SuccessTool::changeScrollOffsetQuest(int direction){
    questOffset += direction;
    if(questOffset < 0) questOffset = 0;
    if(questOffset > successToShow.size() - maxQuestShown) questOffset = successToShow.size() - maxQuestShown;

}

void SuccessTool::changeScrollOffsetBadge(int direction){
    //Number of badges = number of completed success
    int numberOfBadges = 0;
    for(int i = 0; i < successList.size(); i++){
        if(successList[i]->isCompleted()){
            numberOfBadges++;
        }
    }
    badgeOffset += direction;
    if(badgeOffset < 0) badgeOffset = 0;
    if(badgeOffset > numberOfBadges - maxBadgeShown) badgeOffset = numberOfBadges - maxBadgeShown;
}


void SuccessTool::drawBadges(){
    //Draw a badge for each success. The max number of badge shown is maxBadgeShown, and the offset is badgeOffset
    //The badge is drawn in the badge rec, and the position is calculated based on the offset and the number of badge shown

    //For now, each badge is a circle drawn with drawBadgesForQuest(ImVec2 pos, float radius, Success* success)
    //For the position X, you need to take the position of the badge rec, and calculate the position of each badge based on the offset and the number of badge shown

    float radius = hauteurRectBadge*0.325;
    //Patern is space, badge, space, badge, space, badge, space, badge, space....
    //Badge are drawn from left to right, at the same height

    float totalSpace = longueurRects - radius*2*maxBadgeShown;
    float space = totalSpace/(maxBadgeShown+1);

    std::vector<Success*> badgeToDraw;
    //Draw only completed badge
    for(int i = 0; i < successList.size(); i++){
        if(successList[i]->isCompleted()){
            badgeToDraw.push_back(successList[i]);
        }
    }
    if(badgeToDraw.size() == 0){
        //Write "empty" in the middle and return
        ImGui::PushFont(questTitleFont);
        ImVec2 textSize = ImGui::CalcTextSize("Empty");
        ImVec2 textPos = ImVec2(centerPos.x - textSize.x*0.5, centerYBagde-textSize.y*0.5);
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddText(textPos, IM_COL32(255,255,255,150), "Empty");
        ImGui::PopFont();
        return;
    }


    //Draw nextButton if there are more badges to show
    if(badgeOffset + maxBadgeShown < badgeToDraw.size()){
        nextBadge->Draw();
    }
    //Draw prevButton if there are more badges to show to the left
    if(badgeOffset > 0){
        prevBadge->Draw();
    }

    //Min between maxBadgeShown and the number of badge to draw
    int max = std::min(maxBadgeShown, static_cast<int>(badgeToDraw.size()));

        ImGui::PushFont(badgeTitleFont);
    for(int i = 0; i < max; i++){
        ImVec2 pos = ImVec2(topLeftBadge.x + radius + (radius*2+space)*i+space, centerYBagde);
        drawBadgesForQuest(pos, radius, badgeToDraw[i+badgeOffset]);
        std::string text = badgeToDraw[i+badgeOffset]->title;
        ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
        ImVec2 textPos = ImVec2(pos.x - textSize.x*0.5, pos.y + radius*1.5+textSize.y*0.5);
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddText(textPos, IM_COL32(255,255,255,150), text.c_str());
    }
    ImGui::PopFont();
}


GLuint SuccessTool::loadTexture(const char* relativeFilename, float alpha) {
    std::string fullPath = getFullPath(relativeFilename);
    cv::Mat image = cv::imread(fullPath, cv::IMREAD_UNCHANGED);
    if (image.empty()) {
        std::cerr << "Erreur: Image non trouvée " << fullPath << std::endl;
        return 0;
    }
    //Change alpha 
    GLenum format;
    if (image.channels() == 4) {
        format = GL_RGBA;
    } else if (image.channels() == 3) {
        format = GL_RGB;
    } else {
        std::cerr << "Unsupported image format." << std::endl;
        return 0;
    }
    cv::Mat tintedImage = image.clone();
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

GLuint SuccessTool::loadTextureCOMPLETED(const char* relativeFilename, float alpha) {
    std::string fullPath = getFullPath(relativeFilename);
    cv::Mat image = cv::imread(fullPath, cv::IMREAD_UNCHANGED);
    if (image.empty()) {
        std::cerr << "Erreur: Image non trouvée " << fullPath << std::endl;
        return 0;
    }

    // Convertir BGR à RGB ou BGRA à RGBA
    if (image.channels() == 4) {
        cv::cvtColor(image, image, cv::COLOR_BGRA2RGBA);
    } else if (image.channels() == 3) {
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    } else {
        std::cerr << "Unsupported image format." << std::endl;
        return 0;
    }

    GLenum format = (image.channels() == 4) ? GL_RGBA : GL_RGB;

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
    glTexImage2D(GL_TEXTURE_2D, 0, format, image.cols, image.rows, 0, format, GL_UNSIGNED_BYTE, image.data);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}