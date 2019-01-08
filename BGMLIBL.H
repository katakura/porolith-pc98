#define		ON				1	/* フラグ・返値 */
#define		OFF				0
#define		TRUE			1
#define		FALSE			0
#define		FINISH			1
#define		NOTFIN			0
#define		SEND			0
#define		VEND			0
#define		BGM_STAT_PLAY	1
#define		BGM_STAT_MUTE	0
#define		BGM_STAT_REPT	1
#define		BGM_STAT_1TIM	0
#define		BGM_STAT_ON		1
#define		BGM_STAT_OFF	0
#define		BGM_MES_ON		1
#define		BGM_MES_OFF		0
#define		BGM_MUSIC		1
#define		BGM_SOUND		2
#define		BGM_VOICE		4
#define		TITLE			0/
#define		LOADING			1
#define		ENDLOAD			2
#define		BGM_COMPLETE	0
#define		BGM_FILE_ERR	1
#define		BGM_FORMAT_ERR	2
#define		BGM_OVERFLOW	3
#define		BGM_TOOMANY		4
#define		BGM_NO_MUSIC	5
#define		BGM_NOT_PLAY	6
#define		BGM_NOT_STOP	7
#define		BGM_EXTENT_ERR	8
#define		ES_UNDERLINE	"\x1b[4m"
#define		ES_BLACK		"\x1b[30m"
#define		ES_RED			"\x1b[31m"
#define		ES_GREEN		"\x1b[32m"
#define		ES_YELLOW		"\x1b[33m"
#define		ES_BLUE			"\x1b[34m"
#define		ES_PURPLE		"\x1b[35m"
#define		ES_SKYBLUE		"\x1b[36m"
#define		ES_B_BLACK		"\x1b[5;30m"
#define		ES_B_RED		"\x1b[5;31m"
#define		ES_B_GREEN		"\x1b[5;32m"
#define		ES_B_YELLOW		"\x1b[5;33m"
#define		ES_B_BLUE		"\x1b[5;34m"
#define		ES_B_PURPLE		"\x1b[5;35m"
#define		ES_B_SKYBLUE	"\x1b[5;36m"
#define		ES_QUIT			"\x1b[m"
#define		ES_CSRON		"\x1b[>5l"
#define		ES_CSROFF		"\x1b[>5h"
#define		ES_DEL			"\r\x1b[0K"
#define		BEEP_SW			0x37
#define		BEEP_ON			0x06
#define		BEEP_OFF		0x07
#define		BEEP_CNT		0x3fdb
#define		BEEP_MODE		0x3fdf
#define		INTCTRL			0x00
#define		EOI				0x20
#define		IMR				0x02
#define		TIMERMASK		0x01
#define		TIMER_CNT		0x71
#define		TIMER_SET		0x77
#define		CLOCK_CHK		0x42
#define		PMAX			3
#define		ALL_PART		7
#define		BUFMAX			4096
#define		SBUFMAX			256
#define		MINNOTE			32
#define		MINLCNT			1
#define		DEFTEMPO		120
#define		TEMPOMAX		240
#define		TEMPOMIN		30
#define		DEFOCT			4
#define		DEFLEN			4
#define		DEFLCNT			8
#define		DIVNUM			8
#define		REST			'H'
#define		UNDF			100
#define		MMAX			16
#define		SMAX			16
#define		VMAX			16
#define		TIMER			1
#define		TVAL8ORG		1997U
#define		TVAL10ORG		2458U
#define		TCNTMAX			20
#define		TVAL1ms			( ((inp( 0x42 ) & 0x20) ? TVAL8ORG : TVAL10ORG) & 0xfffe )
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef
struct {
	int             imr;
	uint            tval;
	int             tp;
	int             rflg;
	int             pnum;
	int             pcnt;
	int             fin;
	int             rep;
	int             tcnt;
	int             buflast;
	int             mnum;
	int             mcnt;
	int             track[MMAX];
	int             mask;
	int             mtp[MMAX];
	int             effect;
	int             snum;
	int             scnt;
	int             vnum;
	int             music;
	int             sound;
	int             voice;
}               GLB;

typedef
struct {
	uchar far      *ptr;
	char            mbuf[BUFMAX + 1];
	char            note;
	int             oct;
	int             len;
	int             dflen;
	int             lcnt;
	int             mask;
	int             tnt;
}               PART;

typedef
struct {
	uint far       *sptr;
	uint            sbuf[SBUFMAX + 1];
}               ESOUND;

typedef
struct {
	uchar huge     *datp;
	long            dsiz;
	int             rate;
}               VOICE;

typedef
struct {
	int             music;
	int             sound;
	int             voice;
	int             play;
	int             effect;
	int             repeat;
	int             mnum;
	int             rnum;
	int             tempo;
	int             snum;
	int             fnum;
	int             vnum;
}               BSTAT;

void
bgm_title_put(void);
void
bgm_init(void);
void
bgm_finish(void);
int
bgm_read_alldata(char *);
int
bgm_read_data(char *, int, int);
int
bgm_select_music(int);
int
bgm_start_play(void);
int
bgm_stop_play(void);
int
bgm_cont_play(void);
int
bgm_wait_play(void);
int
bgm_read_sdata(char *);
int
bgm_sound(int);
int
bgm_stop_sound(void);
int
bgm_wait_sound(void);
int
bgm_read_vdata(char *, int);
int
bgm_voice(int);
void
bgm_repeat_on(void);
void
bgm_repeat_off(void);
int
bgm_set_tempo(int);
void
bgm_set_mode(int);
void
bgm_read_status(BSTAT *);
void
bgm_timer_init(void);
void
bgm_timer_finish(void);
void
bgm_pinit(PART *);
void
bgm_buffer_clear(void);
int
bgm_mget(PART *);
void
bgm_bell(PART *);
void
bgm_bell2(uint);
void
bgm_bell_org(void);
void
bgm_bell_mode(int, uint);
int
bgm_play(int, int, int);
int
bgm_effect_sound(void);
void
bgm_manage_play(void);
void
bgm_manage_sound(void);
void
bgm_set_timervalue(void);
void interrupt  far
bgm_timerhook(void);
void            (interrupt far * timerorg) ();
void
bgm_voice_out(uchar huge *, int);

extern uint     note_dat[];
extern int      octdat[];
extern int      lcnt[];
extern GLB      glb;
extern PART     part[];
extern ESOUND   esound[];
extern VOICE    voice[];
extern char    *bgm_message[];
