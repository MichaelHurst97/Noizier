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

#include "ModellNoiseOsc.h"

void ModellNoiseOsc::setNoiseType(const int noiseType)
{
    switch (noiseType)
    {
        // White Noise Lambda function for creating a noise oscillator. Return function sets the type, e.g. for sinewave use return std::sin(x)
        // random.nextFloat() always generates values around 0 and 1, so we equate that this should happen around +1 and -1
        case 0:
            initialise([this](float /*x*/) 
            { 
                return random.nextFloat() * 2.0f - 1.0f; 
            });
            break;

        // Approximation of Pink Noise from here https://www.firstpr.com.au/dsp/pink-noise/
        case 1:
            initialise([this](float x)
            {
                white = random.nextFloat() * 2.0f - 1.0f;
                b0 = 0.99765f * b0 + white * 0.0990460f;
                b1 = 0.96300f * b1 + white * 0.2965164f;
                b2 = 0.57000f * b2 + white * 1.0526913f;
                x = b0 + b1 + b2 + white * 0.1848f;
                return x / 5; // Dividing by 5 here leaves us with approximately the same displayed volume as the white noise
            });
            break;
        
        // Comb-ish noise by limiting the lookuptable size
        case 2:
            initialise([this](float /*x*/)
            {
                return random.nextFloat() * 2.0f - 1.0f;
            }, 100);
            break;

        default:
            jassertfalse;
            break;
    }
}