/*
  ==============================================================================

    Controller.cpp
    Created: 29 Dec 2021 11:16:51pm
    Author:  Alfie Ransome

  ==============================================================================
*/

#include "Controller.h"

Controller::Controller(MainView& view, QuakeProcessor& convertr)
{
    mainViewObj = &view;
    converter = &convertr;

    //mainViewObj->getDirChooser()->addListener(this);
    //mainViewObj->getConvertButton()->addListener(this);
}

Controller::~Controller()
{
    
}

void Controller::filenameComponentChanged (FilenameComponent* fileComponentThatHasChanged)
{
    /*
    if(fileComponentThatHasChanged == mainViewObj->getDirChooser())
    {
        converter->loadDIR(mainViewObj->getDirChooser()->getCurrentFile().getFullPathName());
        DBG("DIR selector filenamecompoonent callback triggered...");
    }
     */
}

void Controller::buttonClicked (Button* b)
{
    /*
    if(b == mainViewObj->getConvertButton())
    {
        converter->processDIR();
        DBG("Convert button pressed!");
    }
     */
}
 
