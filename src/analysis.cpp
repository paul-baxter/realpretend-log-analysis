//
// Summer Scientist 2017 Log File Analysis
//
// simple utility to process log files from fact/fiction SS experiment
//
// need to run this once per log file
//
// PEB (pbaxter@lincoln.ac.uk) - 06/09/17
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <math.h>
#include "functions.h"

using namespace std;


int main (int argc, char **argv)
{
    std::vector<std::string> answers;

    //populate the answers
    Answers(answers);

    std::string fileName;
    std::string readIn;

    //results containers
    string participantID;
    vector<string> timingAll;        //reaction times of all stories
    vector<string> timingTrain;      //reaction times of first four stories
    vector<string> timingTest;       //reaction times of rest of stories
    vector<float> correctAll;       //correctness of all stories
    vector<float> correctTrain;       //correctness of first four stories
    vector<float> correctTest;       //correctness of rest of stories
    vector<string> storyCorrectness(16);    //for each story, is it correct or not?

    //arg parsing
    if (argc == 2)
    {
        fileName = argv[1];
    }
    else
    {
        cout << "Incorrect number of arguments, don't be silly..." << endl;
        return 1;
    }

    std::ifstream inFile;
    inFile.exceptions(std::ios::failbit);	//enable exception throwing
    try
    {
        //fileName = "data/" + fileName;
        //cout << fileName << endl;
    	inFile.open(fileName.c_str(), ios::in);
        getline(inFile, readIn);
        cout << "PARTICIPANT: " + readIn << endl;
        participantID = readIn;
    }
    catch (std::ios_base::failure &fail)
    {
    	std::cout << "error: problem with input file: " << fail.what() << std::endl;
    }
    inFile.exceptions(std::ios::goodbit);	//disable exception throwing

    //throw away the second line, as this is data header
    getline(inFile, readIn);

    //the first four stories treated separately
    for (int a = 0; a < 4; a++)
    {
        getline(inFile, readIn);
        //std::cout << readIn << std::endl;
        vector<string> s = Splitter(readIn, ',');
        //timings
        timingAll.push_back(s[3]);
        timingTrain.push_back(s[3]);
        //correctness
        if (answers[(int)StoF(s[4])] == s[1])
        {
            //if this is the case, then story has been classified correctly
            correctTrain.push_back(1.0f);
            correctAll.push_back(1.0f);
            storyCorrectness[(int)StoF(s[4])] = "1";
        }
        else
        {
            //story classified incorrectly
            correctTrain.push_back(0.0f);
            correctAll.push_back(0.0f);
            storyCorrectness[(int)StoF(s[4])] = "0";
        }
    }

    //rest of the stories now
    while (getline(inFile, readIn))
    {
        //read until end: each line is now data
    	//std::cout << readIn << std::endl;
        vector<string> s = Splitter(readIn, ',');
        //timings
        timingAll.push_back(s[3]);
        timingTest.push_back(s[3]);
        //correctness
        if (answers[(int)StoF(s[4])] == s[1])
        {
            //if this is the case, then story has been classified correctly
            correctTest.push_back(1.0f);
            correctAll.push_back(1.0f);
            storyCorrectness[(int)StoF(s[4])] = "1";
        }
        else
        {
            //story classified incorrectly
            correctTest.push_back(0.0f);
            correctAll.push_back(0.0f);
            storyCorrectness[(int)StoF(s[4])] = "0";
        }
    }

    //std::cout << std::endl << "END OF LOG FILE" << std::endl;

    inFile.close();

    //////////////////////////
    // DO THE ANALYSIS
    //////////////////////////

    float correctnessAll = Correctness(correctAll);
    float correctnessTrain = Correctness(correctTrain);
    float correctnessTest = Correctness(correctTest);

    // cout << endl;
    // cout << "Mean time of first four: \t" + Mean(timingTrain) + "\tSD: " + SD(timingTrain) << endl;
    // cout << "Mean time of rest of stories: \t" + Mean(timingTest) + "\tSD: " + SD(timingTest) << endl;
    // cout << "Mean time over all stories: \t" + Mean(timingAll) + "\tSD: " + SD(timingAll) << endl;
    // cout << endl;
    // cout << "Correctness in first four stories: \t" << correctnessTrain << endl;
    // cout << "Correctness in rest of stories: \t" << correctnessTest << endl;
    // cout << "Correctness in all stories: \t\t" << correctnessAll << endl;
    // cout << endl;

    //////////////////////////
    // PUT RESULTS INTO RESULTS FILE
    //////////////////////////

    string resultsFileName = "results.dat";
    ofstream resultsFile;

    resultsFile.exceptions(std::ios::failbit);	//enable exception throwing
    try
    {
        if (!fileExists(resultsFileName))
        {
            //if the file doesn't exist, then open, and write header
            cout << "Opening new file for results..." << endl;
            resultsFile.open(resultsFileName.c_str(), ios::out);
            resultsFile << "ID,TrainCorrect,TestCorrect,AllCorrect,TimeMeanTrain,TimeSDTrain,TimeMeanTest,TimeSDTest,TimeMeanAll,TimeSDAll" << endl;
            resultsFile.flush();
        }
        else
        {
            //if the file does exist, then open in append mode, so can add to it
            cout << "Results file already exists, appending results to it..." << endl;
            resultsFile.open(resultsFileName.c_str(), ios::app);
        }
    }
    catch (std::ios_base::failure &fail)
    {
        std::cout << "error: problem with results file: " << fail.what() << std::endl;
    }
    resultsFile.exceptions(std::ios::goodbit);	//disable exception throwing

    resultsFile << participantID << ",";
    resultsFile << correctnessTrain << ",";
    resultsFile << correctnessTest << ",";
    resultsFile << correctnessAll << ",";
    resultsFile << Mean(timingTrain) << "," << SD(timingTrain) << ",";
    resultsFile << Mean(timingTest) << "," << SD(timingTest) << ",";
    resultsFile << Mean(timingAll) << "," << SD(timingAll);
    resultsFile << endl;

    resultsFile.flush();
    resultsFile.close();


    cout << "DONE" << endl;
    cout << endl;

    return 0;
}
