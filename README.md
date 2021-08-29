**Pattern based replace allows to perform find and replace using intuitive patterns**

The tool allows search and replace text using patterns. It is very useful if you want to replace some macro or functions in your code with new syntax. For example:
```
cepi_callback(PenInterface, ptr, WM_LBUTTONDOWN, FW, ActivateTexPrv)

replace to

FW->connect(WM_LBUTTONDOWN, &PenInterface::ActivateTexPrv, ptr);

```
in this case you may define replacement pattern

```
cepi_callback($expression_Class,$expression_ptr,$expression_msg,$expression_w,$expression_func)
=====>
$expression_w->connect($expression_msg, &$expression_Class::$expression_func, $expression_ptr)

```
The idea is to find text by named patterns and substitute it into the new expression. All tokens consist of  
**$name_of_token**_Your_own_id  
Your_own_id used to distinguish the token from other ones and substitute it into the correct place.  

```
Usage:  
SmartReplacer.exe [options]  
Options list:  
  /i       Input file.  
  /o       Output file. If not specified, the file will be overwritten.  
  /p       Pattern file.  
All next options are for multiple files batch processing:  
  /r       Run recursively through files in the folder.  
  /f       Run non-recursively through files in the folder.  
  /c       Copy all changed files to this folder. By default all changed files will be copied to the folder Changed/ 
  /remove  Remove everything from the folder with changes to place there only fresh results.
  /.ext    Accept all files with this extension. *.* is not acceptable, you should write extension explicitly.  
           You may provide multiple extensions.  

List of tokens:
 $string - quoted string "...".
 $characters - regular characters - [a..z,A..Z,0..9,_,.].
 $floating - floating-point value.
 $numbers - any amount of numbers [0..9].
 $expr - correct expression.*
 $variable - correct variable name.
 $block - block between brackets { ... }.
 $square - block between square brackets [ ... ].
 $brackets - block between round brackets ( ... ).

***When we say "correct expression" we mean:  
The expression contains the same amount of vice-versa brackets ''(){}[]/**/. May contain any charactes within any sort of brackets.  
Outside the brackets scope it may contain #a..z A..Z 0..9 () [] {} +- *&~\!:_/* and spaces.  
Comma ',' may not be outside the brackets scope, comma ends the expression.  

Use $zero token to mark next token as optional.  

Format of pattern file:  

pattern_to_search_1
=====>
pattern_to_replace_1
=============================
pattern_to_search_2
=====>
pattern_to_replace_2
=============================
....
```