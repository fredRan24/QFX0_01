/*
  ==============================================================================

    Properties.h
    Created: 28 Dec 2022 2:09:58pm
    Author:  Alfie Ransome

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Properties {
public:
    Properties();
    ~Properties();
    
    static const Identifier ROOT;
    
    static const Identifier VIEWS;
    static const Identifier FILEBAR;
    static const Identifier LISTWINDOW;
    static const Identifier VISUALISER;
    static const Identifier CONTROLS;
    
    static const Identifier EVENTROOT;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Properties);
};
