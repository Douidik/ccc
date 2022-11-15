## Regex System 🔤:
**WARNING !** This regex implementation does not follow the standard regex.
### Syntax:
| Token    | Name         | Description                          |
|----------|--------------|--------------------------------------|
|          | Space        | Spaces are ignored                   |
| _        | Whitespace   | Whitespaces character set            |
| a        | Alpha        | [A-Za-z]                             |
| o        | Operators    | Operator character set               |
| n        | Numbers      | [0-9]                                |
| Q        | Double Quote | " Avoids character escaping          |
| q        | Single Quote | ' Avoids character escaping          |
| ^        | Any          | Matches any character                |
| '...'    | Text         | Literal text between quotes             |
| {...}    | Sequence     | Declare sub-pattern between braces       |
| \<a\>\|\<b\> | Or           | Matches if either \<a\> or \<b\> matches |
| \<op\>?    | Quest        | Optional match of \<op\>               |
| \<op\>*    | Star         | Matches zero or more \<op\>            |
| \<op\>+    | Plus         | Matches one or more \<op\>             |
| \<a\>~\<b\>  | Wave         | Matches \^ between \<a\> and \<b\>       |

### Examples:
```cpp
" 'abc' "_rx.match("abc") -> "abc"
" 'a' "_rx.match("abc") -> "a"
" 'abc' "_rx.match("a") -> ""

"a|n"_rx.match("f") -> "f"
"a|n"_rx.match("f") -> "1"

"{'-'|'+'}? n+ '.' n+ 'f'?"_rx.match("243.02f;") -> "243.02f"
"{a|'_'} {a|'_'|n}*"_rx.match("MyVec2i + MyVec2f") -> "MyVec2i"
"Q~Q"_rx.match("\"Hello world !\";") -> "\"Hello world !\""
```
