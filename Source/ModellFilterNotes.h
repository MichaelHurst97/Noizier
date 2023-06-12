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

 // Lowpass Filter that uses the frequency of the currently playing note as cutoff
class ModellFilterNotes
{
public:
    ModellFilterNotes();

    void prepareToPlay(double sampleRate, int samplesPerBlock, int numOutputChannels);
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer);
    void updateParameters(const float midiHertzInput);
    void reset();

private:
    std::array<juce::dsp::StateVariableTPTFilter<float>, 3> filters = 
    {
        juce::dsp::StateVariableTPTFilter<float>(), // filterLP1
        juce::dsp::StateVariableTPTFilter<float>(), // filterLP2
        juce::dsp::StateVariableTPTFilter<float>()  // filterLP3
    };

    float midiHertz;
    float midiHertzPrevious;
};