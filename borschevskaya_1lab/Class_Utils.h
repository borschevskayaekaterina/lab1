#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

class Pipe;
class CompressorStation;

int readInt(const std::string& prompt);
double readDouble(const std::string& prompt);

std::vector<int> findPipes(const std::map<int, Pipe>& pipes);
std::vector<int> findStations(const std::map<int, CompressorStation>& stations);

class redirect_output_wrapper
{
    std::ostream& stream;
    std::streambuf* const old_buf;
public:
    redirect_output_wrapper(std::ostream& src)
        : old_buf(src.rdbuf()), stream(src)
    {
    }

    ~redirect_output_wrapper() {
        stream.rdbuf(old_buf);
    }
    void redirect(std::ostream& dest)
    {
        stream.rdbuf(dest.rdbuf());
    }
};

