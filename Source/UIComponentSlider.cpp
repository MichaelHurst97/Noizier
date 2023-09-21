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
#include "UIComponentSlider.h"

UIComponentSlider::UIComponentSlider(juce::AudioProcessorValueTreeState& apvts, juce::String parameterID, 
    juce::String labelID, juce::String labelText, juce::String labelType, juce::String fontHeightID, 
    float fontHeight, double doubleClickValue)
{
    // Add parameter attachments
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, parameterID, slider);

    // Set Basic Slider attributes
    slider.setComponentID(labelID);
    slider.getProperties().set(slider.getComponentID(), labelText);
    slider.getProperties().set("labelType", labelType);
    slider.setComponentID(fontHeightID);
    slider.getProperties().set(slider.getComponentID(), fontHeight);
    slider.setDoubleClickReturnValue(true, doubleClickValue);
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    addAndMakeVisible(slider);
}

void UIComponentSlider::resized()
{
    slider.setBounds(0, 0, sliderBoundsWidth, sliderBoundsHeight);
}

// Gets called from parent that uses this class. Executed in parents resized function.
// Resizing the parent doesnt resize the slider automatically, so this workaround exists
void UIComponentSlider::updateBounds(int newSliderBoundsWidth, int newSliderBoundsHeight)
{
    sliderBoundsWidth = newSliderBoundsWidth;
    sliderBoundsHeight = newSliderBoundsHeight;
    resized();
}
