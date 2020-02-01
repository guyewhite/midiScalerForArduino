/**
 * midiScalerForArduino.ino
 * Guy E. White <guy@guyewhite.com> https://github.com/guyewhite
 * Copyright (c) 2020 Guy E. White. All rights reserved.
 *
 * Produces midi scales based upon a root number. Indended for Arduino.
 * An ideal starting point for creating a midi improviser, arp, or scaler.
 *
 * Provided and distrubuted by Triumphant Heart International, Inc. Happy Valley,
 * Oregon, USA, under GNU General Public License v3.0 (GNU GPLv3)
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. We specifically discliam any and all liability for
 * any errors or omissions. See the attached GNU eneral Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License, version 3,
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// MACROS
#define SPEED 1000000;  // Delay between notes
#define SCALE 0;        // A number between 0-13
#define OCTAVE 5;       // A number 0-10

// DEFINITIONS
void pause(int milliSeconds);
int getRandomInt(int max, int min);
void getOctaveNotes(int octaveNotes[], int octave);
void getScaleNotes(int scale, int octaveNotes[], int octave, int rootNote, int scaleNotes[]);

// DECLARATIONS
typedef struct {
    char name[9];
    int notes[8];
}Scale;

typedef enum {
   FALSE, TRUE
}
bool;

// GLOBALS - NOT RECOMMENDED
int scaleNotesLength = 8;   // How many notes are in the Scale
bool firstRun = TRUE;

// MAIN LOOP
int main(void)
{

    // End Setup Mode
    firstRun = FALSE;

    // Set the Octave and Scale Being Played
    int octave = OCTAVE;
    int scale = SCALE;

    int octaveNotes[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    getOctaveNotes(octaveNotes, octave);

    /**
     * Set root note
     * C(0), C#(1), D(2), D#(3), E(4), F(5), F#(6), G(7), G#(8), A(9), A#(10), B(11)
     */
    int rootNote = 0;

    // Create a place to hold the current sequence
    int scaleNotes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    getScaleNotes(scale, octaveNotes, octave, rootNote, scaleNotes);

    // Create a way to track the playHead and it's predisessor
    int trav = 0;
    int prev = -1;

    // Create a way to track the currentNote
    int currentNote = 0;

    // Simulates the arduino main loop
    while (1) {
        // Play the scaleNotes
        currentNote = currentNote % scaleNotesLength;

        // Skip empty portions of the scale array
        if (scaleNotes[currentNote] != 0) {

            // Here you can use a midi library to output notes
            printf("\n%i", scaleNotes[currentNote]);

            // Pause and wait for the next note
            int delayTime = SPEED;
            pause(1 * delayTime);

            // Depending upon the midi library you use, you may wish to silence the note here
        }
        currentNote++;
    }

}

/**
 * pause - a function that creates a pause for x miliseconds.
 */
void pause(int milliSeconds) {

    clock_t startTime = clock();
    while (clock() < startTime + milliSeconds) {
        // wait
    }
}

/**
 * getRandomInt - a function that returns a random number in a range.
 */
int getRandomInt(int max, int min) {
    int randomNumber = (rand() % (max - min + 1)) + min;
    return randomNumber;
}

/**
 * getOctaveNotes - a function that sets an array of length 12 to include the
 * midi notes of a specific octave. It takes in an array of 12 octaveNotes[] and
 * the octave (0-10) to which the notes should be set.
 */
void getOctaveNotes(int octaveNotes[], int octave) {

    /**
     * Set the notes for this octave, zero indexed. Each octave has 12 notes.
     * The notes include:
     * C, C#, D, D#, E, F, F#, G, G#, A, A#, B
     * Each of these notes, therefore, has a numerical reprentation as:
     * C(0), C#(1), D(2), D#(3), E(4), F(5), F#(6), G(7), G#(8), A(9), A#(10), B(11)
     */
    int octaveBase[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    // Each octave has 12 notes, except octave 11, which has 8 notes
    int notesInOctave = 0;
    if (octave != 10) {
        notesInOctave = 12;
    } else {
        notesInOctave = 8;
    }

    /**
     * Now, use the following formula to discover the notes in this octave:
     *
     * note = octaveBase + (octave * 12);
     *
     * For each of the 12 notes (0-11), apply the formula For example, if
     * the octave is 0, the calculation of C & C# would be as follows:
     *
     * C = 0 + (0 * 12) = 0
     * C# = 1 + (0 * 12) = 1
     *
     * If the octave is 5, the calculation would be as follows:
     *
     * C = 0 + (5 * 12) = 60
     * C# = 1 + (5 * 12) = 61
     *
     */

    for (int i = 0; i < notesInOctave; i++) {
         octaveNotes[i] = octaveBase[i] + (octave * 12);
    }
}

/**
 * getScaleNotes - a function that sets an array of length 8 to include the
 * midi notes of a specific scale. For future impelementation, one could take
 * another paremeter to include the length of any size array rather than a fixed
 * length of 8.
 *
 * Paremeters:
 * int scale = A number 0-13. Each number corresponds to the a scale.
 *
 * int octaveNotes[] = An array of size 12, containing a group of 12 midi notes
 * that are of one octave.
 *
 * int octave = A number 1-10. Each number corresponding to an octave. 5 is the middle.
 *
 * int rootNote = A number 0-11. The note at which the scale begins.
 *
 * int scaleNotes[] = An array of length 8 that includes midi notes of a specific scale.
 */
void getScaleNotes(int scale, int octaveNotes[], int octave, int rootNote, int scaleNotes[]) {

    // Scale definitions in half steps
    Scale presetScales[14] = {
        {{"MAJOR"},     {2, 2, 1, 2, 2, 2, 1}},
        {{"PENTMAJOR"}, {2, 2, 3, 2, 3, 0, 0}},
        {{"BLUMAJOR"},  {2, 1, 2, 3, 2, 3, 0}},
        {{"NATMINOR"},  {2, 1, 2, 2, 1, 2, 2}},
        {{"MELMINOR"},  {2, 1, 2, 2, 2, 2, 1}},
        {{"HARMINOR"},  {2, 1, 2, 2, 1, 3, 1}},
        {{"PENTMINOR"}, {3, 2, 2, 3, 2, 0, 0}},
        {{"BLUMINOR"},  {3, 2, 1, 1, 3, 2, 0}},
        {{"LYDIAN"},    {2, 2, 2, 1, 2, 2, 1}},
        {{"DORIAN"},    {2, 1, 2, 2, 2, 1, 2}},
        {{"PHRYGIAN"},  {1, 2, 2, 2, 1, 2, 2}},
        {{"AOELIAN"},   {2, 1, 2, 2, 1, 2, 2}},
        {{"LOCRIAN"},   {1, 2, 2, 1, 2, 2, 2}},
        {{"MIXLYDIAN"}, {2, 2, 1, 2, 2, 1, 2}}
    };

    // Set the first note of the scale
    scaleNotes[0] = rootNote + (octave * 12);

    // Populate the rest of the scale
    int currentNote = scaleNotes[0];
    for (int i = 0; i < 8; i++) {

        // Only populate this note if the scale includes a note at this position
        if (presetScales[scale].notes[i] != 0) {
            scaleNotes[i+1] = currentNote + presetScales[scale].notes[i];
            currentNote = scaleNotes[i+1];
        } else {
            break;
        }
    }
}
