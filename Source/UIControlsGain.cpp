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
#include "UIControlsGain.h"

UIControlsGain::UIControlsGain(juce::AudioProcessorValueTreeState& apvts)
    : sliderGain(apvts, "gain", "labelText", "GAIN", "typeGain", "fontHeight", 25.0f, 0.4f),
    sliderPan(apvts, "panner", "labelText", "PAN", "typePan", "fontHeight", 8.0f, 0.0f)
{
    addAndMakeVisible(sliderGain);
    addAndMakeVisible(sliderPan);
}

UIControlsGain::~UIControlsGain()
{
}

void UIControlsGain::resized()
{
    auto width = getWidth();
    auto height = getHeight();

    // Scale off parents bounds
    int sliderBoundsWidth = (int)(width * 0.7f);
    int sliderBoundsHeight = (int)(height * 0.7f);
    int sliderBoundsWidthPan = (int)(sliderBoundsWidth / 3.2f);
    int sliderBoundsHeightPan = (int)(sliderBoundsHeight / 3.2f);

    auto widthOffsetCurrent = (width - sliderBoundsWidth) / 6;
    auto heightOffsetCurrent = (height - sliderBoundsHeight) / 6;

    // Initial bounds setting
    sliderGain.setBounds(getLocalBounds().withSizeKeepingCentre(sliderBoundsWidth, sliderBoundsHeight));
    sliderPan.setBounds(width - (sliderBoundsHeightPan) - widthOffsetCurrent, heightOffsetCurrent, sliderBoundsWidthPan, sliderBoundsHeightPan);

    // Need to call this, because resized() by slider object doesnt get called automatically when parent resizes,
    // also new bounds relative to parent object must be passed, so we scale off this objects width and height
    sliderGain.updateBounds(sliderBoundsWidth, sliderBoundsHeight);
    sliderPan.updateBounds(sliderBoundsWidthPan, sliderBoundsHeightPan);
}