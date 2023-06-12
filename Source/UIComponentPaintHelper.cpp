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

#include <JuceHeader.h>
#include "UIComponentPaintHelper.h"

UIComponentPaintHelper::UIComponentPaintHelper()
{
}

UIComponentPaintHelper::~UIComponentPaintHelper()
{
}

void UIComponentPaintHelper::paint(juce::Graphics& g)
{
    g.setColour(backgroundColour);
    g.fillRect(getLocalBounds().toFloat());
    g.setColour(juce::Colours::black);
    g.drawRect(getLocalBounds().toFloat(), 3.0f);
}

void UIComponentPaintHelper::resized()
{
}

// Used if the panel that inherits from this class should be differently coloured
void UIComponentPaintHelper::setBackgroundColour(juce::Colour colour)
{
    backgroundColour = colour;
}