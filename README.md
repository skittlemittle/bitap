# bitap
The bitap algorithm written in C, with printf's everywhere so you can see
how it works.

NOTE: printf can't print in binary so I'm using a function `print_bits` to do it,
      and it takes the size of the thing it's going to print, and I pass 1UL
      as the size each time, so if you want to see more bits pass 
      `sizeof(thing_you_wanna_print)` as the size.

# How this thing works:

you give it some text and a pattern to find, for this example:
text: `skate` pattern: `ate`

the code also has a bit array `R` which is what it works on to find a match,
and an array `pattern_mask` of size `CHAR_MAX+1`.
`pattern_len` is the length of the pattern.

`R` gets set to ~1 or 31 `1's` followed by a `0`.
Each element in `pattern_mask` gets set to ~0 or 32 `1's`.

### Making the pattern mask:

Now a mask is made of each char in `pattern` by doing this:
``` c
pattern_mask[pattern[i]] &= ~(1UL << i);
```

so `ate` gets a pattern mask of:
```
11111110  a
11111101  t
11111011  e
```
### Finding a match:

Loop through the text and do:

``` c
R |= pattern_mask[text[i]];
R <<= 1;
```

This goes and OR's R with the pattern mask of each character in the text,
*chars that don't have a pattern mask don't matter since the mask is made for
each char in the pattern and thats all we want.*
then it left shifts R by 1 bit.

in our case that looks like so:

```
# notice how chars that don't have a mask
# just show up as 11111111

#S
pattern_mask of the text  11111111
R                         11111110

#K
pattern_mask of the text  11111111
R                         11111110

#A
pattern_mask of the text  11111110
R                         11111100

#T
pattern_mask of the text  11111101
R                         11111010

#E
pattern_mask of the text  11111011
R                         11110110
```

Each iteration through the loop check to see if`0 == (R & (1UL << pattern_len))`
returns true, if it does you have a match, for our example this looks like:

```
R & (1UL << m)            00001000 S

R & (1UL << m)            00001000 K

R & (1UL << m)            00001000 A

R & (1UL << m)            00001000 T

# we found a match
R & (1UL << m)            00000000 E
```
This means it finds a match at the last char of the match, so when returning 
the match it does it like this:
``` c
if (0 == (R & (1UL << m)))
            return (text + i - m) + 1;
```