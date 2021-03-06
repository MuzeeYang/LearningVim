"0		Black
"1		DarkBlue
"2		DarkGreen
"3		DarkCyan
"4		DarkRed
"5		DarkMagenta
"6		Brown, DarkYellow
"7		LightGray, LightGrey, Gray, Grey
"8		DarkGray, DarkGrey
"9		Blue, LightBlue
"10		Green, LightGreen
"11		Cyan, LightCyan
"12		Red, LightRed
"13		Magenta, LightMagenta
"14		Yellow, LightYellow
"15		White

"========================================================
" Highlight All Math Operator
"========================================================
syn match		cAssignOperator	  display "="
" C math operators
syn match       cMathOperator     display "[-+\*/%]"
syn match       cMathOperator     display "[-+\*/%]=\="
" C pointer operators
syn match       cPointerOperator  display "->\|\."
" C logical   operators - boolean results
syn match       cLogicalOperator  display "[!<>]=\="
syn match       cLogicalOperator  display "=="
" C bit operators
syn match       cBinaryOperator   display "\(&\||\|\^\|<<\|>>\)=\="
syn match       cBinaryOperator   display "\~"
syn match       cBinaryOperator	  display "\~="
" More C logical operators - highlight in preference to binary
syn match       cLogicalOperator  display "&&\|||"
syn match       cLogicalOperator  display "\(&&\|||\)="

" Math Operator
hi link cAssignOperator			Operator
hi link cMathOperator           Operator 
hi link cLogicalOperator        Operator
hi link cPointerOperator        Operator
hi link cBinaryOperator         Operator

