#pragma once

#include "AirwindowsUI.h"
#include "PluginProcessor.h"
#include "BinaryData.h"



//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor, juce::Slider::Listener
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider *) override;
    void sliderDragStarted(juce::Slider *) override;
    void sliderDragEnded(juce::Slider *) override;
    void sliderDragInternal(juce::Slider *, bool started);
    void updateTrackProperties();
    void updatePluginSize();
    void idle();
 
    struct IdleTimer : juce::Timer
        {
            IdleTimer(PluginEditor *ed) : ed(ed) {}
            ~IdleTimer() = default;
            void timerCallback() override { ed->idle(); }
            PluginEditor *ed;
        };


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processorRef; //quick ref
    
    std::unique_ptr<IdleTimer> idleTimer;
    AirwindowsLookAndFeel airwindowsLookAndFeel;
    AirwindowsMeter meter;
    
    juce::Slider HIPKnob { "Highpass" };
    juce::Slider LOPKnob { "Lowpass" };
    juce::Slider AIRKnob { "Air" };
    juce::Slider FIRKnob { "Fire" };
    juce::Slider STOKnob { "Stone" };
    juce::Slider RNGKnob { "Range" };
    juce::Slider FCTKnob { "FC Threshold" };
    juce::Slider SCTKnob { "SC Threshold" };
    juce::Slider FCRKnob { "FC Ratio" };
    juce::Slider SCRKnob { "SC Ratio" };
    juce::Slider FCAKnob { "FC Attack" };
    juce::Slider SCAKnob { "SC Attack" };
    juce::Slider FCLKnob { "FC Release" };
    juce::Slider SCLKnob { "SC Release" };
    juce::Slider FGTKnob { "FG Threshold" };
    juce::Slider SGTKnob { "SG Threshold" };
    juce::Slider FGRKnob { "FG Ratio" };
    juce::Slider SGRKnob { "SG Ratio" };
    juce::Slider FGSKnob { "FG Sustain" };
    juce::Slider SGSKnob { "SG Sustain" };
    juce::Slider FGLKnob { "FG Release" };
    juce::Slider SGLKnob { "SG Release" };
    juce::Slider TRFKnob { "Treble Freq" };
    juce::Slider TRGKnob { "Treble" };
    juce::Slider TRRKnob { "Treble Reso" };
    juce::Slider HMFKnob { "HighMid Freq" };
    juce::Slider HMGKnob { "HighMid" };
    juce::Slider HMRKnob { "HighMid Reso" };
    juce::Slider LMFKnob { "LowMid Freq" };
    juce::Slider LMGKnob { "LowMid" };
    juce::Slider LMRKnob { "LowMid Reso" };
    juce::Slider BSFKnob { "Bass Freq" };
    juce::Slider BSGKnob { "Bass" };
    juce::Slider BSRKnob { "Bass Reso" };
    juce::Slider DSCKnob { "Top dB" };
    juce::Slider PANKnob { "Pan" };
    juce::Slider FADKnob { "Fader" };

    juce::Colour hostTrackColour = juce::Colours::lightgrey;
    juce::String hostTrackName = juce::String();
    juce::TextButton inspectButton { "Inspect the UI" };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
