#pragma once

#include <JuceHeader.h>
#include "View.h"
#include "Controller.h"
#include "Quake.h"
#include "Yggdrasil.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    Yggdrasil yggdrasil;
    MainView mainView;
    QuakeProcessor dirConvertorObject;
    Controller controllerObject;
    ValueTree igdrassil;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
