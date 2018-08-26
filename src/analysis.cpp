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
    int switchYears = 0;                //if 2018 data, then set this to 1... 0 for 2017
    string participantID;
    string condition;               //experimental condition: 0: congruent, 1: incongruent
    vector<string> timingAll;        //reaction times of all stories, in order of presentation
    vector<string> timingTrain;      //reaction times of first four stories, in order of presentation
    vector<string> timingTest;       //reaction times of rest of stories, in order of presentation
    vector<float> correctAll;       //correctness of all stories, in order of presentation
    vector<float> correctTrain;       //correctness of first four stories, in order of presentation
    vector<float> correctTest;       //correctness of rest of stories, in order of presentation
    vector<float> cueAgreement;     //agreement with robot gaze cue, in order of presentation
    vector<float> cueAgreementTrain;     //agreement with robot gaze cue, in order of presentation
    vector<float> cueAgreementTest;     //agreement with robot gaze cue, in order of presentation
    vector<string> storyCorrectness(16);    //for each story, is it correct or not?
    vector<string> storyReaction(16);       //for each story, what was the reaction time?
    vector<string> robotAgreement(16);      //for each story, did participant agree with robot gaze cue?
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
        if (id.size() == 1)
        {
            switchYears = 0;
            cout << "PARTICIPANT: " + id[0] << endl;
            participantID = id[0];
        }
        else if (id.size() == 2)
        {
            switchYears = 1;
            cout << "PARTICIPANT: " + id[0] << "\t";
            participantID = id[0];
            condition = id[1];
            cout << "Condition: " << condition << endl;
        }
    }
    catch (std::ios_base::failure &fail)
    {
    	std::cout << "error: problem with input file: " << fail.what() << std::endl;
    }
    inFile.exceptions(std::ios::goodbit);	//disable exception throwing

    //throw away the second line, as this is data header
    getline(inFile, readIn);

    if (switchYears == 1)
    {
        //third line of 2018 logs contains the start time stamp, want this...
        getline(inFile, readIn);
        vector<string> startTimeLine = Splitter(readIn, ',');
        if((startTimeLine[0] == "event") && (startTimeLine.size() == 8))
        {
            startTime = startTimeLine[7];
        }
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
        storyReaction[(int)StoF(s[4])] = s[3];
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
        //gaze cue agreement
        if (switchYears == 1)
        {
            if (condition == "0")
            {
                //congruent condition - gaze cue always correct
                if(answers[(int)StoF(s[4])] == s[1])
                {
                    //answer given matches robot cue
                    cueAgreement.push_back(1.0f);
                    cueAgreementTrain.push_back(1.0f);
                    robotAgreement[(int)StoF(s[4])] = "1";
                }
                else
                {
                    //answer given does not match robot cue
                    cueAgreement.push_back(0.0f);
                    cueAgreementTrain.push_back(0.0f);
                    robotAgreement[(int)StoF(s[4])] = "0";
                }
            }
            else if (condition == "1")
            {
                //incongruent condition - gaze cue always wrong
                if(answers[(int)StoF(s[4])] != s[1])
                {
                    //answer given matches robot cue
                    cueAgreement.push_back(1.0f);
                    cueAgreementTrain.push_back(1.0f);
                    robotAgreement[(int)StoF(s[4])] = "1";
                }
                else
                {
                    //answer given does not match robot cue
                    cueAgreement.push_back(0.0f);
                    cueAgreementTrain.push_back(0.0f);
                    robotAgreement[(int)StoF(s[4])] = "0";
                }
            }
            else
            {
                //something wrong - no such condition
                cout << "ERROR: no such condition - " << condition << endl;
            }
        }
    }

    //rest of the stories now
    score = 0;
    while (getline(inFile, readIn))
    {
        //read until end: each line is now data
    	//std::cout << readIn << std::endl;
        vector<string> s = Splitter(readIn, ',');

        if (s[0] == "data")
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
            storyReaction[(int)StoF(s[4])] = s[3];
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
            //gaze cue agreement
            if (switchYears == 1)
            {
                if (condition == "0")
                {
                    //congruent condition - gaze cue always correct
                    if(answers[(int)StoF(s[4])] == s[1])
                    {
                        //answer given matches robot cue
                        cueAgreement.push_back(1.0f);
                        cueAgreementTest.push_back(1.0f);
                        robotAgreement[(int)StoF(s[4])] = "1";
                    }
                    else
                    {
                        //answer given does not match robot cue
                        cueAgreement.push_back(0.0f);
                        cueAgreementTest.push_back(0.0f);
                        robotAgreement[(int)StoF(s[4])] = "0";
                    }
                }
                else if (condition == "1")
                {
                    //incongruent condition - gaze cue always wrong
                    if(answers[(int)StoF(s[4])] != s[1])
                    {
                        //answer given matches robot cue
                        cueAgreement.push_back(1.0f);
                        cueAgreementTest.push_back(1.0f);
                        robotAgreement[(int)StoF(s[4])] = "1";
                    }
                    else
                    {
                        //answer given does not match robot cue
                        cueAgreement.push_back(0.0f);
                        cueAgreementTest.push_back(0.0f);
                        robotAgreement[(int)StoF(s[4])] = "0";
                    }
                }
                else
                {
                    //something wrong - no such condition
                    cout << "ERROR: no such condition - " << condition << endl;
                }
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
    float agreementAll = Correctness(cueAgreement);
    float agreementTrain = Correctness(cueAgreementTrain);
    float agreementTest = Correctness(cueAgreementTest);

    //need to add padding to some containers if necessary: correctAll, timingAll, cueAgreement
    if (correctAll.size() < 16)
    {
        int padSize = 16 - correctAll.size();
        for (int a = 0; a < padSize; a++)
        {
            correctAll.push_back(-1);
        }
    }
    if (timingAll.size() < 16)
    {
        int padSize = 16 - timingAll.size();
        for (int a = 0; a < padSize; a++)
        {
            timingAll.push_back("null");
        }
    }
    if (cueAgreement.size() < 16)
    {
        int padSize = 16 - cueAgreement.size();
        for (int a = 0; a < padSize; a++)
        {
            cueAgreement.push_back(-1);
        }
    }


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
            if (switchYears == 1)
            {
                //2018 data
                resultsFile << "ID,condition,TrainCorrect,TestCorrect,AllCorrect,TimeMeanTrain,TimeSDTrain,TimeMeanTest,TimeSDTest,TimeMeanAll,TimeSDAll,startTimeStamp,duration,testScore,story correctness,story 1,story 2,story 3,story 4,story 5,story 6,story 7,story 8,story 9,story 10,story 11,story 12,story 13,story 14,story 15,story 16,story presentation order,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,story timings,story 1,story 2,story 3,story 4,story 5,story 6,story 7,story 8,story 9,story 10,story 11,story 12,story 13,story 14,story 15,story 16,timings presentation order,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,AgreementTrain,AgreementTest,AgreementAll,story cue agreement,story 1,story 2,story 3,story 4,story 5,story 6,story 7,story 8,story 9,story 10,story 11,story 12,story 13,story 14,story 15,story 16,cue agreement presentation order,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16," << endl;
            }
            else if (switchYears == 0)
            {
                //2017 data
                resultsFile << "ID,TrainCorrect,TestCorrect,AllCorrect,TimeMeanTrain,TimeSDTrain,TimeMeanTest,TimeSDTest,TimeMeanAll,TimeSDAll,testScore,story correctness,story 1,story 2,story 3,story 4,story 5,story 6,story 7,story 8,story 9,story 10,story 11,story 12,story 13,story 14,story 15,story 16,story presentation order,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,story timings,story 1,story 2,story 3,story 4,story 5,story 6,story 7,story 8,story 9,story 10,story 11,story 12,story 13,story 14,story 15,story 16,timings presentation order,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16" << endl;
            }
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
    if (switchYears == 1)
    {
        resultsFile << condition << ",";
    }
    resultsFile << correctnessTrain << ",";
    resultsFile << correctnessTest << ",";
    resultsFile << correctnessAll << ",";
    resultsFile << Mean(timingTrain) << "," << SD(timingTrain) << ",";
    resultsFile << Mean(timingTest) << "," << SD(timingTest) << ",";
    resultsFile << Mean(timingAll) << "," << SD(timingAll) << ",";
    if (switchYears == 1)
    {
        resultsFile << startTime << ",";
        resultsFile << duration << ",";
    }
    resultsFile << score << ",";
    resultsFile << ",";
    //the actual story correctness (*not* in order of presentation)
    for (int a = 0; a < (int)storyCorrectness.size(); a++)
    {
        resultsFile << storyCorrectness[a] << ",";
    }
    resultsFile << ",";
    //now the individual stories in order of presentation
    for (int a = 0; a < (int)correctAll.size(); a++)
    {
        resultsFile << correctAll[a] << ",";
    }
    resultsFile << ",";
    //now reaction times per story (*not* in order of presentation)
    for (int a = 0; a < (int)storyReaction.size(); a++)
    {
        resultsFile << storyReaction[a] << ",";
    }
    resultsFile << ",";
    //now reaction times in order of presentation
    for (int a = 0; a < (int)timingAll.size(); a++)
    {
        resultsFile << timingAll[a] << ",";
    }
    //now agreement stats for the gaze cueing
    if (switchYears == 1)
    {
        resultsFile << agreementTrain << ",";
        resultsFile << agreementTest << ",";
        resultsFile << agreementAll << ",";
        resultsFile << ",";
        for (int a = 0; a < (int)robotAgreement.size(); a++)
        {
            resultsFile << robotAgreement[a] << ",";
        }
        resultsFile << ",";
        for (int a = 0; a < (int)cueAgreement.size(); a++)
        {
            resultsFile << cueAgreement[a] << ",";
        }
    }

    resultsFile << endl;

    resultsFile.flush();
    resultsFile.close();


    cout << "DONE" << endl;
    cout << endl;

    return 0;
}
