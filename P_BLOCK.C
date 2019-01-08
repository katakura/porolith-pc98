/*
		Action Puzzle Game "POROLITH"

			PC-9801,286
			FMR50/60/70
			Dynabook
			and MSDOS Machine

	Copyright(C)1990 by Ko.Mo.Mo. Allrights Reserved.
*/

#include	"porolith.h"

/*	指定されたブロックを落下対象のブロックに設定する
*/
void
block_copy(int num)
{
	int             x, y;
	struct blk_t far *blk;

	blk = (struct blk_t *) & blk_pt[num];
	for (y = 0; y < 5; y++) {
		for (x = 0; x < 5; x++) {
			if (num != -1) {
				/* スペシャルキャラ(=-1)以外ならコピー	*/
				block.blk[y][x] = blk->blk[y][x];
			} else {
				/* スペシャルキャラなら真ん中のみに設定	*/
				if (x == 2 && y == 2) {
					block.blk[y][x] = 0x08;
				} else {
					block.blk[y][x] = 0x00;
				}
			}
		}
	}
}

/*	画面上に落下中のブロックを表示する
*/
void
block_put(int x, int y)
{
	int             i, j;

	for (j = 0; j < 5; j++) {
		for (i = 0; i < 5; i++) {
			if (block.blk[j][i] != 0)
				pattern_put(i + x, j + y, block.blk[j][i]);
		}
	}
}

/*	落下しているブロックを消す
*/
void
block_hide(int x, int y)
{
	int             i, j;
	for (j = 0; j < 5; j++) {
		for (i = 0; i < 5; i++) {
			if (block.blk[j][i] != 0)
				pattern_put(i + x, j + y, 0);
		}
	}
}

/*	ブロックを画面に表示してもいいかチェック
*/
int
block_check(int x, int y)
{
	int             flag;
	int             i, j;
	flag = 0;
	for (j = 0; j < 5; j++) {
		for (i = 0; i < 5; i++) {
			/* 表示すべき場所に既にデータがあれば却下	*/
			if (block.blk[j][i] != 0 && screen_map[y + j][x + i] != 0)
				flag = 1;
		}
	}
	return (flag);
}

/*	画面に縦＆横の線を引く
	（ダイナブック専用）
*/
#ifdef	DYNA
void
dotline_x(int x, int y)
{
	register        i;
	union REGS      inregs, outregs;
	inregs.x.ax = 0x0C07;
	for (i = 0; i < 16; i++) {
		inregs.x.cx = x + i;
		inregs.x.dx = y;
		int86(0x10, &inregs, &outregs);
	}
}
void
dotline_y(int x, int y)
{
	register        i;
	union REGS      inregs, outregs;
	inregs.x.ax = 0x0C07;
	for (i = 0; i < 16; i++) {
		inregs.x.cx = x;
		inregs.x.dx = y + i;
		int86(0x10, &inregs, &outregs);
	}
}
#endif

/*	固定されたブロックに枠線を入れる
*/
#ifndef	ANSI
void
block_line(int x, int y, int def)
{
#ifdef	DYNA
	x += map_offset;
	switch (def) {
	case 0:
		dotline_y(X(x) + 15, Y(y));
		break;
	case 1:
		dotline_y(X(x), Y(y));
		break;
	case 2:
		dotline_x(X(x), Y(y));
		break;
	case 3:
		dotline_x(X(x), Y(y) + 15);
		break;
	}
#endif
#ifdef	PC98
	x += map_offset;
	switch (def) {
	case 0:
		_moveto(X(x) + 15, Y(y));
		_lineto(X(x) + 15, Y(y) + 15);
		break;
	case 1:
		_moveto(X(x), Y(y));
		_lineto(X(x), Y(y) + 15);
		break;
	case 2:
		_moveto(X(x), Y(y));
		_lineto(X(x) + 15, Y(y));
		break;
	case 3:
		_moveto(X(x), Y(y) + 15);
		_lineto(X(x) + 15, Y(y) + 15);
		break;
	}
#endif
#ifdef	FMR
	x += map_offset;
	switch (def) {
	case 0:
		_moveto(X(x) + 15, Y(y));
		_lineto(X(x) + 15, Y(y) + 15);
		break;
	case 1:
		_moveto(X(x), Y(y));
		_lineto(X(x), Y(y) + 15);
		break;
	case 2:
		_moveto(X(x), Y(y));
		_lineto(X(x) + 15, Y(y));
		break;
	case 3:
		_moveto(X(x), Y(y) + 15);
		_lineto(X(x) + 15, Y(y) + 15);
		break;
	}
#endif
}
#endif

/*	落下したブロックを画面に固定する
*/
void
block_keep(int x, int y)
{
	int             bn;
	int             i, j;
	char            msg[24], buf[10];
	int             flg_back;
	char            sp;
#ifndef	ANSI
	int             k;
#endif
	/* 固定されたブロックがスペシャルキャラだった時の処理	*/
	if (now_block == CHR_SPECIAL && SG_SPECIAL == 1) {
		block_hide(x, y);
		sp = screen_map[y + 3][x + 2];
		/* 消去対象となるブロック検索	*/
		for (j = 1; j < MAP_X - 1; j++) {
			for (i = 0; i < MAP_Y; i++) {
				if (screen_map[i][j] == sp)
					screen_map[i][j] |= 0x80;
			}
		}
		bn = 0;
		/* 全体を落下対象とする(BB=0だった時の処理)	*/
		flg_back = SG_BLOCK_BREAK;
		SG_BLOCK_BREAK = 1;
		/* クリアされるブロックがなくなるまでループ	*/
		while (0 != block_clear()) {
			bn += 50;
			block_drop();
			if (bn != 50) {
				/* 連続技の場合のボーナス加算	*/
				strcpy(msg, "BONUS+");
				itoa(bn * 10, buf, 10);
				strcat(msg, buf);
				score_plus(bn);
				reason_put(msg);
			}
		}
		/* BBを元にもどす	*/
		SG_BLOCK_BREAK = flg_back;
		return;
	}

	/* 現在の最上位のブロックの高さを設定（スペシャルキャラのランダム登場の時に判定する）	*/
	block_hight = y;

	/* ブロックを固定する	*/
	for (j = 0; j < 5; j++) {
		for (i = 0; i < 5; i++) {
			if (block.blk[j][i] != 0 && screen_map[y + j][x + i] != 0xff && x + i < MAP_X - 1) {
				screen_map[y + j][x + i] = block.blk[j][i];
			}
		}
	}
#ifndef	ANSI
#ifndef	DYNA
	_setcolor(CG_WHITE);
#endif

	/* 落ちきったブロックに枠を付ける	*/
	for (j = y - 1; j < y + 6; j++) {
		for (i = x - 1; i < x + 6; i++) {
			if (j >= 0 && j < 24 && i > 0 && i < MAP_X - 1) {
				pattern_put(i, j, screen_map[j][i]);
				for (k = 0; k < 4; k++) {
					if (screen_map[j][i] != screen_map[j + dy[k]][i + dx[k]] &&
					    screen_map[j][i] != 0x00)
						block_line(i, j, k);
				}
			}
		}
	}
#else
	block_put(x, y);
#endif
	bn = 0;
	/* クリアされるブロックがなくなるまでループ	*/
	while (0 != block_clear()) {
		bn += 50;
		block_drop();
		if (bn != 50) {
			/* もちろん連続ボーナスも加算する	*/
			strcpy(msg, "BONUS+");
			itoa(bn * 10, buf, 10);
			strcat(msg, buf);
			score_plus(bn);
			reason_put(msg);
		}
	}
}

/*	ブロックを右回転する
*/
void
block_ring_right(void)
{
	int             i, j;
	char            buf;
	for (j = 0; j < 2; j++) {
		for (i = j; i < 4 - j; i++) {
			buf = block.blk[4 - j][i];
			block.blk[4 - j][i] = block.blk[4 - i][4 - j];
			block.blk[4 - i][4 - j] = block.blk[j][4 - i];
			block.blk[j][4 - i] = block.blk[i][j];
			block.blk[i][j] = buf;
		}
	}
}

/*	ブロックを左回転する
*/
void
block_ring_left(void)
{
	int             i, j;
	char            buf;
	for (j = 0; j < 2; j++) {
		for (i = j; i < 4 - j; i++) {
			buf = block.blk[i][j];
			block.blk[i][j] = block.blk[j][4 - i];
			block.blk[j][4 - i] = block.blk[4 - i][4 - j];
			block.blk[4 - i][4 - j] = block.blk[4 - j][i];
			block.blk[4 - j][i] = buf;
		}
	}
}

/*	ブロックのクリア処理
*/
int
block_clear(void)
{
	int             flag, double_flag;
	int             x, y;
	int             pt;
	char            msg[22], buf[22];
	char            bflag;

	flag = 0;
	double_flag = 0;
	bflag = 0x00;
	drop_left = 0;

	block_paint();	/* 面積計算	*/

	/* クリア領域が一部(BB=0)の時の落下範囲検索	*/
	if (SG_BLOCK_BREAK == 0) {
		for (x = 1; x < MAP_X - 1; x++) {
			for (y = 0; y < MAP_Y; y++) {
				if (screen_map[y][x] != 0xff && (screen_map[y][x] & 0x80) != 0x00) {
					drop_right = x;
					if (drop_left == 0)
						drop_left = x;
				}
			}
		}
	} else {
		drop_right = MAP_X - 1;
	}
	pt = 0;
	/* クリアされるブロックがあったか？	*/
	for (y = 0; y < MAP_Y - 1; y++) {
		for (x = 1; x < MAP_X - 1; x++) {
			if (screen_map[y][x] != 0xff && (screen_map[y][x] & 0x80) != 0x00) {
				if (bflag == 0x00)
					bflag = screen_map[y][x];
				/* ダブルクリアのフラグ設定	*/
				if (bflag != screen_map[y][x])
					double_flag = 1;
				screen_map[y][x] = CHR_CLR;
				pattern_put(x, y, CHR_CLR);
				pt++;
				flag = 1;
			}
		}
	}
	/* クリアされるブロックがなければおしまい	*/
	if (flag == 0)
		return (flag);
#ifdef	BGM
	sound_put(BGM_CLEAR);
#endif
#ifdef	BGMFM
	sound_put(BGM_CLEAR);
#endif
#ifdef	BGM
	bgm_wait_sound();
#endif
	score_plus(pt);
	itoa(pt, buf, 10);
	strcpy(msg, "CLEAR ");
	strcat(msg, buf);
	reason_put(msg);
	if (double_flag == 1) {
		score_plus(200);
		reason_put("DOUBLE2000");
	}
	if (score > level_pt) {
		level_pt += SG_LEVEL_PT;
		poro_level++;
		reason_put("<LEVEL UP>");
	}
	speed -= 2;
	if (speed < SG_SPEED_MIN)
		speed = SG_SPEED_MIN;
	/* クリアされたブロックのエリアをnullにする	*/
	for (y = 0; y < MAP_Y - 1; y++) {
		for (x = 1; x < MAP_X - 1; x++) {
			if (screen_map[y][x] == CHR_CLR) {
				screen_map[y][x] = 0x00;
				pattern_put(x, y, 0x00);
				pt++;
			}
		}
	}
	return (flag);
}

/*	ブロックをガラガラと落としまくる
*/
void
block_drop(void)
{
	int             end_flag = 0;
	int             x, y;
#ifndef	ANSI
	int             k;
#endif
	/* 落ちきるまでループ	*/
	do {
		end_flag = 1;
		for (y = MAP_Y - 2; y >= 0; y--) {
			for (x = drop_left; x <= drop_right; x++) {
				/* 対象ブロックの一つ下がnullなら落とす	*/
				if (screen_map[y][x] != 0xff && screen_map[y][x] != 0x00 && screen_map[y + 1][x] == 0x00) {
					screen_map[y + 1][x] = screen_map[y][x];
					screen_map[y][x] = 0x00;
					pattern_put(x, y, 0);
					pattern_put(x, y + 1, screen_map[y + 1][x]);
					end_flag = 0;
				}
			}
		}
	} while (end_flag == 0);
#ifndef	ANSI
#ifndef	DYNA
	_setcolor(CG_WHITE);
#endif
	/* 落ちきったら枠を書く	*/
	for (y = 0; y < MAP_Y; y++) {
		for (x = 1; x < MAP_X; x++) {
			if (y >= 0 && y < 24 && x > 0 && x < MAP_X - 1) {
				pattern_put(x, y, screen_map[y][x]);
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

/*	次に落下するブロックをランダムに選択
*/
void
block_select(void)
{
	int             i, j;
	struct blk_t far *blk;
	now_block = next_block;
	for (;;) {
		next_block = rand() % SG_BLOCK_NUM;
		/* 適当に現在のレベルに相当したブロックを選択する	*/
		if ((block_level[next_block] == poro_level) || (block_level[next_block] < poro_level && rand() % 6 == 0))
			break;
	}
	blk = (struct blk_t *) & blk_pt[next_block];
	if (SG_SPECIAL == 1) {
		/* スコアが一定以上ならスペシャルキャラ	*/
		if (score > special_pt) {
			next_block = -1;
			special_pt += SG_SPECIAL_POINT;
		}
		/* 一定の高さ以上でランダムにスペシャルキャラ	*/
		if (SG_HETA_FLAG != 0 && block_hight < SG_SPECIAL_HIGHT && (rand() % SG_HETA_FLAG) == 0) {
			next_block = -1;
		}
	}
	block_copy(now_block);
	if (SG_NEXT == 1) {
		/* 画面に次に出るブロックを表示してあげる	*/
		map_offset = 0;
#ifdef	PC98
		gcolor(CG_BLACK);
#endif
#ifdef	FMR
		gcolor(CG_BLACK);
#endif
#ifdef	ANSI
		for (i = 2; i < 9; i++) {
			locate(31 * 2, i);
			printf("　　　　　　");
		}
#endif
#ifdef	DYNA
		for (i = 2; i < 9; i++) {
			locate(31 * 2, i);
			printf("            ");
		}
#endif
#ifdef	PC98
		box_full(X(31), Y(2), X(38), Y(8));
#endif
#ifdef	FMR
		box_full(X(31), Y(2), X(38), Y(8));
#endif
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
	}
}

/*	TAKA氏によるブロック面積計算ルーチン
	（以下３関数）再帰処理してるので良くわからない。
*/
void
recurs_paint(int x, int y)
{
	if (x >= 0 && x < MAP_X && y >= 0 && y < MAP_Y && screen_map[y][x] == point) {
		paint_num++;
		screen_map[y][x] |= 0x80;
	} else
		return;
	recurs_paint(x - 1, y);
	recurs_paint(x + 1, y);
	recurs_paint(x, y - 1);
	recurs_paint(x, y + 1);
}
void
resume(int x, int y)
{
	if (x >= 0 && x < MAP_X && y >= 0 && y < MAP_Y && screen_map[y][x] == point) {
		screen_map[y][x] &= ~(0x80);
	} else
		return;
	resume(x - 1, y);
	resume(x + 1, y);
	resume(x, y - 1);
	resume(x, y + 1);
}
void
block_paint(void)
{
	int             x, y;
	for (y = 0; y < MAP_Y - 1; y++)
		for (x = 1; x < MAP_X - 1; x++)
			if (!(screen_map[y][x] & 0x80) && screen_map[y][x] != 0) {
				paint_num = 0;
				point = screen_map[y][x];
				recurs_paint(x, y);
				if (paint_num < SG_BLOCK) {
					point |= 0x80;
					resume(x, y);
				};
			}
}
