#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#if (MSVC)
#include "ipps.h"
#endif

class PluginProcessor : public juce::AudioProcessor, public juce::AudioProcessorParameter::Listener
{
public:
    PluginProcessor();
    ~PluginProcessor() override;
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool starting) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void processBlock (juce::AudioBuffer<double>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    bool supportsDoublePrecisionProcessing() const override;
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    // updateTrackProperties may be called by the host if it feels like it
    // this method calls a similar one in the editor class that updates the editor
    void updateTrackProperties(const TrackProperties& properties) override;
    
    TrackProperties trackProperties;

    //now we can declare variables used in the audio thread
    static constexpr int dscBuf = 90;
    
    enum Parameters
    {
        KNOBHIP,
        KNOBLOP,
        KNOBAIR,
        KNOBFIR,
        KNOBSTO,
        KNOBRNG,
        KNOBFCT,
        KNOBSCT,
        KNOBFCR,
        KNOBSCR,
        KNOBFCA,
        KNOBSCA,
        KNOBFCL,
        KNOBSCL,
        KNOBFGT,
        KNOBSGT,
        KNOBFGR,
        KNOBSGR,
        KNOBFGS,
        KNOBSGS,
        KNOBFGL,
        KNOBSGL,
        KNOBTRF,
        KNOBTRG,
        KNOBTRR,
        KNOBHMF,
        KNOBHMG,
        KNOBHMR,
        KNOBLMF,
        KNOBLMG,
        KNOBLMR,
        KNOBBSF,
        KNOBBSG,
        KNOBBSR,
        KNOBDSC,
        KNOBPAN,
        KNOBFAD,
    };
    static constexpr int n_params = KNOBFAD + 1;
    std::array<juce::AudioParameterFloat *, n_params> params;
    //This is where we're defining things that go into the plugin's interface.
    
    struct UIToAudioMessage
    {
        enum What
        {
            NEW_VALUE,
            BEGIN_EDIT,
            END_EDIT
        } what{NEW_VALUE};
        Parameters which;
        float newValue = 0.0;
    };
    //This is things the interface can tell the audio thread about.
    
    struct AudioToUIMessage
    {
        enum What
        {
            NEW_VALUE,
            SLEW_LEFT,
            SLEW_RIGHT,
            PEAK_LEFT,
            PEAK_RIGHT,
            RMS_LEFT,
            RMS_RIGHT,
            ZERO_LEFT,
            ZERO_RIGHT,
            BLINKEN_FIRE_COMP,
            BLINKEN_STONE_COMP,
            BLINKEN_FIRE_GATE,
            BLINKEN_STONE_GATE,
            INCREMENT
        } what{NEW_VALUE};
        Parameters which;
        float newValue = 0.0;
    };
    //This is kinds of information the audio thread can give the interface.
    
    template <typename T, int qSize = 4096> class LockFreeQueue
    {
      public:
        LockFreeQueue() : af(qSize) {}
        bool push(const T &ad)
        {
            auto ret = false;
            int start1, size1, start2, size2;
            af.prepareToWrite(1, start1, size1, start2, size2);
            if (size1 > 0)
            {
                dq[start1] = ad;
                ret = true;
            }
            af.finishedWrite(size1 + size2);
            return ret;
        }
        bool pop(T &ad)
        {
            bool ret = false;
            int start1, size1, start2, size2;
            af.prepareToRead(1, start1, size1, start2, size2);
            if (size1 > 0)
            {
                ad = dq[start1];
                ret = true;
            }
            af.finishedRead(size1 + size2);
            return ret;
        }
        juce::AbstractFifo af;
        T dq[qSize];
    };
    
    LockFreeQueue<UIToAudioMessage> uiToAudio;
    LockFreeQueue<AudioToUIMessage> audioToUI;
    
    
    enum {
        hilp_freq, hilp_temp,
        hilp_a0, hilp_a1, hilp_b1, hilp_b2,
        hilp_c0, hilp_c1, hilp_d1, hilp_d2,
        hilp_e0, hilp_e1, hilp_f1, hilp_f2,
        hilp_aL1, hilp_aL2, hilp_aR1, hilp_aR2,
        hilp_cL1, hilp_cL2, hilp_cR1, hilp_cR2,
        hilp_eL1, hilp_eL2, hilp_eR1, hilp_eR2,
        hilp_total
    };
    double highpass[hilp_total];
    double lowpass[hilp_total];
    
    enum {
        pvAL1, pvSL1, accSL1, acc2SL1,
        pvAL2, pvSL2, accSL2, acc2SL2,
        pvAL3, pvSL3, accSL3,
        pvAL4, pvSL4,
        gndavgL, outAL, gainAL,
        pvAR1, pvSR1, accSR1, acc2SR1,
        pvAR2, pvSR2, accSR2, acc2SR2,
        pvAR3, pvSR3, accSR3,
        pvAR4, pvSR4,
        gndavgR, outAR, gainAR,
        air_total
    };
    double air[air_total];
    
    enum {
        prevSampL1, prevSlewL1, accSlewL1,
        prevSampL2, prevSlewL2, accSlewL2,
        prevSampL3, prevSlewL3, accSlewL3,
        kalGainL, kalOutL,
        prevSampR1, prevSlewR1, accSlewR1,
        prevSampR2, prevSlewR2, accSlewR2,
        prevSampR3, prevSlewR3, accSlewR3,
        kalGainR, kalOutR,
        kal_total
    };
    double kal[kal_total];
    double fireCompL;
    double fireCompR;
    double fireGate;
    double stoneCompL;
    double stoneCompR;
    double stoneGate;
    double airGainA;
    double airGainB;
    double fireGainA;
    double fireGainB;
    double stoneGainA;
    double stoneGainB;
    
    enum {
        biqs_freq, biqs_reso, biqs_level,
        biqs_nonlin, biqs_temp, biqs_dis,
        biqs_a0, biqs_a1, biqs_b1, biqs_b2,
        biqs_c0, biqs_c1, biqs_d1, biqs_d2,
        biqs_e0, biqs_e1, biqs_f1, biqs_f2,
        biqs_aL1, biqs_aL2, biqs_aR1, biqs_aR2,
        biqs_cL1, biqs_cL2, biqs_cR1, biqs_cR2,
        biqs_eL1, biqs_eL2, biqs_eR1, biqs_eR2,
        biqs_outL, biqs_outR, biqs_total
    };
    double high[biqs_total];
    double hmid[biqs_total];
    double lmid[biqs_total];
    double bass[biqs_total];
    
    double dBaL[dscBuf+5];
    double dBaR[dscBuf+5];
    double dBaPosL;
    double dBaPosR;
    int dBaXL;
    int dBaXR;
    
    double panA;
    double panB;
    double inTrimA;
    double inTrimB;
    
    uint32_t fpdL;
    uint32_t fpdR;
    //default stuff
    
    double peakLeft = 0.0;
    double peakRight = 0.0;
    double slewLeft = 0.0;
    double slewRight = 0.0;
    double rmsLeft = 0.0;
    double rmsRight = 0.0;
    double previousLeft = 0.0;
    double previousRight = 0.0;
    double zeroLeft = 0.0;
    double zeroRight = 0.0;
    double longestZeroLeft = 0.0;
    double longestZeroRight = 0.0;
    double maxFireComp = 1.0;
    double maxStoneComp = 1.0;
    double maxFireGate = 1.0;
    double maxStoneGate = 1.0;
    double baseFire = 0.0;
    double baseStone = 0.0;
    bool wasPositiveL = false;
    bool wasPositiveR = false;
    int rmsCount = 0;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
