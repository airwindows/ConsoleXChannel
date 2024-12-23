// AirwindowsUI by Chris Johnson
// Initial seed code for the meter created by Paul Walker on 8/23/21.
#include "AirwindowsUI.h"

void AirwindowsLookAndFeel::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider) {
    auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat();
    auto bevelW = sqrt((float)width);
    if (slider.isHorizontal()) bevelW = sqrt((float)height);
    auto lineW = sqrt(bevelW)*0.618f;
    auto trackWidth = bevelW;
    //basic variables we'll be using for our controls
    
    juce::Path backgroundTrack;
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.75f)); //highlight
    backgroundTrack.startNewSubPath((slider.isHorizontal()?(float)x:(float)x+(float)width*0.5f)+(lineW*0.5f), (slider.isHorizontal()?(float)y+(float)height*0.5f:(float)((height*0.97f)+y))+(lineW*0.5f));
    backgroundTrack.lineTo ((slider.isHorizontal()?(float)(width+x):(float)x+(float)width*0.5f)+(lineW*0.5f), (slider.isHorizontal()?(float)y+(float)height*0.5f:(float)y)+(lineW*0.5f));
    g.strokePath (backgroundTrack, {trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });
    backgroundTrack.clear();
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.75f)); //shadow
    backgroundTrack.startNewSubPath((slider.isHorizontal()?(float)x:(float)x+(float)width*0.5f)-lineW, (slider.isHorizontal()?(float)y+(float)height*0.5f:(float)((height*0.97f)+y))-lineW);
    backgroundTrack.lineTo ((slider.isHorizontal()?(float)(width+x):(float)x+(float)width*0.5f)-lineW, (slider.isHorizontal()?(float)y+(float)height*0.5f:(float)y)-lineW);
    g.strokePath (backgroundTrack, {trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });
    backgroundTrack.clear();
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId)); //inside slot in which the slider goes
    backgroundTrack.startNewSubPath((slider.isHorizontal()?(float)x:(float)x+(float)width*0.5f), (slider.isHorizontal()?(float)y+(float)height*0.5f:(float)((height*0.97f)+y)));
    backgroundTrack.lineTo ((slider.isHorizontal()?(float)(width+x):(float)x+(float)width*0.5f), (slider.isHorizontal()?(float)y+(float)height*0.5f:(float)y));
    g.strokePath (backgroundTrack, {trackWidth*0.618f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });
    //draw the slot from which the slider moves. Note that we leave a bit of space on the bottom to show the label:
    
    g.setFont(juce::Font(newFont, g.getCurrentFont().getHeight(), 0));
    g.setFont ((((lineW+bevelW)*30.0f) / (float)g.getCurrentFont().getHeight()));
    if (slider.isHorizontal()) bounds.removeFromBottom((bounds.getHeight()*0.5f)-(bevelW*3.0f));
    else bounds.removeFromBottom(-30.0f);
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.75f)); //highlight
    g.drawFittedText(slider.getName(), juce::Rectangle<int>((int)(bounds.getWidth()+lineW),(int)(bounds.getHeight()+lineW)), juce::Justification::centredBottom, 1);
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.75f)); //shadow
    g.drawFittedText(slider.getName(), juce::Rectangle<int>((int)(bounds.getWidth()-lineW),(int)(bounds.getHeight()-lineW)), juce::Justification::centredBottom, 1);
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.25f)); //text inside emboss
    g.drawFittedText(slider.getName(), juce::Rectangle<int>((int)bounds.getWidth(),(int)bounds.getHeight()), juce::Justification::centredBottom, 1);
    //This is the drawing of the text under the slider, to allow the slider to obscure it. Sliders are designed to be packed pretty tightly,
    //but the horizontal ones can still have a lot of text. To control their bulk, narrow the slot they're in.

    juce::Point<float> maxPoint = {slider.isHorizontal()?(sliderPos*0.94f)+(width*0.025f):((float)x+(float)width*0.5f), slider.isHorizontal()?((float)y+(float)height*0.5f):(sliderPos*0.94f)+(height*0.025f)};
    auto thumbWidth = bevelW*2.9f;
    auto rectSlider = juce::Rectangle(thumbWidth*1.618f, thumbWidth).withCentre(maxPoint);
    if (slider.isHorizontal()) rectSlider = juce::Rectangle(thumbWidth, thumbWidth*1.618f).withCentre(maxPoint);
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId)); g.setOpacity(1.0f); g.fillRoundedRectangle (rectSlider, bevelW);
    //solid background for knob so you can't see the track under it
    juce::ColourGradient cg = juce::ColourGradient(juce::Colours::white, rectSlider.getTopLeft(), juce::Colours::black, rectSlider.getBottomRight(),false);
    cg.addColour(0.2f, juce::Colours::white); cg.addColour(0.618f, juce::Colours::transparentBlack); cg.addColour(0.9f, juce::Colours::black); cg.isRadial = true;
    g.setGradientFill(cg);
    auto inset = rectSlider; inset.reduce(bevelW*0.25f, bevelW*0.25f);
    g.drawRoundedRectangle (inset, bevelW*0.8f, bevelW*0.5f);
    cg = juce::ColourGradient(juce::Colours::transparentWhite, rectSlider.getTopLeft(), juce::Colours::black, rectSlider.getBottomRight(),false);
    cg.addColour(0.0955f, juce::Colours::white); cg.addColour(0.382f, slider.findColour (juce::ResizableWindow::backgroundColourId)); cg.addColour(0.618f, slider.findColour (juce::ResizableWindow::backgroundColourId)); cg.isRadial = true;
    g.setGradientFill(cg); inset.reduce(bevelW*0.25f, bevelW*0.25f); g.drawRoundedRectangle (inset, bevelW*0.9f, bevelW*0.382f);
    cg = juce::ColourGradient(juce::Colours::transparentWhite, rectSlider.getTopLeft(), juce::Colours::transparentBlack, rectSlider.getBottomRight(),false);
    cg.addColour(0.04775f, juce::Colours::transparentWhite); cg.addColour(0.382f, slider.findColour (juce::ResizableWindow::backgroundColourId)); cg.addColour(0.618f, slider.findColour (juce::ResizableWindow::backgroundColourId)); cg.isRadial = true;
    g.setGradientFill(cg); inset.reduce(bevelW*0.382f, bevelW*0.382f); g.drawRoundedRectangle (inset, bevelW, bevelW*0.618f);
    g.setColour (juce::Colours::black); g.drawRoundedRectangle (rectSlider, bevelW, lineW);
    //This is the outside area of the slider knob, with the shading/highlighting that renders the 3D effect.
    
    float thumbScale = 0.85f; rectSlider = juce::Rectangle<float> (thumbWidth*thumbScale, thumbWidth*thumbScale).withCentre (maxPoint);
    rectSlider = juce::Rectangle<float> (thumbWidth*thumbScale, thumbWidth*thumbScale).withCentre (maxPoint);
    g.setColour (slider.findColour (juce::Slider::thumbColourId)); g.fillEllipse (rectSlider);
    cg = juce::ColourGradient(juce::Colours::white, rectSlider.getBottomRight(), juce::Colours::black, rectSlider.getTopLeft(),false);
    cg.addColour(0.191f, juce::Colours::white); cg.addColour(0.382f, slider.findColour (juce::Slider::thumbColourId)); cg.addColour(0.618f, slider.findColour (juce::Slider::thumbColourId)); cg.isRadial = true;
    g.setGradientFill(cg);
    inset = rectSlider; inset.reduce(bevelW*0.382f, bevelW*0.382f);
    g.drawEllipse (inset, bevelW*0.5f);
    cg = juce::ColourGradient(juce::Colours::white, rectSlider.getBottomRight(), juce::Colours::black, rectSlider.getTopLeft(),false);
    cg.addColour(0.0955f, juce::Colours::transparentWhite); cg.addColour(0.382f, slider.findColour (juce::Slider::thumbColourId)); cg.addColour(0.618f, slider.findColour (juce::Slider::thumbColourId)); cg.isRadial = true;
    g.setGradientFill(cg);
    inset.reduce(bevelW*0.125f, bevelW*0.125f);
    g.drawEllipse (inset, bevelW*0.5f); g.setColour (juce::Colours::black); g.drawEllipse (rectSlider, lineW);
    //This is the thumb of the knob, allowing a custom color to the thumb
}


void AirwindowsLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) {
    auto tilt = (slider.findColour (juce::Slider::backgroundColourId).getFloatRed()-0.5f)*1.236f;
    auto textbounds = juce::Rectangle<int> (x, y, width, height).toFloat();
    auto bounds = textbounds; bounds.removeFromBottom(4.0f+(height*0.12f)+(height*tilt*tilt*0.12f)+(height*tilt*fabs(tilt)*0.12f)); bounds.reduce(1.0f, 1.0f);
    float scaleHeight = 1.0f-(fabs(tilt)); //proportion of vertical height relative to horizontal
    float trimscaleHeight = scaleHeight + (tilt*0.05f);
    float radius = bounds.getWidth()*0.5f; if (radius > (bounds.getHeight()/scaleHeight)*0.5f) radius = (bounds.getHeight()/scaleHeight)*0.5f;
    auto gradientSquare = juce::Rectangle(bounds.getCentreX()-radius, bounds.getCentreY()-(radius*sqrt(scaleHeight)), radius*2.0f, radius*sqrt(trimscaleHeight)*2.0f).toFloat();
    auto square = juce::Rectangle(bounds.getCentreX()-radius, bounds.getCentreY()-(radius*scaleHeight), radius*2.0f, radius*trimscaleHeight*2.0f).toFloat();
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto bevelW = sqrt(radius*0.5f)*1.618f;
    auto lineW = sqrt(bevelW)*0.55f;
    //basic variables we'll be using for our controls
    
    juce::ColourGradient cg = juce::ColourGradient(juce::Colours::white, gradientSquare.getTopLeft(), juce::Colours::black, gradientSquare.getBottomRight(),false);
    cg.addColour(0.2f, juce::Colours::white); cg.addColour(0.618f, juce::Colours::transparentBlack); cg.addColour(0.9f, juce::Colours::black); cg.isRadial = true;
    g.setGradientFill(cg);
    auto inset = square; inset.reduce(bevelW*0.25f, bevelW*0.25f); inset.removeFromTop(-bevelW*tilt*fabs(tilt+0.15f)*0.618f);
    g.drawEllipse (inset, bevelW*0.5f);
    cg = juce::ColourGradient(juce::Colours::transparentWhite, gradientSquare.getTopLeft(), juce::Colours::black, gradientSquare.getBottomRight(),false);
    cg.addColour(0.0955f, juce::Colours::white); cg.addColour(0.382f, slider.findColour (juce::ResizableWindow::backgroundColourId)); cg.addColour(0.618f, slider.findColour (juce::ResizableWindow::backgroundColourId)); cg.isRadial = true;
    g.setGradientFill(cg);
    inset.reduce(bevelW*0.25f, bevelW*0.25f); inset.removeFromTop(-bevelW*tilt*fabs(tilt+0.15f)*0.618f);
    g.drawEllipse (inset, bevelW*0.382f);
    cg = juce::ColourGradient(juce::Colours::transparentWhite, gradientSquare.getTopLeft(), juce::Colours::transparentBlack, gradientSquare.getBottomRight(),false);
    cg.addColour(0.04775f, juce::Colours::transparentWhite); cg.addColour(0.382f, slider.findColour (juce::ResizableWindow::backgroundColourId)); cg.addColour(0.618f, slider.findColour (juce::ResizableWindow::backgroundColourId)); cg.isRadial = true;
    g.setGradientFill(cg);
    inset.reduce(bevelW*0.382f, bevelW*0.382f); inset.removeFromTop(-bevelW*tilt*fabs(tilt+0.15f)*0.618f);
    g.drawEllipse (inset, bevelW*0.618f); g.setColour (juce::Colours::black); g.drawEllipse (square, lineW);
    //This is the outside circle of the knob, with the shading/highlighting that renders the 3D effect. Tilting of the knob is included
    
    g.setFont(juce::Font(newFont, g.getCurrentFont().getHeight(), 0));
    g.setFont ((((lineW+bevelW)*30.0f) / (float)g.getCurrentFont().getHeight()) + (tilt*0.25f));
    auto padHeight = (sqrt(bevelW)*-2.0f) + ((bounds.getHeight())-(bounds.getWidth()*scaleHeight)) + (tilt*bevelW) + (fabs(tilt)*-bevelW); if (padHeight < 0.0) padHeight = 0.0;
    textbounds.removeFromBottom(padHeight*0.618f);
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.75f)); //highlight
    g.drawFittedText(slider.getName(), juce::Rectangle<int>((int)(textbounds.getWidth()+lineW),(int)(textbounds.getHeight()+lineW)), juce::Justification::centredBottom, 1);
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.75f)); //shadow
    g.drawFittedText(slider.getName(), juce::Rectangle<int>((int)(textbounds.getWidth()-lineW),(int)(textbounds.getHeight()-lineW)), juce::Justification::centredBottom, 1);
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.25f)); //text inside emboss
    g.drawFittedText(slider.getName(), juce::Rectangle<int>((int)textbounds.getWidth(),(int)textbounds.getHeight()), juce::Justification::centredBottom, 1);
    //This is the drawing of the text just under the knob, no matter what tilt setting is included and what the real bounding box is doing. That's why it's so tricky :)

    auto arcRadius = (radius-(bevelW*0.5f))*0.7f;
    auto thumbWidth = pow(bevelW,1.618f)*0.7f;
    juce::Point<float> thumbPoint (bounds.getCentreX()+(arcRadius*std::cos(toAngle-juce::MathConstants<float>::halfPi)),bounds.getCentreY()+(-tilt*bevelW)+(arcRadius*trimscaleHeight*std::sin(toAngle-juce::MathConstants<float>::halfPi)));
    float thumbScale = 1.0f+(std::sin(toAngle-juce::MathConstants<float>::halfPi)*tilt*fabs(tilt)*0.5f);
    square = juce::Rectangle<float> (thumbWidth*thumbScale, thumbWidth*trimscaleHeight*thumbScale).withCentre (thumbPoint);
    gradientSquare = juce::Rectangle<float> (thumbWidth*trimscaleHeight*thumbScale, thumbWidth*thumbScale).withCentre (thumbPoint);
    g.setColour (slider.findColour (juce::Slider::thumbColourId)); g.fillEllipse (square);
    cg = juce::ColourGradient(juce::Colours::white, gradientSquare.getBottomRight(), juce::Colours::black, gradientSquare.getTopLeft(),false);
    cg.addColour(0.191f, juce::Colours::white); cg.addColour(0.382f, slider.findColour (juce::Slider::thumbColourId)); cg.addColour(0.618f, slider.findColour (juce::Slider::thumbColourId)); cg.isRadial = true;
    g.setGradientFill(cg);
    inset = square; inset.reduce(bevelW*0.382f, bevelW*0.382f); inset.removeFromBottom(bevelW*tilt*fabs(tilt+0.15f)*0.125f);
    g.drawEllipse (inset, bevelW*0.5f);
    cg = juce::ColourGradient(juce::Colours::white, gradientSquare.getBottomRight(), juce::Colours::black, gradientSquare.getTopLeft(),false);
    cg.addColour(0.0955f, juce::Colours::transparentWhite); cg.addColour(0.382f, slider.findColour (juce::Slider::thumbColourId)); cg.addColour(0.618f, slider.findColour (juce::Slider::thumbColourId)); cg.isRadial = true;
    g.setGradientFill(cg);
    inset.reduce(bevelW*0.125f, bevelW*0.125f); inset.removeFromBottom(bevelW*tilt*fabs(tilt+0.15f)*0.125f);
    g.drawEllipse (inset, bevelW*0.5f); g.setColour (juce::Colours::black); g.drawEllipse (square, lineW);
    //This is the thumb of the knob, also rendered on a tilt if needed, and allowing a custom color to the thumb
    
}

void AirwindowsMeter::paint(juce::Graphics &g) {
    g.fillAll(findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.8f)); //blank screen before doing anything, using the chart background black
    //this is probably quick and optimized
    
    float hype = 0.618033988749894f;
    float slewL = sqrt(inputSlewL)*300.0f;
    float slewR = sqrt(inputSlewR)*300.0f;
    float peakL = inputPeakL * 200.0f;
    float peakR = inputPeakR * 200.0f;
    if (peakL > 197.0f) peakL = 197.0f;
    if (peakR > 197.0f) peakR = 197.0f;
    float meterZeroL = (sqrt(inputZeroL)*6.0f)-6.0f;
    if (meterZeroL > 192.0f) meterZeroL = 192.0f;
    float meterZeroR = (sqrt(inputZeroR)*6.0f)-6.0f;
    if (meterZeroR > 192.0f) meterZeroR = 192.0f;
    int lightL = 16-(int)(inputPeakL*16.0f); if (lightL < 0) lightL = 0;
    int lightR = 16+(int)(inputPeakR*16.0f); if (lightR > 32) lightR = 32;
    float psDotHypeL = (11.0f * sqrt(inputRMSL * inputRMSR)) / (fabs(((peakL*((hype+6.0f)/7.0f))-slewL) * (7.0f/meterZeroL) )+1.0f);
    float psDotVibeL = sin(pow(fmin(inputPeakL*8.5f,6.18f) / (fabs(((peakL*((hype+4.0f)/5.0f))-slewL) * (7.0f/meterZeroL) )+1.0f),1.618f)*0.13f) * 3.141592f;
    float psDotSizeL = ((psDotVibeL*(1.0f-hype))+(psDotHypeL*hype))*(1.0f+(sin(hype*3.141592f)*0.25f));
    float psDotHypeR = (11.0f * sqrt(inputRMSL * inputRMSR)) / (fabs(((peakR*((hype+6.0f)/7.0f))-slewR) * (7.0f/meterZeroR) )+1.0f);
    float psDotVibeR = sin(pow(fmin(inputSlewR*8.5f,6.18f) / (fabs(((peakR*((hype+4.0f)/5.0f))-slewR) * (7.0f/meterZeroR) )+1.0f),1.618f)*0.13f) * 3.141592f;
    float psDotSizeR = ((psDotVibeR*(1.0f-hype))+(psDotHypeR*hype))*(1.0f+(sin(hype*3.141592f)*0.25f));
    
    if (psDotSizeL > 1.0f) {
        if (dataB[lightL] < 1.618f) dataB[lightL] = 1.618f;
        if (edgeB[lightL] < (psDotSizeL-1.0f)) edgeB[lightL] = (psDotSizeL-1.0f);
    } else {
        if (slewL > peakL) {
            if (dataG[lightL] < fmin((180.0f+(slewL-peakL))/256.0f,1.0f)) dataG[lightL] = fmin((180.0f+(slewL-peakL))/256.0f,1.0f);
            if (edgeG[lightL] < fmin(((180.0f-meterZeroL)+(slewL-peakL))/256.0f,1.0f)) edgeG[lightL] = fmin(((180.0f-meterZeroL)+(slewL-peakL))/256.0f,1.0f);
        } else {
            if (dataR[lightL] < ((255.0f-(peakL-slewL))/256.0f)) dataR[lightL] = ((255.0f-(peakL-slewL))/256.0f);
            if (edgeR[lightL] < ((meterZeroL-(peakL-slewL))/256.0f)) edgeR[lightL] = ((meterZeroL-(peakL-slewL))/256.0f);
        } //set COLOR for bar meter L side
    }
    
    if (psDotSizeR > 1.0f) {
        if (dataB[lightR] < 1.618f) dataB[lightR] = 1.618f;
        if (edgeB[lightR] < (psDotSizeR-1.0f)) edgeB[lightR] = (psDotSizeR-1.0f);
    } else {
        if (slewR > peakR) {
            if (dataG[lightR] < fmin((180.0f+(slewR-peakR))/256.0f,1.0f)) dataG[lightR] = fmin((180.0f+(slewR-peakR))/256.0f,1.0f);
            if (edgeG[lightR] < fmin(((180.0f-meterZeroR)+(slewR-peakR))/256.0f,1.0f)) edgeG[lightR] = fmin(((180.0f-meterZeroR)+(slewR-peakR))/256.0f,1.0f);
        } else {
            if (dataR[lightR] < ((255.0f-(peakR-slewR))/256.0f)) dataR[lightR] = ((255.0f-(peakR-slewR))/256.0f);
            if (edgeR[lightR] < ((meterZeroR-(peakR-slewR))/256.0f)) edgeR[lightR] = ((meterZeroR-(peakR-slewR))/256.0f);
        } //set COLOR for bar meter R side
    }
    
    auto linewidth = getWidth(); if (getHeight() > linewidth) linewidth = getHeight(); linewidth = (int)cbrt(linewidth/2)/2;
    if (meterInterpolation == 0) g.setImageResamplingQuality(g.lowResamplingQuality);
    if (meterInterpolation == 1) g.setImageResamplingQuality(g.mediumResamplingQuality);
    if (meterInterpolation == 2) g.setImageResamplingQuality(g.highResamplingQuality); //copy this section to other stretched-bitmap meters
    for (int count = 0; count < dataPoints; count++) {
        barImage.setPixelAt(count, 0, juce::Colour::fromFloatRGBA (edgeR[count], edgeG[count], fmin(edgeB[count],1.0f), 1.0f));
        barImage.setPixelAt(count, 1, juce::Colour::fromFloatRGBA (dataR[count], dataG[count], fmin(dataB[count],1.0f), 1.0f));
        barImage.setPixelAt(count, 2, juce::Colour::fromFloatRGBA (dataR[count], dataG[count], fmin(dataB[count],1.0f), 1.0f));
        barImage.setPixelAt(count, 3, juce::Colour::fromFloatRGBA (edgeR[count], edgeG[count], fmin(edgeB[count],1.0f), 1.0f));
        dataG[count] *= 0.92f; dataR[count] *= 0.92f; dataB[count] *= 0.98f; //scale brightness down
        edgeG[count] *= 0.91f; edgeR[count] *= 0.91f; edgeB[count] *= 0.97f; //scale brightness down
    }
    g.drawImage(barImage, 0, 0, getWidth(), getHeight()-linewidth, 0, 0, 33, 4); //draw the bar meter
    if ((displayTrackName == juce::String()) || (displayTrackName.length() < 1.0f)) displayTrackName = juce::String("ConsoleX Channel"); //if not track name, then name of plugin
    g.setFont(juce::Font(displayFont, g.getCurrentFont().getHeight(), 0)); g.setFont(getHeight());
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.5f));
    g.fillRect(0, 0, getWidth(), linewidth);
    g.fillRect(0, 0, linewidth, getHeight());
    g.drawText(displayTrackName, 1, 1, getWidth(), getHeight(), juce::Justification::centred); //shadow under text

    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.8f));
    g.fillRect(linewidth, getHeight()-linewidth, getWidth(), linewidth);
    g.fillRect(getWidth()-linewidth, linewidth, linewidth, getHeight()-linewidth); //interior bevel for object
    g.drawText(displayTrackName, 0, 0, getWidth()-1, getHeight()-1, juce::Justification::centred); //just a tiny emboss on our meter/label
    
}
