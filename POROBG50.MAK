p_screen.obj:	p_screen.c porolith.h komomo.h
		cl /DFMR /DBGMFM /Ox /c /AL /W3 /J p_screen.c

p_block.obj:	p_block.c porolith.h komomo.h
		cl /DFMR /DBGMFM /Ox /c /AL /W3 /J p_block.c

p_file.obj:	p_file.c porolith.h komomo.h
		cl /DFMR /DBGMFM /Ox /c /AL /W3 /J p_file.c

p_main.obj:	p_main.c porolith.h komomo.h
		cl /DFMR /DBGMFM /Ox /c /AL /W3 /J p_main.c

p_data.obj:	p_data.c porolith.h komomo.h
		cl /DFMR /DBGMFM /Ox /c /AL /W3 /J p_data.c

poro_r50.exe:	p_screen.obj p_block.obj p_file.obj p_main.obj p_data.obj
		cl /Ox /AL /W3 /J p_screen.obj p_block.obj p_file.obj p_main.obj p_data.obj -o poro_r50.exe \r50lib\graphics.lib+soundys.lib
