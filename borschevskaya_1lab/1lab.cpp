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
};

class CompressorStation {
private:
    string name;            
    int totalShops;         
    int activeShops;       
    int stationClass;       
};

