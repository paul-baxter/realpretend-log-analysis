//
// Summer Scientist Log File Analysis
//
// simple utility to process log files from fact/fiction SS experiment
//
// need to run this once per log file
//
// PEB (pbaxter@lincoln.ac.uk) - 06/09/17
//
// Update 08/2018: updated to process logs from Summer Scientist 2018 study
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
    int score;
    string participantID;
    string condition;
    vector<string> timingAll;        //reaction times of all stories
    vector<string> timingTrain;      //reaction times of first four stories
    vector<string> timingTest;       //reaction times of rest of stories
    vector<float> correctAll;       //correctness of all stories
    vector<float> correctTrain;       //correctness of first four stories
    vector<float> correctTest;       //correctness of rest of stories
    vector<string> storyCorrectness(16);    //for each story, is it correct or not?
    vector<string> storyOrderCorrect;   //in the order of presentation, story correctness
    string startTime;
    string duration;

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
        vector<string> id = Splitter(readIn, ',');
        cout << "PARTICIPANT: " + id[0] << endl;
        participantID = id[0];
        condition = id[1];
    }
    catch (std::ios_base::failure &fail)
    {
    	std::cout << "error: problem with input file: " << fail.what() << std::endl;
    }
    inFile.exceptions(std::ios::goodbit);	//disable exception throwing

    //throw away the second line, as this is data header
    getline(inFile, readIn);

    //third line contains the start time stamp, want this...
    getline(inFile, readIn);
    vector<string> startTimeLine = Splitter(readIn, ',');
    if((startTimeLine[0] == "event") && (startTimeLine.size() == 8))
    {
        startTime = startTimeLine[7];
    }

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
    score = 0;
    while (getline(inFile, readIn))
    {
        //read until end: each line is now data
    	//std::cout << readIn << std::endl;
        vector<string> s = Splitter(readIn, ',');

        if ((s[0] == "data") && (s.size() == 6))
        {
            //update score
            if (s[1] == "1")
            {
                //increment score in line with Corriveau 2009...
                score += 1;
                //
                // children received a 1 whenever they categorized a figure as real and
                // a 0 whenever they categorized a figure as pretend. Thus, correct responding
                // resulted in high scores ... for historical figures and low scores ...
                // for fictional figures.
            }
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

        //final line provides the duration of the interaction with the robot
        if ((s[0] == "event") && (s.size() == 10))
        {
            duration = s[9];
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
            resultsFile << "ID,condition,TrainCorrect,TestCorrect,AllCorrect,TimeMeanTrain,TimeSDTrain,TimeMeanTest,TimeSDTest,TimeMeanAll,TimeSDAll,duration,testScore,story 1,story 2,story 3,story 4,story 5,story 6,story 7,story 8,story 9,story 10,story 11,story 12,story 13,story 14,story 15,story 16" << endl;
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
    resultsFile << condition << ",";
    resultsFile << correctnessTrain << ",";
    resultsFile << correctnessTest << ",";
    resultsFile << correctnessAll << ",";
    resultsFile << Mean(timingTrain) << "," << SD(timingTrain) << ",";
    resultsFile << Mean(timingTest) << "," << SD(timingTest) << ",";
    resultsFile << Mean(timingAll) << "," << SD(timingAll) << ",";
    resultsFile << duration << ",",
    resultsFile << score << ",";
    //now the individual stories
    for (int a = 0; a < (int)correctAll.size(); a++)
    {
        resultsFile << correctAll[a];
        if (a != ((int)correctAll.size() - 1))
        {
            resultsFile << ",";
        }
    }
    resultsFile << endl;

    resultsFile.flush();
    resultsFile.close();


    cout << "DONE" << endl;
    cout << endl;

    return 0;
}
