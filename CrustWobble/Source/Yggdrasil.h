/*
  ==============================================================================

    yggdrasil.h
    Created: 28 Dec 2022 2:02:18pm
    Author:  Alfie Ransome

  ==============================================================================
*/

#pragma once
#include "Properties.h"
#include <JuceHeader.h>

class Yggdrasil : private Properties
{
public:
    Yggdrasil();
    ~Yggdrasil();
    
    void growNewTree();
    
    ValueTree* getROOT();

private:
    ValueTree treeROOT;
    ValueTree treeVIEWS;
    ValueTree treeFILEBAR;
    ValueTree treeLISTWINDOW;
    ValueTree treeVISUALISER;
    ValueTree treeCONTROLS;
    ValueTree treeEVENTROOT;
    
    ValueTree* rootPointer = &treeROOT;
    
    String rootDIR = "";
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Yggdrasil);
};
