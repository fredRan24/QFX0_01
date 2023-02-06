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
#include "EventTreeItem.h"


class DirectoryDisplay : public Component
{
public:
    DirectoryDisplay();
    ~DirectoryDisplay();

    //Component overrides
    void paint(juce::Graphics& g) override;
    void resized() override;

    //My Public Methods
    static ValueTree createTree (const String& desc, const String& path);
    static ValueTree createRootValueTree();
    void setVisualiser(AudioVisualiser* v);
    void searchTree(ValueTree tree, ValueTree& resultTree, const var& childName);

    //Mouse Callbacks
    void mouseDown (const MouseEvent& event) override;
    
private:
    //Private Member Variables
    TreeView eventTree;
    UndoManager undoManager;
    String tempPath = "";
    
    //Private Pointers
    AudioVisualiser* visualiser = nullptr;
    unique_ptr<EventTreeItem> eventItem;
};

