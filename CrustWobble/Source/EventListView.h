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

    void paint(juce::Graphics& g) override;
    void resized() override;

    static ValueTree createTree (const String& desc, const String& path);
    static ValueTree createRootValueTree();
    
    void setVisualiser(AudioVisualiser* v);
    
    void mouseDown (const MouseEvent& event) override;
    
    void searchTree(ValueTree tree, ValueTree& resultTree, const var& childName);
    
    
private:
    TreeView eventTree;
    UndoManager undoManager;
    AudioVisualiser* visualiser = nullptr;
    String tempPath = "";
    
    unique_ptr<EventTreeItem> eventItem;
};

