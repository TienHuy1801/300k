#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <limits>
#include <iomanip>
#include <cctype>
#include <algorithm>
using namespace std;

struct SensorData{
    int id;
    string timestamp;
    float value;
    int aqi;
    string pollution;
};

struct TransactionData{
    string startByte = "00";
    uint8_t packetLength;
    uint8_t id;
    string time;
    float PM2_5Concentration;
    uint16_t aqi;
    uint8_t checksum;
    string stoptByte = "FF";
};

std::string float_to_hex( float f )
{
    static_assert( numeric_limits<float>::is_iec559,"native float must be an IEEE float" ) ;

    union { float fval ; uint32_t ival ; };
    fval = f ;

    std::ostringstream stm ;
    stm << hex << std::uppercase << ival ;

    return stm.str() ;
}

uint32_t getTimeStamp(const std::string& timeString)
{
    std::tm t{};
    std::istringstream ss(timeString);

    ss >> std::get_time(&t, "%Y:%m:%d %H:%M:%S");
    if (ss.fail()) {
        throw std::runtime_error{"failed to parse time string"};
    }   
    std::time_t time_stamp = mktime(&t);
    return time_stamp;
}

string int_to_str_hex(int value){
    std::stringstream ss;
    ss<< std::hex << value; // int decimal_value
    std::string res ( ss.str() );
    if(value < 16) res.insert(0,"0");
    return res;
}

string parse_string_format(string s){
    for(int i = 2; i < s.length(); i+=3){
        s.insert(i, " ");
    }
    return s;
}
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
            data.push_back(tmp);
        }
        else
        {
            cout << "Error parsing line, skipping:\n"<< line << "\n";
        }
    }

    file.close();

    vector<TransactionData> transData;
    for(SensorData tmp : data){
        TransactionData newTransData;
        newTransData.id = tmp.id;
        newTransData.time = tmp.timestamp;
        newTransData.PM2_5Concentration = tmp.value;
        newTransData.aqi = tmp.aqi;
        newTransData.checksum = 1;
        transData.push_back(newTransData);
    }

    for(TransactionData tmp : transData){
        string res = "";
        res+= tmp.startByte + int_to_str_hex(tmp.id)+  int_to_str_hex(getTimeStamp(tmp.time)) + float_to_hex(tmp.PM2_5Concentration) + int_to_str_hex(tmp.aqi) + tmp.stoptByte;
        transform(res.begin(), res.end(),res.begin(), ::toupper);
        cout<< parse_string_format(res) << endl; 
    }
    return 0;
}