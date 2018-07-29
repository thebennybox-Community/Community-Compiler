" Vim syntax file
" Language: Dusk
" Maintainer: thebennybox-Community
" Latest Revision: 21 July 2018

if exists("b:current_syntax")
  finish
endif

syn keyword KEYWORDS  break continue else extern false fn if in impl import infix let loop prefix return struct suffix true var

syn match   NUMBERDEC "\d(?:_|\d)*(?:\.(?:_|\d)\+)?"
syn match   NUMBERBIN "0b[01_]\+"
syn match   NUMBERHEX "0x[0-9A-Fa-f_]\+"

syn match   SINGLLINE "//.*$"

syn region  CODEBLOCK start="{" end="}" fold transparent
syn region  MULTILINE start="/\*" end="\*/" fold
syn region  STRING start='"' end='"' skip=+\\\\\|\\"+

hi def link KEYWORDS  Statement

hi def link NUMBERDEC Number
hi def link NUMBERBIN Number
hi def link NUMBERHEX Number
hi def link MULTILINE Comment
hi def link SINGLLINE Comment

let b:current_syntax = "dusk"

