#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "Pipe.h"
#include "CompressorStation.h"

// Структура ребра графа (Соединение)
struct Connection {
    int fromStationId;
    int toStationId;
    int pipeId;
};

class Manager {
private:
    std::unordered_map<int, Pipe> pipes;
    std::unordered_map<int, CompressorStation> stations;
    std::vector<Connection> network; 

    std::vector<int> findPipes();
    std::vector<int> findStations();

public:
    void addPipe();
    void addStation();
    void viewAll();
    void managePipes();
    void manageStations();
    void controlSingleStation();
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    void connectStations();
    void topologicalSort();
};