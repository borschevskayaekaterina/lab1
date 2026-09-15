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

};

