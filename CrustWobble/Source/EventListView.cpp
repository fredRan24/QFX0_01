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

DirectoryDisplay::DirectoryDisplay()
{
    addAndMakeVisible(eventTree);
    eventTree.setTitle("EventTree");
    eventTree.setDefaultOpenness (false);
    eventTree.setMultiSelectEnabled (false);
    eventItem.reset (new EventTreeItem (createRootValueTree(), undoManager));
    eventTree.setRootItem(eventItem.get());
    eventTree.addMouseListener(this, true);
    
}

DirectoryDisplay::~DirectoryDisplay()
{

}

void DirectoryDisplay::paint(juce::Graphics& g)
{
    g.fillAll(Colours::darkslateblue);
}

void DirectoryDisplay::resized()
{
    auto r = getLocalBounds();
    eventTree.setBounds(r);
}

ValueTree DirectoryDisplay::createTree (const String& desc, const String& path)
{
    ValueTree t ("Item");
    t.setProperty ("name", desc, nullptr);
    t.setProperty ("fullPathname", path, nullptr);
    return t;
}

ValueTree DirectoryDisplay::createRootValueTree()
{
    String path = "/Users/alfie/Desktop/Personal Projects/CrustWobble/QFX0_01/CrustWobble/Events";
    File eventDirectory = File(path);

    auto vt = createTree ("Events", "DIR");
    unique_ptr<ValueTree> currentParent;
    currentParent.reset(new ValueTree(vt));
    WavAudioFormat wavFormat;

    if (eventDirectory.exists())
    {
        for (DirectoryEntry entry : RangedDirectoryIterator(File(eventDirectory), true, "*", File::findFilesAndDirectories))
        {
            File temp = entry.getFile();
            if (temp.exists())
            {
                String dirString = temp.getFullPathName();
                String nameString = temp.getFileName();
                String contentString = "DIR";
                
                auto parent = temp.getParentDirectory();
                String parentName = parent.getFileName();
                String vtParentName = currentParent->getProperty("name").toString();
                
                while (parentName != currentParent->getProperty("name").toString())
                {
                    DBG("ParentDIR: " + parentName + "  ParentVT:  " + vtParentName);
                    auto p = currentParent->getParent();
                    if(p.isValid())
                        currentParent.reset(new ValueTree(p));
                    else
                        currentParent.reset(new ValueTree(vt));
                    DBG(p.getProperty("name").toString());
                }
                
                auto newTree = createTree(nameString, dirString);
                currentParent->appendChild(newTree, nullptr);
                
                currentParent.reset(new ValueTree(newTree));
                
                DBG(nameString + ":  " + dirString);
            }
            else
                DBG("A FILE IN THE DIRECTORY SUPPLIED WAS NOT REAL SOMEHOW");
            }
        }
        else
            DBG("The directory selected as event did not exist");
    
    currentParent.reset(currentParent.release());
    
    return vt;
}

void DirectoryDisplay::setVisualiser(AudioVisualiser* v)
{
    visualiser = v;
}

void DirectoryDisplay::mouseDown (const MouseEvent& event)
{
    DBG("MouseDown Triggered...");
    if(event.mouseWasClicked())
    {
        auto numSelected = eventTree.getNumSelectedItems();
        String num = to_string(numSelected);
        DBG("Number of selected items: " + num);

        if(numSelected>0)
        {
            auto selectedItem = eventTree.getSelectedItem(0);
            auto selecteditemsName = selectedItem->getUniqueName();
            DBG("Selected ITem Name: " + selecteditemsName);
            const var v = selecteditemsName;
            auto theTree = ValueTree(*(eventItem.get()->getThisTree()));
            
            //DBG("The tree: "+ theTree.toXmlString());
            ValueTree theChild;
            searchTree(theTree, theChild, v);
            auto path = theChild.getProperty("fullPathname").toString();
        
            DBG("FilePath im about to feed to the visualiser: " + path);
            
            if(!path.contains(".wav"))
                return;
                
            if(visualiser != nullptr)
            {
                visualiser->loadData(path);
            }
        }
        else
            DBG("There wasnt anything selected...");
    }
    else
        DBG("mouseWasntClicked...");
}

void DirectoryDisplay::searchTree(ValueTree tree, ValueTree& resultTree, const var& childName)
{
    const Identifier id = "name";
    for (const auto& child : tree)
    {
        auto check = child.getChildWithProperty(id, childName);
        if(check.isValid())
        {
            DBG("Found a valid child for ya!");
            resultTree = check;
        }
        else
            searchTree(child, resultTree, childName);
    }
}
