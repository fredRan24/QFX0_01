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
    //make the EventTree visible
    addAndMakeVisible(eventTree);
    
    //set up the EventTree with its name, set them to be automatically closed and disable multiselect
    eventTree.setTitle("EventTree");
    eventTree.setDefaultOpenness (false);
    eventTree.setMultiSelectEnabled (false);
    
    //load a root tree from the local events directory
    eventItem.reset (new EventTreeItem (createRootValueTree(), undoManager));
    
    //Set the root item of the EventTree to be the newly created tree
    eventTree.setRootItem(eventItem.get());
    
    //Add a listener to the tree, so we can tell if the user clicks somewhere over the TreeView eventTree
    eventTree.addMouseListener(this, true);
    
}

//destructor
DirectoryDisplay::~DirectoryDisplay()
{
    //release the EventItem memory just for security
    eventItem.reset(eventItem.release());
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

ValueTree DirectoryDisplay::createTree (const String& desc, const String& path)
{
    //create a fresh ValueTree with the ID item
    ValueTree t ("Item");
    
    //set its name and pathname properties from the arguments the function takes
    t.setProperty ("name", desc, nullptr);
    t.setProperty ("fullPathname", path, nullptr);
    
    //return the new tree
    return t;
}

ValueTree DirectoryDisplay::createRootValueTree()
{
    //Absolute Path to my Events directory - NEEDS REPLACING
    String path = "/Users/alfie/Desktop/Personal Projects/CrustWobble/QFX0_01/CrustWobble/Events";
    
    //WavFormatReader for use in checking if a file is a .wav file
    WavAudioFormat wavFormat;
    
    // Get the directory for the Events folder
    File eventDirectory = File(path);

    //Create a RootTree called Events with the pathname DIR
    auto vt = createTree ("Events", "DIR");
    
    //Declare a local unique_ptr which will be used to remember which child VT we are at when we want to append the next child VY
    unique_ptr<ValueTree> currentParent;
    
    //Set the unique_ptr to the root tree we just created
    currentParent.reset(new ValueTree(vt));
    
    //If the Event directory exists
    if (eventDirectory.exists())
    {
        //Iterate over the directory, grabbing nested files and Directories
        for (DirectoryEntry entry : RangedDirectoryIterator(File(eventDirectory), true, "*", File::findFilesAndDirectories))
        {
            //Get the item at each iteration
            File temp = entry.getFile();
            
            //Check if it exists
            if (temp.exists())
            {
                //Get the pathname and the filename/dirname from the item
                String dirString = temp.getFullPathName();
                String nameString = temp.getFileName();
                
                //initialise a string to be used for content
                String contentString = "DIR";
                
                //Get the items parent
                auto parent = temp.getParentDirectory();
                
                //get the items parents filename
                String parentName = parent.getFileName();
                
                //get the Current ValueTrees parent's name
                String vtParentName = currentParent->getProperty("name").toString();
                
                //If the parents dont lign up, its because the outer for loop has gone back up the directory tree
                while (parentName != currentParent->getProperty("name").toString())
                {
                    //DBG("ParentDIR: " + parentName + "  ParentVT:  " + vtParentName);
                    
                    //Get the currentParent's parent
                    auto p = currentParent->getParent();
                    
                    //if there is a parent, then we reset the currentParent to its own parent, else we set it to the root tree, as we are at the top of the higherarchy
                    if(p.isValid())
                        currentParent.reset(new ValueTree(p));
                    else
                        currentParent.reset(new ValueTree(vt));
                    
                    //DBG(p.getProperty("name").toString());
                }
                
                //As we know the parents lign up,
                //We can create a new tree with matching name and path
                auto newTree = createTree(nameString, dirString);
                
                //Then append that as a child to the correct ValueTree node
                currentParent->appendChild(newTree, nullptr);
                
                //Then set the currentParent to point to the tree we just made, as this is supposedly the "deepest" tree, and the loop above will make sure things are correct when this loops
                currentParent.reset(new ValueTree(newTree));
                
                //DBG(nameString + ":  " + dirString);
            }
            else
            {
                DBG("ERROR: The entry at some itration through the directory in createRootValueTree did not exist somehow...");
                //Return due to error
                return vt;
            }
            DBG("SUCCESS: Directory Was scanned successfully...");
        }
    }
    else
    {
        DBG("ERROR: A FILE IN THE DIRECTORY SUPPLIED TO createRootValueTree() WAS NOT REAL SOMEHOW...");
    }
    
    //Release the currentParent pointer
    currentParent.reset(currentParent.release());
    
    //Return the ValueTree which has been created
    return vt;
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
            auto theTree = ValueTree(*(eventItem.get()->getThisTree()));
            //DBG("The tree: "+ theTree.toXmlString());
            
            //Create a local ValueTree which will be populated by the searchTree function
            ValueTree theChild;
            
            //Search through the tree to find the correct child, which will be assigned to theChild variable
            searchTree(theTree, theChild, v);
            
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

void DirectoryDisplay::searchTree(ValueTree tree, ValueTree& resultTree, const var& childName)
{
    //Create an Identifier called name
    const Identifier id = "name";
    
    //Iterate over the children of the current tree
    for (const auto& child : tree)
    {
        //check if the child has the name property corresponding with the item we are looking for
        auto check = child.getChildWithProperty(id, childName);
        
        //If it is valid, that child was found
        if(check.isValid())
        {
            //So set the result to the child which has been found
            resultTree = check;
            //DBG("SUCCESS: Found a valid child for ya!");
        }
        else
        {
            //As the child was not located here, Recursively iterate deeper
            searchTree(child, resultTree, childName);
        }
    }
}
