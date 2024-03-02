// UITool.h
#ifndef MusicPlayerTool_H
#define MusicPlayerTool_H


#include "RenderComponent.h"
#include "engine/Vec3.h"
#include <chrono>
#include <functional>

//Struct songs with path, name, length
struct Song {
    std::string path;
    std::string name;
    std::string artist;
    int length;
    std::string albumCoverPath;
    ImVec4 albumColor;
    Mix_Music* music = nullptr; // Pointeur vers la musique chargée
    friend bool operator==(const Song& lhs, const Song& rhs) {
        // Choisissez le champ approprié pour la comparaison. Ici, on utilise le chemin comme unique identifiant.
        return lhs.path == rhs.path;
    }
    GLuint albumTexture;
    ImVec4 backgroundColor;
};

enum class PlaybackState {
    Stopped,
    Playing,
    Paused
};

class MusicPlayerTool : public RenderComponent {
public:

    explicit MusicPlayerTool(RenderContext* renderContext);
    ~MusicPlayerTool() override;
    void Draw() override;

    void nextSong();
    void previousSong();
    void play();
    void pause();
    void restartSong();

    bool mouseHoveringPlayer();
    void changeMusicVolume(float volume);
    void setVolumeButtonFunction(std::function<void()> function);
    void volumeButtonFunctionCalled();
    void StartCounting();

private:
    //Vector of songs
    ImVec2 centerPos;
    float longueur,hauteur;
    int winWidth, winHeight;
    std::vector<Song> songs;
    Song* currentSong = nullptr;
    float songProgression = 0;
    float songLength = 100;

    void initUI();
    void initSDL_Mixer();
    void initSongs();
    void update();

    ImageButton* playButton = nullptr;
    ImageButton* pauseButton = nullptr;
    ImageButton* nextButton = nullptr;
    ImageButton* previousButton = nullptr;
    ImageButton* repeatButtonOn = nullptr;
    ImageButton* shuffleButtonOn = nullptr;
    ImageButton* volumeButton = nullptr;
    Labbel* songName = nullptr;
    Labbel* songArtist = nullptr;
    Labbel* songProgressionTime = nullptr;
    Labbel* songTimeLeft = nullptr;
    bool isRepeating = false;
    bool isShuffling = false;
    bool shouldRestartSong = false;

    bool isPlaying = false;
    void drawUI();
    void drawProgressionBar();
    void updateProgressionLabbel();
    void repeatButton();
    void shuffleButton();
    void updateProgress();

    //chrono
    std::chrono::steady_clock::time_point musicStartTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point pauseTimePoint = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point lastUpdateTime = std::chrono::steady_clock::now();
    std::chrono::seconds totalPauseDuration{0};
    double pausedProgression = 0;
    PlaybackState playbackState = PlaybackState::Paused;
    void setSongTime(float time);

    float currentVolume = 1.0;
    void checkVolumeChange();

    std::function<void()> volumeButtonFunction;

    GLuint loadTexture(const char* relativeFilename);

};

#endif // MusicPlayerTool_H