# CAP2MID
Capcom (NES/GB/GBC) to MIDI converter

This tool converts music from NES, Game Boy, and Game Boy Color games using Capcom's in-house sound engine to MIDI format. This tool supports all three drivers, or re-worked versions of the driver.

It works with ROM images. To use it, you must specify the name of the ROM followed by the number of the bank containing the sound data (in hex). Note that on the NES, banks refer to 8kb sections (length 0x2000), while on GB/GBC, banks refer to 16kb sections (length 0x4000).
For GBC games that contain multiple banks of music (either 2 or 3), you must run the program multiple times specifying where each different bank is located. However, in order to prevent files from being overwritten, the MIDI files from the previous bank must either be moved to a separate folder or renamed.
The flag "P" can also be additionally used to adjust the music tempo for the PAL NES. However, this flag only affects the earlier NES games (before 1990).

Examples:
* CAP2MID "Megaman Xtreme (U) [C][!].gbc" 3
* CAP2MID "Megaman Xtreme (U) [C][!].gbc" 4
* CAP2MID "Megaman Xtreme (U) [C][!].gbc" 5
* CAP2MID "Gargoyle's Quest - Ghosts'n Goblins (UE) [!].gb" 4
* CAP2MID "Megaman III (E) (PRG0) [!].nes" 17
* CAP2MID "Trojan (E) [!].nes" D P

The music sequence format was based on my own reverse-engineering, and was found to be almost identical to Capcom's later NES format. I was later able to add support to the NES version of not only this driver, but also the earlier iterations of the driver which are very different, especially thanks to information provided by Matrixx and Justin Olbrantz.

Like most of my other programs, another converter, CAP2TXT, is also included, which prints out information about the song data from each game. This is essentially a prototype of CAP2TXT. This is, however, very out of date and only supports the Game Boy version of the later driver.

Supported games:

NES:
 * 1943: The Battle of Midway
 * Adventures in the Magic Kingdom
 * The California Raisins: The Grape Escape
 * Chip'n Dale: Rescue Rangers
 * Commando
 * Darkwing Duck
 * Destiny of an Emperor
 * DuckTales
 * Gargoyle's Quest II: The Demon Darkness
 * Gun.Smoke
 * Higemaru Makaijima: Nanatsu no Shima Daibouken
 * Ide Yousuke Meijin no Jissen Mahjong
 * Legendary Wings
 * The Little Mermaid
 * Little Nemo: The Dream Master
 * Mega Man
 * Mega Man II
 * Mega Man III
 * Mega Man IV
 * Mega Man V
 * Mega Man VI
 * Mighty Final Fight
 * Mizushima Shinji no Dai Koushien
 * Pro Yakyuu? Satsujin Jiken!
 * Section-Z
 * Street Fighter 2010: The Final Fight
 * Strider
 * Sweet Home
 * TaleSpin
 * Tenchi wo Kurau II: Shokatsu Koumei Den
 * Trojan
 * Willow
 * Wily & Light no Rockboard: That's Paradise

GB/GBC:
  * Capcom Quiz
  * DuckTales
  * Gaiamaster Duel: Card Attackers
  * Gargoyle's Quest
  * Hamster Club
  * Magical Tetris Challenge
  * Mega Man Xtreme
  * Mega Man Xtreme 2
  * Warau Inu no Bouken: Silly Go Lucky!
  * Who Framed Roger Rabbit?

Known issue: The "set tempo" function is sometimes problematic in later games.

## To do:
  * Panning support
  * Support for other versions of the sound engine (e.g. SNES) which are very close
  * NES and GBS file support
