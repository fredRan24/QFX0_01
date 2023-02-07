/*
  ==============================================================================

    EventListView.cpp
    Created: 7 Jan 2023 10:00:55pm
    Author:  Alfie Ransome

  ==============================================================================
*/

#include "EventListView.h"
//////////////////////////////////////////////////
// DirectoryDisplay Component
//////////////////////////////////////////////////

DirectoryDisplay::DirectoryDisplay(Yggdrasil& t)
{
    //make the EventTree visible
    addAndMakeVisible(eventTree);
    
    //set up the EventTree with its name, set them to be automatically closed and disable multiselect
    eventTree.setTitle("EventTree");
    eventTree.setDefaultOpenness (false);
    eventTree.setMultiSelectEnabled (false);
    
    yggdrasil.reset(&t);
    
    const ValueTree vt = yggdrasil.get()->getEventViewTree();
    
    //Set the root item of the EventTree to be the newly created tree
    eventTree.setRootItem(yggdrasil.get()->getEventViewItemPointer());
    
    //Add a listener to the tree, so we can tell if the user clicks somewhere over the TreeView eventTree
    eventTree.addMouseListener(this, true);
}

//destructor
DirectoryDisplay::~DirectoryDisplay()
{
    //release the EventItem memory just for security
    yggdrasil.release();
}

void DirectoryDisplay::paint(juce::Graphics& g)
{
    //set a background colour for the whole component
    g.fillAll(Colours::darkslateblue);
}

void DirectoryDisplay::resized()
{
    //Set the bounds for the TreeView to be the size of this component
    auto r = getLocalBounds();
    eventTree.setBounds(r);
}


void DirectoryDisplay::setVisualiser(AudioVisualiser* v)
{
    visualiser = v;
}

void DirectoryDisplay::mouseDown (const MouseEvent& event)
{
    DBG("SUCCESS: MouseDown Triggered...");
    
    //If the mouse was clicked
    if(event.mouseWasClicked())
    {
        //Get the number of selected items from the TreeView
        auto numSelected = eventTree.getNumSelectedItems();
        
        //If there is only one item selected
        if(numSelected == 1)
        {
            //Get that selected item
            auto selectedItem = eventTree.getSelectedItem(0);
            
            //Get the selected item's name
            auto selecteditemsName = selectedItem->getUniqueName();
            DBG("Selected ITem Name: " + selecteditemsName);
            
            //Create a var with that name
            const var v = selecteditemsName;
            
            //Get the Root ValueTree which the eventItem is pointing to
            auto theTree = *(yggdrasil.get()->getEventViewItemPointer()->getThisTree());
            //DBG("The tree: "+ theTree.toXmlString());
            
            //Create a local ValueTree which will be populated by the searchTree function
            ValueTree theChild;
            
            //Search through the tree to find the correct child, which will be assigned to theChild variable
            yggdrasil.get()->searchTree(theTree, theChild, v);
            
            //Get the path property from the item we were looking for
            auto path = theChild.getProperty("fullPathname").toString();
            //DBG("FilePath im about to feed to the visualiser: " + path);
            
            //Check if the file is a .wav file by checking its path string, if it is, return because we dont want to do anything with it
            if(!path.contains(".wav"))
                return;
                
            //As long as the visualiser exists
            if(visualiser != nullptr)
            {
                //Tell the visualiser to load the audio data from the current file, which we know is a .wav file
                visualiser->loadData(path);
            }
        }
        else
            DBG("ERROR: Nothing, or more than one thing was selected in the TreeView...");
    }
    else
        DBG("mouseWasntClicked...");
}

void DirectoryDisplay::setYggdrasil(Yggdrasil& tree)
{
    yggdrasil.reset(&tree);
}
