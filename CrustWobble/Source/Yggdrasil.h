/*
  ==============================================================================

    yggdrasil.h
    Created: 28 Dec 2022 2:02:18pm
    Author:  Alfie Ransome

  ==============================================================================
*/

#pragma once
#include "Properties.h"
#include <JuceHeader.h>
#include "EventTreeItem.h"
using namespace std;

class Yggdrasil : private Properties
{
public:
    Yggdrasil();
    ~Yggdrasil();
    
    void growNewTree();
    ValueTree createTree (const String& desc, const String& path);
    void createFreshEventTree();
    void searchTree(ValueTree tree, ValueTree& resultTree, const var& childName);
    
    ValueTree getEventViewTree();
    EventTreeItem* getEventViewItemPointer();
    UndoManager* getUndoManager();
    
    ValueTree* getROOT();
    
    void findRootDIR();
    String getRootDIR();

private:
    ValueTree treeROOT;
    ValueTree treeVIEWS;
    ValueTree treeFILEBAR;
    ValueTree treeLISTWINDOW;
    ValueTree treeVISUALISER;
    ValueTree treeCONTROLS;
    ValueTree treeEVENTROOT;
    
    unique_ptr<EventTreeItem> eventItem;
    UndoManager undoManager;
    
    ValueTree* rootPointer = &treeROOT;
    
    String rootDIR = "";
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Yggdrasil);
};
