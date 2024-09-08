# CAP2MID
Capcom (GB/GBC) to MIDI converter

This tool converts music from Game Boy and Game Boy Color games using Capcom's in-house sound engine to MIDI format.

It works with ROM images. To use it, you must specify the name of the ROM followed by the number of the bank containing the sound data (in hex).
For games that contain multiple banks of music (either 2 or 3), you must run the program multiple times specifying where each different bank is located. However, in order to prevent files from being overwritten, the MIDI files from the previous bank must either be moved to a separate folder or renamed.

Examples:
* CAP2MID "Megaman Xtreme (U) [C][!].gbc" 3
* CAP2MID "Megaman Xtreme (U) [C][!].gbc" 4
* CAP2MID "Megaman Xtreme (U) [C][!].gbc" 5
* CAP2MID "Gargoyle's Quest - Ghosts'n Goblins (UE) [!].gb" 4

The music sequence format was based on my own reverse-engineering, and was found to be almost identical to Capcom's later NES format.

Like most of my other programs, another converter, CAP2TXT, is also included, which prints out information about the song data from each game. This is essentially a prototype of CAP2TXT.

Supported games:
  * Capcom Quiz
  * Gaiamaster Duel: Card Attackers
  * Gargoyle's Quest
  * Hamster Club
  * Magical Tetris Challenge
  * Mega Man Xtreme
  * Mega Man Xtreme 2
  * Warau Inu no Bouken: Silly Go Lucky!
  * Who Framed Roger Rabbit?

## To do:
  * Panning support
  * Support for other versions of the sound engine (NES, SNES) which are very close
  * GBS file support
