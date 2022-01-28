/*
  ==============================================================================

    Quake.h
    Created: 28 Oct 2021 6:44:05pm
    Author:  Alfie

  ==============================================================================
*/
#include <JuceHeader.h>

using namespace juce;
using namespace std;


#pragma once
/*
==============================================================================
                                     QuakeProcessor
==============================================================================
*/
class QuakeProcessor
{
public:
    QuakeProcessor();
    ~QuakeProcessor();
    
    bool loadDIR(String pathOfSelectedDIR);
    void processDIR();
    
private:
    /*
    ==============================================================================
                                    Member Variables
    ==============================================================================
    */
    String dirPath;
    File stationFile;
    Array<File> files;
    StringArray folderPaths, codes, filenameTemplates, folderNames, savePaths;
   
    int numFiles = 0;
    
    /** unique_ptr to a AudioBuffer<float> object

        This will be used to stream data into a .wav file using an AudioFormatWriter

        @see wavFileWriter, process
    */
    unique_ptr<AudioBuffer<float>> floatAudioBuffer;

    /** unique_ptr to a AudioFormatWriter object

        This will be used to fill a .wav file from floatAudioBuffer

        @see floatAudioBuffer, process
    */
    unique_ptr<AudioFormatWriter> wavFileWriter;
    
    /*
    ==============================================================================
                                    Private Functions
    ==============================================================================
    */
    void readStationFile();
    void makeFolders();
    void makeSaveLocations();
    void getFloatArrayFromFile(String pathOfFileToInterpret, Array<float>* arrayToFill);
    void processStationFileLine(String& folderNameOut, String& codesOut, String& filenameTemplatesOut, std::string lineToProcess);

    /** Processes a given Array<float> into a WAV file at location savePath

        @param floatArrayToProcess:     The Array<float> to process.
        @param savePath:                The path at which to save the .wav file.
                                        The path be complete with unique fileName
        @see getFloatArrayFromFile
    */
    void processFloatArray(Array<float>* floatArrayToProcess, String savePath);

    /** Converts a string to a usable float value
    
        This process s also fundamentally slow, but for this use case, the time delay is
        not noticable. The files ore often only 45000 samples long. Hence the readme.txt's
        reccommendations.
    */
    float stringToFloat(string str);

    /** Returns true if a float is > 0

    Used in convertStringToResizedFloat
    @param floatToTestIfPositive    the float to be checked

    @see convertStringToResizedFloat
*/
    bool isPositive(float floatToTestIfPositive);

    /** Returns input * -1 (Makes Megative)

        Used in convertStringToResizedFloat
        @param floatToMakeNegative        the float to make negative

        @see convertStringToResizedFloat
    */
    float makeNegative(float floatToMakeNegative);
};

