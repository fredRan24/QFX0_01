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

class Controller : private FilenameComponentListener
{
public:
    Controller(MainView& view, W3DIRConverter& convertr);
    ~Controller();
    
    void filenameComponentChanged (FilenameComponent* fileComponentThatHasChanged) override;
    
private:
    MainView* mainViewObj;
    W3DIRConverter* converter;
};
