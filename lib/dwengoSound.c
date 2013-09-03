#include "dwengoSound.h"
#define EOS 1

//declaring global variables
unsigned int noteCount;
unsigned int beatCount;
unsigned int BPM;
note* currentSong;
unsigned char preloadTMR0L;
unsigned char preloadTMR0H;
unsigned char preloadTMR1L;
unsigned char preloadTMR1H;



void initSound(void){
    initInterrupt();
    registerLowISR(beatLowISR);
    registerHighISR(frequencyHighISR);

    RCONbits.IPEN = 1; /*enable priority levels*/
    RCONbits.SBOREN = 0; /* disable BOR software */

    IPR1bits.TMR1IP = 1; /*set timer 1 priority high*/
    PIE1bits.TMR1IE = 1; /*Enable timer 1 overflow interrupt*/

    INTCON2bits.TMR0IP = 0; /*set Timer 0 priority low*/
    INTCONbits.TMR0IE = 1; /*enable timer 0 overflow interrupt*/
    T0CONbits.T08BIT = 0; /* set timer 0 as 16-bit counter*/
    T0CONbits.T0CS = 0; /* set source timer 0 to internal instruction cycle clock */


    INTCONbits.GIEH = 1; /*enable high priority interrupts*/
    INTCONbits.GIEL = 1; /*enable low priority interrupts*/

    T1CONbits.T1CKPS1 = 0; /* 00 : 1/1  01  : 1/2  10  : 1/4  11  :  8*/
    T1CONbits.T1CKPS0 = 0; /* prescale value set to 1/1 */

    T0CONbits.PSA = 0; /*prescaler is assigned*/
    T0CONbits.T0PS2 = 1; /* set prescale value*/
    T0CONbits.T0PS1 = 0; /* 111: 1/256 110: 1/128 101: 1/64 ... 000: 1/2*/
    T0CONbits.T0PS0 = 1; /* prescale value set to 1/64 */

    PIR1bits.TMR1IF = 0; /*Timer 1 interrupt flag off*/
    INTCONbits.TMR0IF = 0; /*Timer 0 interrupt flag off*/
    TRISCbits.TRISC0 = 0; /*Configure RC0 as output*/
    delay_ms(1000);
}
void startSong(note* song, unsigned int bpm){
    unsigned int frequency;
    unsigned int preloadTMR0;

    noteCount = 0 ;
    currentSong = song;
    BPM = bpm;
    preloadTMR0 = (256*(256-46875/(4*BPM)*60/64));

    frequency  = noteFrequency(currentSong[noteCount]);
    beatCount = currentSong[noteCount].duration;
    playFrequency(frequency);

    preloadTMR0L = 0x00FF & preloadTMR0;
    preloadTMR0H = ((0xFF00 & preloadTMR0) >> 8);
    TMR0L = preloadTMR0L;
    TMR0H = preloadTMR0H;

    T1CONbits.TMR1ON = 1; /*timer 1 on/off */
    T0CONbits.TMR0ON = 1; /*timer 0 on*/
}
void stopSound(){
    T1CONbits.TMR1ON = 0; /*timer 1 on/off */
    T0CONbits.TMR0ON = 0; /*timer 0 on*/
    SPEAKERPORT = 0;
}

void playFrequency(unsigned int frequency){
    if(frequency){
        unsigned int preloadTMR1;
        if(!T1CONbits.TMR1ON) T1CONbits.TMR1ON = 1;
        preloadTMR1 = (65536-12000000/(frequency*2))+440;
        preloadTMR1L = 0x00FF & preloadTMR1;
        preloadTMR1H = ((0xFF00 & preloadTMR1) >> 8);

        TMR1L = preloadTMR1L;
        TMR1H = preloadTMR1H;
    }
    else T1CONbits.TMR1ON = 0;

}

void frequencyHighISR () {
    TMR1L = preloadTMR1L;
    TMR1H = preloadTMR1H;
    SPEAKERPORT = !SPEAKERPORT; /*change status of the port*/
    PIR1bits.TMR1IF = 0; /*Timer 1 interrupt flag off*/
}
void beatLowISR () {
    TMR0L = preloadTMR0L;
    TMR0H = preloadTMR0H;
    beatCount--;
    if(beatCount==0){
        unsigned int frequency;
        noteCount++;
        frequency = noteFrequency(currentSong[noteCount]);
        if(frequency != EOS)
        {
            beatCount=currentSong[noteCount].duration;
            playFrequency(frequency);
        }
        else stopSound();
    }
    INTCONbits.TMR0IF = 0;
}
unsigned int noteFrequency(note Note){
    int freq;
    switch (Note.pitch){
        case 'C': freq= 4186;
                  break;
        case 'd': freq= 4434;
                  break;
        case 'D': freq= 4698;
                  break;
        case 'e': freq= 4978;
                  break;
        case 'E': freq= 5274;
                  break;
        case 'F': freq= 5587;
                  break;
        case 'g': freq= 5919;
                  break;
        case 'G': freq= 6271;
                  break;
        case 'a': freq= 6644;
                  break;
        case 'A': freq= 7040;
                  break;
        case 'b': freq= 7458;
                  break;
        case 'B': freq= 7902;
                  break;
        case 'r': return 0;
                  break;
        case 'Q': return EOS;
                  break;
        default: return EOS;
    }
    return freq/pow(2,4-Note.octave);
}


