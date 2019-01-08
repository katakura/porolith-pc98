/*
		Action Puzzle Game "POROLITH"

			PC-9801,286
			FMR50/60/70
			Dynabook
			and MSDOS Machine

	Copyright(C)1990 by Ko.Mo.Mo. Allrights Reserved.
*/

#include	"porolith.h"

/*	�w�肳�ꂽ�u���b�N�𗎉��Ώۂ̃u���b�N�ɐݒ肷��
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
				/* �X�y�V�����L����(=-1)�ȊO�Ȃ�R�s�[	*/
				block.blk[y][x] = blk->blk[y][x];
			} else {
				/* �X�y�V�����L�����Ȃ�^�񒆂݂̂ɐݒ�	*/
				if (x == 2 && y == 2) {
					block.blk[y][x] = 0x08;
				} else {
					block.blk[y][x] = 0x00;
				}
			}
		}
	}
}

/*	��ʏ�ɗ������̃u���b�N��\������
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

/*	�������Ă���u���b�N������
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

/*	�u���b�N����ʂɕ\�����Ă��������`�F�b�N
*/
int
block_check(int x, int y)
{
	int             flag;
	int             i, j;
	flag = 0;
	for (j = 0; j < 5; j++) {
		for (i = 0; i < 5; i++) {
			/* �\�����ׂ��ꏊ�Ɋ��Ƀf�[�^������΋p��	*/
			if (block.blk[j][i] != 0 && screen_map[y + j][x + i] != 0)
				flag = 1;
		}
	}
	return (flag);
}

/*	��ʂɏc�����̐�������
	�i�_�C�i�u�b�N��p�j
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

/*	�Œ肳�ꂽ�u���b�N�ɘg��������
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

/*	���������u���b�N����ʂɌŒ肷��
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
	/* �Œ肳�ꂽ�u���b�N���X�y�V�����L�������������̏���	*/
	if (now_block == CHR_SPECIAL && SG_SPECIAL == 1) {
		block_hide(x, y);
		sp = screen_map[y + 3][x + 2];
		/* �����ΏۂƂȂ�u���b�N����	*/
		for (j = 1; j < MAP_X - 1; j++) {
			for (i = 0; i < MAP_Y; i++) {
				if (screen_map[i][j] == sp)
					screen_map[i][j] |= 0x80;
			}
		}
		bn = 0;
		/* �S�̂𗎉��ΏۂƂ���(BB=0���������̏���)	*/
		flg_back = SG_BLOCK_BREAK;
		SG_BLOCK_BREAK = 1;
		/* �N���A�����u���b�N���Ȃ��Ȃ�܂Ń��[�v	*/
		while (0 != block_clear()) {
			bn += 50;
			block_drop();
			if (bn != 50) {
				/* �A���Z�̏ꍇ�̃{�[�i�X���Z	*/
				strcpy(msg, "BONUS+");
				itoa(bn * 10, buf, 10);
				strcat(msg, buf);
				score_plus(bn);
				reason_put(msg);
			}
		}
		/* BB�����ɂ��ǂ�	*/
		SG_BLOCK_BREAK = flg_back;
		return;
	}

	/* ���݂̍ŏ�ʂ̃u���b�N�̍�����ݒ�i�X�y�V�����L�����̃����_���o��̎��ɔ��肷��j	*/
	block_hight = y;

	/* �u���b�N���Œ肷��	*/
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

	/* �����������u���b�N�ɘg��t����	*/
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
	/* �N���A�����u���b�N���Ȃ��Ȃ�܂Ń��[�v	*/
	while (0 != block_clear()) {
		bn += 50;
		block_drop();
		if (bn != 50) {
			/* �������A���{�[�i�X�����Z����	*/
			strcpy(msg, "BONUS+");
			itoa(bn * 10, buf, 10);
			strcat(msg, buf);
			score_plus(bn);
			reason_put(msg);
		}
	}
}

/*	�u���b�N���E��]����
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

/*	�u���b�N������]����
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

/*	�u���b�N�̃N���A����
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

	block_paint();	/* �ʐόv�Z	*/

	/* �N���A�̈悪�ꕔ(BB=0)�̎��̗����͈͌���	*/
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
	/* �N���A�����u���b�N�����������H	*/
	for (y = 0; y < MAP_Y - 1; y++) {
		for (x = 1; x < MAP_X - 1; x++) {
			if (screen_map[y][x] != 0xff && (screen_map[y][x] & 0x80) != 0x00) {
				if (bflag == 0x00)
					bflag = screen_map[y][x];
				/* �_�u���N���A�̃t���O�ݒ�	*/
				if (bflag != screen_map[y][x])
					double_flag = 1;
				screen_map[y][x] = CHR_CLR;
				pattern_put(x, y, CHR_CLR);
				pt++;
				flag = 1;
			}
		}
	}
	/* �N���A�����u���b�N���Ȃ���΂����܂�	*/
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
	/* �N���A���ꂽ�u���b�N�̃G���A��null�ɂ���	*/
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

/*	�u���b�N���K���K���Ɨ��Ƃ��܂���
*/
void
block_drop(void)
{
	int             end_flag = 0;
	int             x, y;
#ifndef	ANSI
	int             k;
#endif
	/* ��������܂Ń��[�v	*/
	do {
		end_flag = 1;
		for (y = MAP_Y - 2; y >= 0; y--) {
			for (x = drop_left; x <= drop_right; x++) {
				/* �Ώۃu���b�N�̈����null�Ȃ痎�Ƃ�	*/
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
	/* ������������g������	*/
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

/*	���ɗ�������u���b�N�������_���ɑI��
*/
void
block_select(void)
{
	int             i, j;
	struct blk_t far *blk;
	now_block = next_block;
	for (;;) {
		next_block = rand() % SG_BLOCK_NUM;
		/* �K���Ɍ��݂̃��x���ɑ��������u���b�N��I������	*/
		if ((block_level[next_block] == poro_level) || (block_level[next_block] < poro_level && rand() % 6 == 0))
			break;
	}
	blk = (struct blk_t *) & blk_pt[next_block];
	if (SG_SPECIAL == 1) {
		/* �X�R�A�����ȏ�Ȃ�X�y�V�����L����	*/
		if (score > special_pt) {
			next_block = -1;
			special_pt += SG_SPECIAL_POINT;
		}
		/* ���̍����ȏ�Ń����_���ɃX�y�V�����L����	*/
		if (SG_HETA_FLAG != 0 && block_hight < SG_SPECIAL_HIGHT && (rand() % SG_HETA_FLAG) == 0) {
			next_block = -1;
		}
	}
	block_copy(now_block);
	if (SG_NEXT == 1) {
		/* ��ʂɎ��ɏo��u���b�N��\�����Ă�����	*/
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
			printf("�@�@�@�@�@�@");
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

/*	TAKA���ɂ��u���b�N�ʐόv�Z���[�`��
	�i�ȉ��R�֐��j�ċA�������Ă�̂ŗǂ��킩��Ȃ��B
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