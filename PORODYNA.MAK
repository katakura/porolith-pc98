p_block.obj:	p_block.c porolith.h komomo.h
		cl /DDYNA /Ox /c /AL /W3  /J p_block.c

p_file.obj:	p_file.c porolith.h komomo.h
		cl /DDYNA /Ox /c /AL /W3  /J p_file.c

p_main.obj:	p_main.c porolith.h komomo.h
		cl /DDYNA /Ox /c /AL /W3  /J p_main.c

p_data.obj:	p_data.c porolith.h komomo.h
		cl /DDYNA /Ox /c /AL /W3  /J p_data.c

p_screen.obj:	p_screen.c porolith.h komomo.h
		cl /DDYNA /Ox /c /AL /W3  /J p_screen.c

porodyna.exe:	p_screen.obj p_block.obj p_file.obj p_main.obj p_data.obj
		cl p_screen.obj p_block.obj p_file.obj p_main.obj p_data.obj -o porodyna.exe
