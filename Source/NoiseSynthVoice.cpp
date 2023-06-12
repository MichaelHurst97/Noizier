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

#include "NoiseSynthVoice.h"

bool NoiseSynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    // Check if input sound is correctly casting as SynthesiserSound
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void NoiseSynthVoice::startNote(int midiNoteNumber, float /*velocity*/, juce::SynthesiserSound* /*sound*/,
    int /*currentPitchWheelPosition*/)
{
    // Get frequency to oscillate to.
    // Shouldn't matter sound wise, because all frequencies are distributed equally in white noise
    dspOsc.setFrequency((float)juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber));

    // Apply envelope to note playing, meaning starting the attack phase of adsr envelope
    adsr.noteOn();

    // Important to reset the wave position of the lfo to 0 every time a note starts.
    // Otherwise the lfo would continue to loop and we almost always start a note somewhere in the middle of the lfo wave.
    // This functionality is the reason why the lfo is implemented in here
    lfo.reset();
}

void NoiseSynthVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
    // Starts the release phase of the adsr envelope
    adsr.noteOff();

    // Clear note when adsr stops
    // Tailoff here means that a slight fadeout is given to the voice to avoid clicking when note finishes
    if (!allowTailOff || !adsr.isActive()) clearCurrentNote();
}

void NoiseSynthVoice::pitchWheelMoved(int /*newValue*/)
{
}

void NoiseSynthVoice::controllerMoved(int /*controllerNumber*/, int /*newValue*/)
{
}

void NoiseSynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int numOutputChannels)
{
    // Create specification object for dsp, so it knows about the context it's being called
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numOutputChannels;

    // Pass specifications like samplerate for processing to the dsp modules prepare() function
    dspOsc.prepare(spec);

    // ADSR setup
    adsr.setSampleRate(sampleRate);

    // LFO setup
    // To not update the lfo every sample, do it 100 times less. Otherwise there would be a heavy delay when changing the lfo frequency
    lfo.prepare({ spec.sampleRate / 100, spec.maximumBlockSize, spec.numChannels });
    // Use a triangle wave for lfo modulation
    lfo.initialise([](float x) { return 1 - 2 * abs(x / juce::MathConstants<float>::pi); });
    lfo.setFrequency(10.0f);
    previousLFOGain = 0.0f;
}

void NoiseSynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    // Check if the voice is currently silent, if so it shouldnt return anything
    if (!isVoiceActive()) return;
    
    // Create AudioBlock for dsp so it can work with that: AudioBlock is an alias for AudioBuffer
    juce::dsp::AudioBlock<float> dspAudioBlock{ outputBuffer };

    // Process oscillator via buffer -> play the noise. 
    // Replaces contents of dsps AudioBlock / AudioBuffer
    dspOsc.process(juce::dsp::ProcessContextReplacing<float>(dspAudioBlock));

    // Apply adsr envelope to the AudioBuffer.
    // As dsps AudioBlock is an alias for outputBuffer, we can call outputBuffer here
    adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);

    // Apply the LFO amplitude modulation
    if (lfoSwitch) 
    {
        lfo.setFrequency(lfoFrequency);
        auto lfoOutput = lfo.processSample(0.0f);
        auto currentGain = juce::jmap(lfoOutput, -1.0f, 1.0f, 0.0f, 1.0f);

        // Gain ramp for smooth gain changes
        if (juce::approximatelyEqual(currentGain, previousLFOGain))
        {
            outputBuffer.applyGain(currentGain);
        }
        else
        {
            outputBuffer.applyGainRamp(0, outputBuffer.getNumSamples(), previousLFOGain, currentGain);
            previousLFOGain = currentGain;
        }
    }

    // If the sound that the voice is playing finishes during the rendered block, clear the current note to let the synthesiser know it has finished
    if (!adsr.isActive()) clearCurrentNote();
}

// Gets called from processor
void NoiseSynthVoice::updateParameters(const float attack, const float decay, const float sustain, const float release, 
    const int noiseType, const float lfoFrequencyInput, const bool lfoSwitchInput)
{
    // Update ADSR Modell data
    adsr.updateADSR(attack, decay, sustain, release);
    dspOsc.setNoiseType(noiseType);

    // Update lfo frequency and check if the lfo should be applied
    lfoFrequency = lfoFrequencyInput * 10;
    lfoSwitch = lfoSwitchInput;
}