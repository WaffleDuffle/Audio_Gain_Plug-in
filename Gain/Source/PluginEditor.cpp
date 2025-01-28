/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.12

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "PluginEditor.h"
#include <vector>

//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
GainAudioProcessorEditor::GainAudioProcessorEditor (GainAudioProcessor* ownerFilter)
    : AudioProcessorEditor(ownerFilter)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    gainSlider.reset (new juce::Slider ("Gain Slider"));
    addAndMakeVisible (gainSlider.get());
    gainSlider->setRange (-70, 0, 0.1);
    gainSlider->setSliderStyle (juce::Slider::LinearHorizontal);
    gainSlider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 40, 20);
    gainSlider->setColour (juce::Slider::thumbColourId, juce::Colours::white);
    gainSlider->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0xff282828));
    gainSlider->addListener (this);

    gainSlider->setBounds (104, 120, 271, 32);

    gainLabel.reset (new juce::Label ("Gain Label",
                                      TRANS ("Gain [dB]")));
    addAndMakeVisible (gainLabel.get());
    gainLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    gainLabel->setJustificationType (juce::Justification::centredLeft);
    gainLabel->setEditable (false, false, false);
    gainLabel->setColour (juce::Label::textColourId, juce::Colours::black);
    gainLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    gainLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    gainLabel->setBounds (32, 120, 86, 32);

    delaySlider.reset (new juce::Slider ("Delay Slider"));
    addAndMakeVisible (delaySlider.get());
    delaySlider->setRange (0, 10, 0.2);
    delaySlider->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    delaySlider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 40, 20);
    delaySlider->setColour (juce::Slider::thumbColourId, juce::Colours::white);
    delaySlider->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0xff282828));
    delaySlider->addListener (this);

    delaySlider->setBounds (160, 8, 70, 80);

    delayLabel.reset (new juce::Label ("Delay Label",
                                       TRANS ("Delay [s]\n")));
    addAndMakeVisible (delayLabel.get());
    delayLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    delayLabel->setJustificationType (juce::Justification::centredLeft);
    delayLabel->setEditable (false, false, false);
    delayLabel->setColour (juce::Label::textColourId, juce::Colours::black);
    delayLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    delayLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    delayLabel->setBounds (160, 96, 78, 24);

    muteButton.reset (new juce::ToggleButton ("Mute Button"));
    addAndMakeVisible (muteButton.get());
    muteButton->setButtonText (TRANS ("Mute"));
    muteButton->addListener (this);
    muteButton->setColour (juce::ToggleButton::textColourId, juce::Colours::black);

    muteButton->setBounds (8, 64, 54, 24);

    frecvSlider.reset (new juce::Slider ("Frecv Slider"));
    addAndMakeVisible (frecvSlider.get());
    frecvSlider->setRange (20, 20000, 1);
    frecvSlider->setSliderStyle (juce::Slider::LinearHorizontal);
    frecvSlider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    frecvSlider->setColour (juce::Slider::thumbColourId, juce::Colours::white);
    frecvSlider->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0xff282828));
    frecvSlider->addListener (this);
    frecvSlider->setSkewFactor (0.5);

    frecvSlider->setBounds (104, 152, 272, 32);

    frecvLabel.reset (new juce::Label ("Frecv Label",
                                       TRANS ("Frecv [Hz]")));
    addAndMakeVisible (frecvLabel.get());
    frecvLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    frecvLabel->setJustificationType (juce::Justification::centredLeft);
    frecvLabel->setEditable (false, false, false);
    frecvLabel->setColour (juce::Label::textColourId, juce::Colours::black);
    frecvLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    frecvLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    frecvLabel->setBounds (32, 152, 70, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (400, 200);


    //[Constructor] You can add your own custom stuff here..
    getProcessor()->RequestUIUpdate();
    startTimer(200); //porneste temporizatorul cu un interval de 200 ms
    //[/Constructor]
}

GainAudioProcessorEditor::~GainAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    gainSlider = nullptr;
    gainLabel = nullptr;
    delaySlider = nullptr;
    delayLabel = nullptr;
    muteButton = nullptr;
    frecvSlider = nullptr;
    frecvLabel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GainAudioProcessorEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff89ff96));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void GainAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void GainAudioProcessorEditor::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    GainAudioProcessor* ourProcessor = getProcessor();
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == gainSlider.get())
    {
        //[UserSliderCode_gainSlider] -- add your slider handling code here..
        float normValue = powf(10.f, (float)gainSlider->getValue() / 20.f);
        ourProcessor->setParameter(GainAudioProcessor::gain, normValue);
        //[/UserSliderCode_gainSlider]
    }
    else if (sliderThatWasMoved == delaySlider.get())
    {
        //[UserSliderCode_delaySlider] -- add your slider handling code here..
        float normValue2 = (float)delaySlider->getValue();
        ourProcessor->setParameter(GainAudioProcessor::delay, normValue2);
    //    ourProcessor->dl.resize(10 * 44100, 0.0f);
        //[/UserSliderCode_delaySlider]
    }
    else if (sliderThatWasMoved == frecvSlider.get())
    {
        //[UserSliderCode_frecvSlider] -- add your slider handling code here..
        float normValue3 = (float)(frecvSlider->getValue() - 20.0f) / (20000.0f - 20.0f);
        ourProcessor->setParameter(GainAudioProcessor::frecv, normValue3);
        //[/UserSliderCode_frecvSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void GainAudioProcessorEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    GainAudioProcessor* ourProcessor = getProcessor();
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == muteButton.get())
    {
        //[UserButtonCode_muteButton] -- add your button handler code here..
        ourProcessor->isMuted = muteButton->getToggleState();
        //[/UserButtonCode_muteButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void GainAudioProcessorEditor::timerCallback() {
    GainAudioProcessor* ourProcessor = getProcessor(); //se schimba datele dorite intre elementele interfetei grafice si modulul “ourProcessor”
    if (ourProcessor->NeedsUIUpdate())
    {
        float dbValue = 20.f * log10(ourProcessor->getParameter(GainAudioProcessor::gain));
        gainSlider->setValue(dbValue, juce::dontSendNotification);

        float valueDelay = ourProcessor->getParameter(GainAudioProcessor::delay);
        delaySlider->setValue(valueDelay, juce::dontSendNotification);

        float valueFrecv = ourProcessor->getParameter(GainAudioProcessor::frecv) * (20000.0f - 20.0f) + 20.f;
        frecvSlider->setValue(valueFrecv, juce::dontSendNotification);

        ourProcessor->ClearUIUpdateFlag();
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GainAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor, public Timer"
                 constructorParams="GainAudioProcessor* ownerFilter" variableInitialisers="AudioProcessorEditor(ownerFilter)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="400" initialHeight="200">
  <BACKGROUND backgroundColour="ff89ff96"/>
  <SLIDER name="Gain Slider" id="8f77fd08b320037f" memberName="gainSlider"
          virtualName="" explicitFocusOrder="0" pos="104 120 271 32" thumbcol="ffffffff"
          textboxbkgd="ff282828" min="-70.0" max="0.0" int="0.1" style="LinearHorizontal"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="40"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="Gain Label" id="4892b907a93fecb5" memberName="gainLabel"
         virtualName="" explicitFocusOrder="0" pos="32 120 86 32" textCol="ff000000"
         edTextCol="ff000000" edBkgCol="0" labelText="Gain [dB]" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="Delay Slider" id="c33a0559ab098d2" memberName="delaySlider"
          virtualName="" explicitFocusOrder="0" pos="160 8 70 80" thumbcol="ffffffff"
          textboxbkgd="ff282828" min="0.0" max="10.0" int="0.2" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="40"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="Delay Label" id="eb532a9542dbe041" memberName="delayLabel"
         virtualName="" explicitFocusOrder="0" pos="160 96 78 24" textCol="ff000000"
         edTextCol="ff000000" edBkgCol="0" labelText="Delay [s]&#10;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <TOGGLEBUTTON name="Mute Button" id="31047c9dbeb3271f" memberName="muteButton"
                virtualName="" explicitFocusOrder="0" pos="8 64 54 24" txtcol="ff000000"
                buttonText="Mute" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <SLIDER name="Frecv Slider" id="d24cb00116dd693d" memberName="frecvSlider"
          virtualName="" explicitFocusOrder="0" pos="104 152 272 32" thumbcol="ffffffff"
          textboxbkgd="ff282828" min="20.0" max="20000.0" int="1.0" style="LinearHorizontal"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="0.5" needsCallback="1"/>
  <LABEL name="Frecv Label" id="305cb9c0e9b31b10" memberName="frecvLabel"
         virtualName="" explicitFocusOrder="0" pos="32 152 70 24" textCol="ff000000"
         edTextCol="ff000000" edBkgCol="0" labelText="Frecv [Hz]" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

