#pragma once
#include <iostream>
#include <string>

class Pipe {
private:
    static int nextId;
    int id;
    std::string name;
    double length;
    int diameter; 
    bool repairing;
    bool isBusy;  

    void updateNextId();

public:
    Pipe();
    int getId() const;
    int getDiameter() const;
    bool isBusyPipe() const;
    void setBusy(bool busy);
    void setDiameter(int diam);
    void setName(const std::string& n);

    void input();
    bool matchesName(const std::string& query) const;
    bool matchesRepair(bool repairState) const;

    friend std::ostream& operator<<(std::ostream& out, const Pipe& p);
    friend std::istream& operator>>(std::istream& in, Pipe& p);
};