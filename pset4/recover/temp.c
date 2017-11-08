#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "jpeg.h"

// block size of FAT filesystem (in bytes)
#define BLOCK_SIZE 512

// checks if memory contains a jpeg header
bool is_new_jpeg(BYTE *header);

int main(int argc, char *argv[])
{
	// check for commandline arguments
	if(argc != 2)
	{
		fprintf(stderr, "Usage: ./recover forensic.raw\n");
		return 1;
	}

	// store input file name
	char *img = argv[1];

	FILE *forensic = fopen(img, "r");
	if (forensic == NULL)
	{
		fprintf(stderr, "recover: %s: Unable to open input file\nCheck if the file is currency directory and system has enough memory\n", img);
		return 2;
	}

	BYTE block[BLOCK_SIZE] = {0};

	// count number of recovered images
	uint8_t recovered = 0;

	// array to store name of recovered image (9 here is for the characters ###.jpg and '\0')
	char new_jpeg[8];

	// test number of headers and blocks copied
	bool header_copied = false;

	// file pointer to recovered image
	FILE *recovered_jpeg = NULL;

	do
	{
		// becomes true if new jpeg header is found
		bool new_header = false;

		// read a single block
		fread(block, sizeof block, 1, forensic);

		// check block for jpeg header
		if ( (new_header = is_new_jpeg(block)) )
		{
			header_copied = false;

			// check for previously recovered and open file
			if (recovered > 0)
			{
				printf("recovered: %d\n", recovered);
				// close previously opened jpeg
				fclose(recovered_jpeg);
			}

			// store name of image to recover in array
			sprintf(new_jpeg, "%03d.jpg", recovered);
			
			// sanity check
			fprintf(stdout, "new_jpeg: %s\n", new_jpeg);

			// recover image 
			recovered_jpeg = fopen(new_jpeg, "w");

			if (recovered_jpeg == NULL)
			{
				fclose(forensic);
				fprintf(stderr, "recover: %s: Unable to recover or create image\n", new_jpeg);
				return 3;
			}

			fwrite(block, sizeof block, 1, recovered_jpeg);

			// count recovered image
			recovered++;

			// count copied blocks
			header_copied = true;
		}

		// copy consecutive blocks
		if ( !new_header && header_copied)
		{
			printf("Writing header body\n");

			// write jpeg body
			fwrite(block, sizeof block, 1, recovered_jpeg);
		}
	}
	while( !feof(forensic) );

	// close the last opened file
	fclose(recovered_jpeg);

	// close the forensic image
	fclose(forensic);
}


bool is_new_jpeg(BYTE *header)
{
	if (sizeof((BYTE *) header) < 4)
	{
		fprintf(stderr, "sizeof((BYTE *) header) is %lu\n", sizeof((BYTE *) header));
		return false;
	}
	if((header[0] == 0xff) && (header[1] == 0xd8) && (header[2] == 0xff) && ((header[3] & 0xf0) == 0xe0))
	{
		return true;
	}

	return false;
}