/*Capcom (GB/GBC) to MIDI converter*/
/*By Will Trowbridge*/
/*Portions based on code by ValleyBell*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#define bankSize 16384

FILE* rom, * txt;
long bank;
long offset;
long tablePtrLoc;
long tableOffset;
int i, j;
char outfile[1000000];
int songNum;
long seqPtrs[4];
long songPtr;
long bankAmt;
int foundTable = 0;
int curInst = 0;
int isSong = 1;

unsigned static char* romData;

const char MagicBytes[5] = { 0x7C, 0x65, 0x6F, 0xB4, 0xC8 };

/*Function prototypes*/
unsigned short ReadLE16(unsigned char* Data);
unsigned short ReadBE16(unsigned char* Data);
static void Write8B(unsigned char* buffer, unsigned int value);
static void WriteBE32(unsigned char* buffer, unsigned long value);
static void WriteBE24(unsigned char* buffer, unsigned long value);
static void WriteBE16(unsigned char* buffer, unsigned int value);
void song2txt(int songNum, long ptr);

/*Convert little-endian pointer to big-endian*/
unsigned short ReadLE16(unsigned char* Data)
{
	return (Data[0] << 0) | (Data[1] << 8);
}

/*Convert little-endian pointer to big-endian*/
unsigned short ReadBE16(unsigned char* Data)
{
	return (Data[0] << 8) | (Data[1] << 0);
}

static void Write8B(unsigned char* buffer, unsigned int value)
{
	buffer[0x00] = value;
}

static void WriteBE32(unsigned char* buffer, unsigned long value)
{
	buffer[0x00] = (value & 0xFF000000) >> 24;
	buffer[0x01] = (value & 0x00FF0000) >> 16;
	buffer[0x02] = (value & 0x0000FF00) >> 8;
	buffer[0x03] = (value & 0x000000FF) >> 0;

	return;
}

static void WriteBE24(unsigned char* buffer, unsigned long value)
{
	buffer[0x00] = (value & 0xFF0000) >> 16;
	buffer[0x01] = (value & 0x00FF00) >> 8;
	buffer[0x02] = (value & 0x0000FF) >> 0;

	return;
}

static void WriteBE16(unsigned char* buffer, unsigned int value)
{
	buffer[0x00] = (value & 0xFF00) >> 8;
	buffer[0x01] = (value & 0x00FF) >> 0;

	return;
}

int main(int args, char* argv[])
{
	printf("Capcom (GB/GBC) to TXT converter\n");
	if (args != 3)
	{
		printf("Usage: CAP2TXT <rom> <bank>\n");
		return -1;
	}
	else
	{
		if ((rom = fopen(argv[1], "rb")) == NULL)
		{
			printf("ERROR: Unable to open file %s!\n", argv[1]);
			exit(1);
		}
		else
		{
			bank = strtol(argv[2], NULL, 16);
			if (bank != 1)
			{
				bankAmt = bankSize;
			}
			else
			{
				bankAmt = 0;
			}
			fseek(rom, ((bank - 1) * bankSize), SEEK_SET);
			romData = (unsigned char*)malloc(bankSize);
			fread(romData, 1, bankSize, rom);
			fclose(rom);

			/*Try to search the bank for song table loader*/
			for (i = 0; i < bankSize; i++)
			{
				if ((!memcmp(&romData[i], MagicBytes, 5)) && foundTable != 1)
				{
					tablePtrLoc = bankAmt + i - 5;
					printf("Found pointer to song table at address 0x%04x!\n", tablePtrLoc);
					tableOffset = ReadLE16(&romData[tablePtrLoc - bankAmt]);
					printf("Song table starts at 0x%04x...\n", tableOffset);
					foundTable = 1;
				}
			}

			if (foundTable == 1)
			{
				i = tableOffset - bankAmt;
				songNum = 1;
				while (isSong == 1)
				{
					songPtr = ReadBE16(&romData[i]);
					if (romData[songPtr - bankAmt] != 0 && songPtr != 0)
					{
						if (songNum > 1)
						{
							isSong = 0;
						}

						else if (songNum == 1)
						{
							i += 0x80;
							songPtr = ReadBE16(&romData[i]);
						}

					}
					else
					{
						if (songPtr != 0)
						{
							printf("Song %i: %04X\n", songNum, songPtr);
							song2txt(songNum, songPtr);
						}
						else if (songPtr == 0)
						{
							printf("Song %i: %04X (empty)\n", songNum, songPtr);
						}
						i += 2;
						songNum++;
					}

				}
			}
			else
			{
				printf("ERROR: Magic bytes not found!\n");
				exit(-1);
			}
		}

		printf("The operation was successfully completed!\n");
		return 0;
	}
}

void song2txt(int songNum, long ptr)
{
	unsigned char command[3];
	long seqPos = 0;
	long songPtrs[4];
	int curTrack = 0;
	int seqEnd = 0;
	int octave = 0;
	int transpose1 = 0;
	int transpose2 = 0;
	int curNote = 0;
	int curNoteLen = 0;
	int curVol = 0;
	int flag1 = 0;
	int flag2 = 0;
	int flag3 = 0;
	int flag4 = 0;
	int repeat1 = 0;
	int repeat2 = 0;
	int repeat3 = 0;
	int repeat4 = 0;
	long repeat1Pt = 0;
	long repeat2Pt = 0;
	long repeat3Pt = 0;
	long repeat4Pt = 0;
	long break1 = 0;
	long break2 = 0;
	long break3 = 0;
	long break4 = 0;
	long loopPt = 0;
	long speedCtrl = 0;
	unsigned char lowNibble = 0;
	unsigned char highNibble = 0;
	int chanSpeed = 0;

	sprintf(outfile, "song%i.txt", songNum);
	if ((txt = fopen(outfile, "wb")) == NULL)
	{
		printf("ERROR: Unable to write to file song%i.txt!\n", songNum);
		exit(2);
	}
	else
	{
		songPtrs[0] = ReadBE16(&romData[ptr + 1 - bankAmt]);
		songPtrs[1] = ReadBE16(&romData[ptr + 3 - bankAmt]);
		songPtrs[2] = ReadBE16(&romData[ptr + 5 - bankAmt]);
		songPtrs[3] = ReadBE16(&romData[ptr + 7 - bankAmt]);
		for (curTrack = 0; curTrack < 4; curTrack++)
		{
			seqEnd = 0;
			if (songPtrs[curTrack] == 0)
			{
				seqEnd = 1;
			}
			else
			{
				seqPos = songPtrs[curTrack] - bankAmt;
			}
			fprintf(txt, "Channel %i: 0x%04X\n", curTrack + 1, songPtrs[curTrack]);
			while (seqEnd == 0)
			{
				command[0] = romData[seqPos];
				command[1] = romData[seqPos + 1];
				command[2] = romData[seqPos + 2];

				if (command[0] == 0x00)
				{
					fprintf(txt, "Triplet\n");
					seqPos++;
				}

				else if (command[0] == 0x01)
				{
					fprintf(txt, "Connect notes\n");
					seqPos++;
				}

				else if (command[0] == 0x02)
				{
					fprintf(txt, "Dotted notes\n");
					seqPos++;
				}

				else if (command[0] == 0x03)
				{
					fprintf(txt, "Higher octave\n");
					seqPos++;
				}

				else if (command[0] == 0x04)
				{
					flag1 = command[1];
					fprintf(txt, "Set channel flags: %01X\n", flag1);
					seqPos += 2;
				}

				else if (command[0] == 0x05)
				{
					speedCtrl = ReadBE16(&romData[seqPos + 1]);
					fprintf(txt, "Set tempo/speed: %04X\n", speedCtrl);
					seqPos += 3;
				}

				else if (command[0] == 0x06)
				{
					fprintf(txt, "Set note size: %i\n", command[1]);
					seqPos += 2;
				}

				else if (command[0] == 0x07)
				{
					curVol = command[1];
					fprintf(txt, "Set volume: %i\n", curVol);
					seqPos += 2;
				}

				else if (command[0] == 0x08)
				{
					fprintf(txt, "Set vibrato: %i\n", command[1]);
					seqPos += 2;
				}

				else if (command[0] == 0x09)
				{
					octave = command[1];
					fprintf(txt, "Set octave transpose: %i\n", octave);
					seqPos += 2;
				}

				else if (command[0] == 0x0A)
				{
					transpose2 = (signed char)command[1];
					fprintf(txt, "Transpose (all channels): %i\n", transpose2);
					seqPos += 2;
				}

				else if (command[0] == 0x0B)
				{
					transpose1 = (signed char)command[1];
					fprintf(txt, "Transpose (current channel): %i\n", transpose1);
					seqPos += 2;
				}

				else if (command[0] == 0x0C)
				{
					fprintf(txt, "Set tuning: %i\n", (signed char)command[1]);
					seqPos += 2;
				}
				
				else if (command[0] == 0x0D)
				{
					fprintf(txt, "Pitch slide: %i\n", (signed char)command[1]);
					seqPos += 2;
				}

				else if (command[0] == 0x0E)
				{
					repeat1 = command[1];
					repeat1Pt = ReadBE16(&romData[seqPos + 2]);
					fprintf(txt, "Repeat (v1): %i times, 0x%04X\n", repeat1, repeat1Pt);
					seqPos += 4;
				}

				else if (command[0] == 0x0F)
				{
					repeat2 = command[1];
					repeat2Pt = ReadBE16(&romData[seqPos + 2]);
					fprintf(txt, "Repeat (v2): %i times, 0x%04X\n", repeat2, repeat2Pt);
					seqPos += 4;
				}

				else if (command[0] == 0x10)
				{
					repeat3 = command[1];
					repeat3Pt = ReadBE16(&romData[seqPos + 2]);
					fprintf(txt, "Repeat (v3): %i times, 0x%04X\n", repeat3, repeat3Pt);
					seqPos += 4;
				}

				else if (command[0] == 0x11)
				{
					repeat4 = command[1];
					repeat4Pt = ReadBE16(&romData[seqPos + 2]);
					fprintf(txt, "Repeat (v4): %i times, 0x%04X\n", repeat4, repeat4Pt);
					seqPos += 4;
				}

				else if (command[0] == 0x12)
				{
					flag1 = command[1];
					break1 = ReadBE16(&romData[seqPos + 2]);
					fprintf(txt, "Break (v1): %01X, address 0x%04X\n", flag1, break1);
					seqPos += 4;
				}

				else if (command[0] == 0x13)
				{
					flag2 = command[1];
					break2 = ReadBE16(&romData[seqPos + 2]);
					fprintf(txt, "Break (v2): %01X, address 0x%04X\n", flag2, break2);
					seqPos += 4;
				}

				else if (command[0] == 0x14)
				{
					flag3 = command[1];
					break3 = ReadBE16(&romData[seqPos + 2]);
					fprintf(txt, "Break (v3): %01X, address 0x%04X\n", flag3, break3);
					seqPos += 4;
				}

				else if (command[0] == 0x15)
				{
					flag4 = command[1];
					break4 = ReadBE16(&romData[seqPos + 2]);
					fprintf(txt, "Break (v4): %01X, address 0x%04X\n", flag4, break4);
					seqPos += 4;
				}

				else if (command[0] == 0x16)
				{
					loopPt = ReadBE16(&romData[seqPos + 1]);
					fprintf(txt, "Go to loop point: 0x%04X\n", loopPt);
					seqPos += 3;
				}
				
				else if (command[0] == 0x17)
				{
					fprintf(txt, "End of sequence\n");
					seqEnd = 1;
				}

				else if (command[0] == 0x18)
				{
					fprintf(txt, "Set duty: %i\n", command[1]);
					seqPos += 2;
				}

				else if (command[0] == 0x19)
				{
					fprintf(txt, "Set envelope?: %i\n", command[1]);
					seqPos += 2;
				}

				else if (command[0] >= 0x20)
				{
					curNote = command[0];
					fprintf(txt, "Play note: %01X\n", curNote);
					seqPos++;
				}

				else
				{
					fprintf(txt, "Unknown command: %01X\n", command[0]);
					seqPos += 2;
				}
			}
			fprintf(txt, "\n");
		}
		fclose(txt);
	}
}