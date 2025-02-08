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
    UserParams[delay] = 0.0f;
    UserParams[frecv] = 1.0f;
    UserParams[compRatio] = 1;
    UserParams[threshold] = 0;
    
    Fs = 44100;
    M_max = 10 * Fs;
    resetDelay();
    w[0][0] = 0;
    w[0][1] = 0;
    w[1][0] = 0;
    w[1][1] = 0;
    
    rmsAverage = 0.01;
    attackTime = 0.05;    // all in seconds
    releaseTime = 0.2;
    rmsLevel = 0.0f;
    smoothGain[0] = 1.0f;
    smoothGain[1] = 1.0f;

    alphaCalc(Fs);
  

    UIUpdateFlag = true; // se cere update de la interfața grafică
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
   
   
    float valNorm = UserParams[frecv];
    float ft = valNorm * (20000.f - 20.f) + 20.f;
    coeffCalc(ft, sampleRate);
    
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

void GainAudioProcessor::resetDelay()
{
    dl.clear();
    dl.resize(M_max, 0.0f);
    index_IN = 0;
    index_OUT = 0;
    M = UserParams[delay] * Fs;
}

float GainAudioProcessor::filterSampleFTJ(float in, int ch)
{
    float aux = in - a[1] * w[ch][0] - a[2] * w[ch][1];
    float out = b[0] * aux + b[1] * w[ch][0] + b[2] * w[ch][1];
    w[ch][1] = w[ch][0];
    w[ch][0] = aux;
    return out;
}

void GainAudioProcessor::coeffCalc(float ft, float fs)
{
    float K = tanf(juce::float_Pi * ft / fs);
    float Q = 1 / sqrtf(2);
    float num = K * K * Q + K + Q;

    b[0] = K * K * Q / num;
    b[1] = 2 * b[0];
    b[2] = b[0];

    a[0] = 1;
    a[1] = 2 * Q * (K * K - 1) / num;
    a[2] = (K * K * Q - K + Q) / num;
}

void GainAudioProcessor::alphaCalc(float fs)
{
    alphaAvg = exp(-1 / (fs * rmsAverage));
    alphaAt = exp(-1 / (fs * attackTime));
    alphaRt = exp(-1 / (fs * releaseTime));
}

float GainAudioProcessor::compression(float in, int channel)
{
    rmsLevel = alphaAvg * rmsLevel + (1 - alphaAvg) * (in * in);
    float rmsLevel_dB = 10 * log10(rmsLevel + 0.0001f);
    float gain_dB;

    if (rmsLevel_dB > UserParams[threshold])
        gain_dB = UserParams[compRatio] * (UserParams[threshold] - rmsLevel_dB);
    else
        gain_dB = 0;

    float targetGain = pow(10, gain_dB / 20);
    float c;
    if (targetGain < smoothGain[channel])
        c = alphaAt;
    else
        c = alphaRt;
  //  DBG("RMS Level: " << rmsLevel << " Smooth Gain: " << smoothGain << " Target Gain: " << targetGain);
    smoothGain[channel] = (1 - c) * smoothGain[channel] + c * targetGain;

    float out = smoothGain[channel] * in;

    return out;
}

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
        UIUpdateFlag = true;
    }
    else if(isDelay == 1){
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelDataX = buffer.getReadPointer(channel);

            auto* channelDataY = buffer.getWritePointer(channel);
            
            for (int j = 0; j < buffer.getNumSamples(); j++)
            {
                channelDataY[j] = filterSampleFTJ(channelDataY[j], channel);
                index_OUT = index_IN - M;
                index_OUT = (M_max + index_OUT - M) % M_max;

                channelDataY[j] = channelDataX[j] + UserParams[gain] * dl[index_OUT];
                channelDataY[j] = channelDataY[j] * UserParams[gain];
                dl[index_IN] = channelDataX[j];

                index_IN = (index_IN + 1) % M_max;
            }
        }
    }
    else {
        
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            for (int j = 0; j < buffer.getNumSamples(); j++)
            {
                channelData[j] = filterSampleFTJ(channelData[j], channel);

                channelData[j] = compression(channelData[j], channel);

                channelData[j] = channelData[j] * UserParams[gain];
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

    el = root.createNewChildElement("compRatio");
    el->addTextElement(String(UserParams[compRatio]));

    el = root.createNewChildElement("threshold");
    el->addTextElement(String(UserParams[threshold]));

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

            if (pChild->hasTagName("CompRatio"))
            {
                String text = pChild->getAllSubText();
                setParameter(compRatio, text.getFloatValue());
            }

            if (pChild->hasTagName("Threshold"))
            {
                String text = pChild->getAllSubText();
                setParameter(threshold, text.getFloatValue());
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
    
    case compRatio:
        return UserParams[compRatio];

    case threshold:
        return UserParams[threshold];

    default: return 0.0f; // index invalid
    }
}

void GainAudioProcessor::setParameter(int index, float newValue)
{
    float ft;
    switch (index)
    {
    case gain:
        UserParams[gain] = newValue;
        break;

    case delay:
        UserParams[delay] = newValue;
        resetDelay();
        break;

    case frecv:
        UserParams[frecv] = newValue;
        ft = newValue * (20000.f - 20.f) + 20.f;
        coeffCalc(ft, getSampleRate());
        break;
    
    case compRatio:
        UserParams[compRatio] = newValue;

        break;
    
    case threshold:
        UserParams[threshold] = newValue;

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

    case compRatio: return "CompRatio";
        break;

    case threshold: return "Threshold";
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
