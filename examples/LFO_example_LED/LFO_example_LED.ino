// --------------------------------------------------
//
// LFO - based on micros() function - Arduino Due
// by mo_thunderz (mo-thunderz @github)
//
// this example can be helpful if you cannot get the main
// example running. This file generates a simple Square 
// waveform and sends it to the internal LED (13) 
// It starts at 1 Hz and then gradually speeds up to 7 Hz.
// this example should work on any arduino, although you
// might need to update the LED_pin if it is not at 13
//
// Parameters: please refer to lfo.h
//
// --------------------------------------------------


#include <lfo.h>        // required for function generation

#define DACSIZE 4096             // vertical resolution of the DACs

// variables
int             lfo_waveform = 4;                   // saw wave
int             lfo_freq = 1;                       // start at 6 Hz -> value will be varied over time
int             LED_pin = 13;                       // LED pin


// internal variables
unsigned long   t = 0;                              // timestamp: current time
unsigned long   t_lfo_param_delta = 2000000;        // time in µs to changed lfo parameters (needed only to periodically change waveform)
unsigned long   t_lfo_param_0 = 0;                  // timestamp: last time parameter change (needed only to periodically change waveform)

// internal classes
lfo             lfo_class(DACSIZE);

void setup() {
  delay(100);

  pinMode (LED_pin, OUTPUT);
  
  lfo_class.setWaveForm(lfo_waveform);              // initialize waveform
  lfo_class.setAmpl(DACSIZE-1);                     // set amplitude to maximum
  lfo_class.setAmplOffset(0);                       // no offset to the amplitude
  lfo_class.setMode(0);                             // set sync mode to mode0 -> no sync to BPM
  lfo_class.setMode0Freq(lfo_freq);                 // set LFO to 30 Hz
}

void loop() {
  t = micros();                                     // take timestamp

  analogWrite(LED_pin, lfo_class.getWave(t));          // update LFO and write to DAC
  
  if (t > t_lfo_param_0 + t_lfo_param_delta) {            // check if lfo parameters are to be changed
    lfo_freq++;             // increase LFO speed with 1
    if (lfo_freq > 7)       // reset to 1Hz of the LFO speed exceeds 7
      lfo_freq = 1;
    lfo_class.setMode0Freq(lfo_freq);
    t_lfo_param_0 = t;
  }
}
