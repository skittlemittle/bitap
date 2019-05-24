#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* prints stuff in binary, assumes little endian */
void print_bits(size_t const size, void const * const ptr)
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
    int pattern_len = strlen(pattern);
    unsigned long R;
    unsigned long pattern_mask[CHAR_MAX+1];
    int i;
 
    if (pattern[0] == '\0')
        return text;
    if (pattern_len > 31)
        return "The pattern is too long!";
  
    /* Initialize the bit array R */
    R = ~1;
    printf("%s\n%x ", "=====R===== ", R);
    print_bits(1UL, &R);

    /* Initialize the pattern bitmasks */
    for (i = 0; i <= CHAR_MAX; ++i) {
        pattern_mask[i] = ~0; // pattern masks = binary 1
    }
    printf("%s\n", "=====pattern_mask of each char of the pattern=====");
    for (i = 0; i < pattern_len; ++i) {
        pattern_mask[pattern[i]] &= ~(1UL << i);
        printf("%x\t", pattern_mask[pattern[i]]);
        print_bits(1UL, &pattern_mask[pattern[i]]);
    }

    printf("\n%s\n", "=====finding a match=====");
    for (i = 0; text[i] != '\0'; ++i) {
        printf("\n%s", "pattern_mask of the text  ");
        print_bits(1UL, &pattern_mask[text[i]]);

        /* Update the bit array */
        R |= pattern_mask[text[i]];
        R <<= 1;

        printf("%s", "R                         ");
        print_bits(1UL, &R);

        int isRzero = (R & (1UL << pattern_len)); // so print_bits doesn't panic
        printf("%s", "R & (1UL << pattern_len)            ");
        print_bits(1UL, &isRzero);
 
        if (0 == (R & (1UL << pattern_len)))
            return (text + i - pattern_len) + 1;
    }
 
    return NULL;
}

int main(int argc, char const *argv[])
{
    char pattern[100], txt[500];
    printf("%s\n", "enter: text pattern ");
    scanf("%s %s", txt, pattern);

    printf("%s\n", bitap_bitwise_search(txt, pattern));
    return 0;
}