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
    string kilometerMark;   
    double length;         
    int diameter;           
    bool isUnderRepair;    

public:

    Pipe() : length(0.0), diameter(0), isUnderRepair(false) {}

    void inputFromConsole() {
        cout << "Enter kilometer mark (name): ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, kilometerMark);
        cout << "Enter length (km): ";
        while (!(cin >> length) || length <= 0) {
            cout << "Invalid input! Length must be positive number. Try again: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Enter diameter (mm): ";
        while (!(cin >> diameter) || diameter <= 0) {
            cout << "Invalid input! Diameter must be positive integer. Try again: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        isUnderRepair = false;
    }

    void display() const {
        cout << "Pipe Info:\n";
        cout << "  Kilometer mark: " << kilometerMark << "\n";
        cout << "  Length: " << length << " km\n";
        cout << "  Diameter: " << diameter << " mm\n";
        cout << "  Under repair: " << (isUnderRepair ? "Yes" : "No") << "\n";
    }

    void toggleRepair() {
        isUnderRepair = !isUnderRepair;
        cout << "Repair status toggled. Now: " << (isUnderRepair ? "Under repair" : "Not under repair") << "\n";
    }

    bool isEmpty() const {
        return kilometerMark.empty() && length == 0.0 && diameter == 0;
    }

    void saveToFile(ofstream& out) const {
        out << "Pipe\n";
        out << kilometerMark << "\n";
        out << length << "\n";
        out << diameter << "\n";
        out << isUnderRepair << "\n";
    }

    void loadFromFile(ifstream& in) {
        getline(in, kilometerMark);
        in >> length;
        in >> diameter;
        in >> isUnderRepair;
        in.ignore(numeric_limits<streamsize>::max(), '\n');
    }
};

class CompressorStation {
private:
    string name;            
    int totalShops;         
    int activeShops;       
    int stationClass;  

public:

    CompressorStation() : totalShops(0), activeShops(0), stationClass(0) {}

    void inputFromConsole() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter station name: ";
        getline(cin, name);

        cout << "Enter total number of shops: ";
        while (!(cin >> totalShops) || totalShops < 0) {
            cout << "Invalid input! Must be non-negative integer. Try again: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Enter number of active shops: ";
        while (!(cin >> activeShops) || activeShops < 0 || activeShops > totalShops) {
            cout << "Invalid input! Must be between 0 and " << totalShops << ". Try again: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Enter station class (integer): ";
        while (!(cin >> stationClass) || stationClass < 0) {
            cout << "Invalid input! Must be non-negative integer. Try again: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    void display() const {
        cout << "Compressor Station Info:\n";
        cout << "  Name: " << name << "\n";
        cout << "  Total shops: " << totalShops << "\n";
        cout << "  Active shops: " << activeShops << "\n";
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

    bool isEmpty() const {
        return name.empty() && totalShops == 0 && activeShops == 0 && stationClass == 0;
    }

    void saveToFile(ofstream& out) const {
        out << "CompressorStation\n";
        out << name << "\n";
        out << totalShops << "\n";
        out << activeShops << "\n";
        out << stationClass << "\n";
    }


    void loadFromFile(ifstream& in) {
        getline(in, name);
        in >> totalShops;
        in >> activeShops;
        in >> stationClass;
        in.ignore(numeric_limits<streamsize>::max(), '\n');
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
