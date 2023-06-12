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
#include "UIComponentPainthelper.h"
#include "UIComponentSlider.h"

// ADSR Panel for UI
class UIControlsADSR : public UIComponentPaintHelper
{
public:
    UIControlsADSR(juce::AudioProcessorValueTreeState& apvts);
    ~UIControlsADSR() override;

    void resized() override;

private:
    UIComponentSlider sliderAttack, sliderDecay, sliderSustain, sliderRelease;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UIControlsADSR)
};