#include "Manager.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<int> Manager::findPipes() {
    vector<int> foundIds;
    int choice = readInt("Искать по названию-1, по ремонту-2: ");

    if (choice == 1) {
        cout << "Введите название: ";
        string query;
        getline(cin, query);
        for (const auto& pair : pipes) {
            if (pair.second.matchesName(query)) foundIds.push_back(pair.first);
        }
    }
    else if (choice == 2) {
        int state = readInt("Искать в ремонте? (1 - да, 0 - нет): ");
        for (const auto& pair : pipes) {
            if (pair.second.matchesRepair(state != 0)) foundIds.push_back(pair.first);
        }
    }
    return foundIds;
}

vector<int> Manager::findStations() {
    vector<int> foundIds;
    int choice = readInt("Искать по названию-1, по проценту простоя-2: ");

    if (choice == 1) {
        cout << "Введите название: ";
        string query;
        getline(cin, query);
        for (const auto& pair : stations) {
            if (pair.second.matchesName(query)) foundIds.push_back(pair.first);
        }
    }
    else if (choice == 2) {
        cout << "Пояснение: простой = (Всего - Работает) / Всего * 100%.\n";
        double percent = readDouble("Введите минимальный процент простоя (0-100): ");
        while (percent < 0 || percent > 100) {
            cout << "Ошибка! Введите число от 0 до 100: ";
            percent = readDouble("");
        }
        for (const auto& pair : stations) {
            if (pair.second.matchesIdlePercent(percent)) foundIds.push_back(pair.first);
        }
    }
    return foundIds;
}

void Manager::addPipe() {
    Pipe newPipe;
    newPipe.input();
    pipes.emplace(newPipe.getId(), newPipe);
    cout << "Труба добавлена (ID " << newPipe.getId() << ")\n";
    cerr << "Добавлена труба ID: " << newPipe.getId() << "\n";
}

void Manager::addStation() {
    CompressorStation newCs;
    newCs.input();
    stations.emplace(newCs.getId(), newCs);
    cout << "КС добавлена (ID " << newCs.getId() << ")\n";
    cerr << "Добавлена КС ID: " << newCs.getId() << "\n";
}

void Manager::viewAll() {
    if (pipes.empty() && stations.empty()) {
        cout << "Пока ничего не введено\n";
        return;
    }
    for (const auto& pair : pipes) cout << pair.second;      
    for (const auto& pair : stations) cout << pair.second;   
}

void Manager::managePipes() {
    if (pipes.empty()) { cout << "Нет труб.\n"; return; }
    vector<int> foundIds = findPipes();
    if (foundIds.empty()) { cout << "Ничего не найдено.\n"; return; }

    cout << "Найдено труб: " << foundIds.size() << "\n";
    for (int id : foundIds) cout << pipes.at(id);

    int action = readInt("\nДействие:\n1-В ремонт\n2-Из ремонта\n3-Удалить\n0-Отмена\nВыбор: ");
    if (action == 0) return;

    int count = 0;
    for (int id : foundIds) {
        if (action == 1) { pipes.at(id).setRepairing(true); count++; }
        else if (action == 2) { pipes.at(id).setRepairing(false); count++; }
        else if (action == 3) { pipes.erase(id); count++; }
    }
    cout << "Обработано труб: " << count << "\n";
    cerr << "Пакетное управление трубами. Обработано: " << count << "\n";
}

void Manager::manageStations() {
    if (stations.empty()) { cout << "Нет КС.\n"; return; }
    vector<int> foundIds = findStations();
    if (foundIds.empty()) { cout << "Ничего не найдено.\n"; return; }

    cout << "Найдено КС: " << foundIds.size() << "\n";
    for (int id : foundIds) cout << stations.at(id);

    int targetChoice = 1;
    if (foundIds.size() > 1) {
        cout << "\n1 - Применить ко ВСЕМ\n2 - Выбрать одну по ID\n0 - Отмена\nВыбор: ";
        targetChoice = readInt("");
        if (targetChoice == 0) return;
    }

    int action = readInt("\nДействие:\n1-Запустить цех\n2-Остановить цех\n3-Удалить\n0-Отмена\nВыбор: ");
    if (action == 0) return;

    if (targetChoice == 2) {
        int chosenId = readInt("Введите ID из списка: ");
        if (stations.find(chosenId) == stations.end()) { cout << "Нет такого ID.\n"; return; }
        if (action == 1) stations.at(chosenId).startWorkshop();
        else if (action == 2) stations.at(chosenId).stopWorkshop();
        else if (action == 3) stations.erase(chosenId);
    }
    else {
        int count = 0;
        for (int id : foundIds) {
            if (action == 1) { stations.at(id).startWorkshop(); count++; }
            else if (action == 2) { stations.at(id).stopWorkshop(); count++; }
            else if (action == 3) { stations.erase(id); count++; }
        }
        cout << "Обработано КС: " << count << "\n";
    }
}

void Manager::controlSingleStation() {
    if (stations.empty()) { cout << "Нет КС.\n"; return; }
    int id = readInt("Введите ID станции: ");
    if (stations.find(id) == stations.end()) { cout << "КС не найдена.\n"; return; }

    cout << stations.at(id);

    int action = readInt("1 - Запустить цех\n2 - Остановить цех\n0 - Отмена\nВыбор: ");
    if (action == 1) { stations.at(id).startWorkshop(); cerr << "Запущен цех ID: " << id << "\n"; }
    else if (action == 2) { stations.at(id).stopWorkshop(); cerr << "Остановлен цех ID: " << id << "\n"; }
}

void Manager::saveToFile(const string& filename) const {
    ofstream fout(filename);
    if (!fout.is_open()) { cout << "Ошибка открытия файла\n"; return; }

    fout << pipes.size() << "\n";
    for (const auto& pair : pipes) {
        fout << pair.second << "\n"; 
    }
    fout << stations.size() << "\n";
    for (const auto& pair : stations) {
        fout << pair.second << "\n";
    }
    cout << "Сохранено!\n";
    cerr << "Сохранено в файл: " << filename << "\n";
}

void Manager::loadFromFile(const string& filename) {
    ifstream fin(filename);
    if (!fin.is_open()) { cout << "Файл не найден\n"; return; }

    pipes.clear();
    stations.clear();

    int pCount;
    fin >> pCount;
    fin.ignore(10000, '\n');
    for (int i = 0; i < pCount; ++i) {
        Pipe p;
        fin >> p; 
        pipes.emplace(p.getId(), p);
    }

    int sCount;
    fin >> sCount;
    fin.ignore(10000, '\n');
    for (int i = 0; i < sCount; ++i) {
        CompressorStation s;
        fin >> s;
        stations.emplace(s.getId(), s);
    }
    cout << "Загружено!\n";
    cerr << "Загружено из файла: " << filename << "\n";
}