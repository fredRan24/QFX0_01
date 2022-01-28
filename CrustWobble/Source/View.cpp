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

/*
==============================================================================
                                MainView
==============================================================================
*/

MainView::MainView() : dirChooser("Directory Chooser",
                                  File::getCurrentWorkingDirectory(),
                                  false, true, false, "", "",
                                  "Please select a directory!")
{
    addAndMakeVisible(fileMenuBar);
    addAndMakeVisible(dirChooser);
    addAndMakeVisible(convertButton);
    
    convertButton.setButtonText("Convert");
}


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
    
    auto header = window.removeFromTop(30);
    header = header.reduced(5);
    auto dirChooserBounds = header.removeFromRight((header.getWidth()/5)*4);
    dirChooser.setBounds(dirChooserBounds);
    
    auto convertButtonBounds = header;
    convertButton.setBounds(convertButtonBounds);
    
}

FilenameComponent* MainView::getDirChooser()
{
    
    return dirChooserPtr;
}

TextButton* MainView::getConvertButton()
{
    return convertButtonPtr;
}

/*
==============================================================================
                                FileMenuBar
==============================================================================
*/

FileMenuBar::FileMenuBar() {
    //menuBar.setModel(this);
    addAndMakeVisible(menuBar);
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


WebBrowser::WebBrowser()
{
    
}

WebBrowser::~WebBrowser()
{
    
}
