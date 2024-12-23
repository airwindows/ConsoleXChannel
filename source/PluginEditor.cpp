#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    setResizable(true, true);
    setLookAndFeel(&airwindowsLookAndFeel);
    if (hostTrackColour != juce::Colour()) {
        airwindowsLookAndFeel.setColour(juce::ResizableWindow::backgroundColourId, hostTrackColour);
        airwindowsLookAndFeel.setColour(juce::Slider::thumbColourId, hostTrackColour);
    } else {
        airwindowsLookAndFeel.setColour(juce::ResizableWindow::backgroundColourId, airwindowsLookAndFeel.defaultColour);
        airwindowsLookAndFeel.setColour(juce::Slider::thumbColourId, airwindowsLookAndFeel.defaultColour);
    }
    updateTrackProperties();

    idleTimer = std::make_unique<IdleTimer>(this);
    idleTimer->startTimer(1000/30); //space between UI screen updates. Larger is slower updates to screen

    meter.setOpaque(true);
    meter.resetArrays();
    meter.displayTrackName = hostTrackName;
    meter.meterInterpolation = airwindowsLookAndFeel.alfInterpolation;
    addAndMakeVisible(meter);

    if (airwindowsLookAndFeel.knobMode == 0) {
        HIPKnob.setSliderStyle(juce::Slider::Rotary);
        LOPKnob.setSliderStyle(juce::Slider::Rotary);
        AIRKnob.setSliderStyle(juce::Slider::Rotary);
        FIRKnob.setSliderStyle(juce::Slider::Rotary);
        STOKnob.setSliderStyle(juce::Slider::Rotary);
        RNGKnob.setSliderStyle(juce::Slider::Rotary);
        FCRKnob.setSliderStyle(juce::Slider::Rotary);
        SCRKnob.setSliderStyle(juce::Slider::Rotary);
        FCAKnob.setSliderStyle(juce::Slider::Rotary);
        SCAKnob.setSliderStyle(juce::Slider::Rotary);
        FCLKnob.setSliderStyle(juce::Slider::Rotary);
        SCLKnob.setSliderStyle(juce::Slider::Rotary);
        FGRKnob.setSliderStyle(juce::Slider::Rotary);
        SGRKnob.setSliderStyle(juce::Slider::Rotary);
        FGSKnob.setSliderStyle(juce::Slider::Rotary);
        SGSKnob.setSliderStyle(juce::Slider::Rotary);
        FGLKnob.setSliderStyle(juce::Slider::Rotary);
        SGLKnob.setSliderStyle(juce::Slider::Rotary);
        TRFKnob.setSliderStyle(juce::Slider::Rotary);
        TRRKnob.setSliderStyle(juce::Slider::Rotary);
        HMFKnob.setSliderStyle(juce::Slider::Rotary);
        HMGKnob.setSliderStyle(juce::Slider::Rotary);
        HMRKnob.setSliderStyle(juce::Slider::Rotary);
        LMFKnob.setSliderStyle(juce::Slider::Rotary);
        LMGKnob.setSliderStyle(juce::Slider::Rotary);
        LMRKnob.setSliderStyle(juce::Slider::Rotary);
        BSFKnob.setSliderStyle(juce::Slider::Rotary);
        BSGKnob.setSliderStyle(juce::Slider::Rotary);
        BSRKnob.setSliderStyle(juce::Slider::Rotary);
        DSCKnob.setSliderStyle(juce::Slider::Rotary);
        PANKnob.setSliderStyle(juce::Slider::Rotary);
        FADKnob.setSliderStyle(juce::Slider::Rotary);
    }

    if (airwindowsLookAndFeel.knobMode == 1) {
        HIPKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        LOPKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        AIRKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        FIRKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        STOKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        RNGKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        FCRKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        SCRKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        FCAKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        SCAKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        FCLKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        SCLKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        FGRKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        SGRKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        FGSKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        SGSKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        FGLKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        SGLKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        TRFKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        TRRKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        HMFKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        HMGKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        HMRKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        LMFKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        LMGKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        LMRKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        BSFKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        BSGKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        BSRKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        DSCKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        PANKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        FADKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    }

    if (airwindowsLookAndFeel.knobMode == 2) {
        HIPKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        LOPKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        AIRKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        FIRKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        STOKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        RNGKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        FCRKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        SCRKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        FCAKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        SCAKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        FCLKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        SCLKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        FGRKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        SGRKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        FGSKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        SGSKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        FGLKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        SGLKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        TRFKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        TRRKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        HMFKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        HMGKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        HMRKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        LMFKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        LMGKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        LMRKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        BSFKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        BSGKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        BSRKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        DSCKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        PANKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
        FADKnob.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
    }

    HIPKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    HIPKnob.setRange(0.0f, 1.0f);
    HIPKnob.setValue(processorRef.params[PluginProcessor::KNOBHIP]->get(), juce::NotificationType::dontSendNotification);
    HIPKnob.addListener(this);
    addAndMakeVisible(HIPKnob);
    
    LOPKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    LOPKnob.setRange(0.0f, 1.0f);
    LOPKnob.setValue(processorRef.params[PluginProcessor::KNOBLOP]->get(), juce::NotificationType::dontSendNotification);
    LOPKnob.addListener(this);
    addAndMakeVisible(LOPKnob);
    
    AIRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    AIRKnob.setRange(0.0f, 1.0f);
    AIRKnob.setValue(processorRef.params[PluginProcessor::KNOBAIR]->get(), juce::NotificationType::dontSendNotification);
    AIRKnob.addListener(this);
    addAndMakeVisible(AIRKnob);
    AIRKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.949f, 0.913f, 0.117f, 1.0f)); //fixed thumb color yellow

    FIRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    FIRKnob.setRange(0.0f, 1.0f);
    FIRKnob.setValue(processorRef.params[PluginProcessor::KNOBFIR]->get(), juce::NotificationType::dontSendNotification);
    FIRKnob.addListener(this);
    addAndMakeVisible(FIRKnob);
    FIRKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(1.0f, 0.486f, 0.129f, 1.0f)); //fixed thumb color orange

    STOKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    STOKnob.setRange(0.0f, 1.0f);
    STOKnob.setValue(processorRef.params[PluginProcessor::KNOBSTO]->get(), juce::NotificationType::dontSendNotification);
    STOKnob.addListener(this);
    addAndMakeVisible(STOKnob);
    STOKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.466f, 0.313f, 0.505f, 1.0f)); //fixed thumb color purple

    RNGKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    RNGKnob.setRange(0.0f, 1.0f);
    RNGKnob.setValue(processorRef.params[PluginProcessor::KNOBRNG]->get(), juce::NotificationType::dontSendNotification);
    RNGKnob.addListener(this);
    addAndMakeVisible(RNGKnob);
    
    FCTKnob.setSliderStyle(juce::Slider::LinearHorizontal);
    FCTKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    FCTKnob.setRange(0.0f, 1.0f);
    FCTKnob.setValue(processorRef.params[PluginProcessor::KNOBFCT]->get(), juce::NotificationType::dontSendNotification);
    FCTKnob.addListener(this);
    addAndMakeVisible(FCTKnob);
    
    SCTKnob.setSliderStyle(juce::Slider::LinearHorizontal);
    SCTKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    SCTKnob.setRange(0.0f, 1.0f);
    SCTKnob.setValue(processorRef.params[PluginProcessor::KNOBSCT]->get(), juce::NotificationType::dontSendNotification);
    SCTKnob.addListener(this);
    addAndMakeVisible(SCTKnob);
    
    FCRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    FCRKnob.setRange(0.0f, 1.0f);
    FCRKnob.setValue(processorRef.params[PluginProcessor::KNOBFCR]->get(), juce::NotificationType::dontSendNotification);
    FCRKnob.addListener(this);
    addAndMakeVisible(FCRKnob);
    
    SCRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    SCRKnob.setRange(0.0f, 1.0f);
    SCRKnob.setValue(processorRef.params[PluginProcessor::KNOBSCR]->get(), juce::NotificationType::dontSendNotification);
    SCRKnob.addListener(this);
    addAndMakeVisible(SCRKnob);
    
    FCAKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    FCAKnob.setRange(0.0f, 1.0f);
    FCAKnob.setValue(processorRef.params[PluginProcessor::KNOBFCA]->get(), juce::NotificationType::dontSendNotification);
    FCAKnob.addListener(this);
    addAndMakeVisible(FCAKnob);
    
    SCAKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    SCAKnob.setRange(0.0f, 1.0f);
    SCAKnob.setValue(processorRef.params[PluginProcessor::KNOBSCA]->get(), juce::NotificationType::dontSendNotification);
    SCAKnob.addListener(this);
    addAndMakeVisible(SCAKnob);
    
    FCLKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    FCLKnob.setRange(0.0f, 1.0f);
    FCLKnob.setValue(processorRef.params[PluginProcessor::KNOBFCL]->get(), juce::NotificationType::dontSendNotification);
    FCLKnob.addListener(this);
    addAndMakeVisible(FCLKnob);
    
    SCLKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    SCLKnob.setRange(0.0f, 1.0f);
    SCLKnob.setValue(processorRef.params[PluginProcessor::KNOBSCL]->get(), juce::NotificationType::dontSendNotification);
    SCLKnob.addListener(this);
    addAndMakeVisible(SCLKnob);
    
    FGTKnob.setSliderStyle(juce::Slider::LinearHorizontal);
    FGTKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    FGTKnob.setRange(0.0f, 1.0f);
    FGTKnob.setValue(processorRef.params[PluginProcessor::KNOBFGT]->get(), juce::NotificationType::dontSendNotification);
    FGTKnob.addListener(this);
    addAndMakeVisible(FGTKnob);
    
    SGTKnob.setSliderStyle(juce::Slider::LinearHorizontal);
    SGTKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    SGTKnob.setRange(0.0f, 1.0f);
    SGTKnob.setValue(processorRef.params[PluginProcessor::KNOBSGT]->get(), juce::NotificationType::dontSendNotification);
    SGTKnob.addListener(this);
    addAndMakeVisible(SGTKnob);
    
    FGRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    FGRKnob.setRange(0.0f, 1.0f);
    FGRKnob.setValue(processorRef.params[PluginProcessor::KNOBFGR]->get(), juce::NotificationType::dontSendNotification);
    FGRKnob.addListener(this);
    addAndMakeVisible(FGRKnob);
    FGRKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.2f, 0.466f, 0.2f, 1.0f)); //fixed thumb color green

    SGRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    SGRKnob.setRange(0.0f, 1.0f);
    SGRKnob.setValue(processorRef.params[PluginProcessor::KNOBSGR]->get(), juce::NotificationType::dontSendNotification);
    SGRKnob.addListener(this);
    addAndMakeVisible(SGRKnob);
    SGRKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.2f, 0.466f, 0.2f, 1.0f)); //fixed thumb color green

    FGSKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    FGSKnob.setRange(0.0f, 1.0f);
    FGSKnob.setValue(processorRef.params[PluginProcessor::KNOBFGS]->get(), juce::NotificationType::dontSendNotification);
    FGSKnob.addListener(this);
    addAndMakeVisible(FGSKnob);
    FGSKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.2f, 0.466f, 0.2f, 1.0f)); //fixed thumb color green

    SGSKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    SGSKnob.setRange(0.0f, 1.0f);
    SGSKnob.setValue(processorRef.params[PluginProcessor::KNOBSGS]->get(), juce::NotificationType::dontSendNotification);
    SGSKnob.addListener(this);
    addAndMakeVisible(SGSKnob);
    SGSKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.2f, 0.466f, 0.2f, 1.0f)); //fixed thumb color green

    FGLKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    FGLKnob.setRange(0.0f, 1.0f);
    FGLKnob.setValue(processorRef.params[PluginProcessor::KNOBFGL]->get(), juce::NotificationType::dontSendNotification);
    FGLKnob.addListener(this);
    addAndMakeVisible(FGLKnob);
    FGLKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.2f, 0.466f, 0.2f, 1.0f)); //fixed thumb color green

    SGLKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    SGLKnob.setRange(0.0f, 1.0f);
    SGLKnob.setValue(processorRef.params[PluginProcessor::KNOBSGL]->get(), juce::NotificationType::dontSendNotification);
    SGLKnob.addListener(this);
    addAndMakeVisible(SGLKnob);
    SGLKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.2f, 0.466f, 0.2f, 1.0f)); //fixed thumb color green

    TRFKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    TRFKnob.setRange(0.0f, 1.0f);
    TRFKnob.setValue(processorRef.params[PluginProcessor::KNOBTRF]->get(), juce::NotificationType::dontSendNotification);
    TRFKnob.addListener(this);
    addAndMakeVisible(TRFKnob);
    TRFKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.698f, 0.152f, 0.164f, 1.0f)); //fixed thumb color red
    
    TRGKnob.setSliderStyle(juce::Slider::Rotary);
    TRGKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    TRGKnob.setRange(0.0f, 1.0f);
    TRGKnob.setValue(processorRef.params[PluginProcessor::KNOBTRG]->get(), juce::NotificationType::dontSendNotification);
    TRGKnob.addListener(this);
    addAndMakeVisible(TRGKnob);
    TRGKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.698f, 0.152f, 0.164f, 1.0f)); //fixed thumb color red

    TRRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    TRRKnob.setRange(0.0f, 1.0f);
    TRRKnob.setValue(processorRef.params[PluginProcessor::KNOBTRR]->get(), juce::NotificationType::dontSendNotification);
    TRRKnob.addListener(this);
    addAndMakeVisible(TRRKnob);
    TRRKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.698f, 0.152f, 0.164f, 1.0f)); //fixed thumb color red

    HMFKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    HMFKnob.setRange(0.0f, 1.0f);
    HMFKnob.setValue(processorRef.params[PluginProcessor::KNOBHMF]->get(), juce::NotificationType::dontSendNotification);
    HMFKnob.addListener(this);
    addAndMakeVisible(HMFKnob);
    HMFKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.2f, 0.466f, 0.2f, 1.0f)); //fixed thumb color green

    HMGKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    HMGKnob.setRange(0.0f, 1.0f);
    HMGKnob.setValue(processorRef.params[PluginProcessor::KNOBHMG]->get(), juce::NotificationType::dontSendNotification);
    HMGKnob.addListener(this);
    addAndMakeVisible(HMGKnob);
    HMGKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.2f, 0.466f, 0.2f, 1.0f)); //fixed thumb color green

    HMRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    HMRKnob.setRange(0.0f, 1.0f);
    HMRKnob.setValue(processorRef.params[PluginProcessor::KNOBHMR]->get(), juce::NotificationType::dontSendNotification);
    HMRKnob.addListener(this);
    addAndMakeVisible(HMRKnob);
    HMRKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.2f, 0.466f, 0.2f, 1.0f)); //fixed thumb color green

    LMFKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    LMFKnob.setRange(0.0f, 1.0f);
    LMFKnob.setValue(processorRef.params[PluginProcessor::KNOBLMF]->get(), juce::NotificationType::dontSendNotification);
    LMFKnob.addListener(this);
    addAndMakeVisible(LMFKnob);
    LMFKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.078f, 0.466f, 0.662f, 1.0f)); //fixed thumb color blue

    LMGKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    LMGKnob.setRange(0.0f, 1.0f);
    LMGKnob.setValue(processorRef.params[PluginProcessor::KNOBLMG]->get(), juce::NotificationType::dontSendNotification);
    LMGKnob.addListener(this);
    addAndMakeVisible(LMGKnob);
    LMGKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.078f, 0.466f, 0.662f, 1.0f)); //fixed thumb color blue

    LMRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    LMRKnob.setRange(0.0f, 1.0f);
    LMRKnob.setValue(processorRef.params[PluginProcessor::KNOBLMR]->get(), juce::NotificationType::dontSendNotification);
    LMRKnob.addListener(this);
    addAndMakeVisible(LMRKnob);
    LMRKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.078f, 0.466f, 0.662f, 1.0f)); //fixed thumb color blue

    BSFKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    BSFKnob.setRange(0.0f, 1.0f);
    BSFKnob.setValue(processorRef.params[PluginProcessor::KNOBBSF]->get(), juce::NotificationType::dontSendNotification);
    BSFKnob.addListener(this);
    addAndMakeVisible(BSFKnob);
    BSFKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.513f, 0.27f, 0.153f, 1.0f)); //fixed thumb color brown

    BSGKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    BSGKnob.setRange(0.0f, 1.0f);
    BSGKnob.setValue(processorRef.params[PluginProcessor::KNOBBSG]->get(), juce::NotificationType::dontSendNotification);
    BSGKnob.addListener(this);
    addAndMakeVisible(BSGKnob);
    BSGKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.513f, 0.27f, 0.153f, 1.0f)); //fixed thumb color brown

    BSRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    BSRKnob.setRange(0.0f, 1.0f);
    BSRKnob.setValue(processorRef.params[PluginProcessor::KNOBBSR]->get(), juce::NotificationType::dontSendNotification);
    BSRKnob.addListener(this);
    addAndMakeVisible(BSRKnob);
    BSRKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.513f, 0.27f, 0.153f, 1.0f)); //fixed thumb color brown

    DSCKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    DSCKnob.setRange(0.0f, 1.0f);
    DSCKnob.setValue(processorRef.params[PluginProcessor::KNOBDSC]->get(), juce::NotificationType::dontSendNotification);
    DSCKnob.addListener(this);
    addAndMakeVisible(DSCKnob);
    DSCKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.698f, 0.152f, 0.164f, 1.0f)); //fixed thumb color red
    
    PANKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    PANKnob.setRange(0.0f, 1.0f);
    PANKnob.setValue(processorRef.params[PluginProcessor::KNOBPAN]->get(), juce::NotificationType::dontSendNotification);
    PANKnob.addListener(this);
    addAndMakeVisible(PANKnob);
    
    FADKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    FADKnob.setRange(0.0f, 1.0f);
    FADKnob.setValue(processorRef.params[PluginProcessor::KNOBFAD]->get(), juce::NotificationType::dontSendNotification);
    FADKnob.addListener(this);
    addAndMakeVisible(FADKnob);
    
    setSize (airwindowsLookAndFeel.userWidth, airwindowsLookAndFeel.userHeight);
    // Make sure that before the constructor has finished, you've set the editor's size to whatever you need it to be.
}

PluginEditor::~PluginEditor(){
    setLookAndFeel(nullptr);
}

void PluginEditor::paint (juce::Graphics& g)
{
    if (airwindowsLookAndFeel.blurImage == juce::Image()) {
        g.fillAll (airwindowsLookAndFeel.defaultColour);
        if (hostTrackColour != juce::Colour()) {
            g.setFillType(juce::FillType(hostTrackColour)); g.setOpacity(airwindowsLookAndFeel.applyTrackColour); g.fillAll();
        }
        airwindowsLookAndFeel.setColour(juce::ResizableWindow::backgroundColourId, airwindowsLookAndFeel.defaultColour.interpolatedWith (hostTrackColour, airwindowsLookAndFeel.applyTrackColour));
        airwindowsLookAndFeel.setColour(juce::Slider::thumbColourId, airwindowsLookAndFeel.defaultColour.interpolatedWith (hostTrackColour, airwindowsLookAndFeel.applyTrackColour));
    } else {
        if (airwindowsLookAndFeel.usingNamedImage) {
            g.drawImageWithin(airwindowsLookAndFeel.backgroundImage, 0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight(), 0);
        } else {
            g.setTiledImageFill(airwindowsLookAndFeel.backgroundImage, 0, 0, 1.0f); g.fillAll();
        }
                
        if (hostTrackColour != juce::Colour()) {
            g.setFillType(juce::FillType(hostTrackColour)); g.setOpacity(airwindowsLookAndFeel.applyTrackColour); g.fillAll();
        }
        airwindowsLookAndFeel.defaultColour = juce::Colour::fromRGBA(airwindowsLookAndFeel.blurImage.getPixelAt(1,1).getRed(),airwindowsLookAndFeel.blurImage.getPixelAt(1,1).getGreen(),airwindowsLookAndFeel.blurImage.getPixelAt(1,1).getBlue(),1.0);
        airwindowsLookAndFeel.setColour(juce::ResizableWindow::backgroundColourId, airwindowsLookAndFeel.defaultColour);
        airwindowsLookAndFeel.setColour(juce::Slider::thumbColourId, airwindowsLookAndFeel.defaultColour);
    } //find the color of the background tile or image, if there is one. Please use low-contrast stuff, but I'm not your mom :)
    
    g.setFont(juce::Font(airwindowsLookAndFeel.newFont, g.getCurrentFont().getHeight(), 0));
    auto linewidth = getLocalBounds().getWidth(); if (getLocalBounds().getHeight() > linewidth) linewidth = getLocalBounds().getHeight();  linewidth = (int)cbrt(linewidth/2)/2;
    if ((hostTrackName == juce::String()) || (hostTrackName.length() < 1.0f)) hostTrackName = juce::String("ConsoleX Channel");
    meter.displayTrackName = hostTrackName; //if not track name, then name of plugin. To be displayed on the actual peakmeter
    meter.displayFont = airwindowsLookAndFeel.newFont; //in the custom font, if we're using one
        
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.618f));
    g.fillRect(0, 0, getLocalBounds().getWidth(), linewidth); g.fillRect(0, 0, linewidth, getLocalBounds().getHeight());
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.382f));
    g.fillRect(linewidth, getLocalBounds().getHeight()-linewidth, getLocalBounds().getWidth(), linewidth); g.fillRect(getLocalBounds().getWidth()-linewidth, linewidth, linewidth, getLocalBounds().getHeight()-linewidth);
    g.setColour (juce::Colours::black); g.drawRect(0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight());
    //draw global bevel effect, either from the color or from the color of the blurred texture, and a black border
}

void PluginEditor::resized()
{
    auto area = getLocalBounds(); // this is a huge huge routine, but not all of it runs at all times!
    auto linewidth = area.getWidth();
    float aspectRatio = (float)linewidth / (float)area.getHeight(); //Larger than 1: horisontal. Smaller than 1: vertical
    // 12h-1w = 0.11  (0.26)  6h-2w = 0.41   4h-3w = 1.0    3h-4w = 1.8    2h-6w = 3.85     1h-12w = 15.42
    if (area.getHeight() > linewidth) linewidth = area.getHeight();
    linewidth = (int)cbrt(linewidth/2)/2;
    area.reduce(linewidth, linewidth);
    meter.setBounds(area.getProportion(juce::Rectangle{((float)linewidth*2.0f)/area.getWidth(), (float)(linewidth*2.0f)/area.getHeight(), 1.0f-(((float)linewidth*4.0f)/area.getWidth()), (float)((linewidth*8.0f)/area.getHeight())}));
    inspectButton.setBounds(0, 0, 30, 10);
    area.removeFromTop((int)(linewidth*8.0f)); //remaining area is for controls. getProportion sets first start X and Y placement, then size X and Y placement
    float skew = airwindowsLookAndFeel.applyTilt; //this is the amount of tilt the knobs experience at top and bottom. MAX 0.5 becomes full tilt
    
    float lilKnob = 0.34f; float bigKnob = 0.69f;
    float sliderW = 0.94f; float sliderH = 0.32f; //these are knob sizes scaled to the size of the block
    
    float A1x = 0.15f; float A1y = 0.12f; float A2x = 0.06f; float A3x = 0.6f; float A23y = 0.7f; //EQs
    float B1x = 0.15f; float B1y = 0.34f; float B2x = 0.6f; float B2y = 0.12f; //stone fire fader pan
    //knobs go large to small, then top to bottom, left to right. threshold slider is biggest, gets to be first
    //###now do the three knob and slider ones. 1 is always the slider, 234y are the same
    float C1x = 0.05f; float C1y = 0.5f; float C2x = 0.0f; float C3x = 0.3333f; float C4x = 0.6666f; float C234y = 0.12f;
    //these are always the same for all aspect ratios, but panels and offsets are unique to the aspect ratios, as are the sequence of control blocks
    
    if (aspectRatio >= 0.0f && aspectRatio < 0.18f) { //12h-1w
        float pY = (float)area.getHeight()/12.0f; //size of each movable block
        float pX = (float)area.getWidth()/1.0f;
        float offsetY = 0.0f; float offsetX = 0.0f; float panelTilt = 0.5f; //1 and 2 wide don't have tilted knobs
        FCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        FCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        FCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        FCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //FireComp Thresh Ratio Attack Release D block
        offsetY = pY; //put between vertical SECTIONS (not knobs)
        FIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        AIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        AIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Fire Air B block
        offsetY = pY*2.0f; //put between vertical SECTIONS (not knobs)
        FGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        FGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        FGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        FGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //FireGate Thresh Ratio Sustain Release E block
        offsetY = pY*3.0f; //put between vertical SECTIONS (not knobs)
        SCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        SCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        SCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        SCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //StoneComp Thresh Ratio Attack Release D block
        offsetY = pY*4.0f; //put between vertical SECTIONS (not knobs)
        STOKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        STOKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        RNGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        RNGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Stone Range B block
        offsetY = pY*5.0f; //put between vertical SECTIONS (not knobs)
        SGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        SGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        SGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        SGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //StoneGate Thresh Ratio Sustain Release E block
        offsetY = pY*6.0f; //put between vertical SECTIONS (not knobs)
        LOPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LOPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        HIPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HIPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        DSCKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        DSCKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Lowpass Highpass Discontinuity A block
        offsetY = pY*7.0f; //put between vertical SECTIONS (not knobs)
        TRGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        TRRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        TRFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Treble Width Freq A block
        offsetY = pY*8.0f; //put between vertical SECTIONS (not knobs)
        HMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        HMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        HMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //HighMid Width Freq A block
        offsetY = pY*9.0f; //put between vertical SECTIONS (not knobs)
        LMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        LMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        LMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //LowMid Width Freq A block
        offsetY = pY*10.0f; //put between vertical SECTIONS (not knobs)
        BSGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        BSRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        BSFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Bass Width Freq A block
        offsetY = pY*11.0f; //put between vertical SECTIONS (not knobs)
        FADKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FADKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        PANKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        PANKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Pan Fader C block
    } //12h-1w
    
    if (aspectRatio >= 0.18f && aspectRatio < 0.7f) { //6h-2w
        float pY = (float)area.getHeight()/6.0f; //size of each movable block
        float pX = (float)area.getWidth()/2.0f;
        float offsetY = 0.0f; float offsetX = 0.0f; float panelTilt = 0.5f; //1 and 2 wide don't have angled knobs
        FCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        FCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        FCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        FCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //FireComp Thresh Ratio Attack Release D block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        TRGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        TRRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        TRFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Treble Width Freq A block
        
        offsetY = pY; offsetX = 0.0f; //put between vertical SECTIONS (not knobs)
        FIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        AIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        AIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Fire Air B block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        HMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        HMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        HMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //HighMid Width Freq A block

        offsetY = pY*1.99f; offsetX = 0.0f; //put between vertical SECTIONS (not knobs)
        FGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        FGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        FGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        FGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //FireGate Thresh Ratio Sustain Release E block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        LMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        LMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        LMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //LowMid Width Freq A block
 
        offsetY = pY*2.98f; offsetX = 0.0f; //put between vertical SECTIONS (not knobs)
        SCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        SCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        SCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        SCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //StoneComp Thresh Ratio Attack Release D block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        BSGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        BSRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        BSFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Bass Width Freq A block

        offsetY = pY*3.97f; offsetX = 0.0f; //put between vertical SECTIONS (not knobs)
        STOKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        STOKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        RNGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        RNGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Stone Range B block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        LOPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LOPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        HIPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HIPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        DSCKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        DSCKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Lowpass Highpass Discontinuity A block

        offsetY = pY*4.96f; offsetX = 0.0f; //put between vertical SECTIONS (not knobs)
        SGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        SGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        SGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        SGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //StoneGate Thresh Ratio Sustain Release E block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FADKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FADKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        PANKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        PANKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Pan Fader C block
    } //6h-2w

    if (aspectRatio >= 0.7f && aspectRatio < 1.2f) { //4h-3w
        float pX = (float)area.getWidth()/3.0f;
        float offsetX = 0.0f; float panelTilt = 0.5f-skew; //update the new panel tilt each time offsetY is updated. 0.0 is top panel, seen from underneath. 1.0 is bottom panel, seen from above
        float pY = (float)(area.getHeight()/4.0f);
        float offsetY = 0.0f;
        pY -= (pY*skew*0.4f); //top panel is smaller
        TRGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        TRRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        TRFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //Treble Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        SCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), ((float)(area.getHeight()/4.0f)*sliderH)/area.getHeight()}));
        SCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        SCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        SCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //StoneComp Thresh Ratio Attack Release D block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), ((float)(area.getHeight()/4.0f)*sliderH)/area.getHeight()}));
        FCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        FCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        FCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //FireComp Thresh Ratio Attack Release D block
        
        pY = (float)area.getHeight()/4.0f;
        offsetY = (pY-(pY*skew*1.0f)); offsetX = 0.0f; panelTilt = 0.5f-(skew*0.75f); //put between vertical SECTIONS (not knobs)
        pY += (pY*skew*1.1f); //center panels are larger
        HMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        HMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        HMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //HighMid Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        STOKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        STOKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        RNGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        RNGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //Stone Range B block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        AIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        AIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //Fire Air B block

        pY = (float)area.getHeight()/4.0f;
        offsetY = ((pY*1.99f)-(pY*skew*0.384f)); offsetX = 0.0f; panelTilt = 0.5f+(skew*0.75f); //put between vertical SECTIONS (not knobs)
        pY += (pY*skew*1.1f); //center panels are larger
        LMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        LMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        LMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //LowMid Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        SGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), ((float)(area.getHeight()/4.0f)*sliderH)/area.getHeight()}));
        SGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        SGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        SGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //StoneGate Thresh Ratio Sustain Release E block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), ((float)(area.getHeight()/4.0f)*sliderH)/area.getHeight()}));
        FGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        FGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        FGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //FireGate Thresh Ratio Sustain Release E block
        
        pY = (float)area.getHeight()/4.0f;
        offsetY = (pY*2.98f)+(pY*skew*0.5f); offsetX = 0.0f; panelTilt = 0.5f+skew; //put between vertical SECTIONS (not knobs)
        pY -= (pY*skew*0.4f); //bottom panel is smaller
        BSGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        BSRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        BSFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //Bass Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        LOPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LOPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        HIPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HIPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        DSCKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        DSCKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //Lowpass Highpass Discontinuity A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FADKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FADKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        PANKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        PANKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //Pan Fader C block
    } //4h-3w
    
    if (aspectRatio >= 1.2f && aspectRatio < 3.0f) { //3h-4w
        float pX = (float)area.getWidth()/4.0f;
        float offsetX = 0.0f; float panelTilt = 0.5f-skew; //update the new panel tilt each time offsetY is updated. 0.0 is top panel, seen from underneath. 1.0 is bottom panel, seen from above
        float offsetY = 0.0f;
        float pY = (float)area.getHeight()/3.0f; //size of each movable block
        pY -= (pY*skew*0.33f); //top panel is smaller
        
        BSGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        BSRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        BSFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //Bass Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        LMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        LMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        LMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //LowMid Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        HMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        HMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        HMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //HighMid Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        TRGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        TRRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        TRFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //Treble Width Freq A block
        
        pY = (float)area.getHeight()/3.0f;
        offsetY = pY-(pY*skew*0.6f); offsetX = 0.0f; panelTilt = 0.5f; //put between vertical SECTIONS (not knobs)
        pY += (pY*skew*0.95f); //middle panel is larger
        
        FCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), ((float)(area.getHeight()/4.0f)*sliderH)/area.getHeight()}));
        FCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        FCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        FCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //FireComp Thresh Ratio Attack Release D block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        AIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        AIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Fire Air B block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), ((float)(area.getHeight()/4.0f)*sliderH)/area.getHeight()}));
        FGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        FGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        FGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //FireGate Thresh Ratio Sustain Release E block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        LOPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LOPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        HIPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HIPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        DSCKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        DSCKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Lowpass Highpass Discontinuity A block
        
        pY = (float)area.getHeight()/3.0f;
        offsetY = (pY*2.0f)+(pY*skew*0.33f); offsetX = 0.0f; panelTilt = 0.5f+skew; //put between vertical SECTIONS (not knobs)
        pY -= (pY*skew*0.33f); //bottom panel is smaller
        
        SCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), ((float)(area.getHeight()/4.0f)*sliderH)/area.getHeight()}));
        SCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        SCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        SCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //StoneComp Thresh Ratio Attack Release D block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        STOKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        STOKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        RNGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        RNGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //Stone Range B block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        SGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), ((float)(area.getHeight()/4.0f)*sliderH)/area.getHeight()}));
        SGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        SGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        SGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //StoneGate Thresh Ratio Sustain Release E block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FADKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FADKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        PANKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        PANKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //Pan Fader C block
    } //3h-4w

    if (aspectRatio >= 3.0f && aspectRatio < 5.0f) { //2h-6w
        float pY = (float)area.getHeight()/2.0f; //size of each movable block
        float pX = (float)area.getWidth()/6.0f;
        float offsetY = 0.0f; float offsetX = 0.0f; float panelTilt = 0.5f; //2 or 1 high don't have tilted knobs
        SCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        SCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        SCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        SCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //StoneComp Thresh Ratio Attack Release D block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        STOKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        STOKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        RNGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        RNGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Stone Range B block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        SGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        SGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        SGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        SGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //StoneGate Thresh Ratio Sustain Release E block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        FCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        FCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        FCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //FireComp Thresh Ratio Attack Release D block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        AIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        AIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Fire Air B block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        FGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        FGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        FGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //FireGate Thresh Ratio Sustain Release E block

        offsetY = (pY*0.9f); offsetX = 0.0f; //put between vertical SECTIONS (not knobs)
        BSGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        BSRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        BSFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Bass Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        LMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        LMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        LMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //LowMid Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        HMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        HMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        HMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //HighMid Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        TRGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        TRRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        TRFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Treble Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        LOPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LOPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        HIPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HIPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        DSCKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        DSCKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Lowpass Highpass Discontinuity A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FADKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FADKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        PANKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        PANKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Pan Fader C block
     } //2h-6w
    
    if (aspectRatio >= 5.0f && aspectRatio < 999999.0f) { //1h-12w
        float pY = (float)area.getHeight()/1.0f; //size of each movable block
        float pX = (float)area.getWidth()/12.0f;
        float offsetY = 0.0f; float offsetX = 0.0f; float panelTilt = 0.5f; //update the new panel tilt each time offsetY is updated. 0.0 is top panel, seen from underneath. 1.0 is bottom panel, seen from above
        FCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        FCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        FCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        FCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //FireComp Thresh Ratio Attack Release D block
        offsetX = pX; //put between vertical SECTIONS (not knobs)
        FIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        AIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        AIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Fire Air B block
        offsetX = pX*2.0f; //put between vertical SECTIONS (not knobs)
        FGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        FGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        FGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        FGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //FireGate Thresh Ratio Sustain Release E block
        offsetX = pX*3.0f; //put between vertical SECTIONS (not knobs)
        SCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        SCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        SCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        SCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //StoneComp Thresh Ratio Attack Release D block
        offsetX = pX*4.0f; //put between vertical SECTIONS (not knobs)
        STOKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        STOKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        RNGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        RNGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Stone Range B block
        offsetX = pX*5.0f; //put between vertical SECTIONS (not knobs)
        SGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        SGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        SGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C3x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        SGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C4x))/area.getWidth(), (offsetY+(pY*C234y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //StoneGate Thresh Ratio Sustain Release E block
        offsetX = pX*6.0f; //put between vertical SECTIONS (not knobs)
        LOPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LOPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        HIPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HIPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        DSCKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        DSCKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Lowpass Highpass Discontinuity A block
        offsetX = pX*7.0f; //put between vertical SECTIONS (not knobs)
        TRGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        TRRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        TRFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Treble Width Freq A block
        offsetX = pX*8.0f; //put between vertical SECTIONS (not knobs)
        HMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        HMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        HMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //HighMid Width Freq A block
        offsetX = pX*9.0f; //put between vertical SECTIONS (not knobs)
        LMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        LMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        LMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //LowMid Width Freq A block
        offsetX = pX*10.0f; //put between vertical SECTIONS (not knobs)
        BSGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        BSRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()}));
        BSFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Bass Width Freq A block
        offsetX = pX*11.0f; //put between vertical SECTIONS (not knobs)
        FADKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FADKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*bigKnob)/area.getWidth(), (pY*bigKnob)/area.getHeight()}));
        PANKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        PANKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*lilKnob)/area.getWidth(), (pY*lilKnob)/area.getHeight()})); //Pan Fader C block
    } //1h-12w

}

void PluginEditor::sliderValueChanged(juce::Slider *s)
{
    if (s == &HIPKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBHIP;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &LOPKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBLOP;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &AIRKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBAIR;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FIRKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFIR;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &STOKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSTO;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &RNGKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBRNG;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FCTKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFCT;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SCTKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSCT;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FCRKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFCR;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SCRKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSCR;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FCAKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFCA;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SCAKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSCA;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FCLKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFCL;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SCLKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSCL;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FGTKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFGT;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SGTKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSGT;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FGRKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFGR;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SGRKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSGR;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FGSKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFGS;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SGSKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSGS;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FGLKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFGL;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SGLKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSGL;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &TRFKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBTRF;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &TRGKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBTRG;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &TRRKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBTRR;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &HMFKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBHMF;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &HMGKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBHMG;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &HMRKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBHMR;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &LMFKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBLMF;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &LMGKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBLMG;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &LMRKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBLMR;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &BSFKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBBSF;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &BSGKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBBSG;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &BSRKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBBSR;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &DSCKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBDSC;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &PANKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBPAN;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FADKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFAD;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
}

void PluginEditor::sliderDragStarted(juce::Slider *s) {sliderDragInternal(s, true);}
void PluginEditor::sliderDragEnded(juce::Slider *s) {sliderDragInternal(s, false);}
void PluginEditor::sliderDragInternal(juce::Slider *s, bool bv)
{
    if (s == &HIPKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBHIP;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &LOPKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBLOP;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &AIRKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBAIR;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FIRKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFIR;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &STOKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSTO;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &RNGKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBRNG;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FCTKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFCT;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SCTKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSCT;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FCRKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFCR;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SCRKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSCR;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FCAKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFCA;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SCAKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSCA;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FCLKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFCL;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SCLKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSCL;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FGTKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFGT;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SGTKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSGT;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FGRKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFGR;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SGRKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSGR;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FGSKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFGS;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SGSKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSGS;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FGLKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFGL;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SGLKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSGL;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &TRFKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBTRF;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &TRGKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBTRG;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &TRRKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBTRR;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &HMFKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBHMF;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &HMGKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBHMG;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &HMRKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBHMR;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &LMFKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBLMF;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &LMGKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBLMG;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &LMRKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBLMR;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &BSFKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBBSF;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &BSGKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBBSG;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &BSRKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBBSR;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &DSCKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBDSC;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &PANKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBPAN;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FADKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFAD;
        processorRef.uiToAudio.push(msg);
    }
}

void PluginEditor::updateTrackProperties() {hostTrackColour=processorRef.trackProperties.colour; hostTrackName=processorRef.trackProperties.name; repaint();}

void PluginEditor::idle()
{
    PluginProcessor::AudioToUIMessage msg;
    bool repaintTS{false}; //we don't redraw interface just for getting data into the GUI section
    while (processorRef.audioToUI.pop(msg)) {
        switch (msg.what) {
        case PluginProcessor::AudioToUIMessage::NEW_VALUE:
                if (msg.which == PluginProcessor::KNOBHIP) {HIPKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBLOP) {LOPKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBAIR) {AIRKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBFIR) {FIRKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBSTO) {STOKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBRNG) {RNGKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBFCT) {FCTKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBSCT) {SCTKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBFCR) {FCRKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBSCR) {SCRKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBFCA) {FCAKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBSCA) {SCAKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBFCL) {FCLKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBSCL) {SCLKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBFGT) {FGTKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBSGT) {SGTKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBFGR) {FGRKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBSGR) {SGRKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBFGS) {FGSKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBSGS) {SGSKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBFGL) {FGLKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBSGL) {SGLKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBTRF) {TRFKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBTRG) {TRGKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBTRR) {TRRKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBHMF) {HMFKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBHMG) {HMGKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBHMR) {HMRKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBLMF) {LMFKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBLMG) {LMGKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBLMR) {LMRKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBBSF) {BSFKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBBSG) {BSGKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBBSR) {BSRKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBDSC) {DSCKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBPAN) {PANKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBFAD) {FADKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                break; //this can grab the knobs away from the user! Should cause the knob to repaint, too.
                
        case PluginProcessor::AudioToUIMessage::SLEW_LEFT: meter.pushA(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::SLEW_RIGHT: meter.pushB(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::PEAK_LEFT: meter.pushC(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::PEAK_RIGHT: meter.pushD(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::RMS_LEFT: meter.pushE(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::RMS_RIGHT: meter.pushF(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::ZERO_LEFT: meter.pushG(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::ZERO_RIGHT: meter.pushH(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::BLINKEN_FIRE_COMP: meter.pushI(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::BLINKEN_STONE_COMP: meter.pushJ(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::BLINKEN_FIRE_GATE: meter.pushK(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::BLINKEN_STONE_GATE: meter.pushL(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::INCREMENT: //Increment is running at 24 FPS and giving the above calculations
                meter.pushIncrement(); repaintTS = true; //we will repaint GUI after doing the following
                
                FCTKnob.setColour(juce::Slider::thumbColourId, juce::Colour::fromFloatRGBA (meter.blinkenFireComp*airwindowsLookAndFeel.LEDColour.getFloatRed(), meter.blinkenFireComp*airwindowsLookAndFeel.LEDColour.getFloatGreen(), meter.blinkenFireComp*airwindowsLookAndFeel.LEDColour.getFloatBlue(), 1.0f));
                
                SCTKnob.setColour(juce::Slider::thumbColourId, juce::Colour::fromFloatRGBA (meter.blinkenStoneComp*airwindowsLookAndFeel.LEDColour.getFloatRed(), meter.blinkenStoneComp*airwindowsLookAndFeel.LEDColour.getFloatGreen(), meter.blinkenStoneComp*airwindowsLookAndFeel.LEDColour.getFloatBlue(), 1.0f));
                
                FGTKnob.setColour(juce::Slider::thumbColourId, juce::Colour::fromFloatRGBA (meter.blinkenFireGate*airwindowsLookAndFeel.LEDColour.getFloatRed(), meter.blinkenFireGate*airwindowsLookAndFeel.LEDColour.getFloatGreen(), meter.blinkenFireGate*airwindowsLookAndFeel.LEDColour.getFloatBlue(), 1.0f));
                
                SGTKnob.setColour(juce::Slider::thumbColourId, juce::Colour::fromFloatRGBA (meter.blinkenStoneGate*airwindowsLookAndFeel.LEDColour.getFloatRed(), meter.blinkenStoneGate*airwindowsLookAndFeel.LEDColour.getFloatGreen(), meter.blinkenStoneGate*airwindowsLookAndFeel.LEDColour.getFloatBlue(), 1.0f));
                
                //User color LEDS are done like this: choose the same meter.data selection for each, and then the color will always be the user color
                //SGTKnob.setColour(juce::Slider::thumbColourId, juce::Colour::fromFloatRGBA (meter.inputRMSL, meter.inputRMSL, meter.inputRMSL, 1.0f));
                //for an RGB or specified color blinken-knob, we don't reference user color, instead we just use the multiple meter.data directly without bringing in LEDColour
                
                //here is where we can make any control's thumb be a continuous blinkenlight with any value in meter. It runs at about 24fps.
                //We can do if statements etc. here, only thing we can NOT do is instantiate new variables. Do it as shown above and it works.
                //Also, this defaults to colors flashing against black, like LEDs: that's a good way to distinguish blinken-knobs from knobs
                break;
        default: break;
        } //end of switch statement for msg.what
    } if (repaintTS) meter.repaint();
}
