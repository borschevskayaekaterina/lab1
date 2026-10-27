#include "Pipe.h"
#include "Utils.h"

using namespace std;

int Pipe::nextId = 1;

Pipe::Pipe()
    : id(nextId++), name(""), length(0.0), diameter(0.0), repairing(false) {
}

void Pipe::updateNextId() {
    if (id >= nextId) nextId = id + 1;
}

int Pipe::getId() const { return id; }
string Pipe::getName() const { return name; }
bool Pipe::isRepairing() const { return repairing; }

void Pipe::setRepairing(bool newRepairing) {
    repairing = newRepairing;
}

void Pipe::input() {
    cout << "Введите название трубы: ";
    getline(cin, name);

    length = readDouble("Введите длину (км): ");
    while (length <= 0) {
        cout << "Ошибка! Длина должна быть положительной.\n";
        length = readDouble("Введите длину (км): ");
    }

    diameter = readInt("Введите диаметр (мм): ");
    while (diameter <= 0) {
        cout << "Ошибка! Диаметр должен быть положительным.\n";
        diameter = readInt("Введите диаметр (мм): ");
    }

    repairing = false;
}

void Pipe::print() const {
    cout << "\nТруба с ID: " << id << "\n"
        << "Название: " << name << "\n"
        << "Длина: " << length << " км\n"
        << "Диаметр: " << diameter << " мм\n"
        << "Состояние: " << (repairing ? "В ремонте" : "В работе") << "\n";
}

void Pipe::inTheRepair() {
    repairing = !repairing;
    cout << "Признак изменён. Труба теперь: " << (repairing ? "В ремонте" : "В работе") << "\n";
}

void Pipe::saveToFile(ofstream& file) const {
    file << id << "\n" << name << "\n" << length << "\n" << diameter << "\n" << repairing << "\n";
}

bool Pipe::loadFromFile(ifstream& file) {
    if (!(file >> id)) return false;
    file.ignore(10000, '\n');
    if (!getline(file, name)) return false;
    if (!(file >> length)) return false;
    if (!(file >> diameter)) return false;
    if (!(file >> repairing)) return false;
    file.ignore(10000, '\n');

    updateNextId();
    return true;
}

bool Pipe::matchesName(const string& query) const {
    if (query.empty()) return false;
    return name.find(query) != string::npos;
}

bool Pipe::matchesRepair(bool repairState) const {
    return repairing == repairState;
}