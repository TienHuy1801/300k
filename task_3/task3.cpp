#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <limits>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include<map>
#include<unordered_map>
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
    string packetLength = "0F";
    uint8_t id;
    string time;
    float PM2_5Concentration;
    uint16_t aqi;
    uint8_t checksum;
    string stoptByte = "FF";
};

string float_to_hex( float f )
{
    static_assert( numeric_limits<float>::is_iec559,"native float must be an IEEE float" ) ;

    union { float fval ; uint32_t ival ; };
    fval = f ;

    ostringstream stm ;
    stm << hex << uppercase << ival ;

    return stm.str() ;
}

uint32_t getTimeStamp(const string& timeString)
{
    tm t{};
    istringstream ss(timeString);

    ss >> get_time(&t, "%Y:%m:%d %H:%M:%S");
    if (ss.fail()) {
        throw runtime_error{"failed to parse time string"};
    }   
    time_t time_stamp = mktime(&t);
    return time_stamp;
}

string int_to_str_hex(int value){
    stringstream ss;
    ss<< hex << value; // int decimal_value
    string res ( ss.str() );
    if(value < 16) res.insert(0,"0");
    return res;
}

vector<string> parse_string_format(string s){
    vector<string> res;
    string ss ="";
    for(int i = 0; i < s.length(); i+=2){
        ss += s[i];
        ss += s[i+1];
        res.push_back(ss);
        ss = "";
    }
    return res;
}

string plus_hex(string hex1, string hex2)
{
    if (hex1.length() < hex2.length())
        hex1.swap(hex2);

    //for convenience, make sure that hex1 is longer.

    /*Strat algorithm*/

    int length1, length2;
    length1 = hex1.length();
    length2 = hex2.length();
    int flag = 0; // carry
    int get1, get2;
    int sum;

    while (length1>0)
    {
        //get first number
        if (hex1[length1 - 1] >= 'A')
            get1 = hex1[length1 - 1] - 55;
        else
            get1 = hex1[length1 - 1] - '0';

        //get second number

        if (length2 > 0)
        {
            if (hex2[length2 - 1] >= 'A')
                get2 = hex2[length2 - 1] - 55;
            else
                get2 = hex2[length2 - 1] - '0';
        }
        else
            get2 = 0;

        //get the sum
        sum = get1 + get2 + flag;

        if (sum >= 16)
        {
            int left = sum % 16;
            if (left >= 10)
                hex1[length1 - 1] = 'A' + left % 10;
            else
                hex1[length1 - 1] = '0' + left;
            flag = 1;
        }
        else
        {
            if (sum >= 10)
                hex1[length1 - 1] = 'A' + sum % 10;
            else
                hex1[length1 - 1] = '0' + sum;
            flag = 0;
        }

        length1--;
        length2--;
    }

    if (flag == 1)
        return "1" + hex1;
    else
        return hex1;

    /*End of algorithm*/
}

string HexToBin(string hexdec)
{
    long int i = 0;
    string res = "";
    while (hexdec[i]) {
 
        switch (hexdec[i]) {
        case '0':
            res+= "0000";
            break;
        case '1':
            res+= "0001";
            break;
        case '2':
            res+= "0010";
            break;
        case '3':
            res+= "0011";
            break;
        case '4':
            res+= "0100";
            break;
        case '5':
            res+= "0101";
            break;
        case '6':
            res+= "0110";
            break;
        case '7':
            res+= "0111";
            break;
        case '8':
            res+= "1000";
            break;
        case '9':
            res+= "1001";
            break;
        case 'A':
        case 'a':
            res+= "1010";
            break;
        case 'B':
        case 'b':
            res+= "1011";
            break;
        case 'C':
        case 'c':
            res+= "1100";
            break;
        case 'D':
        case 'd':
            res+= "1101";
            break;
        case 'E':
        case 'e':
            res+= "1110";
            break;
        case 'F':
        case 'f':
            res+= "1111";
            break;
        default:
            cout <<  "\nInvalid hexadecimal digit "
                 << hexdec[i];
        }
        i++;
    }
    return res;
}

// Returns '0' for '1' and '1' for '0'
char flip(char c) {return (c == '0')? '1': '0';}
  
// Print 1's and 2's complement of binary number
// represented by "bin"
string TwosComplement(string bin)
{
    int n = bin.length();
    int i;
  
    string ones, twos;
    ones = twos = "";
  
    //  for ones complement flip every bit
    for (i = 0; i < n; i++)
        ones += flip(bin[i]);
  
    //  for two's complement go from right to left in
    //  ones complement and if we get 1 make, we make
    //  them 0 and keep going left when we get first
    //  0, make that 1 and go out of loop
    twos = ones;
    for (i = n - 1; i >= 0; i--)
    {
        if (ones[i] == '1')
            twos[i] = '0';
        else
        {
            twos[i] = '1';
            break;
        }
    }
  
    // If No break : all are 1  as in 111  or  11111;
    // in such case, add extra 1 at beginning
    if (i == -1)
        twos = '1' + twos;
  
  
    return twos;
}

void createMap(unordered_map<string, char> *um)
{
    (*um)["0000"] = '0';
    (*um)["0001"] = '1';
    (*um)["0010"] = '2';
    (*um)["0011"] = '3';
    (*um)["0100"] = '4';
    (*um)["0101"] = '5';
    (*um)["0110"] = '6';
    (*um)["0111"] = '7';
    (*um)["1000"] = '8';
    (*um)["1001"] = '9';
    (*um)["1010"] = 'A';
    (*um)["1011"] = 'B';
    (*um)["1100"] = 'C';
    (*um)["1101"] = 'D';
    (*um)["1110"] = 'E';
    (*um)["1111"] = 'F';
}
 

string convertBinToHex(string bin)
{
    int l = bin.size();
    int t = bin.find_first_of('.');
     
    // length of string before '.'
    int len_left = t != -1 ? t : l;
     
    // add min 0's in the beginning to make
    // left substring length divisible by 4
    for (int i = 1; i <= (4 - len_left % 4) % 4; i++)
        bin = '0' + bin;
     
    // if decimal point exists   
    if (t != -1)   
    {
        // length of string after '.'
        int len_right = l - len_left - 1;
         
        // add min 0's in the end to make right
        // substring length divisible by 4
        for (int i = 1; i <= (4 - len_right % 4) % 4; i++)
            bin = bin + '0';
    }
     
    // create map between binary and its
    // equivalent hex code
    unordered_map<string, char> bin_hex_map;
    createMap(&bin_hex_map);
     
    int i = 0;
    string hex = "";
     
    while (1)
    {
        // one by one extract from left, substring
        // of size 4 and add its hex code
        hex += bin_hex_map[bin.substr(i, 4)];
        i += 4;
        if (i == bin.size())
            break;
             
        // if '.' is encountered add it
        // to result
        if (bin.at(i) == '.')   
        {
            hex += '.';
            i++;
        }
    }
     
    // required hexadecimal number
    return hex;   
}

int main(int argc, char *argv[])
{
    // Reading data from file name provided as command line argument.
    string filename = argc > 1 ? argv[1] : "dust_aqi.csv";
    string outFileName = argc > 2 ? argv[2] : " hex_packet.dat";

    ofstream logOutFile("task3.log");
    if (!logOutFile)
    {
        cout << "Error: unable to open output file.\n";
        return 1;
    }
    
    ifstream file(filename);
    if (!file.is_open())
    {
        logOutFile << "Error 01: file not found or cannot be accessed";
        return 0;
    }

    vector<SensorData> data;

    string line;
    getline(file, line);
    if (!line.compare("id,time,values,aqi,pollution" )&& !(line == "id,time,values,aqi,pollution"))
    {
        logOutFile << "Error 02: invalid csv file\n";
    }
    else
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
                logOutFile << "Error 03: data is missing at line \n"<< line << "\n";
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

    ofstream outFile(outFileName);
    if (!outFile)
    {
        cerr << "Error: unable to open output file.\n";
        return 1;
    }


    for(TransactionData tmp : transData){
        string preRes = "";
        string aqiRes = int_to_str_hex(tmp.aqi);
        while(aqiRes.length() < 4){
            aqiRes.insert(0,"0");
        }
        preRes += tmp.startByte + tmp.packetLength + int_to_str_hex(tmp.id)+  int_to_str_hex(getTimeStamp(tmp.time)) + float_to_hex(tmp.PM2_5Concentration) + aqiRes;
        transform(preRes.begin(), preRes.end(),preRes.begin(), ::toupper);
        vector<string> out = parse_string_format(preRes);

        string sumHex = "00";
        for(int i = 1; i < out.size(); i++){
            sumHex = plus_hex(sumHex, out[i]);
        }

        sumHex.erase(0, sumHex.size() - 2);

        out.push_back(convertBinToHex(TwosComplement(HexToBin(sumHex))));
        out.push_back(tmp.stoptByte);
        for(string i : out){
            outFile << i << " ";
        }
        outFile << endl;
    }

    return 0;
}