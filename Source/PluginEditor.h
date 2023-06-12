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
#include "PluginProcessor.h"
#include "UICustomLookAndFeel.h"
#include "UIComponentAnimatedBackground.h"
#include "UIControlsADSR.h"
#include "UIControlsGain.h"
#include "UIControlsPlugin.h"
#include "UIControlsLFO.h"
#include "UIControlsFilter.h"

// Plugin Editor thats responsible for the UI
class NoisePluginAudioProcessorEditor  : public juce::AudioProcessorEditor 
{
public:
    NoisePluginAudioProcessorEditor(NoisePluginAudioProcessor&);
    ~NoisePluginAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for the editor to access the processor object that created it
    // Used for getting parameter states
    NoisePluginAudioProcessor& audioProcessor;

    UIComponentAnimatedBackground animatedBackground;

    UIControlsADSR uiControlsADSR;
    UIControlsLFO uiControlsLFO;
    UIControlsPlugin uiControlsPlugin;
    UIControlsGain uiControlsGain;
    UIControlsFilter uiControlsFilter;

    CustomLookAndFeel customLook;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoisePluginAudioProcessorEditor);
};
