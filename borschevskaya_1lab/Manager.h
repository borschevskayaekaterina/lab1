#pragma once
#include <unordered_map>
#include "Pipe.h"
#include "CompressorStation.h"

void managePipes(std::unordered_map<int, Pipe>& pipes);
void manageStations(std::unordered_map<int, CompressorStation>& stations);
void controlSingleStation(std::unordered_map<int, CompressorStation>& stations);