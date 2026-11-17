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
    void startWorkshop();
    void stopWorkshop();
    double getIdlePercent() const;
    bool matchesName(const std::string& query) const;
    bool matchesIdlePercent(double minPercent) const;

    friend std::ostream& operator<<(std::ostream& out, const CompressorStation& s);
    friend std::istream& operator>>(std::istream& in, CompressorStation& s);
};