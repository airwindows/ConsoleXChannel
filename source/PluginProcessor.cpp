#include "PluginProcessor.h"
#include "PluginEditor.h"
#ifndef M_PI
#  define M_PI (3.14159265358979323846)
#endif
#ifndef M_PI_2
#  define M_PI_2 (1.57079632679489661923)
#endif

//==============================================================================
PluginProcessor::PluginProcessor():AudioProcessor (
                    BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
){
    for (int x = 0; x < hilp_total; x++) {
        highpass[x] = 0.0;
        lowpass[x] = 0.0;
    }
    
    for (int x = 0; x < air_total; x++) air[x] = 0.0;
    for (int x = 0; x < kal_total; x++) kal[x] = 0.0;
    fireCompL = 1.0;
    fireCompR = 1.0;
    fireGate = 1.0;
    stoneCompL = 1.0;
    stoneCompR = 1.0;
    stoneGate = 1.0;
    
    for (int x = 0; x < biqs_total; x++) {
        high[x] = 0.0;
        hmid[x] = 0.0;
        lmid[x] = 0.0;
        bass[x] = 0.0;
    }
    
    for(int count = 0; count < dscBuf+2; count++) {
        dBaL[count] = 0.0;
        dBaR[count] = 0.0;
    }
    dBaPosL = 0.0;
    dBaPosR = 0.0;
    dBaXL = 1;
    dBaXR = 1;
    
    airGainA = 0.5; airGainB = 0.5;
    fireGainA = 0.5; fireGainB = 0.5;
    stoneGainA = 0.5; stoneGainB = 0.5;
    panA = 0.5; panB = 0.5;
    inTrimA = 1.0; inTrimB = 1.0;
    
    fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
    fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
    //this is reset: values being initialized only once. Startup values, whatever they are.

    // (internal ID, how it's shown in DAW generic view, {min, max}, default)
    addParameter(params[KNOBHIP] = new juce::AudioParameterFloat("highpass", "Highpass", {0.0f, 1.0f}, 0.0f));              params[KNOBHIP]->addListener(this);
    addParameter(params[KNOBLOP] = new juce::AudioParameterFloat("lowpass", "Lowpass", {0.0f, 1.0f}, 0.0f));                params[KNOBLOP]->addListener(this);
    addParameter(params[KNOBAIR] = new juce::AudioParameterFloat("air", "Air", {0.0f, 1.0f}, 0.5f));                        params[KNOBAIR]->addListener(this);
    addParameter(params[KNOBFIR] = new juce::AudioParameterFloat("fire", "Fire", {0.0f, 1.0f}, 0.5f));                      params[KNOBFIR]->addListener(this);
    addParameter(params[KNOBSTO] = new juce::AudioParameterFloat("stone", "Stone", {0.0f, 1.0f}, 0.5f));                    params[KNOBSTO]->addListener(this);
    addParameter(params[KNOBRNG] = new juce::AudioParameterFloat("range", "Range", {0.0f, 1.0f}, 0.5f));                    params[KNOBRNG]->addListener(this);
    addParameter(params[KNOBFCT] = new juce::AudioParameterFloat("fcthreshold", "FC Threshold", {0.0f, 1.0f}, 1.0f));       params[KNOBFCT]->addListener(this);
    addParameter(params[KNOBSCT] = new juce::AudioParameterFloat("scthreshold", "SC Threshold", {0.0f, 1.0f}, 1.0f));       params[KNOBSCT]->addListener(this);
    addParameter(params[KNOBFCR] = new juce::AudioParameterFloat("fcratio", "FC Ratio", {0.0f, 1.0f}, 1.0f));               params[KNOBFCR]->addListener(this);
    addParameter(params[KNOBSCR] = new juce::AudioParameterFloat("scratio", "SC Ratio", {0.0f, 1.0f}, 1.0f));               params[KNOBSCR]->addListener(this);
    addParameter(params[KNOBFCA] = new juce::AudioParameterFloat("fcattack", "FC Attack", {0.0f, 1.0f}, 0.5f));             params[KNOBFCA]->addListener(this);
    addParameter(params[KNOBSCA] = new juce::AudioParameterFloat("scattack", "SC ATtack", {0.0f, 1.0f}, 0.5f));             params[KNOBSCA]->addListener(this);
    addParameter(params[KNOBFCL] = new juce::AudioParameterFloat("fcrelease", "FC Release", {0.0f, 1.0f}, 0.5f));           params[KNOBFCL]->addListener(this);
    addParameter(params[KNOBSCL] = new juce::AudioParameterFloat("screlease", "SC Release", {0.0f, 1.0f}, 0.5f));           params[KNOBSCL]->addListener(this);
    addParameter(params[KNOBFGT] = new juce::AudioParameterFloat("fgthreshold", "FG Threshold", {0.0f, 1.0f}, 0.0f));       params[KNOBFGT]->addListener(this);
    addParameter(params[KNOBSGT] = new juce::AudioParameterFloat("sgthreshold", "SG Threshold", {0.0f, 1.0f}, 0.0f));       params[KNOBSGT]->addListener(this);
    addParameter(params[KNOBFGR] = new juce::AudioParameterFloat("fgratio", "FG Ratio", {0.0f, 1.0f}, 1.0f));               params[KNOBFGR]->addListener(this);
    addParameter(params[KNOBSGR] = new juce::AudioParameterFloat("sgratio", "SG Ratio", {0.0f, 1.0f}, 1.0f));               params[KNOBSGR]->addListener(this);
    addParameter(params[KNOBFGS] = new juce::AudioParameterFloat("fgsustain", "FG Sustain", {0.0f, 1.0f}, 0.5f));           params[KNOBFGS]->addListener(this);
    addParameter(params[KNOBSGS] = new juce::AudioParameterFloat("sgsustain", "SG Sustain", {0.0f, 1.0f}, 0.5f));           params[KNOBSGS]->addListener(this);
    addParameter(params[KNOBFGL] = new juce::AudioParameterFloat("fgrelease", "FG Release", {0.0f, 1.0f}, 0.5f));           params[KNOBFGL]->addListener(this);
    addParameter(params[KNOBSGL] = new juce::AudioParameterFloat("sgrelease", "SG Release", {0.0f, 1.0f}, 0.5f));           params[KNOBSGL]->addListener(this);
    addParameter(params[KNOBTRF] = new juce::AudioParameterFloat("treblefreq", "Treble Freq", {0.0f, 1.0f}, 0.5f));         params[KNOBTRF]->addListener(this);
    addParameter(params[KNOBTRG] = new juce::AudioParameterFloat("treble", "Treble", {0.0f, 1.0f}, 0.5f));                  params[KNOBTRG]->addListener(this);
    addParameter(params[KNOBTRR] = new juce::AudioParameterFloat("treblereso", "Treble Reso", {0.0f, 1.0f}, 0.5f));         params[KNOBTRR]->addListener(this);
    addParameter(params[KNOBHMF] = new juce::AudioParameterFloat("highmidfreq", "HighMid Freq", {0.0f, 1.0f}, 0.5f));       params[KNOBHMF]->addListener(this);
    addParameter(params[KNOBHMG] = new juce::AudioParameterFloat("highmid", "HighMid", {0.0f, 1.0f}, 0.5f));                params[KNOBHMG]->addListener(this);
    addParameter(params[KNOBHMR] = new juce::AudioParameterFloat("highmidreso", "HighMid Reso", {0.0f, 1.0f}, 0.5f));       params[KNOBHMR]->addListener(this);
    addParameter(params[KNOBLMF] = new juce::AudioParameterFloat("lowmidfreq", "LowMid Freq", {0.0f, 1.0f}, 0.5f));         params[KNOBLMF]->addListener(this);
    addParameter(params[KNOBLMG] = new juce::AudioParameterFloat("lowmid", "LowMid", {0.0f, 1.0f}, 0.5f));                  params[KNOBLMG]->addListener(this);
    addParameter(params[KNOBLMR] = new juce::AudioParameterFloat("lowmidreso", "LowMid Reso", {0.0f, 1.0f}, 0.5f));         params[KNOBLMR]->addListener(this);
    addParameter(params[KNOBBSF] = new juce::AudioParameterFloat("bassfreq", "Bass Freq", {0.0f, 1.0f}, 0.5f));             params[KNOBBSF]->addListener(this);
    addParameter(params[KNOBBSG] = new juce::AudioParameterFloat("bass", "Bass", {0.0f, 1.0f}, 0.5f));                      params[KNOBBSG]->addListener(this);
    addParameter(params[KNOBBSR] = new juce::AudioParameterFloat("bassreso", "Bass Reso", {0.0f, 1.0f}, 0.5f));             params[KNOBBSR]->addListener(this);
    addParameter(params[KNOBDSC] = new juce::AudioParameterFloat("topdb", "Top dB", {0.0f, 1.0f}, 0.5f));                   params[KNOBDSC]->addListener(this);
    addParameter(params[KNOBPAN] = new juce::AudioParameterFloat("pan", "Pan", {0.0f, 1.0f}, 0.5f));                        params[KNOBPAN]->addListener(this);
    addParameter(params[KNOBFAD] = new juce::AudioParameterFloat("fader", "Fader", {0.0f, 1.0f}, 0.5f));                    params[KNOBFAD]->addListener(this);
}

PluginProcessor::~PluginProcessor() {}
void PluginProcessor::parameterValueChanged(int parameterIndex, float newValue)
{
    AudioToUIMessage msg;
    msg.what = AudioToUIMessage::NEW_VALUE;
    msg.which = (PluginProcessor::Parameters)parameterIndex;
    msg.newValue = params[parameterIndex]->convertFrom0to1(newValue);
    audioToUI.push(msg);
}
void PluginProcessor::parameterGestureChanged(int parameterIndex, bool starting) {}
const juce::String PluginProcessor::getName() const {return JucePlugin_Name;}
bool PluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}
bool PluginProcessor::supportsDoublePrecisionProcessing() const
{
   #if JucePlugin_SupportsDoublePrecisionProcessing
    return true;
   #else
    return true;
    //note: I don't know whether that config option is set, so I'm hardcoding it
    //knowing I have enabled such support: keeping the boilerplate stuff tho
    //in case I can sort out where it's enabled as a flag
   #endif
}
bool PluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}
bool PluginProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}
double PluginProcessor::getTailLengthSeconds() const {return 0.0;}
int PluginProcessor::getNumPrograms() {return 1;}
int PluginProcessor::getCurrentProgram() {return 0;}
void PluginProcessor::setCurrentProgram (int index) {juce::ignoreUnused (index);}
const juce::String PluginProcessor::getProgramName (int index) {juce::ignoreUnused (index); return {};}
void PluginProcessor::changeProgramName (int index, const juce::String& newName) {juce::ignoreUnused (index, newName);}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {juce::ignoreUnused (sampleRate, samplesPerBlock);}
void PluginProcessor::releaseResources() {}
bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this metering code we only support stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

//==============================================================================

bool PluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor (*this);
}

//==============================================================================
void PluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    std::unique_ptr<juce::XmlElement> xml(new juce::XmlElement("consolexchannel")); //note! All ConsoleX plugins
    //think they're Channel, with regard to XML tags. This is a mistake: they're supposed to have unique element
    //names, but it's been out in the wild for a year, so they must continue to all seek the same element name.
    //Turns out the DAW will seek this in the specific instance so you don't have to also have unique XML name
    //for it to be able to find the one for that specific instance: it'll be looking at the right set of data,
    //because you can have the same plugin in the same DAW set to different settings.
    xml->setAttribute("streamingVersion", (int)8524);

    for (unsigned long i = 0; i < n_params; ++i)
    {
        juce::String nm = juce::String("awcxc_") + std::to_string(i);
        float val = 0.0f; if (i < n_params) val = *(params[i]);
        xml->setAttribute(nm, val);
    }
    if (pluginWidth < 8 || pluginWidth > 16386) pluginWidth = 900;
    xml->setAttribute(juce::String("awcxc_width"), pluginWidth);
    if (pluginHeight < 8 || pluginHeight > 16386) pluginHeight = 300;
    xml->setAttribute(juce::String("awcxc_height"), pluginHeight);
    copyXmlToBinary(*xml, destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName("consolexchannel"))
        {
            for (unsigned long i = 0; i < n_params; ++i)
            {
                juce::String nm = juce::String("awcxc_") + std::to_string(i);
                auto f = xmlState->getDoubleAttribute(nm);
                params[i]->setValueNotifyingHost((float)f);
            }
            auto w = xmlState->getIntAttribute(juce::String("awcxc_width"));
            if (w < 8 || w > 16386) w = 900;
            auto h = xmlState->getIntAttribute(juce::String("awcxc_height"));
            if (h < 8 || h > 16386) h = 300;
            updatePluginSize(w, h);
        }
        updateHostDisplay();
    }
    //These functions are adapted (simplified) from baconpaul's airwin2rack and all thanks to getting
    //it working shall go there, though sudara or anyone could've spotted that I hadn't done these.
    //baconpaul pointed me to the working versions in airwin2rack, that I needed to see.
}

void PluginProcessor::updateTrackProperties(const TrackProperties& properties)
{
    trackProperties = properties;
    // call the version in the editor to update there
    if (auto* editor = dynamic_cast<PluginEditor*> (getActiveEditor()))
        editor->updateTrackProperties();
}

void PluginProcessor::updatePluginSize(int w, int h)
{
    pluginWidth = w;
    pluginHeight = h;
    // call the version in the editor to update there
    if (auto* editor = dynamic_cast<PluginEditor*> (getActiveEditor()))
        editor->updatePluginSize();
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}


void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    if (!(getBus(false, 0)->isEnabled() && getBus(true, 0)->isEnabled())) return;
    auto mainOutput = getBusBuffer(buffer, false, 0); //if we have audio busses at all,
    auto mainInput = getBusBuffer(buffer, true, 0); //they're now mainOutput and mainInput.
    
    UIToAudioMessage uim;
    while (uiToAudio.pop(uim)) {
        switch (uim.what) {
        case UIToAudioMessage::NEW_VALUE: params[uim.which]->setValueNotifyingHost(params[uim.which]->convertTo0to1(uim.newValue)); break;
        case UIToAudioMessage::BEGIN_EDIT: params[uim.which]->beginChangeGesture(); break;
        case UIToAudioMessage::END_EDIT: params[uim.which]->endChangeGesture(); break;
        }
    } //Handle inbound messages from the UI thread

    double rmsSize = (1881.0 / 44100.0)*getSampleRate(); //higher is slower with larger RMS buffers
    double zeroCrossScale = (1.0 / getSampleRate())*44100.0;
    int inFramesToProcess = buffer.getNumSamples(); //vst doesn't give us this as a separate variable so we'll make it
    double overallscale = 1.0;
    overallscale /= 44100.0;
    overallscale *= getSampleRate();

    highpass[hilp_freq] = ((params[KNOBHIP]->get()*330.0)+20.0)/getSampleRate();
    bool highpassEngage = true; if (params[KNOBHIP]->get() == 0.0) highpassEngage = false;
    
    lowpass[hilp_freq] = ((pow(1.0-params[KNOBLOP]->get(),2)*17000.0)+3000.0)/getSampleRate();
    bool lowpassEngage = true; if (params[KNOBLOP]->get() == 0.0) lowpassEngage = false;
    
    double K = tan(M_PI * highpass[hilp_freq]); //highpass
    double norm = 1.0 / (1.0 + K / 1.93185165 + K * K);
    highpass[hilp_a0] = norm;
    highpass[hilp_a1] = -2.0 * highpass[hilp_a0];
    highpass[hilp_b1] = 2.0 * (K * K - 1.0) * norm;
    highpass[hilp_b2] = (1.0 - K / 1.93185165 + K * K) * norm;
    norm = 1.0 / (1.0 + K / 0.70710678 + K * K);
    highpass[hilp_c0] = norm;
    highpass[hilp_c1] = -2.0 * highpass[hilp_c0];
    highpass[hilp_d1] = 2.0 * (K * K - 1.0) * norm;
    highpass[hilp_d2] = (1.0 - K / 0.70710678 + K * K) * norm;
    norm = 1.0 / (1.0 + K / 0.51763809 + K * K);
    highpass[hilp_e0] = norm;
    highpass[hilp_e1] = -2.0 * highpass[hilp_e0];
    highpass[hilp_f1] = 2.0 * (K * K - 1.0) * norm;
    highpass[hilp_f2] = (1.0 - K / 0.51763809 + K * K) * norm;
    
    K = tan(M_PI * lowpass[hilp_freq]); //lowpass
    norm = 1.0 / (1.0 + K / 1.93185165 + K * K);
    lowpass[hilp_a0] = K * K * norm;
    lowpass[hilp_a1] = 2.0 * lowpass[hilp_a0];
    lowpass[hilp_b1] = 2.0 * (K * K - 1.0) * norm;
    lowpass[hilp_b2] = (1.0 - K / 1.93185165 + K * K) * norm;
    norm = 1.0 / (1.0 + K / 0.70710678 + K * K);
    lowpass[hilp_c0] = K * K * norm;
    lowpass[hilp_c1] = 2.0 * lowpass[hilp_c0];
    lowpass[hilp_d1] = 2.0 * (K * K - 1.0) * norm;
    lowpass[hilp_d2] = (1.0 - K / 0.70710678 + K * K) * norm;
    norm = 1.0 / (1.0 + K / 0.51763809 + K * K);
    lowpass[hilp_e0] = K * K * norm;
    lowpass[hilp_e1] = 2.0 * lowpass[hilp_e0];
    lowpass[hilp_f1] = 2.0 * (K * K - 1.0) * norm;
    lowpass[hilp_f2] = (1.0 - K / 0.51763809 + K * K) * norm;
    
    airGainA = airGainB; airGainB = params[KNOBAIR]->get() *2.0;
    fireGainA = fireGainB; fireGainB = params[KNOBFIR]->get() *2.0;
    stoneGainA = stoneGainB; stoneGainB = params[KNOBSTO]->get() *2.0;
    //simple three band to adjust
    double kalmanRange = 1.0-(pow(params[KNOBRNG]->get(),2)/overallscale);
    //crossover frequency between mid/bass
    
    double compFThresh = pow(params[KNOBFCT]->get(),4);
    double compSThresh = pow(params[KNOBSCT]->get(),4);
    double compFRatio = 1.0-pow(1.0-params[KNOBFCR]->get(),2);
    double compSRatio = 1.0-pow(1.0-params[KNOBSCR]->get(),2);
    double compFAttack = 1.0/(((pow(params[KNOBFCA]->get(),3)*5000.0)+500.0)*overallscale);
    double compSAttack = 1.0/(((pow(params[KNOBSCA]->get(),3)*5000.0)+500.0)*overallscale);
    double compFRelease = 1.0/(((pow(params[KNOBFCL]->get(),5)*50000.0)+500.0)*overallscale);
    double compSRelease = 1.0/(((pow(params[KNOBSCL]->get(),5)*50000.0)+500.0)*overallscale);
    double gateFThresh = pow(params[KNOBFGT]->get(),4);
    double gateSThresh = pow(params[KNOBSGT]->get(),4);
    double gateFRatio = 1.0-pow(1.0-params[KNOBFGR]->get(),2);
    double gateSRatio = 1.0-pow(1.0-params[KNOBSGR]->get(),2);
    double gateFSustain = M_PI_2 * pow(params[KNOBFGS]->get()+1.0,4.0);
    double gateSSustain = M_PI_2 * pow(params[KNOBSGS]->get()+1.0,4.0);
    double gateFRelease = 1.0/(((pow(params[KNOBFGL]->get(),5)*500000.0)+500.0)*overallscale);
    double gateSRelease = 1.0/(((pow(params[KNOBSGL]->get(),5)*500000.0)+500.0)*overallscale);
    
    high[biqs_freq] = (((pow(params[KNOBTRF]->get(),3)*14500.0)+1500.0)/getSampleRate());
    if (high[biqs_freq] < 0.0001) high[biqs_freq] = 0.0001;
    high[biqs_nonlin] = params[KNOBTRG]->get();
    high[biqs_level] = (high[biqs_nonlin]*2.0)-1.0;
    if (high[biqs_level] > 0.0) high[biqs_level] *= 2.0;
    high[biqs_reso] = ((0.5+(high[biqs_nonlin]*0.5)+sqrt(high[biqs_freq]))-(1.0-pow(1.0-params[KNOBTRR]->get(),2.0)))+0.5+(high[biqs_nonlin]*0.5);
    K = tan(M_PI * high[biqs_freq]);
    norm = 1.0 / (1.0 + K / (high[biqs_reso]*1.93185165) + K * K);
    high[biqs_a0] = K / (high[biqs_reso]*1.93185165) * norm;
    high[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
    high[biqs_b2] = (1.0 - K / (high[biqs_reso]*1.93185165) + K * K) * norm;
    norm = 1.0 / (1.0 + K / (high[biqs_reso]*0.70710678) + K * K);
    high[biqs_c0] = K / (high[biqs_reso]*0.70710678) * norm;
    high[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
    high[biqs_d2] = (1.0 - K / (high[biqs_reso]*0.70710678) + K * K) * norm;
    norm = 1.0 / (1.0 + K / (high[biqs_reso]*0.51763809) + K * K);
    high[biqs_e0] = K / (high[biqs_reso]*0.51763809) * norm;
    high[biqs_f1] = 2.0 * (K * K - 1.0) * norm;
    high[biqs_f2] = (1.0 - K / (high[biqs_reso]*0.51763809) + K * K) * norm;
    //high
    
    hmid[biqs_freq] = (((pow(params[KNOBHMF]->get(),3)*6400.0)+600.0)/getSampleRate());
    if (hmid[biqs_freq] < 0.0001) hmid[biqs_freq] = 0.0001;
    hmid[biqs_nonlin] = params[KNOBHMG]->get();
    hmid[biqs_level] = (hmid[biqs_nonlin]*2.0)-1.0;
    if (hmid[biqs_level] > 0.0) hmid[biqs_level] *= 2.0;
    hmid[biqs_reso] = ((0.5+(hmid[biqs_nonlin]*0.5)+sqrt(hmid[biqs_freq]))-(1.0-pow(1.0-params[KNOBHMR]->get(),2.0)))+0.5+(hmid[biqs_nonlin]*0.5);
    K = tan(M_PI * hmid[biqs_freq]);
    norm = 1.0 / (1.0 + K / (hmid[biqs_reso]*1.93185165) + K * K);
    hmid[biqs_a0] = K / (hmid[biqs_reso]*1.93185165) * norm;
    hmid[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
    hmid[biqs_b2] = (1.0 - K / (hmid[biqs_reso]*1.93185165) + K * K) * norm;
    norm = 1.0 / (1.0 + K / (hmid[biqs_reso]*0.70710678) + K * K);
    hmid[biqs_c0] = K / (hmid[biqs_reso]*0.70710678) * norm;
    hmid[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
    hmid[biqs_d2] = (1.0 - K / (hmid[biqs_reso]*0.70710678) + K * K) * norm;
    norm = 1.0 / (1.0 + K / (hmid[biqs_reso]*0.51763809) + K * K);
    hmid[biqs_e0] = K / (hmid[biqs_reso]*0.51763809) * norm;
    hmid[biqs_f1] = 2.0 * (K * K - 1.0) * norm;
    hmid[biqs_f2] = (1.0 - K / (hmid[biqs_reso]*0.51763809) + K * K) * norm;
    //hmid
    
    lmid[biqs_freq] = (((pow(params[KNOBLMF]->get(),3)*2200.0)+200.0)/getSampleRate());
    if (lmid[biqs_freq] < 0.0001) lmid[biqs_freq] = 0.0001;
    lmid[biqs_nonlin] = params[KNOBLMG]->get();
    lmid[biqs_level] = (lmid[biqs_nonlin]*2.0)-1.0;
    if (lmid[biqs_level] > 0.0) lmid[biqs_level] *= 2.0;
    lmid[biqs_reso] = ((0.5+(lmid[biqs_nonlin]*0.5)+sqrt(lmid[biqs_freq]))-(1.0-pow(1.0-params[KNOBLMR]->get(),2.0)))+0.5+(lmid[biqs_nonlin]*0.5);
    K = tan(M_PI * lmid[biqs_freq]);
    norm = 1.0 / (1.0 + K / (lmid[biqs_reso]*1.93185165) + K * K);
    lmid[biqs_a0] = K / (lmid[biqs_reso]*1.93185165) * norm;
    lmid[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
    lmid[biqs_b2] = (1.0 - K / (lmid[biqs_reso]*1.93185165) + K * K) * norm;
    norm = 1.0 / (1.0 + K / (lmid[biqs_reso]*0.70710678) + K * K);
    lmid[biqs_c0] = K / (lmid[biqs_reso]*0.70710678) * norm;
    lmid[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
    lmid[biqs_d2] = (1.0 - K / (lmid[biqs_reso]*0.70710678) + K * K) * norm;
    norm = 1.0 / (1.0 + K / (lmid[biqs_reso]*0.51763809) + K * K);
    lmid[biqs_e0] = K / (lmid[biqs_reso]*0.51763809) * norm;
    lmid[biqs_f1] = 2.0 * (K * K - 1.0) * norm;
    lmid[biqs_f2] = (1.0 - K / (lmid[biqs_reso]*0.51763809) + K * K) * norm;
    //lmid
    
    bass[biqs_freq] = (((pow(params[KNOBBSF]->get(),3)*570.0)+30.0)/getSampleRate());
    if (bass[biqs_freq] < 0.0001) bass[biqs_freq] = 0.0001;
    bass[biqs_nonlin] = params[KNOBBSG]->get();
    bass[biqs_level] = (bass[biqs_nonlin]*2.0)-1.0;
    if (bass[biqs_level] > 0.0) bass[biqs_level] *= 2.0;
    bass[biqs_reso] = ((0.5+(bass[biqs_nonlin]*0.5)+sqrt(bass[biqs_freq]))-(1.0-pow(1.0-params[KNOBBSR]->get(),2.0)))+0.5+(bass[biqs_nonlin]*0.5);
    K = tan(M_PI * bass[biqs_freq]);
    norm = 1.0 / (1.0 + K / (bass[biqs_reso]*1.93185165) + K * K);
    bass[biqs_a0] = K / (bass[biqs_reso]*1.93185165) * norm;
    bass[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
    bass[biqs_b2] = (1.0 - K / (bass[biqs_reso]*1.93185165) + K * K) * norm;
    norm = 1.0 / (1.0 + K / (bass[biqs_reso]*0.70710678) + K * K);
    bass[biqs_c0] = K / (bass[biqs_reso]*0.70710678) * norm;
    bass[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
    bass[biqs_d2] = (1.0 - K / (bass[biqs_reso]*0.70710678) + K * K) * norm;
    norm = 1.0 / (1.0 + K / (bass[biqs_reso]*0.51763809) + K * K);
    bass[biqs_e0] = K / (bass[biqs_reso]*0.51763809) * norm;
    bass[biqs_f1] = 2.0 * (K * K - 1.0) * norm;
    bass[biqs_f2] = (1.0 - K / (bass[biqs_reso]*0.51763809) + K * K) * norm;
    //bass
    
    double refdB = (params[KNOBDSC]->get()*70.0)+70.0;
    double topdB = 0.000000075 * pow(10.0,refdB/20.0) * overallscale;
    
    panA = panB; panB = params[KNOBPAN]->get()*1.57079633;
    inTrimA = inTrimB; inTrimB = params[KNOBFAD]->get()*2.0;
    
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        auto outL = mainOutput.getWritePointer(0, i);
        auto outR = mainOutput.getWritePointer(1, i);
        auto inL = mainInput.getReadPointer(0, i); //in isBussesLayoutSupported, we have already
        auto inR = mainInput.getReadPointer(1, i); //specified that we can only be stereo and never mono
        long double inputSampleL = *inL;
        long double inputSampleR = *inR;
        if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
        if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
        //NOTE: I don't yet know whether counting the buffer backwards means that gainA and gainB must be reversed.
        //If the audio flow is in fact reversed we must swap the temp and (1.0-temp) and I have done this provisionally
        //Original VST2 is counting DOWN with -- operator, but this counts UP with ++
        //If this doesn't create zipper noise on sine processing then it's correct
        
        if (highpassEngage) { //distributed Highpass
            highpass[hilp_temp] = (inputSampleL*highpass[hilp_a0])+highpass[hilp_aL1];
            highpass[hilp_aL1] = (inputSampleL*highpass[hilp_a1])-(highpass[hilp_temp]*highpass[hilp_b1])+highpass[hilp_aL2];
            highpass[hilp_aL2] = (inputSampleL*highpass[hilp_a0])-(highpass[hilp_temp]*highpass[hilp_b2]); inputSampleL = highpass[hilp_temp];
            highpass[hilp_temp] = (inputSampleR*highpass[hilp_a0])+highpass[hilp_aR1];
            highpass[hilp_aR1] = (inputSampleR*highpass[hilp_a1])-(highpass[hilp_temp]*highpass[hilp_b1])+highpass[hilp_aR2];
            highpass[hilp_aR2] = (inputSampleR*highpass[hilp_a0])-(highpass[hilp_temp]*highpass[hilp_b2]); inputSampleR = highpass[hilp_temp];
        } else highpass[hilp_aR1] = highpass[hilp_aR2] = highpass[hilp_aL1] = highpass[hilp_aL2] = 0.0;
        if (lowpassEngage) { //distributed Lowpass
            lowpass[hilp_temp] = (inputSampleL*lowpass[hilp_a0])+lowpass[hilp_aL1];
            lowpass[hilp_aL1] = (inputSampleL*lowpass[hilp_a1])-(lowpass[hilp_temp]*lowpass[hilp_b1])+lowpass[hilp_aL2];
            lowpass[hilp_aL2] = (inputSampleL*lowpass[hilp_a0])-(lowpass[hilp_temp]*lowpass[hilp_b2]); inputSampleL = lowpass[hilp_temp];
            lowpass[hilp_temp] = (inputSampleR*lowpass[hilp_a0])+lowpass[hilp_aR1];
            lowpass[hilp_aR1] = (inputSampleR*lowpass[hilp_a1])-(lowpass[hilp_temp]*lowpass[hilp_b1])+lowpass[hilp_aR2];
            lowpass[hilp_aR2] = (inputSampleR*lowpass[hilp_a0])-(lowpass[hilp_temp]*lowpass[hilp_b2]); inputSampleR = lowpass[hilp_temp];
        } else lowpass[hilp_aR1] = lowpass[hilp_aR2] = lowpass[hilp_aL1] = lowpass[hilp_aL2] = 0.0;
        //first Highpass/Lowpass blocks aliasing before the nonlinearity of ConsoleXBuss and Parametric
        
        //get all Parametric bands before any other processing is done
        //begin Stacked Biquad With Reversed Neutron Flow L
        high[biqs_outL] = inputSampleL * fabs(high[biqs_level]);
        high[biqs_dis] = fabs(high[biqs_a0] * (1.0+(high[biqs_outL]*high[biqs_nonlin])));
        if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
        high[biqs_temp] = (high[biqs_outL] * high[biqs_dis]) + high[biqs_aL1];
        high[biqs_aL1] = high[biqs_aL2] - (high[biqs_temp]*high[biqs_b1]);
        high[biqs_aL2] = (high[biqs_outL] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_b2]);
        high[biqs_outL] = high[biqs_temp];
        high[biqs_dis] = fabs(high[biqs_c0] * (1.0+(high[biqs_outL]*high[biqs_nonlin])));
        if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
        high[biqs_temp] = (high[biqs_outL] * high[biqs_dis]) + high[biqs_cL1];
        high[biqs_cL1] = high[biqs_cL2] - (high[biqs_temp]*high[biqs_d1]);
        high[biqs_cL2] = (high[biqs_outL] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_d2]);
        high[biqs_outL] = high[biqs_temp];
        high[biqs_dis] = fabs(high[biqs_e0] * (1.0+(high[biqs_outL]*high[biqs_nonlin])));
        if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
        high[biqs_temp] = (high[biqs_outL] * high[biqs_dis]) + high[biqs_eL1];
        high[biqs_eL1] = high[biqs_eL2] - (high[biqs_temp]*high[biqs_f1]);
        high[biqs_eL2] = (high[biqs_outL] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_f2]);
        high[biqs_outL] = high[biqs_temp]; high[biqs_outL] *= high[biqs_level];
        if (high[biqs_level] > 1.0) high[biqs_outL] *= high[biqs_level];
        //end Stacked Biquad With Reversed Neutron Flow L
        
        //begin Stacked Biquad With Reversed Neutron Flow L
        hmid[biqs_outL] = inputSampleL * fabs(hmid[biqs_level]);
        hmid[biqs_dis] = fabs(hmid[biqs_a0] * (1.0+(hmid[biqs_outL]*hmid[biqs_nonlin])));
        if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
        hmid[biqs_temp] = (hmid[biqs_outL] * hmid[biqs_dis]) + hmid[biqs_aL1];
        hmid[biqs_aL1] = hmid[biqs_aL2] - (hmid[biqs_temp]*hmid[biqs_b1]);
        hmid[biqs_aL2] = (hmid[biqs_outL] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_b2]);
        hmid[biqs_outL] = hmid[biqs_temp];
        hmid[biqs_dis] = fabs(hmid[biqs_c0] * (1.0+(hmid[biqs_outL]*hmid[biqs_nonlin])));
        if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
        hmid[biqs_temp] = (hmid[biqs_outL] * hmid[biqs_dis]) + hmid[biqs_cL1];
        hmid[biqs_cL1] = hmid[biqs_cL2] - (hmid[biqs_temp]*hmid[biqs_d1]);
        hmid[biqs_cL2] = (hmid[biqs_outL] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_d2]);
        hmid[biqs_outL] = hmid[biqs_temp];
        hmid[biqs_dis] = fabs(hmid[biqs_e0] * (1.0+(hmid[biqs_outL]*hmid[biqs_nonlin])));
        if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
        hmid[biqs_temp] = (hmid[biqs_outL] * hmid[biqs_dis]) + hmid[biqs_eL1];
        hmid[biqs_eL1] = hmid[biqs_eL2] - (hmid[biqs_temp]*hmid[biqs_f1]);
        hmid[biqs_eL2] = (hmid[biqs_outL] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_f2]);
        hmid[biqs_outL] = hmid[biqs_temp]; hmid[biqs_outL] *= hmid[biqs_level];
        if (hmid[biqs_level] > 1.0) hmid[biqs_outL] *= hmid[biqs_level];
        //end Stacked Biquad With Reversed Neutron Flow L
        
        //begin Stacked Biquad With Reversed Neutron Flow L
        lmid[biqs_outL] = inputSampleL * fabs(lmid[biqs_level]);
        lmid[biqs_dis] = fabs(lmid[biqs_a0] * (1.0+(lmid[biqs_outL]*lmid[biqs_nonlin])));
        if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
        lmid[biqs_temp] = (lmid[biqs_outL] * lmid[biqs_dis]) + lmid[biqs_aL1];
        lmid[biqs_aL1] = lmid[biqs_aL2] - (lmid[biqs_temp]*lmid[biqs_b1]);
        lmid[biqs_aL2] = (lmid[biqs_outL] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_b2]);
        lmid[biqs_outL] = lmid[biqs_temp];
        lmid[biqs_dis] = fabs(lmid[biqs_c0] * (1.0+(lmid[biqs_outL]*lmid[biqs_nonlin])));
        if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
        lmid[biqs_temp] = (lmid[biqs_outL] * lmid[biqs_dis]) + lmid[biqs_cL1];
        lmid[biqs_cL1] = lmid[biqs_cL2] - (lmid[biqs_temp]*lmid[biqs_d1]);
        lmid[biqs_cL2] = (lmid[biqs_outL] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_d2]);
        lmid[biqs_outL] = lmid[biqs_temp];
        lmid[biqs_dis] = fabs(lmid[biqs_e0] * (1.0+(lmid[biqs_outL]*lmid[biqs_nonlin])));
        if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
        lmid[biqs_temp] = (lmid[biqs_outL] * lmid[biqs_dis]) + lmid[biqs_eL1];
        lmid[biqs_eL1] = lmid[biqs_eL2] - (lmid[biqs_temp]*lmid[biqs_f1]);
        lmid[biqs_eL2] = (lmid[biqs_outL] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_f2]);
        lmid[biqs_outL] = lmid[biqs_temp]; lmid[biqs_outL] *= lmid[biqs_level];
        if (lmid[biqs_level] > 1.0) lmid[biqs_outL] *= lmid[biqs_level];
        //end Stacked Biquad With Reversed Neutron Flow L
        
        //begin Stacked Biquad With Reversed Neutron Flow L
        bass[biqs_outL] = inputSampleL * fabs(bass[biqs_level]);
        bass[biqs_dis] = fabs(bass[biqs_a0] * (1.0+(bass[biqs_outL]*bass[biqs_nonlin])));
        if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
        bass[biqs_temp] = (bass[biqs_outL] * bass[biqs_dis]) + bass[biqs_aL1];
        bass[biqs_aL1] = bass[biqs_aL2] - (bass[biqs_temp]*bass[biqs_b1]);
        bass[biqs_aL2] = (bass[biqs_outL] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_b2]);
        bass[biqs_outL] = bass[biqs_temp];
        bass[biqs_dis] = fabs(bass[biqs_c0] * (1.0+(bass[biqs_outL]*bass[biqs_nonlin])));
        if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
        bass[biqs_temp] = (bass[biqs_outL] * bass[biqs_dis]) + bass[biqs_cL1];
        bass[biqs_cL1] = bass[biqs_cL2] - (bass[biqs_temp]*bass[biqs_d1]);
        bass[biqs_cL2] = (bass[biqs_outL] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_d2]);
        bass[biqs_outL] = bass[biqs_temp];
        bass[biqs_dis] = fabs(bass[biqs_e0] * (1.0+(bass[biqs_outL]*bass[biqs_nonlin])));
        if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
        bass[biqs_temp] = (bass[biqs_outL] * bass[biqs_dis]) + bass[biqs_eL1];
        bass[biqs_eL1] = bass[biqs_eL2] - (bass[biqs_temp]*bass[biqs_f1]);
        bass[biqs_eL2] = (bass[biqs_outL] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_f2]);
        bass[biqs_outL] = bass[biqs_temp]; bass[biqs_outL] *= bass[biqs_level];
        if (bass[biqs_level] > 1.0) bass[biqs_outL] *= bass[biqs_level];
        //end Stacked Biquad With Reversed Neutron Flow L
        
        //begin Stacked Biquad With Reversed Neutron Flow R
        high[biqs_outR] = inputSampleR * fabs(high[biqs_level]);
        high[biqs_dis] = fabs(high[biqs_a0] * (1.0+(high[biqs_outR]*high[biqs_nonlin])));
        if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
        high[biqs_temp] = (high[biqs_outR] * high[biqs_dis]) + high[biqs_aR1];
        high[biqs_aR1] = high[biqs_aR2] - (high[biqs_temp]*high[biqs_b1]);
        high[biqs_aR2] = (high[biqs_outR] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_b2]);
        high[biqs_outR] = high[biqs_temp];
        high[biqs_dis] = fabs(high[biqs_c0] * (1.0+(high[biqs_outR]*high[biqs_nonlin])));
        if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
        high[biqs_temp] = (high[biqs_outR] * high[biqs_dis]) + high[biqs_cR1];
        high[biqs_cR1] = high[biqs_cR2] - (high[biqs_temp]*high[biqs_d1]);
        high[biqs_cR2] = (high[biqs_outR] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_d2]);
        high[biqs_outR] = high[biqs_temp];
        high[biqs_dis] = fabs(high[biqs_e0] * (1.0+(high[biqs_outR]*high[biqs_nonlin])));
        if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
        high[biqs_temp] = (high[biqs_outR] * high[biqs_dis]) + high[biqs_eR1];
        high[biqs_eR1] = high[biqs_eR2] - (high[biqs_temp]*high[biqs_f1]);
        high[biqs_eR2] = (high[biqs_outR] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_f2]);
        high[biqs_outR] = high[biqs_temp]; high[biqs_outR] *= high[biqs_level];
        if (high[biqs_level] > 1.0) high[biqs_outR] *= high[biqs_level];
        //end Stacked Biquad With Reversed Neutron Flow R
        
        //begin Stacked Biquad With Reversed Neutron Flow R
        hmid[biqs_outR] = inputSampleR * fabs(hmid[biqs_level]);
        hmid[biqs_dis] = fabs(hmid[biqs_a0] * (1.0+(hmid[biqs_outR]*hmid[biqs_nonlin])));
        if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
        hmid[biqs_temp] = (hmid[biqs_outR] * hmid[biqs_dis]) + hmid[biqs_aR1];
        hmid[biqs_aR1] = hmid[biqs_aR2] - (hmid[biqs_temp]*hmid[biqs_b1]);
        hmid[biqs_aR2] = (hmid[biqs_outR] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_b2]);
        hmid[biqs_outR] = hmid[biqs_temp];
        hmid[biqs_dis] = fabs(hmid[biqs_c0] * (1.0+(hmid[biqs_outR]*hmid[biqs_nonlin])));
        if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
        hmid[biqs_temp] = (hmid[biqs_outR] * hmid[biqs_dis]) + hmid[biqs_cR1];
        hmid[biqs_cR1] = hmid[biqs_cR2] - (hmid[biqs_temp]*hmid[biqs_d1]);
        hmid[biqs_cR2] = (hmid[biqs_outR] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_d2]);
        hmid[biqs_outR] = hmid[biqs_temp];
        hmid[biqs_dis] = fabs(hmid[biqs_e0] * (1.0+(hmid[biqs_outR]*hmid[biqs_nonlin])));
        if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
        hmid[biqs_temp] = (hmid[biqs_outR] * hmid[biqs_dis]) + hmid[biqs_eR1];
        hmid[biqs_eR1] = hmid[biqs_eR2] - (hmid[biqs_temp]*hmid[biqs_f1]);
        hmid[biqs_eR2] = (hmid[biqs_outR] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_f2]);
        hmid[biqs_outR] = hmid[biqs_temp]; hmid[biqs_outR] *= hmid[biqs_level];
        if (hmid[biqs_level] > 1.0) hmid[biqs_outR] *= hmid[biqs_level];
        //end Stacked Biquad With Reversed Neutron Flow R
        
        //begin Stacked Biquad With Reversed Neutron Flow R
        lmid[biqs_outR] = inputSampleR * fabs(lmid[biqs_level]);
        lmid[biqs_dis] = fabs(lmid[biqs_a0] * (1.0+(lmid[biqs_outR]*lmid[biqs_nonlin])));
        if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
        lmid[biqs_temp] = (lmid[biqs_outR] * lmid[biqs_dis]) + lmid[biqs_aR1];
        lmid[biqs_aR1] = lmid[biqs_aR2] - (lmid[biqs_temp]*lmid[biqs_b1]);
        lmid[biqs_aR2] = (lmid[biqs_outR] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_b2]);
        lmid[biqs_outR] = lmid[biqs_temp];
        lmid[biqs_dis] = fabs(lmid[biqs_c0] * (1.0+(lmid[biqs_outR]*lmid[biqs_nonlin])));
        if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
        lmid[biqs_temp] = (lmid[biqs_outR] * lmid[biqs_dis]) + lmid[biqs_cR1];
        lmid[biqs_cR1] = lmid[biqs_cR2] - (lmid[biqs_temp]*lmid[biqs_d1]);
        lmid[biqs_cR2] = (lmid[biqs_outR] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_d2]);
        lmid[biqs_outR] = lmid[biqs_temp];
        lmid[biqs_dis] = fabs(lmid[biqs_e0] * (1.0+(lmid[biqs_outR]*lmid[biqs_nonlin])));
        if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
        lmid[biqs_temp] = (lmid[biqs_outR] * lmid[biqs_dis]) + lmid[biqs_eR1];
        lmid[biqs_eR1] = lmid[biqs_eR2] - (lmid[biqs_temp]*lmid[biqs_f1]);
        lmid[biqs_eR2] = (lmid[biqs_outR] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_f2]);
        lmid[biqs_outR] = lmid[biqs_temp]; lmid[biqs_outR] *= lmid[biqs_level];
        if (lmid[biqs_level] > 1.0) lmid[biqs_outR] *= lmid[biqs_level];
        //end Stacked Biquad With Reversed Neutron Flow R
        
        //begin Stacked Biquad With Reversed Neutron Flow R
        bass[biqs_outR] = inputSampleR * fabs(bass[biqs_level]);
        bass[biqs_dis] = fabs(bass[biqs_a0] * (1.0+(bass[biqs_outR]*bass[biqs_nonlin])));
        if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
        bass[biqs_temp] = (bass[biqs_outR] * bass[biqs_dis]) + bass[biqs_aR1];
        bass[biqs_aR1] = bass[biqs_aR2] - (bass[biqs_temp]*bass[biqs_b1]);
        bass[biqs_aR2] = (bass[biqs_outR] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_b2]);
        bass[biqs_outR] = bass[biqs_temp];
        bass[biqs_dis] = fabs(bass[biqs_c0] * (1.0+(bass[biqs_outR]*bass[biqs_nonlin])));
        if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
        bass[biqs_temp] = (bass[biqs_outR] * bass[biqs_dis]) + bass[biqs_cR1];
        bass[biqs_cR1] = bass[biqs_cR2] - (bass[biqs_temp]*bass[biqs_d1]);
        bass[biqs_cR2] = (bass[biqs_outR] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_d2]);
        bass[biqs_outR] = bass[biqs_temp];
        bass[biqs_dis] = fabs(bass[biqs_e0] * (1.0+(bass[biqs_outR]*bass[biqs_nonlin])));
        if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
        bass[biqs_temp] = (bass[biqs_outR] * bass[biqs_dis]) + bass[biqs_eR1];
        bass[biqs_eR1] = bass[biqs_eR2] - (bass[biqs_temp]*bass[biqs_f1]);
        bass[biqs_eR2] = (bass[biqs_outR] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_f2]);
        bass[biqs_outR] = bass[biqs_temp]; bass[biqs_outR] *= bass[biqs_level];
        if (bass[biqs_level] > 1.0) bass[biqs_outR] *= bass[biqs_level];
        //end Stacked Biquad With Reversed Neutron Flow R
        
        double temp = (double)buffer.getNumSamples()/inFramesToProcess;
        double gainR = (panA*temp)+(panB*(1.0-temp));
        double gainL = 1.57079633-gainR;
        gainR = sin(gainR); gainL = sin(gainL);
        double gain = (inTrimA*temp)+(inTrimB*(1.0-temp));
        if (gain > 1.0) gain *= gain; else gain = 1.0-pow(1.0-gain,2);
        gain *= 0.763932022500211;
        double airGain = (airGainA*temp)+(airGainB*(1.0-temp));
        if (airGain > 1.0) airGain *= airGain; else airGain = 1.0-pow(1.0-airGain,2);
        double fireGain = (fireGainA*temp)+(fireGainB*(1.0-temp));
        if (fireGain > 1.0) fireGain *= fireGain; else fireGain = 1.0-pow(1.0-fireGain,2);
        double firePad = fireGain; if (firePad > 1.0) firePad = 1.0;
        double stoneGain = (stoneGainA*temp)+(stoneGainB*(1.0-temp));
        if (stoneGain > 1.0) stoneGain *= stoneGain; else stoneGain = 1.0-pow(1.0-stoneGain,2);
        double stonePad = stoneGain; if (stonePad > 1.0) stonePad = 1.0;
        //set up smoothed gain controls
        
        //begin Air3L
        double drySampleL = inputSampleL;
        air[pvSL4] = air[pvAL4] - air[pvAL3]; air[pvSL3] = air[pvAL3] - air[pvAL2];
        air[pvSL2] = air[pvAL2] - air[pvAL1]; air[pvSL1] = air[pvAL1] - inputSampleL;
        air[accSL3] = air[pvSL4] - air[pvSL3]; air[accSL2] = air[pvSL3] - air[pvSL2];
        air[accSL1] = air[pvSL2] - air[pvSL1];
        air[acc2SL2] = air[accSL3] - air[accSL2]; air[acc2SL1] = air[accSL2] - air[accSL1];
        air[outAL] = -(air[pvAL1] + air[pvSL3] + air[acc2SL2] - ((air[acc2SL2] + air[acc2SL1])*0.5));
        air[gainAL] *= 0.5; air[gainAL] += fabs(drySampleL-air[outAL])*0.5;
        if (air[gainAL] > 0.3*sqrt(overallscale)) air[gainAL] = 0.3*sqrt(overallscale);
        air[pvAL4] = air[pvAL3]; air[pvAL3] = air[pvAL2];
        air[pvAL2] = air[pvAL1]; air[pvAL1] = (air[gainAL] * air[outAL]) + drySampleL;
        double fireL = drySampleL - ((air[outAL]*0.5)+(drySampleL*(0.457-(0.017*overallscale))));
        temp = (fireL + air[gndavgL])*0.5; air[gndavgL] = fireL; fireL = temp;
        double airL = (drySampleL-fireL)*airGain;
        inputSampleL = fireL;
        //end Air3L
        //begin Air3R
        double drySampleR = inputSampleR;
        air[pvSR4] = air[pvAR4] - air[pvAR3]; air[pvSR3] = air[pvAR3] - air[pvAR2];
        air[pvSR2] = air[pvAR2] - air[pvAR1]; air[pvSR1] = air[pvAR1] - inputSampleR;
        air[accSR3] = air[pvSR4] - air[pvSR3]; air[accSR2] = air[pvSR3] - air[pvSR2];
        air[accSR1] = air[pvSR2] - air[pvSR1];
        air[acc2SR2] = air[accSR3] - air[accSR2]; air[acc2SR1] = air[accSR2] - air[accSR1];
        air[outAR] = -(air[pvAR1] + air[pvSR3] + air[acc2SR2] - ((air[acc2SR2] + air[acc2SR1])*0.5));
        air[gainAR] *= 0.5; air[gainAR] += fabs(drySampleR-air[outAR])*0.5;
        if (air[gainAR] > 0.3*sqrt(overallscale)) air[gainAR] = 0.3*sqrt(overallscale);
        air[pvAR4] = air[pvAR3]; air[pvAR3] = air[pvAR2];
        air[pvAR2] = air[pvAR1]; air[pvAR1] = (air[gainAR] * air[outAR]) + drySampleR;
        double fireR = drySampleR - ((air[outAR]*0.5)+(drySampleR*(0.457-(0.017*overallscale))));
        temp = (fireR + air[gndavgR])*0.5; air[gndavgR] = fireR; fireR = temp;
        double airR = (drySampleR-fireR)*airGain;
        inputSampleR = fireR;
        //end Air3R
        //begin KalmanL
        temp = inputSampleL = inputSampleL*(1.0-kalmanRange)*0.777;
        inputSampleL *= (1.0-kalmanRange);
        //set up gain levels to control the beast
        kal[prevSlewL3] += kal[prevSampL3] - kal[prevSampL2]; kal[prevSlewL3] *= 0.5;
        kal[prevSlewL2] += kal[prevSampL2] - kal[prevSampL1]; kal[prevSlewL2] *= 0.5;
        kal[prevSlewL1] += kal[prevSampL1] - inputSampleL; kal[prevSlewL1] *= 0.5;
        //make slews from each set of samples used
        kal[accSlewL2] += kal[prevSlewL3] - kal[prevSlewL2]; kal[accSlewL2] *= 0.5;
        kal[accSlewL1] += kal[prevSlewL2] - kal[prevSlewL1]; kal[accSlewL1] *= 0.5;
        //differences between slews: rate of change of rate of change
        kal[accSlewL3] += (kal[accSlewL2] - kal[accSlewL1]); kal[accSlewL3] *= 0.5;
        //entering the abyss, what even is this
        kal[kalOutL] += kal[prevSampL1] + kal[prevSlewL2] + kal[accSlewL3]; kal[kalOutL] *= 0.5;
        //resynthesizing predicted result (all iir smoothed)
        kal[kalGainL] += fabs(temp-kal[kalOutL])*kalmanRange*8.0; kal[kalGainL] *= 0.5;
        //madness takes its toll. Kalman Gain: how much dry to retain
        if (kal[kalGainL] > kalmanRange*0.5) kal[kalGainL] = kalmanRange*0.5;
        //attempts to avoid explosions
        kal[kalOutL] += (temp*(1.0-(0.68+(kalmanRange*0.157))));
        //this is for tuning a really complete cancellation up around Nyquist
        kal[prevSampL3] = kal[prevSampL2]; kal[prevSampL2] = kal[prevSampL1];
        kal[prevSampL1] = (kal[kalGainL] * kal[kalOutL]) + ((1.0-kal[kalGainL])*temp);
        //feed the chain of previous samples
        if (kal[prevSampL1] > 1.0) kal[prevSampL1] = 1.0; if (kal[prevSampL1] < -1.0) kal[prevSampL1] = -1.0;
        double stoneL = kal[kalOutL]*0.777;
        fireL -= stoneL;
        //end KalmanL
        //begin KalmanR
        temp = inputSampleR = inputSampleR*(1.0-kalmanRange)*0.777;
        inputSampleR *= (1.0-kalmanRange);
        //set up gain levels to control the beast
        kal[prevSlewR3] += kal[prevSampR3] - kal[prevSampR2]; kal[prevSlewR3] *= 0.5;
        kal[prevSlewR2] += kal[prevSampR2] - kal[prevSampR1]; kal[prevSlewR2] *= 0.5;
        kal[prevSlewR1] += kal[prevSampR1] - inputSampleR; kal[prevSlewR1] *= 0.5;
        //make slews from each set of samples used
        kal[accSlewR2] += kal[prevSlewR3] - kal[prevSlewR2]; kal[accSlewR2] *= 0.5;
        kal[accSlewR1] += kal[prevSlewR2] - kal[prevSlewR1]; kal[accSlewR1] *= 0.5;
        //differences between slews: rate of change of rate of change
        kal[accSlewR3] += (kal[accSlewR2] - kal[accSlewR1]); kal[accSlewR3] *= 0.5;
        //entering the abyss, what even is this
        kal[kalOutR] += kal[prevSampR1] + kal[prevSlewR2] + kal[accSlewR3]; kal[kalOutR] *= 0.5;
        //resynthesizing predicted result (all iir smoothed)
        kal[kalGainR] += fabs(temp-kal[kalOutR])*kalmanRange*8.0; kal[kalGainR] *= 0.5;
        //madness takes its toll. Kalman Gain: how much dry to retain
        if (kal[kalGainR] > kalmanRange*0.5) kal[kalGainR] = kalmanRange*0.5;
        //attempts to avoid explosions
        kal[kalOutR] += (temp*(1.0-(0.68+(kalmanRange*0.157))));
        //this is for tuning a really complete cancellation up around Nyquist
        kal[prevSampR3] = kal[prevSampR2]; kal[prevSampR2] = kal[prevSampR1];
        kal[prevSampR1] = (kal[kalGainR] * kal[kalOutR]) + ((1.0-kal[kalGainR])*temp);
        //feed the chain of previous samples
        if (kal[prevSampR1] > 1.0) kal[prevSampR1] = 1.0; if (kal[prevSampR1] < -1.0) kal[prevSampR1] = -1.0;
        double stoneR = kal[kalOutR]*0.777;
        fireR -= stoneR;
        //end KalmanR
        
        if (baseFire < fabs(fireR*64.0)) baseFire = fmin(fabs(fireR*64.0),1.0);
        if (baseFire < fabs(fireL*64.0)) baseFire = fmin(fabs(fireL*64.0),1.0);
        if (baseStone < fabs(stoneR*256.0)) baseStone = fmin(fabs(stoneR*256.0),1.0);
        if (baseStone < fabs(stoneL*256.0)) baseStone = fmin(fabs(stoneL*256.0),1.0);
        //blinkenlights are off when there's no audio

        //fire dynamics
        if (fabs(fireL) > compFThresh) { //compression L
            fireCompL -= (fireCompL * compFAttack);
            fireCompL += ((compFThresh / fabs(fireL))*compFAttack);
        } else fireCompL = (fireCompL*(1.0-compFRelease))+compFRelease;
        if (fabs(fireR) > compFThresh) { //compression R
            fireCompR -= (fireCompR * compFAttack);
            fireCompR += ((compFThresh / fabs(fireR))*compFAttack);
        } else fireCompR = (fireCompR*(1.0-compFRelease))+compFRelease;
        if (fireCompL > fireCompR) fireCompL -= (fireCompL * compFAttack);
        if (fireCompR > fireCompL) fireCompR -= (fireCompR * compFAttack);
        if (fabs(fireL) > gateFThresh) fireGate = gateFSustain;
        else if (fabs(fireR) > gateFThresh) fireGate = gateFSustain;
        else fireGate *= (1.0-gateFRelease);
        if (fireGate < 0.0) fireGate = 0.0;
        fireCompL = fmax(fmin(fireCompL,1.0),0.0);
        fireCompR = fmax(fmin(fireCompR,1.0),0.0);
        if (maxFireComp > fmin(fireCompL,fireCompR)) maxFireComp = fmin(fireCompL,fireCompR);
        fireL *= (((1.0-compFRatio)*firePad)+(fireCompL*compFRatio*fireGain));
        fireR *= (((1.0-compFRatio)*firePad)+(fireCompR*compFRatio*fireGain));
        if (fireGate < M_PI_2) {
            temp = ((1.0-gateFRatio)+(sin(fireGate)*gateFRatio));
            if (maxFireGate > temp*temp) maxFireGate = temp*temp;
            airL *= temp;
            airR *= temp;
            fireL *= temp;
            fireR *= temp;
            high[biqs_outL] *= temp;
            high[biqs_outR] *= temp;
            hmid[biqs_outL] *= temp; //if Fire gating, gate Air, high and hmid
            hmid[biqs_outR] *= temp; //note that we aren't compressing these
        }
        //stone dynamics
        if (fabs(stoneL) > compSThresh) { //compression L
            stoneCompL -= (stoneCompL * compSAttack);
            stoneCompL += ((compSThresh / fabs(stoneL))*compSAttack);
        } else stoneCompL = (stoneCompL*(1.0-compSRelease))+compSRelease;
        if (fabs(stoneR) > compSThresh) { //compression R
            stoneCompR -= (stoneCompR * compSAttack);
            stoneCompR += ((compSThresh / fabs(stoneR))*compSAttack);
        } else stoneCompR = (stoneCompR*(1.0-compSRelease))+compSRelease;
        if (stoneCompL > stoneCompR) stoneCompL -= (stoneCompL * compSAttack);
        if (stoneCompR > stoneCompL) stoneCompR -= (stoneCompR * compSAttack);
        if (fabs(stoneL) > gateSThresh) stoneGate = gateSSustain;
        else if (fabs(stoneR) > gateSThresh) stoneGate = gateSSustain;
        else stoneGate *= (1.0-gateSRelease);
        if (stoneGate < 0.0) stoneGate = 0.0;
        stoneCompL = fmax(fmin(stoneCompL,1.0),0.0);
        stoneCompR = fmax(fmin(stoneCompR,1.0),0.0);
        if (maxStoneComp > fmin(stoneCompL,stoneCompR)) maxStoneComp = fmin(stoneCompL,stoneCompR);
        stoneL *= (((1.0-compSRatio)*stonePad)+(stoneCompL*compSRatio*stoneGain));
        stoneR *= (((1.0-compSRatio)*stonePad)+(stoneCompR*compSRatio*stoneGain));
        if (stoneGate < M_PI_2) {
            temp = ((1.0-gateSRatio)+(sin(stoneGate)*gateSRatio));
            if (maxStoneGate > temp*temp) maxStoneGate = temp*temp;
            stoneL *= temp;
            stoneR *= temp;
            lmid[biqs_outL] *= temp;
            lmid[biqs_outR] *= temp;
            bass[biqs_outL] *= temp; //if Stone gating, gate lmid and bass
            bass[biqs_outR] *= temp; //note that we aren't compressing these
        }
        inputSampleL = stoneL + fireL + airL;
        inputSampleR = stoneR + fireR + airR;
        //create Stonefire output
        
        if (highpassEngage) { //distributed Highpass
            highpass[hilp_temp] = (inputSampleL*highpass[hilp_c0])+highpass[hilp_cL1];
            highpass[hilp_cL1] = (inputSampleL*highpass[hilp_c1])-(highpass[hilp_temp]*highpass[hilp_d1])+highpass[hilp_cL2];
            highpass[hilp_cL2] = (inputSampleL*highpass[hilp_c0])-(highpass[hilp_temp]*highpass[hilp_d2]); inputSampleL = highpass[hilp_temp];
            highpass[hilp_temp] = (inputSampleR*highpass[hilp_c0])+highpass[hilp_cR1];
            highpass[hilp_cR1] = (inputSampleR*highpass[hilp_c1])-(highpass[hilp_temp]*highpass[hilp_d1])+highpass[hilp_cR2];
            highpass[hilp_cR2] = (inputSampleR*highpass[hilp_c0])-(highpass[hilp_temp]*highpass[hilp_d2]); inputSampleR = highpass[hilp_temp];
        } else highpass[hilp_cR1] = highpass[hilp_cR2] = highpass[hilp_cL1] = highpass[hilp_cL2] = 0.0;
        if (lowpassEngage) { //distributed Lowpass
            lowpass[hilp_temp] = (inputSampleL*lowpass[hilp_c0])+lowpass[hilp_cL1];
            lowpass[hilp_cL1] = (inputSampleL*lowpass[hilp_c1])-(lowpass[hilp_temp]*lowpass[hilp_d1])+lowpass[hilp_cL2];
            lowpass[hilp_cL2] = (inputSampleL*lowpass[hilp_c0])-(lowpass[hilp_temp]*lowpass[hilp_d2]); inputSampleL = lowpass[hilp_temp];
            lowpass[hilp_temp] = (inputSampleR*lowpass[hilp_c0])+lowpass[hilp_cR1];
            lowpass[hilp_cR1] = (inputSampleR*lowpass[hilp_c1])-(lowpass[hilp_temp]*lowpass[hilp_d1])+lowpass[hilp_cR2];
            lowpass[hilp_cR2] = (inputSampleR*lowpass[hilp_c0])-(lowpass[hilp_temp]*lowpass[hilp_d2]); inputSampleR = lowpass[hilp_temp];
        } else lowpass[hilp_cR1] = lowpass[hilp_cR2] = lowpass[hilp_cL1] = lowpass[hilp_cL2] = 0.0;
        //another stage of Highpass/Lowpass before bringing in the parametric bands
        
        inputSampleL += (high[biqs_outL] + hmid[biqs_outL] + lmid[biqs_outL] + bass[biqs_outL]);
        inputSampleR += (high[biqs_outR] + hmid[biqs_outR] + lmid[biqs_outR] + bass[biqs_outR]);
        //add parametric boosts or cuts: clean as possible for maximal rawness and sonority
        
        inputSampleL = inputSampleL * gainL * gain;
        inputSampleR = inputSampleR * gainR * gain;
        //applies pan section, and smoothed fader gain
        
        inputSampleL *= topdB;
        if (inputSampleL < -0.222) inputSampleL = -0.222; if (inputSampleL > 0.222) inputSampleL = 0.222;
        dBaL[dBaXL] = inputSampleL; dBaPosL *= 0.5; dBaPosL += fabs((inputSampleL*((inputSampleL*0.25)-0.5))*0.5);
        int dBdly = (int)floor(dBaPosL*dscBuf);
        double dBi = (dBaPosL*dscBuf)-dBdly;
        inputSampleL = dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
        dBdly++; inputSampleL += dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*dBi;
        dBaXL++; if (dBaXL < 0 || dBaXL >= dscBuf) dBaXL = 0;
        inputSampleL /= topdB;
        inputSampleR *= topdB;
        if (inputSampleR < -0.222) inputSampleR = -0.222; if (inputSampleR > 0.222) inputSampleR = 0.222;
        dBaR[dBaXR] = inputSampleR; dBaPosR *= 0.5; dBaPosR += fabs((inputSampleR*((inputSampleR*0.25)-0.5))*0.5);
        dBdly = (int)floor(dBaPosR*dscBuf);
        dBi = (dBaPosR*dscBuf)-dBdly;
        inputSampleR = dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
        dBdly++; inputSampleR += dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*dBi;
        dBaXR++; if (dBaXR < 0 || dBaXR >= dscBuf) dBaXR = 0;
        inputSampleR /= topdB;
        //top dB processing for distributed discontinuity modeling air nonlinearity
        
        //ConsoleXChannel before final Highpass/Lowpass stages
        inputSampleL *= 0.618033988749895;
        if (inputSampleL > 1.0) inputSampleL = 1.0;
        else if (inputSampleL > 0.0) inputSampleL = -expm1((log1p(-inputSampleL) * 1.618033988749895));
        if (inputSampleL < -1.0) inputSampleL = -1.0;
        else if (inputSampleL < 0.0) inputSampleL = expm1((log1p(inputSampleL) * 1.618033988749895));
        inputSampleR *= 0.618033988749895;
        if (inputSampleR > 1.0) inputSampleR = 1.0;
        else if (inputSampleR > 0.0) inputSampleR = -expm1((log1p(-inputSampleR) * 1.618033988749895));
        if (inputSampleR < -1.0) inputSampleR = -1.0;
        else if (inputSampleR < 0.0) inputSampleR = expm1((log1p(inputSampleR) * 1.618033988749895));
        //ConsoleXChannel before final Highpass/Lowpass stages
                
        if (highpassEngage) { //distributed Highpass
            highpass[hilp_temp] = (inputSampleL*highpass[hilp_e0])+highpass[hilp_eL1];
            highpass[hilp_eL1] = (inputSampleL*highpass[hilp_e1])-(highpass[hilp_temp]*highpass[hilp_f1])+highpass[hilp_eL2];
            highpass[hilp_eL2] = (inputSampleL*highpass[hilp_e0])-(highpass[hilp_temp]*highpass[hilp_f2]); inputSampleL = highpass[hilp_temp];
            highpass[hilp_temp] = (inputSampleR*highpass[hilp_e0])+highpass[hilp_eR1];
            highpass[hilp_eR1] = (inputSampleR*highpass[hilp_e1])-(highpass[hilp_temp]*highpass[hilp_f1])+highpass[hilp_eR2];
            highpass[hilp_eR2] = (inputSampleR*highpass[hilp_e0])-(highpass[hilp_temp]*highpass[hilp_f2]); inputSampleR = highpass[hilp_temp];
        } else highpass[hilp_eR1] = highpass[hilp_eR2] = highpass[hilp_eL1] = highpass[hilp_eL2] = 0.0;
        if (lowpassEngage) { //distributed Lowpass
            lowpass[hilp_temp] = (inputSampleL*lowpass[hilp_e0])+lowpass[hilp_eL1];
            lowpass[hilp_eL1] = (inputSampleL*lowpass[hilp_e1])-(lowpass[hilp_temp]*lowpass[hilp_f1])+lowpass[hilp_eL2];
            lowpass[hilp_eL2] = (inputSampleL*lowpass[hilp_e0])-(lowpass[hilp_temp]*lowpass[hilp_f2]); inputSampleL = lowpass[hilp_temp];
            lowpass[hilp_temp] = (inputSampleR*lowpass[hilp_e0])+lowpass[hilp_eR1];
            lowpass[hilp_eR1] = (inputSampleR*lowpass[hilp_e1])-(lowpass[hilp_temp]*lowpass[hilp_f1])+lowpass[hilp_eR2];
            lowpass[hilp_eR2] = (inputSampleR*lowpass[hilp_e0])-(lowpass[hilp_temp]*lowpass[hilp_f2]); inputSampleR = lowpass[hilp_temp];
        } else lowpass[hilp_eR1] = lowpass[hilp_eR2] = lowpass[hilp_eL1] = lowpass[hilp_eL2] = 0.0;
        //final Highpass/Lowpass continues to address aliasing
        //final stacked biquad section is the softest Q for smoothness

        //begin bar display section
        if ((fabs(inputSampleL-previousLeft)/28000.0f)*getSampleRate() > slewLeft) slewLeft =  (fabs(inputSampleL-previousLeft)/28000.0f)*getSampleRate();
        if ((fabs(inputSampleR-previousRight)/28000.0f)*getSampleRate() > slewRight) slewRight = (fabs(inputSampleR-previousRight)/28000.0f)*getSampleRate();
        previousLeft = inputSampleL; previousRight = inputSampleR; //slew measurement is NOT rectified
        double rectifiedL = fabs(inputSampleL);
        double rectifiedR = fabs(inputSampleR);
        if (rectifiedL > peakLeft) peakLeft = rectifiedL;
        if (rectifiedR > peakRight) peakRight = rectifiedR;
        rmsLeft += (rectifiedL * rectifiedL);
        rmsRight += (rectifiedR * rectifiedR);
        rmsCount++;
        zeroLeft += zeroCrossScale;
        if (longestZeroLeft < zeroLeft) longestZeroLeft = zeroLeft;
        if (wasPositiveL && inputSampleL < 0.0) {
            wasPositiveL = false;
            zeroLeft = 0.0;
        } else if (!wasPositiveL && inputSampleL > 0.0) {
            wasPositiveL = true;
            zeroLeft = 0.0;
        }
        zeroRight += zeroCrossScale;
        if (longestZeroRight < zeroRight) longestZeroRight = zeroRight;
        if (wasPositiveR && inputSampleR < 0.0) {
            wasPositiveR = false;
            zeroRight = 0.0;
        } else if (!wasPositiveR && inputSampleR > 0.0) {
            wasPositiveR = true;
            zeroRight = 0.0;
        } //end bar display section

        //begin 32 bit stereo floating point dither
        int expon; frexpf((float)inputSampleL, &expon);
        fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
        inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
        frexpf((float)inputSampleR, &expon);
        fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
        inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
        //end 32 bit stereo floating point dither
        
        *outL = inputSampleL;
        *outR = inputSampleR;
    }


    if (rmsCount > rmsSize)
    {
        AudioToUIMessage msg; //define the thing we're telling JUCE
        msg.what = AudioToUIMessage::SLEW_LEFT; msg.newValue = (float)slewLeft; audioToUI.push(msg);
        msg.what = AudioToUIMessage::SLEW_RIGHT; msg.newValue = (float)slewRight; audioToUI.push(msg);
        msg.what = AudioToUIMessage::PEAK_LEFT; msg.newValue = (float)sqrt(peakLeft); audioToUI.push(msg);
        msg.what = AudioToUIMessage::PEAK_RIGHT; msg.newValue = (float)sqrt(peakRight); audioToUI.push(msg);
        msg.what = AudioToUIMessage::RMS_LEFT; msg.newValue = (float)sqrt(sqrt(rmsLeft/rmsCount)); audioToUI.push(msg);
        msg.what = AudioToUIMessage::RMS_RIGHT; msg.newValue = (float)sqrt(sqrt(rmsRight/rmsCount)); audioToUI.push(msg);
        msg.what = AudioToUIMessage::ZERO_LEFT; msg.newValue = (float)longestZeroLeft; audioToUI.push(msg);
        msg.what = AudioToUIMessage::ZERO_RIGHT; msg.newValue = (float)longestZeroRight; audioToUI.push(msg);
        msg.what = AudioToUIMessage::BLINKEN_FIRE_COMP; msg.newValue = (float)maxFireComp; audioToUI.push(msg);
        msg.what = AudioToUIMessage::BLINKEN_STONE_COMP; msg.newValue = (float)maxStoneComp; audioToUI.push(msg);
        msg.what = AudioToUIMessage::BLINKEN_FIRE_GATE; msg.newValue = (float)maxFireGate; audioToUI.push(msg);
        msg.what = AudioToUIMessage::BLINKEN_STONE_GATE; msg.newValue = (float)maxStoneGate; audioToUI.push(msg);
        msg.what = AudioToUIMessage::INCREMENT; msg.newValue = 0.0f; audioToUI.push(msg);
        slewLeft = 0.0;
        slewRight = 0.0;
        peakLeft = 0.0;
        peakRight = 0.0;
        rmsLeft = 0.0;
        rmsRight = 0.0;
        zeroLeft = 0.0;
        zeroRight = 0.0;
        longestZeroLeft = 0.0;
        longestZeroRight = 0.0;
        maxFireComp = baseFire;
        maxStoneComp = baseStone;
        maxFireGate = baseFire;
        maxStoneGate = baseStone;
        baseFire = 0.0;
        baseStone = 0.0;
        rmsCount = 0;
    }
}

//==============================================================================

void PluginProcessor::processBlock (juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    if (!(getBus(false, 0)->isEnabled() && getBus(true, 0)->isEnabled())) return;
    auto mainOutput = getBusBuffer(buffer, false, 0); //if we have audio busses at all,
    auto mainInput = getBusBuffer(buffer, true, 0); //they're now mainOutput and mainInput.
    UIToAudioMessage uim;
    while (uiToAudio.pop(uim)) {
        switch (uim.what) {
        case UIToAudioMessage::NEW_VALUE: params[uim.which]->setValueNotifyingHost(params[uim.which]->convertTo0to1(uim.newValue)); break;
        case UIToAudioMessage::BEGIN_EDIT: params[uim.which]->beginChangeGesture(); break;
        case UIToAudioMessage::END_EDIT: params[uim.which]->endChangeGesture(); break;
        }
    } //Handle inbound messages from the UI thread
    
    double rmsSize = (1881.0 / 44100.0)*getSampleRate(); //higher is slower with larger RMS buffers
    double zeroCrossScale = (1.0 / getSampleRate())*44100.0;
    int inFramesToProcess = buffer.getNumSamples(); //vst doesn't give us this as a separate variable so we'll make it
    double overallscale = 1.0;
    overallscale /= 44100.0;
    overallscale *= getSampleRate();
    
    highpass[hilp_freq] = ((params[KNOBHIP]->get()*330.0)+20.0)/getSampleRate();
    bool highpassEngage = true; if (params[KNOBHIP]->get() == 0.0) highpassEngage = false;
    
    lowpass[hilp_freq] = ((pow(1.0-params[KNOBLOP]->get(),2)*17000.0)+3000.0)/getSampleRate();
    bool lowpassEngage = true; if (params[KNOBLOP]->get() == 0.0) lowpassEngage = false;
    
    double K = tan(M_PI * highpass[hilp_freq]); //highpass
    double norm = 1.0 / (1.0 + K / 1.93185165 + K * K);
    highpass[hilp_a0] = norm;
    highpass[hilp_a1] = -2.0 * highpass[hilp_a0];
    highpass[hilp_b1] = 2.0 * (K * K - 1.0) * norm;
    highpass[hilp_b2] = (1.0 - K / 1.93185165 + K * K) * norm;
    norm = 1.0 / (1.0 + K / 0.70710678 + K * K);
    highpass[hilp_c0] = norm;
    highpass[hilp_c1] = -2.0 * highpass[hilp_c0];
    highpass[hilp_d1] = 2.0 * (K * K - 1.0) * norm;
    highpass[hilp_d2] = (1.0 - K / 0.70710678 + K * K) * norm;
    norm = 1.0 / (1.0 + K / 0.51763809 + K * K);
    highpass[hilp_e0] = norm;
    highpass[hilp_e1] = -2.0 * highpass[hilp_e0];
    highpass[hilp_f1] = 2.0 * (K * K - 1.0) * norm;
    highpass[hilp_f2] = (1.0 - K / 0.51763809 + K * K) * norm;
    
    K = tan(M_PI * lowpass[hilp_freq]); //lowpass
    norm = 1.0 / (1.0 + K / 1.93185165 + K * K);
    lowpass[hilp_a0] = K * K * norm;
    lowpass[hilp_a1] = 2.0 * lowpass[hilp_a0];
    lowpass[hilp_b1] = 2.0 * (K * K - 1.0) * norm;
    lowpass[hilp_b2] = (1.0 - K / 1.93185165 + K * K) * norm;
    norm = 1.0 / (1.0 + K / 0.70710678 + K * K);
    lowpass[hilp_c0] = K * K * norm;
    lowpass[hilp_c1] = 2.0 * lowpass[hilp_c0];
    lowpass[hilp_d1] = 2.0 * (K * K - 1.0) * norm;
    lowpass[hilp_d2] = (1.0 - K / 0.70710678 + K * K) * norm;
    norm = 1.0 / (1.0 + K / 0.51763809 + K * K);
    lowpass[hilp_e0] = K * K * norm;
    lowpass[hilp_e1] = 2.0 * lowpass[hilp_e0];
    lowpass[hilp_f1] = 2.0 * (K * K - 1.0) * norm;
    lowpass[hilp_f2] = (1.0 - K / 0.51763809 + K * K) * norm;
    
    airGainA = airGainB; airGainB = params[KNOBAIR]->get() *2.0;
    fireGainA = fireGainB; fireGainB = params[KNOBFIR]->get() *2.0;
    stoneGainA = stoneGainB; stoneGainB = params[KNOBSTO]->get() *2.0;
    //simple three band to adjust
    double kalmanRange = 1.0-(pow(params[KNOBRNG]->get(),2)/overallscale);
    //crossover frequency between mid/bass
    
    double compFThresh = pow(params[KNOBFCT]->get(),4);
    double compSThresh = pow(params[KNOBSCT]->get(),4);
    double compFRatio = 1.0-pow(1.0-params[KNOBFCR]->get(),2);
    double compSRatio = 1.0-pow(1.0-params[KNOBSCR]->get(),2);
    double compFAttack = 1.0/(((pow(params[KNOBFCA]->get(),3)*5000.0)+500.0)*overallscale);
    double compSAttack = 1.0/(((pow(params[KNOBSCA]->get(),3)*5000.0)+500.0)*overallscale);
    double compFRelease = 1.0/(((pow(params[KNOBFCL]->get(),5)*50000.0)+500.0)*overallscale);
    double compSRelease = 1.0/(((pow(params[KNOBSCL]->get(),5)*50000.0)+500.0)*overallscale);
    double gateFThresh = pow(params[KNOBFGT]->get(),4);
    double gateSThresh = pow(params[KNOBSGT]->get(),4);
    double gateFRatio = 1.0-pow(1.0-params[KNOBFGR]->get(),2);
    double gateSRatio = 1.0-pow(1.0-params[KNOBSGR]->get(),2);
    double gateFSustain = M_PI_2 * pow(params[KNOBFGS]->get()+1.0,4.0);
    double gateSSustain = M_PI_2 * pow(params[KNOBSGS]->get()+1.0,4.0);
    double gateFRelease = 1.0/(((pow(params[KNOBFGL]->get(),5)*500000.0)+500.0)*overallscale);
    double gateSRelease = 1.0/(((pow(params[KNOBSGL]->get(),5)*500000.0)+500.0)*overallscale);
    
    high[biqs_freq] = (((pow(params[KNOBTRF]->get(),3)*14500.0)+1500.0)/getSampleRate());
    if (high[biqs_freq] < 0.0001) high[biqs_freq] = 0.0001;
    high[biqs_nonlin] = params[KNOBTRG]->get();
    high[biqs_level] = (high[biqs_nonlin]*2.0)-1.0;
    if (high[biqs_level] > 0.0) high[biqs_level] *= 2.0;
    high[biqs_reso] = ((0.5+(high[biqs_nonlin]*0.5)+sqrt(high[biqs_freq]))-(1.0-pow(1.0-params[KNOBTRR]->get(),2.0)))+0.5+(high[biqs_nonlin]*0.5);
    K = tan(M_PI * high[biqs_freq]);
    norm = 1.0 / (1.0 + K / (high[biqs_reso]*1.93185165) + K * K);
    high[biqs_a0] = K / (high[biqs_reso]*1.93185165) * norm;
    high[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
    high[biqs_b2] = (1.0 - K / (high[biqs_reso]*1.93185165) + K * K) * norm;
    norm = 1.0 / (1.0 + K / (high[biqs_reso]*0.70710678) + K * K);
    high[biqs_c0] = K / (high[biqs_reso]*0.70710678) * norm;
    high[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
    high[biqs_d2] = (1.0 - K / (high[biqs_reso]*0.70710678) + K * K) * norm;
    norm = 1.0 / (1.0 + K / (high[biqs_reso]*0.51763809) + K * K);
    high[biqs_e0] = K / (high[biqs_reso]*0.51763809) * norm;
    high[biqs_f1] = 2.0 * (K * K - 1.0) * norm;
    high[biqs_f2] = (1.0 - K / (high[biqs_reso]*0.51763809) + K * K) * norm;
    //high
    
    hmid[biqs_freq] = (((pow(params[KNOBHMF]->get(),3)*6400.0)+600.0)/getSampleRate());
    if (hmid[biqs_freq] < 0.0001) hmid[biqs_freq] = 0.0001;
    hmid[biqs_nonlin] = params[KNOBHMG]->get();
    hmid[biqs_level] = (hmid[biqs_nonlin]*2.0)-1.0;
    if (hmid[biqs_level] > 0.0) hmid[biqs_level] *= 2.0;
    hmid[biqs_reso] = ((0.5+(hmid[biqs_nonlin]*0.5)+sqrt(hmid[biqs_freq]))-(1.0-pow(1.0-params[KNOBHMR]->get(),2.0)))+0.5+(hmid[biqs_nonlin]*0.5);
    K = tan(M_PI * hmid[biqs_freq]);
    norm = 1.0 / (1.0 + K / (hmid[biqs_reso]*1.93185165) + K * K);
    hmid[biqs_a0] = K / (hmid[biqs_reso]*1.93185165) * norm;
    hmid[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
    hmid[biqs_b2] = (1.0 - K / (hmid[biqs_reso]*1.93185165) + K * K) * norm;
    norm = 1.0 / (1.0 + K / (hmid[biqs_reso]*0.70710678) + K * K);
    hmid[biqs_c0] = K / (hmid[biqs_reso]*0.70710678) * norm;
    hmid[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
    hmid[biqs_d2] = (1.0 - K / (hmid[biqs_reso]*0.70710678) + K * K) * norm;
    norm = 1.0 / (1.0 + K / (hmid[biqs_reso]*0.51763809) + K * K);
    hmid[biqs_e0] = K / (hmid[biqs_reso]*0.51763809) * norm;
    hmid[biqs_f1] = 2.0 * (K * K - 1.0) * norm;
    hmid[biqs_f2] = (1.0 - K / (hmid[biqs_reso]*0.51763809) + K * K) * norm;
    //hmid
    
    lmid[biqs_freq] = (((pow(params[KNOBLMF]->get(),3)*2200.0)+200.0)/getSampleRate());
    if (lmid[biqs_freq] < 0.0001) lmid[biqs_freq] = 0.0001;
    lmid[biqs_nonlin] = params[KNOBLMG]->get();
    lmid[biqs_level] = (lmid[biqs_nonlin]*2.0)-1.0;
    if (lmid[biqs_level] > 0.0) lmid[biqs_level] *= 2.0;
    lmid[biqs_reso] = ((0.5+(lmid[biqs_nonlin]*0.5)+sqrt(lmid[biqs_freq]))-(1.0-pow(1.0-params[KNOBLMR]->get(),2.0)))+0.5+(lmid[biqs_nonlin]*0.5);
    K = tan(M_PI * lmid[biqs_freq]);
    norm = 1.0 / (1.0 + K / (lmid[biqs_reso]*1.93185165) + K * K);
    lmid[biqs_a0] = K / (lmid[biqs_reso]*1.93185165) * norm;
    lmid[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
    lmid[biqs_b2] = (1.0 - K / (lmid[biqs_reso]*1.93185165) + K * K) * norm;
    norm = 1.0 / (1.0 + K / (lmid[biqs_reso]*0.70710678) + K * K);
    lmid[biqs_c0] = K / (lmid[biqs_reso]*0.70710678) * norm;
    lmid[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
    lmid[biqs_d2] = (1.0 - K / (lmid[biqs_reso]*0.70710678) + K * K) * norm;
    norm = 1.0 / (1.0 + K / (lmid[biqs_reso]*0.51763809) + K * K);
    lmid[biqs_e0] = K / (lmid[biqs_reso]*0.51763809) * norm;
    lmid[biqs_f1] = 2.0 * (K * K - 1.0) * norm;
    lmid[biqs_f2] = (1.0 - K / (lmid[biqs_reso]*0.51763809) + K * K) * norm;
    //lmid
    
    bass[biqs_freq] = (((pow(params[KNOBBSF]->get(),3)*570.0)+30.0)/getSampleRate());
    if (bass[biqs_freq] < 0.0001) bass[biqs_freq] = 0.0001;
    bass[biqs_nonlin] = params[KNOBBSG]->get();
    bass[biqs_level] = (bass[biqs_nonlin]*2.0)-1.0;
    if (bass[biqs_level] > 0.0) bass[biqs_level] *= 2.0;
    bass[biqs_reso] = ((0.5+(bass[biqs_nonlin]*0.5)+sqrt(bass[biqs_freq]))-(1.0-pow(1.0-params[KNOBBSR]->get(),2.0)))+0.5+(bass[biqs_nonlin]*0.5);
    K = tan(M_PI * bass[biqs_freq]);
    norm = 1.0 / (1.0 + K / (bass[biqs_reso]*1.93185165) + K * K);
    bass[biqs_a0] = K / (bass[biqs_reso]*1.93185165) * norm;
    bass[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
    bass[biqs_b2] = (1.0 - K / (bass[biqs_reso]*1.93185165) + K * K) * norm;
    norm = 1.0 / (1.0 + K / (bass[biqs_reso]*0.70710678) + K * K);
    bass[biqs_c0] = K / (bass[biqs_reso]*0.70710678) * norm;
    bass[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
    bass[biqs_d2] = (1.0 - K / (bass[biqs_reso]*0.70710678) + K * K) * norm;
    norm = 1.0 / (1.0 + K / (bass[biqs_reso]*0.51763809) + K * K);
    bass[biqs_e0] = K / (bass[biqs_reso]*0.51763809) * norm;
    bass[biqs_f1] = 2.0 * (K * K - 1.0) * norm;
    bass[biqs_f2] = (1.0 - K / (bass[biqs_reso]*0.51763809) + K * K) * norm;
    //bass
    
    double refdB = (params[KNOBDSC]->get()*70.0)+70.0;
    double topdB = 0.000000075 * pow(10.0,refdB/20.0) * overallscale;
    
    panA = panB; panB = params[KNOBPAN]->get()*1.57079633;
    inTrimA = inTrimB; inTrimB = params[KNOBFAD]->get()*2.0;
    
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        auto outL = mainOutput.getWritePointer(0, i);
        auto outR = mainOutput.getWritePointer(1, i);
        auto inL = mainInput.getReadPointer(0, i); //in isBussesLayoutSupported, we have already
        auto inR = mainInput.getReadPointer(1, i); //specified that we can only be stereo and never mono
        long double inputSampleL = *inL;
        long double inputSampleR = *inR;
        if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
        if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
        //NOTE: I don't yet know whether counting the buffer backwards means that gainA and gainB must be reversed.
        //If the audio flow is in fact reversed we must swap the temp and (1.0-temp) and I have done this provisionally
        //Original VST2 is counting DOWN with -- operator, but this counts UP with ++
        //If this doesn't create zipper noise on sine processing then it's correct
        
        if (highpassEngage) { //distributed Highpass
            highpass[hilp_temp] = (inputSampleL*highpass[hilp_a0])+highpass[hilp_aL1];
            highpass[hilp_aL1] = (inputSampleL*highpass[hilp_a1])-(highpass[hilp_temp]*highpass[hilp_b1])+highpass[hilp_aL2];
            highpass[hilp_aL2] = (inputSampleL*highpass[hilp_a0])-(highpass[hilp_temp]*highpass[hilp_b2]); inputSampleL = highpass[hilp_temp];
            highpass[hilp_temp] = (inputSampleR*highpass[hilp_a0])+highpass[hilp_aR1];
            highpass[hilp_aR1] = (inputSampleR*highpass[hilp_a1])-(highpass[hilp_temp]*highpass[hilp_b1])+highpass[hilp_aR2];
            highpass[hilp_aR2] = (inputSampleR*highpass[hilp_a0])-(highpass[hilp_temp]*highpass[hilp_b2]); inputSampleR = highpass[hilp_temp];
        } else highpass[hilp_aR1] = highpass[hilp_aR2] = highpass[hilp_aL1] = highpass[hilp_aL2] = 0.0;
        if (lowpassEngage) { //distributed Lowpass
            lowpass[hilp_temp] = (inputSampleL*lowpass[hilp_a0])+lowpass[hilp_aL1];
            lowpass[hilp_aL1] = (inputSampleL*lowpass[hilp_a1])-(lowpass[hilp_temp]*lowpass[hilp_b1])+lowpass[hilp_aL2];
            lowpass[hilp_aL2] = (inputSampleL*lowpass[hilp_a0])-(lowpass[hilp_temp]*lowpass[hilp_b2]); inputSampleL = lowpass[hilp_temp];
            lowpass[hilp_temp] = (inputSampleR*lowpass[hilp_a0])+lowpass[hilp_aR1];
            lowpass[hilp_aR1] = (inputSampleR*lowpass[hilp_a1])-(lowpass[hilp_temp]*lowpass[hilp_b1])+lowpass[hilp_aR2];
            lowpass[hilp_aR2] = (inputSampleR*lowpass[hilp_a0])-(lowpass[hilp_temp]*lowpass[hilp_b2]); inputSampleR = lowpass[hilp_temp];
        } else lowpass[hilp_aR1] = lowpass[hilp_aR2] = lowpass[hilp_aL1] = lowpass[hilp_aL2] = 0.0;
        //first Highpass/Lowpass blocks aliasing before the nonlinearity of ConsoleXBuss and Parametric
        
        //get all Parametric bands before any other processing is done
        //begin Stacked Biquad With Reversed Neutron Flow L
        high[biqs_outL] = inputSampleL * fabs(high[biqs_level]);
        high[biqs_dis] = fabs(high[biqs_a0] * (1.0+(high[biqs_outL]*high[biqs_nonlin])));
        if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
        high[biqs_temp] = (high[biqs_outL] * high[biqs_dis]) + high[biqs_aL1];
        high[biqs_aL1] = high[biqs_aL2] - (high[biqs_temp]*high[biqs_b1]);
        high[biqs_aL2] = (high[biqs_outL] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_b2]);
        high[biqs_outL] = high[biqs_temp];
        high[biqs_dis] = fabs(high[biqs_c0] * (1.0+(high[biqs_outL]*high[biqs_nonlin])));
        if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
        high[biqs_temp] = (high[biqs_outL] * high[biqs_dis]) + high[biqs_cL1];
        high[biqs_cL1] = high[biqs_cL2] - (high[biqs_temp]*high[biqs_d1]);
        high[biqs_cL2] = (high[biqs_outL] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_d2]);
        high[biqs_outL] = high[biqs_temp];
        high[biqs_dis] = fabs(high[biqs_e0] * (1.0+(high[biqs_outL]*high[biqs_nonlin])));
        if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
        high[biqs_temp] = (high[biqs_outL] * high[biqs_dis]) + high[biqs_eL1];
        high[biqs_eL1] = high[biqs_eL2] - (high[biqs_temp]*high[biqs_f1]);
        high[biqs_eL2] = (high[biqs_outL] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_f2]);
        high[biqs_outL] = high[biqs_temp]; high[biqs_outL] *= high[biqs_level];
        if (high[biqs_level] > 1.0) high[biqs_outL] *= high[biqs_level];
        //end Stacked Biquad With Reversed Neutron Flow L
        
        //begin Stacked Biquad With Reversed Neutron Flow L
        hmid[biqs_outL] = inputSampleL * fabs(hmid[biqs_level]);
        hmid[biqs_dis] = fabs(hmid[biqs_a0] * (1.0+(hmid[biqs_outL]*hmid[biqs_nonlin])));
        if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
        hmid[biqs_temp] = (hmid[biqs_outL] * hmid[biqs_dis]) + hmid[biqs_aL1];
        hmid[biqs_aL1] = hmid[biqs_aL2] - (hmid[biqs_temp]*hmid[biqs_b1]);
        hmid[biqs_aL2] = (hmid[biqs_outL] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_b2]);
        hmid[biqs_outL] = hmid[biqs_temp];
        hmid[biqs_dis] = fabs(hmid[biqs_c0] * (1.0+(hmid[biqs_outL]*hmid[biqs_nonlin])));
        if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
        hmid[biqs_temp] = (hmid[biqs_outL] * hmid[biqs_dis]) + hmid[biqs_cL1];
        hmid[biqs_cL1] = hmid[biqs_cL2] - (hmid[biqs_temp]*hmid[biqs_d1]);
        hmid[biqs_cL2] = (hmid[biqs_outL] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_d2]);
        hmid[biqs_outL] = hmid[biqs_temp];
        hmid[biqs_dis] = fabs(hmid[biqs_e0] * (1.0+(hmid[biqs_outL]*hmid[biqs_nonlin])));
        if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
        hmid[biqs_temp] = (hmid[biqs_outL] * hmid[biqs_dis]) + hmid[biqs_eL1];
        hmid[biqs_eL1] = hmid[biqs_eL2] - (hmid[biqs_temp]*hmid[biqs_f1]);
        hmid[biqs_eL2] = (hmid[biqs_outL] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_f2]);
        hmid[biqs_outL] = hmid[biqs_temp]; hmid[biqs_outL] *= hmid[biqs_level];
        if (hmid[biqs_level] > 1.0) hmid[biqs_outL] *= hmid[biqs_level];
        //end Stacked Biquad With Reversed Neutron Flow L
        
        //begin Stacked Biquad With Reversed Neutron Flow L
        lmid[biqs_outL] = inputSampleL * fabs(lmid[biqs_level]);
        lmid[biqs_dis] = fabs(lmid[biqs_a0] * (1.0+(lmid[biqs_outL]*lmid[biqs_nonlin])));
        if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
        lmid[biqs_temp] = (lmid[biqs_outL] * lmid[biqs_dis]) + lmid[biqs_aL1];
        lmid[biqs_aL1] = lmid[biqs_aL2] - (lmid[biqs_temp]*lmid[biqs_b1]);
        lmid[biqs_aL2] = (lmid[biqs_outL] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_b2]);
        lmid[biqs_outL] = lmid[biqs_temp];
        lmid[biqs_dis] = fabs(lmid[biqs_c0] * (1.0+(lmid[biqs_outL]*lmid[biqs_nonlin])));
        if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
        lmid[biqs_temp] = (lmid[biqs_outL] * lmid[biqs_dis]) + lmid[biqs_cL1];
        lmid[biqs_cL1] = lmid[biqs_cL2] - (lmid[biqs_temp]*lmid[biqs_d1]);
        lmid[biqs_cL2] = (lmid[biqs_outL] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_d2]);
        lmid[biqs_outL] = lmid[biqs_temp];
        lmid[biqs_dis] = fabs(lmid[biqs_e0] * (1.0+(lmid[biqs_outL]*lmid[biqs_nonlin])));
        if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
        lmid[biqs_temp] = (lmid[biqs_outL] * lmid[biqs_dis]) + lmid[biqs_eL1];
        lmid[biqs_eL1] = lmid[biqs_eL2] - (lmid[biqs_temp]*lmid[biqs_f1]);
        lmid[biqs_eL2] = (lmid[biqs_outL] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_f2]);
        lmid[biqs_outL] = lmid[biqs_temp]; lmid[biqs_outL] *= lmid[biqs_level];
        if (lmid[biqs_level] > 1.0) lmid[biqs_outL] *= lmid[biqs_level];
        //end Stacked Biquad With Reversed Neutron Flow L
        
        //begin Stacked Biquad With Reversed Neutron Flow L
        bass[biqs_outL] = inputSampleL * fabs(bass[biqs_level]);
        bass[biqs_dis] = fabs(bass[biqs_a0] * (1.0+(bass[biqs_outL]*bass[biqs_nonlin])));
        if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
        bass[biqs_temp] = (bass[biqs_outL] * bass[biqs_dis]) + bass[biqs_aL1];
        bass[biqs_aL1] = bass[biqs_aL2] - (bass[biqs_temp]*bass[biqs_b1]);
        bass[biqs_aL2] = (bass[biqs_outL] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_b2]);
        bass[biqs_outL] = bass[biqs_temp];
        bass[biqs_dis] = fabs(bass[biqs_c0] * (1.0+(bass[biqs_outL]*bass[biqs_nonlin])));
        if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
        bass[biqs_temp] = (bass[biqs_outL] * bass[biqs_dis]) + bass[biqs_cL1];
        bass[biqs_cL1] = bass[biqs_cL2] - (bass[biqs_temp]*bass[biqs_d1]);
        bass[biqs_cL2] = (bass[biqs_outL] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_d2]);
        bass[biqs_outL] = bass[biqs_temp];
        bass[biqs_dis] = fabs(bass[biqs_e0] * (1.0+(bass[biqs_outL]*bass[biqs_nonlin])));
        if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
        bass[biqs_temp] = (bass[biqs_outL] * bass[biqs_dis]) + bass[biqs_eL1];
        bass[biqs_eL1] = bass[biqs_eL2] - (bass[biqs_temp]*bass[biqs_f1]);
        bass[biqs_eL2] = (bass[biqs_outL] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_f2]);
        bass[biqs_outL] = bass[biqs_temp]; bass[biqs_outL] *= bass[biqs_level];
        if (bass[biqs_level] > 1.0) bass[biqs_outL] *= bass[biqs_level];
        //end Stacked Biquad With Reversed Neutron Flow L
        
        //begin Stacked Biquad With Reversed Neutron Flow R
        high[biqs_outR] = inputSampleR * fabs(high[biqs_level]);
        high[biqs_dis] = fabs(high[biqs_a0] * (1.0+(high[biqs_outR]*high[biqs_nonlin])));
        if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
        high[biqs_temp] = (high[biqs_outR] * high[biqs_dis]) + high[biqs_aR1];
        high[biqs_aR1] = high[biqs_aR2] - (high[biqs_temp]*high[biqs_b1]);
        high[biqs_aR2] = (high[biqs_outR] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_b2]);
        high[biqs_outR] = high[biqs_temp];
        high[biqs_dis] = fabs(high[biqs_c0] * (1.0+(high[biqs_outR]*high[biqs_nonlin])));
        if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
        high[biqs_temp] = (high[biqs_outR] * high[biqs_dis]) + high[biqs_cR1];
        high[biqs_cR1] = high[biqs_cR2] - (high[biqs_temp]*high[biqs_d1]);
        high[biqs_cR2] = (high[biqs_outR] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_d2]);
        high[biqs_outR] = high[biqs_temp];
        high[biqs_dis] = fabs(high[biqs_e0] * (1.0+(high[biqs_outR]*high[biqs_nonlin])));
        if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
        high[biqs_temp] = (high[biqs_outR] * high[biqs_dis]) + high[biqs_eR1];
        high[biqs_eR1] = high[biqs_eR2] - (high[biqs_temp]*high[biqs_f1]);
        high[biqs_eR2] = (high[biqs_outR] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_f2]);
        high[biqs_outR] = high[biqs_temp]; high[biqs_outR] *= high[biqs_level];
        if (high[biqs_level] > 1.0) high[biqs_outR] *= high[biqs_level];
        //end Stacked Biquad With Reversed Neutron Flow R
        
        //begin Stacked Biquad With Reversed Neutron Flow R
        hmid[biqs_outR] = inputSampleR * fabs(hmid[biqs_level]);
        hmid[biqs_dis] = fabs(hmid[biqs_a0] * (1.0+(hmid[biqs_outR]*hmid[biqs_nonlin])));
        if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
        hmid[biqs_temp] = (hmid[biqs_outR] * hmid[biqs_dis]) + hmid[biqs_aR1];
        hmid[biqs_aR1] = hmid[biqs_aR2] - (hmid[biqs_temp]*hmid[biqs_b1]);
        hmid[biqs_aR2] = (hmid[biqs_outR] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_b2]);
        hmid[biqs_outR] = hmid[biqs_temp];
        hmid[biqs_dis] = fabs(hmid[biqs_c0] * (1.0+(hmid[biqs_outR]*hmid[biqs_nonlin])));
        if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
        hmid[biqs_temp] = (hmid[biqs_outR] * hmid[biqs_dis]) + hmid[biqs_cR1];
        hmid[biqs_cR1] = hmid[biqs_cR2] - (hmid[biqs_temp]*hmid[biqs_d1]);
        hmid[biqs_cR2] = (hmid[biqs_outR] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_d2]);
        hmid[biqs_outR] = hmid[biqs_temp];
        hmid[biqs_dis] = fabs(hmid[biqs_e0] * (1.0+(hmid[biqs_outR]*hmid[biqs_nonlin])));
        if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
        hmid[biqs_temp] = (hmid[biqs_outR] * hmid[biqs_dis]) + hmid[biqs_eR1];
        hmid[biqs_eR1] = hmid[biqs_eR2] - (hmid[biqs_temp]*hmid[biqs_f1]);
        hmid[biqs_eR2] = (hmid[biqs_outR] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_f2]);
        hmid[biqs_outR] = hmid[biqs_temp]; hmid[biqs_outR] *= hmid[biqs_level];
        if (hmid[biqs_level] > 1.0) hmid[biqs_outR] *= hmid[biqs_level];
        //end Stacked Biquad With Reversed Neutron Flow R
        
        //begin Stacked Biquad With Reversed Neutron Flow R
        lmid[biqs_outR] = inputSampleR * fabs(lmid[biqs_level]);
        lmid[biqs_dis] = fabs(lmid[biqs_a0] * (1.0+(lmid[biqs_outR]*lmid[biqs_nonlin])));
        if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
        lmid[biqs_temp] = (lmid[biqs_outR] * lmid[biqs_dis]) + lmid[biqs_aR1];
        lmid[biqs_aR1] = lmid[biqs_aR2] - (lmid[biqs_temp]*lmid[biqs_b1]);
        lmid[biqs_aR2] = (lmid[biqs_outR] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_b2]);
        lmid[biqs_outR] = lmid[biqs_temp];
        lmid[biqs_dis] = fabs(lmid[biqs_c0] * (1.0+(lmid[biqs_outR]*lmid[biqs_nonlin])));
        if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
        lmid[biqs_temp] = (lmid[biqs_outR] * lmid[biqs_dis]) + lmid[biqs_cR1];
        lmid[biqs_cR1] = lmid[biqs_cR2] - (lmid[biqs_temp]*lmid[biqs_d1]);
        lmid[biqs_cR2] = (lmid[biqs_outR] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_d2]);
        lmid[biqs_outR] = lmid[biqs_temp];
        lmid[biqs_dis] = fabs(lmid[biqs_e0] * (1.0+(lmid[biqs_outR]*lmid[biqs_nonlin])));
        if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
        lmid[biqs_temp] = (lmid[biqs_outR] * lmid[biqs_dis]) + lmid[biqs_eR1];
        lmid[biqs_eR1] = lmid[biqs_eR2] - (lmid[biqs_temp]*lmid[biqs_f1]);
        lmid[biqs_eR2] = (lmid[biqs_outR] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_f2]);
        lmid[biqs_outR] = lmid[biqs_temp]; lmid[biqs_outR] *= lmid[biqs_level];
        if (lmid[biqs_level] > 1.0) lmid[biqs_outR] *= lmid[biqs_level];
        //end Stacked Biquad With Reversed Neutron Flow R
        
        //begin Stacked Biquad With Reversed Neutron Flow R
        bass[biqs_outR] = inputSampleR * fabs(bass[biqs_level]);
        bass[biqs_dis] = fabs(bass[biqs_a0] * (1.0+(bass[biqs_outR]*bass[biqs_nonlin])));
        if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
        bass[biqs_temp] = (bass[biqs_outR] * bass[biqs_dis]) + bass[biqs_aR1];
        bass[biqs_aR1] = bass[biqs_aR2] - (bass[biqs_temp]*bass[biqs_b1]);
        bass[biqs_aR2] = (bass[biqs_outR] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_b2]);
        bass[biqs_outR] = bass[biqs_temp];
        bass[biqs_dis] = fabs(bass[biqs_c0] * (1.0+(bass[biqs_outR]*bass[biqs_nonlin])));
        if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
        bass[biqs_temp] = (bass[biqs_outR] * bass[biqs_dis]) + bass[biqs_cR1];
        bass[biqs_cR1] = bass[biqs_cR2] - (bass[biqs_temp]*bass[biqs_d1]);
        bass[biqs_cR2] = (bass[biqs_outR] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_d2]);
        bass[biqs_outR] = bass[biqs_temp];
        bass[biqs_dis] = fabs(bass[biqs_e0] * (1.0+(bass[biqs_outR]*bass[biqs_nonlin])));
        if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
        bass[biqs_temp] = (bass[biqs_outR] * bass[biqs_dis]) + bass[biqs_eR1];
        bass[biqs_eR1] = bass[biqs_eR2] - (bass[biqs_temp]*bass[biqs_f1]);
        bass[biqs_eR2] = (bass[biqs_outR] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_f2]);
        bass[biqs_outR] = bass[biqs_temp]; bass[biqs_outR] *= bass[biqs_level];
        if (bass[biqs_level] > 1.0) bass[biqs_outR] *= bass[biqs_level];
        //end Stacked Biquad With Reversed Neutron Flow R
        
        double temp = (double)buffer.getNumSamples()/inFramesToProcess;
        double gainR = (panA*temp)+(panB*(1.0-temp));
        double gainL = 1.57079633-gainR;
        gainR = sin(gainR); gainL = sin(gainL);
        double gain = (inTrimA*temp)+(inTrimB*(1.0-temp));
        if (gain > 1.0) gain *= gain; else gain = 1.0-pow(1.0-gain,2);
        gain *= 0.763932022500211;
        double airGain = (airGainA*temp)+(airGainB*(1.0-temp));
        if (airGain > 1.0) airGain *= airGain; else airGain = 1.0-pow(1.0-airGain,2);
        double fireGain = (fireGainA*temp)+(fireGainB*(1.0-temp));
        if (fireGain > 1.0) fireGain *= fireGain; else fireGain = 1.0-pow(1.0-fireGain,2);
        double firePad = fireGain; if (firePad > 1.0) firePad = 1.0;
        double stoneGain = (stoneGainA*temp)+(stoneGainB*(1.0-temp));
        if (stoneGain > 1.0) stoneGain *= stoneGain; else stoneGain = 1.0-pow(1.0-stoneGain,2);
        double stonePad = stoneGain; if (stonePad > 1.0) stonePad = 1.0;
        //set up smoothed gain controls
        
        //begin Air3L
        double drySampleL = inputSampleL;
        air[pvSL4] = air[pvAL4] - air[pvAL3]; air[pvSL3] = air[pvAL3] - air[pvAL2];
        air[pvSL2] = air[pvAL2] - air[pvAL1]; air[pvSL1] = air[pvAL1] - inputSampleL;
        air[accSL3] = air[pvSL4] - air[pvSL3]; air[accSL2] = air[pvSL3] - air[pvSL2];
        air[accSL1] = air[pvSL2] - air[pvSL1];
        air[acc2SL2] = air[accSL3] - air[accSL2]; air[acc2SL1] = air[accSL2] - air[accSL1];
        air[outAL] = -(air[pvAL1] + air[pvSL3] + air[acc2SL2] - ((air[acc2SL2] + air[acc2SL1])*0.5));
        air[gainAL] *= 0.5; air[gainAL] += fabs(drySampleL-air[outAL])*0.5;
        if (air[gainAL] > 0.3*sqrt(overallscale)) air[gainAL] = 0.3*sqrt(overallscale);
        air[pvAL4] = air[pvAL3]; air[pvAL3] = air[pvAL2];
        air[pvAL2] = air[pvAL1]; air[pvAL1] = (air[gainAL] * air[outAL]) + drySampleL;
        double fireL = drySampleL - ((air[outAL]*0.5)+(drySampleL*(0.457-(0.017*overallscale))));
        temp = (fireL + air[gndavgL])*0.5; air[gndavgL] = fireL; fireL = temp;
        double airL = (drySampleL-fireL)*airGain;
        inputSampleL = fireL;
        //end Air3L
        //begin Air3R
        double drySampleR = inputSampleR;
        air[pvSR4] = air[pvAR4] - air[pvAR3]; air[pvSR3] = air[pvAR3] - air[pvAR2];
        air[pvSR2] = air[pvAR2] - air[pvAR1]; air[pvSR1] = air[pvAR1] - inputSampleR;
        air[accSR3] = air[pvSR4] - air[pvSR3]; air[accSR2] = air[pvSR3] - air[pvSR2];
        air[accSR1] = air[pvSR2] - air[pvSR1];
        air[acc2SR2] = air[accSR3] - air[accSR2]; air[acc2SR1] = air[accSR2] - air[accSR1];
        air[outAR] = -(air[pvAR1] + air[pvSR3] + air[acc2SR2] - ((air[acc2SR2] + air[acc2SR1])*0.5));
        air[gainAR] *= 0.5; air[gainAR] += fabs(drySampleR-air[outAR])*0.5;
        if (air[gainAR] > 0.3*sqrt(overallscale)) air[gainAR] = 0.3*sqrt(overallscale);
        air[pvAR4] = air[pvAR3]; air[pvAR3] = air[pvAR2];
        air[pvAR2] = air[pvAR1]; air[pvAR1] = (air[gainAR] * air[outAR]) + drySampleR;
        double fireR = drySampleR - ((air[outAR]*0.5)+(drySampleR*(0.457-(0.017*overallscale))));
        temp = (fireR + air[gndavgR])*0.5; air[gndavgR] = fireR; fireR = temp;
        double airR = (drySampleR-fireR)*airGain;
        inputSampleR = fireR;
        //end Air3R
        //begin KalmanL
        temp = inputSampleL = inputSampleL*(1.0-kalmanRange)*0.777;
        inputSampleL *= (1.0-kalmanRange);
        //set up gain levels to control the beast
        kal[prevSlewL3] += kal[prevSampL3] - kal[prevSampL2]; kal[prevSlewL3] *= 0.5;
        kal[prevSlewL2] += kal[prevSampL2] - kal[prevSampL1]; kal[prevSlewL2] *= 0.5;
        kal[prevSlewL1] += kal[prevSampL1] - inputSampleL; kal[prevSlewL1] *= 0.5;
        //make slews from each set of samples used
        kal[accSlewL2] += kal[prevSlewL3] - kal[prevSlewL2]; kal[accSlewL2] *= 0.5;
        kal[accSlewL1] += kal[prevSlewL2] - kal[prevSlewL1]; kal[accSlewL1] *= 0.5;
        //differences between slews: rate of change of rate of change
        kal[accSlewL3] += (kal[accSlewL2] - kal[accSlewL1]); kal[accSlewL3] *= 0.5;
        //entering the abyss, what even is this
        kal[kalOutL] += kal[prevSampL1] + kal[prevSlewL2] + kal[accSlewL3]; kal[kalOutL] *= 0.5;
        //resynthesizing predicted result (all iir smoothed)
        kal[kalGainL] += fabs(temp-kal[kalOutL])*kalmanRange*8.0; kal[kalGainL] *= 0.5;
        //madness takes its toll. Kalman Gain: how much dry to retain
        if (kal[kalGainL] > kalmanRange*0.5) kal[kalGainL] = kalmanRange*0.5;
        //attempts to avoid explosions
        kal[kalOutL] += (temp*(1.0-(0.68+(kalmanRange*0.157))));
        //this is for tuning a really complete cancellation up around Nyquist
        kal[prevSampL3] = kal[prevSampL2]; kal[prevSampL2] = kal[prevSampL1];
        kal[prevSampL1] = (kal[kalGainL] * kal[kalOutL]) + ((1.0-kal[kalGainL])*temp);
        //feed the chain of previous samples
        if (kal[prevSampL1] > 1.0) kal[prevSampL1] = 1.0; if (kal[prevSampL1] < -1.0) kal[prevSampL1] = -1.0;
        double stoneL = kal[kalOutL]*0.777;
        fireL -= stoneL;
        //end KalmanL
        //begin KalmanR
        temp = inputSampleR = inputSampleR*(1.0-kalmanRange)*0.777;
        inputSampleR *= (1.0-kalmanRange);
        //set up gain levels to control the beast
        kal[prevSlewR3] += kal[prevSampR3] - kal[prevSampR2]; kal[prevSlewR3] *= 0.5;
        kal[prevSlewR2] += kal[prevSampR2] - kal[prevSampR1]; kal[prevSlewR2] *= 0.5;
        kal[prevSlewR1] += kal[prevSampR1] - inputSampleR; kal[prevSlewR1] *= 0.5;
        //make slews from each set of samples used
        kal[accSlewR2] += kal[prevSlewR3] - kal[prevSlewR2]; kal[accSlewR2] *= 0.5;
        kal[accSlewR1] += kal[prevSlewR2] - kal[prevSlewR1]; kal[accSlewR1] *= 0.5;
        //differences between slews: rate of change of rate of change
        kal[accSlewR3] += (kal[accSlewR2] - kal[accSlewR1]); kal[accSlewR3] *= 0.5;
        //entering the abyss, what even is this
        kal[kalOutR] += kal[prevSampR1] + kal[prevSlewR2] + kal[accSlewR3]; kal[kalOutR] *= 0.5;
        //resynthesizing predicted result (all iir smoothed)
        kal[kalGainR] += fabs(temp-kal[kalOutR])*kalmanRange*8.0; kal[kalGainR] *= 0.5;
        //madness takes its toll. Kalman Gain: how much dry to retain
        if (kal[kalGainR] > kalmanRange*0.5) kal[kalGainR] = kalmanRange*0.5;
        //attempts to avoid explosions
        kal[kalOutR] += (temp*(1.0-(0.68+(kalmanRange*0.157))));
        //this is for tuning a really complete cancellation up around Nyquist
        kal[prevSampR3] = kal[prevSampR2]; kal[prevSampR2] = kal[prevSampR1];
        kal[prevSampR1] = (kal[kalGainR] * kal[kalOutR]) + ((1.0-kal[kalGainR])*temp);
        //feed the chain of previous samples
        if (kal[prevSampR1] > 1.0) kal[prevSampR1] = 1.0; if (kal[prevSampR1] < -1.0) kal[prevSampR1] = -1.0;
        double stoneR = kal[kalOutR]*0.777;
        fireR -= stoneR;
        //end KalmanR
        
        if (baseFire < fabs(fireR*64.0)) baseFire = fmin(fabs(fireR*64.0),1.0);
        if (baseFire < fabs(fireL*64.0)) baseFire = fmin(fabs(fireL*64.0),1.0);
        if (baseStone < fabs(stoneR*256.0)) baseStone = fmin(fabs(stoneR*256.0),1.0);
        if (baseStone < fabs(stoneL*256.0)) baseStone = fmin(fabs(stoneL*256.0),1.0);
        //blinkenlights are off when there's no audio
        
        //fire dynamics
        if (fabs(fireL) > compFThresh) { //compression L
            fireCompL -= (fireCompL * compFAttack);
            fireCompL += ((compFThresh / fabs(fireL))*compFAttack);
        } else fireCompL = (fireCompL*(1.0-compFRelease))+compFRelease;
        if (fabs(fireR) > compFThresh) { //compression R
            fireCompR -= (fireCompR * compFAttack);
            fireCompR += ((compFThresh / fabs(fireR))*compFAttack);
        } else fireCompR = (fireCompR*(1.0-compFRelease))+compFRelease;
        if (fireCompL > fireCompR) fireCompL -= (fireCompL * compFAttack);
        if (fireCompR > fireCompL) fireCompR -= (fireCompR * compFAttack);
        if (fabs(fireL) > gateFThresh) fireGate = gateFSustain;
        else if (fabs(fireR) > gateFThresh) fireGate = gateFSustain;
        else fireGate *= (1.0-gateFRelease);
        if (fireGate < 0.0) fireGate = 0.0;
        fireCompL = fmax(fmin(fireCompL,1.0),0.0);
        fireCompR = fmax(fmin(fireCompR,1.0),0.0);
        if (maxFireComp > fmin(fireCompL,fireCompR)) maxFireComp = fmin(fireCompL,fireCompR);
        fireL *= (((1.0-compFRatio)*firePad)+(fireCompL*compFRatio*fireGain));
        fireR *= (((1.0-compFRatio)*firePad)+(fireCompR*compFRatio*fireGain));
        if (fireGate < M_PI_2) {
            temp = ((1.0-gateFRatio)+(sin(fireGate)*gateFRatio));
            if (maxFireGate > temp*temp) maxFireGate = temp*temp;
            airL *= temp;
            airR *= temp;
            fireL *= temp;
            fireR *= temp;
            high[biqs_outL] *= temp;
            high[biqs_outR] *= temp;
            hmid[biqs_outL] *= temp; //if Fire gating, gate Air, high and hmid
            hmid[biqs_outR] *= temp; //note that we aren't compressing these
        }
        //stone dynamics
        if (fabs(stoneL) > compSThresh) { //compression L
            stoneCompL -= (stoneCompL * compSAttack);
            stoneCompL += ((compSThresh / fabs(stoneL))*compSAttack);
        } else stoneCompL = (stoneCompL*(1.0-compSRelease))+compSRelease;
        if (fabs(stoneR) > compSThresh) { //compression R
            stoneCompR -= (stoneCompR * compSAttack);
            stoneCompR += ((compSThresh / fabs(stoneR))*compSAttack);
        } else stoneCompR = (stoneCompR*(1.0-compSRelease))+compSRelease;
        if (stoneCompL > stoneCompR) stoneCompL -= (stoneCompL * compSAttack);
        if (stoneCompR > stoneCompL) stoneCompR -= (stoneCompR * compSAttack);
        if (fabs(stoneL) > gateSThresh) stoneGate = gateSSustain;
        else if (fabs(stoneR) > gateSThresh) stoneGate = gateSSustain;
        else stoneGate *= (1.0-gateSRelease);
        if (stoneGate < 0.0) stoneGate = 0.0;
        stoneCompL = fmax(fmin(stoneCompL,1.0),0.0);
        stoneCompR = fmax(fmin(stoneCompR,1.0),0.0);
        if (maxStoneComp > fmin(stoneCompL,stoneCompR)) maxStoneComp = fmin(stoneCompL,stoneCompR);
        stoneL *= (((1.0-compSRatio)*stonePad)+(stoneCompL*compSRatio*stoneGain));
        stoneR *= (((1.0-compSRatio)*stonePad)+(stoneCompR*compSRatio*stoneGain));
        if (stoneGate < M_PI_2) {
            temp = ((1.0-gateSRatio)+(sin(stoneGate)*gateSRatio));
            if (maxStoneGate > temp*temp) maxStoneGate = temp*temp;
            stoneL *= temp;
            stoneR *= temp;
            lmid[biqs_outL] *= temp;
            lmid[biqs_outR] *= temp;
            bass[biqs_outL] *= temp; //if Stone gating, gate lmid and bass
            bass[biqs_outR] *= temp; //note that we aren't compressing these
        }
        inputSampleL = stoneL + fireL + airL;
        inputSampleR = stoneR + fireR + airR;
        //create Stonefire output
        
        if (highpassEngage) { //distributed Highpass
            highpass[hilp_temp] = (inputSampleL*highpass[hilp_c0])+highpass[hilp_cL1];
            highpass[hilp_cL1] = (inputSampleL*highpass[hilp_c1])-(highpass[hilp_temp]*highpass[hilp_d1])+highpass[hilp_cL2];
            highpass[hilp_cL2] = (inputSampleL*highpass[hilp_c0])-(highpass[hilp_temp]*highpass[hilp_d2]); inputSampleL = highpass[hilp_temp];
            highpass[hilp_temp] = (inputSampleR*highpass[hilp_c0])+highpass[hilp_cR1];
            highpass[hilp_cR1] = (inputSampleR*highpass[hilp_c1])-(highpass[hilp_temp]*highpass[hilp_d1])+highpass[hilp_cR2];
            highpass[hilp_cR2] = (inputSampleR*highpass[hilp_c0])-(highpass[hilp_temp]*highpass[hilp_d2]); inputSampleR = highpass[hilp_temp];
        } else highpass[hilp_cR1] = highpass[hilp_cR2] = highpass[hilp_cL1] = highpass[hilp_cL2] = 0.0;
        if (lowpassEngage) { //distributed Lowpass
            lowpass[hilp_temp] = (inputSampleL*lowpass[hilp_c0])+lowpass[hilp_cL1];
            lowpass[hilp_cL1] = (inputSampleL*lowpass[hilp_c1])-(lowpass[hilp_temp]*lowpass[hilp_d1])+lowpass[hilp_cL2];
            lowpass[hilp_cL2] = (inputSampleL*lowpass[hilp_c0])-(lowpass[hilp_temp]*lowpass[hilp_d2]); inputSampleL = lowpass[hilp_temp];
            lowpass[hilp_temp] = (inputSampleR*lowpass[hilp_c0])+lowpass[hilp_cR1];
            lowpass[hilp_cR1] = (inputSampleR*lowpass[hilp_c1])-(lowpass[hilp_temp]*lowpass[hilp_d1])+lowpass[hilp_cR2];
            lowpass[hilp_cR2] = (inputSampleR*lowpass[hilp_c0])-(lowpass[hilp_temp]*lowpass[hilp_d2]); inputSampleR = lowpass[hilp_temp];
        } else lowpass[hilp_cR1] = lowpass[hilp_cR2] = lowpass[hilp_cL1] = lowpass[hilp_cL2] = 0.0;
        //another stage of Highpass/Lowpass before bringing in the parametric bands
        
        inputSampleL += (high[biqs_outL] + hmid[biqs_outL] + lmid[biqs_outL] + bass[biqs_outL]);
        inputSampleR += (high[biqs_outR] + hmid[biqs_outR] + lmid[biqs_outR] + bass[biqs_outR]);
        //add parametric boosts or cuts: clean as possible for maximal rawness and sonority
        
        inputSampleL = inputSampleL * gainL * gain;
        inputSampleR = inputSampleR * gainR * gain;
        //applies pan section, and smoothed fader gain
        
        inputSampleL *= topdB;
        if (inputSampleL < -0.222) inputSampleL = -0.222; if (inputSampleL > 0.222) inputSampleL = 0.222;
        dBaL[dBaXL] = inputSampleL; dBaPosL *= 0.5; dBaPosL += fabs((inputSampleL*((inputSampleL*0.25)-0.5))*0.5);
        int dBdly = (int)floor(dBaPosL*dscBuf);
        double dBi = (dBaPosL*dscBuf)-dBdly;
        inputSampleL = dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
        dBdly++; inputSampleL += dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*dBi;
        dBaXL++; if (dBaXL < 0 || dBaXL >= dscBuf) dBaXL = 0;
        inputSampleL /= topdB;
        inputSampleR *= topdB;
        if (inputSampleR < -0.222) inputSampleR = -0.222; if (inputSampleR > 0.222) inputSampleR = 0.222;
        dBaR[dBaXR] = inputSampleR; dBaPosR *= 0.5; dBaPosR += fabs((inputSampleR*((inputSampleR*0.25)-0.5))*0.5);
        dBdly = (int)floor(dBaPosR*dscBuf);
        dBi = (dBaPosR*dscBuf)-dBdly;
        inputSampleR = dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
        dBdly++; inputSampleR += dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*dBi;
        dBaXR++; if (dBaXR < 0 || dBaXR >= dscBuf) dBaXR = 0;
        inputSampleR /= topdB;
        //top dB processing for distributed discontinuity modeling air nonlinearity
        
        //ConsoleXChannel before final Highpass/Lowpass stages
        inputSampleL *= 0.618033988749895;
        if (inputSampleL > 1.0) inputSampleL = 1.0;
        else if (inputSampleL > 0.0) inputSampleL = -expm1((log1p(-inputSampleL) * 1.618033988749895));
        if (inputSampleL < -1.0) inputSampleL = -1.0;
        else if (inputSampleL < 0.0) inputSampleL = expm1((log1p(inputSampleL) * 1.618033988749895));
        inputSampleR *= 0.618033988749895;
        if (inputSampleR > 1.0) inputSampleR = 1.0;
        else if (inputSampleR > 0.0) inputSampleR = -expm1((log1p(-inputSampleR) * 1.618033988749895));
        if (inputSampleR < -1.0) inputSampleR = -1.0;
        else if (inputSampleR < 0.0) inputSampleR = expm1((log1p(inputSampleR) * 1.618033988749895));
        //ConsoleXChannel before final Highpass/Lowpass stages
                
        if (highpassEngage) { //distributed Highpass
            highpass[hilp_temp] = (inputSampleL*highpass[hilp_e0])+highpass[hilp_eL1];
            highpass[hilp_eL1] = (inputSampleL*highpass[hilp_e1])-(highpass[hilp_temp]*highpass[hilp_f1])+highpass[hilp_eL2];
            highpass[hilp_eL2] = (inputSampleL*highpass[hilp_e0])-(highpass[hilp_temp]*highpass[hilp_f2]); inputSampleL = highpass[hilp_temp];
            highpass[hilp_temp] = (inputSampleR*highpass[hilp_e0])+highpass[hilp_eR1];
            highpass[hilp_eR1] = (inputSampleR*highpass[hilp_e1])-(highpass[hilp_temp]*highpass[hilp_f1])+highpass[hilp_eR2];
            highpass[hilp_eR2] = (inputSampleR*highpass[hilp_e0])-(highpass[hilp_temp]*highpass[hilp_f2]); inputSampleR = highpass[hilp_temp];
        } else highpass[hilp_eR1] = highpass[hilp_eR2] = highpass[hilp_eL1] = highpass[hilp_eL2] = 0.0;
        if (lowpassEngage) { //distributed Lowpass
            lowpass[hilp_temp] = (inputSampleL*lowpass[hilp_e0])+lowpass[hilp_eL1];
            lowpass[hilp_eL1] = (inputSampleL*lowpass[hilp_e1])-(lowpass[hilp_temp]*lowpass[hilp_f1])+lowpass[hilp_eL2];
            lowpass[hilp_eL2] = (inputSampleL*lowpass[hilp_e0])-(lowpass[hilp_temp]*lowpass[hilp_f2]); inputSampleL = lowpass[hilp_temp];
            lowpass[hilp_temp] = (inputSampleR*lowpass[hilp_e0])+lowpass[hilp_eR1];
            lowpass[hilp_eR1] = (inputSampleR*lowpass[hilp_e1])-(lowpass[hilp_temp]*lowpass[hilp_f1])+lowpass[hilp_eR2];
            lowpass[hilp_eR2] = (inputSampleR*lowpass[hilp_e0])-(lowpass[hilp_temp]*lowpass[hilp_f2]); inputSampleR = lowpass[hilp_temp];
        } else lowpass[hilp_eR1] = lowpass[hilp_eR2] = lowpass[hilp_eL1] = lowpass[hilp_eL2] = 0.0;
        //final Highpass/Lowpass continues to address aliasing
        //final stacked biquad section is the softest Q for smoothness

        //begin bar display section
        if ((fabs(inputSampleL-previousLeft)/28000.0f)*getSampleRate() > slewLeft) slewLeft =  (fabs(inputSampleL-previousLeft)/28000.0f)*getSampleRate();
        if ((fabs(inputSampleR-previousRight)/28000.0f)*getSampleRate() > slewRight) slewRight = (fabs(inputSampleR-previousRight)/28000.0f)*getSampleRate();
        previousLeft = inputSampleL; previousRight = inputSampleR; //slew measurement is NOT rectified
        double rectifiedL = fabs(inputSampleL);
        double rectifiedR = fabs(inputSampleR);
        if (rectifiedL > peakLeft) peakLeft = rectifiedL;
        if (rectifiedR > peakRight) peakRight = rectifiedR;
        rmsLeft += (rectifiedL * rectifiedL);
        rmsRight += (rectifiedR * rectifiedR);
        rmsCount++;
        zeroLeft += zeroCrossScale;
        if (longestZeroLeft < zeroLeft) longestZeroLeft = zeroLeft;
        if (wasPositiveL && inputSampleL < 0.0) {
            wasPositiveL = false;
            zeroLeft = 0.0;
        } else if (!wasPositiveL && inputSampleL > 0.0) {
            wasPositiveL = true;
            zeroLeft = 0.0;
        }
        zeroRight += zeroCrossScale;
        if (longestZeroRight < zeroRight) longestZeroRight = zeroRight;
        if (wasPositiveR && inputSampleR < 0.0) {
            wasPositiveR = false;
            zeroRight = 0.0;
        } else if (!wasPositiveR && inputSampleR > 0.0) {
            wasPositiveR = true;
            zeroRight = 0.0;
        } //end bar display section

        //begin 64 bit stereo floating point dither
        int expon; frexp((double)inputSampleL, &expon);
        fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
        inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
        frexp((double)inputSampleR, &expon);
        fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
        inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
        //end 64 bit stereo floating point dither

        *outL = inputSampleL;
        *outR = inputSampleR;
    }

    if (rmsCount > rmsSize)
    {
        AudioToUIMessage msg; //define the thing we're telling JUCE
        msg.what = AudioToUIMessage::SLEW_LEFT; msg.newValue = (float)slewLeft; audioToUI.push(msg);
        msg.what = AudioToUIMessage::SLEW_RIGHT; msg.newValue = (float)slewRight; audioToUI.push(msg);
        msg.what = AudioToUIMessage::PEAK_LEFT; msg.newValue = (float)sqrt(peakLeft); audioToUI.push(msg);
        msg.what = AudioToUIMessage::PEAK_RIGHT; msg.newValue = (float)sqrt(peakRight); audioToUI.push(msg);
        msg.what = AudioToUIMessage::RMS_LEFT; msg.newValue = (float)sqrt(sqrt(rmsLeft/rmsCount)); audioToUI.push(msg);
        msg.what = AudioToUIMessage::RMS_RIGHT; msg.newValue = (float)sqrt(sqrt(rmsRight/rmsCount)); audioToUI.push(msg);
        msg.what = AudioToUIMessage::ZERO_LEFT; msg.newValue = (float)longestZeroLeft; audioToUI.push(msg);
        msg.what = AudioToUIMessage::ZERO_RIGHT; msg.newValue = (float)longestZeroRight; audioToUI.push(msg);
        msg.what = AudioToUIMessage::BLINKEN_FIRE_COMP; msg.newValue = (float)maxFireComp; audioToUI.push(msg);
        msg.what = AudioToUIMessage::BLINKEN_STONE_COMP; msg.newValue = (float)maxStoneComp; audioToUI.push(msg);
        msg.what = AudioToUIMessage::BLINKEN_FIRE_GATE; msg.newValue = (float)maxFireGate; audioToUI.push(msg);
        msg.what = AudioToUIMessage::BLINKEN_STONE_GATE; msg.newValue = (float)maxStoneGate; audioToUI.push(msg);
        msg.what = AudioToUIMessage::INCREMENT; msg.newValue = 0.0f; audioToUI.push(msg);
        slewLeft = 0.0;
        slewRight = 0.0;
        peakLeft = 0.0;
        peakRight = 0.0;
        rmsLeft = 0.0;
        rmsRight = 0.0;
        zeroLeft = 0.0;
        zeroRight = 0.0;
        longestZeroLeft = 0.0;
        longestZeroRight = 0.0;
        maxFireComp = baseFire;
        maxStoneComp = baseStone;
        maxFireGate = baseFire;
        maxStoneGate = baseStone;
        baseFire = 0.0;
        baseStone = 0.0;
        rmsCount = 0;
    }
}

