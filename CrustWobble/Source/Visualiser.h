/*
  ==============================================================================

    Visualiser.h
    Created: 7 Jan 2023 10:01:37pm
    Author:  Alfie Ransome

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class AudioVisualiser   : public Component
{
public:

    AudioVisualiser();
    ~AudioVisualiser();
                                               
    void paint(Graphics& g) override;

    void resized() override;

    AudioThumbnail& getThumbnail();
                                                          
    void thumbnailChanged();
    void loadData(String filePath);

private:

    void paintIfNoFileLoaded(Graphics& g, const juce::Rectangle<int>& thumbnailBounds);

    void paintIfFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds);

    AudioFormatManager thumbnailFormatManager;
    AudioThumbnailCache thumbnailCache;
    AudioThumbnail thumbnail;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioVisualiser)

};
