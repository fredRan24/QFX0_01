/*
  ==============================================================================

    Controller.h
    Created: 29 Dec 2021 11:16:51pm
    Author:  Alfie Ransome

  ==============================================================================
*/

#pragma once
#include "Quake.h"
#include "View.h"
#include "Audio.h"

class Controller :  public FilenameComponentListener,
                    public Button::Listener
{
public:
    Controller(MainView& view, QuakeProcessor& convertr);
    ~Controller();
    
    void filenameComponentChanged (FilenameComponent* fileComponentThatHasChanged) override;
    void buttonClicked (Button*) override;
    
private:
    MainView* mainViewObj;
    QuakeProcessor* converter;
};
