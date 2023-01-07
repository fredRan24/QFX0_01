/*
  ==============================================================================

    FileBar.cpp
    Created: 7 Jan 2023 10:03:56pm
    Author:  Alfie Ransome

  ==============================================================================
*/

#include "FileBar.h"

//////////////////////////////////////////////////
// ConvertNewEventView Component
//////////////////////////////////////////////////

ConvertNewEventView::ConvertNewEventView() : dirChooser("Directory Chooser",
                                                        File::getCurrentWorkingDirectory(),
                                                        false, true, false, "", "",
                                                        "Please select a directory!")
{
    addAndMakeVisible(dirChooser);
    addAndMakeVisible(convertButton);
}

ConvertNewEventView::~ConvertNewEventView() {}
FilenameComponent* ConvertNewEventView::getDirChooser(){    return dirChooserPtr;       }
TextButton* ConvertNewEventView::getConvertButton() {       return convertButtonPtr;    }

void ConvertNewEventView::resized()
{

    auto window = getLocalBounds();

    auto header = window.removeFromTop(30);
    header = header.reduced(5);
    auto dirChooserBounds = header.removeFromRight((header.getWidth()/5)*4);
    dirChooser.setBounds(dirChooserBounds);

    auto convertButtonBounds = header;
    convertButton.setBounds(convertButtonBounds);

}

void ConvertNewEventView::paint(juce::Graphics &g)
{
    //nothing here yet...
}






//////////////////////////////////////////////////
// FileMenuBar Component
//////////////////////////////////////////////////

FileMenuBar::FileMenuBar()
{
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

