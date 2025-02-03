#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define GET_R(color) ((color) & 0x1F)
#define GET_G(color) (((color) >> 5) & 0x1F)
#define GET_B(color) (((color) >> 10) & 0x1F)

static bool HasCharacter(char string[])
{
	int i = 0;
	
	while (string[i] != '\0')
	{
		if ((string[i] >= 'a' && string[i] <= 'f') || (string[i] >= 'A' && string[i] <= 'F'))
			return true;
		
		++i;
	}
	return false;
}

// Converts a four digits hex value to the GBA's RGB color mode(values between 0 and 31)
int main()
{
    int hexNum, startIndex;
    char hexString[5], input[7];
    bool isHexNumber;
    
	// Get input
    printf("Input the hex color code: ");
    scanf("%7s", input);
	
	// Copy the four digits
	isHexNumber = (strstr(input, "0x") != NULL);
	startIndex = isHexNumber ? 2 : 0;
	input[5 + startIndex] = '\0';
	strcpy(hexString, input + startIndex);
	
	// Check if its a hex number without the '0x' prefix
	if (!isHexNumber && HasCharacter(hexString))
		printf("\nError: Hex number without '0x' prefix. Add the hex prefix and run again.");
	else
	{
		// Convert the hex number to the GBA's RGB color mode
		hexNum = (int)strtol(hexString, NULL, isHexNumber ? 16 : 0);
		printf("\nBinary code: %i\n\nR: %i\nG: %i\nB: %i", hexNum, GET_R(hexNum), GET_G(hexNum), GET_B(hexNum));
	}
    return 0;
}
