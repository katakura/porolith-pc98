/*
		Action Puzzle Game "POROLITH"

			PC-9801,286
			FMR50/60/70
			Dynabook
			and MSDOS Machine

	Copyright(C)1990 by Ko.Mo.Mo. Allrights Reserved.
*/

/*	グラフィック制御を簡素化するマクロ定義
*/
#define		X(x)	(x<<4)
#define		Y(y)	(y<<4)
#define		CG_BLACK	0
#define		CG_BLUE		1
#define		CG_RED		2
#define		CG_MAGENDA	3
#define		CG_GREEN	4
#define		CG_CYAN		5
#define		CG_YELLOW	6
#define		CG_WHITE	7
#define		TEX_BLACK	30
#define		TEX_RED		31
#define		TEX_GREEN	32
#define		TEX_YELLOW	33
#define		TEX_BLUE	34
#define		TEX_MAGENDA	35
#define		TEX_CYAN	36
#define		TEX_WHITE	37
#define		TEX_R_BLACK	40
#define		TEX_R_RED	41
#define		TEX_R_GREEN	42
#define		TEX_R_YELLOW	43
#define		TEX_R_BLUE	44
#define		TEX_R_MAGENDA	45
#define		TEX_R_CYAN	46
#define		TEX_R_WHITE	47
#define		ESC_BLACK	"\x1b[30m"
#define		ESC_RED		"\x1b[31m"
#define		ESC_GREEN	"\x1b[32m"
#define		ESC_YELLOW	"\x1b[33m"
#define		ESC_BLUE	"\x1b[34m"
#define		ESC_MAGENDA	"\x1b[35m"
#define		ESC_CYAN	"\x1b[36m"
#define		ESC_WHITE	"\x1b[37m"
#define		ESC_R_BLACK	"\x1b[40m"
#define		ESC_R_RED	"\x1b[41m"
#define		ESC_R_GREEN	"\x1b[42m"
#define		ESC_R_YELLOW	"\x1b[43m"
#define		ESC_R_BLUE	"\x1b[44m"
#define		ESC_R_MAGENDA	"\x1b[45m"
#define		ESC_R_CYAN	"\x1b[46m"
#define		ESC_R_WHITE	"\x1b[47m"
#define		ESC_CLS		"\x1b[2J"
#define		locate(x,y)						\
			printf("\x1b[%d;%dH",y+1,x+1)
#define		color(col)						\
			printf("\x1b[%dm",col)
#define		gcolor(col)						\
			_setcolor(col)
#ifdef	DYNA
#define		gstart							\
			printf("\x1b[>1h") ;				\
			printf("\x1b[>5h") ;				\
			printf("%s",ESC_CLS)
#define		gstop							\
			printf("%s",ESC_CLS) ;				\
			printf("\x1b[>5l") ;				\
			printf("\x1b[>1l") ;				\
			printf("%s",ESC_CLS)
#endif
#ifdef	ANSI
#define		gstart							\
			printf("\x1b[>1h") ;				\
			printf("\x1b[>5h") ;				\
			printf("%s",ESC_CLS)
#define		gstop							\
			printf("%s",ESC_CLS) ;				\
			printf("\x1b[>5l") ;				\
			printf("\x1b[>1l") ;				\
			printf("%s",ESC_CLS)
#endif
#ifdef	FMR
#define		gstart							\
			_displaycursor(_GCURSOROFF) ;			\
			_setvideomode(_FMRESSCOLOR) ;			\
			_clearscreen(_GCLEARSCREEN)
#define		gstop							\
			_displaycursor(_GCURSORON) ;			\
			_setvideomode(4/*_FMTEXT8024*/) ;		\
			_clearscreen(_GCLEARSCREEN)
#endif
#ifdef	PC98
#define		gstart							\
			printf("\x1b[>1h") ;				\
			_displaycursor(_GCURSOROFF) ;			\
			_setvideomode(_98RESS8COLOR) ;			\
			_clearscreen(_GCLEARSCREEN)
#define		gstop							\
			_displaycursor(_GCURSORON) ;			\
			_clearscreen(_GCLEARSCREEN) ;			\
			printf("\x1b[>1l")
#endif
#ifdef	DYNA
#define		cls							\
			printf("\x1b[2J")
#else
#define		cls							\
			_clearscreen(_GCLEARSCREEN)
#endif
#define		line(x1,y1,x2,y2,col)					\
			_moveto(x1,y1) ;				\
			_setcolor(col) ;				\
			_lineto(x2,y2)
#define		box_full(x1,y1,x2,y2)					\
			_rectangle(_GFILLINTERIOR,x1,y1,x2,y2)
