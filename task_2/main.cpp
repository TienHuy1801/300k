#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
#include <regex>
#include <map>
#include <cmath>
#include <chrono>

using namespace std;

struct SensorData
{
    int id;
    string time;
    double value;
};

struct AqiData
{
    int id;
    string time;
    double value;
    double aqi;
    string pollution;
};

struct SummaryData
{
    int id;
    string parameters;
    string time;
    double value;
};

struct StatisticsData
{
    int id;
    string pollution;
    int duration;
};

const string POLLUTION[7] = {"Good", "Moderate", "Slightly unhealthy", "Unhealthy", "Very unhealthy", "Hazardous", "Extremely Hazardous"};

bool isDateValid(const string &date)
{
    regex pattern("\\d{4}:\\d{2}:\\d{2} \\d{2}:\\d{2}:\\d{2}");
    if (regex_match(date, pattern))
        return true;
    else
        return false;
}

vector<SensorData> input(int argc, char *argv[], ofstream &logOutFile)
{

    // Reading data from file name provided as command line argument.
    string filename = argc > 1 ? argv[1] : "dust_sensor.csv";
    ifstream file(filename);
    vector<SensorData> data;
    if (!file.is_open())
    {
        logOutFile << "Error 01: file not found or cannot be accessed\n";
        return data;
    }

    int index = 1;
    string line;
    // Skip header in csv file and check its valid.
    getline(file, line);
    if (line != "id,time,values")
    {
        logOutFile << "Error 02: invalid csv file\n";
    }
    else
        while (getline(file, line))
        {
            istringstream ss(line);
            int id;
            string time;
            double value;
            char comma;

            ss >> id;
            ss >> comma;
            getline(ss, time, ',');
            ss >> value;
            if (ss.fail() || id <= 0 || !isDateValid(time))
            {
                logOutFile << "Error 03: data is missing at line " << index;
                break;
            }
            data.push_back({id, time, value});
            index++;
        }
    file.close();
    return data;
}

vector<SensorData> processTask21(vector<SensorData> data, ofstream &logOutFile)
{
    ofstream outFile("dust_outlier.csv");
    // Check if the file was successfully opened
    if (!outFile.is_open())
    {
        logOutFile << "Error: Unable to create file task 2.1\n";
    }
    vector<SensorData> validData;
    validData.clear();

    for (const SensorData &record : data)
    {
        if (record.value > 5 && record.value < 550.5)
            validData.push_back(record);
    }

    outFile << "number of outliers: " << validData.size() << "\n";
    outFile << "id,time,values\n";
    for (const SensorData &record : validData)
    {
        outFile << record.id << "," << record.time << "," << record.value << "\n";
    }
    outFile.close();
    return validData;
}

vector<AqiData> processTask22(vector<SensorData> data, ofstream &logOutFile)
{
    ofstream outFile("dust_aqi.csv");
    // Check if the file was successfully opened
    if (!outFile.is_open())
    {
        logOutFile << "Error: Unable to create file task 2.2\n";
    }
    map<pair<string, int>, double> sums;
    map<pair<string, int>, int> counts;
    for (const SensorData &record : data)
    {
        sums[{record.time.substr(0, 13), record.id}] += record.value;
        counts[{record.time.substr(0, 13), record.id}]++;
    }

    vector<AqiData> aqis;
    aqis.clear();

    for (auto it = sums.begin(); it != sums.end(); ++it)
    {
        double avg = it->second / counts[it->first];
        string pollution;
        double aqi;
        if (avg < 12)
        {
            pollution = POLLUTION[0];
            aqi = floorf((rand() / (float)RAND_MAX) * 50 * 10) / 10;
        }
        else if (avg < 35.5)
        {
            pollution = POLLUTION[1];
            aqi = floorf((rand() / (float)RAND_MAX) * (100 - 50) * 10 + 50 * 10) / 10;
        }
        else if (avg < 55.5)
        {
            pollution = POLLUTION[2];
            aqi = floorf((rand() / (float)RAND_MAX) * (150 - 100) * 10 + 100 * 10) / 10;
        }
        else if (avg < 150.5)
        {
            pollution = POLLUTION[3];
            aqi = floorf((rand() / (float)RAND_MAX) * (200 - 150) * 10 + 150 * 10) / 10;
        }
        else if (avg < 250.5)
        {
            pollution = POLLUTION[4];
            aqi = floorf((rand() / (float)RAND_MAX) * (300 - 200) * 10 + 200 * 10) / 10;
        }
        else if (avg < 350.5)
        {
            pollution = POLLUTION[5];
            aqi = floorf((rand() / (float)RAND_MAX) * (400 - 300) * 10 + 300 * 10) / 10;
        }
        else if (avg < 550.5)
        {
            pollution = POLLUTION[6];
            aqi = floorf((rand() / (float)RAND_MAX) * (500 - 400) * 10 + 300 * 10) / 10;
        }
        aqis.push_back({it->first.second, it->first.first + ":00:00", avg, aqi, pollution});
    }
    outFile << "id,time,values,aqi,pollution\n";
    for (const AqiData &record : aqis)
    {
        outFile << record.id << "," << record.time << "," << record.value << "," << record.aqi << "," << record.pollution << "\n";
    }
    outFile.close();
    return aqis;
}

vector<SummaryData> processTask23(vector<SensorData> data, ofstream &logOutFile)
{
    ofstream outFile("dust_summary.csv");
    // Check if the file was successfully opened
    if (!outFile.is_open())
    {
        logOutFile << "Error: Unable to create file task 2.3\n";
    }

    map<int, pair<string, double>> max;
    map<int, pair<string, double>> min;
    map<int, double> sum;
    map<int, int> count;
    string startTime = data[0].time;
    string endTime = data[0].time;
    for (const SensorData &record : data)
    {
        if (startTime > record.time)
            startTime = record.time;
        if (endTime < record.time)
            endTime = record.time;
        if (min[record.id].second > record.value)
        {
            min[record.id] = {record.time, record.value};
        }
        if (max[record.id].second < record.value)
        {
            max[record.id] = {record.time, record.value};
        }
        sum[record.id] += record.value;
        count[record.id]++;
    }

    tm startTm = {};
    tm endTm = {};
    strptime(startTime.c_str(), "%Y:%m:%d %H:%M:%S", &startTm);
    strptime(endTime.c_str(), "%Y:%m:%d %H:%M:%S", &endTm);
    int duration = difftime(mktime(&endTm), mktime(&startTm)) / 3600;

    vector<SummaryData> summary;
    summary.clear();

    for (auto it = max.begin(); it != max.end(); ++it)
    {
        summary.push_back({it->first, "max", it->second.first, it->second.second});
        summary.push_back({it->first, "min", it->second.first, it->second.second});
        summary.push_back({it->first, "mean", to_string(duration) + ":00:00", sum[it->first] / count[it->first]});
    }
    outFile << "id,parameters,time,values\n";
    for (const SummaryData &record : summary)
    {
        outFile << record.id << "," << record.parameters << "," << record.time << "," << record.value << "\n";
    }
    outFile.close();
    return summary;
}

vector<StatisticsData> processTask24(vector<AqiData> data, ofstream &logOutFile)
{
    ofstream outFile("dust_statistics.csv");
    // Check if the file was successfully opened
    if (!outFile.is_open())
    {
        logOutFile << "Error: Unable to create file task 2.4\n";
    }
    int maxId = 0;
    map<pair<int, string>, int> count;
    for (const AqiData &record : data)
    {
        if (maxId < record.id)
            maxId = record.id;
        count[{record.id, record.pollution}]++;
    }
    vector<StatisticsData> statistics;
    statistics.clear();

    for (int i = 1; i <= maxId; i++)
    {
        for (string pollution : POLLUTION)
        {
            statistics.push_back({i, pollution, count[{i, pollution}]});
        }
    }
    outFile << "id,pollution,duration\n";
    for (const StatisticsData &record : statistics)
    {
        outFile << record.id << "," << record.pollution << "," << record.duration << "\n";
    }
    outFile.close();
    return statistics;
}

int main(int argc, char *argv[])
{
    // Seed the random number generator
    srand(time(0));
    ofstream logOutFile("task2.log");
    if (!logOutFile)
    {
        cout << "Error: unable to open output file.\n";
        return 1;
    }
    vector<SensorData> data = input(argc, argv, logOutFile);
    vector<SensorData> validData = processTask21(data, logOutFile);
    vector<AqiData> aqiData = processTask22(validData, logOutFile);
    vector<SummaryData> summaryData = processTask23(validData, logOutFile);
    vector<StatisticsData> statisticsData = processTask24(aqiData, logOutFile);

    return 0;
}
