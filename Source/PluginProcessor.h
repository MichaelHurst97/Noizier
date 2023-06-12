/*
 * Noizier - a simple noise generator, built by Michael Hurst
 *
 * Copyright 2023, with code adaptions based on the JUCE Documentation
 * and Joshua Hodge (The Audio Programmer), as described in the GitHub
 * https://github.com/MichaelHurst97/Noizier
 *
 * Noizier is released under the GNU General Public Licence v3
 * or later (GPL-3.0-or-later). The license is found in the "LICENSE"
 * file in the root of this repository, or at
 * https://www.gnu.org/licenses/gpl-3.0.en.html
 */

#pragma once

#include <JuceHeader.h>
#include "NoiseSynthVoice.h"
#include "NoiseSynthSound.h"
#include "ModellFilter.h"
#include "ModellGain.h"
#include "ModellPanner.h"
#include "ModellFilterNotes.h"

// Plugin Processor that acts as the starting point.
// This is the "Brains" of the plugin that does the actual audio processing / playback
// and handles plugin parameters.
class NoisePluginAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    NoisePluginAudioProcessor();
    ~NoisePluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    void processBlockBypassed (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // APVTS to manage AudioProcessors entire state including Parameters. 
    // Used for creating, storing and restoring parameters that can be changed by the ui or programm
    juce::AudioProcessorValueTreeState apvts;
    

private:
    // Function for plugin parameter creation
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // Create synthesizer that will use our synthesiser voice, noise and dsp
    juce::Synthesiser noiseSynth;

    // Create filters to apply to whole audio signal at the end of synth audio generation
    ModellFilter filter;

    // Create filter that uses the frequency of the currently playing note as cutoff
    ModellFilterNotes filterNotes;

    // Panner for stereo manipulation
    ModellPanner panner;

    // Apply gain as the last step in audio processing
    ModellGain gain;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoisePluginAudioProcessor)
};
