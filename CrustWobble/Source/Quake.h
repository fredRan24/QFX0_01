/*
  ==============================================================================

    Quake.h
    Created: 28 Oct 2021 6:44:05pm
    Author:  Alfie

  ==============================================================================
*/
#include <JuceHeader.h>

using namespace juce;

#pragma once
class Wilber3DIR
{
public:
    Wilber3DIR();
    ~Wilber3DIR();
    
    void checkDIR();
    
    void getNumFiles();
    void checkFiletype();
    void locateStationFile();
    
    void readStationFile();
    
private:
    String W3DIRName = "";
    StringArray files;
    String stationFilePath = "";
};

class W3DIRConverter
{
public:
    W3DIRConverter();
    ~W3DIRConverter();
    
    void convertW3DIR(Wilber3DIR directory);
    
private:
    
};

