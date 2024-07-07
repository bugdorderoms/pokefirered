#include <stdio.h>
#include <string.h>

#define TXT "This form of Pikachu is somewhat rare. It wears the hat of its Trainer, who is also its partner."

int main() {
    int i;
    char *TEXT = TXT;
    char dest[2000] = " ", *lineStart, buffer[2000] = " ";
    long k = 0, l = 0;
    
    lineStart = &dest[0];
    strcpy(buffer, TEXT);
    
    while (buffer[k] != '\0')
    {
        if (strlen(lineStart) >= 42)
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
                dest[l + 2] = '\n';
                
                for (i = 0; i < 39; i++)
                    dest[l + i + 3] = ' ';
                    
                l += 41;
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