/*
  ==============================================================================

    Quake.cpp
    Created: 28 Oct 2021 6:44:05pm
    Author:  Alfie

  ==============================================================================
*/

#include "Quake.h"
using namespace std;

/*
==============================================================================
                            QuakeProcessor
==============================================================================
*/
QuakeProcessor::QuakeProcessor() : stationFile()
{

}

QuakeProcessor::~QuakeProcessor() {
    
}

/*
==============================================================================
                            Public Functions
==============================================================================
*/
/*                              LOAD DIR                                    */
bool QuakeProcessor::loadDIR(String pathOfSelectedDIR) {
    bool iFoundTheStationFile = false;
    dirPath = pathOfSelectedDIR;
    File tempDir = File(dirPath);
    
    if (tempDir.exists() && tempDir.isDirectory())
    {
        //Get all the .txt files in the directory
        Array<File> filesInDir = tempDir.findChildFiles(File::TypesOfFileToFind::findFiles, false, "*.txt");
        
        //Go through the directory
        for (int i = 0; i<filesInDir.size(); i++)
        {
            //double check the file exists and is a .txt file
            if(filesInDir[i].existsAsFile() && filesInDir[i].getFileExtension() == ".txt")
            {
                //Check if the file is the station file
                if(filesInDir[i].getFullPathName().contains("wilber-stations.txt"))
                {
                    //If it is, change the found variable, set the local file variable, remove it from the file array
                    iFoundTheStationFile = true;
                    stationFile = File(filesInDir[i]);
                    filesInDir.remove(i);
                    
                    DBG("The station file has been found...");
                }
                else
                {
                    files.add(filesInDir[i]);
                }
            }
        }
        if(iFoundTheStationFile == true)
        {
            //if all ok, return true
            numFiles = filesInDir.size();
            DBG("All Good! loaded the DIR...");
            return true;
        }
        else {
            DBG("Didnt find a station file...");
            return false;
        }
    }
    else {
        DBG("The DIR does not exist...");
        return false;
    }
}

/*                              PROCESS DIR                                    */
void QuakeProcessor::processDIR()
{
    if(files.size()>0 && stationFile.existsAsFile())
    {
        readStationFile();
        makeFolders();
        makeSaveLocations();
        
        unique_ptr<Array<float>> floatArrayToUse(new Array<float>());
        Array<float>* floatArrayToUsePtr = floatArrayToUse.get();

        //Iterate through the files
        for (auto i = 0; i < files.size(); i++) {
            //process them
            getFloatArrayFromFile(files[i].getFullPathName(), floatArrayToUsePtr);
            processFloatArray(floatArrayToUsePtr, savePaths[i]);
            
            //clear the FloatArray for the next file
            floatArrayToUse->clear();
        }
    }
}

/*
==============================================================================
                            Private Functions
==============================================================================
*/

/*                           Read Station File                             */
void QuakeProcessor::readStationFile()
{
    StringArray linesOfTheFile;

    //Read the lines of the file into a StringArray
    stationFile.readLines(linesOfTheFile);

    //Iterate through those lines
    for (auto i = 1; i < linesOfTheFile.size(); i++)
    {
        string thisLine = linesOfTheFile[i].toStdString();

        //Not the header
        if (i > 0)
        {
            String newFolderName = "", newStationCode = "", newFilenameTP = "";
            processStationFileLine(newFolderName, newStationCode, newFilenameTP, thisLine);
            
            folderNames.add(newFolderName);
            codes.add(newStationCode);
            filenameTemplates.add(newFilenameTP);
            
        }
        else {
            DBG("Something broke in readStationFile.. \n");
        }
    }
}

void QuakeProcessor::makeFolders()
{
    for(int i = 0; i<folderNames.size(); i++)
    {
        String fullPath = dirPath;
        String wavFolderName;

        if ((SystemStats::getOperatingSystemType() & SystemStats::Windows) != 0)
        {
            wavFolderName = "\\Converted WAV Files\\";

        }
        else if ((SystemStats::getOperatingSystemType() & SystemStats::MacOSX) != 0)
        {
            wavFolderName = "/Converted WAV Files/";
        }
        else
        {
            DBG("Did not recognise the operating system...");
            return;
        }

        fullPath.append(wavFolderName, wavFolderName.length());
        fullPath.append(folderNames[i], folderNames[i].length());
        File newFolder = File(fullPath);
        if(!newFolder.isDirectory())
        {
            folderPaths.add(fullPath);
            newFolder.createDirectory();
            DBG("Created a folder at " << fullPath);
        }
    }
}

void QuakeProcessor::makeSaveLocations()
{
    for(int i = 0; i<files.size(); i++)
    {
        String fullPathName = files[i].getFullPathName();
        for (int j = 0; j<codes.size(); j++)
        {
            if (fullPathName.contains(codes[j]))
            {
                File correctDirectory = File(folderPaths[j]);
                String savePathToAdd = folderPaths[j];

                String fileNumber = "";

                if ((SystemStats::getOperatingSystemType() & SystemStats::Windows) != 0)
                {
                    savePathToAdd.append("\\", 1);
                }
                else if((SystemStats::getOperatingSystemType() & SystemStats::MacOSX) != 0)
                {
                    savePathToAdd.append("/", 1);
                }
                else
                {
                    DBG("Did not recognise the operating system...");
                    return;
                }

                savePathToAdd.append(filenameTemplates[j], filenameTemplates[j].length());
                savePathToAdd.append("-0.wav", 6);
                
                DBG("SavePath Created for: " << savePathToAdd);
                savePaths.add(savePathToAdd);
            }
        }
    }
    if(files.size() == savePaths.size())
    {
        DBG("The # of files matches # of savePaths...");
    }
}

void QuakeProcessor::getFloatArrayFromFile(String pathOfFileToInterpret, Array<float>* arrayToFill)
{
    File fileToInterpret(pathOfFileToInterpret);
    StringArray linesOfTheFile = StringArray();

    //If the file exists
    if (fileToInterpret.existsAsFile())
    {
        //Read the lines from the file
        fileToInterpret.readLines(linesOfTheFile);

        //Check that its not too short
        if (!(linesOfTheFile.size() > 1)) {
            DBG("There werent enough lines in that file...");
            return;
        }

        //Iterate through the lines of the file
        for (auto i = 0; i < linesOfTheFile.size() - 1; i++)
        {
            string stringFromLine = linesOfTheFile[i].toStdString();
            //If not the header
            if (i > 0)
            {
                //Convert the line to a float
                float valueForStoring = stringToFloat(stringFromLine);
                //Append to FloatArray
                arrayToFill->add(valueForStoring);
            }
        }
    }
    else
        DBG("The file for pulling a Float Array did not exist...");
}

void QuakeProcessor::processStationFileLine(String& folderNameOut, String& codesOut, String& filenameTemplatesOut, string lineToProcess)
{
    size_t  lineStart = 0, firstSep = 0, secondSep = 0, thirdSep = 0, fourthSep = 0,
        fifthSep = 0, sixthSep = 0, seventhSep = 0, lastSep = 0, lineEnd = lineToProcess.length();
    String Lat = "", Long = "", Azimuth = "", StationName = "";
    String folderName = "", station = "", fileNameTemplate = "";

    //Find seperators
    firstSep = lineToProcess.find_first_of('|');
    secondSep = lineToProcess.find('|', firstSep + 1);
    thirdSep = lineToProcess.find('|', secondSep + 1);
    fourthSep = lineToProcess.find('|', thirdSep + 1);
    fifthSep = lineToProcess.find('|', fourthSep + 1);
    sixthSep = lineToProcess.find('|', fifthSep + 1);
    seventhSep = lineToProcess.find('|', sixthSep + 1);
    lastSep = lineToProcess.find('|', seventhSep + 1);

    //Create substrings and station code
    station = lineToProcess.substr(lineStart, firstSep - 1);
    Lat = lineToProcess.substr(thirdSep + 1, ((fourthSep - 1) - (thirdSep + 1)));
    Long = lineToProcess.substr(fourthSep + 1, ((fifthSep - 1) - (fourthSep + 1)));
    Azimuth = lineToProcess.substr(fifthSep + 1, ((sixthSep - 1) - (fifthSep + 1)));
    StationName = lineToProcess.substr(lastSep + 1, (lineEnd - (lastSep + 1)));

    //Create folderName
    folderName.append(StationName, StationName.length());
    folderName.append(" | ", 1);
    folderName.append(station, station.length());
    
    //Create Corresponding Filenametemplate
    fileNameTemplate.append(StationName, StationName.length());
    
    folderNameOut = folderName;
    codesOut = station;
    filenameTemplatesOut = fileNameTemplate;
}

void QuakeProcessor::processFloatArray(Array<float>* floatArrayToProcess, String savePath)
{
    const size_t arraySize = floatArrayToProcess->size();

    floatAudioBuffer.reset(new AudioBuffer<float>());
    floatAudioBuffer->setSize(1, (int)arraySize);
    floatAudioBuffer->clear();

    float* tempPtr = const_cast<float*>(floatArrayToProcess->getRawDataPointer()); //right thing to do??

    //Fill a buffer with the floats
    if (arraySize > 0)
        floatAudioBuffer->addFrom(0, 0, tempPtr, (int)arraySize);
    else {
        DBG("The FloatArray was empty..");
        return;
    }

    //Create an immaginary file at a savePath
    int counter = 0;
    while(File(savePath).existsAsFile())
    {
        String newPath = savePath.dropLastCharacters(5);
        counter++;
        newPath.append(to_string(counter), 1);
        newPath.append(".wav", 4);
        savePath = newPath;
    }
    File fileToWriteInto(savePath);
    String relativePath = savePath;

    WavAudioFormat format;

    //Create a writer to that file
    wavFileWriter.reset(format.createWriterFor(new FileOutputStream(fileToWriteInto),
        2500,
        floatAudioBuffer->getNumChannels(),
        24,
        {},
        0));

    AudioBuffer<float>& readptr = *floatAudioBuffer;

    //Write to the file from buffer
    if (wavFileWriter != nullptr)
        wavFileWriter->writeFromAudioSampleBuffer(readptr, 0, floatAudioBuffer->getNumSamples());

    //Release Resources
    wavFileWriter.reset();
    floatAudioBuffer.reset();
    //DBG("FileProcessed Successfully");
}

float QuakeProcessor::stringToFloat(string str)
{
    float valueForStoring = 0.f;
                                                                                         
    float lineStringAsfloat = stof(str);
    float absConvertedValue = (abs(lineStringAsfloat)) * 0.00005;

    if (isPositive(lineStringAsfloat) != false)
        valueForStoring = absConvertedValue;
    else
        valueForStoring = (makeNegative(absConvertedValue));
    return valueForStoring;
}

bool QuakeProcessor::isPositive(float floatToTestIfPositive)
{
    return floatToTestIfPositive > 0.f;
}

float QuakeProcessor::makeNegative(float floatToMakeNegative)
{
    return floatToMakeNegative = floatToMakeNegative * -1.f;
}



