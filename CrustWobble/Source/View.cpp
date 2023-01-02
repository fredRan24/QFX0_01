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
    //g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}
                                                   
void MainView::resized()
{
    auto window = getLocalBounds();

    auto menuBarBounds = window.removeFromTop(25);
    fileMenuBar.setBounds(menuBarBounds);
    
}

//////////////////////////////////////////////////
// FileMenuBar Component
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
    //g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
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
// DirectoryDisplay Component
//////////////////////////////////////////////////

DirectoryDisplay::DirectoryDisplay()
{
    
}

DirectoryDisplay::~DirectoryDisplay()
{

}

void DirectoryDisplay::paint(juce::Graphics& g)
{

}

void DirectoryDisplay::resized()
{

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

}

void TopWindowView::resized()
{

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
