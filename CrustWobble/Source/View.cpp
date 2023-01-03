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

//////////////////////////////////////////////////
// DirectoryDisplay Component
//////////////////////////////////////////////////

DirectoryDisplay::DirectoryDisplay()
{
    addAndMakeVisible(eventTree);
    
    eventTree.setTitle("EventTree");
    eventTree.setDefaultOpenness (true);
    eventTree.setMultiSelectEnabled (true);
    eventItem.reset (new EventTreeItem (createRootValueTree(), undoManager));
    eventTree.setRootItem(eventItem.get());
    
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

ValueTree DirectoryDisplay::createTree (const String& desc)
{
    ValueTree t ("Item");
    t.setProperty ("name", desc, nullptr);
    return t;
}

ValueTree DirectoryDisplay::createRootValueTree()
{
    auto vt = createTree ("This demo displays a ValueTree as a treeview.");
    vt.appendChild (createTree ("You can drag around the nodes to rearrange them"),               nullptr);
    vt.appendChild (createTree ("..and press 'delete' or 'backspace' to delete them"),            nullptr);
    vt.appendChild (createTree ("Then, you can use the undo/redo buttons to undo these changes"), nullptr);

    int n = 1;
    vt.appendChild (createRandomTree (n, 0), nullptr);

    return vt;
}

ValueTree DirectoryDisplay::createRandomTree (int& counter, int depth)
{
    auto t = createTree ("Item " + String (counter++));

    if (depth < 3)
        for (int i = 1 + Random::getSystemRandom().nextInt (7); --i >= 0;)
            t.appendChild (createRandomTree (counter, depth + 1), nullptr);

    return t;
}

void DirectoryDisplay::deleteSelectedItems()
{
    OwnedArray<ValueTree> selectedItems;
    EventTreeItem::getSelectedTreeViewItems (eventTree, selectedItems);

    for (auto* v : selectedItems)
    {
        if (v->getParent().isValid())
            v->getParent().removeChild (*v, &undoManager);
    }
}

void DirectoryDisplay::loadDirectoryIntoFileTreeComponent(File& directory)
{

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
