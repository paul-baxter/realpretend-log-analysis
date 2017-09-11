////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>

using namespace std;

float StoF (string in)
{
    //convert the string into a double
    float out = atof(in.c_str());
    return out;
}

float Correctness (vector<float> in)
{
    //calculate the proportion correct
    float correct = 0.0f;
    float num = 0.0f;
    for (int a = 0; a < (int)in.size(); a++)
    {
        num += 1.0f;
        correct += in[a];
    }
    correct = (correct / num);
    return correct;
}

string Mean (vector<string> in)
{
    //take in a vector of numbers in string form, and return the calculated mean in a string
    float result = 0.0f;        //the calculated mean, back in string form...
    float sum = 0.0f;
    float num = 0.0f;
    for (int a = 0; a < (int)in.size(); a++)
    {
        num += 1.0f;
        sum += StoF(in[a]);
    }
    result = sum / num;
    stringstream ss;
    ss << result;
    string r = ss.str();
    return r;
}

string SD (vector<string> in)
{
    //poulation standard deviation...
    string m = Mean(in);
    float mean = StoF(m);
    float numerator = 0.0f;
    float num = 0.0;
    float ss = 0.f;
    float result = 0.0f;
    for (int a = 0; a < (int)in.size(); a++)
    {
        float temp = StoF(in[a]) - mean;
        temp = (temp * temp);
        numerator += temp;
        num += 1.0f;
    }
    ss = numerator / num;
    result = sqrt(ss);
    stringstream temp;
    temp << result;
    string toReturn = temp.str();
    return toReturn;
}

vector<float> LinearRegression (vector<float> in)
{
    //take float data in, and calculate a linear regression
    // returning a vector with contents "[a, b]
    // for the regression formula "y = a + bx"
    // Assume that data points equally spaced (1,2,3,...)
    vector<float> result;
}

vector<string> Splitter (string _in, char _delim)
{
    //function to split up the input string, _in, into a vector of strings
    // which is separated by the _delim character, e.g. ',' or ';'
    std::vector<std::string> _split;    //the split up vector
    _split.clear();
    std::vector<int> _loc;              //location of delims in string
    _loc.clear();
    std::string _temp = std::string();  //a temporary string
    int _size = _in.size();
    _loc.push_back(-1);                  //store starting point-11
    //iterate through string and find where the delims are
    for (int a = 0; a < _size; a++)
    {
        if (_in[a] == _delim)
        {
            //the current character matches the delimiter
            // so store this location in the string
            _loc.push_back(a);
        }
    }
    //now based on this, create new substrings and add to _split
    int loc_size = _loc.size();
    int start = -1;     //start position of substring
    int end = -1;       //end position of substring
    int length = 0;     //length of substring
    //the number of substrings will equal the size of _loc
    for (int b = 0; b < loc_size; b++)
    {
        //split up the input string into substrings
        start = (_loc[b] + 1);
        if (b == (loc_size - 1)) //modified 06/09/12 due to Valgrind error pickup...
        {
            end = _loc[loc_size - 1];
        }
        else
        {
            end = _loc[b+1];
        }
        length = end - start;
        _temp = _in.substr(start,length);

        //remove possible carriage return
        if(_temp[_temp.size()-1] == '\r')
        {
            //remove carriage return if present...
            std::string temp;
            temp = _temp.substr(0, _temp.size()-1);
            _temp = temp.c_str();
        }
        _split.push_back(_temp);
    }
    //vector of substrings now completely constructed, return
    return _split;
}

void Answers (vector<string> &answers)
{
    // 0
    answers.push_back("1");

    // 1
    answers.push_back("0");

    // 2
    answers.push_back("0");

    // 3
    answers.push_back("1");

    // 4
    answers.push_back("0");

    // 5
    answers.push_back("1");

    // 6
    answers.push_back("0");

    // 7
    answers.push_back("1");

    // 8
    answers.push_back("0");

    // 9
    answers.push_back("1");

    // 10
    answers.push_back("0");

    // 11
    answers.push_back("1");

    // 12
    answers.push_back("0");

    // 13
    answers.push_back("1");

    // 14
    answers.push_back("0");

    // 15
    answers.push_back("1");
}
