#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <cctype>
#include <ctime>
#include <iomanip>

using namespace std;

class Logger {
private:
    ofstream logFile;

    string currentTime() const {
        time_t now = time(nullptr);
        tm localTm;
#ifdef _WIN32
        localtime_s(&localTm, &now);
#else
        localtime_r(&now, &localTm);
#endif
        char buf[32];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &localTm);
        return string(buf);
    }

public:
    Logger(const string& filename) {
        logFile.open(filename, ios::app);
        if (!logFile) {
            cerr << "Warning: Cannot open log file '" << filename << "'\n";
        }
    }

    void log(const string& message) {
        if (logFile) {
            logFile << "[" << currentTime() << "] " << message << "\n";
            logFile.flush();
        }
    }

    ~Logger() {
        if (logFile) logFile.close();
    }
};

Logger* g_logger = nullptr;

void log(const string& msg) {
    if (g_logger) g_logger->log(msg);
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
            cout << "Invalid input! Enter a valid integer.\n";
            log("Invalid int input: \"" + line + "\"");
            continue;
        }

        try {
            return stoi(line);
        }
        catch (...) {
            cout << "Invalid input! Number too large.\n";
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
            cout << "Invalid input! Enter a valid number.\n";
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
    string line;
    cout << prompt;
    getline(cin, line);
    return line;
}

class Pipe {
private:
    static int nextId;              
    int id;
    string kilometerMark;   
    double length;         
    int diameter;           
    bool isUnderRepair;    

public:

    Pipe() : id(nextId++), length(0.0), diameter(0), isUnderRepair(false) {}

    Pipe(int existingId, const string& mark, double len, int diam, bool repair)
        : id(existingId), kilometerMark(mark), length(len), diameter(diam), isUnderRepair(repair) {
        if (existingId >= nextId) nextId = existingId + 1;
    }

    int getId() const { return id; }
    string getMark() const { return kilometerMark; }
    bool getRepairStatus() const { return isUnderRepair; }

    void inputFromConsole() {
        kilometerMark = readLine("Enter kilometer mark (name): ");
        length = readDouble("Enter length (km): ");
        while (length <= 0) {
            cout << "Length must be positive!\n";
            length = readDouble("Enter length (km): ");
        }
        diameter = readInt("Enter diameter (mm): ");
        while (diameter <= 0) {
            cout << "Diameter must be positive!\n";
            diameter = readInt("Enter diameter (mm): ");
        }
        isUnderRepair = false;
    }

    void display() const {
        cout << "  [ID: " << id << "] Pipe\n";
        cout << "  Kilometer mark: " << kilometerMark << "\n";
        cout << "  Length: " << length << " km\n";
        cout << "  Diameter: " << diameter << " mm\n";
        cout << "  Under repair: " << (isUnderRepair ? "Yes" : "No") << "\n";
    }

    void toggleRepair() {
        isUnderRepair = !isUnderRepair;
    }

    void setRepair(bool value) {
        isUnderRepair = value;
    }

    void saveToFile(ofstream& out) const {
        out << "Pipe\n";
        out << id << "\n";
        out << kilometerMark << "\n";
        out << length << "\n";
        out << diameter << "\n";
        out << isUnderRepair << "\n";
    }

    bool loadFromFile(ifstream& in) {
        string line;
        if (!getline(in, line)) return false;
        try { id = stoi(line); }
        catch (...) { return false; }

        if (!getline(in, kilometerMark)) return false;
        if (!getline(in, line)) return false;
        try { length = stod(line); }
        catch (...) { return false; }
        if (!getline(in, line)) return false;
        try { diameter = stoi(line); }
        catch (...) { return false; }
        if (!getline(in, line)) return false;
        isUnderRepair = (line == "1");

        if (id >= nextId) nextId = id + 1;
        return true;
    }
};

int Pipe::nextId = 1;

class CompressorStation {
private:
    static int nextId;
    int id;
    string name;            
    int totalShops;         
    int activeShops;       
    int stationClass;  

public:

    CompressorStation() : id(nextId++), totalShops(0), activeShops(0), stationClass(0) {}

    CompressorStation(int existingId, const string& n, int total, int active, int cls)
        : id(existingId), name(n), totalShops(total), activeShops(active), stationClass(cls) {
        if (existingId >= nextId) nextId = existingId + 1;
    }

    int getId() const { return id; }
    string getName() const { return name; }

    double getIdlePercent() const {
        if (totalShops == 0) return 0.0;
        return (static_cast<double>(totalShops - activeShops) / totalShops) * 100.0;
    }

    void inputFromConsole() {
        name = readLine("Enter station name: ");

        totalShops = readInt("Enter total number of shops: ");
        while (totalShops < 0) {
            cout << "Must be non-negative!\n";
            totalShops = readInt("Enter total number of shops: ");
        }

        activeShops = readInt("Enter number of active shops: ");
        while (activeShops < 0 || activeShops > totalShops) {
            cout << "Must be between 0 and " << totalShops << "!\n";
            activeShops = readInt("Enter number of active shops: ");
        }

        stationClass = readInt("Enter station class: ");
        while (stationClass < 0) {
            cout << "Must be non-negative!\n";
            stationClass = readInt("Enter station class: ");
        }
    }

    void display() const {
        cout << "  [ID: " << id << "] Compressor Station\n";
        cout << "  Name: " << name << "\n";
        cout << "  Total shops: " << totalShops << "\n";
        cout << "  Active shops: " << activeShops << "\n";
        cout << "  Idle percent: " << fixed << setprecision(1)<< getIdlePercent() << "%\n";
        cout << "  Station class: " << stationClass << "\n";
    }

    void startShop() {
        if (activeShops < totalShops) {
            activeShops++;
            cout << "Shop started. Active shops now: " << activeShops << "\n";
        }
        else {
            cout << "Cannot start shop: all shops are already active!\n";
        }
    }

    void stopShop() {
        if (activeShops > 0) {
            activeShops--;
            cout << "Shop stopped. Active shops now: " << activeShops << "\n";
        }
        else {
            cout << "Cannot stop shop: no active shops!\n";
        }
    }

    void saveToFile(ofstream& out) const {
        out << "CompressorStation\n";
        out << id << "\n";
        out << name << "\n";
        out << totalShops << "\n";
        out << activeShops << "\n";
        out << stationClass << "\n";
    }


    bool loadFromFile(ifstream& in) {
        string line;
        if (!getline(in, line)) return false;
        try { id = stoi(line); }
        catch (...) { return false; }

        if (!getline(in, name)) return false;
        if (!getline(in, line)) return false;
        try { totalShops = stoi(line); }
        catch (...) { return false; }
        if (!getline(in, line)) return false;
        try { activeShops = stoi(line); }
        catch (...) { return false; }
        if (!getline(in, line)) return false;
        try { stationClass = stoi(line); }
        catch (...) { return false; }

        if (id >= nextId) nextId = id + 1;
        return true;
    }

};

int CompressorStation::nextId = 1;

struct PipeFilter {
    enum Type { NONE, BY_MARK, BY_REPAIR } type;
    string markPattern;
    bool repairValue;

    PipeFilter() : type(NONE), repairValue(false) {}

    static PipeFilter byMark(const string& pattern) {
        PipeFilter f; f.type = BY_MARK; f.markPattern = pattern; return f;
    }
    static PipeFilter byRepair(bool value) {
        PipeFilter f; f.type = BY_REPAIR; f.repairValue = value; return f;
    }

    bool matches(const Pipe& p) const {
        switch (type) {
        case BY_MARK:   return p.getMark().find(markPattern) != string::npos;
        case BY_REPAIR: return p.getRepairStatus() == repairValue;
        default:        return true;
        }
    }
};

struct StationFilter {
    enum Type { NONE, BY_NAME, BY_IDLE_PERCENT } type;
    string namePattern;
    double idlePercentMin;
    double idlePercentMax;

    StationFilter() : type(NONE), idlePercentMin(0), idlePercentMax(100) {}

    static StationFilter byName(const string& pattern) {
        StationFilter f; f.type = BY_NAME; f.namePattern = pattern; return f;
    }
    static StationFilter byIdlePercent(double minP, double maxP) {
        StationFilter f; f.type = BY_IDLE_PERCENT;
        f.idlePercentMin = minP; f.idlePercentMax = maxP; return f;
    }

    bool matches(const CompressorStation& s) const {
        switch (type) {
        case BY_NAME:
            return s.getName().find(namePattern) != string::npos;
        case BY_IDLE_PERCENT: {
            double p = s.getIdlePercent();
            return p >= idlePercentMin && p <= idlePercentMax;
        }
        default: return true;
        }
    }
};


void addPipe(Pipe& pipe);
void addStation(CompressorStation& station);
void viewAll(const Pipe& pipe, const CompressorStation& station);
void editPipe(Pipe& pipe);
void editStation(CompressorStation& station);
void saveData(const Pipe& pipe, const CompressorStation& station);
void loadData(Pipe& pipe, CompressorStation& station);
void readLine(string& value);

int main() {
    Pipe pipe;
    CompressorStation station;

    int choice;


    while (true) {
        cout << "\n===== Pipeline Management System =====\n";
        cout << "1. Add Pipe\n";
        cout << "2. Add Compressor Station\n";
        cout << "3. View All Objects\n";
        cout << "4. Edit Pipe (toggle repair)\n";
        cout << "5. Edit Station (start/stop shop)\n";
        cout << "6. Save Data\n";
        cout << "7. Load Data\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";

        while (!(cin >> choice)) {
            cout << "Invalid input!";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        char next = cin.peek();
        if (next != '\n' && next != ' ' && next != '\t' && next != EOF) {
            cout << "Invalid input! Only integers 0-7 allowed.\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: addPipe(pipe); break;
        case 2: addStation(station); break;
        case 3: viewAll(pipe, station); break;
        case 4: editPipe(pipe); break;
        case 5: editStation(station); break;
        case 6: saveData(pipe,station); break;
        case 7: loadData(pipe, station); break;
        case 0:
            cout << "Exiting program. Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice! Please select 0-7.\n";
        }
    }

    return 0;
}

void readLine(string& value) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, value);
}

void addPipe(Pipe& pipe) {
    if (!pipe.isEmpty()) {
        cout << "Pipe already exists! Overwrite? (y/n): ";
        char answer;
        cin >> answer;
        if (answer != 'y' && answer != 'Y') {
            cout << "Operation cancelled.\n";
            return;
        }
    }
    pipe.inputFromConsole();
    cout << "Pipe added successfully!\n";
}

void addStation(CompressorStation& station) {
    if (!station.isEmpty()) {
        cout << "Station already exists! Overwrite? (y/n): ";
        char answer;
        cin >> answer;
        if (answer != 'y' && answer != 'Y') {
            cout << "Operation cancelled.\n";
            return;
        }
    }
    station.inputFromConsole();
    cout << "Compressor Station added successfully!\n";
}

void viewAll(const Pipe& pipe, const CompressorStation& station) {
    cout << "\n--- Current Objects ---\n";
    if (!pipe.isEmpty()) {
        pipe.display();
    }
    else {
        cout << "Pipe: Not created yet.\n";
    }

    cout << "\n";

    if (!station.isEmpty()) {
        station.display();
    }
    else {
        cout << "Compressor Station: Not created yet.\n";
    }
    cout << "------------------------\n";
}

void editPipe(Pipe& pipe) {
    if (pipe.isEmpty()) {
        cout << "Pipe does not exist! Please add pipe first.\n";
        return;
    }
    pipe.toggleRepair();
}

void editStation(CompressorStation& station) {
    if (station.isEmpty()) {
        cout << "Station does not exist! Please add station first.\n";
        return;
    }

    int choice;
    cout << "Edit Station:\n";
    cout << "1. Start a shop\n";
    cout << "2. Stop a shop\n";
    cout << "Enter choice: ";

    while (!(cin >> choice) || (choice != 1 && choice != 2)) {
        cout << "Invalid input! Enter 1 or 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (choice == 1) {
        station.startShop();
    }
    else {
        station.stopShop();
    }
}

void saveData(const Pipe& pipe, const CompressorStation& station) {
    bool pipeExists = !pipe.isEmpty();
    bool stationExists = !station.isEmpty();

    if (!pipeExists && !stationExists) {
        cout << "Nothing to save: both objects are empty!\n";
        return;
    }

    cout << "\nWhat do you want to save?\n";
    if (pipeExists)    cout << "1. Only Pipe\n";
    if (stationExists) cout << "2. Only Compressor Station\n";
    if (pipeExists && stationExists) cout << "3. Both objects\n";
    cout << "Enter choice: ";

    int saveChoice;
    while (!(cin >> saveChoice) ||
        (saveChoice == 1 && !pipeExists) ||
        (saveChoice == 2 && !stationExists) ||
        (saveChoice == 3 && !(pipeExists && stationExists))) {
        cout << "Invalid input! Try again: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    string filename;
    cout << "Enter filename to save (may contain spaces): ";
    readLine(filename);

    ofstream outFile(filename);
    if (!outFile) {
        cout << "Error: Cannot open file for writing!\n";
        return;
    }

    switch (saveChoice) {
    case 1:
        pipe.saveToFile(outFile);
        break;
    case 2:
        station.saveToFile(outFile);
        break;
    case 3:
        pipe.saveToFile(outFile);
        station.saveToFile(outFile);
        break;
    }

    outFile.close();
    cout << "Data saved successfully to \"" << filename << "\"\n";
}

void loadData(Pipe& pipe, CompressorStation& station) {
    string filename;
    cout << "Enter filename to load: ";
    cin >> filename;

    ifstream inFile(filename);
    if (!inFile) {
        cout << "Error: Cannot open file for reading!\n";
        return;
    }

    Pipe tempPipe;
    CompressorStation tempStation;

    bool pipeLoaded = false;
    bool stationLoaded = false;

    string type;
    while (getline(inFile, type)) {
        if (type == "Pipe") {
            tempPipe.loadFromFile(inFile);
            pipeLoaded = true;
        }
        else if (type == "CompressorStation") {
            tempStation.loadFromFile(inFile);
            stationLoaded = true;
        }
        else if (type.empty()) {
            continue;
        }
        else {
            cout << "Warning: Unknown record type \"" << type << "\" — skipped.\n";
        }
    }

    inFile.close();

    if (!pipeLoaded && !stationLoaded) {
        cout << "Error: No valid data found in file!\n";
        return;
    }

    if (pipeLoaded)    pipe = tempPipe;
    if (stationLoaded) station = tempStation;

    cout << "Data loaded successfully:\n";
    if (pipeLoaded)    cout << "  - Pipe\n";
    if (stationLoaded) cout << "  - Compressor Station\n";
}
