/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <vector>
//==============================================================================
GainAudioProcessor::GainAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
    UserParams[gain] = 1.0f; // implicit castigul este 1
    UserParams[delay] = 1.0f;
    UserParams[frecv] = 20000.0f;
    UIUpdateFlag = true; // se cere update de la interfața grafică
    Fs = 44100;
    M_max = 10 * Fs;
    dl.resize(M_max, 0.0f);
    index_IN = 0;
    index_OUT = 0;
    M = UserParams[delay] * Fs;

}

GainAudioProcessor::~GainAudioProcessor()
{
    dl.clear();
}

//==============================================================================
const juce::String GainAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GainAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool GainAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool GainAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double GainAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GainAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int GainAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GainAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String GainAudioProcessor::getProgramName(int index)
{
    return {};
}

void GainAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void GainAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void GainAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GainAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
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

void GainAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());


    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.


    if (isMuted == 1) {
        buffer.clear();
    }
    else {
        
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelDataX = buffer.getReadPointer(channel);

            auto* channelDataY = buffer.getWritePointer(channel);
            
            for (int j = 0; j < buffer.getNumSamples(); j++)
            {
                
                index_OUT = index_IN - M;


                index_OUT = (M_max + index_OUT - M) % M_max;

                channelDataY[j] = channelDataX[j] + UserParams[gain] * dl[index_OUT];
             //   channelDataY[j] = channelDataY[j] * UserParams[gain] * 10;
                dl[index_IN] = channelDataX[j];

                index_IN = (index_IN + 1) % M_max;
            }
        }
    }

}

//==============================================================================
bool GainAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GainAudioProcessor::createEditor()
{
    return new GainAudioProcessorEditor(this);
}

//==============================================================================
void GainAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    XmlElement root("Root");
    XmlElement* el;
    el = root.createNewChildElement("Gain");
    el->addTextElement(String(UserParams[gain]));

    el = root.createNewChildElement("Delay");
    el->addTextElement(String(UserParams[delay]));

    el = root.createNewChildElement("Frecv");
    el->addTextElement(String(UserParams[frecv]));
    copyXmlToBinary(root, destData);
}

void GainAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    static std::unique_ptr< XmlElement > pRoot = getXmlFromBinary(data, sizeInBytes);
    if (pRoot != NULL)
    {
        forEachXmlChildElement((*pRoot), pChild)
        {
            if (pChild->hasTagName("Gain"))
            {
                String text = pChild->getAllSubText();
                setParameter(gain, text.getFloatValue());
            }

            if (pChild->hasTagName("Delay"))
            {
                String text = pChild->getAllSubText();
                setParameter(delay, text.getFloatValue());
            }

            if (pChild->hasTagName("Frecv"))
            {
                String text = pChild->getAllSubText();
                setParameter(frecv, text.getFloatValue());
            }
        }
        UIUpdateFlag = true;
    }
}

int GainAudioProcessor::getNumParameters()
{
    return totalNumParam;
}

float GainAudioProcessor::getParameter(int index)
{
    switch (index)
    {
    case gain: // exemplu
        return UserParams[gain];

    case delay:
        return UserParams[delay];

    case frecv:
        return UserParams[frecv];

    default: return 0.0f; // index invalid
    }
}

void GainAudioProcessor::setParameter(int index, float newValue)
{
    switch (index)
    {
    case gain:
        UserParams[gain] = newValue;
        break;

    case delay:
        UserParams[delay] = newValue;
        dl.clear();
        dl.resize(M_max, 0.0f);
        index_IN = 0;
        index_OUT = 0;
        M = UserParams[delay] * Fs;

        break;

    case frecv:
        UserParams[frecv] = newValue;
        break;

    default: return;
    }
    UIUpdateFlag = true;//Request UI update -- Some OSX hosts use alternate editors, this updates ours
}

const String GainAudioProcessor::getParameterName(int index)
{
    switch (index)
    {
    case gain: return "Gain";
        break;

    case delay: return "Delay";
        break;

    case frecv: return "Frecv";
        break;

    default:return String();
    }
}

const String GainAudioProcessor::getParameterText(int index)
{
    if (index >= 0 && index < totalNumParam)
        return String(UserParams[index]);
    else return String();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GainAudioProcessor();
}
