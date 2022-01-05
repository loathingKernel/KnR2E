# Exercise 1 - 23

## Problem
>Write a program to remove all comments from a C program.
Do not forget to handle quoted strings and character constants properly.
C comments do not nest.

## What is a comment
>Except within a character constant, a string literal, or a comment,
the characters `/*` introduce a comment. The contents of such a comment are
examined only to identify multibyte characters and to find the
characters `*/` that terminate it.

```C
/* This is a comment */

/* This is
   a comment */

'/* This is not a comment */'

"/* This is not a comment */"
```

>Except within a character constant, a string literal, or a comment,
the characters `//` introduce a comment that includes all multibyte characters
up to, but not including, the next new-line character. The contents of such
a comment are examined only to identify multibyte characters and to find the
terminating new-line character.

```C
// This is a comment

'// This is not a comment'

"// This is not a comment"
```

## What about line splices and trigraph sequences?
### Line splices
Line splices or line continuations, are constructs which allow a single
logical line to span over multiple physical lines. These constructs consist of
a backslash `\` character immediately followed by a newline character. According
to this the following examples will eventually become comments.

```C
//\
 This will be a comment.

/\
/ This will be a comment.

/\
/\
 This \
will be \
a \
comment \
that \
ends \
here.
```

### Trigraph sequences
>Before any other processing takes place, each occurrence of one of the
following sequences of three characters (called trigraph sequences)) is replaced
with the corresponding single character.
```
??=     #                       ??)     ]                       ??!     |
??(     [                       ??'     ^                       ??>     }
??/     \                       ??<     {                       ??-     ~
```
>No other trigraph sequences exist. Each `?` that does not begin one of the
trigraphs listed above is not changed.

Two of those are most relevant for this use case. The `??/` which translates to
`\` and as such can be used to construct a sinle line comment and
escape sequences. The second one is `??'` which translates to `^` and can
confuse character constant detection. So the potential comments above can be
written also as follows.

```C
//??/
 This will be a comment.

/??/
/ This will be a comment.

/??/
/??/
 This ??/
will be ??/
a ??/
comment ??/
that ??/
ends ??/
here.

'??'/this will not be a comment/'
```

So should these cases be handled? Well, they should, but not before the
translation of these constructs take place. According to the C11 Standard the
translation phases relevant to this case are described as follows.

>1. Physical source file multibyte characters are mapped, in an implementation-
defined manner, to the source character set (introducing new-line characters for
end-of-line indicators) if necessary. Trigraph sequences are replaced by
corresponding single-character internal representations.

>2. Each instance of a backslash character (`\`) immediately followed by a
new-line character is deleted, splicing physical source lines to form
logical source lines. Only the last backslash on any physical source line shall
be eligible for being part of such a splice. A source file that is not empty
shall end in a new-line character, which shall not be immediately preceded by
a backslash character before any such splicing takes place.

>3. The source file is decomposed into preprocessing tokens) and sequences of
white-space characters (including comments). A source file shall not end in a
partial preprocessing token or in a partial comment. Each comment is replaced by
one space character. New-line characters are retained. Whether each nonempty
sequence of white-space characters other than new-line is retained or replaced
by one space character is implementation-defined.

To my understanding, following the word of the C11 Standard, the only
constructs that constitute comments are `/*..*/` and
`//...*new-line character*`. Comments constructed using line splices and
trigraph sequences will become literatim comments after the first two steps of
the translation have taken place. So, in this case the sequence of processing
can be:

1. Replace trigraph sequences

2. Splice physical lines to logical lines if they end in a backslash followed
by a newline

3. Remove comments

## Implementation and solution
Both solutions use only material taught thus far in the book.

Variant `ex1-23_a` works using these rules. As such, the output will not be just
comment removal, it will also contain trigraph replacement and line splicing.

Variant `ex1-23_b` does not do line splicing and also reverts trigraph sequence
replacement.

In my opinion variant `a` is more correct and cleaner. The code is more compact
and does not have to deal with too many cases. Variant `b` has too many moving
parts for what it does and was kind of beaten with a hammer to work.

That being said, in the same tests their output was equivalent with the
exception of trigraph sequences and line splicing.

## Tests
Here you can find some of the tests used while developing this solution
### Input
```C
"a//b"                             //   four-character string literal
#include "//e"                     //   undefined behavior
// */                              //   comment, not syntax error
f = g/**//h;                       //   equivalent to f = g / h;
//\
i();                               // part of a two-line comment
/\
/ j();                             // part of a two-line comment
#define glue(x,y) x##y
glue(/,/) k();                     // syntax error, not comment
/*//*/ l();                        // equivalent to l();
m = n//**/o
   + p;                            // equivalent to m = n + p;

/\
 / j();                            // not a comment

char *test = "/* asa\
dsa*/";

char *test2 = "asdasd \" // asdasaasd";
char *test3 = "asdas \' \" /*asdasd*/";

//
char *p = "\\";
/*/
char *p = "\\"; /* This is a comment. */

#include <stdio.h>
int main(void)
{
    /* print the number three */
    printf("%d\n", 6/'\2');
    /* remember to return a value from main */
    return 0;
}

'??'/this is not a comment/'
```

### Output
```C
"a//b"                             
#include "//e"                     

f = g /h;                       


#define glue(x,y) x##y
glue(/,/) k();                     
  l();                        
m = n
   + p;                            

/\
 / j();                            

char *test = "/* asa\
dsa*/";

char *test2 = "asdasd \" // asdasaasd";
char *test3 = "asdas \' \" /*asdasd*/";


char *p = "\\";
 

#include <stdio.h>
int main(void)
{
     
    printf("%d\n", 6/'\2');
     
    return 0;
}

'??'/this is not a comment/'
```
