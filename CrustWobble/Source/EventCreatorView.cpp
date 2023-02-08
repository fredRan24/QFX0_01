/*
  ==============================================================================

    EventCreatorView.cpp
    Created: 7 Feb 2023 1:22:17pm
    Author:  Alfie Ransome

  ==============================================================================
*/

#include "EventCreatorView.h"
EventCreator::EventCreator()
{
    
}

EventCreator::~EventCreator()
{
    
}

void EventCreator::paint(juce::Graphics& g)
{
    g.fillAll(Colours::darkslateblue);
}

void EventCreator::resized()
{
    
}

void EventCreator::setSavePath(String path)
{
    savePath = path;
}

void EventCreator::createNewEvent(String directoryPath)
{
    //Create a new processor object for us to use to process this new dir.
    processor.reset(new QuakeProcessor());
    
    bool success = processor->loadDIR(directoryPath);
    if(success)
    {
        processor->processDIR();
    }
    else
        DBG("ERROR: failed to load the supplied directory into the processor unit...");
}
