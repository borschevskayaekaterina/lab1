#include "Utils.h"
#include "Pipe.h"
#include "CompressorStation.h"
#include <cctype>
#include <limits>

using namespace std;

static bool isAllDigits(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

int readInt(const string& prompt) {
    string line;
    while (true) {
        cout << prompt;
        getline(cin, line);

        size_t start = line.find_first_not_of(" \t");
        size_t end = line.find_last_not_of(" \t");
        if (start == string::npos) {
            cout << "Ошибка! Введите целое число.\n";
            continue;
        }
        line = line.substr(start, end - start + 1);

        if (!isAllDigits(line)) {
            cout << "Ошибка! \"" << line << "\" не является целым числом.\n";
            continue;
        }
        try {
            return stoi(line);
        }
        catch (...) {
            cout << "Ошибка! Слишком большое число.\n";
        }
    }
}

double readDouble(const string& prompt) {
    string line;
    while (true) {
        cout << prompt;
        getline(cin, line);

        size_t start = line.find_first_not_of(" \t");
        size_t end = line.find_last_not_of(" \t");
        if (start == string::npos) {
            cout << "Ошибка! Введите число.\n";
            continue;
        }
        line = line.substr(start, end - start + 1);

        bool hasDigit = false, hasDot = false, ok = true;
        for (char c : line) {
            if (isdigit(static_cast<unsigned char>(c))) hasDigit = true;
            else if (c == '.') {
                if (hasDot) { ok = false; break; }
                hasDot = true;
            }
            else { ok = false; break; }
        }
        if (!ok || !hasDigit) {
            cout << "Ошибка! \"" << line << "\" не является числом.\n";
            continue;
        }
        try {
            return stod(line);
        }
        catch (...) {
            cout << "Ошибка! Слишком большое число.\n";
        }
    }
}

vector<int> findPipes(const map<int, Pipe>& pipes) {
    vector<int> foundIds;
    int choice = readInt("Искать по названию-1, по ремонту-2: ");

    if (choice == 1) {
        cout << "Введите название: ";
        string query;
        getline(cin, query);
        for (const auto& pair : pipes) {
            if (pair.second.matchesName(query)) {
                foundIds.push_back(pair.first);
            }
        }
    }
    else if (choice == 2) {
        int state = readInt("Искать в ремонте? (1 - да, 0 - нет): ");
        for (const auto& pair : pipes) {
            if (pair.second.matchesRepair(state != 0)) {
                foundIds.push_back(pair.first);
            }
        }
    }
    return foundIds;
}

vector<int> findStations(const map<int, CompressorStation>& stations) {
    vector<int> foundIds;
    int choice = readInt("Искать по названию-1, по проценту незадействованных цехов-2: ");

    if (choice == 1) {
        cout << "Введите название: ";
        string query;
        getline(cin, query);
        for (const auto& pair : stations) {
            if (pair.second.matchesName(query)) {
                foundIds.push_back(pair.first);
            }
        }
    }
    else if (choice == 2) {
        double percent = readDouble("Введите минимальный процент (0-100): ");
        for (const auto& pair : stations) {
            if (pair.second.matchesIdlePercent(percent)) {
                foundIds.push_back(pair.first);
            }
        }
    }
    return foundIds;
}