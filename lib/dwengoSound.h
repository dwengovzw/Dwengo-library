/** \file dwengoSound.h
 *  \brief Sound library for Dwengo
*/
#ifndef DWENGOSOUND_H
#define DWENGOSOUND_H

#include "dwengoInterrupt.h"
#include <math.h>


#define SPEAKERPORT LATCbits.LATC0
//struct

/**
 * \struct note
 * \brief datastructure to store a note.
 */
typedef struct{
    char pitch; /**<Character representation of the pitch of a note according the international music notation standard (A to G ). A lowercase letter is the flat of the equivalent uppercase note. There is no notation for sharp notes.*/
    char octave; /**<Octave of the note, ranging from -1 to 4. 0 Beeing the octave in which A is 440hz. */
    char duration; /**<Length of the note expressed in amount of sixteenth notes.*/
} note;
 // functions


 /** \brief Initialize sound functionality.
 * This routine initializes sound functionality of the Dwengo board. It
 * configures the C pins as digital outputs. C0 is used as the speaker output.
 * It also configures timer0 and timer1 and dispatches the sound interrupt service routine,
 * using the dwengo interrupt library.
 * Timer0 is the low interrupt timer. Its interrupts determine the rythm.
 * Timer1 is the high interrupt timer. This one regulates the frequency of the notes.
 **/
void initSound(void);
/**
 * \brief Plays a song.
 * @param song pointer to an array of notes.
 * @param bpm the tempo of the song, expressed in beats per minute.
 * Runs through an array of notes at a specified tempo. Each note encoutered
 * will be played trough \a playFrequency for the specified amount of time.
 */
void startSong(note* song, unsigned int bpm);
/**
 * \brief stops the timers, effectively stopping the sound.
 */
void stopSound(void);

/**
 * \brief keeps generating a tone on the C0 pin untill \a stopSound() is called
 * @param frequency desired frequency.
 * This function generates a square wave on the C0 pin.
 * Attaching a speaker to this pin will result in a audible tone.
 */
void playFrequency(unsigned int frequency);

/**
 * \brief ISR that handles the frequency.
 */
void frequencyHighISR ();
/**
 * \brief ISR that handles the tempo.
 */
void beatLowISR ();
/**
 * \brief help function that returns the frequency of a note.
 * @param Note input note of which the frequency must be determined.
 */
unsigned int noteFrequency(note Note);

#endif
