// persona.h
#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>
#include <fstream>
#include <cstdint>

using std::string;

class Player {
protected:
    string name;
    bool IsWin = false;
    unsigned int PlayedSeconds = 0;
    uint16_t day;
    uint16_t month;
    uint16_t year;
    
public:
    Player(const string& name) : name(name) {}
    string GetName() const { return name; }
    void SetWin(bool win) { IsWin = win; }
    void SetSeconds(unsigned int sec) { PlayedSeconds = sec; }
    void SetDate(uint16_t d, uint16_t m, uint16_t y) { day = d; month = m; year = y; }
    void SaveToFile(const string& filename){
        std::ofstream file(filename, std::ios::app);
        if (file.is_open()) {
            file << "Name: " << name << "\n";
            file << "Win: " << (IsWin ? "Yes" : "No") << "\n";
            file << "Played Seconds: " << PlayedSeconds << "\n";
            file << "Date: " << day << "/" << month << "/" << year << "\n";
            file << "--------------------------\n";
            file.close();
        } else {
            std::cerr << "Error trying to open file\n";
        }
    }
};

class ActualPlayer : public Player {
public:
    ActualPlayer(const std::string& name) : Player(name) {}
};

#endif