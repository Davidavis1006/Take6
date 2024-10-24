#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "take6.h"

/* 遊戲選單：設定模式、玩家人數、難度 */
void menu(int32_t *mode, int32_t *playerNum, int32_t *level)
{
	while (1)
	{
		int32_t choice=0;
		printf( MAG_BOLD"\nPlease select the game mode:\n"RESET );
		printf( CYAN_BOLD"--------------------------------------------------------------------\n"RESET );
		printf( CYAN_BOLD"(1) Single player & single computer agent    --> recommend to novice\n"RESET ); // 雙人
		printf( CYAN_BOLD"(2) Single player & multiple computer agents --> recommend to expert\n"RESET ); // 多人
		printf( CYAN_BOLD"--------------------------------------------------------------------\n"RESET );	
		printf( MAG_BOLD"Enter your choice...>"RESET );
		scanf( "%d", &choice );

		CLEAR_INPUT_BUFFER;

		if (!(choice==1 || choice==2))
		{
			printf( RED_BOLD"Invalid choice! Please input again!\n"RESET );
		}
		else if (choice==1)
		{
			*mode=1;
			*playerNum=2;

			while (1)
			{
				int32_t gameLevel=0;
				printf( MAG_BOLD"\nPlease select the game level:\n"RESET );
				printf( CYAN_BOLD"----------\n"RESET );
				printf( CYAN_BOLD"(1) Easy\n"RESET );
				printf( CYAN_BOLD"(2) Normal\n"RESET );
				printf( CYAN_BOLD"(3) Hard\n"RESET );
				printf( CYAN_BOLD"----------\n"RESET );
				printf( MAG_BOLD"Your choice...>"RESET );
				scanf( "%d", &gameLevel );

				CLEAR_INPUT_BUFFER;

				if (!(gameLevel==1 || gameLevel==2 || gameLevel==3))
				{
					printf( RED_BOLD"Invalid choice! Please input again!\n"RESET );
				}
				else
				{
					*level=gameLevel;
					break;
				}
			}

			break;
		}
		else // choice==2
		{
			*mode=2;

			while (1)
			{
				int32_t cpu=0;
				printf( MAG_BOLD"\nPlease enter the number of computer agents (2~9)...>"RESET );
				scanf( "%d", &cpu );

				CLEAR_INPUT_BUFFER;

				if (!(cpu>=2 && cpu<=9))
				{
					printf( RED_BOLD"Invalid number of players! Please input again!\n"RESET );
				}
				else
				{
					*playerNum=cpu+1; // 加上自己
					break;
				}
			}

			while (1)
			{
				int32_t gameLevel=0;
				printf( MAG_BOLD"\nPlease select the game level:\n"RESET );
				printf( CYAN_BOLD"----------\n"RESET );
				printf( CYAN_BOLD"(1) Easy\n"RESET );
				printf( CYAN_BOLD"(2) Medium\n"RESET );
				printf( CYAN_BOLD"(3) Hard\n"RESET );
				printf( CYAN_BOLD"----------\n"RESET );
				printf( MAG_BOLD"Your choice...>"RESET );
				scanf( "%d", &gameLevel );

				CLEAR_INPUT_BUFFER;

				if (!(gameLevel==1 || gameLevel==2 || gameLevel==3))
				{
					printf( RED_BOLD"Invalid choice! Please input again!\n"RESET );
				}
				else
				{
					*level=gameLevel;
					break;
				}
			}

			break;
		}
	}

	//printf( "Mode: %d, Player number: %d, Level: %d\n", *mode, *playerNum, *level );
}

/* 初始化牌組：編號、牛頭數、是否出過 */
void setup(sCard deck[])
{
	for (size_t i=0; i<DECK_SIZE; i++)
	{
		deck[i].cardNum=i+1; // 設定編號
		deck[i].flag=0; // 皆尚未出過該張牌
		deck[i].owner=0; // 沒人出的

		// 設定牛頭數
		if (deck[i].cardNum==55) // 1 張
		{
			deck[i].cattleHead=7;
		}
		else if (deck[i].cardNum%11==0) // 8 張
		{
			deck[i].cattleHead=5;
		}
		else if (deck[i].cardNum%10==0) // 10 張
		{
			deck[i].cattleHead=3;
		}
		else if (deck[i].cardNum%5==0) // 9 張
		{
			deck[i].cattleHead=2;
		}
		else // 76 張
		{
			deck[i].cattleHead=1;
		}
	}
}

/* 洗牌、發牌 */
void shuffleDeal(sCard deck[], sCard playerCard[][MAX_CARD], int32_t playerNum)
{
	srand(time(0));

	// 洗牌
	// 以兩兩交換牌 10000 次當作洗牌
	for (int32_t i=1; i<=10000; i++)
	{
		size_t x=rand()%104; // 欲交換的牌1
		size_t y=rand()%104; // 欲交換的牌2

		// swap
		sCard temp={};
		memcpy(&temp, &deck[x], sizeof(sCard));
		memcpy(&deck[x], &deck[y], sizeof(sCard));
		memcpy(&deck[y], &temp, sizeof(sCard));
	}

	// 發牌
	for (size_t i=0; i<playerNum; i++) // playerNum 個玩家
	{
		for (size_t j=0; j<MAX_CARD; j++) // 每人發 10 張
		{
			deck[i*10+j].owner=i+1; // 更新擁有者
			memcpy(&playerCard[i][j], &deck[i*10+j], sizeof(sCard));
		}
	}
}

/* 顯示牌組內容（編號、牛頭數、是否出過）*/
void printDeck(const sCard deck[])
{
	const char *status[]={"not used", "used"};

	printf( CYAN_BOLD"\n---------------------------------------------------------\n"RESET );
	printf( CYAN_BOLD"After initialized/shuffled, the contents of the deck are:\n"RESET );
	printf( CYAN_BOLD"---------------------------------------------------------\n"RESET );
	for (size_t i=0; i<DECK_SIZE; i++)
	{
		printf( "Card number: %hhu, Cattle heads: %hhu, Status: %s, Owner: %hhu\n", deck[i].cardNum, deck[i].cattleHead, status[deck[i].flag], deck[i].owner );
	}
}

/* 顯示每位玩家的手牌（編號、牛頭數、是否出過）*/
void printPlayerCard(const sCard playerCard[][MAX_CARD], const int32_t playerNum)
{
	const char *status[]={"not used", "used"};

	printf( CYAN_BOLD"\n-------------------------------------------\n"RESET );
	printf( CYAN_BOLD"After dealed, the cards of each player are:\n"RESET );
	printf( CYAN_BOLD"-------------------------------------------\n"RESET );
	for (size_t i=0; i<playerNum; i++)
	{
		printf( YEL_BOLD"(Player %lu):\n"RESET, i+1 );
		for (size_t j=0; j<MAX_CARD; j++)
		{
			printf( "Card number: %hhu, Cattle heads: %hhu, Status: %s. Owner: %hhu\n", playerCard[i][j].cardNum, playerCard[i][j].cattleHead, status[playerCard[i][j].flag], playerCard[i][j].owner );
		}
		puts("-----");
	}
}

/* 顯示目前桌面上牌堆（編號、牛頭數） */
void printTable(const sCard table[][TABLE_SIZE])
{
	printf( CYAN_BOLD"\n------------------------\n"RESET );
	printf( CYAN_BOLD"Current table situation:\n"RESET );
	printf( CYAN_BOLD"------------------------\n"RESET );
	for (size_t i=0; i<4; i++)
	{	
		printf( YEL_BOLD"(Row %lu) "RESET, i+1 );
		for (size_t j=0; j<TABLE_SIZE; j++)
		{
			if (table[i][j].cardNum==0)
			{
				break;
			}
			else
			{
				printf( MAG_BOLD"%3hhu(%hhu) "RESET, table[i][j].cardNum, table[i][j].cattleHead );
			}
		}
		putchar('\n');
	}
}

/* 於螢幕上顯示玩家手牌 */
void showMyCard(const sCard playerCard[][MAX_CARD])
{
	printf( CYAN_BOLD"\n---------------\n"RESET );
	printf( CYAN_BOLD"Your cards are:\n"RESET );
	printf( CYAN_BOLD"---------------\n"RESET );
	printf( YEL_BOLD"(Card number) "RESET );
	for (size_t i=0; i<MAX_CARD; i++)
	{
		if (!playerCard[0][i].flag) // 尚未出過
		{
			printf( MAG_BOLD"%3hhu "RESET, playerCard[0][i].cardNum );
		}
	}
	putchar('\n');
	printf( YEL_BOLD"(Cattle head) "RESET );
	for (size_t i=0; i<MAX_CARD; i++)
	{
		if (!playerCard[0][i].flag) // 尚未出過
		{
			printf( MAG_BOLD"%3hhu "RESET, playerCard[0][i].cattleHead );
		}
	}
	putchar('\n');
}

/* 於螢幕上顯示電腦手牌 */
void showCpuCard(const sCard playerCard[][MAX_CARD], const int32_t playerNum)
{
	for (size_t i=1; i<playerNum; i++)
	{
		printf( CYAN_BOLD"\n--------------------\n"RESET );
		printf( CYAN_BOLD"CPU %lu's cards are:\n"RESET, i );
		printf( CYAN_BOLD"--------------------\n"RESET );
		printf( YEL_BOLD"(Card number) "RESET );
		for (size_t j=0; j<MAX_CARD; j++)
		{
			if (!playerCard[i][j].flag) // 尚未出過
			{
				printf( MAG_BOLD"%3hhu "RESET, playerCard[i][j].cardNum );
			}
		}
		putchar('\n');
		printf( YEL_BOLD"(Cattle head) "RESET );
		for (size_t j=0; j<MAX_CARD; j++)
		{
			if (!playerCard[i][j].flag) // 尚未出過
			{
				printf( MAG_BOLD"%3hhu "RESET, playerCard[i][j].cattleHead );
			}
		}
		putchar('\n');
	}
}

/* 於螢幕上顯示出牌區牌堆 */
void printOut(const sCard out[], const int32_t playerNum, const int32_t round)
{
	printf( CYAN_BOLD"\n-----------------\n"RESET );
	printf( CYAN_BOLD"Output cards are:\n"RESET );
	printf( CYAN_BOLD"-----------------\n"RESET );
	for (size_t i=0; i<playerNum-round+1; i++)
	{
		printf( GRN_BOLD"%hhu(%hhu) "RESET, out[i].cardNum, out[i].cattleHead );
	}
	putchar('\n');
}

/* 整體遊戲進行區 */
void game(sCard deck[], sCard playerCard[][MAX_CARD], int32_t playerNum, int32_t level)
{
	uint8_t point[playerNum]; // 分數紀錄表（初始為0）
	for (size_t i=0; i<playerNum; i++)
	{
		point[i]=0;
	}

	// 將玩家手牌排序（大到小）
	for (size_t i=0; i<playerNum; i++)
	{
		qsort(playerCard[i], MAX_CARD, sizeof(sCard), compare);
	}
	//printPlayerCard(playerCard, playerNum);

	// 初始化桌上牌堆（翻開四張牌當起始牌）
	sCard table[4][TABLE_SIZE]; // 存放桌上的牌堆
	for (size_t i=0; i<4; i++)
	{
		for (size_t j=0; j<TABLE_SIZE; j++)
		{
			table[i][j].cardNum=table[i][j].cattleHead=table[i][j].flag=table[i][j].owner=table[i][j].index=0;
		}
	}
	deck[10*playerNum].flag=deck[10*playerNum+1].flag=deck[10*playerNum+2].flag=deck[10*playerNum+3].flag=1;
	memcpy(&table[0][0], &deck[10*playerNum], sizeof(sCard));
	memcpy(&table[1][0], &deck[10*playerNum+1], sizeof(sCard));
	memcpy(&table[2][0], &deck[10*playerNum+2], sizeof(sCard));
	memcpy(&table[3][0], &deck[10*playerNum+3], sizeof(sCard));

	/* 重複直到有玩家的牛頭數>=66 */
	for (int32_t cnt=1; cnt<=10; cnt++)
	{
		printPoint(point, playerNum);
		printTable(table);

		// 自己出牌
		sCard out[playerNum]; // 出牌區
		showMyCard(playerCard);
		int32_t card=0, returnIndex=0;
		while (1)
		{
			printf( MAG_BOLD"Please play a card...>"RESET );
			scanf( "%d", &card );

			CLEAR_INPUT_BUFFER;

			if (checkValidCard(playerCard, 0, card, &returnIndex)) // 檢查有沒有該張牌
			{
				playerCard[0][returnIndex].flag=1; // 更新 flag
				memcpy(&out[0], &playerCard[0][returnIndex], sizeof(sCard)); // 放置出牌區
				
				break;
			}

			printf( RED_BOLD"Error! No such card! Please input again!\n"RESET );
		}

		// 電腦出牌
		//showCpuCard(playerCard, playerNum);
		cpuTurnEasy(playerCard, table, out, playerNum);
		qsort(out, playerNum, sizeof(sCard), compare);
 
		// 將出牌區由小到大放至桌上牌堆
		int32_t round=1;
		for (size_t i=0; i<playerNum; i++) // 重複 playerNum 次
		{
			printPoint(point, playerNum);
			printOut(out, playerNum, round++);			
			printTable(table);
			addCardToTable(table, out, point, playerNum);

			printf( "\n\n-----\n\n" );

			sleep(1);
		}

		printPoint(point, playerNum);
		printOut(out, playerNum, round++);			
		printTable(table);

		PAUSE;
		system( "clear" );
	}

	endGame(point, playerNum); // 遊戲結束
}

/* 檢查  玩家輸入的手牌是否存在 */
/* 若存在，紀錄該張牌的索引值 */
bool checkValidCard(const sCard playerCard[][MAX_CARD], const size_t index, const int32_t card, int32_t *returnIndex)
{
	for (size_t i=0; i<MAX_CARD; i++)
	{
		if (playerCard[index][i].cardNum==card && playerCard[index][i].flag==0)
		{
			*returnIndex=i; // 紀錄 index
			return true;
		}
	}

	return false;
}

/* 設定每回合電腦的出牌（Easy 等級）*/
void cpuTurnEasy(sCard playerCard[][MAX_CARD], sCard table[][TABLE_SIZE], sCard out[], int32_t playerNum)
{
	int32_t cpuSelect[4][3]; // 存手排中分別和四列最靠近的牌（編號、索引值、差）

	for (size_t i=1; i<playerNum; i++) // playerNum-1 位電腦
	{
		for (size_t j=0; j<4; j++) // 4個 row 
		{
			// 找每個 row 排頭
			size_t lastIndex=0;
			for (size_t k=1; k<6; k++)
			{
				if (table[j][k].cardNum==0)
				{
					//printf( "%hhu\n", table[j][k-1].cardNum );
					lastIndex=k-1;
					break;
				}
			}

			// 從手牌最後找最接近排頭的牌（前提是尚未出過）
			int32_t dif=0, card=0, min=104;
			size_t idx=0;
			for (int32_t k=MAX_CARD-1; k>=0; k--)
			{
				if (playerCard[i][k].flag==0)
				{
					dif=playerCard[i][k].cardNum-table[j][lastIndex].cardNum;
					if (dif<min && dif>0)
					{
						min=dif;
						card=playerCard[i][k].cardNum;
						idx=k;
					}
				}
			}

			cpuSelect[j][0]=playerCard[i][idx].cardNum;
			cpuSelect[j][1]=idx;
			cpuSelect[j][2]=min;
		}

		/*printf( "\n-------------------------------------------\n" );
		printf( "Minimum difference of each row & hand card:\n" );
		printf( "-------------------------------------------\n" );
		for (size_t j=0; j<4; j++)
		{
			printf( "(Row %lu) ", j );
			printf( "Card number: %3d, Index: %3d, Dif: %3d\n", cpuSelect[j][0], cpuSelect[j][1], cpuSelect[j][2] );
		}*/

		// 選差距最小的出牌，放到區牌區
		if (cpuSelect[0][2]==104 && cpuSelect[1][2]==104 && cpuSelect[2][2]==104 && cpuSelect[3][2]==104) // 若沒有牌出，出手牌中最大的
		{
			int32_t Max=0;
			size_t selectIdx=0;
			for (size_t j=0; j<MAX_CARD; j++)
			{
				if (playerCard[i][j].cardNum>Max && playerCard[i][j].flag==0) // 尚未出過
				{
					Max=playerCard[i][j].cardNum;
					selectIdx=j;
				}
			}

			playerCard[i][selectIdx].flag=1; // 更新為已出過
			memcpy(&out[i], &playerCard[i][selectIdx], sizeof(sCard));	
		}
		else // 選差距最小的
		{
			int32_t Min=104;
			size_t selectIdx=0;
			for (size_t j=0; j<4; j++)
			{
				if (cpuSelect[j][2]<Min)
				{
					Min=cpuSelect[j][2];
					selectIdx=j;
				}
			}
			//printf( "Select card %d\n", cpuSelect[selectIdx][0] );
		
			playerCard[i][cpuSelect[selectIdx][1]].flag=1; // 更新為已出過
			memcpy(&out[i], &playerCard[i][cpuSelect[selectIdx][1]], sizeof(sCard));	
		}
	}
}

/* 將出牌區牌堆從小到大放置到桌面 */
void addCardToTable(sCard table[][TABLE_SIZE], sCard out[], uint8_t point[], int32_t playerNum)
{
	// 先找每個 row 的排頭，記錄下來
	sCard head[4]={};
	for (size_t i=0; i<4; i++) 
	{
		for (size_t j=1; j<6; j++)
		{
			if (table[i][j].cardNum==0)
			{
				memcpy(&head[i], &table[i][j-1], sizeof(sCard));
				head[i].index=j-1;
				//printf( "%hhu %hhu\n", head[i].cardNum, head[i].index );
				break;
			}
		}
	}

	// 情況一：不安全，比排頭都小，選一列吃掉
	if (!checkSafe(out[0], head))
	{
		size_t choice=0;

		if (out[0].owner==1) // 玩家自己出的
		{
			while (1)
			{
				printf( RED_BOLD"Not safe! You must choose a row to eat!\n"RESET );
				printf( MAG_BOLD"Which row do you want to choose...>"RESET );
				scanf( "%lu", &choice );

				CLEAR_INPUT_BUFFER;

				if (!(choice>=1 && choice<=4))
				{
					printf( RED_BOLD"Invalid row number! Please input again!\n"RESET );
				}
				else
				{
					break;					
				}
			}

			uint8_t totalHead=0;
			for (size_t i=0; i<=head[choice-1].index; i++)
			{
				totalHead+=table[choice-1][i].cattleHead;
			}
			printf( "Total cattle heads of row %lu: %hhu\n", choice, totalHead );

			// 更新得分
			point[0]+=totalHead;

			// 更新桌面牌堆（table）
			clearTableRow(table, choice-1);
			memcpy(&table[choice-1][0], &out[0], sizeof(sCard)); // 將該張牌放到被吃掉列的排頭

			// 更新排頭（head）
			//memcpy(&head[choice-1], &out[0], sizeof(sCard));

			// 更新出牌區（out）
			moveBackwardOut(out, playerNum);
		}
		else // 電腦出的，直接幫它選（選牛頭數和最小的 row）
		{
			uint8_t leastRowSum=0;
			choice=findLeastRow(table, head, &leastRowSum);

			// 更新得分
			point[out[0].owner-1]+=leastRowSum;

			// 更新桌面牌堆（table）
			clearTableRow(table, choice);
			memcpy(&table[choice][0], &out[0], sizeof(sCard)); // 將該張牌放到被吃掉列的排頭

			// 更新排頭（head）
			//memcpy(&head[choice], &out[0], sizeof(sCard));

			// 更新出牌區（out）
			moveBackwardOut(out, playerNum);
		}
	}
	else
	{
		size_t choice=findClosestRow(head, out[0]); // 找該牌和哪個 row 的排頭最接近

		// 情況二：不安全，該列已有5張牌，必須吃掉該列
		if (head[choice].index==4)
		{
			uint8_t totalHead=0;
			for (size_t i=0; i<=4; i++)
			{
				totalHead+=table[choice][i].cattleHead;
			}
			printf( "Total cattle heads of row %lu: %hhu\n", choice+1, totalHead );

			// 更新得分
			point[out[0].owner-1]+=totalHead;

			// 更新桌面牌堆（table）
			clearTableRow(table, choice);
			memcpy(&table[choice][0], &out[0], sizeof(sCard));

			// 更新排頭（head）
			//memcpy(&head[choice], &out[0], sizeof(sCard));

			// 更新出牌區（out）
			moveBackwardOut(out, playerNum);
		}
		// 情況三：安全，將該牌加入排頭
		else
		{
			// 更新桌面牌堆（table）
			memcpy(&table[choice][head[choice].index+1], &out[0], sizeof(sCard));

			// 更新排頭（head）
			//memcpy(&head[choice], &out[0], sizeof(sCard));

			// 更新出牌區（out）
			moveBackwardOut(out, playerNum);
		}
	}
}

/* 檢查該張出牌區的牌有沒有比排頭都小 */
bool checkSafe(const sCard target, const sCard head[])
{
	if (target.cardNum<head[0].cardNum && target.cardNum<head[1].cardNum && target.cardNum<head[2].cardNum && target.cardNum<head[3].cardNum)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/* 找牛頭數和最小的 row，並回傳該 row 索引值 */
size_t findLeastRow(const sCard table[][TABLE_SIZE], const sCard head[], uint8_t *leastRowSum)
{
	uint8_t min=100; // 紀錄該 row 牛頭數和
	size_t rowIdx=0; // 紀錄哪一個 row

	for (size_t i=0; i<4; i++) // 4個 row
	{
		uint8_t sum=0;
		for (size_t j=0; j<=head[i].index; j++) // 加總到該 row 的排頭
		{
			sum+=table[i][j].cattleHead;
		}
		printf( "Total cattle heads of row %lu: %hhu\n", i+1, sum );

		if (sum<min)
		{
			min=sum;
			rowIdx=i;
		}
	}

	*leastRowSum=min;
	return rowIdx;
}

/* 清除桌上牌堆的指定 row */
void clearTableRow(sCard table[][TABLE_SIZE], size_t row)
{
	for (size_t i=0; i<TABLE_SIZE; i++)
	{
		table[row][i].cardNum=table[row][i].cattleHead=table[row][i].flag=table[row][i].owner=table[row][i].index=0;
	}
}

/* 更新出牌區的內容（全部往後移動一格） */
void moveBackwardOut(sCard out[], int32_t playerNum)
{
	for (size_t i=1; i<playerNum; i++)
	{
		memcpy(&out[i-1], &out[i], sizeof(sCard));
	}
}

/* 找出牌區的牌和哪個 row 的排頭最接近，並回傳該 row 索引值 */
size_t findClosestRow(const sCard head[], const sCard target)
{
	uint8_t min=104; // 紀錄與排頭的差
	size_t rowIdx=0; // 紀錄哪一個 row

	for (size_t i=0; i<4; i++) // 只需要分別比對4個 row 的排頭即可
	{
		uint8_t dif=target.cardNum-head[i].cardNum;
		if (dif<min && dif>0)
		{
			min=dif;
			rowIdx=i;
		}
	}

	return rowIdx;
}

/* 於螢幕上顯示每位玩家目前的分數 */
void printPoint(const uint8_t point[], const int32_t playerNum)
{
	printf( CYAN_BOLD"\n----------------------\n"RESET );
	printf( CYAN_BOLD"Points of each player:\n"RESET );
	printf( CYAN_BOLD"----------------------\n"RESET );
	printf( YEL_BOLD"(Player) "RESET );
	for (size_t i=0; i<playerNum; i++)
	{
		printf( MAG_BOLD"%3lu "RESET, i+1 );
	}
	putchar('\n');
	printf( YEL_BOLD"(Points) "RESET );
	for (size_t i=0; i<playerNum; i++)
	{
		printf( MAG_BOLD"%3hhu "RESET, point[i] );
	}
	putchar('\n');
}

/* 遊戲結束，宣告誰是贏家 */
void endGame(const uint8_t point[], const int32_t playerNum)
{
	uint8_t min=100; // 紀錄最少牛頭數
	size_t playerIdx=0; // 紀錄贏家

	for (size_t i=0; i<playerNum; i++)
	{
		uint8_t p=point[i];
		if (p<min)
		{
			min=p;
			playerIdx=i;
		}
	}

	printf( YEL_BOLD"\n==========================================\n"RESET );
	printf( YEL_BOLD"The game is over!\n"RESET );
	if (playerIdx==0) // 玩家贏
	{
		printf( CYAN_BOLD"Congratulations! You are the winner:)\n"RESET );
	}
	else // 電腦贏
	{
		printf( GRN_BOLD"So sad! The computer(player %lu) wins!\n"RESET, playerIdx+1 );
	}
	printf( YEL_BOLD"==========================================\n"RESET );
}

int32_t compare(const void *a, const void *b)
{
	return ((sCard *)a)->cardNum-((sCard *)b)->cardNum;
}

/* 顯示遊戲規則 */
void printGameRules(void)
{
	
}