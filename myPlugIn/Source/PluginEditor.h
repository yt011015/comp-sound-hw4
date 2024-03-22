/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MyPlugInAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener, private juce::Button::Listener
{
public:
    MyPlugInAudioProcessorEditor (MyPlugInAudioProcessor&);
    ~MyPlugInAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged (juce::Slider *slider) override;
    void buttonClicked (juce::Button* button) override;
    

private:
    juce::Slider gainSlider;
    juce::ToggleButton pitchShiftSwitch;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MyPlugInAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyPlugInAudioProcessorEditor)
};
