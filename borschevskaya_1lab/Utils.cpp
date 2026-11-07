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

vector<int> findPipes(const unordered_map<int, Pipe>& pipes) {
    vector<int> foundIds;
    int choice = readInt("Искать по названию-1, по ремонту-2: ");

    if (choice == 1) {
        cout << "Введите название (или часть названия): ";
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

vector<int> findStations(const unordered_map<int, CompressorStation>& stations) {
    vector<int> foundIds;
    int choice = readInt("Искать по названию-1, по проценту простоя-2: ");

    if (choice == 1) {
        cout << "Введите название (или часть названия): ";
        string query;
        getline(cin, query);
        for (const auto& pair : stations) {
            if (pair.second.matchesName(query)) {
                foundIds.push_back(pair.first);
            }
        }
    }
    else if (choice == 2) {
        cout << "Пояснение: простой = (Всего цехов - Работающие) / Всего * 100%.\n";
        cout << "Пример: если 5 цехов всего, а работает 2, то простой = 60%.\n";
        double percent = readDouble("Введите минимальный процент простоя (от 0 до 100): ");
        while (percent < 0 || percent > 100) {
            cout << "Ошибка! Введите число от 0 до 100: ";
            percent = readDouble("");
        }
        for (const auto& pair : stations) {
            if (pair.second.matchesIdlePercent(percent)) {
                foundIds.push_back(pair.first);
            }
        }
    }
    return foundIds;
}