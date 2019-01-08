p_screen.obj:	p_screen.c porolith.h komomo.h
		cl /DPC98 /DBGM /Ox /c /AL /W3 /J p_screen.c

p_block.obj:	p_block.c porolith.h komomo.h
		cl /DPC98 /DBGM /Ox /c /AL /W3 /J p_block.c

p_file.obj:	p_file.c porolith.h komomo.h
		cl /DPC98 /DBGM /Ox /c /AL /W3 /J p_file.c

p_main.obj:	p_main.c porolith.h komomo.h
		cl /DPC98 /DBGM /Ox /c /AL /W3 /J p_main.c

p_data.obj:	p_data.c porolith.h komomo.h
		cl /DPC98 /DBGM /Ox /c /AL /W3 /J p_data.c

p_input.obj:	p_input.asm
		masm /Ml p_input.asm p_input.obj nul nul

porolith.exe:	p_screen.obj p_block.obj p_file.obj p_main.obj p_data.obj p_input.obj
		cl p_screen.obj p_block.obj p_file.obj p_main.obj p_data.obj p_input.obj -o porolith.exe bgmlibl.lib+graphics.lib
