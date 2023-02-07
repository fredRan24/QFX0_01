/*
  ==============================================================================

    View.cpp
    Created: 28 Oct 2021 6:43:51pm
    Author:  Alfie

  ==============================================================================
*/

#include "View.h"
using namespace std;


//////////////////////////////////////////////////
//       MainView
//////////////////////////////////////////////////

MainView::MainView(Yggdrasil& yggdrasil) : dirDisplay(yggdrasil)
{
    addAndMakeVisible(fileMenuBar);
    addAndMakeVisible(topWindow);
    addAndMakeVisible(dirDisplay);
    dirDisplay.setVisualiser(topWindow.getVisualiser());
    eventCreator.setSavePath(yggdrasil.getRootDIR());
}

//destructor
MainView::~MainView() {}

//Overrides
void MainView::paint(juce::Graphics& g)
{
    g.fillAll(Colours::white);
}
                                                   
void MainView::resized()
{
    auto window = getLocalBounds();

    auto menuBarBounds = window.removeFromTop(25);
    fileMenuBar.setBounds(menuBarBounds);
    
    auto topSectionBounds = window.removeFromTop(window.getHeight()/5);
    topWindow.setBounds(topSectionBounds);
    
    
    dirDisplay.setBounds(window.reduced(10,10));
}

//////////////////////////////////////////////////
// TopWindowView Component
//////////////////////////////////////////////////

TopWindowView::TopWindowView() {
    addAndMakeVisible(visualiser);
    addAndMakeVisible(controls);
}

TopWindowView::~TopWindowView() {
    
}

void TopWindowView::updateVisualiser() {
    
}

void TopWindowView::paint(juce::Graphics& g)
{
    g.fillAll(Colours::yellowgreen);
}

void TopWindowView::resized()
{
    auto segment = getLocalBounds();
    
    auto logoBounds = segment.removeFromLeft(segment.getWidth()/6);
    
    auto controlBounds = segment.removeFromLeft(segment.getWidth()/4);
    controls.setBounds(controlBounds.reduced(10,10));
    
    visualiser.setBounds(segment.reduced(10,10));
}

AudioVisualiser* TopWindowView::getVisualiser()
{
    return &visualiser;
}
