#include "CompressorStation.h"
#include "Utils.h"

using namespace std;

int CompressorStation::nextId = 1;

CompressorStation::CompressorStation()
    : id(nextId++), name(""), numberOfWorkshops(0), operatingWorkshops(0), stationClass(0) {
}

void CompressorStation::updateNextId() {
    if (id >= nextId) nextId = id + 1;
}

int CompressorStation::getId() const { return id; }
string CompressorStation::getName() const { return name; }

void CompressorStation::input() {
    cout << "Введите название КС: ";
    getline(cin, name);

    numberOfWorkshops = readInt("Количество цехов: ");
    while (numberOfWorkshops <= 0) {
        cout << "Ошибка! Количество цехов должно быть положительным.\n";
        numberOfWorkshops = readInt("Количество цехов: ");
    }

    operatingWorkshops = 0;

    stationClass = readInt("Класс станции: ");
    while (stationClass <= 0) {
        cout << "Ошибка! Класс должен быть положительным.\n";
        stationClass = readInt("Класс станции: ");
    }
}

void CompressorStation::print() const {
    cout << "\nКС с ID: " << id << "\n"
        << "Название: " << name << "\n"
        << "Количество цехов: " << numberOfWorkshops << "\n"
        << "Количество работающих цехов: " << operatingWorkshops << "\n"
        << "Класс станции: " << stationClass << "\n"
        << "Станция " << (operatingWorkshops > 0 ? "Работает" : "Простаивает") << "\n";
}

void CompressorStation::startWorkshop() {
    if (operatingWorkshops < numberOfWorkshops) {
        operatingWorkshops++;
        cout << "Цех запущен.\n";
    }
    else cout << "Нельзя, все цеха в работе\n";
}

void CompressorStation::stopWorkshop() {
    if (operatingWorkshops > 0) {
        operatingWorkshops--;
        cout << "Цех остановлен.\n";
    }
    else cout << "Нельзя, нет работающих цехов\n";
}

void CompressorStation::saveToFile(ofstream& file) const {
    file << id << "\n" << name << "\n" << numberOfWorkshops << "\n" << operatingWorkshops << "\n" << stationClass << "\n";
}

bool CompressorStation::loadFromFile(ifstream& file) {
    if (!(file >> id)) return false;
    file.ignore(10000, '\n');
    if (!getline(file, name)) return false;
    if (!(file >> numberOfWorkshops)) return false;
    if (!(file >> operatingWorkshops)) return false;
    if (!(file >> stationClass)) return false;
    file.ignore(10000, '\n');

    updateNextId();
    return true;
}

double CompressorStation::getIdlePercent() const {
    if (numberOfWorkshops == 0) return 0;
    return (numberOfWorkshops - operatingWorkshops) * 100.0 / numberOfWorkshops;
}

bool CompressorStation::matchesName(const string& query) const {
    if (query.empty()) return false;
    return name.find(query) != string::npos;
}

bool CompressorStation::matchesIdlePercent(double minPercent) const {
    return getIdlePercent() >= minPercent;
}