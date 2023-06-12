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

// Acts as a container class for the other UI components to be drawn and therefore only implements basic paint functionality.
class UIComponentPaintHelper : public juce::Component
{
public:
    UIComponentPaintHelper();
    ~UIComponentPaintHelper() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void setBackgroundColour(juce::Colour colour);

private:
    juce::Colour backgroundColour = juce::Colours::white;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UIComponentPaintHelper)
};