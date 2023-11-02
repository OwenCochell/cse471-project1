# Additive

Author: Emily Feuer Score: scores/additive.score

This file describes the implementative of the additive synthesizer component. The following features have been implemented:
- Polyphony
- Glissando
- Sound cross-fading
- Envelope generation

## Polyphony
The additive component has the ability to play more than one note at a time. A single instance of the additive component can contain multiple notes at the same beat of a measure.

## Glissando
The additive component supports glissando through the `FreqManager` class, which handles the smooth transition in pitch from one note to another. Notes in this component are passed to the `FreqManager` class, which determines if the note requires glisando. Glissando in a note is defined through the `gliss` attribute, which states the note that is being reachedin the glissando.

## Sound Cross-Fading
Sound cross-fading is supported through having two sets of sound definitions in the class for this component, and interpolating between them to "fill the gap" 
between the two. This allows for a more varied sound in a note.

## Envelope Generation
This component uses an ADSR envelope that is shared between components.
