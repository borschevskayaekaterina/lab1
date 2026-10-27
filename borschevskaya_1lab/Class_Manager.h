#pragma once
#include <map>
#include "Pipe.h"
#include "CompressorStation.h"

void managePipes(std::map<int, Pipe>& pipes);
void manageStations(std::map<int, CompressorStation>& stations);
void controlSingleStation(std::map<int, CompressorStation>& stations);

