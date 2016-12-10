

#include <util/delay.h>

const int speakerPin = 4;
volatile int mod = 1; // 0 - stop, 1 - play
volatile int track = 0; // numarul piesei
volatile int n;

const int length[8] = {73,69,71,29,51,77,64,63}; // numarul de note din fiecare piesa

const char * notes[8] = {"ggagsed deggsgg ggagsed deggsgg DCbCDbCbabCabagabgagsgasgsesgeseddeggsgg ", \
                     "ddaagfedcdefga ddaagfedcdefga avgavCDagfdefgfgavaagfedfedgfgavCDagfed" , \
                     "cfffgagavCavafggfcfffgagavCavafggffaCDCvagfgavCcfagfccfffgagavCavafggf " , \
                     "faagfvvagaCCbCaDCvagfeagffef ", \
                     "aavCagfgagdgavCaggfgagff vavCDDaaCagfecavCagfgagff ", \
                     "cffefaagCCCvagacffefaagCffeedcCCCfvaagCCCfvaagDDDCvavgavCffgaDDDCvavgavCffgf ", \
                     "ggdgadbabCbaggsesgabsedd DCbCbabgasedggsgagdbbabCbabCbagsgCbagg ", \
                     "egbbbaCbagabgegasedeaaeesgsedgabbbaCbag DD bb baCdagabgegasede " } ;
const byte beats[8][77] = { { 2,2,1,1,1,1,4,2,2,2,2,2,2,4,2,2,2,2,1,1,1,1,4,2,2,2,2,2,2,4,2,2,3,1,1,1,1,1,3,1,1,1,1,1,3,1,1,1,1,1,3,1,1,1,1,1,3,1,1,1,1,1,3,1,2,2,2,2,2,2,4,2,2 }, \
                 { 2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,4,2,2,2,2,2,2,4,1,1,2,4,2,2,2,2,2,2,2,2,2,2,8 } , \
                 { 2,2,2,2,2,1,1,1,1,2,2,2,1,1,2,2,6,2,2,2,2,2,1,1,1,1,2,2,2,1,1,2,2,6,1,1,3,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,4,4,2,2,2,2,1,1,1,1,2,2,2,1,1,2,2,6,2 } , \
                 { 2,3,1,2,2,2,2,2,2,2,2,2,2,6,2,3,1,2,2,2,2,2,2,2,2,2,2,6,2 }, \
                 { 2,3,1,2,2,4,4,3,1,2,2,8,3,1,2,2,3,1,4,2,2,3,1,6,2,3,1,2,2,2,2,2,2,2,2,2,2,6,2,2,2,2,2,4,4,2,2,3,1,8,8}, \
                 { 2,2,3,1,2,2,2,2,2,2,3,1,2,2,4,2,2,3,1,2,2,2,2,2,2,3,1,2,2,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,1,1,3,1,2,2,4,3,1,2,2,2,2,4,2,1,1,3,1,2,2,4,8} ,\
                 { 2,4,2,2,4,4,2,2,2,2,4,2,2,4,2,2,2,2,2,2,4,3,1,6,2,4,2,2,4,4,2,2,2,2,3,1,2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,4,3,1,6,8 }, \
                 { 2,2,2,2,2,2,2,2,2,6,2,2,2,2,2,2,2,2,2,6,2,2,2,2,2,4,2,2,6,2,2,2,2,2,2,2,2,2,4,2,2,4,2,2,4,2,2,2,2,2,2,6,2,2,2,2,2,2,2,2,2,8,8 }};
          

 
const int tempo[8] = {150,150,150,150,150,150,150,150};

            

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {

const char names[] = { 'c', 'd', 'e', 'f', 's', 'g', 'a', 'v', 'b', 'C', 'D', 'E' }; // note
const int tones[] = { 1915, 1700, 1519, 1432, 1352, 1275, 1136, 1073, 1014, 956, 852, 758 }; // frecvente

  for (int i = 0; i < 12; i++) 
  {
    if (names[i] == note) playTone(tones[i], duration);
  }
}

void setup() {
  pinMode(speakerPin, OUTPUT);
  pinMode(2, INPUT_PULLUP);
 
  //activare int0 si int1
  EIMSK |= (1 << INT0);
  //activare intreruperi globale
  sei();
}

void loop() 
{
 
   if(mod == 1)
   {
    for (n = 0; n < length[track]; n++) 
    {
      if (mod==0) 
        break;
     
      if (notes[track][n] == ' ') 
        delay(beats[track][n] * tempo[track]); // pauză
      else 
        playNote(notes[track][n], beats[track][n] * tempo[track]);
    }
    // pauza intre note
    delay(tempo[track] / 2);
   }
}

ISR(INT0_vect) {
    mod = 1 - mod;
    if(mod == 1){
      track++;
      track = (track % 8);
      n = 0; // resetez la prima nota din melodie
    }
 
 _delay_ms(400);
}

