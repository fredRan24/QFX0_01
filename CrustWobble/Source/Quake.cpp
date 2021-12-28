/*
  ==============================================================================

    Quake.cpp
    Created: 28 Oct 2021 6:44:05pm
    Author:  Alfie

  ==============================================================================
*/

#include "Quake.h"

/*
==============================================================================
                                Wilber3DIR
==============================================================================
*/
Wilber3DIR::Wilber3DIR(String pathOfSelectedDIR) {
    dirPath = pathOfSelectedDIR;
}

Wilber3DIR::~Wilber3DIR() {
    
}

bool Wilber3DIR::loadDIR() {
    bool iFoundTheStationFile = false;
    
    //Check that its a valid folder path and not a file
    File potentialEventDIR = File(dirPath);
    if (potentialEventDIR.exists())
    {
        for (DirectoryEntry entry : RangedDirectoryIterator(File(potentialEventDIR), false))
        {
            File tempFile = entry.getFile();
            
            //Check if the dir only contains .txt files
            if(tempFile.existsAsFile() && tempFile.getFileExtension() == ".txt")
            {
                DBG("The file tested exists as a .txt file...");
                //Check if the file is the station file
                if(tempFile.getFullPathName().contains("wilbur-stations.txt"))
                {
                    //If it is, change the variable
                    iFoundTheStationFile = true;
                    DBG("The station file has been found...");
                }
            }
            else
            {
                DBG("The file does not exist or is not a .txt file...");
                return false;
            }
        }
        if(iFoundTheStationFile == true)
        {
            //if all ok, return true
            return true;
            DBG("All Good! loaded the DIR...");
        }
        else {
            return false;
        }
    }
    else {
        return false;
        DBG("The DIR does not exist...");
    }
}

void Wilber3DIR::getNumFiles() {
    numFiles = files.size();
}


void Wilber3DIR::readStationFile() {
    
}
/*
==============================================================================
                                W3DIRConverter
==============================================================================
*/

W3DIRConverter::W3DIRConverter() {
    
}

W3DIRConverter::~W3DIRConverter() {
    
}

void W3DIRConverter::convertW3DIR(Wilber3DIR directory) {
    
}
                                                        
