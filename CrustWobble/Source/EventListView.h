/*
  ==============================================================================

    EventListView.h
    Created: 7 Jan 2023 10:00:55pm
    Author:  Alfie Ransome

  ==============================================================================
*/

#pragma once
using namespace std;
#include <JuceHeader.h>
#include "Visualiser.h"
#include "Yggdrasil.h"
#include "EventTreeItem.h"


class DirectoryDisplay : public Component
{
public:
    DirectoryDisplay(Yggdrasil& yggdrasil);
    ~DirectoryDisplay();

    //Component overrides
    void paint(juce::Graphics& g) override;
    void resized() override;

    //My Public Methods
    void setVisualiser(AudioVisualiser* v);
    void setYggdrasil(Yggdrasil& tree);

    //Mouse Callbacks
    void mouseDown (const MouseEvent& event) override;
    
private:
    //Private Member Variables
    TreeView eventTree;
    UndoManager undoManager;
    
    
    //Private Pointers
    AudioVisualiser* visualiser = nullptr;
    unique_ptr<Yggdrasil> yggdrasil;
};

