/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TheOchoAudioProcessor::TheOchoAudioProcessor()
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

TheOchoAudioProcessor::~TheOchoAudioProcessor()
{
}

//==============================================================================
const juce::String TheOchoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TheOchoAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TheOchoAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TheOchoAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TheOchoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TheOchoAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TheOchoAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TheOchoAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TheOchoAudioProcessor::getProgramName (int index)
{
    return {};
}

void TheOchoAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TheOchoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void TheOchoAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TheOchoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void TheOchoAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear blank channels for safety
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Only take input from Left channel for now. Can update this later to select an input like Guitar Rig, or take a stereo input. Idk.
    auto* samples = buffer.getWritePointer(0);
    int numSamples = buffer.getNumSamples();
    
    for (int i = 0; i < numSamples; i++)
    {
        float currentSample = samples[i];
        if (lastInputSample < 0.0f && currentSample >= 0.0f)
            flipMult *= -1.0f;
            
        samples[i] = flipMult * 0.3f;
        lastInputSample = currentSample;
        
        for (int ch = 1; ch < totalNumOutputChannels; ++ch)
            buffer.setSample(ch, i, samples[i]);
    }
    
    
    
}

//==============================================================================
bool TheOchoAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TheOchoAudioProcessor::createEditor()
{
    return new TheOchoAudioProcessorEditor (*this);
}

//==============================================================================
void TheOchoAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TheOchoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TheOchoAudioProcessor();
}
