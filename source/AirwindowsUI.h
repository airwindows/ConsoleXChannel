// AirwindowsUI by Chris Johnson
// Initial seed code for the meter created by Paul Walker on 8/23/21.

#ifndef AIRWINDOWSUI_H
#define AIRWINDOWSUI_H
#include "PluginProcessor.h"


class AirwindowsLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider) override;
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override;

    AirwindowsLookAndFeel()
    {
        setColour(juce::Slider::backgroundColourId, juce::Colours::red);
        setColour(juce::Slider::thumbColourId, juce::Colours::grey);
        setColour(juce::Slider::trackColourId, juce::Colours::grey);
        setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::grey);
        setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black); //track after thumb
        setColour(juce::Slider::textBoxTextColourId, juce::Colours::grey);
        setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::grey);
        setColour(juce::Slider::textBoxHighlightColourId, juce::Colours::grey);
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::grey);
        setColour(juce::TextButton::buttonColourId, juce::Colours::lightgrey);
        setColour(juce::TextButton::buttonOnColourId, juce::Colours::lightgrey);
        setColour(juce::TextButton::textColourOffId, juce::Colours::lightgrey);
        setColour(juce::TextButton::textColourOnId, juce::Colours::lightgrey);
        setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::lightgrey);
        
        juce::String newWidth = juce::String(); juce::String namedWidth = JucePlugin_Name; namedWidth.append("Width",1024);
        juce::String newHeight = juce::String(); juce::String namedHeight = JucePlugin_Name; namedHeight.append("Height",1024);
        juce::String namedFont = JucePlugin_Name; namedFont.append("Font",1024);
        juce::String newColour = juce::String(); juce::String namedColour = JucePlugin_Name; namedColour.append("Colour",1024);
        juce::String newImage = juce::String(); juce::String namedImage = JucePlugin_Name; namedImage.append("Image",1024);
        juce::String newApplyTrackColourAmount = juce::String(); juce::String namedApplyTrackColourAmount = JucePlugin_Name; namedApplyTrackColourAmount.append("TrackColourAmount",1024);
        juce::String newLEDColour = juce::String(); juce::String namedLEDColour = JucePlugin_Name; namedLEDColour.append("LEDColour",1024);
        juce::String newKnobMode = juce::String(); juce::String namedKnobMode = JucePlugin_Name; namedKnobMode.append("KnobMode",1024);
        juce::String newInterpolation = juce::String(); juce::String namedInterpolation = JucePlugin_Name; namedInterpolation.append("Interpolation",1024);
        juce::String newTilt = juce::String(); juce::String namedTilt = JucePlugin_Name; namedInterpolation.append("Tilt",1024);

        juce::File customSettings = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory).getChildFile("Airwindows").getChildFile("AirwindowsGlobals.txt");
        juce::String xmlFile = customSettings.loadFileAsString();
        std::unique_ptr<juce::XmlElement> body (juce::XmlDocument::parse (xmlFile));
        if ((xmlFile != juce::String()) && (xmlFile.length() > 0)) {
            for (auto* e : body->getChildIterator()) {
                if (e->hasTagName ("option")) { // find the "PARAM" sub-element
                    juce::String attributeValueAsString = e->getStringAttribute("id");
                    if (attributeValueAsString.equalsIgnoreCase("userWidth") && newWidth == juce::String()) newWidth = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase(namedWidth)) newWidth = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase("userHeight") && newHeight == juce::String()) newHeight = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase(namedHeight)) newHeight = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase("userFont") && newFont == juce::String()) newFont = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase(namedFont)) newFont = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase("userColour") && newColour == juce::String()) newColour = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase(namedColour)) newColour = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase("userImage") && newImage == juce::String()) newImage = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase(namedImage)) {newImage = e->getStringAttribute("value"); usingNamedImage = true;}
                    if (attributeValueAsString.equalsIgnoreCase("userTrackColourAmount") && newApplyTrackColourAmount == juce::String()) newApplyTrackColourAmount = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase(namedApplyTrackColourAmount)) newApplyTrackColourAmount = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase("userLEDColour") && newLEDColour == juce::String()) newLEDColour = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase(namedLEDColour)) newLEDColour = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase("userKnobMode") && newKnobMode == juce::String()) newKnobMode = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase(namedKnobMode)) newKnobMode = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase("userInterpolation") && newInterpolation == juce::String()) newInterpolation = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase(namedInterpolation)) newInterpolation = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase("userTilt") && newTilt == juce::String()) newTilt = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase(namedTilt)) newTilt = e->getStringAttribute("value");
                }
            }
            body.release();
        }
        
        juce::File customBackground = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory).getChildFile("Airwindows").getChildFile (newImage);
        if (customBackground.existsAsFile()) {
            backgroundImage = juce::ImageFileFormat::loadFrom(juce::File(customBackground));
            blurImage = backgroundImage.rescaled(3, 3);
         }
        userWidth = newWidth.getIntValue(); if (userWidth < 8 || userWidth > 16386) userWidth = 900;
        userHeight = newHeight.getIntValue(); if (userHeight < 8 || userHeight > 16386) userHeight = 300;
        //if you've not specified anything or your settings are crazy enough we go with defaults
        defaultColour = juce::Colours::findColourForName(newColour, juce::Colours::lightgrey);
        applyTrackColour = fmax(fmin(newApplyTrackColourAmount.getFloatValue(),1.0f),0.0f);
        LEDColour = juce::Colours::findColourForName(newLEDColour, juce::Colours::red);
        applyTilt = fmax(fmin(newTilt.getFloatValue(),1.0f),0.0f) * 0.5f; //value is 0-1 but in use it's 0-0.5

        knobMode = 1; //defaults to rotary because it allows for really fine adjustments
        if (newKnobMode.equalsIgnoreCase("rotary")) knobMode = 0;
        if (newKnobMode.equalsIgnoreCase("airwindows")) knobMode = 0;
        if (newKnobMode.equalsIgnoreCase("realistic")) knobMode = 0;
        if (newKnobMode.equalsIgnoreCase("vertical")) knobMode = 1;
        if (newKnobMode.equalsIgnoreCase("up and down")) knobMode = 1;
        if (newKnobMode.equalsIgnoreCase("up-and-down")) knobMode = 1;
        if (newKnobMode.equalsIgnoreCase("normal")) knobMode = 1; //I'm told vertical is customary ;)
        if (newKnobMode.equalsIgnoreCase("default")) knobMode = 1; // so here ya go ;)
        if (newKnobMode.equalsIgnoreCase("horizontal")) knobMode = 2;
        if (newKnobMode.equalsIgnoreCase("sideways")) knobMode = 2;
        if (newKnobMode.equalsIgnoreCase("horizontalvertical")) knobMode = 3;
        if (newKnobMode.equalsIgnoreCase("verticalhorizontal")) knobMode = 3;
        if (newKnobMode.equalsIgnoreCase("horizontal-vertical")) knobMode = 3;
        if (newKnobMode.equalsIgnoreCase("vertical-horizontal")) knobMode = 3;
        if (newKnobMode.equalsIgnoreCase("diagonal")) knobMode = 3;
        if (newKnobMode.equalsIgnoreCase("both")) knobMode = 3;
        
        alfInterpolation = 2; //defaults to bicubic 'cloud' interpolation
        if (newInterpolation.equalsIgnoreCase("none")) alfInterpolation = 0;
        if (newInterpolation.equalsIgnoreCase("off")) alfInterpolation = 0;
        if (newInterpolation.equalsIgnoreCase("nearestneighbor")) alfInterpolation = 0;
        if (newInterpolation.equalsIgnoreCase("nearest-neighbor")) alfInterpolation = 0;
        if (newInterpolation.equalsIgnoreCase("nearest neighbor")) alfInterpolation = 0; //variations on mondrian-meter
        if (newInterpolation.equalsIgnoreCase("bilinear")) alfInterpolation = 1; //option for softer definition
     }
    int userWidth;
    int userHeight;
    juce::Colour defaultColour = juce::Colours::lightgrey;
    juce::Image backgroundImage = juce::Image();
    juce::Image blurImage = juce::Image();
    juce::String newFont = juce::String();
    bool usingNamedImage = false;
    float applyTrackColour = 0.5;
    juce::Colour LEDColour = juce::Colours::red;
    int knobMode;
    int alfInterpolation;
    float applyTilt = 0.0;
};

struct AirwindowsMeter : public juce::Component
{
    void paint(juce::Graphics &g) override;
    
    juce::Image barImage = juce::Image(juce::Image::RGB, 34, 5, true);
    juce::String displayTrackName = juce::String();
    juce::String displayFont = juce::String();
    int meterInterpolation = 2;
    static constexpr int dataPoints = 34;
    std::array<float, dataPoints> dataR;
    std::array<float, dataPoints> dataG;
    std::array<float, dataPoints> dataB;
    std::array<float, dataPoints> edgeR;
    std::array<float, dataPoints> edgeG;
    std::array<float, dataPoints> edgeB;
    float inputSlewL;
    float inputSlewR;
    float inputPeakL;
    float inputPeakR;
    float inputRMSL;
    float inputRMSR;
    float inputZeroL;
    float inputZeroR;
    float blinkenFireComp;
    float blinkenStoneComp;
    float blinkenFireGate;
    float blinkenStoneGate;

    void pushA(float X) {inputSlewL = X;}
    void pushB(float X) {inputSlewR = X;}
    void pushC(float X) {inputPeakL = X;}
    void pushD(float X) {inputPeakR = X;}
    void pushE(float X) {inputRMSL = X;}
    void pushF(float X) {inputRMSR = X;}
    void pushG(float X) {inputZeroL = X;}
    void pushH(float X) {inputZeroR = X;}
    void pushI(float X) {blinkenFireComp = X;}
    void pushJ(float X) {blinkenStoneComp = X;}
    void pushK(float X) {blinkenFireGate = X;}
    void pushL(float X) {blinkenStoneGate = X;}
    void pushIncrement() {}
    
   void resetArrays(){
        for (int count = 0; count < dataPoints; ++count) //count through all the points in the array
        {
            dataG[count] = 0.0f;
            dataR[count] = 0.0f;
            dataB[count] = 0.0f;
            edgeG[count] = 0.0f;
            edgeR[count] = 0.0f;
            edgeB[count] = 0.0f;
            barImage = juce::Image(juce::Image::RGB, 34, 5, true);
        }
    }
    float lastLOutline = 0.0;
    float lastROutline = 0.0;
    float lastLPeak = 0.0;
    float lastRPeak = 0.0;
    float lastLSlew = 0.0;
    float lastRSlew = 0.0;
};

extern AirwindowsLookAndFeel airwindowsLookAndFeel;
extern AirwindowsMeter meter;

#endif // AIRWINDOWSUI_H
