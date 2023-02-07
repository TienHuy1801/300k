#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

struct SensorData{
    int id;
    string timestamp;
    double value;
    int aqi;
    string pollution;
};

int main(int argc, char *argv[])
{
    // Reading data from file name provided as command line argument.
    string filename = argc > 1 ? argv[1] : "dust_aqi.csv";
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error 01: file not found or cannot be accessed";
        return 0;
    }

    vector<SensorData> data;

    string line;
    getline(file, line);
    while (getline(file, line))
    {
        istringstream ss(line);
        int id;
        string timestamp;
        double value;
        char comma, comma2, comma3, comma4;;
        int aqi;
        string pollution;

        SensorData tmp;
        if (ss >> tmp.id >> comma && getline(ss, tmp.timestamp, ',') && ss >> tmp.value >> comma3 >> tmp.aqi >> comma4 && getline(ss, tmp.pollution, ','))
        {
            cout << tmp.id << " " << tmp.timestamp << " " << tmp.value << " "<< tmp.aqi << " "<< tmp.pollution << endl;
            data.push_back(tmp);
        }
        else
        {
            cout << "Error parsing line, skipping:\n"<< line << "\n";
        }
    }

    file.close();

    return 0;
}