#define		PC98	1

#include	<stdio.h>
#include	<malloc.h>
#include	"porofont.h"

#define		FNT_NUM		256

struct fnt_t {
	char            fnt[32];
};
struct fnthan_t {
	char            fnt[16];
};

static char far *cgpt_b = (char far *) 0xa8000000;
static char far *cgpt_r = (char far *) 0xb0000000;
static char far *cgpt_g = (char far *) 0xb8000000;
static struct fnt_t far *fnt_pt[16];
/*
 * static	struct	fnthan_t	far	*fnthan_pt[16] ; 
 */
static char     r_mask;
static char     g_mask;
static char     b_mask;
static int      r_flag;
static int      buf_no;

FILE           *
fpopen(fname, mode, envname)
	char           *fname;
	char           *mode;
	char           *envname;
{
	char            path_buffer[80];
	int             i, j;
	char            buff[258];
	FILE           *fp;
	if ((fp = fopen(fname, mode)) != NULL)
		return (fp);
	_searchenv(fname, envname, path_buffer);
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
font_read(char *fname, char *envname)
{
	struct fnt_t far *fnt;
	/*
	 * struct	fnthan_t	far	*fnthan ; 
	 */
	register        i, j, k, l;
	char            r;
	char            buf[80];
	FILE           *fp;
	fp = fpopen(fname, "rb", envname);
	if (!fp)
		return (1);
	fnt_pt[buf_no] = _fmalloc(sizeof(struct fnt_t) * FNT_NUM);
	if (!fnt_pt) {
		fclose(fp);
		return (READ_NG);
	}
	/*
	 * fnthan_pt[buf_no]=_fmalloc(sizeof(struct fnthan_t)*FNT_NUM) ;
	 * if(!fnthan_pt) { fclose(fp) ; return(READ_NG) ; } 
	 */
	fread(buf, 32, 1, fp);
	fread(fnt_pt[buf_no], FNT_NUM * sizeof(struct fnt_t), 1, fp);
	/*
	 * for(l=0;l<=FNT_NUM;l++) { fnthan =(struct
	 * fnthan_t*)&fnthan_pt[buf_no][l] ; fnt =(struct fnt_t*)&fnt_pt[l] ;
	 * for(i=0;i<16;i++) { r = 0; for(k=0;k<2;k++) { for (j=1;j<129;j*=4)
	 * { r = r>>1 ; if ((fnt->fnt[i*2+1-k]&j)!=0)	r|=0x80 ; } }
	 * fnthan->fnt[i] = r; } } 
	 */
	fclose(fp);
	return (READ_OK);
}
void
out_text(int x, int y, char *str)
{
	register        i, j;
	unsigned long   offset;
	struct fnt_t far *fnt;
	for (j = 0; j < strlen(str); j++) {
		if (str[j] >= 'a' && str[j] <= 'z')
			str[j] -= 'a' - 'A';
		fnt = (struct fnt_t *) & fnt_pt[buf_no][str[j]];
		offset = 1280 * y + x * 2;
		for (i = 0; i < 16; i++) {
			if (r_flag == 1) {
				*(cgpt_r + offset + i * 80) = 255 - (fnt->fnt[i * 2]) & g_mask;
				*(cgpt_r + offset + i * 80 + 1) = 255 - (fnt->fnt[i * 2 + 1]) & g_mask;
				*(cgpt_g + offset + i * 80) = 255 - (fnt->fnt[i * 2]) & r_mask;
				*(cgpt_g + offset + i * 80 + 1) = 255 - (fnt->fnt[i * 2 + 1]) & r_mask;
				*(cgpt_b + offset + i * 80) = 255 - (fnt->fnt[i * 2]) & b_mask;
				*(cgpt_b + offset + i * 80 + 1) = 255 - (fnt->fnt[i * 2 + 1]) & b_mask;
			} else {
				*(cgpt_r + offset + i * 80) = (fnt->fnt[i * 2]) & g_mask;
				*(cgpt_r + offset + i * 80 + 1) = (fnt->fnt[i * 2 + 1]) & g_mask;
				*(cgpt_g + offset + i * 80) = (fnt->fnt[i * 2]) & r_mask;
				*(cgpt_g + offset + i * 80 + 1) = (fnt->fnt[i * 2 + 1]) & r_mask;
				*(cgpt_b + offset + i * 80) = (fnt->fnt[i * 2]) & b_mask;
				*(cgpt_b + offset + i * 80 + 1) = (fnt->fnt[i * 2 + 1]) & b_mask;
			}
		}
		x++;
	}
}
/*
 * void out_text_han(int x,int y,char *str) { register		i,j ;
 * unsigned	long	offset ; struct		fnthan_t	far *fnthan ;
 * for(j=0;j<strlen(str);j++) { if(str[j] >= 'a' && str[j] <= 'z') str[j] -=
 * 'a'-'A' ; fnthan = (struct fnthan_t*)&fnthan_pt[buf_no][str[j]] ; offset =
 * 1280*y+x ; for(i=0;i<16;i++) { if(r_flag == 1) { (cgpt_r+offset+i*80) =
 * 0xff-fnthan->fnt[i]&g_mask ; (cgpt_g+offset+i*80) =
 * 0xff-fnthan->fnt[i]&r_mask ; (cgpt_b+offset+i*80) =
 * 0xff-fnthan->fnt[i]&b_mask ; } else { (cgpt_r+offset+i*80) =
 * fnthan->fnt[i]&g_mask ; (cgpt_g+offset+i*80) = fnthan->fnt[i]&r_mask ;
 * (cgpt_b+offset+i*80) = fnthan->fnt[i]&b_mask ; } } x ++ ; } } 
 */
void
mask_set(char rr, char gg, char bb, int rev)
{
	r_mask = rr;
	g_mask = gg;
	b_mask = bb;
	r_flag = rev;
}
void
font_unread(void)
{
	int             i;

	for (i = 0; i < 16; i++) {
		free(fnt_pt[i]);
		/*
		 * free(fnthan_pt[i]) ; 
		 */
	}
}

int
font_select(int no)
{
	if (no < 0 || no > 16)
		return (1);
	else {
		buf_no = no;
		return (0);
	}
}
