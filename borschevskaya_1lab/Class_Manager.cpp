#include "Manager.h"
#include "Utils.h"
#include <iostream>

using namespace std;

void managePipes(map<int, Pipe>& pipes) {
    if (pipes.empty()) {
        cout << "Нет труб для работы.\n";
        return;
    }

    vector<int> foundIds = findPipes(pipes);
    if (foundIds.empty()) {
        cout << "Ничего не найдено.\n";
        return;
    }

    cout << "Найдено труб: " << foundIds.size() << "\n";
    for (int id : foundIds) {
        pipes.at(id).print();
    }

    int action = readInt("\nДействие:\n1-В ремонт\n2-Из ремонта\n3-Удалить\n0-Отмена\nВыбор: ");
    if (action == 0) return;

    int count = 0;
    for (int id : foundIds) {
        if (action == 1) { pipes.at(id).setRepairing(true); count++; }
        else if (action == 2) { pipes.at(id).setRepairing(false); count++; }
        else if (action == 3) { pipes.erase(id); count++; }
    }
    cout << "Обработано труб: " << count << "\n";
    cerr << "Пакетное управление трубами. Действие: " << action << ". Обработано: " << count << "\n";
}

void manageStations(map<int, CompressorStation>& stations) {
    if (stations.empty()) {
        cout << "Нет КС для работы.\n";
        return;
    }

    vector<int> foundIds = findStations(stations);
    if (foundIds.empty()) {
        cout << "Ничего не найдено.\n";
        return;
    }

    cout << "Найдено КС: " << foundIds.size() << "\n";
    for (int id : foundIds) {
        stations.at(id).print();
    }

    int action = readInt("\nДействие:\n1-Запустить цех\n2-Остановить цех\n3-Удалить\n0-Отмена\nВыбор: ");
    if (action == 0) return;

    int count = 0;
    for (int id : foundIds) {
        if (action == 1) { stations.at(id).startWorkshop(); count++; }
        else if (action == 2) { stations.at(id).stopWorkshop(); count++; }
        else if (action == 3) { stations.erase(id); count++; }
    }
    cout << "Обработано КС: " << count << "\n";
    cerr << "Пакетное управление КС. Действие: " << action << ". Обработано: " << count << "\n";
}

void controlSingleStation(map<int, CompressorStation>& stations) {
    if (stations.empty()) {
        cout << "Нет КС для управления.\n";
        return;
    }

    int id = readInt("Введите ID станции для управления: ");
    auto it = stations.find(id);

    if (it == stations.end()) {
        cout << "КС с таким ID не найдена.\n";
        return;
    }

    it->second.print();

    int action = readInt("1 - Запустить цех\n2 - Остановить цех\n0 - Отмена\nВыбор: ");
    if (action == 1) {
        it->second.startWorkshop();
        cerr << "Запущен цех на КС ID: " << id << "\n";
    }
    else if (action == 2) {
        it->second.stopWorkshop();
        cerr << "Остановлен цех на КС ID: " << id << "\n";
    }
    else if (action == 0) {
        cout << "Отмена.\n";
    }
    else {
        cout << "Неверный выбор.\n";
    }
}