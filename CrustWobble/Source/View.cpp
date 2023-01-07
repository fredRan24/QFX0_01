/*
  ==============================================================================

    View.cpp
    Created: 28 Oct 2021 6:43:51pm
    Author:  Alfie

  ==============================================================================
*/

#pragma once
#include "View.h"
using namespace std;


//////////////////////////////////////////////////
//       MainView
//////////////////////////////////////////////////

MainView::MainView()
{
    addAndMakeVisible(fileMenuBar);
    addAndMakeVisible(topWindow);
    addAndMakeVisible(dirDisplay);
    dirDisplay.setVisualiser(topWindow.getVisualiser());
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
// ConvertNewEventView Component
//////////////////////////////////////////////////

ConvertNewEventView::ConvertNewEventView() : dirChooser("Directory Chooser", File::getCurrentWorkingDirectory(),
    false, true, false, "", "",
    "Please select a directory!")
{
    addAndMakeVisible(dirChooser);
    addAndMakeVisible(convertButton);
}

ConvertNewEventView::~ConvertNewEventView() {
    
}

FilenameComponent* ConvertNewEventView::getDirChooser()
{
    
    return dirChooserPtr;
}

TextButton* ConvertNewEventView::getConvertButton()
{
    return convertButtonPtr;
}

void ConvertNewEventView::resized() {

    auto window = getLocalBounds();

    auto header = window.removeFromTop(30);
    header = header.reduced(5);
    auto dirChooserBounds = header.removeFromRight((header.getWidth()/5)*4);
    dirChooser.setBounds(dirChooserBounds);

    auto convertButtonBounds = header;
    convertButton.setBounds(convertButtonBounds);

}

void ConvertNewEventView::paint(juce::Graphics &g) {
    
}

//////////////////////////////////////////////////
// FileMenuBar Component
//////////////////////////////////////////////////

FileMenuBar::FileMenuBar() {
    //menuBar.setModel(this);
    addAndMakeVisible(menuBar);
    addAndMakeVisible(converter);
}

FileMenuBar::~FileMenuBar() {}


//Overrides
void FileMenuBar::paint(juce::Graphics& g)
{
    g.fillAll(Colours::pink);
}

void FileMenuBar::resized()
{
    menuBar.setBounds(getLocalBounds());
}

StringArray FileMenuBar::getMenuBarNames()
{
    auto names = { "File" };
    return StringArray(names);
}

PopupMenu FileMenuBar::getMenuForIndex(int topLevelMenuIndex, const String& menuName)
{
    PopupMenu menu;
    //if the menu is empty, add AudioPrefs to the menu
    if (topLevelMenuIndex == 0)
        menu.addItem(FileMenuItems::AudioPrefs, "Audio Prefrences", true, false);
    return menu;
}

void FileMenuBar::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
    /*
    //if the File menu was selected
    if (topLevelMenuIndex == Menus::FileMenu)
    {
        //if the AudioPreferences menu was selected
        if (menuItemID == FileMenuItems::AudioPrefs)
        {
            //local LaunchOptions obj
            DialogWindow::LaunchOptions la;
            la.dialogTitle = "Audio Settings";

            //Configure temp AudioDeviceSelectorComponent
            OptionalScopedPointer<Component> osp(make_unique<AudioDeviceSelectorComponent>
                (core.getAudioAudioDeviceManager(),
                    1, 2, 2, 2,
                    true, true, true, false));
            osp->setSize(450, 350);
            la.content = std::move(osp);
            la.componentToCentreAround = this;

            //Launch the popup
            la.launchAsync();
        }
    }
    */
}

//////////////////////////////////////////////////
// EventTreeItem Class
//////////////////////////////////////////////////

EventTreeItem::EventTreeItem(const ValueTree& v, UndoManager& um) : tree (v), undoManager(um)
{
    tree.addListener(this);
}

EventTreeItem::~EventTreeItem()
{
    
}

String EventTreeItem::getUniqueName() const
{
    return tree["name"].toString();
}

bool EventTreeItem::mightContainSubItems()
{
    return tree.getNumChildren() > 0;
}

void EventTreeItem::paintItem (Graphics& g, int width, int height)
{
    if (isSelected())
        g.fillAll (Colours::teal);

    g.setColour (Colours::black);
    g.setFont (15.0f);

    g.drawText (tree["name"].toString(),
                4, 0, width - 4, height,
                Justification::centredLeft, true);
}

void EventTreeItem::itemOpennessChanged (bool isNowOpen)
{
    if (isNowOpen && getNumSubItems() == 0)
        refreshSubItems();
    else
        clearSubItems();
}


void EventTreeItem::treeChildrenChanged (const ValueTree& parentTree)
{
    if (parentTree == tree)
    {
        refreshSubItems();
        treeHasChanged();
        setOpen (true);
    }
}

void EventTreeItem::refreshSubItems()
{
    clearSubItems();

    for (int i = 0; i < tree.getNumChildren(); ++i) {
        addSubItem (new EventTreeItem(tree.getChild (i), undoManager));
    }
}

void EventTreeItem::valueTreePropertyChanged (ValueTree&, const Identifier&)
{
    repaintItem();
}

void EventTreeItem::getSelectedTreeViewItems (TreeView& treeView, OwnedArray<ValueTree>& items)
{
    auto numSelected = treeView.getNumSelectedItems();

    for (int i = 0; i < numSelected; ++i)
        if (auto* vti = dynamic_cast<EventTreeItem*> (treeView.getSelectedItem (i)))
            items.add (new ValueTree (vti->tree));
}

String EventTreeItem::getAudioDataAsString()
{
    return tree.getProperty("audio");
}


ValueTree* EventTreeItem::getThisTree()
{
    return &tree;
}

String EventTreeItem::getThisItemsAudio()
{
    return tree.getProperty("audio");
}

//////////////////////////////////////////////////
// DirectoryDisplay Component
//////////////////////////////////////////////////

DirectoryDisplay::DirectoryDisplay()
{
    addAndMakeVisible(eventTree);
    
    eventTree.setTitle("EventTree");
    eventTree.setDefaultOpenness (true);
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
    g.fillAll(Colours::orange);

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

//////////////////////////////////////////////////
// ControlsView Component
//////////////////////////////////////////////////

ControlsView::ControlsView()
{

}

ControlsView::~ControlsView()
{

}

void ControlsView::paint(juce::Graphics& g)
{
    g.fillAll(Colours::blue);
}

void ControlsView::resized()
{

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
//////////////////////////////////////////////////
// AudioVisualiser Component
//////////////////////////////////////////////////

AudioVisualiser::AudioVisualiser() : thumbnailCache(5), thumbnail(512,thumbnailFormatManager,thumbnailCache)
{
    //register the basic audio file formats with the thumbnailFormatManager
    thumbnailFormatManager.registerBasicFormats();
}

AudioVisualiser::~AudioVisualiser() {
    
}

void AudioVisualiser::paintIfFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds)
{
    //set the brush to background colour
    g.setColour(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    //Fill background
    g.fillRect(thumbnailBounds);
    //set the Waveform colour
    g.setColour(Colours::goldenrod);
    //draw the waveform
    thumbnail.drawChannels(g,
        thumbnailBounds,
        0.0,                                    // start time
        thumbnail.getTotalLength(),             // end time
        0.95f);                                 // vertical zoom
}

void AudioVisualiser::paintIfNoFileLoaded(Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
    //set the brush to background colour
    g.setColour(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    //Fill background
    g.fillRect(thumbnailBounds);
    //set text colour
    g.setColour(Colours::grey);
    //draw text
    g.drawFittedText("Please Load an Event and File", thumbnailBounds, Justification::centred, 1);
}

void AudioVisualiser::thumbnailChanged()
{
    repaint();
}

AudioThumbnail& AudioVisualiser::getThumbnail()
{
    return thumbnail;
}

void AudioVisualiser::resized() {
    
}

void AudioVisualiser::paint(Graphics& g)
{
    //Rectangle of component bounds
    auto r = getLocalBounds();

    //If a thumbnail is loaded, paint x, if not, y
    if (thumbnail.getNumChannels() == 0)
        paintIfNoFileLoaded(g, r);
    else
        paintIfFileLoaded(g, r);
}

void AudioVisualiser::loadData(String filePath)
{
    File temp = File(filePath);
    if(temp.existsAsFile())
    {
        thumbnail.setSource(new FileInputSource(temp));
    }
    repaint();
}
