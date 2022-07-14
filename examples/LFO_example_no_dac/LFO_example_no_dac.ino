// --------------------------------------------------
//
// LFO - based on micros() function - any Arduino platform
// by mo_thunderz (mo-thunderz @github)
//
// this example generates an LFO signal and writes the value
// to the serial port. The waveform can be chosen with lfo_waveform 
// 0 - off
// 1 - saw
// 2 - triangle
// 3 - sin
// 4 - square
// test different vertical resolutions by changing the DACSIZE. For
// other parameters refer to lfo.h
//
// --------------------------------------------------


#include <lfo.h>        // required for function generation

#define DACSIZE 4096             // vertical resolution of the DACs

// variables
int             lfo_waveform = 3;                   // 

// internal variables
unsigned long   t = 0;                              // timestamp: current time

// internal classes
lfo             lfo_class(DACSIZE);

void setup() {
  delay(100);

  lfo_class.setWaveForm(lfo_waveform);              // initialize waveform
  lfo_class.setAmpl(DACSIZE-1);                     // set amplitude to maximum
  lfo_class.setAmplOffset(0);                       // no offset to the amplitude
  lfo_class.setMode(0);                             // set sync mode to mode0 -> no sync to BPM
  lfo_class.setMode0Freq(30);                       // set LFO to 30 Hz

  Serial.begin(115200);
}

void loop() {

  Serial.println(lfo_class.getWave(t));
  t = t + 2000;
  delay(200);
  
}
