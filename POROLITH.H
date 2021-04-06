/*
		Action Puzzle Game "POROLITH"

			PC-9801,286
			FMR50/60/70
			Dynabook
			and MSDOS Machine

	Copyright(C)1990 by Ko.Mo.Mo. Allrights Reserved.
*/

/*	各種ヘッダファイルの一括include
*/
#include	<stdio.h>
#include	<conio.h>
#ifdef	PC98
#include	<graph.h>
#endif
#ifdef	FMR
#include	<graph.h>
#endif
#include	<malloc.h>
#include	<string.h>
#include	<stdlib.h>
#include	<time.h>
#include	<ctype.h>
#include	<dos.h>
#include	<signal.h>
#include	"komomo.h"	/* komomo作のグラフィック簡素化ヘッダ	*/
#ifdef	BGM
#include	"bgmlibl.h"	/* Femy氏作のBGMLIBのラージモデル版	*/
#endif
#ifdef	BGMFM
#include	"soundys.h"	/* Paci氏作のFMR用BGMライブラリ	*/
#endif

/*	各種BGMのエントリ番号
*/
#define		BGM_RING	1
#define		BGM_DROP	2
#define		BGM_UNMOVE	3
#define		BGM_MOVE	4
#define		BGM_CLEAR	5
#define		BGM_GAME_OVER	6
#define		BGM_NAME_INPUT	7

/*	現在のぽろりす☆の履歴管理番号
*/
#define		VERSION		"1.56"

/*	porolith.chrのキャラクタエントリ番号
*/
#define		FNT_NUM		256
#define		CG_NUM		12
#define		MAP_X		15
#define		MAP_Y		25
#define		MAP_XX		13
#define		CHR_WAKU	9
#define		CHR_BGC		10
#define		CHR_CLR		11
#define		CHR_SPECIAL	-1

/*	グラフィックフォント表示時のノーマル／リバースのフラグ
*/
#define		MASK_NORMAL	0
#define		MASK_REVERCE	1

/*	９８のキーＢＩＯＳ取得時のリターン値
*/
#ifdef	PC98
#define		scan_ESC	0
#define		scan_RET	0x1c
#define		scan_SPC	0x34
#define		scan_0		0x4e
#define		scan_2		0x4b
#define		scan_4		0x46
#define		scan_5		0x47
#define		scan_6		0x48
#define		scan_8		0x43
#define		scan_Q		0x10
#define		scan_J		0x23
#define		scan_K		0x24
#define		scan_L		0x25
#define		scan_N		0x2e
#endif

/*	ハイスコアの管理用型定義
*/
struct high_score_t {
	int             score;
	char            name[13];
	char            date[9];
	char            time[9];
};

/*	グラフィックキャラクタ管理用型定義
*/
struct cg_t {
	char            r[32];
	char            g[32];
	char            b[32];
};

/*	ブロックデータ管理用の型定義
*/
struct blk_t {
	char            blk[5][5];
};

/*	フォントデータ管理用の型定義
*/
struct fnt_t {
	char            fnt[32];
};
struct fnthan_t {
	char            fnt[16];
};

/*	各種外部参照定義
*/
extern int
porolith(void);
extern int
game_over(void);
extern void
score_plus(int);
extern void
poro_init(void);
extern int
cg_buf_read(void);
extern int
blk_buf_read(void);
extern int
fnt_buf_read(void);
extern int
config_read(void);
extern void
recurs_paint(int, int);
extern void
resume(int, int);
extern void
block_paint(void);
extern void
block_copy(int);
extern void
block_put(int, int);
extern void
block_hide(int, int);
extern int
block_check(int, int);
#ifndef	ANSI
extern void
block_line(int, int, int);
#endif
extern void
block_keep(int, int);
extern void
block_ring_right(void);
extern void
block_ring_left(void);
extern int
block_clear(void);
extern void
block_drop(void);
extern void
block_select(void);
extern void
pattern_put(int, int, int);
extern int
title_put(void);
extern int
title_put2(void);
extern void
initial_screen(void);
extern void
reason_put(char *);
extern void
out_text(int, int, char *);
extern void
out_text_han(int, int, char *);
extern void
mask_set(char, char, char, int);
extern void
score_put(int, int, int);
#ifdef	PC98
extern void
gscreen(int);
#endif
#ifdef	FMR
extern void
gscreen(int);
#endif
extern void
panic_put(void);
extern void
screen_paint(int, int, int, int, int);
extern void
indent(char *);
extern void
usage_put(void);
extern int
data_set(char *);
extern FILE    *
fpopen(char *, char *);
extern int
scr_read(char *, int);
extern void
high_score_put(int);
#ifdef	BGM
extern int
bgm_read(void);
#endif
#ifdef	BGMFM
extern void
fs_read_sdata(char *);
extern int
bgm_read(void);
#endif
#ifdef	PC98
extern void
sound_put(int);
#endif
#ifdef	FMR
extern void
sound_put(int);
#endif
extern void
name_input(char *nam);
extern int
rank_sort(void);
extern void
file_out(void);
extern void
porolith_abort(void);
#ifdef	PC98
extern int
kbhit2(void);
extern char
getch2(void);
#endif
#ifdef	ANSI
extern void
ansi_rescreen(void);
extern int
set_RAW_mode(void);
extern int
set_COOKED_mode(void);
#endif
#ifdef	DYNA
extern void
ansi_rescreen(void);
#endif

/*	共通データの外部参照定義
*/
extern FILE    *fp;
extern struct cg_t far *chr_pt;
extern struct blk_t far *blk_pt;
extern struct fnt_t far *fnt_pt;
extern struct fnthan_t far *fnthan_pt;
extern struct blk_t block;
extern char     screen_map[MAP_Y][MAP_X];
extern int      dx[4];
extern int      dy[4];
extern int      map_offset;
extern int      paint_num;
extern char     point;
extern int      score;
extern char     score_reason[5][24];
extern int      speed;
#ifdef	DYNA
extern char far *cgpt[4];
#else
extern char far *cgpt_b;
extern char far *cgpt_r;
extern char far *cgpt_g;
#endif
extern char     r_mask;
extern char     g_mask;
extern char     b_mask;
extern int      r_flag;
extern int      drop_left;
extern int      drop_right;
extern int      now_block;
extern int      next_block;
extern union REGS inregs, outregs;
extern struct SREGS segregs;
extern int      special_pt;
extern int      block_hight;
extern int      poro_level;
extern int      block_level[255];
extern int      level_pt;
extern char     open_file[128];
extern struct high_score_t high_score[10];
extern char     rank[10][5];
extern char     score_file[80];
#ifdef	FMR
extern char     image_buf[162];
extern int     *bgm[8];
#endif
#ifdef	ANSI
extern char     ansi_block[25];
extern char     ansi_font[121];
#endif

/*	ぽろりす☆の各種環境設定用データの外部参照定義
*/
extern int      SG_BLOCK_BREAK;
extern int      SG_SPEED;
extern int      SG_BLOCK;
extern int      SG_NEXT;
extern int      SG_BLOCK_NUM;
extern char     SG_BLOCK_NAME[80];
extern char     SG_FONT_NAME[80];
extern char     SG_CHAR_NAME[80];
extern char     SG_PANIC_NAME[80];
extern int      SG_SPEED_MIN;
extern char     SG_CONFIG[80];
extern int      SG_SPECIAL;
extern int      SG_SPECIAL_POINT;
extern int      SG_HETA_FLAG;
extern int      SG_SPECIAL_HIGHT;
extern int      SG_LEVEL_PT;
extern char     SG_ANSI_CHAR[25];
extern char     SG_EFS_NAME[80];
extern int      SG_BGM;
extern int      SG_NETWORK;
extern int      SG_TITLE;
extern int      SG_BLOCKDEMO;
