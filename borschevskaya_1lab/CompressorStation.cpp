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

    numberOfWorkshops = readInt("Общее количество цехов: ");
    while (numberOfWorkshops <= 0) {
        cout << "Ошибка! Количество цехов должно быть положительным.\n";
        numberOfWorkshops = readInt("Общее количество цехов: ");
    }

    operatingWorkshops = readInt("Сколько цехов работает сейчас: ");
    while (operatingWorkshops < 0 || operatingWorkshops > numberOfWorkshops) {
        cout << "Ошибка! Работающих цехов не может быть больше общего (от 0 до " << numberOfWorkshops << ").\n";
        operatingWorkshops = readInt("Сколько цехов работает сейчас: ");
    }

    stationClass = readInt("Класс станции: ");
    while (stationClass <= 0) {
        cout << "Ошибка! Класс должен быть положительным.\n";
        stationClass = readInt("Класс станции: ");
    }
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

ostream& operator<<(ostream& out, const CompressorStation& s) {
    if (&out == &cout) {
        out << "\n[КС ID: " << s.id << "] " << s.name
            << " | Всего цехов: " << s.numberOfWorkshops
            << " | Работает: " << s.operatingWorkshops
            << " | Простой: " << s.getIdlePercent() << "%\n";
    }
    else {
        out << s.id << "\n" << s.name << "\n" << s.numberOfWorkshops << "\n" << s.operatingWorkshops << "\n" << s.stationClass;
    }
    return out;
}

istream& operator>>(istream& in, CompressorStation& s) {
    in >> s.id;
    in.ignore(10000, '\n');
    getline(in, s.name);
    in >> s.numberOfWorkshops >> s.operatingWorkshops >> s.stationClass;
    in.ignore(10000, '\n');
    s.updateNextId();
    return in;
}