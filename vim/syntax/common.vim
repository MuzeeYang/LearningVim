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


let g:colors_name="earthmelon_8"

set background=dark

if version > 580
    hi clear
    if exists("syntax_on")
        syntax reset
    endif
endif


hi	user1				ctermfg=white		ctermbg=red
hi	normal				ctermfg=white		ctermbg=black
hi	colorcolumn			ctermfg=white		ctermbg=grey

hi	errormsg			term=standout
hi	errormsg			ctermfg=lightblue	ctermbg=darkblue
hi	warningmsg			term=standout
hi	warningmsg			ctermfg=lightblue	ctermbg=darkblue
hi	modemsg				term=bold			cterm=bold
hi	modemsg				ctermfg=gray		ctermbg=black
hi	moremsg				term=bold			ctermfg=lightgreen
hi	moremsg				ctermfg=lightblue	ctermbg=black
hi	question			term=standout		gui=bold
hi	question			ctermfg=lightblue	ctermbg=black
hi	error				term=bold			cterm=bold
hi	error				ctermfg=lightblue	ctermbg=black

hi	linenr				ctermfg=darkblue	ctermbg=black
hi	cursorcolumn		ctermfg=white		ctermbg=grey
hi	cursorline			term=bold			cterm=bold
hi	cursorline			ctermfg=none		ctermbg=none
hi	colorcolumn			ctermfg=white		ctermbg=grey

hi	incsearch			ctermfg=white		ctermbg=blue
hi	search				ctermfg=white		ctermbg=blue

hi	vertsplit			ctermfg=grey		ctermbg=grey
hi	visual				term=bold			cterm=bold
hi	visual				ctermfg=black		ctermbg=grey

hi	pmenu				ctermfg=darkgray	ctermbg=darkblue
hi	pmenusel			ctermfg=white		ctermbg=darkblue

hi	comment				ctermfg=darkgray	ctermbg=black
hi	preproc				ctermfg=darkmagenta	ctermbg=black
hi	type				ctermfg=darkgreen	ctermbg=black
hi	constant			ctermfg=darkred		ctermbg=black
hi	statement			ctermfg=brown		ctermbg=black
hi	special				ctermfg=yellow		ctermbg=black
hi	specialkey			ctermfg=darkcyan	ctermbg=black
hi	operator			ctermfg=lightgreen		ctermbg=black
hi	string				ctermfg=brown		ctermbg=black
hi	number				ctermfg=red			ctermbg=black
hi	identifier			ctermfg=darkcyan	ctermbg=black
hi	todo				ctermfg=black		ctermbg=gray
hi	nontext				ctermfg=lightblue	ctermbg=black
hi	directory			ctermfg=blue		ctermbg=black
hi	folded				ctermfg=darkblue	ctermbg=black
hi	foldcolumn			ctermfg=lightblue	ctermbg=black
hi	underlined			ctermfg=lightblue	ctermbg=black	cterm=underline
hi	title				ctermfg=lightblue	ctermbg=black
hi	ignore				ctermfg=lightblue	ctermbg=black

hi	directory			ctermfg=lightblue	ctermbg=black
hi	browsesynopsis		ctermfg=lightblue	ctermbg=black
hi	browsecurdir		ctermfg=lightblue	ctermbg=black
hi	favoritedirectory	ctermfg=lightblue	ctermbg=black
hi	browsedirectory		ctermfg=lightblue	ctermbg=black
hi	browsesuffixinfo	ctermfg=lightblue	ctermbg=black
hi	browsesortby		ctermfg=lightblue	ctermbg=black
hi	browsefilter		ctermfg=lightblue	ctermbg=black
hi	browsefiletime		ctermfg=lightblue	ctermbg=black
hi	browsesuffixes		ctermfg=lightblue	ctermbg=black

"Tag
"hi	taglistcomment		ctermfg=lightblue	ctermbg=black
"hi	taglistfilename		ctermfg=lightblue	ctermbg=black
"hi	taglisttitle		ctermfg=lightblue	ctermbg=black
"hi	taglisttagscope		ctermfg=lightblue	ctermbg=black
"hi	taglisttagname		ctermfg=lightblue	ctermbg=black
"hi	Tag					ctermfg=LightBlue	ctermbg=Black

