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

#include "PluginProcessor.h"
#include "PluginEditor.h"

NoisePluginAudioProcessorEditor::NoisePluginAudioProcessorEditor (NoisePluginAudioProcessor& p)
    : AudioProcessorEditor (&p), 
    audioProcessor (p), 
    animatedBackground(audioProcessor.apvts),
    uiControlsADSR(audioProcessor.apvts),
    uiControlsLFO(audioProcessor.apvts),
    uiControlsPlugin(audioProcessor.apvts),
    uiControlsGain(audioProcessor.apvts),
    uiControlsFilter(audioProcessor.apvts)
{
    // Set editors size, resizing limits and fix the aspect ratio
    setResizable(true, false);
    setSize(600, 320);
    setResizeLimits(600, 320, 900, 480);
    getConstrainer()->setFixedAspectRatio(1.875);
    juce::Desktop::getInstance().setGlobalScaleFactor(1.25);

    addAndMakeVisible(animatedBackground);

    addAndMakeVisible(uiControlsADSR);
    addAndMakeVisible(uiControlsLFO);
    addAndMakeVisible(uiControlsPlugin);
    addAndMakeVisible(uiControlsGain);
    addAndMakeVisible(uiControlsFilter);
    

    // Assign custom look / "skin"
    // Not needed because LookAndFeel::setDefaultLookAndFeel() is already
    // called upon l&f object creation in header
    //setLookAndFeel(&customLook);
}

NoisePluginAudioProcessorEditor::~NoisePluginAudioProcessorEditor()
{
    // Make sure that a LookAndFeel object is not used on shutdown
    setLookAndFeel(nullptr);
}

void NoisePluginAudioProcessorEditor::paint (juce::Graphics& /*g*/)
{
    // We don't need to paint a background here, because the animated background component takes care of this
    // All sub components paint their knobs and panels individually
}

void NoisePluginAudioProcessorEditor::resized()
{
    // Positions of any subcomponents in the editor
    auto width = getWidth();
    auto height = getHeight();
    auto spacing = getWidth() / 60;

    animatedBackground.setBounds(0, 0, width, height);

    uiControlsADSR.setBounds(spacing, spacing, 
        ((width / 3) * 2) - spacing * 2, height - spacing * 22);

    uiControlsLFO.setBounds(uiControlsADSR.getRight() + spacing, spacing, 
        (width / 3) - spacing, height - spacing * 22);

    uiControlsPlugin.setBounds(spacing, uiControlsADSR.getBottom() + spacing,
        (width / 3) - (int)(spacing * 1.5f), height - uiControlsADSR.getBottom() - spacing * 2);

    uiControlsGain.setBounds(uiControlsPlugin.getRight() + spacing, uiControlsADSR.getBottom() + spacing,
        (width / 3) - (int)(spacing * 1.5f), height - uiControlsADSR.getBottom() - spacing * 2);

    uiControlsFilter.setBounds(uiControlsGain.getRight() + spacing, uiControlsADSR.getBottom() + spacing,
        (width / 3) - spacing, height - uiControlsADSR.getBottom() - spacing * 2);
}