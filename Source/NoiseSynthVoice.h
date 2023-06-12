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
#include "NoiseSynthSound.h"
#include "ModellADSR.h"
#include "ModellNoiseOsc.h"
#include "ModellFilter.h"

// Generates the synthesiser sound.
// Each instance of synthesizer voice plays back a synthesizer noise, because synthesizer can be polyphonic
class NoiseSynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newValue) override;
    void controllerMoved(int controllerNumber, int newValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int numOutputChannels);
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

    void updateParameters(const float attack, const float decay, const float sustain, const float release, 
        const int noiseType, const float lfoFrequencyInput, const bool lfoSwitchInput);

private:
    ModellNoiseOsc dspOsc;

    ModellADSR adsr;

    float lfoFrequency;
    bool lfoSwitch;
    float previousLFOGain;
    juce::dsp::Oscillator<float> lfo;
};
