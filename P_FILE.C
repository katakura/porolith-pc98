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
cg_buf_read(void)
{
	struct cg_t far *cg;
	int             i, j, k, l;
	char            r, g, b, num;
	char            buf[80];
	printf("reading %s ", SG_CHAR_NAME);
	fp = fpopen(SG_CHAR_NAME, "r");
	if (!fp) {
		printf("/ can't open\n");
		return (1);
	}
	chr_pt = _fmalloc(sizeof(struct cg_t) * CG_NUM);
	if (!chr_pt) {
		puts("/ memory over flow!");
		fclose(fp);
		return (1);
	}
	for (l = 0; l < CG_NUM; l++) {
		cg = (struct cg_t *) & chr_pt[l];
		fgets(buf, sizeof(buf), fp);
		indent(buf);
		if (buf[0] != ';') {
			printf("/ illigal character file\n");
			fclose(fp);
			return (1);
		}
		for (i = 0; i < 16; i++) {
			fgets(buf, sizeof(buf), fp);
			indent(buf);
			for (k = 0; k < 2; k++) {
				r = g = b = 0x00;
				for (j = k * 8; j < k * 8 + 8; j++) {
					num = buf[j] - '0';
					r = r << 1;
					g = g << 1;
					b = b << 1;
					r |= ((num & 0x04) != 0) ? 1 : 0;
					g |= ((num & 0x02) != 0) ? 1 : 0;
					b |= ((num & 0x01) != 0) ? 1 : 0;
				}
				cg->r[i * 2 + k] = (char) r;
				cg->g[i * 2 + k] = (char) g;
				cg->b[i * 2 + k] = (char) b;
			}
		}
	}
	printf("\n");
	fclose(fp);
	return (0);
}
int
blk_buf_read(void)
{
	struct blk_t far *blk;
	register        i, j, k, flg;
	char            buf[80];
	printf("reading %s ", SG_BLOCK_NAME);
	fp = fpopen(SG_BLOCK_NAME, "r");
	if (!fp) {
		printf("/ can't open\n");
		return (1);
	}
	blk_pt = _fmalloc(sizeof(struct blk_t) * SG_BLOCK_NUM);
	if (!blk_pt) {
		puts("/ memory over flow");
		fclose(fp);
		return (1);
	}
	for (i = 0; i < SG_BLOCK_NUM; i++) {
		blk = (struct blk_t *) & blk_pt[i];
		fgets(buf, sizeof(buf), fp);
		indent(buf);
		if (buf[0] != ';') {
			printf("/ illigal block file\n");
			fclose(fp);
			return (1);
		}
		block_level[i] = 1;
		for (j = 0; j < 5; j++) {
			fgets(buf, sizeof(buf), fp);
			indent(buf);
			if (NULL != strstr(buf, "LEVEL=")) {
				block_level[i] = atoi(&buf[6]);
				fgets(buf, sizeof(buf), fp);
				indent(buf);
			}
			for (k = 0; k < 5; k++) {
				blk->blk[j][k] = buf[k] - '0';
			}
		}
	}
	fclose(fp);
	flg = 0;
	for (i = 0; i < SG_BLOCK_NUM; i++) {
		if (block_level[i] == 1)
			flg = 1;
	}
	if (flg == 0) {
		printf("/ illegal level definition in block data\n");
		return (1);
	}
	printf("\n");
	return (0);
}
int
fnt_buf_read(void)
{
	struct fnt_t far *fnt;
	struct fnthan_t far *fnthan;
	register        i, j, k, l;
	char            r;
	char            buf[80];
	printf("reading %s ", SG_FONT_NAME);
	fp = fpopen(SG_FONT_NAME, "rb");
	if (!fp) {
		printf("/ can't open!\n");
		return (1);
	}
	fnt_pt = _fmalloc(sizeof(struct fnt_t) * FNT_NUM);
	if (!fnt_pt) {
		puts("/ memory over flow!");
		fclose(fp);
		return (1);
	}
	fnthan_pt = _fmalloc(sizeof(struct fnthan_t) * FNT_NUM);
	if (!fnthan_pt) {
		puts("/ memory over flow!");
		fclose(fp);
		return (1);
	}
	fread(buf, 32, 1, fp);
	fread(fnt_pt, FNT_NUM * sizeof(struct fnt_t), 1, fp);
	for (l = 0; l <= FNT_NUM; l++) {
		fnthan = (struct fnthan_t *) & fnthan_pt[l];
		fnt = (struct fnt_t *) & fnt_pt[l];
		for (i = 0; i < 16; i++) {
			r = 0;
			for (k = 0; k < 2; k++) {
				for (j = 1; j < 129; j *= 4) {
					r = r >> 1;
					if ((fnt->fnt[i * 2 + 1 - k] & j) != 0)
						r |= 0x80;
				}
			}
			fnthan->fnt[i] = r;
		}
	}
	printf("\n");
	fclose(fp);
	return (0);
}
int
config_read(void)
{
	char            buf[80];
	printf("reading %s ", SG_CONFIG);
	fp = fpopen(SG_CONFIG, "r");
	if (!fp) {
		printf("/ can't open!\n");
		return (1);
	}
	fgets(buf, sizeof(buf), fp);
	indent(buf);
	while (!feof(fp)) {
		if (1 == data_set(buf) && strstr(buf, "=")) {
			printf("/ parameter error!! '%s'\n", buf);
			fclose(fp);
			exit(1);
		}
		fgets(buf, sizeof(buf), fp);
		indent(buf);
	}
	fclose(fp);
	printf("\n");
	return (0);
}
void
panic_put(void)
{
	int             i;
	char            buf[512];
#ifdef	ANSI
	printf("%s", ESC_CLS);
#endif
#ifdef	DYNA
	printf("%s", ESC_CLS);
#endif
#ifdef	PC98
	gscreen(1);
#endif
#ifdef	FMR
	gscreen(1);
#endif
	fp = fpopen(SG_PANIC_NAME, "r");
	if (!fp) {
		printf("%s can't open !!\n", SG_PANIC_NAME);
	} else {
		for (i = 0; i < 25; i++) {
			fgets(buf, sizeof(buf), fp);
			buf[strlen(buf) - 1] = 0x00;
			locate(0, i);
			printf("%s", buf);
		}
		fclose(fp);
	}
	getch();
#ifdef	PC98
	printf("\x1b[m%s", ESC_CLS);
	gscreen(0);
#endif
#ifdef	FMR
	printf("\x1b[m%s", ESC_CLS);
	gscreen(0);
#endif
}
void
indent(char *str)
{
	char            buf[128];
	int             i;
	int             pos;
	strcpy(buf, str);
	pos = 0;
	for (i = 0; i < strlen(buf); i++) {
		if (buf[i] != ' ' && buf[i] != '	' && buf[i] != '"') {
			str[pos] = buf[i];
			pos++;
		}
	}
	str[pos] = 0x00;
	if (str[0] == ';')
		str[1] = 0x00;
	for (i = 0; i < strlen(str); i++) {
		if (i != 0 && str[i] == ';' || str[i] == '\n') {
			str[i] = 0x00;
			return;
		}
	}
}
int
data_set(char *str)
{
	char            buf[40];
	int             i;
	strcpy(buf, str);
	for (i = 0; i < strlen(buf); i++) {
		if (buf[i] >= 'a' && buf[i] <= 'z')
			buf[i] -= 'a' - 'A';
	}
	if (buf[0] == ';' || buf[0] == 0x00)
		return (0);
	if (strstr(buf, "ANSICHAR=")) {
		strcpy(SG_ANSI_CHAR, &buf[9]);
		return (0);
	}
	if (strstr(buf, "AN=")) {
		strcpy(SG_ANSI_CHAR, &buf[3]);
		return (0);
	}
	if (strstr(buf, "BLOCKNAME=")) {
		strcpy(SG_BLOCK_NAME, &buf[10]);
		if (!strchr(SG_BLOCK_NAME, '.'))
			strcat(SG_BLOCK_NAME, ".BLK");
		return (0);
	}
	if (strstr(buf, "BF=")) {
		strcpy(SG_BLOCK_NAME, &buf[3]);
		if (!strchr(SG_BLOCK_NAME, '.'))
			strcat(SG_BLOCK_NAME, ".BLK");
		return (0);
	}
	if (strstr(buf, "PANICNAME=")) {
		strcpy(SG_PANIC_NAME, &buf[10]);
		if (!strchr(SG_PANIC_NAME, '.'))
			strcat(SG_PANIC_NAME, ".PNC");
		return (0);
	}
	if (strstr(buf, "PF=")) {
		strcpy(SG_PANIC_NAME, &buf[3]);
		if (!strchr(SG_PANIC_NAME, '.'))
			strcat(SG_PANIC_NAME, ".PNC");
		return (0);
	}
	if (strstr(buf, "FONTNAME=")) {
		strcpy(SG_FONT_NAME, &buf[9]);
		if (!strchr(SG_FONT_NAME, '.'))
			strcat(SG_FONT_NAME, ".FNT");
		return (0);
	}
	if (strstr(buf, "FF=")) {
		strcpy(SG_FONT_NAME, &buf[3]);
		if (!strchr(SG_FONT_NAME, '.'))
			strcat(SG_FONT_NAME, ".FNT");
		return (0);
	}
	if (strstr(buf, "CHRNAME=")) {
		strcpy(SG_CHAR_NAME, &buf[8]);
		if (!strchr(SG_CHAR_NAME, '.'))
			strcat(SG_CHAR_NAME, ".CHR");
		return (0);
	}
	if (strstr(buf, "CF=")) {
		strcpy(SG_CHAR_NAME, &buf[3]);
		if (!strchr(SG_CHAR_NAME, '.'))
			strcat(SG_CHAR_NAME, ".CHR");
		return (0);
	}
	if (strstr(buf, "EFSNAME=")) {
		strcpy(SG_EFS_NAME, &buf[8]);
		if (!strchr(SG_EFS_NAME, '.'))
			strcat(SG_EFS_NAME, ".EFS");
		return (0);
	}
	if (strstr(buf, "EF=")) {
		strcpy(SG_EFS_NAME, &buf[3]);
		if (!strchr(SG_EFS_NAME, '.'))
			strcat(SG_EFS_NAME, ".EFS");
		return (0);
	}
	if (strstr(buf, "BLOCKNUM=")) {
		SG_BLOCK_NUM = atoi(&buf[9]);
		return (0);
	}
	if (strstr(buf, "BN=")) {
		SG_BLOCK_NUM = atoi(&buf[3]);
		return (0);
	}
	if (strstr(buf, "LEVELPT=")) {
		SG_LEVEL_PT = atoi(&buf[8]);
		SG_LEVEL_PT /= 10;
		return (0);
	}
	if (strstr(buf, "LP=")) {
		SG_LEVEL_PT = atoi(&buf[3]);
		SG_LEVEL_PT /= 10;
		return (0);
	}
	if (strstr(buf, "SPEED=")) {
		SG_SPEED = atoi(&buf[6]);
		return (0);
	}
	if (strstr(buf, "SP=")) {
		SG_SPEED = atoi(&buf[3]);
		return (0);
	}
	if (strstr(buf, "SPECIAL=")) {
		SG_SPECIAL = atoi(&buf[8]);
		return (0);
	}
	if (strstr(buf, "SS=")) {
		SG_SPECIAL = atoi(&buf[3]);
		return (0);
	}
	if (strstr(buf, "SPECIALPT=")) {
		SG_SPECIAL_POINT = atoi(&buf[10]);
		SG_SPECIAL_POINT /= 10;
		return (0);
	}
	if (strstr(buf, "ST=")) {
		SG_SPECIAL_POINT = atoi(&buf[3]);
		SG_SPECIAL_POINT /= 10;
		return (0);
	}
	if (strstr(buf, "SPEEDMIN=")) {
		SG_SPEED_MIN = atoi(&buf[9]);
		return (0);
	}
	if (strstr(buf, "SM=")) {
		SG_SPEED_MIN = atoi(&buf[3]);
		return (0);
	}
	if (strstr(buf, "CLEARBLOCK=")) {
		SG_BLOCK = atoi(&buf[11]);
		return (0);
	}
	if (strstr(buf, "CB=")) {
		SG_BLOCK = atoi(&buf[3]);
		return (0);
	}
	if (strstr(buf, "SPECIALRATE=")) {
		SG_HETA_FLAG = atoi(&buf[12]);
		return (0);
	}
	if (strstr(buf, "SR=")) {
		SG_HETA_FLAG = atoi(&buf[3]);
		return (0);
	}
	if (strstr(buf, "SPECIALHIGHT=")) {
		SG_SPECIAL_HIGHT = atoi(&buf[13]);
		return (0);
	}
	if (strstr(buf, "SH=")) {
		SG_SPECIAL_HIGHT = atoi(&buf[3]);
		return (0);
	}
	if (strstr(buf, "DROP=")) {
		SG_BLOCK_BREAK = atoi(&buf[5]);
		return (0);
	}
	if (strstr(buf, "DR=")) {
		SG_BLOCK_BREAK = atoi(&buf[3]);
		return (0);
	}
	if (strstr(buf, "NEXT=")) {
		SG_NEXT = atoi(&buf[5]);
		return (0);
	}
	if (strstr(buf, "NX=")) {
		SG_NEXT = atoi(&buf[3]);
		return (0);
	}
	if (strstr(buf, "BGM=")) {
		SG_BGM = atoi(&buf[4]);
		return (0);
	}
	if (strstr(buf, "BG=")) {
		SG_BGM = atoi(&buf[3]);
		return (0);
	}
	if (strstr(buf, "NETWORK=")) {
		SG_NETWORK = atoi(&buf[8]);
		return (0);
	}
	if (strstr(buf, "NT=")) {
		SG_NETWORK = atoi(&buf[3]);
		return (0);
	}
	if (strstr(buf, "TITLE=")) {
		SG_TITLE = atoi(&buf[6]);
		return (0);
	}
	if (strstr(buf, "TT=")) {
		SG_TITLE = atoi(&buf[3]);
		return (0);
	}
	if (strstr(buf, "BLOCKDEMO=")) {
		SG_BLOCKDEMO = atoi(&buf[10]);
		return (0);
	}
	if (strstr(buf, "BD=")) {
		SG_BLOCKDEMO = atoi(&buf[3]);
		return (0);
	}
	return (1);
}
FILE           *
fpopen(fname, mode)
	char           *fname;
	char           *mode;
{
	char            path_buffer[80];
	int             i, j;
	char            buff[258];
	if ((fp = fopen(fname, mode)) != NULL)
		return (fp);
	_searchenv(fname, "POROLITH", path_buffer);
	if (strlen(path_buffer) == 0) {
		_searchenv(fname, "PATH", path_buffer);
		if (strlen(path_buffer) == 0) {
			return (NULL);
		}
	}
	for (i = 0, j = 0; i < strlen(path_buffer); i++) {
		if (path_buffer[i] == '\\' && path_buffer[i + 1] == '\\');
		else {
			buff[j++] = path_buffer[i];
			buff[j] = '\0';
		}
	}
	fp = fopen(buff, mode);
	return (fp);
}
int
scr_read(char *exe_path, int slct_flg)
{
	char            drive[3];
	char            dir[80];
	char            fname[9];
	char            ext[5];
	int             ch, i;
	char            buf[40];
	_splitpath(exe_path, drive, dir, fname, ext);
	strcpy(open_file, drive);
	strcat(open_file, dir);
	_splitpath(SG_CONFIG, drive, dir, fname, ext);
	strcat(open_file, fname);
	strcat(open_file, ".scr");
	if (slct_flg == 1)
		printf("reading score file\n");
	if (NULL == (fp = fopen(open_file, "r"))) {
		if (slct_flg == 0)
			return (0);
		printf("no '%s' . create?(y/n):", open_file);
		ch = getch();
		printf("\n");
		if (ch != 'y' && ch != 'Y') {
			printf("exit porolith\n");
			return (1);
		}
		for (i = 0; i < 10; i++) {
			high_score[i].score = 0;
			strcpy(high_score[i].name, "------------");
			strcpy(high_score[i].date, "00/00/00");
			strcpy(high_score[i].time, "00:00:00");
		}
	} else {
		for (i = 0; i < 10; i++) {
			fgets(buf, sizeof(buf), fp);
			if (buf[5] != '0' || buf[6] != ' ' || buf[28] != ' ') {
				printf("illigal file format of score file!\n");
				fclose(fp);
				return (1);
			}
			buf[5] = 0x00;
			buf[19] = 0x00;
			buf[28] = 0x00;
			buf[37] = 0x00;
			high_score[i].score = atoi(&buf[0]);
			strcpy(high_score[i].name, &buf[7]);
			strcpy(high_score[i].date, &buf[20]);
			strcpy(high_score[i].time, &buf[29]);
		}
		fclose(fp);
	}
	return (0);
}
int
bgm_read(void)
{
#ifdef	BGM
	int             err;
	char            buff[258];
	char            path_buffer[80];
	int             i, j;
	bgm_init();
	err = bgm_read_sdata(SG_EFS_NAME);
	if (err != BGM_COMPLETE) {
		_searchenv(SG_EFS_NAME, "POROLITH", path_buffer);
		if (strlen(path_buffer) == 0) {
			_searchenv(SG_EFS_NAME, "PATH", path_buffer);
			if (strlen(path_buffer) == 0) {
				printf("'%s' can't open!\n", SG_EFS_NAME);
				return (1);
			}
		}
		for (i = 0, j = 0; i < strlen(path_buffer); i++) {
			if (path_buffer[i] == '\\' && path_buffer[i + 1] == '\\');
			else {
				buff[j++] = path_buffer[i];
				buff[j] = '\0';
			}
		}
		err = bgm_read_sdata(buff);
		if (err != BGM_COMPLETE) {
			printf("'%s' illigal file format of efsfile", buff);
			return (1);
		}
	}
#endif

#ifdef	BGMFM
	char            path_buffer[80];
	int             i, j;
	char            buff[258];
	_searchenv(SG_EFS_NAME, "POROLITH", path_buffer);
	if (strlen(path_buffer) == 0) {
		_searchenv(SG_EFS_NAME, "PATH", path_buffer);
		if (strlen(path_buffer) == 0) {
			printf("'%s' can't open!\n", SG_EFS_NAME);
			return (1);
		}
	}
	for (i = 0, j = 0; i < strlen(path_buffer); i++) {
		if (path_buffer[i] == '\\' && path_buffer[i + 1] == '\\');
		else {
			buff[j++] = path_buffer[i];
			buff[j] = '\0';
		}
	}
	fs_read_sdata(buff);
#endif
	return (0);
}
void
config_out(FILE * out_fp)
{
	fprintf(out_fp, ";=============================\n");
	fprintf(out_fp, "; porolith configuration file\n");
	fprintf(out_fp, ";=============================\n");
	fprintf(out_fp, ";\n");
	fprintf(out_fp, "	BLOCKNAME	=	%s\n", SG_BLOCK_NAME);
	fprintf(out_fp, "	BLOCKNUM	=	%d\n", SG_BLOCK_NUM);
	fprintf(out_fp, "	CHRNAME		=	%s\n", SG_CHAR_NAME);
	fprintf(out_fp, "	PANICNAME	=	%s\n", SG_PANIC_NAME);
	fprintf(out_fp, "	FONTNAME	=	%s\n", SG_FONT_NAME);
	fprintf(out_fp, "	EFSNAME		=	%s\n", SG_EFS_NAME);
	fprintf(out_fp, "	BGM		=	%d\n", SG_BGM);
	fprintf(out_fp, "	SPEED		=	%d\n", SG_SPEED);
	fprintf(out_fp, "	SPEEDMIN	=	%d\n", SG_SPEED_MIN);
	fprintf(out_fp, "	CLEARBLOCK	=	%d\n", SG_BLOCK);
	fprintf(out_fp, "	LEVELPT		=	%d\n", SG_LEVEL_PT * 10);
	fprintf(out_fp, "	DROP		=	%d\n", SG_BLOCK_BREAK);
	fprintf(out_fp, "	NEXT		=	%d\n", SG_NEXT);
	fprintf(out_fp, "	SPECIAL		=	%d\n", SG_SPECIAL);
	fprintf(out_fp, "	SPECIALPT	=	%d\n", SG_SPECIAL_POINT * 10);
	fprintf(out_fp, "	SPECIALHIGHT	=	%d\n", SG_SPECIAL_HIGHT);
	fprintf(out_fp, "	SPECIALRATE	=	%d\n", SG_HETA_FLAG);
	fprintf(out_fp, "	NETWORK		=	%d\n", SG_NETWORK);
	fprintf(out_fp, "	ANSICHAR	=	\"%s\"\n", SG_ANSI_CHAR);
}
void
file_out(void)
{
	int             c;
#ifdef	ANSI
	printf("%s", ESC_CLS);
#else
	cls;
#endif
	locate(0, 0);
	color(TEX_WHITE);
	puts("＊＊＊　ぽろりす☆の現在の環境をファイルにセーブします　＊＊＊");
	puts(" ");
	printf("Configuration file name:%s\n\n", SG_CONFIG);
	if (!strcmp(SG_CONFIG, "porolith.cfg"))
		printf("このファイルはマスターファイルですけど・・\n");
	printf("上書きしてよろしいですか？ (Y/N) :");
	c = getch();
	if (c == 'y' || c == 'Y') {
		fp = fpopen(SG_CONFIG, "w+");
		config_out(fp);
		fclose(fp);
	}
}



#ifdef	BGMFM
void
fs_read_sdata(char *name)
{
	FILE           *fp;
	int             a, i;
	char            p[128];
	int             w[128];
	int             b0 = 1;
	int             b1 = 0;
	fp = fopen(name, "rt");
	while (fgets(p, 127, fp) != NULL) {
		if (*p != '\n' && sscanf(p, "%u", &a) != 0) {
			w[b1] = a;
			b1++;
			if (a == 0) {
				bgm[b0] = malloc(b1 * 2);
				for (i = 0; i < b1; i++)
					bgm[b0][i] = w[i];
				b0++;
				b1 = 0;
			}
		}
	}
	fclose(fp);
}
#endif
