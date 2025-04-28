#ifndef ROAD_H_INCLUDED
#define ROAD_H_INCLUDED
#include<fstream>
#include<iostream>
using namespace std;
void saveHighscore(int highscore) {
    ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highscore;
        file.close();
    }
}
int loadHighscore() {
    ifstream file("highscore.txt");
    int savedScore = 0;
    if (file.is_open()) {
        file >> savedScore;
        file.close();
    }
    return savedScore;
}


#endif // ROAD_H
