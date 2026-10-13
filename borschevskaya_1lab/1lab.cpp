#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <algorithm>

using namespace std;

ofstream logFile("pipeline.log");

void log(const string& msg) {
    if (logFile) logFile << msg << "\n";
}

bool isAllDigits(const string& s) {
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
            cout << "Invalid input! Enter an integer.\n";
            continue;
        }
        line = line.substr(start, end - start + 1);

        if (!isAllDigits(line)) {
            cout << "Invalid input! \"" << line << "\" is not a valid integer.\n";
            log("Invalid int input: \"" + line + "\"");
            continue;
        }

        try {
            return stoi(line);
        }
        catch (...) {
            cout << "Invalid input! Number is too large.\n";
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
            cout << "Invalid input! Enter a number.\n";
            continue;
        }
        line = line.substr(start, end - start + 1);

        bool hasDigit = false;
        bool hasDot = false;
        bool ok = true;
        for (char c : line) {
            if (isdigit(static_cast<unsigned char>(c))) hasDigit = true;
            else if (c == '.') {
                if (hasDot) { ok = false; break; }
                hasDot = true;
            }
            else { ok = false; break; }
        }

        if (!ok || !hasDigit) {
            cout << "Invalid input! \"" << line << "\" is not a valid number.\n";
            log("Invalid double input: \"" + line + "\"");
            continue;
        }

        try {
            return stod(line);
        }
        catch (...) {
            cout << "Invalid input! Try again.\n";
        }
    }
}
string readLine(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

class Pipe {
public:
    int id;
    string name;   
    double length;         
    int diameter;           
    bool isUnderRepair;    

    Pipe() : id(0), length(0.0), diameter(0), isUnderRepair(false) {}

    void input() {
        name = readLine("Name: ");

        length = readDouble("Length (km): ");
        while (length <= 0) {
            cout << "Length must be positive!\n";
            length = readDouble("Length (km): ");
        }

        diameter = readInt("Diameter (mm): ");
        while (diameter <= 0) {
            cout << "Diameter must be positive!\n";
            diameter = readInt("Diameter (mm): ");
        }

        isUnderRepair = false;
    }

    void display() const {
        cout << "[ID " << id << "] " << name
            << " | " << length << " km"
            << " | " << diameter << " mm"
            << " | Repair: " << (isUnderRepair ? "Yes" : "No") << "\n";
    }
};

class CompressorStation {
public:
    int id;
    string name;            
    int totalShops;         
    int activeShops;       
    int stationClass;  

public:

    CompressorStation() : id(0), totalShops(0), activeShops(0), stationClass(0) {}

    void input() {
        name = readLine("Name: ");

        totalShops = readInt("Total shops: ");
        while (totalShops < 0) {
            cout << "Total shops must be non-negative!\n";
            totalShops = readInt("Total shops: ");
        }

        activeShops = readInt("Active shops: ");
        while (activeShops < 0 || activeShops > totalShops) {
            cout << "Active shops must be between 0 and " << totalShops << "!\n";
            activeShops = readInt("Active shops: ");
        }

        stationClass = readInt("Station class: ");
        while (stationClass < 0) {
            cout << "Station class must be non-negative!\n";
            stationClass = readInt("Station class: ");
        }
    }

    double idlePercent() const {
        if (totalShops == 0) return 0;
        return 100.0 * (totalShops - activeShops) / totalShops;
    }

    void display() const {
        cout << "[ID " << id << "] " << name
            << " | Shops: " << activeShops << "/" << totalShops
            << " | Idle: " << idlePercent() << "%"
            << " | Class: " << stationClass << "\n";
    }
};

class PipelineManager {
private:
    vector<Pipe> pipes;
    vector<CompressorStation> stations;
    int nextPipeId = 1;
    int nextStationId = 1;

public:
    void addPipe() {
        Pipe p;
        p.input();
        p.id = nextPipeId++;
        pipes.push_back(p);
        cout << "Pipe added with ID = " << p.id << "\n";
        log("Added Pipe ID=" + to_string(p.id));
    }

    void addStation() {
        CompressorStation s;
        s.input();
        s.id = nextStationId++;
        stations.push_back(s);
        cout << "Station added with ID = " << s.id << "\n";
        log("Added Station ID=" + to_string(s.id));
    }

    void viewAll() const {
        cout << "\n--- Pipes ---\n";
        if (pipes.empty()) cout << "No pipes.\n";
        for (const auto& p : pipes) p.display();

        cout << "\n--- Stations ---\n";
        if (stations.empty()) cout << "No stations.\n";
        for (const auto& s : stations) s.display();
    }

    void searchPipes() {
        cout << "1. By name\n2. By repair status\n";
        int mode = readInt("Choose: ");

        bool found = false;
        if (mode == 1) {
            string pattern = readLine("Substring: ");
            for (const auto& p : pipes) {
                if (p.name.find(pattern) != string::npos) {
                    p.display();
                    found = true;
                }
            }
        }
        else if (mode == 2) {
            int r;
            while (true) {
                r = readInt("Repair (0/1): ");
                if (r == 0 || r == 1) break;
                cout << "Enter 0 or 1 only!\n";
            }
            for (const auto& p : pipes) {
                if (p.isUnderRepair == (r != 0)) {
                    p.display();
                    found = true;
                }
            }
        }

        if (!found) cout << "Nothing found.\n";
        log("Search pipes, mode=" + to_string(mode));
    }

    void searchStations() {
        cout << "1. By name\n2. By idle percent range\n";
        int mode = readInt("Choose: ");

        bool found = false;
        if (mode == 1) {
            string pattern = readLine("Substring: ");
            for (const auto& s : stations) {
                if (s.name.find(pattern) != string::npos) {
                    s.display();
                    found = true;
                }
            }
        }
        else if (mode == 2) {
            double lo = readDouble("Min idle %: ");
            double hi = readDouble("Max idle %: ");
            if (lo > hi) swap(lo, hi);
            for (const auto& s : stations) {
                double p = s.idlePercent();
                if (p >= lo && p <= hi) {
                    s.display();
                    found = true;
                }
            }
        }

        if (!found) cout << "Nothing found.\n";
        log("Search stations, mode=" + to_string(mode));
    }

    void batchEditPipes() {
        if (pipes.empty()) {
            cout << "No pipes.\n";
            return;
        }

        cout << "1. All pipes\n";
        cout << "2. Only in repair\n";
        cout << "3. Only not in repair\n";
        int mode = readInt("Choose selection: ");

        vector<int> selected;
        for (int i = 0; i < (int)pipes.size(); i++) {
            if (mode == 1) selected.push_back(i);
            else if (mode == 2 && pipes[i].isUnderRepair) selected.push_back(i);
            else if (mode == 3 && !pipes[i].isUnderRepair) selected.push_back(i);
        }

        if (selected.empty()) {
            cout << "Nothing selected.\n";
            return;
        }

        cout << "Selected " << selected.size() << " pipe(s).\n";
        cout << "1. Set repair = Yes\n";
        cout << "2. Set repair = No\n";
        cout << "3. Delete selected\n";
        int action = readInt("Choose action: ");

        switch (action) {
        case 1:
            for (int i : selected) pipes[i].isUnderRepair = true;
            cout << "Updated " << selected.size() << " pipe(s).\n";
            log("Batch: set repair=1 for " + to_string(selected.size()) + " pipes");
            break;
        case 2:
            for (int i : selected) pipes[i].isUnderRepair = false;
            cout << "Updated " << selected.size() << " pipe(s).\n";
            log("Batch: set repair=0 for " + to_string(selected.size()) + " pipes");
            break;
        case 3:
            sort(selected.rbegin(), selected.rend());
            for (int i : selected) pipes.erase(pipes.begin() + i);
            cout << "Deleted " << selected.size() << " pipe(s).\n";
            log("Batch: deleted " + to_string(selected.size()) + " pipes");
            break;
        default:
            cout << "Invalid action.\n";
        }
    }

    void save(const string& filename) const {
        ofstream out(filename);
        if (!out) {
            cout << "Cannot open file!\n";
            log("Save failed: " + filename);
            return;
        }

        out << pipes.size() << "\n";
        for (const auto& p : pipes) {
            out << p.id << "\n" << p.name << "\n"
                << p.length << "\n" << p.diameter << "\n"
                << p.isUnderRepair << "\n";
        }

        out << stations.size() << "\n";
        for (const auto& s : stations) {
            out << s.id << "\n" << s.name << "\n"
                << s.totalShops << "\n" << s.activeShops << "\n"
                << s.stationClass << "\n";
        }

        out.close();
        cout << "Saved " << pipes.size() << " pipes, "
            << stations.size() << " stations to " << filename << "\n";
        log("Saved: " + filename);
    }

    void load(const string& filename) {
        ifstream in(filename);
        if (!in) {
            cout << "Cannot open file!\n";
            log("Load failed: " + filename);
            return;
        }

        vector<Pipe> newPipes;
        vector<CompressorStation> newStations;

        int pCount;
        in >> pCount;
        in.ignore();

        for (int i = 0; i < pCount; i++) {
            Pipe p;
            string line;

            getline(in, line); p.id = stoi(line);
            getline(in, p.name);
            getline(in, line); p.length = stod(line);
            getline(in, line); p.diameter = stoi(line);
            getline(in, line); p.isUnderRepair = (line == "1");

            newPipes.push_back(p);
            if (p.id >= nextPipeId) nextPipeId = p.id + 1;
        }

        int sCount;
        in >> sCount;
        in.ignore();

        for (int i = 0; i < sCount; i++) {
            CompressorStation s;
            string line;

            getline(in, line); s.id = stoi(line);
            getline(in, s.name);
            getline(in, line); s.totalShops = stoi(line);
            getline(in, line); s.activeShops = stoi(line);
            getline(in, line); s.stationClass = stoi(line);

            newStations.push_back(s);
            if (s.id >= nextStationId) nextStationId = s.id + 1;
        }

        in.close();

        pipes = newPipes;
        stations = newStations;

        cout << "Loaded " << pipes.size() << " pipes, "
            << stations.size() << " stations from " << filename << "\n";
        log("Loaded: " + filename);
    }
};
int main() {
    Logger logger("pipeline.log");
    g_logger = &logger;
    log("=== Program started ===");

    PipelineManager manager;

    while (true) {
        cout << "\n===== Pipeline Management System =====\n";
        cout << "1.  Add Pipe\n";
        cout << "2.  Add Compressor Station\n";
        cout << "3.  View All Objects\n";
        cout << "4.  Edit Pipe (toggle repair by ID)\n";
        cout << "5.  Edit Station (start/stop shop by ID)\n";
        cout << "6.  Save Data to File\n";
        cout << "7.  Load Data from File\n";
        cout << "8.  Search Pipes\n";
        cout << "9.  Search Stations\n";
        cout << "10. Batch Edit Pipes\n";
        cout << "0.  Exit\n";
        cout << "Enter your choice: ";

        int choice = readInt("");
        switch (choice) {
        case 1:  manager.addPipe(); break;
        case 2:  manager.addStation(); break;
        case 3:
            cout << "\n--- Pipes ---\n";
            manager.displayPipes();
            cout << "\n--- Stations ---\n";
            manager.displayStations();
            break;
        case 4:{
            int id = readInt("Enter pipe ID: ");
            bool found = false;
            cout << "Use 'Search Pipes' or 'Batch Edit' to modify pipes.\n";
            break;
        }
        case 5:{
            cout << "Use 'Search Stations' to modify stations.\n";
            break;
        }
        case 6: {
            string filename = readLine("Enter filename to save: ");
            manager.saveToFile(filename);
            break;
        }
        case 7: {
            string filename = readLine("Enter filename to load: ");
            manager.loadFromFile(filename);
            break;
        }
        case 8:  manager.searchPipes(); break;
        case 9:  manager.searchStations(); break;
        case 10: manager.batchEditPipes(); break;
        case 0:
            cout << "Exiting program. Goodbye!\n";
            log("=== Program exited ===");
            return 0;
        default:
            cout << "Invalid choice! Please select 0-10.\n";
        }
    }

    return 0;
}