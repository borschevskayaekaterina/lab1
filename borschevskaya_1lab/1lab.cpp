#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

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
        cin >> kilometerMark;
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
        in.ignore(numeric_limits<streamsize>::max(), '\n'); // очистка после bool
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
        cout << "Enter station name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
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

Pipe pipe;
CompressorStation station;

void addPipe();
void addStation();
void viewAll();
void editPipe();
void editStation();
void saveData();
void loadData();

int main() {
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
            cout << "Invalid input! Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
        case 1: addPipe(); break;
        case 2: addStation(); break;
        case 3: viewAll(); break;
        case 4: editPipe(); break;
        case 5: editStation(); break;
        case 6: saveData(); break;
        case 7: loadData(); break;
        case 0:
            cout << "Exiting program. Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice! Please select 0-7.\n";
        }
    }

    return 0;
}

