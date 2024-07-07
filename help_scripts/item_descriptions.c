#include <stdio.h>
#include <string.h>

#define TXT "A spray-type medicine for treating wounds. It can be used to restore 20 HP to a Pokémon."

int main() {
    char *TEXT = TXT;
    char dest[500] = " ", *lineStart, buffer[500] = " ";
    long k = 0, l = 0;

    lineStart = &dest[0];
    strcpy(buffer, TEXT);

    while (buffer[k] != '\0')
    {
        if (strlen(lineStart) >= 36)
        {
            do
            {
                l--;
                k--;
            } while (buffer[k] != ' ' && buffer[k] != '\n');

            if (buffer[k + 1] != '\0')
            {
                dest[l] = 0x5C;
                dest[l + 1] = 'n';
                
                l++;
            }

            l++;
            k++;
            lineStart = &dest[l];
            continue;
        }
        dest[l] = buffer[k];

        if (buffer[k] == '\n')
        {
            if (buffer[k - 1] != ' ')
                dest[l] = ' ';
            else
                l--;
        }
        l++;
        k++;
    }
    dest[l] = '\0';

    printf(dest);
}