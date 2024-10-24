#ifndef _TAKE6_H_
#define _TAKE6_H_

#include <stdint.h>
#include <stdbool.h>

#define DECK_SIZE 104
#define MAX_CARD 10
#define TABLE_SIZE 6
#define CLEAR_INPUT_BUFFER do { \
						int32_t c=0; \
						while ((c=fgetc(stdin))!='\n' && c!=EOF) {} \
						} while (0)
// Linux 系統無 Windows 中的 system( "pause" )，在此用define處理
#define PAUSE printf( "Press Enter key to continue..." ); fgetc(stdin)
// 設定顏色
#define RED_BOLD "\x1b[;31;1m"
#define GRN_BOLD "\x1b[;32;1m"
#define YEL_BOLD "\x1b[;33;1m"
#define BLU_BOLD "\x1b[;34;1m"
#define MAG_BOLD "\x1b[;35;1m"
#define CYAN_BOLD "\x1b[;36;1m"
#define RESET "\x1b[0;m"

typedef struct _sCard_
{
	uint8_t cardNum; // 編號
	uint8_t cattleHead; // 牛頭數
	uint8_t flag; // 紀錄有沒有出過此張牌
	uint8_t owner; // 紀錄誰出的
	uint8_t index; // 紀錄在 table[] 中的索引值（僅 addCardToTable 有用到）
}sCard;

void menu(int32_t *, int32_t *, int32_t *);
void setup(sCard deck[]);
void shuffleDeal(sCard deck[], sCard playerCard[][MAX_CARD], int32_t);
void printDeck(const sCard deck[]);
void printPlayerCard(const sCard playerCard[][MAX_CARD], const int32_t);
void printTable(const sCard table[][TABLE_SIZE]);
void showMyCard(const sCard playerCard[][MAX_CARD]);
void showCpuCard(const sCard playerCard[][MAX_CARD], const int32_t);
void printOut(const sCard out[], const int32_t, const int32_t);
void game(sCard deck[], sCard playerCard[][MAX_CARD], int32_t, int32_t);
bool checkValidCard(const sCard playerCard[][MAX_CARD], const size_t, const int32_t, int32_t *);
void cpuTurnEasy(sCard playerCard[][MAX_CARD], sCard table[][TABLE_SIZE], sCard out[], int32_t);
void addCardToTable(sCard table[][TABLE_SIZE], sCard out[], uint8_t point[], int32_t);
bool checkSafe(const sCard, const sCard head[]);
size_t findLeastRow(const sCard table[][TABLE_SIZE], const sCard head[], uint8_t *);
void clearTableRow(sCard table[][TABLE_SIZE], size_t);
void moveBackwardOut(sCard out[], int32_t);
size_t findClosestRow(const sCard head[], const sCard);
void printPoint(const uint8_t point[], const int32_t);
void endGame(const uint8_t point[], const int32_t);
int32_t compare(const void *, const void *);
void printGameRules(void);

#endif