#include <iostream>
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <cmath>
#include<fstream>
#include<ctime>
using namespace std;
using namespace sf;

void saveGameState(const int arr[5][5]) {
    ofstream outFile("saved_game.txt");
    if (outFile.is_open()) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                outFile << arr[i][j] << " ";
            }
        }
        outFile.close();
    }
}

void loadGameState(int arr[5][5]) {
    ifstream inFile("saved_game.txt");
    if (inFile.is_open()) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                inFile >> arr[i][j];
            }
        }

        inFile.close();
    }
}

void resumeGame(int arr[5][5]) {
    ifstream checkFile("saved_game.txt");
    if (checkFile.is_open()) {
        loadGameState(arr);
        cout << "Game resumed successfully." << endl;
    }
}



void coin_toss(int& move) {
    int random;
    cout << "Tail is for Green & Head is for Red!" << endl;
    srand(static_cast<unsigned int>(time(NULL)));
    random = rand() % 100 + 1;
    if (random % 2 == 0) {
        move = 0;
        cout << "\t --> Tails Win! <--" << "\n";
    }
    else {
        move = 1;
        cout << "\t --> Heads Win! <--" << "\n";
    }
}

int main() {
    int arr[5][5] = { {1, 1, 1, 1, 1},{1, 1, 1, 1, 1},{1, 1, 0, 2, 2},{2, 2, 2, 2, 2},{2, 2, 2, 2, 2} };

    RenderWindow window(VideoMode(950, 950), "12 Bead Game");
    SoundBuffer move_, elimination, valid;
    move_.loadFromFile("./movesound.mp3");
    elimination.loadFromFile("./eliminationsound.mp3");
    valid.loadFromFile("./invalidsound.mp3");
    Sound movement, eliminate, in, sound;
    movement.setBuffer(move_);
    eliminate.setBuffer(elimination);
    in.setBuffer(valid);

    Texture backgroundTexture, start, logoBackgroundTexture, button, greenwin, redwin, pause;
    greenwin.loadFromFile("./Redwin.png");
    redwin.loadFromFile("./greenwin.png");
    button.loadFromFile("./exit.png");
    start.loadFromFile("./start.png");
    pause.loadFromFile("./resume.png");
    Sprite win1(greenwin), win2(redwin);
    if (!backgroundTexture.loadFromFile("12_bead_board.png")) {
        cout << "Failed to load background image" << endl;
        return 1;
    }

    if (!logoBackgroundTexture.loadFromFile("./board_background.jpg")) {
        cout << "Failed to load logo background image" << endl;
        return 1;
    }

    Sprite logoBackgroundSprite(logoBackgroundTexture), exit(button);
    exit.setPosition(300, 550);
    exit.setScale(0.6, 0.6);
    Sprite backgroundSprite(backgroundTexture), main(start), resume(pause);
    resume.setPosition(260, 300);
    resume.setScale(0.8, 0.8);
    bool starting = 0;

    int move;
    coin_toss(move);

    int redbeads = 12;
    int greenbeads = 12;
    float radius = 25;

    int x1 = -1, y1 = -1, x2 = -1, y2 = -1;
    main.setPosition(270, 130);
    main.setScale(0.7, 0.7);

    bool end_sound = -1;

    Texture pauseTexture;
    if (!pauseTexture.loadFromFile("pause.png")) {
        cout << "Failed to load pause image" << endl;
        return 1;
    }
    Sprite pauseSprite(pauseTexture);
    pauseSprite.setPosition(40, 875);
    pauseSprite.setScale(1, 1);
    bool isPaused = false;

    Texture pauseTexture2;
    if (!pauseTexture2.loadFromFile("pause2.png")) {
        cout << "Failed to load pause image" << endl;
        return 1;
    }
    Sprite pauseSprite2(pauseTexture2);
    pauseSprite2.setPosition(40, 875);
    pauseSprite2.setScale(1, 1);

    Texture greenturntexture;
    if (!greenturntexture.loadFromFile("greenturn.png")) {
        cout << "Failed to load pause image" << endl;
        return 1;
    }
    Sprite greenturnsprite(greenturntexture);
    greenturnsprite.setPosition(40, -15);
    greenturnsprite.setScale(1, 0.8);

    Texture redturntexture;
    if (!redturntexture.loadFromFile("redturn.png")) {
        cout << "Failed to load pause image" << endl;
        return 1;
    }
    Sprite redturnsprite(redturntexture);
    redturnsprite.setPosition(40, -15);
    redturnsprite.setScale(1, 0.8);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                saveGameState(arr);
                window.close();
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::P) {
                isPaused = !isPaused;


            }
            if (!isPaused) {

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
                    if (exit.getGlobalBounds().contains(mousePos) && starting == 0) {
                        return 0;
                    }


                    if (main.getGlobalBounds().contains(mousePos)) {
                        starting = 1;
                    }
                    if (resume.getGlobalBounds().contains(mousePos) && starting != 1) {
                        resumeGame(arr);
                        starting = 1;
                    }
                    if (starting == 1) {
                        for (int i = 0; i < 5; i++) {
                            for (int j = 0; j < 5; j++) {
                                // Calculate the center and the distance of the circle
                                float centerX = 169.5 * j + 136;
                                float centerY = 169.5 * i + 136;
                                float distance = sqrt(pow(mousePos.x - centerX, 2) + pow(mousePos.y - centerY, 2));
                                if (distance <= radius) {
                                    if (x1 == -1 && y1 == -1) {
                                        x1 = i;
                                        y1 = j;
                                        //Check for Green beads
                                        if (move % 2 == 0) {
                                            // For Upward check
                                            if ((x1 != -1 && y1 != -1) && (arr[x1][y1] == 2) && (arr[x1 - 1][y1] == 0)) {
                                                arr[x1 - 1][y1] = 3;
                                            }
                                            // For Downward check
                                            if ((x1 != -1 && y1 != -1) && (arr[x1][y1] == 2) && (arr[x1 + 1][y1] == 0)) {
                                                arr[x1 + 1][y1] = 3;
                                            }
                                            // For Right check
                                            if ((x1 != -1 && y1 != -1) && (arr[x1][y1] == 2) && (arr[x1][y1 + 1] == 0)) {
                                                arr[x1][y1 + 1] = 3;
                                            }
                                            // For Left check
                                            if ((x1 != -1 && y1 != -1) && (arr[x1][y1] == 2) && (arr[x1][y1 - 1] == 0)) {
                                                arr[x1][y1 - 1] = 3;
                                            }
                                            //Check for elimination by upward move
                                            if ((x1 != -1 && y1 != -1) && (arr[x1][y1] == 2) && (arr[x1 - 1][y1] == 1) && (arr[x1 - 2][y1] == 0)) {
                                                arr[x1 - 2][y1] = 3;
                                            }
                                            //Check for elimination by downward move
                                            if ((x1 != -1 && y1 != -1) && (arr[x1][y1] == 2) && (arr[x1 + 1][y1] == 1) && (arr[x1 + 2][y1] == 0)) {
                                                arr[x1 + 2][y1] = 3;
                                            }
                                            //Check for elimination by right move
                                            if ((x1 != -1 && y1 != -1) && (arr[x1][y1] == 2) && (arr[x1][y1 + 1] == 1) && (arr[x1][y1 + 2] == 0)) {
                                                arr[x1][y1 + 2] = 3;
                                            }
                                            //Check for elimination by left move
                                            if ((x1 != -1 && y1 != -1) && (arr[x1][y1] == 2) && (arr[x1][y1 - 1] == 1) && (arr[x1][y1 - 2] == 0)) {
                                                arr[x1][y1 - 2] = 3;
                                            }


                                            //Check for diagonal movement
                                            if ((x1 % 2 == 0 && y1 % 2 == 0) || (x1 % 2 == 1 && y1 % 2 == 1)) {
                                                //Check for top right
                                                if ((arr[x1][y1] == 2) && (arr[x1 - 1][y1 + 1] == 0)) {
                                                    arr[x1 - 1][y1 + 1] = 3;
                                                }
                                                //Check for top left
                                                if ((arr[x1][y1] == 2) && (arr[x1 - 1][y1 - 1] == 0)) {
                                                    arr[x1 - 1][y1 - 1] = 3;
                                                }
                                                //Check for bottom left
                                                if ((arr[x1][y1] == 2) && (arr[x1 + 1][y1 - 1] == 0)) {
                                                    arr[x1 + 1][y1 - 1] = 3;
                                                }
                                                //Check for bottom right
                                                if ((arr[x1][y1] == 2) && (arr[x1 + 1][y1 + 1] == 0)) {
                                                    arr[x1 + 1][y1 + 1] = 3;
                                                }

                                                //Elimination by top right movement
                                                if ((arr[x1][y1] == 2) && (arr[x1 - 1][y1 + 1] == 1) && (arr[x1 - 2][y1 + 2] == 0)) {
                                                    arr[x1 - 2][y1 + 2] = 3;
                                                }
                                                //Elimination by top left movement
                                                if ((arr[x1][y1] == 2) && (arr[x1 - 1][y1 - 1] == 1) && (arr[x1 - 2][y1 - 2] == 0)) {
                                                    arr[x1 - 2][y1 - 2] = 3;
                                                }
                                                //Elimination by bottom left movement
                                                if ((arr[x1][y1] == 2) && (arr[x1 + 1][y1 - 1] == 1) && (arr[x1 + 2][y1 - 2] == 0)) {
                                                    arr[x1 + 2][y1 - 2] = 3;
                                                }
                                                //Elimination by bottom right movement
                                                if ((arr[x1][y1] == 2) && (arr[x1 + 1][y1 + 1] == 1) && (arr[x1 + 2][y1 + 2] == 0)) {
                                                    arr[x1 + 2][y1 + 2] = 3;
                                                }


                                            }

                                            //Special Handling
                                            if (arr[x1][y1] == 2 && (x1 == 0 && y1 == 0) && (arr[0][4] == 0 || arr[0][4] == 3)) {
                                                arr[0][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 4) && (arr[0][0] == 0 || arr[0][0] == 3)) {
                                                arr[0][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 4) && (arr[1][0] == 0 || arr[1][0] == 3)) {
                                                arr[1][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 4) && (arr[2][0] == 0 || arr[2][0] == 3)) {
                                                arr[2][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 4) && (arr[2][0] == 0 || arr[2][0] == 3)) {
                                                arr[2][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 0) && (arr[0][4] == 0 || arr[0][4] == 3)) {
                                                arr[0][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 0) && (arr[0][4] == 0 || arr[0][4] == 3)) {
                                                arr[0][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 0) && (arr[1][4] == 0 || arr[1][4] == 3)) {
                                                arr[1][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 0) && (arr[2][4] == 0 || arr[2][4] == 3)) {
                                                arr[2][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 4) && (arr[2][0] == 0 || arr[2][0] == 3)) {
                                                arr[2][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 4) && (arr[3][0] == 0 || arr[3][0] == 3)) {
                                                arr[3][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 4) && (arr[4][0] == 0 || arr[4][0] == 3)) {
                                                arr[4][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 4) && (arr[2][0] == 0 || arr[2][0] == 3)) {
                                                arr[2][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 4) && (arr[4][0] == 0 || arr[4][0] == 3)) {
                                                arr[4][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 0) && (arr[2][4] == 0 || arr[2][4] == 3)) {
                                                arr[2][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 0) && (arr[2][4] == 0 || arr[2][4] == 3)) {
                                                arr[2][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 0) && (arr[3][4] == 0 || arr[3][4] == 3)) {
                                                arr[3][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 0) && (arr[4][4] == 0 || arr[4][4] == 3)) {
                                                arr[4][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 4) && (arr[4][0] == 0 || arr[4][0] == 3)) {
                                                arr[4][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 4) && (arr[1][1] == 0 || arr[1][1] == 3)) {
                                                arr[1][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 4) && (arr[3][1] == 0 || arr[3][1] == 3)) {
                                                arr[3][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 1) && (arr[1][4] == 0 || arr[1][4] == 3)) {
                                                arr[1][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 1) && (arr[2][4] == 0 || arr[2][4] == 3)) {
                                                arr[2][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 1) && (arr[3][4] == 0 || arr[3][4] == 3)) {
                                                arr[3][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 3) && (arr[1][0] == 0 || arr[1][0] == 3)) {
                                                arr[1][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 3) && (arr[2][0] == 0 || arr[2][0] == 3)) {
                                                arr[2][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 3) && (arr[3][0] == 0 || arr[3][0] == 3)) {
                                                arr[3][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 3) && (arr[0][0] == 0 || arr[0][0] == 3)) {
                                                arr[0][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 3) && (arr[4][0] == 0 || arr[4][0] == 3)) {
                                                arr[4][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 1) && (arr[0][4] == 0 || arr[0][4] == 3)) {
                                                arr[0][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 1) && (arr[4][4] == 0 || arr[4][4] == 3)) {
                                                arr[4][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 3) && (arr[1][0] == 0 || arr[1][0] == 3)) {
                                                arr[1][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 3) && (arr[2][0] == 0 || arr[2][0] == 3)) {
                                                arr[2][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 3) && (arr[3][0] == 0 || arr[3][0] == 3)) {
                                                arr[3][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 3) && (arr[0][0] == 0 || arr[0][0] == 3)) {
                                                arr[0][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 3) && (arr[4][0] == 0 || arr[4][0] == 3)) {
                                                arr[4][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 3) && (arr[1][0] == 0 || arr[1][0] == 3)) {
                                                arr[1][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 3) && (arr[2][0] == 0 || arr[2][0] == 3)) {
                                                arr[2][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 3) && (arr[3][0] == 0 || arr[3][0] == 3)) {
                                                arr[3][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 3) && (arr[0][0] == 0 || arr[0][0] == 3)) {
                                                arr[0][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 3) && (arr[4][0] == 0 || arr[4][0] == 3)) {
                                                arr[4][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 3) && (arr[1][0] == 0 || arr[1][0] == 3)) {
                                                arr[1][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 3) && (arr[2][0] == 0 || arr[2][0] == 3)) {
                                                arr[2][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 3) && (arr[3][0] == 0 || arr[3][0] == 3)) {
                                                arr[3][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 3) && (arr[0][0] == 0 || arr[0][0] == 3)) {
                                                arr[0][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 3) && (arr[4][0] == 0 || arr[4][0] == 3)) {
                                                arr[4][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 3) && (arr[1][0] == 0 || arr[1][0] == 3)) {
                                                arr[1][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 3) && (arr[2][0] == 0 || arr[2][0] == 3)) {
                                                arr[2][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 3) && (arr[3][0] == 0 || arr[3][0] == 3)) {
                                                arr[3][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 3) && (arr[0][0] == 0 || arr[0][0] == 3)) {
                                                arr[0][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 3) && (arr[4][0] == 0 || arr[4][0] == 3)) {
                                                arr[4][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 1) && (arr[0][4] == 0 || arr[0][4] == 3)) {
                                                arr[0][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 1) && (arr[1][4] == 0 || arr[1][4] == 3)) {
                                                arr[1][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 1) && (arr[2][4] == 0 || arr[2][4] == 3)) {
                                                arr[2][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 1) && (arr[3][4] == 0 || arr[3][4] == 3)) {
                                                arr[3][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 1) && (arr[4][4] == 0 || arr[4][4] == 3)) {
                                                arr[4][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 1) && (arr[0][4] == 0 || arr[0][4] == 3)) {
                                                arr[0][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 1) && (arr[1][4] == 0 || arr[1][4] == 3)) {
                                                arr[1][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 1) && (arr[2][4] == 0 || arr[2][4] == 3)) {
                                                arr[2][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 1) && (arr[3][4] == 0 || arr[3][4] == 3)) {
                                                arr[3][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 1) && (arr[4][4] == 0 || arr[4][4] == 3)) {
                                                arr[4][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 1) && (arr[0][4] == 0 || arr[0][4] == 3)) {
                                                arr[0][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 1) && (arr[1][4] == 0 || arr[1][4] == 3)) {
                                                arr[1][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 1) && (arr[2][4] == 0 || arr[2][4] == 3)) {
                                                arr[2][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 1) && (arr[3][4] == 0 || arr[3][4] == 3)) {
                                                arr[3][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 1) && (arr[4][4] == 0 || arr[4][4] == 3)) {
                                                arr[4][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 1) && (arr[0][4] == 0 || arr[0][4] == 3)) {
                                                arr[0][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 1) && (arr[1][4] == 0 || arr[1][4] == 3)) {
                                                arr[1][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 1) && (arr[2][4] == 0 || arr[2][4] == 3)) {
                                                arr[2][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 1) && (arr[3][4] == 0 || arr[3][4] == 3)) {
                                                arr[3][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 1) && (arr[4][4] == 0 || arr[4][4] == 3)) {
                                                arr[4][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 4) && (arr[0][1] == 0 || arr[0][1] == 3)) {
                                                arr[0][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 4) && (arr[1][1] == 0 || arr[1][1] == 3)) {
                                                arr[1][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 4) && (arr[2][1] == 0 || arr[2][1] == 3)) {
                                                arr[2][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 4) && (arr[3][1] == 0 || arr[3][1] == 3)) {
                                                arr[3][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 4) && (arr[4][1] == 0 || arr[4][1] == 3)) {
                                                arr[4][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 4) && (arr[0][1] == 0 || arr[0][1] == 3)) {
                                                arr[0][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 4) && (arr[1][1] == 0 || arr[1][1] == 3)) {
                                                arr[1][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 4) && (arr[2][1] == 0 || arr[2][1] == 3)) {
                                                arr[2][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 4) && (arr[3][1] == 0 || arr[3][1] == 3)) {
                                                arr[3][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 4) && (arr[4][1] == 0 || arr[4][1] == 3)) {
                                                arr[4][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 4) && (arr[0][1] == 0 || arr[0][1] == 3)) {
                                                arr[0][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 4) && (arr[1][1] == 0 || arr[1][1] == 3)) {
                                                arr[1][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 4) && (arr[2][1] == 0 || arr[2][1] == 3)) {
                                                arr[2][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 4) && (arr[3][1] == 0 || arr[3][1] == 3)) {
                                                arr[3][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 4) && (arr[4][1] == 0 || arr[4][1] == 3)) {
                                                arr[4][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 4) && (arr[0][1] == 0 || arr[0][1] == 3)) {
                                                arr[0][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 4) && (arr[1][1] == 0 || arr[1][1] == 3)) {
                                                arr[1][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 4) && (arr[2][1] == 0 || arr[2][1] == 3)) {
                                                arr[2][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 4) && (arr[3][1] == 0 || arr[3][1] == 3)) {
                                                arr[3][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 4) && (arr[4][1] == 0 || arr[4][1] == 3)) {
                                                arr[4][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 4) && (arr[0][1] == 0 || arr[0][1] == 3)) {
                                                arr[0][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 4) && (arr[1][1] == 0 || arr[1][1] == 3)) {
                                                arr[1][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 4) && (arr[2][1] == 0 || arr[2][1] == 3)) {
                                                arr[2][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 4) && (arr[3][1] == 0 || arr[3][1] == 3)) {
                                                arr[3][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 4) && (arr[4][1] == 0 || arr[4][1] == 3)) {
                                                arr[4][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 0) && (arr[0][3] == 0 || arr[0][3] == 3)) {
                                                arr[0][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 0) && (arr[1][3] == 0 || arr[1][3] == 3)) {
                                                arr[1][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 0) && (arr[2][3] == 0 || arr[2][3] == 3)) {
                                                arr[2][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 0) && (arr[3][3] == 0 || arr[3][3] == 3)) {
                                                arr[3][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 0 && y1 == 0) && (arr[4][3] == 0 || arr[4][3] == 3)) {
                                                arr[4][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 0) && (arr[0][3] == 0 || arr[0][3] == 3)) {
                                                arr[0][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 0) && (arr[1][3] == 0 || arr[1][3] == 3)) {
                                                arr[1][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 0) && (arr[2][3] == 0 || arr[2][3] == 3)) {
                                                arr[2][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 0) && (arr[3][3] == 0 || arr[3][3] == 3)) {
                                                arr[3][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 1 && y1 == 0) && (arr[4][3] == 0 || arr[4][3] == 3)) {
                                                arr[4][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 0) && (arr[0][3] == 0 || arr[0][3] == 3)) {
                                                arr[0][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 0) && (arr[1][3] == 0 || arr[1][3] == 3)) {
                                                arr[1][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 0) && (arr[2][3] == 0 || arr[2][3] == 3)) {
                                                arr[2][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 0) && (arr[3][3] == 0 || arr[3][3] == 3)) {
                                                arr[3][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 2 && y1 == 0) && (arr[4][3] == 0 || arr[4][3] == 3)) {
                                                arr[4][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 0) && (arr[0][3] == 0 || arr[0][3] == 3)) {
                                                arr[0][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 0) && (arr[1][3] == 0 || arr[1][3] == 3)) {
                                                arr[1][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 0) && (arr[2][3] == 0 || arr[2][3] == 3)) {
                                                arr[2][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 0) && (arr[3][3] == 0 || arr[3][3] == 3)) {
                                                arr[3][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 3 && y1 == 0) && (arr[4][3] == 0 || arr[4][3] == 3)) {
                                                arr[4][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 0) && (arr[0][3] == 0 || arr[0][3] == 3)) {
                                                arr[0][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 0) && (arr[1][3] == 0 || arr[1][3] == 3)) {
                                                arr[1][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 0) && (arr[2][3] == 0 || arr[2][3] == 3)) {
                                                arr[2][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 0) && (arr[3][3] == 0 || arr[3][3] == 3)) {
                                                arr[3][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 2) && (x1 == 4 && y1 == 0) && (arr[4][3] == 0 || arr[4][3] == 3)) {
                                                arr[4][3] = 0;
                                            }
                                        }

                                        //Check for Red beads
                                        if (move % 2 != 0) {
                                            // For Upward check
                                            if ((x1 != -1 && y1 != -1) && (arr[x1][y1] == 1) && (arr[x1 - 1][y1] == 0)) {
                                                arr[x1 - 1][y1] = 3;
                                            }
                                            // For Downward check
                                            if ((x1 != -1 && y1 != -1) && (arr[x1][y1] == 1) && (arr[x1 + 1][y1] == 0)) {
                                                arr[x1 + 1][y1] = 3;
                                            }
                                            // For Right check
                                            if ((x1 != -1 && y1 != -1) && (arr[x1][y1] == 1) && (arr[x1][y1 + 1] == 0)) {
                                                arr[x1][y1 + 1] = 3;
                                            }
                                            // For Left check
                                            if ((x1 != -1 && y1 != -1) && (arr[x1][y1] == 1) && (arr[x1][y1 - 1] == 0)) {
                                                arr[x1][y1 - 1] = 3;
                                            }
                                            //Check for elimination by upward move
                                            if ((x1 != -1 && y1 != -1) && (arr[x1][y1] == 1) && (arr[x1 - 1][y1] == 2) && (arr[x1 - 2][y1] == 0)) {
                                                arr[x1 - 2][y1] = 3;
                                            }
                                            //Check for elimination by downward move
                                            if ((x1 != -1 && y1 != -1) && (arr[x1][y1] == 1) && (arr[x1 + 1][y1] == 2) && (arr[x1 + 2][y1] == 0)) {
                                                arr[x1 + 2][y1] = 3;
                                            }
                                            //Check for elimination by right move
                                            if ((x1 != -1 && y1 != -1) && (arr[x1][y1] == 1) && (arr[x1][y1 + 1] == 2) && (arr[x1][y1 + 2] == 0)) {
                                                arr[x1][y1 + 2] = 3;
                                            }
                                            //Check for elimination by left move
                                            if ((x1 != -1 && y1 != -1) && (arr[x1][y1] == 1) && (arr[x1][y1 - 1] == 2) && (arr[x1][y1 - 2] == 0)) {
                                                arr[x1][y1 - 2] = 3;
                                            }


                                            //Check for diagonal movement
                                            if ((x1 % 2 == 0 && y1 % 2 == 0) || (x1 % 2 == 1 && y1 % 2 == 1)) {
                                                //Check for top right
                                                if ((arr[x1][y1] == 1) && (arr[x1 - 1][y1 + 1] == 0)) {
                                                    arr[x1 - 1][y1 + 1] = 3;
                                                }
                                                //Check for top left
                                                if ((arr[x1][y1] == 1) && (arr[x1 - 1][y1 - 1] == 0)) {
                                                    arr[x1 - 1][y1 - 1] = 3;
                                                }
                                                //Check for bottom left
                                                if ((arr[x1][y1] == 1) && (arr[x1 + 1][y1 - 1] == 0)) {
                                                    arr[x1 + 1][y1 - 1] = 3;
                                                }
                                                //Check for bottom right
                                                if ((arr[x1][y1] == 1) && (arr[x1 + 1][y1 + 1] == 0)) {
                                                    arr[x1 + 1][y1 + 1] = 3;
                                                }

                                                //Elimination by top right movement
                                                if ((arr[x1][y1] == 1) && (arr[x1 - 1][y1 + 1] == 2) && (arr[x1 - 2][y1 + 2] == 0)) {
                                                    arr[x1 - 2][y1 + 2] = 3;
                                                }
                                                //Elimination by top left movement
                                                if ((arr[x1][y1] == 1) && (arr[x1 - 1][y1 - 1] == 2) && (arr[x1 - 2][y1 - 2] == 0)) {
                                                    arr[x1 - 2][y1 - 2] = 3;
                                                }
                                                //Elimination by bottom left movement
                                                if ((arr[x1][y1] == 1) && (arr[x1 + 1][y1 - 1] == 2) && (arr[x1 + 2][y1 - 2] == 0)) {
                                                    arr[x1 + 2][y1 - 2] = 3;
                                                }
                                                //Elimination by bottom right movement
                                                if ((arr[x1][y1] == 1) && (arr[x1 + 1][y1 + 1] == 2) && (arr[x1 + 2][y1 + 2] == 0)) {
                                                    arr[x1 + 2][y1 + 2] = 3;
                                                }


                                            }

                                            //Special Handling
                                            if (arr[x1][y1] == 1 && (x1 == 0 && y1 == 0) && (arr[0][4] == 0 || arr[0][4] == 3)) {
                                                arr[0][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 4) && (arr[0][0] == 0 || arr[0][0] == 3)) {
                                                arr[0][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 4) && (arr[1][0] == 0 || arr[1][0] == 3)) {
                                                arr[1][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 4) && (arr[2][0] == 0 || arr[2][0] == 3)) {
                                                arr[2][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 4) && (arr[2][0] == 0 || arr[2][0] == 3)) {
                                                arr[2][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 0) && (arr[0][4] == 0 || arr[0][4] == 3)) {
                                                arr[0][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 0) && (arr[0][4] == 0 || arr[0][4] == 3)) {
                                                arr[0][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 0) && (arr[1][4] == 0 || arr[1][4] == 3)) {
                                                arr[1][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 0) && (arr[2][4] == 0 || arr[2][4] == 3)) {
                                                arr[2][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 4) && (arr[2][0] == 0 || arr[2][0] == 3)) {
                                                arr[2][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 4) && (arr[3][0] == 0 || arr[3][0] == 3)) {
                                                arr[3][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 4) && (arr[4][0] == 0 || arr[4][0] == 3)) {
                                                arr[4][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 4) && (arr[2][0] == 0 || arr[2][0] == 3)) {
                                                arr[2][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 4) && (arr[4][0] == 0 || arr[4][0] == 3)) {
                                                arr[4][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 0) && (arr[2][4] == 0 || arr[2][4] == 3)) {
                                                arr[2][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 0) && (arr[2][4] == 0 || arr[2][4] == 3)) {
                                                arr[2][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 0) && (arr[3][4] == 0 || arr[3][4] == 3)) {
                                                arr[3][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 0) && (arr[4][4] == 0 || arr[4][4] == 3)) {
                                                arr[4][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 4) && (arr[4][0] == 0 || arr[4][0] == 3)) {
                                                arr[4][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 4) && (arr[1][1] == 0 || arr[1][1] == 3)) {
                                                arr[1][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 4) && (arr[3][1] == 0 || arr[3][1] == 3)) {
                                                arr[3][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 1) && (arr[1][4] == 0 || arr[1][4] == 3)) {
                                                arr[1][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 1) && (arr[2][4] == 0 || arr[2][4] == 3)) {
                                                arr[2][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 1) && (arr[3][4] == 0 || arr[3][4] == 3)) {
                                                arr[3][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 3) && (arr[1][0] == 0 || arr[1][0] == 3)) {
                                                arr[1][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 3) && (arr[2][0] == 0 || arr[2][0] == 3)) {
                                                arr[2][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 3) && (arr[3][0] == 0 || arr[3][0] == 3)) {
                                                arr[3][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 3) && (arr[0][0] == 0 || arr[0][0] == 3)) {
                                                arr[0][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 3) && (arr[4][0] == 0 || arr[4][0] == 3)) {
                                                arr[4][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 1) && (arr[0][4] == 0 || arr[0][4] == 3)) {
                                                arr[0][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 1) && (arr[4][4] == 0 || arr[4][4] == 3)) {
                                                arr[4][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 3) && (arr[1][0] == 0 || arr[1][0] == 3)) {
                                                arr[1][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 3) && (arr[2][0] == 0 || arr[2][0] == 3)) {
                                                arr[2][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 3) && (arr[3][0] == 0 || arr[3][0] == 3)) {
                                                arr[3][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 3) && (arr[0][0] == 0 || arr[0][0] == 3)) {
                                                arr[0][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 3) && (arr[4][0] == 0 || arr[4][0] == 3)) {
                                                arr[4][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 3) && (arr[1][0] == 0 || arr[1][0] == 3)) {
                                                arr[1][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 3) && (arr[2][0] == 0 || arr[2][0] == 3)) {
                                                arr[2][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 3) && (arr[3][0] == 0 || arr[3][0] == 3)) {
                                                arr[3][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 3) && (arr[0][0] == 0 || arr[0][0] == 3)) {
                                                arr[0][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 3) && (arr[4][0] == 0 || arr[4][0] == 3)) {
                                                arr[4][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 3) && (arr[1][0] == 0 || arr[1][0] == 3)) {
                                                arr[1][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 3) && (arr[2][0] == 0 || arr[2][0] == 3)) {
                                                arr[2][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 3) && (arr[3][0] == 0 || arr[3][0] == 3)) {
                                                arr[3][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 3) && (arr[0][0] == 0 || arr[0][0] == 3)) {
                                                arr[0][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 3) && (arr[4][0] == 0 || arr[4][0] == 3)) {
                                                arr[4][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 3) && (arr[1][0] == 0 || arr[1][0] == 3)) {
                                                arr[1][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 3) && (arr[2][0] == 0 || arr[2][0] == 3)) {
                                                arr[2][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 3) && (arr[3][0] == 0 || arr[3][0] == 3)) {
                                                arr[3][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 3) && (arr[0][0] == 0 || arr[0][0] == 3)) {
                                                arr[0][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 3) && (arr[4][0] == 0 || arr[4][0] == 3)) {
                                                arr[4][0] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 1) && (arr[0][4] == 0 || arr[0][4] == 3)) {
                                                arr[0][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 1) && (arr[1][4] == 0 || arr[1][4] == 3)) {
                                                arr[1][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 1) && (arr[2][4] == 0 || arr[2][4] == 3)) {
                                                arr[2][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 1) && (arr[3][4] == 0 || arr[3][4] == 3)) {
                                                arr[3][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 1) && (arr[4][4] == 0 || arr[4][4] == 3)) {
                                                arr[4][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 1) && (arr[0][4] == 0 || arr[0][4] == 3)) {
                                                arr[0][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 1) && (arr[1][4] == 0 || arr[1][4] == 3)) {
                                                arr[1][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 1) && (arr[2][4] == 0 || arr[2][4] == 3)) {
                                                arr[2][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 1) && (arr[3][4] == 0 || arr[3][4] == 3)) {
                                                arr[3][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 1) && (arr[4][4] == 0 || arr[4][4] == 3)) {
                                                arr[4][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 1) && (arr[0][4] == 0 || arr[0][4] == 3)) {
                                                arr[0][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 1) && (arr[1][4] == 0 || arr[1][4] == 3)) {
                                                arr[1][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 1) && (arr[2][4] == 0 || arr[2][4] == 3)) {
                                                arr[2][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 1) && (arr[3][4] == 0 || arr[3][4] == 3)) {
                                                arr[3][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 1) && (arr[4][4] == 0 || arr[4][4] == 3)) {
                                                arr[4][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 1) && (arr[0][4] == 0 || arr[0][4] == 3)) {
                                                arr[0][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 1) && (arr[1][4] == 0 || arr[1][4] == 3)) {
                                                arr[1][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 1) && (arr[2][4] == 0 || arr[2][4] == 3)) {
                                                arr[2][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 1) && (arr[3][4] == 0 || arr[3][4] == 3)) {
                                                arr[3][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 1) && (arr[4][4] == 0 || arr[4][4] == 3)) {
                                                arr[4][4] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 4) && (arr[0][1] == 0 || arr[0][1] == 3)) {
                                                arr[0][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 4) && (arr[1][1] == 0 || arr[1][1] == 3)) {
                                                arr[1][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 4) && (arr[2][1] == 0 || arr[2][1] == 3)) {
                                                arr[2][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 4) && (arr[3][1] == 0 || arr[3][1] == 3)) {
                                                arr[3][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 4) && (arr[4][1] == 0 || arr[4][1] == 3)) {
                                                arr[4][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 4) && (arr[0][1] == 0 || arr[0][1] == 3)) {
                                                arr[0][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 4) && (arr[1][1] == 0 || arr[1][1] == 3)) {
                                                arr[1][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 4) && (arr[2][1] == 0 || arr[2][1] == 3)) {
                                                arr[2][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 4) && (arr[3][1] == 0 || arr[3][1] == 3)) {
                                                arr[3][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 4) && (arr[4][1] == 0 || arr[4][1] == 3)) {
                                                arr[4][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 4) && (arr[0][1] == 0 || arr[0][1] == 3)) {
                                                arr[0][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 4) && (arr[1][1] == 0 || arr[1][1] == 3)) {
                                                arr[1][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 4) && (arr[2][1] == 0 || arr[2][1] == 3)) {
                                                arr[2][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 4) && (arr[3][1] == 0 || arr[3][1] == 3)) {
                                                arr[3][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 4) && (arr[4][1] == 0 || arr[4][1] == 3)) {
                                                arr[4][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 4) && (arr[0][1] == 0 || arr[0][1] == 3)) {
                                                arr[0][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 4) && (arr[1][1] == 0 || arr[1][1] == 3)) {
                                                arr[1][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 4) && (arr[2][1] == 0 || arr[2][1] == 3)) {
                                                arr[2][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 4) && (arr[3][1] == 0 || arr[3][1] == 3)) {
                                                arr[3][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 4) && (arr[4][1] == 0 || arr[4][1] == 3)) {
                                                arr[4][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 4) && (arr[0][1] == 0 || arr[0][1] == 3)) {
                                                arr[0][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 4) && (arr[1][1] == 0 || arr[1][1] == 3)) {
                                                arr[1][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 4) && (arr[2][1] == 0 || arr[2][1] == 3)) {
                                                arr[2][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 4) && (arr[3][1] == 0 || arr[3][1] == 3)) {
                                                arr[3][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 4) && (arr[4][1] == 0 || arr[4][1] == 3)) {
                                                arr[4][1] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 0) && (arr[0][3] == 0 || arr[0][3] == 3)) {
                                                arr[0][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 0) && (arr[1][3] == 0 || arr[1][3] == 3)) {
                                                arr[1][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 0) && (arr[2][3] == 0 || arr[2][3] == 3)) {
                                                arr[2][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 0) && (arr[3][3] == 0 || arr[3][3] == 3)) {
                                                arr[3][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 0 && y1 == 0) && (arr[4][3] == 0 || arr[4][3] == 3)) {
                                                arr[4][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 0) && (arr[0][3] == 0 || arr[0][3] == 3)) {
                                                arr[0][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 0) && (arr[1][3] == 0 || arr[1][3] == 3)) {
                                                arr[1][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 0) && (arr[2][3] == 0 || arr[2][3] == 3)) {
                                                arr[2][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 0) && (arr[3][3] == 0 || arr[3][3] == 3)) {
                                                arr[3][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 1 && y1 == 0) && (arr[4][3] == 0 || arr[4][3] == 3)) {
                                                arr[4][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 0) && (arr[0][3] == 0 || arr[0][3] == 3)) {
                                                arr[0][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 0) && (arr[1][3] == 0 || arr[1][3] == 3)) {
                                                arr[1][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 0) && (arr[2][3] == 0 || arr[2][3] == 3)) {
                                                arr[2][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 0) && (arr[3][3] == 0 || arr[3][3] == 3)) {
                                                arr[3][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 2 && y1 == 0) && (arr[4][3] == 0 || arr[4][3] == 3)) {
                                                arr[4][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 0) && (arr[0][3] == 0 || arr[0][3] == 3)) {
                                                arr[0][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 0) && (arr[1][3] == 0 || arr[1][3] == 3)) {
                                                arr[1][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 0) && (arr[2][3] == 0 || arr[2][3] == 3)) {
                                                arr[2][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 0) && (arr[3][3] == 0 || arr[3][3] == 3)) {
                                                arr[3][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 3 && y1 == 0) && (arr[4][3] == 0 || arr[4][3] == 3)) {
                                                arr[4][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 0) && (arr[0][3] == 0 || arr[0][3] == 3)) {
                                                arr[0][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 0) && (arr[1][3] == 0 || arr[1][3] == 3)) {
                                                arr[1][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 0) && (arr[2][3] == 0 || arr[2][3] == 3)) {
                                                arr[2][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 0) && (arr[3][3] == 0 || arr[3][3] == 3)) {
                                                arr[3][3] = 0;
                                            }
                                            if ((arr[x1][y1] == 1) && (x1 == 4 && y1 == 0) && (arr[4][3] == 0 || arr[4][3] == 3)) {
                                                arr[4][3] = 0;
                                            }
                                        }
                                    }
                                    else if (x2 == -1 && y2 == -1) {
                                        x2 = i;
                                        y2 = j;
                                        //Check for Green Beads
                                        if (move % 2 == 0) {
                                            //For Upward Check
                                            if ((x2 != -1 && y2 != -1) && (arr[x1][y1] == 2) && (arr[x1 - 1][y1] == 3)) {
                                                arr[x1 - 1][y1] = 0;
                                            }
                                            // For Downward check
                                            if ((x2 != -1 && y2 != -1) && (arr[x1][y1] == 2) && (arr[x1 + 1][y1] == 3)) {
                                                arr[x1 + 1][y1] = 0;
                                            }
                                            // For Right check
                                            if ((x2 != -1 && y2 != -1) && (arr[x1][y1] == 2) && (arr[x1][y1 + 1] == 3)) {
                                                arr[x1][y1 + 1] = 0;
                                            }
                                            // For Left check
                                            if ((x2 != -1 && y2 != -1) && (arr[x1][y1] == 2) && (arr[x1][y1 - 1] == 3)) {
                                                arr[x1][y1 - 1] = 0;
                                            }
                                            //Check for elimination by upward move
                                            if ((x2 != -1 && y2 != -1) && (arr[x1][y1] == 2) && (arr[x1 - 1][y1] == 1) && (arr[x1 - 2][y1] == 3)) {
                                                arr[x1 - 2][y1] = 0;
                                            }
                                            //Check for elimination by downward move
                                            if ((x2 != -1 && y2 != -1) && (arr[x1][y1] == 2) && (arr[x1 + 1][y1] == 1) && (arr[x1 + 2][y1] == 3)) {
                                                arr[x1 + 2][y1] = 0;
                                            }
                                            //Check for elimination by right move
                                            if ((x2 != -1 && y2 != -1) && (arr[x1][y1] == 2) && (arr[x1][y1 + 1] == 1) && (arr[x1][y1 + 2] == 3)) {
                                                arr[x1][y1 + 2] = 0;
                                            }
                                            //Check for elimination by left move
                                            if ((x2 != -1 && y2 != -1) && (arr[x1][y1] == 2) && (arr[x1][y1 - 1] == 1) && (arr[x1][y1 - 2] == 3)) {
                                                arr[x1][y1 - 2] = 0;
                                            }
                                            //Check for diagonal movement
                                            if ((x1 % 2 == 0 && y1 % 2 == 0) || (x1 % 2 == 1 && y1 % 2 == 1)) {
                                                //Check for top right
                                                if ((arr[x1][y1] == 2) && (arr[x1 - 1][y1 + 1] == 3)) {
                                                    arr[x1 - 1][y1 + 1] = 0;
                                                }
                                                //Check for top left
                                                if ((arr[x1][y1] == 2) && (arr[x1 - 1][y1 - 1] == 3)) {
                                                    arr[x1 - 1][y1 - 1] = 0;
                                                }
                                                //Check for bottom left
                                                if ((arr[x1][y1] == 2) && (arr[x1 + 1][y1 - 1] == 3)) {
                                                    arr[x1 + 1][y1 - 1] = 0;
                                                }
                                                //Check for top right
                                                if ((arr[x1][y1] == 2) && (arr[x1 + 1][y1 + 1] == 3)) {
                                                    arr[x1 + 1][y1 + 1] = 0;
                                                }

                                                //Elimination by top right movement
                                                if ((arr[x1][y1] == 2) && (arr[x1 - 1][y1 + 1] == 1) && (arr[x1 - 2][y1 + 2] == 3)) {
                                                    arr[x1 - 2][y1 + 2] = 0;
                                                }
                                                //Elimination by top left movement
                                                if ((arr[x1][y1] == 2) && (arr[x1 - 1][y1 - 1] == 1) && (arr[x1 - 2][y1 - 2] == 3)) {
                                                    arr[x1 - 2][y1 - 2] = 0;
                                                }
                                                //Elimination by bottom left movement
                                                if ((arr[x1][y1] == 2) && (arr[x1 + 1][y1 - 1] == 1) && (arr[x1 + 2][y1 - 2] == 3)) {
                                                    arr[x1 + 2][y1 - 2] = 0;
                                                }
                                                //Elimination by bottom right movement
                                                if ((arr[x1][y1] == 2) && (arr[x1 + 1][y1 + 1] == 1) && (arr[x1 + 2][y1 + 2] == 3)) {
                                                    arr[x1 + 2][y1 + 2] = 0;
                                                }

                                            }
                                        }

                                        //Check for Red Beads
                                        if (move % 2 != 0) {
                                            //For Upward Check
                                            if ((x2 != -1 && y2 != -1) && (arr[x1][y1] == 1) && (arr[x1 - 1][y1] == 3)) {
                                                arr[x1 - 1][y1] = 0;
                                            }
                                            // For Downward check
                                            if ((x2 != -1 && y2 != -1) && (arr[x1][y1] == 1) && (arr[x1 + 1][y1] == 3)) {
                                                arr[x1 + 1][y1] = 0;
                                            }
                                            // For Right check
                                            if ((x2 != -1 && y2 != -1) && (arr[x1][y1] == 1) && (arr[x1][y1 + 1] == 3)) {
                                                arr[x1][y1 + 1] = 0;
                                            }
                                            // For Left check
                                            if ((x2 != -1 && y2 != -1) && (arr[x1][y1] == 1) && (arr[x1][y1 - 1] == 3)) {
                                                arr[x1][y1 - 1] = 0;
                                            }
                                            //Check for elimination by upward move
                                            if ((x2 != -1 && y2 != -1) && (arr[x1][y1] == 1) && (arr[x1 - 1][y1] == 2) && (arr[x1 - 2][y1] == 3)) {
                                                arr[x1 - 2][y1] = 0;
                                            }
                                            //Check for elimination by downward move
                                            if ((x2 != -1 && y2 != -1) && (arr[x1][y1] == 1) && (arr[x1 + 1][y1] == 2) && (arr[x1 + 2][y1] == 3)) {
                                                arr[x1 + 2][y1] = 0;
                                            }
                                            //Check for elimination by right move
                                            if ((x2 != -1 && y2 != -1) && (arr[x1][y1] == 1) && (arr[x1][y1 + 1] == 2) && (arr[x1][y1 + 2] == 3)) {
                                                arr[x1][y1 + 2] = 0;
                                            }
                                            //Check for elimination by left move
                                            if ((x2 != -1 && y2 != -1) && (arr[x1][y1] == 1) && (arr[x1][y1 - 1] == 2) && (arr[x1][y1 - 2] == 3)) {
                                                arr[x1][y1 - 2] = 0;
                                            }
                                            //Check for diagonal movement
                                            if ((x1 % 2 == 0 && y1 % 2 == 0) || (x1 % 2 == 1 && y1 % 2 == 1)) {
                                                //Check for top right
                                                if ((arr[x1][y1] == 1) && (arr[x1 - 1][y1 + 1] == 3)) {
                                                    arr[x1 - 1][y1 + 1] = 0;
                                                }
                                                //Check for top left
                                                if ((arr[x1][y1] == 1) && (arr[x1 - 1][y1 - 1] == 3)) {
                                                    arr[x1 - 1][y1 - 1] = 0;
                                                }
                                                //Check for bottom left
                                                if ((arr[x1][y1] == 1) && (arr[x1 + 1][y1 - 1] == 3)) {
                                                    arr[x1 + 1][y1 - 1] = 0;
                                                }
                                                //Check for top right
                                                if ((arr[x1][y1] == 1) && (arr[x1 + 1][y1 + 1] == 3)) {
                                                    arr[x1 + 1][y1 + 1] = 0;
                                                }

                                                //Elimination by top right movement
                                                if ((arr[x1][y1] == 1) && (arr[x1 - 1][y1 + 1] == 2) && (arr[x1 - 2][y1 + 2] == 3)) {
                                                    arr[x1 - 2][y1 + 2] = 0;
                                                }
                                                //Elimination by top left movement
                                                if ((arr[x1][y1] == 1) && (arr[x1 - 1][y1 - 1] == 2) && (arr[x1 - 2][y1 - 2] == 3)) {
                                                    arr[x1 - 2][y1 - 2] = 0;
                                                }
                                                //Elimination by bottom left movement
                                                if ((arr[x1][y1] == 1) && (arr[x1 + 1][y1 - 1] == 2) && (arr[x1 + 2][y1 - 2] == 3)) {
                                                    arr[x1 + 2][y1 - 2] = 0;
                                                }
                                                //Elimination by bottom right movement
                                                if ((arr[x1][y1] == 1) && (arr[x1 + 1][y1 + 1] == 2) && (arr[x1 + 2][y1 + 2] == 3)) {
                                                    arr[x1 + 2][y1 + 2] = 0;
                                                }

                                            }
                                        }

                                        // Movements of Red Beads
                                        if (move % 2 != 0) {
                                            // Vertical and Horizontal Movement of Red Beads
                                            if ((arr[x1][y1] == 1 && arr[x2][y2] == 0) && ((x1 == x2 && y1 == y2 - 1) ||
                                                (x1 == x2 + 1 && y1 == y2) || (x1 == x2 && y1 == y2 + 1) || (x1 == x2 - 1 && y1 == y2))) {

                                                swap(arr[x1][y1], arr[x2][y2]);
                                                move++;
                                                movement.play();
                                            }
                                            // Eliminating Green Beads by Vertical and Horizontal Movement
                                            else if ((arr[x1][y1] == 1 && arr[x2][y2] == 0 && arr[(x1 + x2) / 2][(y1 + y2) / 2] == 2) &&
                                                ((x1 == x2 && y1 == y2 - 2) || (x1 == x2 + 2 && y1 == y2) || (x1 == x2 && y1 == y2 + 2) || (x1 == x2 - 2 && y1 == y2))) {

                                                arr[(x1 + x2) / 2][(y1 + y2) / 2] = 0;
                                                swap(arr[x1][y1], arr[x2][y2]);
                                                move++;
                                                greenbeads--;
                                                eliminate.play();
                                            }
                                            // Diagonal movement of Red Beads
                                            else if ((x1 % 2 == 0 && y1 % 2 == 0) || (x1 % 2 == 1 && y1 % 2 == 1)) {
                                                if ((arr[x1][y1] == 1 && arr[x2][y2] == 0) && ((x1 == x2 + 1 && y1 == y2 - 1) ||
                                                    (x1 == x2 + 1 && y1 == y2 + 1) || (x1 == x2 - 1 && y1 == y2 + 1) || (x1 == x2 - 1 && y1 == y2 - 1))) {

                                                    swap(arr[x1][y1], arr[x2][y2]);
                                                    move++;
                                                    movement.play();
                                                }
                                                // Eliminating Green Beads by Diagonal Movement
                                                else if ((arr[x1][y1] == 1 && arr[x2][y2] == 0 && arr[(x1 + x2) / 2][(y1 + y2) / 2] == 2) &&
                                                    ((x1 == x2 + 2 && y1 == y2 - 2) || (x1 == x2 + 2 && y1 == y2 + 2) || (x1 == x2 - 2 && y1 == y2 + 2) ||
                                                        (x1 == x2 - 2 && y1 == y2 - 2))) {

                                                    arr[(x1 + x2) / 2][(y1 + y2) / 2] = 0;
                                                    swap(arr[x1][y1], arr[x2][y2]);
                                                    move++;
                                                    greenbeads--;
                                                    eliminate.play();
                                                }
                                                else {
                                                    cout << "Invalid Move." << endl;
                                                    in.play();
                                                }
                                            }
                                            else {
                                                cout << "Invalid Move." << endl;
                                                in.play();
                                            }
                                        }

                                        // Movements of Green Beads
                                        else if (move % 2 == 0) {
                                            // Vertical and Horizontal Movement of Green Beads
                                            if ((arr[x1][y1] == 2 && arr[x2][y2] == 0) && ((x1 == x2 && y1 == y2 - 1) || (x1 == x2 + 1 && y1 == y2) ||
                                                (x1 == x2 && y1 == y2 + 1) || (x1 == x2 - 1 && y1 == y2))) {

                                                swap(arr[x1][y1], arr[x2][y2]);
                                                move++;
                                                movement.play();
                                            }
                                            // Eliminating Red Beads by Vertical and Horizontal Movement
                                            else if ((arr[x1][y1] == 2 && arr[x2][y2] == 0 && arr[(x1 + x2) / 2][(y1 + y2) / 2] == 1) &&
                                                ((x1 == x2 && y1 == y2 - 2) || (x1 == x2 + 2 && y1 == y2) || (x1 == x2 && y1 == y2 + 2) ||
                                                    (x1 == x2 - 2 && y1 == y2))) {

                                                arr[(x1 + x2) / 2][(y1 + y2) / 2] = 0;
                                                swap(arr[x1][y1], arr[x2][y2]);
                                                move++;
                                                redbeads--;
                                                eliminate.play();
                                            }
                                            // Diagonal movement of Green Beads
                                            else if ((x1 % 2 == 0 && y1 % 2 == 0) || (x1 % 2 == 1 && y1 % 2 == 1)) {
                                                if ((arr[x1][y1] == 2 && arr[x2][y2] == 0) && ((x1 == x2 + 1 && y1 == y2 - 1) ||
                                                    (x1 == x2 + 1 && y1 == y2 + 1) || (x1 == x2 - 1 && y1 == y2 + 1) || (x1 == x2 - 1 && y1 == y2 - 1))) {

                                                    swap(arr[x1][y1], arr[x2][y2]);
                                                    move++;
                                                    movement.play();
                                                }
                                                // Eliminating Red Beads by Diagonal Movement
                                                else if ((arr[x1][y1] == 2 && arr[x2][y2] == 0 && arr[(x1 + x2) / 2][(y1 + y2) / 2] == 1) &&
                                                    ((x1 == x2 + 2 && y1 == y2 - 2) || (x1 == x2 + 2 && y1 == y2 + 2) ||
                                                        (x1 == x2 - 2 && y1 == y2 + 2) || (x1 == x2 - 2 && y1 == y2 - 2))) {

                                                    arr[(x1 + x2) / 2][(y1 + y2) / 2] = 0;
                                                    swap(arr[x1][y1], arr[x2][y2]);
                                                    move++;
                                                    redbeads--;
                                                    eliminate.play();
                                                }
                                                else {
                                                    cout << "Invalid Move." << endl;
                                                    in.play();
                                                }
                                            }
                                            else {
                                                cout << "Invalid Move." << endl;
                                                in.play();
                                            }
                                        }
                                        else {
                                            cout << "Invalid Move." << endl;
                                            in.play();
                                        }

                                        x1 = -1;
                                        y1 = -1;
                                        x2 = -1;
                                        y2 = -1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        bool red = false;
        bool green = false;
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (arr[i][j] == 1) {
                    red = true;
                    end_sound = 0;
                }
                if (arr[i][j] == 2) {
                    green = true;
                    end_sound = 1;
                }

            }
        }
        if (!red) {
            window.clear(Color::White);
            window.draw(win2);
            window.display();
        }
        else if (!green) {
            window.clear(Color::White);
            window.draw(win1);
            window.display();
        }

        else {
            window.clear();
            if (starting == 0) {
                window.clear(Color::White);
                window.draw(logoBackgroundSprite);
                window.draw(main);
                window.draw(exit);
                window.draw(resume);
            }
            else {

                window.draw(backgroundSprite);

                for (int i = 0; i < 5; i++) {
                    for (int j = 0; j < 5; j++) {
                        CircleShape circle(radius);
                        circle.setPosition(169.5 * j + 111, 169.5 * i + 111);
                        if (arr[i][j] == 1) {
                            circle.setFillColor(Color::Red);
                            circle.setOutlineColor(Color(150, 0, 0, 255));
                            circle.setOutlineThickness(8);
                        }
                        else if (arr[i][j] == 2) {
                            circle.setFillColor(Color::Green);
                            circle.setOutlineColor(Color(0, 150, 0, 255));
                            circle.setOutlineThickness(8);
                        }
                        else if (arr[i][j] == 0) {
                            circle.setFillColor(Color(0, 0, 0, 0));
                            circle.setOutlineColor(Color(0, 0, 0, 0));
                            circle.setOutlineThickness(8);
                        }
                        else if (arr[i][j] == 3) {
                            circle.setFillColor(Color(0, 0, 255, 77));
                            circle.setOutlineColor(Color(0, 0, 255, 77));
                            circle.setOutlineThickness(8);
                        }
                        window.draw(circle);
                    }
                }
                if (move % 2 == 0) {
                    window.draw(greenturnsprite);
                }
                else if (move % 2 == 1) {
                    window.draw(redturnsprite);
                }
                if (isPaused) {
                    window.draw(pauseSprite);

                }
                else if (!isPaused) {
                    window.draw(pauseSprite2);

                }
            }
            window.display();
        }
    }
    return 0;
}
