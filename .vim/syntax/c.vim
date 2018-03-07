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

"custom c
syn	match	cFunctions	"\<[a-zA-Z_][a-zA-Z_0-9]*\>[^()]*)("me=e-2
syn	match	cFunctions	"\<[a-zA-Z_][a-zA-Z_0-9]*\>\s*("me=e-1
"syn	match	cOperators	"\v[\+-\*/\=\>\<\!\|\&\~]"
syn	match	cOperators	"\v\+"
syn	match	cOperators	"\v-"
syn	match	cOperators	"\v\*"
syn	match	cOperators	"\v/"
syn	match	cOperators	"\v\="
syn	match	cOperators	"\v\>"
syn	match	cOperators	"\v\<"
syn	match	cOperators	"\v\!"
syn	match	cOperators	"\v\~"
syn	match	cOperators	"\v\|"
syn	match	cOperators	"\v\&"

hi	cFunctions			ctermfg=lightblue	ctermbg=black
hi	link	cOperators	Operator
