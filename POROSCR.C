/*
		Action Puzzle Game "POROLITH"

			PC-9801,286
			FMR50/60/70
			Dynabook
			and MSDOS Machine

	Copyright(C)1990 by Ko.Mo.Mo. Allrights Reserved.
*/

#include	<stdio.h>
#include	<string.h>

/*	ハイスコアデータの格納用型定義
*/
struct high_score_t {
	int             score;
	char            name[13];
	char            date[9];
	char            time[9];
	char            fname[80];
};

/*	意味もなく共通データ宣言
*/
struct high_score_t rank[128];
struct high_score_t rankwk;
struct high_score_t rankok[10];
FILE           *fp;

/*	実際にはmainしか関数がない
*/
int
main(void)
{
	int             i, j;
	char            buf[80], fname[80];
	int             num;
	int             err;

	num = 0;
	printf("\x1b[2Jporolith score file compare manager version 1.10\n\n");
	for (;;) {
		printf("input porolith score file (null enter=end):");
		fgets(buf, sizeof(buf), stdin);
		/* リターンの空打ちがあるまでループする	*/
		if (buf[0] == '\n')
			break;
		buf[strlen(buf) - 1] = 0x00;
		strcpy(fname, buf);
		err = 0;
		/* もし同一ファイルの指定だったらエラー	*/
		for (i = 0; i < num; i++) {
			if (!strcmp(buf, rank[i].fname))
				err = 1;
		}
		/* ファイルが不在の場合もエラー	*/
		if (NULL == (fp = fopen(buf, "r")) || err == 1) {
			printf("can't open '%s'\n", buf);
		} else {
			for (i = 0; i < 10; i++) {
				fgets(buf, sizeof(buf), fp);
				/* ファイルは本当にスコアファイルか？	*/
				if (buf[5] != '0' || buf[6] != ' ' || buf[19] != ' ' || buf[28] != ' ') {
					printf("illigal file format of score file !\n");
					fclose(fp);
					return (1);
				}
				buf[5] = 0x00;
				buf[19] = 0x00;
				buf[28] = 0x00;
				buf[37] = 0x00;
				/* 読み込みリストに追加する	*/
				rank[num].score = atoi(&buf[0]);
				strcpy(rank[num].name, &buf[7]);
				strcpy(rank[num].date, &buf[20]);
				strcpy(rank[num].time, &buf[29]);
				strcpy(rank[num].fname, fname);
				num++;
			}
			fclose(fp);
		}
	}
	if (num == 0)
		return (0);
	/* スコアの高い順にソートする	*/
	for (i = 0; i < num; i++) {
		for (j = i; j < num; j++) {
			if (rank[i].score < rank[j].score) {
				rankwk = rank[i];
				rank[i] = rank[j];
				rank[j] = rankwk;
			}
		}
	}
	j = 1;
	rankwk = rank[0];
	rankok[0] = rank[0];
	/* 上位１０名の抽出	*/
	for (i = 1; i < num; i++) {
		if (rankwk.score == rank[i].score && !strcmp(rankwk.name, rank[i].name) && !strcmp(rankwk.date, rank[i].date) && !strcmp(rankwk.time, rank[i].time) && strcmp(rankwk.fname, rank[i].fname)) {
			;
		} else {
			rankwk = rank[i];
			rankok[j] = rank[i];
			j++;
			if (j == 10)
				break;
		}
	}
	for (i = 0; i < 10; i++) {
		printf("no:%2d sc:%05d0 name:%12s dt:%8s tm:%8s fname:%s\n", i + 1, rankok[i].score, rankok[i].name, rankok[i].date, rankok[i].time, rankok[i].fname);
	}
	for (;;) {
		/* ファイルに書き出す	*/
		printf("\nwrite file name? (null return=exit):");
		fgets(buf, sizeof(buf), stdin);
		if (buf[0] == '\n')
			return (0);
		buf[strlen(buf) - 1] = 0x00;
		if (NULL == (fp = fopen(buf, "w+"))) {
			printf("why?\n");
		} else {
			for (i = 0; i < 10; i++) {
				fprintf(fp, "%05d0 %12s %8s %8s\n", rankok[i].score, rankok[i].name, rankok[i].date, rankok[i].time);
			}
			fclose(fp);
			return (0);
		}
	}
}
