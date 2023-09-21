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

// Describes sound available for synthesizer to use, synthvoice does the actual playback.
// This is only a header file, because we just need to implement pure virtual functions
// for using midi note input and define a midi channel
class NoiseSynthSound : public juce::SynthesiserSound
{
public:
    // Declare that sound should be played when a given midi note is pressed
    bool appliesToNote(int /*midiNoteNumber*/) override { return true; }

    // Declare that sound should be played on a given midi channel
    bool appliesToChannel(int /*midiChannel*/) override { return true; }
};