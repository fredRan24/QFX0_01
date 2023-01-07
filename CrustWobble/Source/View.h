/*
  ==============================================================================

    View.h
    Created: 28 Oct 2021 6:43:51pm
    Author:  Alfie

  ==============================================================================
*/
#pragma once
#include "Visualiser.h"
#include "Controls.h"
#include "FileBar.h"
#include "EventListView.h"
#include <JuceHeader.h>

using namespace std;
using namespace juce;


// ----------------------------------------------------------


class TopWindowView : public Component
{
public:
    TopWindowView();
    ~TopWindowView();
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void updateVisualiser();
    AudioVisualiser* getVisualiser();
    
private:
    AudioVisualiser visualiser;
    ControlsView controls;
};


// ----------------------------------------------------------


class MainView : public Component
{
public:
    MainView();
    ~MainView();

    //Overrides
    void paint(juce::Graphics& g) override;
    void resized() override;
    
private:
    
    FileMenuBar fileMenuBar;
    TopWindowView topWindow;
    DirectoryDisplay dirDisplay;
};


// ----------------------------------------------------------


