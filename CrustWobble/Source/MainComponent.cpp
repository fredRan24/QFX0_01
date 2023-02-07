#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : controllerObject(mainView, dirConvertorObject), mainView(yggdrasil)
{
    addAndMakeVisible(mainView);

    setSize (1024, 720);
}

MainComponent::~MainComponent()
{

}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
 
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    mainView.setBounds(getLocalBounds());
}
