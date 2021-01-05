# LFO
LFO for Arduino Due (and other Arduino/Teensy/exp32/etc). LFO can be free-running, or synced to a master-clock.

See as well my Youtube video on how to use this class: https://youtu.be/ch03-75Fkuw

# Installation
1) Make a new folder called "lfo" in your arduino libraries folder
2) Copy lfo.cpp and lfo.h into the "lfo" folder

# How to use
Please refer to the LFO_example.ino file in the examples folder. This file is written for the Arduino Due and can be run on other Arduino boards with some small modifications. I used the Due as it has a built-in DAC. For use on other boards the following changes are to be made to the LFO_example.INO file:
1) Reduce the DACSIZE to 256
2) Remove the analogWriteResolution(12) statement 
3) Write to a different port in this statement: 
analogWrite(DAC0, lfo_class.getWave(t));
----> for ESP32 use:
dacWrite(25, lfo_class.getWave(t));

# Updates
29.12.2020: initially _mode1_rate was setup as an INT. However, that is problematic when rates smaller than 1 are applied. This is now changed to float. I also added version numbering and a few more get functions needed for the Ableton-control SW.

Have fun ;-)

mo thunderz
