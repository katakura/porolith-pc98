/*
		Action Puzzle Game "POROLITH"

			PC-9801,286
			FMR50/60/70
			Dynabook
			and MSDOS Machine

	Copyright(C)1990 by Ko.Mo.Mo. Allrights Reserved.
*/
#define		CG_BLACK	0
#define		CG_BLUE		1
#define		CG_RED		2
#define		CG_MAGENDA	3
#define		CG_GREEN	4
#define		CG_CYAN		5
#define		CG_YELLOW	6
#define		CG_WHITE	7
#define		gstart							\
			printf("\x1b[>1h") ;				\
			_displaycursor(_GCURSOROFF) ;			\
			_setvideomode(_98RESS8COLOR) ;			\
			_clearscreen(_GCLEARSCREEN)
#define		gstop							\
			_displaycursor(_GCURSORON) ;			\
			_clearscreen(_GCLEARSCREEN) ;			\
			printf("\x1b[>1l")
#define		cls							\
			_clearscreen(_GCLEARSCREEN)
#define		MASK_NORMAL	0
#define		MASK_REVERCE	1

#define		READ_OK		0
#define		READ_NG		1
extern int
font_read(char *fname, char *envname);
extern void
mask_set(char rr, char gg, char bb, int rev);
extern void
out_text(int x, int y, char *str);
extern void
font_unread(void);
extern int
font_select(int);
