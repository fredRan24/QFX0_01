/*
  ==============================================================================

    EventCreatorView.h
    Created: 7 Feb 2023 1:22:17pm
    Author:  Alfie Ransome

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Quake.h"

class EventCreator : public Component
{
public:
    //Constuctor and destructor
    EventCreator();
    ~EventCreator();
    
    void setSavePath(String path);
    void createNewEvent(String directoryPath);
    
private:
    String savePath;
    unique_ptr<QuakeProcessor> processor;
};
