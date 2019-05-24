#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* prints stuff in binary, assumes little endian */
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--) {
        for (j=7;j>=0;j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

/* the bitap algorithm */
const char *bitap_bitwise_search(const char *text, const char *pattern)
{
    int m = strlen(pattern);
    unsigned long R;
    unsigned long pattern_mask[CHAR_MAX+1];
    int i;
 
    if (pattern[0] == '\0')
        return text;
    if (m > 31)
        return "The pattern is too long!";
  
    /* Initialize the bit array R */
    R = ~1;
    printf("%s\n%x ", "=====R===== ", R);
    printBits(1UL, &R);

    /* Initialize the pattern bitmasks */
    for (i = 0; i <= CHAR_MAX; ++i) {
        pattern_mask[i] = ~0; // pattern masks = binary 1
    }
    printf("%s\n", "=====pattern_mask of each char of the pattern=====");
    for (i = 0; i < m; ++i) {
        pattern_mask[pattern[i]] &= ~(1UL << i);
        printf("%x\t", pattern_mask[pattern[i]]);
        printBits(1UL, &pattern_mask[pattern[i]]);
    }

    printf("\n%s\n", "=====finding a match=====");
    for (i = 0; text[i] != '\0'; ++i) {
        printf("\n%s", "pattern_mask of the text  ");
        printBits(1UL, &pattern_mask[text[i]]);

        /* Update the bit array */
        R |= pattern_mask[text[i]];
        R <<= 1;

        printf("%s", "R                         ");
        printBits(1UL, &R);

        int isRzero = (R & (1UL << m)); // so printBits doesn't freak out
        printf("%s", "R & (1UL << m)            ");
        printBits(1UL, &isRzero);
 
        if (0 == (R & (1UL << m)))
            return (text + i - m) + 1;
    }
 
    return NULL;
}


int main() 
{
    char pattern[100], txt[500];
    printf("%s\n", "enter: text pattern ");
    scanf("%s %s", txt, pattern);

    printf("%s\n", bitap_bitwise_search(txt, pattern));
}