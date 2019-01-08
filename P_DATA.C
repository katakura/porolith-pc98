/*
		Action Puzzle Game "POROLITH"

			PC-9801,286
			FMR50/60/70
			Dynabook
			and MSDOS Machine

	Copyright(C)1990 by Ko.Mo.Mo. Allrights Reserved.
*/
#include	"porolith.h"
FILE           *fp;
struct cg_t far *chr_pt;
struct blk_t far *blk_pt;
struct fnt_t far *fnt_pt;
struct fnthan_t far *fnthan_pt;
struct blk_t    block;
char            screen_map[MAP_Y][MAP_X];
int             dx[4] = {1, -1, 0, 0};
int             dy[4] = {0, 0, -1, 1};
int             map_offset;
int             paint_num;
char            point;
int             score;
char            score_reason[5][24];
int             speed;
char            r_mask;
char            g_mask;
char            b_mask;
int             r_flag;
int             drop_left;
int             drop_right;
int             now_block;
int             next_block;
union REGS      inregs, outregs;
struct SREGS    segregs;
int             special_pt;
int             block_hight;
int             poro_level;
int             block_level[255];
int             level_pt;
char            open_file[128];
char            rank[10][5] = {" 1ST", " 2ND", " 3RD", " 4TH", " 5TH", " 6TH", " 7TH", " 8TH", " 9TH", "10TH"};
char            score_file[80];
#ifdef	FMR
char            image_buf[162];
int            *bgm[8];
#endif
#ifdef	ANSI
char            ansi_font[121] = {"@Ih”“•fij–{C|D^‚O‚P‚Q‚R‚S‚T‚U‚V‚W‚XFGƒ„H—‚`‚a‚b‚c‚d‚e‚f‚g‚h‚i‚j‚k‚l‚m‚n‚o‚p‚q‚r‚s‚t‚u‚v‚w‚x‚y"};
#endif
#ifdef	DYNA
char far       *cgpt[4] = {(char far *) 0xb8000000, (char far *) 0xba000000, (char far *) 0xbc000000, (char far *) 0xbe000000};
#else
char far       *cgpt_b = (char far *) 0xa8000000;
char far       *cgpt_r = (char far *) 0xb0000000;
char far       *cgpt_g = (char far *) 0xb8000000;
#endif
struct high_score_t high_score[10];
int             SG_BLOCK_BREAK = 0;
int             SG_SPEED = 300;
int             SG_BLOCK = 23;
int             SG_NEXT = 1;
int             SG_BLOCK_NUM = 31;
char            SG_BLOCK_NAME[80] = {"porolith.blk"};
char            SG_CHAR_NAME[80] = {"porolith.chr"};
char            SG_PANIC_NAME[80] = {"porolith.pnc"};
char            SG_FONT_NAME[80] = {"porolith.fnt"};
int             SG_SPEED_MIN = 50;
char            SG_CONFIG[80] = {"porolith.cfg"};
int             SG_SPECIAL = 1;
int             SG_SPECIAL_POINT = 500;
int             SG_HETA_FLAG = 0;
int             SG_SPECIAL_HIGHT = 8;
int             SG_LEVEL_PT = 300;
char            SG_ANSI_CHAR[25] = {"@ ¡›œ™š–”E@"};
char            SG_EFS_NAME[80] = {"porolith.efs"};
int             SG_BGM = 1;
int             SG_NETWORK = 0;
int             SG_TITLE = 1;
int             SG_BLOCKDEMO = 1;
