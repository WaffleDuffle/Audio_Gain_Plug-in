/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
//==============================================================================
/**
*/
class GainAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    GainAudioProcessor();
    ~GainAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    int getNumParameters() override;
    float getParameter(int index) override;
    void setParameter(int index, float newValue) override;
    const String getParameterName(int index) override;
    const String getParameterText(int index) override;
    enum Parameters { gain = 0, delay = 1, frecv = 2, compRatio = 3, threshold = 4, totalNumParam };
    bool NeedsUIUpdate() { return UIUpdateFlag; };
    void RequestUIUpdate() { UIUpdateFlag = true; };
    void ClearUIUpdateFlag() { UIUpdateFlag = false; };
    bool isMuted;
    bool isDelay;
    
private:
    //==============================================================================
    // pentru filtru
    float a[3] = {}, b[3] = {};
    float w[2][2];

    //pentru delay
    int M_max;
    std::vector<float> dl;
    int index_IN;
    int index_OUT;
    int Fs;
    int M;

    // pentru compressor
    float rmsAverage;
    float attackTime;
    float releaseTime;
    float delayTime;
    int delaySamples;
    float rmsLevel;
    float smoothGain;
    int bufferIndex;
    std::vector<float> buffer;
    float alphaAvg;
    float alphaAt;
    float alphaRt;

    void GainAudioProcessor::resetDelay();
    float GainAudioProcessor::filterSampleFTJ(float in, int ch);
    void GainAudioProcessor::coeffCalc(float ft, float fs);
    void GainAudioProcessor::resetCompression();
    void GainAudioProcessor::alphaCalc(float fs);
    float UserParams[totalNumParam];
    bool UIUpdateFlag; //Valoare booleana pentru reimprospatarea interfetei grafice
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainAudioProcessor)
};
