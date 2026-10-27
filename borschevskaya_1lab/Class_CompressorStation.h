#pragma once
#include <iostream>
#include <string>
#include <fstream>

class CompressorStation {
private:
    static int nextId;
    int id;
    std::string name;
    int numberOfWorkshops;
    int operatingWorkshops;
    int stationClass;

    void updateNextId();

public:
    CompressorStation();

    int getId() const;
    std::string getName() const;

    void input();
    void print() const;

    void startWorkshop();
    void stopWorkshop();

    void saveToFile(std::ofstream& file) const;
    bool loadFromFile(std::ifstream& file);

    double getIdlePercent() const;
    bool matchesName(const std::string& query) const;
    bool matchesIdlePercent(double minPercent) const;
};