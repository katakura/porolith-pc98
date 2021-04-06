/*
		Action Puzzle Game "POROLITH"

			PC-9801,286
			FMR50/60/70
			Dynabook
			and MSDOS Machine

	Copyright(C)1990 by Ko.Mo.Mo. Allrights Reserved.
*/
#include	"porolith.h"
int
main(int argc, char **argv)
{
	int             i;
	int             sta;
#ifdef	DYNA
	struct SREGS    segregs;
	union REGS      inregs, outregs;
#endif

	signal(SIGINT, porolith_abort);
	signal(SIGTERM, porolith_abort);
	strcpy(score_file, argv[0]);
	sta = 1;
	strcpy(SG_CONFIG, "porolith.cfg");
	if (argc > 1) {
		if (strstr(argv[1], "/?") || strstr(argv[1], "-?")) {
			usage_put();
			return (1);
		} else {
			if (strstr(argv[1], "/CFG=") || strstr(argv[1], "-CFG=") ||
			    strstr(argv[1], "/cfg=") || strstr(argv[1], "-cfg=")) {
				strcpy(SG_CONFIG, &argv[1][5]);
				if (!strchr(SG_CONFIG, '.'))
					strcat(SG_CONFIG, ".cfg");
				sta = 2;
			}
		}
	}
	printf("%s", ESC_CLS);
#ifdef	DYNA
	for (;;) {
		printf("\n");
		segread(&segregs);
		inregs.h.ah = 0x83;
		inregs.h.al = 0x00;
		int86x(0x10, &inregs, &outregs, &segregs);
		if (outregs.x.ax == 0)
			break;
	}
	printf("\x1b[2J");
#endif
	if (1 == config_read())
		return (1);
	if (argc > sta) {
		printf("reading COMMAND PARAMETER ");
		for (i = sta; i < argc; i++) {
			if (argv[i][0] == '-' || argv[i][0] == '/') {
				if (1 == data_set(&argv[i][1])) {
					printf("/ command line parameter error!! '%s'\n", argv[i]);
					return (1);
				}
			}
		}
		printf("\n");
	}
	if (SG_SPEED_MIN > SG_SPEED)
		SG_SPEED_MIN = SG_SPEED;
#ifdef	BGM
	bgm_title_put();
	if (1 == bgm_read()) {
		bgm_finish();
		return (1);
	}
#endif
#ifdef	BGMFM
	bgm_read();
	if (fs_open(0))
		return (1);
#endif
	if (1 == blk_buf_read()) {
#ifdef	BGM
		bgm_finish();
#endif
#ifdef	BGMFM
		fs_close();
#endif
		return (1);
	}
	if (1 == scr_read(score_file, 1)) {
#ifdef	BGM
		bgm_finish();
#endif
#ifdef	BGMFM
		fs_close();
#endif
		return (1);
	}
#ifndef	ANSI
	if (1 == cg_buf_read()) {
#ifdef	BGM
		bgm_finish();
#endif
#ifdef	BGMFM
		fs_close();
#endif
		return (1);
	}
	if (1 == fnt_buf_read()) {
#ifdef	BGM
		bgm_finish();
#endif
#ifdef	BGMFM
		fs_close();
#endif
		return (1);
	}
#endif
#ifdef	ANSI
	gstart;
#endif
#ifdef	FMR
	if (SG_TITLE == 1) {
		if (NULL == (fp = fpopen("porolith.tit", "rb"))) {
			puts("can't open 'porolith.tit'\n");
			return (1);
		} else
			fclose(fp);
		gstart;
		while (kbhit())
			getch();
		if (1 == title_put()) {
#ifdef	BGMFM
			fs_close();
#endif
			return (1);
		}
		while (kbhit())
			getch();
		getch();
	} else {
		gstart;
	}
#endif
#ifdef	DYNA
	if (SG_TITLE == 1) {
		if (NULL == (fp = fpopen("porolith.tit", "rb"))) {
			puts("can't open 'porolith.tit'\n");
			return (1);
		} else
			fclose(fp);
		gstart;
		while (kbhit())
			getch();
		if (1 == title_put()) {
			return (1);
		}
		while (kbhit())
			getch();
		getch();
	} else {
		gstart;
	}
#endif
#ifdef	PC98
	if (SG_TITLE == 1) {
		if (NULL == (fp = fpopen("porolith.r1", "rb"))) {
			puts("can't open 'porolith.tit'\n");
#ifdef	BGM
			bgm_finish();
#endif
			return (1);
		} else
			fclose(fp);
		if (NULL == (fp = fpopen("porolith.g1", "rb"))) {
			puts("can't open 'porolith.tit'\n");
#ifdef	BGM
			bgm_finish();
#endif
			return (1);
		} else
			fclose(fp);
		if (NULL == (fp = fpopen("porolith.b1", "rb"))) {
			puts("can't open 'porolith.tit'\n");
#ifdef	BGM
			bgm_finish();
#endif
			return (1);
		} else
			fclose(fp);
		gstart;
		while (kbhit2())
			getch2();
		if (1 == title_put2()) {
#ifdef	BGM
			bgm_finish();
#endif
			gstop;
			return (1);
		}
		while (kbhit2())
			getch2();
		getch2();
		cls;
	} else {
		gstart;
	}
#endif
	high_score_put(-1);
	mask_set(0xff, 0x00, 0x00, MASK_NORMAL);
	out_text(13, 22, "<HIT ANY KEY>");
	getch();
	do {
		poro_init();
		initial_screen();
#ifndef	PC98
		while (1 == porolith())
			while (kbhit())
				getch();
#else
		while (1 == porolith())
			while (kbhit2())
				getch2();
#endif
	} while (1 == game_over());
	gstop;
	free(chr_pt);
	free(blk_pt);
	free(fnt_pt);
	free(fnthan_pt);
	printf("ぽろりす Version %s\n", VERSION);
	color(TEX_YELLOW);
	puts("また遊んでね!");
	color(TEX_WHITE);
#ifdef	BGM
	bgm_finish();
#endif
#ifdef	BGMFM
	fs_close();
#endif
	return (0);
}
int
porolith(void)
{
	int             block_y, block_x;
	int             a;
	int             cc;
	block_y = 0;
	block_select();
	block_x = (MAP_X / 2) - 2;
	block_put(block_x, block_y);
	if (1 == block_check(block_x, block_y)) {
		return (0);
	}
	for (;;) {
		for (a = 0; a < speed; a++) {
#ifndef	PC98
			if (255 == kbhit()) {
				cc = getch();
				block_hide(block_x, block_y);
				switch (cc) {
#ifdef	FMR
				case 0x0d:
					panic_put();
					break;
#endif
#ifdef	ANSI
				case 0x0d:
					panic_put();
					ansi_rescreen();
					break;
#endif
#ifdef	DYNA
				case 0x0d:
					panic_put();
					ansi_rescreen();
					break;
#endif
				case 'Q':
				case 'q':
					gstop;
#ifdef	BGMFM
					fs_close();
#endif
					exit(1);
					break;
				case 0x1b:
					reason_put("<<PAUSE!>>");
					getch();
					break;
				case 'k':
				case 'K':
				case '5':
					block_ring_left();
					if (1 == block_check(block_x, block_y)) {
						block_ring_right();
#ifdef	BGMFM
						sound_put(BGM_UNMOVE);
					} else
						sound_put(BGM_RING);
#else
					}
#endif
					break;
				case 'j':
				case 'J':
				case '4':
					block_x--;
					if (1 == block_check(block_x, block_y)) {
						block_x++;
#ifdef	BGMFM
						sound_put(BGM_UNMOVE);
					} else {
						sound_put(BGM_MOVE);
#endif
					}
					break;
				case 'l':
				case 'L':
				case '6':
					block_x++;
					if (1 == block_check(block_x, block_y)) {
						block_x--;
#ifdef	BGMFM
						sound_put(BGM_UNMOVE);
					} else {
						sound_put(BGM_MOVE);
#endif
					}
					break;
				case '0':
				case 'n':
				case 'N':
					block_y++;
					if (1 == block_check(block_x, block_y))
						block_y--;
#ifdef	BGMFM
					else
						sound_put(BGM_MOVE);
#endif
					break;
				case ' ':
				case '2':
#ifdef	BGMFM
					sound_put(BGM_DROP);
#endif
#ifdef	ANSI
					set_RAW_mode();
#endif
					for (;;) {
						if (SG_BLOCKDEMO == 1)
							block_hide(block_x, block_y);
						if (1 == block_check(block_x, block_y + 1)) {
							block_keep(block_x, block_y);
							return (1);
						}
						block_y++;
						if (SG_BLOCKDEMO == 1)
							block_put(block_x, block_y);
					}
#ifdef	ANSI
					set_COOKED_mode();
#endif
					break;
				}
			}
#else
			if (kbhit2()) {
				cc = getch2();
				block_hide(block_x, block_y);
				switch (cc) {
				case scan_RET:
					panic_put();
					break;
				case scan_Q:
					gstop;
#ifdef	BGM
					bgm_finish();
#endif
					exit(1);
					break;
				case scan_ESC:
					reason_put("<<PAUSE!>>");
					getch();
					break;
				case scan_5:
				case scan_K:
					block_ring_left();
					if (1 == block_check(block_x, block_y)) {
						block_ring_right();
#ifdef	BGM
						sound_put(BGM_UNMOVE);
					} else
						sound_put(BGM_RING);
#else
					}
#endif
					break;
				case scan_4:
				case scan_J:
					block_x--;
					if (1 == block_check(block_x, block_y)) {
						block_x++;
#ifdef	BGM
						sound_put(BGM_UNMOVE);
					} else {
						sound_put(BGM_MOVE);
#endif
					}
					break;
				case scan_6:
				case scan_L:
					block_x++;
					if (1 == block_check(block_x, block_y)) {
						block_x--;
#ifdef	BGM
						sound_put(BGM_UNMOVE);
					} else {
						sound_put(BGM_MOVE);
#endif
					}
					break;
				case scan_0:
				case scan_N:
					block_y++;
					if (1 == block_check(block_x, block_y))
						block_y--;
#ifdef	BGM
					else
						sound_put(BGM_MOVE);
#endif
					break;
				case scan_SPC:
				case scan_2:
#ifdef	BGM
					sound_put(BGM_DROP);
#endif
					for (;;) {
						if (SG_BLOCKDEMO == 1)
							block_hide(block_x, block_y);
						if (1 == block_check(block_x, block_y + 1)) {
							block_keep(block_x, block_y);
							return (1);
						}
						block_y++;
						if (SG_BLOCKDEMO == 1)
							block_put(block_x, block_y);
					}
					break;
				}
			}
#endif
			block_put(block_x, block_y);
		}
		block_hide(block_x, block_y);
		if (1 == block_check(block_x, block_y + 1)) {
			block_keep(block_x, block_y);
			return (1);
		}
		block_y++;
		block_put(block_x, block_y);
	}
}
void
name_input(char *nam)
{
	int             x;
	char            c;
	strcpy(nam, "            ");
	x = 0;
	mask_set(0x00, 0x55, 0xff, MASK_NORMAL);
	out_text(5, 20, "YOUR SCORE IS");
	score_put(19, 20, score);
	mask_set(0xaa, 0x55, 0xff, MASK_NORMAL);
	out_text(5, 21, "INPUT YOUR NAME:");
	mask_set(0xaa, 0x55, 0x00, MASK_NORMAL);
	for (;;) {
		mask_set(0xaa, 0x55, 0xff, MASK_NORMAL);
		out_text(21 + x, 21, "*");
		c = (char) getch();
		out_text(21 + x, 21, " ");
		if (c >= 'a' && c <= 'z')
			c -= 'a' - 'A';
		if (c >= ' ' && c <= 'Z' && x < 12) {
#ifdef	BGM
			sound_put(BGM_NAME_INPUT);
#endif
#ifdef	BGMFM
			sound_put(BGM_NAME_INPUT);
#endif
			nam[x] = c;
			x++;
		}
		if (c == 0x08 && x > 0) {
			x--;
			nam[x] = ' ';
		}
		if (c == 0x0d)
			return;
		mask_set(0xff, 0xff, 0xff, MASK_NORMAL);
		out_text(21, 21, nam);
	}
}
int
rank_sort(void)
{
	int             i, j;
	int             rnk;
	char            nam[20];
	char            dat[20];
	char            buf[80];
	time_t          lotim;
	struct tm      *timpt;
	name_input(nam);
	for (i = 0; i < 10; i++) {
		if (score > high_score[i].score) {
			for (j = 9; j > i; j--) {
				high_score[j] = high_score[j - 1];
			}
			high_score[i].score = score;
			strcpy(high_score[i].name, nam);
			lotim = time(NULL);
			timpt = localtime(&lotim);
			/* fix for Y2K */
			if (timpt->tm_year >= 100) {
				timpt->tm_year -= 100;
			}
			sprintf(dat, "%02d/%02d/%02d", timpt->tm_year, timpt->tm_mon + 1, timpt->tm_mday);
			strcpy(high_score[i].date, dat);
			sprintf(dat, "%02d:%02d:%02d", timpt->tm_hour, timpt->tm_min, timpt->tm_sec);
			strcpy(high_score[i].time, dat);
			rnk = i;
			score = 0;
		}
	}
	fp = fopen(open_file, "w+");
	for (i = 0; i < 10; i++) {
		sprintf(buf, "%05d0 %12s %8s %8s\n", high_score[i].score, high_score[i].name, high_score[i].date, high_score[i].time);
		fputs(buf, fp);
	}
	fclose(fp);
	return (rnk);
}
int
game_over(void)
{
	int             c, rnk;
#ifdef	BGM
	sound_put(BGM_GAME_OVER);
	bgm_wait_sound();
#endif
#ifdef	BGMFM
	sound_put(BGM_GAME_OVER);
#endif
	reason_put("");
	reason_put("   KEY! ");
	reason_put(" HIT ANY");
	reason_put("");
	reason_put("GAME OVER!");
	getch();
	if (SG_NETWORK == 1)
		scr_read(score_file, 0);
	high_score_put(-1);
	if (score > high_score[9].score) {
		rnk = rank_sort();
		high_score_put(rnk);
	} else {
		mask_set(0x00, 0x55, 0xff, MASK_NORMAL);
		out_text(9, 20, "YOUR SCORE IS");
		score_put(23, 20, score);
	}
	mask_set(0xff, 0xff, 0xff, MASK_NORMAL);
	out_text(14, 22, "RETRY? (Y/N)");
	for (;;) {
		c = getch();
		if (c == 'y' || c == 'Y')
			return (1);
		if (c == 'n' || c == 'N')
			return (0);
		if (c == 'w' || c == 'W') {
			file_out();
			return (0);
		}
	}
}
void
poro_init(void)
{
	register        x, y, i;
	srand((unsigned int) time(0L));
	map_offset = MAP_XX;
	for (i = 0; i < 5; i++)
		score_reason[i][0] = 0x00;
	score = 0;
	speed = SG_SPEED;
	special_pt = SG_SPECIAL_POINT;
	level_pt = SG_LEVEL_PT;
	poro_level = 1;
	block_hight = MAP_Y;
	for (;;) {
		next_block = rand() % SG_BLOCK_NUM;
		if ((block_level[next_block] == poro_level) || (block_level[next_block] < poro_level && rand() % 3 == 0))
			break;
	}
	for (y = 0; y < MAP_Y; y++) {
		for (x = 0; x < MAP_X; x++) {
			screen_map[y][x] = 0x00;
		}
	}
	for (i = 0; i < MAP_Y; i++) {
		screen_map[i][0] = 0xff;
		screen_map[i][MAP_X - 1] = 0xff;
	}
	for (i = 1; i < MAP_X; i++) {
		screen_map[MAP_Y - 1][i] = 0xff;
	}
}
void
usage_put(void)
{
	printf("*** porolith version %s ***\n\n", VERSION);
	puts("usage: porolith [/CFG=configuration file name] [/DATA_NAME=value] ...");
	puts(" ");
	puts("[hit any key]");
	puts(" ");
	getch();
	puts("■ファイル関係の設定");
	puts("ブロックデータが登録されているﾌｧｲﾙ名  BLOCKNAME   (BF) 実際にあるﾌｧｲﾙ");
	puts("登録されているブロックの個数          BLOCKNUM    (BN) BLOCKNAMEの登録個数");
	puts("キャラクタデータが登録されてるﾌｧｲﾙ名  CHRNAME     (CF) 実際にあるﾌｧｲﾙ");
	puts("パニックデータが登録されているﾌｧｲﾙ名　PANICNAME   (PF) 実際にあるﾌｧｲﾙ");
	puts("フォントデータが登録されているﾌｧｲﾙ名　FONTNAME    (FF) 実際にあるﾌｧｲﾙ");
	puts("効果音データが登録されているﾌｧｲﾙ名    EFSNAME     (EF) 実際にあるﾌｧｲﾙ");
	puts(" ");
	puts("[hit any key]");
	puts(" ");
	getch();
	puts("■ゲームの「色」に関する設定");
	puts("ブロック高速落下                      BLOCKDEMO   (BD) 0=普通,1=早い");
	puts("タイトルデモの省略                    TITLE       (TT) 0=出ない,1=出る");
	puts("効果音，音楽のフラグ　　　　          BGM         (BG) 0=off,1=on");
	puts("ＡＮＳＩ版用キャラクタ定義            ANSICHAR    (AN) 全角のみ");
	puts(" ");
	puts("[hit any key]");
	puts(" ");
	getch();
	puts("■ゲームの難易度の設定");
	puts("ブロック落下の初期スピード値          SPEED       (SP) 値が小さいほど早い");
	puts("スピード値の最小値                    SPEEDMIN    (SM) SPEEDMIN<SPEED");
	puts("ブロッククリアとなる面積のノルマ      CLEARBLOCK  (CB) 値が小さいほど簡単");
	puts("レベルアップする得点単位              LEVELPT     (LP) 適度な点数");
	puts("ブロッククリア時の他の浮いたブロック  DROP        (DR) 0=一部,1=全部");
	puts("次に落ちてくるブロックの表示指定      NEXT        (NX) 0=みえない,1=見える");
	puts("スペシャルキャラのサポート            SPECIAL     (SS) 0=しない,1=する");
	puts("スペシャルキャラの登場得点単位        SPECIALPT   (ST) 少ないと簡単");
	puts("スペシャルキャラの出る高さ            SPECIALHIGHT(SH) ｽﾍﾟｷｬﾗをだす対象の高さ");
	puts("スペシャルキャラの出る確率            SPECIALRATE (SR) 1/Rateの確率で出る");
	puts(" ");
	puts("[hit any key]");
	puts(" ");
	getch();
	puts("■その他の特別な設定");
	puts("ネットーワーク接続時のフラグ(?)       NETWORK     (NT) 0=未,1=有");
}
#ifdef	BGM
void
sound_put(int no)
{
	if (SG_BGM == 1)
		bgm_sound(no);
}
#endif
#ifdef	BGMFM
void
sound_put(int no)
{
	if (SG_BGM == 1)
		fs_bgm(bgm[no]);
}
#endif
void
porolith_abort(void)
{
#ifdef	BGM
	bgm_finish();
#endif
#ifdef	BGMFM
	fs_close();
#endif
	gstop;
	free(chr_pt);
	free(blk_pt);
	free(fnt_pt);
	free(fnthan_pt);
	printf("ぽろりす Version %s\n", VERSION);
	color(TEX_YELLOW);
	puts("強制終了しました。");
	color(TEX_WHITE);
	exit(-1);
}
