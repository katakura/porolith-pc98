DGROUP	group	_DATA

_DATA	segment	word public 'DATA'
_DATA	ends

public	_kbhit2, _getch2

_TEXT	segment	word public 'CODE'
	assume	cs:_TEXT,ss:DGROUP,ds:DGROUP

even
_kbhit2	proc far	;near
	mov	ah,1
	int	18h
	xor	ah,ah
	mov	al,bh
	ret
_kbhit2	endp
even
_getch2	proc far	;near
	xor	ah,ah
	int	18h
	mov	al,ah
	xor	ah,ah	;returns scan code
	ret
_getch2	endp

_TEXT	ends
	end
