#include "Pipe.h"
#include "Utils.h"

using namespace std;

int Pipe::nextId = 1;

Pipe::Pipe() : id(nextId++), name(""), length(0.0), diameter(0), repairing(false), isBusy(false) {}
void Pipe::updateNextId() { if (id >= nextId) nextId = id + 1; }

int Pipe::getId() const { return id; }
int Pipe::getDiameter() const { return diameter; }
bool Pipe::isBusyPipe() const { return isBusy; }
void Pipe::setBusy(bool busy) { isBusy = busy; }
void Pipe::setDiameter(int diam) { diameter = diam; }
void Pipe::setName(const string& n) { name = n; }

void Pipe::input() {
    cout << "Введите название трубы: ";
    getline(cin, name);

    length = readDouble("Введите длину (км): ");

    while (true) {
        diameter = readInt("Введите диаметр (500, 700, 1000, 1400): ");
        if (diameter == 500 || diameter == 700 || diameter == 1000 || diameter == 1400) break;
        cout << "Ошибка! Разрешены только 500, 700, 1000, 1400.\n";
    }

    int rep = readInt("Труба в ремонте? (1 - да, 0 - нет): ");
    repairing = (rep == 1);
    isBusy = false; 
}

bool Pipe::matchesName(const string& query) const {
    if (query.empty()) return false;
    return name.find(query) != string::npos;
}

bool Pipe::matchesRepair(bool repairState) const { return repairing == repairState; }

ostream& operator<<(ostream& out, const Pipe& p) {
    if (&out == &cout) {
        out << "\n[Труба ID: " << p.id << "] " << p.name
            << " | Длина: " << p.length << " км | Диаметр: " << p.diameter << " мм"
            << " | " << (p.repairing ? "В ремонте" : "В работе")
            << " | " << (p.isBusy ? "Занята в сети" : "Свободна") << "\n";
    }
    else {
        out << p.id << "\n" << p.name << "\n" << p.length << "\n" << p.diameter << "\n" << p.repairing << "\n" << p.isBusy;
    }
    return out;
}

istream& operator>>(istream& in, Pipe& p) {
    in >> p.id;
    in.ignore(10000, '\n');
    getline(in, p.name);
    in >> p.length >> p.diameter >> p.repairing >> p.isBusy;
    in.ignore(10000, '\n');
    p.updateNextId();
    return in;
}