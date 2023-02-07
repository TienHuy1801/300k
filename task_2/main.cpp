#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>

using namespace std;

struct SensorData
{
    int id;
    string timestamp;
    double value;
};

bool isDateValid(const std::string &date)
{
    regex pattern("\\d{4}:\\d{2}:\\d{2} \\d{2}:\\d{2}:\\d{2}");
    if (regex_match(date, pattern))
        return true;
    else
        return false;
}

int main(int argc, char *argv[])
{
    // Reading data from file name provided as command line argument.
    string filename = argc > 1 ? argv[1] : "dust_sensor.csv";
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error 01: file not found or cannot be accessed";
        return 0;
    }

    vector<SensorData> data;

    int index = 1;
    string line;
    // Skip header in csv file.
    getline(file, line);
    while (getline(file, line))
    {
        istringstream ss(line);
        int id;
        string timestamp;
        double value;
        char comma;

        ss >> id;
        ss >> comma;
        getline(ss, timestamp, ',');
        ss >> value;
        if (ss.fail() || id <= 0 || !isDateValid(timestamp))
        {
            cout << "Error 03: data is missing at line " << index;
            break;
        }
        index++;
    }

    file.close();

    return 0;
}
