#include	<stdio.h>
#include	<graph.h>
#include	"porofont.h"
char            fname[8][20] = {
				"BABORE16.FNT",
				"FZ.FNT",
				"GRA.FNT",
				"GRANAME.FNT",
				"LANGDON.FNT",
				"MEGATEN.FNT",
				"THREE16.FNT",
				"XEV.FNT"
};

main()
{
	int             i;
	int             rtn;
	char            buf1[30];
	char            buf2[30];

	strcpy(buf1, " !\"#$%'()*+,-./0123456789:;<=");
	strcpy(buf2, ">?@ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	printf("\x1b*■Taco.x氏作fontlib1画面展開ぷろぐらむ～\n\n");
	for (i = 0; i < 8; i++) {
		printf("'%12s' Reading ", fname[i]);
		font_select(i);
		rtn = font_read(fname[i], "TACOX");
		if (rtn == READ_NG) {
			printf(" / NG!\n");
			font_unread();
			return (0);
		} else
			printf("\n");
	}
	printf("\n読み込み終了。何かキーをおしてやってくれや。\n");
	getch();
	gstart;
	for (i = 0; i < 8; i++) {
		font_select(i);
		mask_set(0xff, 0xff, 0xff, MASK_NORMAL);
		out_text(0, i * 3, fname[i]);
		switch (i) {
		case 0:
			mask_set(0xaa, 0xff, 0xff, MASK_NORMAL);
			break;
		case 1:
			mask_set(0xff, 0xaa, 0xff, MASK_NORMAL);
			break;
		case 2:
			mask_set(0xff, 0xff, 0xaa, MASK_NORMAL);
			break;
		case 3:
			mask_set(0xaa, 0xff, 0xff, MASK_NORMAL);
			break;
		case 4:
			mask_set(0xff, 0xaa, 0xff, MASK_NORMAL);
			break;
		case 5:
			mask_set(0xff, 0xff, 0xaa, MASK_NORMAL);
			break;
		case 6:
			mask_set(0xaa, 0xff, 0xff, MASK_NORMAL);
			break;
		case 7:
			mask_set(0xff, 0xaa, 0xff, MASK_NORMAL);
			break;
		}
		out_text(4, i * 3 + 1, buf1);
		out_text(4, i * 3 + 2, buf2);
	}
	getch();
	gstop;
	font_unread();
}
