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
#include "UIComponentAnimatedBackground.h"

UIComponentAnimatedBackground::UIComponentAnimatedBackground(juce::AudioProcessorValueTreeState& apvts)
    : parameters(apvts), gainParameterValue(apvts.getRawParameterValue("gain")->load())
{
    // Attaches a callback to the gain parameter, which will call our listener when the gain value changes
    parameters.addParameterListener("gain", this);
}

UIComponentAnimatedBackground::~UIComponentAnimatedBackground()
{
}

void UIComponentAnimatedBackground::paint(juce::Graphics& g)
{
    // Fill the whole window from white to lightgray when the gain parameter is turned up
    g.fillAll(juce::Colours::white);

    g.fillAll(juce::Colour(0.0f, 0.0f, 0.9f, gainParameterValue));

    // Turning up the Gain parameter creates random pixels that should look like noise
    // Note: Fixing the seed will make the redraw static and the pixel values won't move around
    // Note: The performance impact of this visual effect should be neglactable when 
    // pixels are only repainted once the callback from the gain parameter happens
    random.setSeed((juce::int64)(gainParameterValue * 100));

    auto width = getWidth();
    auto height = getHeight();
    auto pixelSize = 8;

    for (int i = 0; i < 1500; i++) {
        // Crudely round to an int divisible by the pixel height and width so we get an aligned raster of pixels
        int a = random.nextInt(width) / pixelSize * pixelSize;
        int b = random.nextInt(height) / pixelSize * pixelSize;

        float opacity = gainParameterValue - random.nextFloat();
        g.setColour(juce::Colour(uint8_t(0), uint8_t(0), uint8_t(0), opacity));

        g.fillRect(a, b, pixelSize, pixelSize);
    }
}

void UIComponentAnimatedBackground::resized()
{
}

// Register when the listener gets a callback from the gain parameter and pass it's value, then repaint
void UIComponentAnimatedBackground::parameterChanged(const juce::String& /*parameterID*/, float newValue)
{
    gainParameterValue = newValue;
    repaint();
}