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

public:
    void addPipe() {
        Pipe p;
        p.inputFromConsole();
        pipes.push_back(p);
        cout << "Pipe added with ID = " << p.getId() << "\n";
        log("Added Pipe ID=" + to_string(p.getId()));
    }

    void displayPipes() const {
        if (pipes.empty()) {
            cout << "No pipes.\n";
            return;
        }
        for (const auto& p : pipes) p.display();
    }

    void addStation() {
        CompressorStation s;
        s.inputFromConsole();
        stations.push_back(s);
        cout << "Station added with ID = " << s.getId() << "\n";
        log("Added Station ID=" + to_string(s.getId()));
    }

    void displayStations() const {
        if (stations.empty()) {
            cout << "No stations.\n";
            return;
        }
        for (const auto& s : stations) s.display();
    }

    vector<int> findPipes(const PipeFilter& filter) const {
        vector<int> indices;
        for (size_t i = 0; i < pipes.size(); ++i) {
            if (filter.matches(pipes[i])) indices.push_back(static_cast<int>(i));
        }
        return indices;
    }

    vector<int> findStations(const StationFilter& filter) const {
        vector<int> indices;
        for (size_t i = 0; i < stations.size(); ++i) {
            if (filter.matches(stations[i])) indices.push_back(static_cast<int>(i));
        }
        return indices;
    }

    void batchEditPipes() {
        if (pipes.empty()) {
            cout << "No pipes to edit.\n";
            return;
        }

        cout << "\n--- Batch Edit Pipes ---\n";
        cout << "1. Select All pipes\n";
        cout << "2. Select by filter (mark / repair status)\n";
        cout << "3. Select by IDs manually\n";
        int mode = readInt("Choose selection mode: ");

        vector<int> indices;

        if (mode == 1) {
            for (size_t i = 0; i < pipes.size(); ++i) indices.push_back(static_cast<int>(i));
        }
        else if (mode == 2) {
            cout << "1. By name (substring)\n2. By repair status\n";
            int sub = readInt("Choose filter: ");
            if (sub == 1) {
                string pattern = readLine("Enter substring: ");
                indices = findPipes(PipeFilter::byMark(pattern));
            }
            else if (sub == 2) {
                int r = readInt("Repair status (0 = no, 1 = yes): ");
                indices = findPipes(PipeFilter::byRepair(r != 0));
            }
            else {
                cout << "Invalid filter.\n";
                return;
            }
        }
        else if (mode == 3) {
            while (true) {
                int id = readInt("Enter pipe ID (0 to finish): ");
                if (id == 0) break;
                bool found = false;
                for (size_t i = 0; i < pipes.size(); ++i) {
                    if (pipes[i].getId() == id) {
                        indices.push_back(static_cast<int>(i));
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Pipe with ID " << id << " not found.\n";
            }
        }
        else {
            cout << "Invalid mode.\n";
            return;
        }

        if (indices.empty()) {
            cout << "No pipes selected.\n";
            return;
        }

        cout << "\nSelected " << indices.size() << " pipe(s):\n";
        for (int idx : indices) pipes[idx].display();

        cout << "\nAction:\n1. Set repair status\n2. Delete\n";
        int action = readInt("Choose action: ");

        if (action == 1) {
            int r = readInt("Set repair status (0 = no, 1 = yes): ");
            for (int idx : indices) pipes[idx].setRepair(r != 0);
            cout << "Updated " << indices.size() << " pipe(s).\n";
            log("Batch edit: set repair=" + to_string(r) + " for " +
                to_string(indices.size()) + " pipes");
        }
        else if (action == 2) {
            vector<int> sorted = indices;
            sort(sorted.rbegin(), sorted.rend());
            for (int idx : sorted) {
                log("Deleted Pipe ID=" + to_string(pipes[idx].getId()));
                pipes.erase(pipes.begin() + idx);
            }
            cout << "Deleted " << indices.size() << " pipe(s).\n";
        }
        else {
            cout << "Invalid action.\n";
        }
    }

    void saveToFile(const string& filename) const {
        ofstream out(filename);
        if (!out) {
            cout << "Error: Cannot open file for writing!\n";
            log("Failed to open file for writing: " + filename);
            return;
        }
        out << "Pipe_count " << pipes.size() << "\n";
        for (const auto& p : pipes) p.saveToFile(out);
        out << "Station_count " << stations.size() << "\n";
        for (const auto& s : stations) s.saveToFile(out);
        out.close();
        cout << "Saved " << pipes.size() << " pipe(s) and " << stations.size() << " station(s) to \"" << filename << "\"\n";
        log("Saved to file: " + filename);
    }

    void loadFromFile(const string& filename) {
        ifstream in(filename);
        if (!in) {
            cout << "Error: Cannot open file for reading!\n";
            log("Failed to open file for reading: " + filename);
            return;
        }

        vector<Pipe> tempPipes;
        vector<CompressorStation> tempStations;

        string line;
        if (!getline(in, line) || line.substr(0, 11) != "Pipe_count ") {
            cout << "Error: Invalid file format (expected Pipe_count).\n";
            return;
        }
        int pCount = stoi(line.substr(11));

        for (int i = 0; i < pCount; ++i) {
            if (!getline(in, line) || line != "Pipe") {
                cout << "Error: Invalid pipe record.\n";
                return;
            }
            Pipe p;
            if (!p.loadFromFile(in)) {
                cout << "Error: Failed to read pipe data.\n";
                return;
            }
            tempPipes.push_back(p);
        }

        if (!getline(in, line) || line.substr(0, 14) != "Station_count ") {
            cout << "Error: Invalid file format (expected Station_count).\n";
            return;
        }
        int sCount = stoi(line.substr(14));

        for (int i = 0; i < sCount; ++i) {
            if (!getline(in, line) || line != "CompressorStation") {
                cout << "Error: Invalid station record.\n";
                return;
            }
            CompressorStation s;
            if (!s.loadFromFile(in)) {
                cout << "Error: Failed to read station data.\n";
                return;
            }
            tempStations.push_back(s);
        }

        in.close();

        pipes = tempPipes;
        stations = tempStations;

        cout << "Loaded " << pipes.size() << " pipe(s) and "
            << stations.size() << " station(s) from \"" << filename << "\"\n";
        log("Loaded from file: " + filename);
    }

    void searchPipes() {
        cout << "\nSearch Pipes:\n1. By name (substring)\n2. By repair status\n";
        int mode = readInt("Choose filter: ");
        vector<int> found;
        if (mode == 1) {
            string pattern = readLine("Enter substring: ");
            found = findPipes(PipeFilter::byMark(pattern));
        }
        else if (mode == 2) {
            int r = readInt("Repair status (0 = no, 1 = yes): ");
            found = findPipes(PipeFilter::byRepair(r != 0));
        }
        else {
            cout << "Invalid filter.\n";
            return;
        }

        if (found.empty()) {
            cout << "No pipes found.\n";
        }
        else {
            cout << "Found " << found.size() << " pipe(s):\n";
            for (int idx : found) pipes[idx].display();
        }
        log("Searched pipes, found: " + to_string(found.size()));
    }

    void searchStations() {
        cout << "\nSearch Stations:\n1. By name (substring)\n2. By idle percent range\n";
        int mode = readInt("Choose filter: ");
        vector<int> found;
        if (mode == 1) {
            string pattern = readLine("Enter substring: ");
            found = findStations(StationFilter::byName(pattern));
        }
        else if (mode == 2) {
            double lo = readDouble("Min idle percent: ");
            double hi = readDouble("Max idle percent: ");
            found = findStations(StationFilter::byIdlePercent(lo, hi));
        }
        else {
            cout << "Invalid filter.\n";
            return;
        }

        if (found.empty()) {
            cout << "No stations found.\n";
        }
        else {
            cout << "Found " << found.size() << " station(s):\n";
            for (int idx : found) stations[idx].display();
        }
        log("Searched stations, found: " + to_string(found.size()));
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