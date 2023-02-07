#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream> 
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;

struct SensorData {
    int id;
    time_t timestamp;
    double value;
};

// Get current time as a time_t value
time_t currentTime(void) {
    return chrono::system_clock::to_time_t(
        chrono::system_clock::now());
}

// Convert time_t to string in format YYYY:MM:DD hh:mm:ss
string time_to_string(const time_t& time) {
    char buffer[20];
    strftime(buffer, 20, "%Y:%m:%d %H:%M:%S", localtime(&time));
    return string(buffer);
}

int main(int argc, char *argv[]) {
    
    int num_sensors = 1; // default value for number of sensors
    int sampling = 30; // default value for sampling frequency
    int interval = 24; // default value for simulation interval

    ofstream logOutFile("task1.log");
    if (!logOutFile) {
        cerr << "Error: unable to open output file." << endl;
        return 1;
    }
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            if (++i < argc) {
                num_sensors = stoi(argv[i]);
            } else {
                logOutFile << "Error 01: expecting number after '-n' argument." << endl;
                return 1;
            }
        } else if (strcmp(argv[i], "-st") == 0) {
            if (++i < argc) {
                sampling = stoi(argv[i]);
                if (sampling < 1) {
                    logOutFile << "Error 02: minimum sample rate is 1." << endl;
                    return 1;
                }
            } else {
                logOutFile << "Error 01: expecting number after '-st' argument." << endl;
                return 1;
            }
        } else if (strcmp(argv[i], "-si") == 0) {
            if (++i < argc) {
                interval = stoi(argv[i]);
                if (interval < 1) {
                    logOutFile << "Error 02: minimum simulation interval is 1." << endl;
                    return 1;
                }
            } else {
                logOutFile << "Error 01: expecting number after '-si' argument." << endl;
                return 1;
            }
        } else {
            logOutFile << "Error 03: unknown argument '" << argv[i] << "'." << endl;
            return 1;
        }
    
    }
    logOutFile.close(); // close

    // Create output file and open it
    ofstream outFile("dust_sensor.csv");
    if (!outFile) {
        cerr << "Error: unable to open output file." << endl;
        return 1;
    }

    // Write header to the file
    outFile << "id,time,values" << endl;

    // Random number engine for generating values
    mt19937 engine{random_device{}()};
    uniform_real_distribution<double> dist{0.0, 100.0};

    // Get start time as a time_t value
    time_t now = currentTime();
    time_t stop = now - interval * 3600;

    // Loop over all (num_sensors) sensors and the specified interval
    for (int id = 1; id <= num_sensors; id++) {
        for (time_t t = now; t > stop; t -= sampling) {
            SensorData data;
            data.id = id;
            data.timestamp = t;
            data.value = dist(engine);

            // Write sensor data to the file 
            outFile << data.id << "," 
                    << time_to_string(data.timestamp) << "," 
                    << fixed << setprecision(1) << data.value << "\n";

        }
    }
    outFile.close();
}