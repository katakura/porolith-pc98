/*
		Action Puzzle Game "POROLITH"

			PC-9801,286
			FMR50/60/70
			Dynabook
			and MSDOS Machine

	Copyright(C)1990 by Ko.Mo.Mo. Allrights Reserved.
*/
#include	"porolith.h"
void
screen_paint(int x1, int y1, int x2, int y2, int num)
{
	int             i;
	for (i = y1; i <= y2; i++) {
		pattern_put(x1, i, num);
		pattern_put(x2, i, num);
	}
	for (i = x1; i <= x2; i++) {
		pattern_put(i, y1, num);
		pattern_put(i, y2, num);
	}
}
void
pattern_put(int x, int y, int num)
{
#ifndef	ANSI
#ifndef	FMR
	unsigned long   offset;
#endif
	register        i;
	struct cg_t far *cg;
#endif
#ifdef	DYNA
	register        j;
#endif
#ifdef	ANSI
	locate((x + map_offset) * 2, y);
	putchar(SG_ANSI_CHAR[num * 2]);
	putchar(SG_ANSI_CHAR[num * 2 + 1]);
#endif
#ifdef	FMR
	cg = (struct cg_t *) & chr_pt[num];
	x += map_offset;;
	image_buf[0] = 0x10;
	image_buf[1] = 0x00;
	image_buf[2] = 0x10;
	image_buf[3] = 0x00;
	for (i = 0; i < 16; i++) {
		image_buf[i * 8 + 4] = cg->b[i * 2];
		image_buf[i * 8 + 5] = cg->b[i * 2 + 1];
		image_buf[i * 8 + 6] = cg->r[i * 2];
		image_buf[i * 8 + 7] = cg->r[i * 2 + 1];
		image_buf[i * 8 + 8] = cg->g[i * 2];
		image_buf[i * 8 + 9] = cg->g[i * 2 + 1];
		image_buf[i * 8 + 10] = 0x00;
		image_buf[i * 8 + 11] = 0x00;
	}
	_putimage(x * 16, y * 16, image_buf, _GPSET);
#endif
#ifdef	PC98
	cg = (struct cg_t *) & chr_pt[num];
	x += map_offset;;
	offset = 1280 * y + x * 2;
	for (i = 0; i < 16; i++) {
		*(cgpt_r + offset + i * 80) = cg->r[i * 2];
		*(cgpt_r + offset + i * 80 + 1) = cg->r[i * 2 + 1];
		*(cgpt_g + offset + i * 80) = cg->g[i * 2];
		*(cgpt_g + offset + i * 80 + 1) = cg->g[i * 2 + 1];
		*(cgpt_b + offset + i * 80) = cg->b[i * 2];
		*(cgpt_b + offset + i * 80 + 1) = cg->b[i * 2 + 1];
	}
#endif
#ifdef	DYNA
	cg = (struct cg_t *) & chr_pt[num];
	x += map_offset;;
	offset = 320 * y + x * 2;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			*(cgpt[j] + offset + 80 * i) = cg->r[i * 8 + j * 2];
			*(cgpt[j] + offset + 80 * i + 1) = cg->r[i * 8 + j * 2 + 1];
		}
	}
#endif
}
int
title_put(void)
{
#ifdef	DYNA
	int             i, j, k;
	unsigned char   style;
#endif
#ifdef	FMR
	int             i;
	unsigned char   style;
#endif
	map_offset = 0;
#ifndef	ANSI
	fp = fpopen("porolith.tit", "rb");
	if (!fp) {
		puts("porolith.tit can't open!");
		return (1);
	}
	printf("%s", ESC_CLS);
	mask_set(0xff, 0xff, 0xaa, MASK_REVERCE);
	out_text(0, 0, "  ACTION PUZZLE GAME POROLITH VER       ");
	out_text(34, 0, VERSION);
	out_text(0, 24, "   COPYRIGHT @ 1990.10    BY KO.MO.MO   ");
	mask_set(0xaa, 0xaa, 0xaa, MASK_NORMAL);
	out_text(1, 10, "    GAME DESIGN : KO.MO.MO (PCS32378)");
	out_text(1, 11, "MAIN PROGRAMMER : KO.MO.MO (PCS32378)");
	out_text(1, 12, " SUB PROGRAMMER : TAKA     (PCS31440)");
	out_text(1, 13, "                  RYOSHI");
	out_text(1, 14, "   DEFAULT FONT : TACOX    (PCS35738)");
	out_text(1, 15, " FONT EDIT TOOL : NOENOE   (PCS26448)");
#ifdef	BGMFM
	out_text(1, 16, "    BGM LIBRARY : PACI     (PCS35438)");
#endif
	mask_set(0xaa, 0xaa, 0xaa, MASK_NORMAL);
	out_text(0, 18, "SPECIAL THANKS TO JUNK.TEST ALL MEMBERS.");
	out_text(0, 19, "              AND MICHIKO KIHARA.");
	mask_set(0xff, 0xaa, 0xaa, MASK_NORMAL);
	out_text(18, 19, "M");
	out_text(26, 19, "K");
	mask_set(0xff, 0x00, 0x00, MASK_NORMAL);
	out_text(8, 22, "<PUSH ANY KEY TO START>");
#ifdef	FMR
	for (i = 0; i < 5600; i++) {
		fread(&style, 1, 1, fp);
		image_buf[0] = 0x08;
		image_buf[1] = 0x00;
		image_buf[2] = 0x01;
		image_buf[3] = 0x00;
		image_buf[4] = 0x00;
		image_buf[5] = style & 0xaa;
		image_buf[6] = style;
		image_buf[7] = 0x00;
		_putimage(i % 80 * 8, i / 80 + 50, image_buf, _GPSET);
	}
#endif
#ifdef	DYNA
	for (i = 0; i < 20; i++) {
		for (j = 0; j < 4; j++) {
			for (k = 0; k < 80; k++) {
				fread(&style, 1, 1, fp);
				*(960 + cgpt[j] + k + i * 80) = (char) (style & 0xaa);
			}
		}
	}
#endif
	fclose(fp);
#endif
	return (0);
}
void
initial_screen(void)
{
	int             x, y, i;

#ifdef	ANSI
	set_RAW_mode();
	printf("%s", ESC_CLS);
#else
	cls;
#endif
	map_offset = 0;
	screen_paint(0, 0, 11, 3, CHR_BGC);
#ifndef	DYNA
	mask_set(0xff, 0xaa, 0xaa, MASK_NORMAL);
#else
	mask_set(0xff, 0xff, 0xff, MASK_NORMAL);
#endif
	out_text(1, 1, " POROLITH ");
	mask_set(0xff, 0xff, 0x00, MASK_NORMAL);
	out_text(1, 2, " VER      ");
	out_text(6, 2, VERSION);
	screen_paint(0, 5, 11, 8, CHR_BGC);
	mask_set(0x00, 0xaa, 0xff, MASK_NORMAL);
	out_text(1, 6, "HIGH-SCORE");
	mask_set(0xFF, 0xaa, 0xff, MASK_NORMAL);
	score_put(3, 7, high_score[0].score);
	screen_paint(0, 10, 11, 13, CHR_BGC);
	mask_set(0x00, 0xaa, 0xff, MASK_NORMAL);
	out_text(1, 11, "YOUR-SCORE");
	mask_set(0xFF, 0xaa, 0xff, MASK_NORMAL);
	score_put(3, 12, score);
	screen_paint(0, 15, 11, 24, CHR_BGC);
#ifdef	DYNA
	mask_set(0xaa, 0xaa, 0xaa, MASK_NORMAL);
#else
	mask_set(0xff, 0xff, 0x00, MASK_NORMAL);
#endif
	out_text(2, 16, "HISTORY");
	screen_paint(29, 0, 39, 9, CHR_BGC);
#ifndef	DYNA
	mask_set(0xaa, 0x00, 0xff, MASK_NORMAL);
#else
	mask_set(0xff, 0xff, 0xff, MASK_NORMAL);
#endif
	out_text(31, 1, "NEXT BL");
#ifdef	DYNA
	mask_set(0xff, 0xff, 0xff, MASK_NORMAL);
#else
	mask_set(0xaa, 0xaa, 0x55, MASK_NORMAL);
#endif
	out_text(29, 12, "KEY-FUNC.");
	mask_set(0xff, 0xaa, 0x55, MASK_NORMAL);
#ifdef	DYNA
	out_text(29, 14, "J----LEFT");
	out_text(29, 15, "L----RIGHT");
	out_text(29, 16, "K----RING");
	out_text(29, 17, "N----DOWN");
	out_text(29, 18, "SP---DROP");
#else
	out_text(29, 14, "4----LEFT");
	out_text(29, 15, "6----RIGHT");
	out_text(29, 16, "5----RING");
	out_text(29, 17, "0----DOWN");
	out_text(29, 18, "2----DROP");
#endif
	out_text(29, 19, "ESC--PAUSE");
	out_text(29, 20, "CR---PANIC!");
	mask_set(0xff, 0xff, 0xff, MASK_NORMAL);
	out_text(29, 23, "WRITTEN BY");
	out_text(29, 24, " KO.MO.MO");
	map_offset = MAP_XX;
	for (y = 0; y < MAP_Y - 1; y++) {
		for (x = 1; x < MAP_X - 1; x++) {
			pattern_put(x, y, screen_map[y][x]);
		}
	}
	for (i = 0; i < MAP_Y; i++) {
		pattern_put(0, i, CHR_WAKU);
		pattern_put(MAP_X - 1, i, CHR_WAKU);
	}
	for (i = 1; i < MAP_X; i++) {
		pattern_put(i, MAP_Y - 1, CHR_WAKU);
	}
#ifdef	ANSI
	set_COOKED_mode();
#endif
}
void
score_plus(int scr)
{
	score += scr;
	mask_set(0xFF, 0xaa, 0xff, MASK_NORMAL);
	score_put(3, 12, score);
}
void
reason_put(char *msg)
{
	int             i;
	strcpy(score_reason[4], score_reason[3]);
	strcpy(score_reason[3], score_reason[2]);
	strcpy(score_reason[2], score_reason[1]);
	strcpy(score_reason[1], score_reason[0]);
	strcpy(score_reason[0], msg);
	strcat(score_reason[0], "          ");
	score_reason[0][10] = 0x00;
#ifdef	BGM
	sound_put(BGM_NAME_INPUT);
#endif
#ifdef	BGMFM
	sound_put(BGM_NAME_INPUT);
#endif
	for (i = 0; i < 5; i++) {
		if (i == 0)
			mask_set(0xff, 0xff, 0xff, MASK_NORMAL);
		else
			mask_set(0xaa, 0xaa, 0xaa, MASK_NORMAL);
		out_text(1, 18 + i, score_reason[i]);
	}
}
void
out_text(int x, int y, char *str)
{
	register        j;
#ifndef	ANSI
	register        i;
#ifndef	FMR
	unsigned long   offset;
#endif
	struct fnt_t far *fnt;
#endif
#ifdef	DYNA
	register        k;
#endif
	for (j = 0; j < strlen(str); j++) {
		if (str[j] >= 'a' && str[j] <= 'z')
			str[j] -= 'a' - 'A';
#ifdef	ANSI
		locate(x * 2, y);
		printf("%c%c", ansi_font[(str[j] - ' ') * 2], ansi_font[(str[j] - ' ') * 2 + 1]);
#endif
#ifdef	FMR
		fnt = (struct fnt_t *) & fnt_pt[str[j]];
		image_buf[0] = 0x10;
		image_buf[1] = 0x00;
		image_buf[2] = 0x10;
		image_buf[3] = 0x00;
		for (i = 0; i < 16; i++) {
			if (r_mask != 0xff && r_mask != 0x00)
				r_mask = (char) (0xff - r_mask);
			if (g_mask != 0xff && g_mask != 0x00)
				g_mask = (char) (0xff - g_mask);
			if (b_mask != 0xff && b_mask != 0x00)
				b_mask = (char) (0xff - b_mask);
			if (r_flag == 1) {
				image_buf[i * 8 + 4] = (char) (0xff - fnt->fnt[i * 2] & b_mask);
				image_buf[i * 8 + 5] = (char) (0xff - fnt->fnt[i * 2 + 1] & b_mask);
				image_buf[i * 8 + 6] = (char) (0xff - fnt->fnt[i * 2] & g_mask);
				image_buf[i * 8 + 7] = (char) (0xff - fnt->fnt[i * 2 + 1] & g_mask);
				image_buf[i * 8 + 8] = (char) (0xff - fnt->fnt[i * 2] & r_mask);
				image_buf[i * 8 + 9] = (char) (0xff - fnt->fnt[i * 2 + 1] & r_mask);
			} else {
				image_buf[i * 8 + 4] = (char) (fnt->fnt[i * 2] & b_mask);
				image_buf[i * 8 + 5] = (char) (fnt->fnt[i * 2 + 1] & b_mask);
				image_buf[i * 8 + 6] = (char) (fnt->fnt[i * 2] & g_mask);
				image_buf[i * 8 + 7] = (char) (fnt->fnt[i * 2 + 1] & g_mask);
				image_buf[i * 8 + 8] = (char) (fnt->fnt[i * 2] & r_mask);
				image_buf[i * 8 + 9] = (char) (fnt->fnt[i * 2 + 1] & r_mask);
			}
			image_buf[i * 8 + 10] = 0x00;
			image_buf[i * 8 + 11] = 0x00;
		}
		_putimage(x * 16, y * 16, image_buf, _GPSET);
#endif
#ifdef	PC98
		fnt = (struct fnt_t *) & fnt_pt[str[j]];
		offset = 1280 * y + x * 2;
		for (i = 0; i < 16; i++) {
			if (r_mask != 0xff && r_mask != 0x00)
				r_mask = (char) (0xff - r_mask);
			if (g_mask != 0xff && g_mask != 0x00)
				g_mask = (char) (0xff - g_mask);
			if (b_mask != 0xff && b_mask != 0x00)
				b_mask = (char) (0xff - b_mask);
			if (r_flag == 1) {
				*(cgpt_r + offset + i * 80) = (char) (255 - (fnt->fnt[i * 2]) & g_mask);
				*(cgpt_r + offset + i * 80 + 1) = (char) (255 - (fnt->fnt[i * 2 + 1]) & g_mask);
				*(cgpt_g + offset + i * 80) = (char) (255 - (fnt->fnt[i * 2]) & r_mask);
				*(cgpt_g + offset + i * 80 + 1) = (char) (255 - (fnt->fnt[i * 2 + 1]) & r_mask);
				*(cgpt_b + offset + i * 80) = (char) (255 - (fnt->fnt[i * 2]) & b_mask);
				*(cgpt_b + offset + i * 80 + 1) = (char) (255 - (fnt->fnt[i * 2 + 1]) & b_mask);
			} else {
				*(cgpt_r + offset + i * 80) = (char) ((fnt->fnt[i * 2]) & g_mask);
				*(cgpt_r + offset + i * 80 + 1) = (char) ((fnt->fnt[i * 2 + 1]) & g_mask);
				*(cgpt_g + offset + i * 80) = (char) ((fnt->fnt[i * 2]) & r_mask);
				*(cgpt_g + offset + i * 80 + 1) = (char) ((fnt->fnt[i * 2 + 1]) & r_mask);
				*(cgpt_b + offset + i * 80) = (char) ((fnt->fnt[i * 2]) & b_mask);
				*(cgpt_b + offset + i * 80 + 1) = (char) ((fnt->fnt[i * 2 + 1]) & b_mask);
			}
		}
#endif
#ifdef	DYNA
		fnt = (struct fnt_t *) & fnt_pt[str[j]];
		offset = 320 * y + x * 2;
		for (i = 0; i < 4; i++) {
			for (k = 0; k < 4; k++) {
				if (g_mask != 0xff && g_mask != 0x00)
					g_mask = (char) (0xff - g_mask);
				if (r_flag == 1) {
					*(cgpt[k] + offset + i * 80) = (char) (255 - (fnt->fnt[i * 8 + k * 2]) & g_mask);
					*(cgpt[k] + offset + i * 80 + 1) = (char) (255 - (fnt->fnt[i * 8 + k * 2 + 1]) & g_mask);
				} else {
					*(cgpt[k] + offset + i * 80) = (char) ((fnt->fnt[i * 8 + k * 2]) & g_mask);
					*(cgpt[k] + offset + i * 80 + 1) = (char) ((fnt->fnt[i * 8 + k * 2 + 1]) & g_mask);
				}
			}
		}
#endif
		x++;
	}
}
void
mask_set(char rr, char gg, char bb, int rev)
{
#ifndef	DYNA
	r_mask = rr;
	g_mask = gg;
	b_mask = bb;
	r_flag = rev;
#else
	r_mask = rr;
	if (rr == 0xff && bb == 0xff)
		g_mask = 0xff;
	else
		g_mask = (char) ((gg == 0x00) ? 0xaa : gg);
	b_mask = bb;
	r_flag = rev;
#endif
}
void
score_put(int x, int y, int sc)
{
	char            buf[10];
	char            sco[10];
	int             i;
	itoa(sc, buf, 10);
	strcpy(sco, "000000");
	for (i = 0; i < strlen(buf); i++) {
		sco[i + 5 - strlen(buf)] = buf[i];
	}
	out_text(x, y, sco);
}
#ifdef	FMR
void
gscreen(int flag)
{
	inregs.h.ah = 0x01;
	inregs.h.al = (unsigned char) ((flag == 0) ? 0x08 : 0x0a);
	segread(&segregs);
	int86x(0x91, &inregs, &outregs, &segregs);
}
#endif
#ifdef	PC98
void
gscreen(int flag)
{
	inregs.h.ah = (unsigned char) (0x40 + flag);
	segread(&segregs);
	int86x(0x18, &inregs, &outregs, &segregs);
}
#endif
void
high_score_put(int rnk)
{
	int             i, j;
	char            drive[3];
	char            dir[80];
	char            fname[9];
	char            ext[5];
	char            buf[30], sco[8];
#ifdef	ANSI
	printf("%s", ESC_CLS);
#else
	cls;
#endif
	_splitpath(SG_CONFIG, drive, dir, fname, ext);
	strcpy(buf, "'");
	strcat(buf, fname);
	strcat(buf, ext);
	strcat(buf, "'");
	mask_set(0xff, 0xff, 0xaa, MASK_REVERCE);
#ifdef	ANSI
	set_RAW_mode();
	out_text(9, 0, "POROLITH HIGH SCORE RANK");
#else
	out_text(0, 0, "         POROLITH HIGH SCORE RANK       ");
	out_text(0, 24, "                                        ");
#endif
	mask_set(0xff, 0xaa, 0xaa, MASK_NORMAL);
	out_text(13, 2, buf);
	mask_set(0x00, 0xff, 0xff, MASK_NORMAL);
	out_text(17, 4, "TOP 10");
	mask_set(0xff, 0xff, 0xaa, MASK_NORMAL);
	out_text(0, 6, "RANK SCORE    LV NAME         DATE TIME");
	out_text(0, 7, "----------------------------------------");
	out_text(0, 18, "----------------------------------------");
	for (i = 0; i < 10; i++) {
		if (i == rnk) {
			mask_set(0xff, 0x55, 0x55, MASK_NORMAL);
		} else {
			mask_set(0xaa, 0xff, 0x55, MASK_NORMAL);
		}
		out_text(0, 8 + i, rank[i]);
		out_text_han(23, 8 + i, "PTS");
		itoa(high_score[i].score, buf, 10);
		strcpy(sco, "000000");
		for (j = 0; j < strlen(buf); j++) {
			sco[j + 5 - strlen(buf)] = buf[j];
		}
		out_text(5, 8 + i, sco);
		sprintf(sco, "%02d", high_score[i].score / SG_LEVEL_PT + 1);
		out_text(14, 8 + i, sco);
		out_text(17, 8 + i, high_score[i].name);
		out_text_han(60, 8 + i, high_score[i].date);
		out_text_han(70, 8 + i, high_score[i].time);
	}
#ifdef	ANSI
	set_COOKED_mode();
#endif
}
void
ansi_rescreen(void)
{
#ifdef	DYNA
	int             k, x, y;
#endif
	int             i, j;
	struct blk_t far *blk;
	blk = (struct blk_t *) & blk_pt[next_block];
	printf("%s", ESC_CLS);
	initial_screen();
	map_offset = 0;
	if (next_block != -1) {
		for (j = 0; j < 5; j++) {
			for (i = 0; i < 5; i++) {
				if (blk->blk[j][i] != 0x00)
					pattern_put(i + 32, j + 3, blk->blk[j][i]);
			}
		}
	} else {
		pattern_put(34, 5, 0x08);
	}
	map_offset = MAP_XX;
#ifdef	DYNA
	for (y = 0; y < MAP_Y; y++) {
		for (x = 1; x < MAP_X; x++) {
			if (y >= 0 && y < 24 && x > 0 && x < MAP_X - 1) {
				for (k = 0; k < 4; k++) {
					if (screen_map[y][x] != screen_map[y + dy[k]][x + dx[k]] &&
					    screen_map[y][x] != 0x00)
						block_line(x, y, k);
				}
			}
		}
	}
#endif
}
void
out_text_han(int x, int y, char *str)
{
	register        j;
#ifndef	ANSI
#ifndef	FMR
	unsigned long   offset;
#endif
	register        i;
	struct fnthan_t far *fnthan;
#endif
#ifdef	DYNA
	register        k;
#endif
	for (j = 0; j < strlen(str); j++) {
		if (str[j] >= 'a' && str[j] <= 'z')
			str[j] -= 'a' - 'A';
#ifdef	ANSI
		locate(x, y);
		printf("%c", str[j]);
#endif
#ifdef	FMR
		fnthan = (struct fnthan_t *) & fnthan_pt[str[j]];
		image_buf[0] = 0x08;
		image_buf[1] = 0x00;
		image_buf[2] = 0x10;
		image_buf[3] = 0x00;
		for (i = 0; i < 16; i++) {
			if (r_mask != 0xff && r_mask != 0x00)
				r_mask = (char) (0xff - r_mask);
			if (g_mask != 0xff && g_mask != 0x00)
				g_mask = (char) (0xff - g_mask);
			if (b_mask != 0xff && b_mask != 0x00)
				b_mask = (char) (0xff - b_mask);
			if (r_flag == 1) {
				image_buf[i * 4 + 4] = (char) (0xff - fnthan->fnt[i] & b_mask);
				image_buf[i * 4 + 5] = (char) (0xff - fnthan->fnt[i] & g_mask);
				image_buf[i * 4 + 6] = (char) (0xff - fnthan->fnt[i] & r_mask);
			} else {
				image_buf[i * 4 + 4] = (char) (fnthan->fnt[i] & b_mask);
				image_buf[i * 4 + 5] = (char) (fnthan->fnt[i] & g_mask);
				image_buf[i * 4 + 6] = (char) (fnthan->fnt[i] & r_mask);
			}
			image_buf[i * 4 + 7] = 0x00;
		}
		_putimage(x * 8, y * 16, image_buf, _GPSET);
#endif
#ifdef	PC98
		fnthan = (struct fnthan_t *) & fnthan_pt[str[j]];
		offset = 1280 * y + x;
		for (i = 0; i < 16; i++) {
			if (r_mask != 0xff && r_mask != 0x00)
				r_mask = (char) (0xff - r_mask);
			if (g_mask != 0xff && g_mask != 0x00)
				g_mask = (char) (0xff - g_mask);
			if (b_mask != 0xff && b_mask != 0x00)
				b_mask = (char) (0xff - b_mask);
			if (r_flag == 1) {
				*(cgpt_r + offset + i * 80) = (char) (0xff - fnthan->fnt[i] & g_mask);
				*(cgpt_g + offset + i * 80) = (char) (0xff - fnthan->fnt[i] & r_mask);
				*(cgpt_b + offset + i * 80) = (char) (0xff - fnthan->fnt[i] & b_mask);
			} else {
				if (r_flag == 2) {
					if (str[j] != ' ') {
						*(cgpt_r + offset + i * 80) &= (char) (0xff - fnthan->fnt[i]);
						*(cgpt_g + offset + i * 80) &= (char) (0xff - fnthan->fnt[i]);
						*(cgpt_b + offset + i * 80) &= (char) (0xff - fnthan->fnt[i]);
						*(cgpt_r + offset + i * 80) &= (char) (0xff - (fnthan->fnt[i] >> 1));
						*(cgpt_g + offset + i * 80) &= (char) (0xff - (fnthan->fnt[i] >> 1));
						*(cgpt_b + offset + i * 80) &= (char) (0xff - (fnthan->fnt[i] >> 1));
						*(cgpt_r + offset + i * 80) &= (char) (0xff - (fnthan->fnt[i] << 1));
						*(cgpt_g + offset + i * 80) &= (char) (0xff - (fnthan->fnt[i] << 1));
						*(cgpt_b + offset + i * 80) &= (char) (0xff - (fnthan->fnt[i] << 1));
						*(cgpt_r + offset + i * 80) |= (char) (fnthan->fnt[i] & g_mask);
						*(cgpt_g + offset + i * 80) |= (char) (fnthan->fnt[i] & r_mask);
						*(cgpt_b + offset + i * 80) |= (char) (fnthan->fnt[i] & b_mask);
					}
				} else {
					*(cgpt_r + offset + i * 80) = fnthan->fnt[i] & g_mask;
					*(cgpt_g + offset + i * 80) = fnthan->fnt[i] & r_mask;
					*(cgpt_b + offset + i * 80) = fnthan->fnt[i] & b_mask;
				}
			}
		}
#endif
#ifdef	DYNA
		fnthan = (struct fnthan_t *) & fnthan_pt[str[j]];
		offset = 320 * y + x;
		for (i = 0; i < 4; i++) {
			for (k = 0; k < 4; k++) {
				if (g_mask != 0xff && g_mask != 0x00)
					g_mask = (char) (0xff - g_mask);
				if (r_flag == 1) {
					*(cgpt[k] + offset + i * 80) = (char) (0xff - fnthan->fnt[i * 4 + k] & g_mask);
				} else {
					*(cgpt[k] + offset + i * 80) = (char) (fnthan->fnt[i * 4 + k] & g_mask);
				}
			}
		}
#endif
		x++;
	}
}
int
title_put2(void)
{
#ifdef	PC98
	color(TEX_YELLOW);
	locate(30, 13);
	puts("ちょっとまってネ☆");
	gscreen(1);
	fp = fpopen("porolith.g1", "rb");
	fread(cgpt_r, 80 * 400, 1, fp);
	fclose(fp);
	fp = fpopen("porolith.b1", "rb");
	fread(cgpt_b, 80 * 400, 1, fp);
	fclose(fp);
	fp = fpopen("porolith.r1", "rb");
	fread(cgpt_g, 80 * 400, 1, fp);
	fclose(fp);
	mask_set(0xaa, 0xff, 0x00, 2 /* MASK_OR */ );
	out_text_han(2, 14, "*** POROLITH  VERSION      ***");
	out_text_han(24, 14, VERSION);
	mask_set(0xaa, 0xff, 0xaa, 2 /* MASK_OR */ );
	out_text_han(2, 15, "      GAME DESIGN : KO.MO.MO ");
	out_text_han(2, 16, "  MAIN PROGRAMMER : KO.MO.MO ");
	out_text_han(2, 17, "   SUB PROGRAMMER : TAKA     ");
	out_text_han(2, 18, "                    RYOSHI   ");
	out_text_han(2, 19, "     DEFAULT FONT : TACOX    ");
	out_text_han(2, 20, "   FONT EDIT TOOL : NOENOE   ");
	out_text_han(2, 21, "     TITLE DESIGN : KEEN     ");
#ifdef	BGM
	out_text_han(2, 22, "      BGM LIBRARY : FEMY     ");
#endif
	mask_set(0xff, 0xff, 0xaa, 2);
	out_text_han(2, 23, "SPECIAL THANKS TO : MICHIKO");
	mask_set(0xff, 0xaa, 0xaa, 2);
	out_text_han(22, 23, "M");
	color(TEX_WHITE);
	printf("%s", ESC_CLS);
	gscreen(0);
#endif
	return (0);
}
/*
 * 画面表示モードを高速（ＲＡＷ）にする 
 */
int
set_RAW_mode(void)
{
	struct SREGS    segregs;
	union REGS      inregs, outregs;

	segread(&segregs);
	inregs.x.ax = 0x4400;	/* IOCTLデータの取得 */
	inregs.x.bx = 1;	/* STDOUT */
	intdosx(&inregs, &outregs, &segregs);
	if (outregs.x.cflag != 0)
		return (outregs.x.ax);

	inregs.x.ax = 0x4401;	/* IOCTLデータの設定 */
	inregs.x.bx = 1;	/* STDOUT */
	inregs.h.dh = 0;
	inregs.h.dl = outregs.h.dl | 0x20;
	intdosx(&inregs, &outregs, &segregs);
	if (outregs.x.cflag != 0)
		return (outregs.x.ax);
	return (0);
}
#ifdef	ANSI
/*
 * 画面表示モードを低速（ＣＯＯＫＥＤ）にする 
 */
int
set_COOKED_mode(void)
{
	struct SREGS    segregs;
	union REGS      inregs, outregs;

	segread(&segregs);
	inregs.x.ax = 0x4400;	/* IOCTLデータの取得 */
	inregs.x.bx = 1;	/* STDOUT */
	intdosx(&inregs, &outregs, &segregs);
	if (outregs.x.cflag != 0)
		return (outregs.x.ax);

	inregs.x.ax = 0x4401;	/* IOCTLデータの設定 */
	inregs.x.bx = 1;	/* STDOUT */
	inregs.h.dh = 0;
	inregs.h.dl = outregs.h.dl & 0xdf;
	intdosx(&inregs, &outregs, &segregs);
	if (outregs.x.cflag != 0)
		return (outregs.x.ax);
	return (0);
}
#endif
