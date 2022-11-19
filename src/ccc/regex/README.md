# Regex System :abc:

**WARNING !** The implementation of the ccc regex system does not match the standard regex specifications.
## Syntax:
| Token        | Name         | Description                                  |
|--------------|--------------|----------------------------------------------|
|              | Space        | Spaces are ignored                           |
| _            | Whitespace   | Whitespaces character set                    |
| a            | Alpha        | [A-Za-z]                                     |
| o            | Operators    | Operator character set                       |
| n            | Numbers      | [0-9]                                        |
| Q            | Double Quote | " Avoids character escaping                  |
| q            | Single Quote | ' Avoids character escaping                  |
| ^            | Any          | Matches any character (can't be an ok state) |
| /            | Dash         | Discards every match after '/'               |
| '...'        | Text         | Literal text between quotes                  |
| {...}        | Sequence     | Declare sub-pattern between braces           |
| \<a\>\|\<b\> | Or           | Matches if either \<a\> or \<b\> matches     |
| \<op\>?      | Quest        | Optional match of \<op\>                     |
| \<op\>*      | Star         | Matches zero or more \<op\>                  |
| \<op\>+      | Plus         | Matches one or more \<op\>                   |
| \<a\>~\<b\>  | Wave         | Matches \^ between \<a\> and \<b\>           |

## Examples:
```cpp
" 'abc' "_rx.match("abc") -> "abc"
" 'a' "_rx.match("abc") -> "a"
" 'abc' "_rx.match("a") -> ""

"a|n"_rx.match("f") -> "f"
"a|n"_rx.match("f") -> "1"

"'ab'/'c'"_rx.match("abc") -> "ab"
"'ab'/'c'"_rx.match("abe") -> ""

"{'-'|'+'}? n+ '.' n+ 'f'?"_rx.match("243.02f;") -> "243.02f"
"{a|'_'} {a|'_'|n}*"_rx.match("MyVec2i + MyVec2f") -> "MyVec2i"
"Q~Q"_rx.match("\"Hello world !\";") -> "\"Hello world !\""
```

## Structures:
> Each control flow token \[|?*+~\] generates a node structure

```cpp
# From graphviz.cpp
# <?> Error
# <$> Epsilon state
# </> Dash state
# <^> Any state
# '...' Text
# [a..b] Range
```

### Or
![Or Graph](/docs/graph_or.svg)


### Quest
![Quest Graph](/docs/graph_quest.svg)


### Star
![Star Graph](/docs/graph_star.svg)


### Plus
![Plus Graph](/docs/graph_plus.svg)


### Wave
![Wave Graph](/docs/graph_wave.svg)
