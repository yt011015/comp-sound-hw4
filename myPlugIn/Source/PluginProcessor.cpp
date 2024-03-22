/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyPlugInAudioProcessor::MyPlugInAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

MyPlugInAudioProcessor::~MyPlugInAudioProcessor()
{
}

//==============================================================================
const juce::String MyPlugInAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MyPlugInAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MyPlugInAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MyPlugInAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MyPlugInAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MyPlugInAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MyPlugInAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MyPlugInAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MyPlugInAudioProcessor::getProgramName (int index)
{
    return {};
}

void MyPlugInAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MyPlugInAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MyPlugInAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MyPlugInAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MyPlugInAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if (!pitchShiftIsToggled)
    {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer (channel);

            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                channelData[sample] *= gain;
            }
        }
    }
    
    else
    {
        juce::AudioBuffer<float> tempBuffer(totalNumInputChannels, buffer.getNumSamples());
            
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            auto* tempBufferData = tempBuffer.getWritePointer(channel);
            
            int writeIndex = 0;

            for (int i = 0; i < 2; i += 1)
            {
                for (int sample = 0; sample < buffer.getNumSamples(); sample += 2)
                {
                    float processedSample = channelData[sample] * gain;

                    if (writeIndex < buffer.getNumSamples() - 1)
                    {
                        tempBufferData[writeIndex++] = processedSample;
                    }
                }
            }
            std::copy(tempBufferData, tempBufferData + buffer.getNumSamples(), channelData);
        }
    }
    
}

void MyPlugInAudioProcessorEditor::sliderValueChanged (juce::Slider *slider)
{
    if (slider == &gainSlider)
    {
        audioProcessor.gain = gainSlider.getValue();
    }
}

void MyPlugInAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &pitchShiftSwitch)
    {
        audioProcessor.pitchShiftIsToggled = pitchShiftSwitch.getToggleState();
    }
}


//==============================================================================
bool MyPlugInAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MyPlugInAudioProcessor::createEditor()
{
    return new MyPlugInAudioProcessorEditor (*this);
}

//==============================================================================
void MyPlugInAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MyPlugInAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MyPlugInAudioProcessor();
}
