/*
  ==============================================================================

    Controller.cpp
    Created: 29 Dec 2021 11:16:51pm
    Author:  Alfie Ransome

  ==============================================================================
*/

#include "Controller.h"

Controller::Controller(MainView& view, W3DIRConverter& convertr)
{
    mainViewObj = &view;
    converter = &convertr;

}

Controller::~Controller()
{
    
}

void Controller::filenameComponentChanged (FilenameComponent* fileComponentThatHasChanged)
{
    if(fileComponentThatHasChanged == mainViewObj->getDirChooser())
    {
        DBG("A file has changed!!");
    }
}
