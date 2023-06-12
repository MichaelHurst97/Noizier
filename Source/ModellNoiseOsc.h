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

// Oscillator that uses various functions for generating noise
class ModellNoiseOsc : public juce::dsp::Oscillator<float>
{
public:
    void setNoiseType(const int noiseType);

private:
    juce::Random random;
    float b0 = 0.0f, b1 = 0.0f, b2 = 0.0f, white = 0.0f;
};