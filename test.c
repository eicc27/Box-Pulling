#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#define Y 100
#define X 100
#define debug() MapRenderer()
#define GoUp() Go(0, -1)
#define GoDown() Go(0, 1)
#define GoLeft() Go(-1, 0)
#define GoRight() Go(1, 0)
int map[Y + 1][X + 1] = {};
//int hist_map[5][Y + 1][X + 1] = {};
//记录4次历史记录：使用x键撤回一次，使用/x键选择撤回到第几次——改成结构体：最多10次——改完了！
int availMoveCnt, lastMoveCnt, gameInputCnt;
int player_x, player_y;
int row, column, chests;
char level;
char guiInputKey[3] = {'-', '-'};
//0-main menu, 1-level selection
char game[X] = {'-'};
char histGameInput[2];
//if game0=/,enable superman mode!：字符串中没有非法字符；保证能赢的出来——完成！
int saveFreq = 5;
int guiInputCheck;
float record;
float top[5] = {1000, 1000, 1000, 1000, 1000};
long long gameContinue, gamePause, timeCache;
int histRecordMax = 5;
struct histMapCache
{
    int serial[11];
} histMap[100][100];
//本质就是一个三维数组，但是算法需要优化！——完成！
void Timing()
{
    if (gameInputCnt == 1)
        gameContinue = clock();
    //当不为移动指令（不为超级移动指令）时，暂停。
    if (game[0] != 'w' && game[0] != 'a' && game[0] != 's' && game[0] != 'd')
    {
        gamePause = clock();
        timeCache += gamePause - gameContinue;
    }
    else if (histGameInput[0] != 'w' && histGameInput[0] != 'a' && histGameInput[0] != 's' && histGameInput[0] != 'd')
    {
        gameContinue = clock();
    }
}
void TimingHist()
{
    record = (float)(timeCache) / CLK_TCK;
    FILE *fp;
    if (level == '1')
        fp = fopen(".\\Recorded Times\\1_record_time.txt", "r+");
    else if (level == '2')
        fp = fopen(".\\Recorded Times\\2_record_time.txt", "r+");
    else if (level == '3')
        fp = fopen(".\\Recorded Times\\3_record_time.txt", "r+");
    else if (level == '4')
        fp = fopen(".\\Recorded Times\\4_record_time.txt", "r+");
    else if (level == '5')
        fp = fopen(".\\Recorded Times\\5_record_time.txt", "r+");
    else if (level == '6')
        fp = fopen(".\\Recorded Times\\6_record_time.txt", "r+");
    else if (level == '7')
        fp = fopen(".\\Recorded Times\\7_record_time.txt", "r+");
    else if (level == '8')
        fp = fopen(".\\Recorded Times\\8_record_time.txt", "r+");
    else if (level == '9')
        fp = fopen(".\\Recorded Times\\9_record_time.txt", "r+");
    int a = fscanf(fp, "%f %f %f %f %f", &top[0], &top[1], &top[2], &top[3], &top[4]);
    fclose(fp);
    //printf("%d\n",a);
    if (level == '1')
        fp = fopen(".\\Recorded Times\\1_record_time.txt", "w+");
    else if (level == '2')
        fp = fopen(".\\Recorded Times\\2_record_time.txt", "w+");
    else if (level == '3')
        fp = fopen(".\\Recorded Times\\3_record_time.txt", "w+");
    else if (level == '4')
        fp = fopen(".\\Recorded Times\\4_record_time.txt", "w+");
    else if (level == '5')
        fp = fopen(".\\Recorded Times\\5_record_time.txt", "w+");
    else if (level == '6')
        fp = fopen(".\\Recorded Times\\6_record_time.txt", "w+");
    else if (level == '7')
        fp = fopen(".\\Recorded Times\\7_record_time.txt", "w+");
    else if (level == '8')
        fp = fopen(".\\Recorded Times\\8_record_time.txt", "w+");
    else if (level == '9')
        fp = fopen(".\\Recorded Times\\9_record_time.txt", "w+");
    if (a == -1)
    {
        fprintf(fp, "%.2f ", record);
        top[0] = record;
        printf("Congratulations! You've made a new 1st record!\n");
        printf("Now let's see your top 5 record(s):\n%.2fsecs\n", record);
    }
    else if (a == 1)
    {
        if (record <= top[0])
        {
            fprintf(fp, "%.2f ", record);
            top[0] = record;
            printf("Congratulations! You've made a new 1st record!\n");
            printf("Now let's see your top 5 record(s):\n%.2fsecs\n", record);
        }
        else
        {
            fprintf(fp, "%.2f %.2f", top[0], record);
            top[1] = record;
            printf("Congratulations! You've made a new 2nd record!\n");
            printf("Now let's see your top 5 record(s):\n%.2fsecs\n%.2fsecs\n", top[0], record);
        }
    }
    else if (a == 2)
    {
        if (record <= top[0])
        {
            fprintf(fp, "%.2f %.2f", record, top[1]);
            top[0] = record;
            printf("Congratulations! You've made a new 1st record!\n");
            printf("Now let's see your top 5 record(s):\n%.2fsecs\n%.2fsecs\n", record, top[1]);
        }
        else if (record <= top[1])
        {
            fprintf(fp, "%.2f %.2f", top[0], record);
            top[1] = record;
            printf("Congratulations! You've made a new 2nd record!\n");
            printf("Now let's see your top 5 record(s):\n%.2fsecs\n%.2fsecs\n", top[0], record);
        }
        else
        {
            fprintf(fp, "%.2f %.2f %.2f", top[0], top[1], record);
            top[2] = record;
            printf("Congratulations! You've made a new 3rd record!\n");
            printf("Now let's see your top 5 record(s):\n%.2fsecs\n%.2fsecs\n%.2fsecs\n", top[0], top[1], record);
        }
    }
    else if (a == 3)
    {
        if (record <= top[0])
        {
            fprintf(fp, "%.2f %.2f %.2f", record, top[1], top[2]);
            top[0] = record;
            printf("Congratulations! You've made a new 1st record!\n");
            printf("Now let's see your top 5 record(s):\n%.2fsecs\n%.2fsecs\n%.2fsecs\n", record, top[1], top[2]);
        }
        else if (record <= top[1])
        {
            fprintf(fp, "%.2f %.2f %.2f", top[0], record, top[2]);
            top[1] = record;
            printf("Congratulations! You've made a new 2nd record!\n");
            printf("Now let's see your top 5 record(s):\n%.2fsecs\n%.2fsecs\n%.2fsecs\n", top[0], record, top[2]);
        }
        else if (record <= top[2])
        {
            fprintf(fp, "%.2f %.2f %.2f", top[0], top[1], record);
            top[2] = record;
            printf("Congratulations! You've made a new 3rd record!\n");
            printf("Now let's see your top 5 record(s):\n%.2fsecs\n%.2fsecs\n%.2fsecs\n", top[0], top[1], record);
        }
        else
        {
            fprintf(fp, "%.2f %.2f %.2f %.2f", top[0], top[1], top[2], record);
            top[3] = record;
            printf("Congratulations! You've made a new 4rd record!\n");
            printf("Now let's see your top 5 record(s):\n%.2fsecs\n%.2fsecs\n%.2fsecs\n%.2fsecs\n", top[0], top[1], top[2], record);
        }
    }
    else if (a == 4)
    {
        if (record <= top[0])
        {
            fprintf(fp, "%.2f %.2f %.2f %.2f", record, top[1], top[2], top[3]);
            top[0] = record;
            printf("Congratulations! You've made a new 1st record!\n");
            printf("Now let's see your top 5 record(s):\n%.2fsecs\n%.2fsecs\n%.2fsecs\n%.2fsecs\n", record, top[1], top[2], top[3]);
        }
        else if (record <= top[1])
        {
            fprintf(fp, "%.2f %.2f %.2f %.2f", top[0], record, top[2], top[3]);
            top[1] = record;
            printf("Congratulations! You've made a new 2nd record!\n");
            printf("Now let's see your top 5 record(s):\n%.2fsecs\n%.2fsecs\n%.2fsecs\n%.2fsecs\n", top[0], record, top[2], top[3]);
        }
        else if (record <= top[2])
        {
            fprintf(fp, "%.2f %.2f %.2f %.2f", top[0], top[1], record, top[3]);
            top[2] = record;
            printf("Congratulations! You've made a new 3rd record!\n");
            printf("Now let's see your top 5 record(s):\n%.2fsecs\n%.2fsecs\n%.2fsecs\n%.2fsecs\n", top[0], top[1], record, top[3]);
        }
        else if (record <= top[3])
        {
            fprintf(fp, "%.2f %.2f %.2f %.2f", top[0], top[1], top[2], record);
            top[3] = record;
            printf("Congratulations! You've made a new 4th record!\n");
            printf("Now let's see your top 5 record(s):\n%.2fsecs\n%.2fsecs\n%.2fsecs\n%.2fsecs\n", top[0], top[1], top[2], record);
        }
        else
        {
            fprintf(fp, "%.2f %.2f %.2f %.2f %.2f", top[0], top[1], top[2], top[3], record);
            top[4] = record;
            printf("Congratulations! You've made a new 5th record!\n");
            printf("Now let's see your top 5 record(s):\n%.2fsecs\n%.2fsecs\n%.2fsecs\n%.2fsecs\n%.2fsecs\n", top[0], top[1], top[2], top[3], record);
        }
    }
    else if (a == 5)
    {
        if (record <= top[0])
        {
            fprintf(fp, "%.2f %.2f %.2f %.2f %.2f", record, top[1], top[2], top[3], top[4]);
            top[0] = record;
            printf("Congratulations! You've made a new 1st record!\n");
            printf("Now let's see your top 5 record(s):\n%.2fsecs\n%.2fsecs\n%.2fsecs\n%.2fsecs\n%.2fsecs\n", record, top[1], top[2], top[3], top[4]);
        }
        else if (record <= top[1])
        {
            fprintf(fp, "%.2f %.2f %.2f %.2f %.2f", top[0], record, top[2], top[3], top[4]);
            top[1] = record;
            printf("Congratulations! You've made a new 2nd record!\n");
            printf("Now let's see your top 5 record(s):\n%.2fsecs\n%.2fsecs\n%.2fsecs\n%.2fsecs\n%.2fsecs\n", top[0], record, top[2], top[3], top[4]);
        }
        else if (record <= top[2])
        {
            fprintf(fp, "%.2f %.2f %.2f %.2f %.2f", top[0], top[1], record, top[3], top[4]);
            top[2] = record;
            printf("Congratulations! You've made a new 3rd record!\n");
            printf("Now let's see your top 5 record(s):\n%.2fsecs\n%.2fsecs\n%.2fsecs\n%.2fsecs\n%.2fsecs\n", top[0], top[1], record, top[3], top[4]);
        }
        else if (record <= top[3])
        {
            fprintf(fp, "%.2f %.2f %.2f %.2f %.2f", top[0], top[1], top[2], record, top[4]);
            top[3] = record;
            printf("Congratulations! You've made a new 4th record!\n");
            printf("Now let's see your top 5 record(s):\n%.2fsecs\n%.2fsecs\n%.2fsecs\n%.2fsecs\n%.2fsecs\n", top[0], top[1], top[2], record, top[4]);
        }
        else if (record <= top[4])
        {
            fprintf(fp, "%.2f %.2f %.2f %.2f %.2f", top[0], top[1], top[2], top[3], record);
            top[4] = record;
            printf("Congratulations! You've made a new 5th record!\n");
            printf("Now let's see your top 5 record(s):\n%.2fsecs\n%.2fsecs\n%.2fsecs\n%.2fsecs\n%.2fsecs\n", top[0], top[1], top[2], top[3], record);
        }
        else if (record > top[4])
        {
            fprintf(fp, "%.2f %.2f %.2f %.2f %.2f", top[0], top[1], top[2], top[3], top[4]);
            printf("It seems that you are not faster than before...\n");
            printf("Now let's see your top 5 record(s):\n%.2fsecs\n%.2fsecs\n%.2fsecs\n%.2fsecs\n%.2fsecs\n", top[0], top[1], top[2], top[3], top[4]);
        }
    }
    fclose(fp);
}
void PlayerInit()
{
    printf("Initiating game file. It won't take long...\n");
    int i, j;
    for (i = 0; i <= column; i++)
    {
        for (j = 0; j <= row; j++)
        {
            if (map[i][j] == 5 || map[i][j] == 7)
            {
                player_x = j;
                player_y = i;
            }
        }
    }
    printf("Initialization complete.\n");
}
void MapRenderer()
{
    int i, j;
    for (i = 0; i < column; i++)
    {
        for (j = 0; j < row; j++)
        {
            switch (map[i][j])
            {
            case 0:
                printf("  ");
                break; //
            case 1:
                printf("■");
                break; //
            case 2:
                printf("  ");
                break; //
            case 3:
                printf("□");
                break; //
            case 4:
                printf("○");
                break; //
            case 5:
                printf("※");
                break; //
            case 6:
                printf("●");
                break; //
            case 7:
                printf("※");
                break; //
            }
        }
        printf("\n");
    }
}
void MapRenderer(int order)
{
    int i, j;
    for (i = 0; i < column; i++)
    {
        for (j = 0; j < row; j++)
        {
            switch (histMap[i][j].serial[order])
            {
            case 0:
                printf("  ");
                break; //
            case 1:
                printf("■");
                break; //
            case 2:
                printf("  ");
                break; //
            case 3:
                printf("□");
                break; //
            case 4:
                printf("○");
                break; //
            case 5:
                printf("※");
                break; //
            case 6:
                printf("●");
                break; //
            case 7:
                printf("※");
                break; //
            }
        }
        printf("\n");
    }
}
void Go(int vec_x, int vec_y)
{
    lastMoveCnt = availMoveCnt;
    int x_1 = player_x + vec_x;
    int y_1 = player_y + vec_y;
    int x_2 = x_1 + vec_x;
    int y_2 = y_1 + vec_y;
    if (map[player_y][player_x] == 7)
    {
        if (map[y_1][x_1] == 1)
            return;
        else if (map[y_1][x_1] == 3)
        {
            if (map[y_2][x_2] == 1 || map[y_2][x_2] == 3 || map[y_2][x_2] == 6)
                return;
            else if (map[y_2][x_2] == 4)
            {
                map[y_2][x_2] = 6;
                map[y_1][x_1] = 5;
                map[player_y][player_x] = 4;
                chests--;
                availMoveCnt++;
            }
            else if (map[y_2][x_2] == 2)
            {
                map[y_2][x_2] = 3;
                map[y_1][x_1] = 5;
                map[player_y][player_x] = 4;
                availMoveCnt++;
            }
        }
        else if (map[y_1][x_1] == 6)
        {
            if (map[y_2][x_1] == 1 || map[y_2][x_2] == 3 || map[y_2][x_2] == 6)
                return;
            else if (map[y_2][x_2] == 4)
            {
                map[y_2][x_2] = 6;
                map[y_1][x_1] = 7;
                map[player_y][player_x] = 4;
                availMoveCnt++;
            }
            else if (map[y_2][x_2] == 2)
            {
                map[y_2][x_2] = 3;
                map[y_1][x_1] = 7;
                map[player_y][player_x] = 4;
                availMoveCnt++;
            }
        }
        else if (map[y_1][x_1] == 4)
        {
            map[player_y][player_x] = 4;
            map[y_1][x_1] = 7;
            availMoveCnt++;
        }
        else
        {
            map[player_y][player_x] = 4;
            map[y_1][x_1] = 5;
            availMoveCnt++;
        }
    }
    else if (map[y_1][x_1] == 1)
        return;
    else if (map[y_1][x_1] == 3)
    {
        if (map[y_2][x_2] == 1 || map[y_2][x_2] == 3 || map[y_2][x_2] == 6)
            return;
        else if (map[y_2][x_2] == 4)
        {
            map[y_2][x_2] = 6;
            map[y_1][x_1] = 5;
            map[player_y][player_x] = 2;
            chests--;
            availMoveCnt++;
        }
        else if (map[y_2][x_2] == 2)
        {
            map[y_2][x_2] = 3;
            map[y_1][x_1] = 5;
            map[player_y][player_x] = 2;
            availMoveCnt++;
        }
    }
    else if (map[y_1][x_1] == 6)
    {
        if (map[y_2][x_2] == 1 || map[y_2][x_2] == 3 || map[y_2][x_2] == 6)
            return;
        else if (map[y_2][x_2] == 4)
        {
            map[y_2][x_2] = 6;
            map[y_1][x_1] = 7;
            map[player_y][player_x] = 2;
            availMoveCnt++;
        }
        else if (map[y_2][x_2] == 2)
        {
            map[y_2][x_2] = 3;
            map[y_1][x_1] = 7;
            map[player_y][player_x] = 2;
            availMoveCnt++;
        }
    }
    else if (map[y_1][x_1] == 4)
    {
        map[y_1][x_1] = 7;
        map[player_y][player_x] = 2;
        availMoveCnt++;
    }
    else
    {
        map[player_y][player_x] = 2;
        map[y_1][x_1] = 5;
        availMoveCnt++;
    }
    player_x = x_1;
    player_y = y_1;
}
int isAvailableMove()
{
    if (availMoveCnt == lastMoveCnt)
        return 0;
    else
        return 1;
}
void HelpFileOpen()
{
    FILE *fp;
    char str[100000];
    if ((fp = fopen("help.txt", "r")) == NULL)
    {
        puts("Fail to open file!");
    }
    while (fgets(str, 100000, fp) != NULL)
    {
        printf("%s", str);
    }
    fclose(fp);
}
void NewMapFileOpen()
{
    FILE *fp;
    if (level == '1')
        fp = fopen(".\\Maps\\1.map", "rwb+");
    else if (level == '2')
        fp = fopen(".\\Maps\\2.map", "rwb+");
    else if (level == '3')
        fp = fopen(".\\Maps\\3.map", "rwb+");
    else if (level == '4')
        fp = fopen(".\\Maps\\4.map", "rwb+");
    else if (level == '5')
        fp = fopen(".\\Maps\\5.map", "rwb+");
    else if (level == '6')
        fp = fopen(".\\Maps\\6.map", "rwb+");
    else if (level == '7')
        fp = fopen(".\\Maps\\7.map", "rwb+");
    else if (level == '8')
        fp = fopen(".\\Maps\\8.map", "rwb+");
    else if (level == '9')
        fp = fopen(".\\Maps\\9.map", "rwb+");
    else if (level == 's')
        fp = fopen(".\\Maps\\mymap.map", "rwb+");
    else
        return;
    fscanf(fp, "%d %d %d", &row, &column, &chests);
    for (int i = 0; i < column; i++)
    {
        for (int j = 0; j < row; j++)
        {
            fscanf(fp, "%1d", &map[i][j]);
        }
    }
    fclose(fp);
}
void Save()
{
    printf("Saving map file...\n");
    guiInputCheck = 0;
    FILE *_fp;
    _fp = fopen(".\\Maps\\mymap.map", "w+");
    fprintf(_fp, "%d", row);
    fprintf(_fp, " ");
    fprintf(_fp, "%d", column);
    fprintf(_fp, " ");
    fprintf(_fp, "%d", chests);
    fprintf(_fp, "\n");
    for (int i = 0; i < column; i++)
    {
        for (int j = 0; j < row; j++)
        {
            fprintf(_fp, "%d", map[i][j]);
        }
        fprintf(_fp, "\n");
    }
    fclose(_fp);
    printf("Map file saved. \n");
}
void AutoSave()
{
    if (saveFreq == 0)
        return;
    else if (availMoveCnt % saveFreq == 0)
    {
        guiInputCheck = 0;
        FILE *_fp;
        _fp = fopen(".\\Maps\\mymap.map", "w+");
        fprintf(_fp, "%d", row);
        fprintf(_fp, " ");
        fprintf(_fp, "%d", column);
        fprintf(_fp, " ");
        fprintf(_fp, "%d", chests);
        fprintf(_fp, "\n");
        for (int i = 0; i < column; i++)
        {
            for (int j = 0; j < row; j++)
            {
                fprintf(_fp, "%d", map[i][j]);
            }
            fprintf(_fp, "\n");
        }
        fclose(_fp);
    }
}
void HistStore()
{
    if (histRecordMax == 0)
        return;
    else if (isAvailableMove())
    {
        if (availMoveCnt < histRecordMax)
        {
            for (int i = availMoveCnt - 1; i >= 0; i--)
            {
                for (int j = 0; j < column; j++)
                {
                    for (int k = 0; k < row; k++)
                    {
                        histMap[j][k].serial[i + 1] = histMap[j][k].serial[i];
                    }
                }
            }
            //一定要倒着赋值才能保证不丢失数据！！！
            for (int i = 0; i < column; i++)
            {
                for (int j = 0; j < row; j++)
                {
                    histMap[i][j].serial[0] = map[i][j];
                }
            }
        }
        //if减少初始计算量
        else
        {
            for (int i = histRecordMax + 1; i >= 0; i--)
            {
                for (int j = 0; j < column; j++)
                {
                    for (int k = 0; k < row; k++)
                    {
                        histMap[j][k].serial[i + 1] = histMap[j][k].serial[i];
                    }
                }
            }
            for (int i = 0; i < column; i++)
            {
                for (int j = 0; j < row; j++)
                {
                    histMap[i][j].serial[0] = map[i][j];
                }
            }
        }
    }
}
void EmptyCache(int order) //时空穿越后初始化之前的缓存区
{
    for (int i = 0; i < column; i++)
    {
        for (int j = 0; j < row; j++)
        {
            histMap[i][j].serial[order] = 0;
        }
    }
}
int isEmptyCache(int order)
{
    for (int i = 0; i <= column; i++)
    {
        for (int j = 0; j <= row; j++)
        {
            if (histMap[i][j].serial[order] != 0)
                return 0;
        }
    }
    return 1;
}
void Recall(int order) //要开始穿越了！千万注意逻辑！！！这里的order是穿越到order步之前。
{
    for (int i = 0; i < column; i++)
    {
        for (int j = 0; j < row; j++)
        {
            if (map[i][j] == 6 && histMap[i][j].serial[order] == 4)
                chests++;
        }
    } //一开始就要恢复箱子数：后面要操作数组了；箱子数=0是判断胜利的条件
    for (int i = 0; i < column; i++)
    {
        for (int j = 0; j < row; j++)
        {
            map[i][j] = histMap[i][j].serial[order];
        }
    } //将地图更新为历史地图
    for (int i = 0; i <= histRecordMax - order; i++)
    {
        for (int j = 0; j < column; j++)
        {
            for (int k = 0; k < row; k++)
            {
                histMap[j][k].serial[i] = histMap[j][k].serial[order + i];
            }
        }
    }
    for (int i = histRecordMax - order + 1; i < histRecordMax + 1; i++)
        EmptyCache(i); //清空剩下的内存
    for (int i = 0; i < column; i++)
    {
        for (int j = 0; j < row; j++)
        {
            if (map[i][j] == 5 || map[i][j] == 7)
            {
                player_x = j;
                player_y = i;
            }
        }
    } //PlayerInit without output
}
void debugHistMap(int order)
{
    MapRenderer(order);
}
void MainMenu()
{
    printf("Guten tag. This is my chest pulling game.\n");
    printf("If this is your first trial, plz press 'h' to call for help.\n");
    printf("Or if you are really familiar with the keybinding, you can directly go to the map selection menu.\n");
    guiInputCheck = 1;
    while (guiInputCheck)
    {
        fflush(stdin);
        scanf("%c", &guiInputKey[0]);
        if (guiInputKey[0] == 'h')
            HelpFileOpen();
        else if (guiInputKey[0] == 'q')
        {
            printf("Then it's time to say goodbye!\n");
            Sleep(1000);
            printf("Wish you have a good day!\n");
            Sleep(1000);
            return;
        }
        else if (guiInputKey[0] == 's')
        {
            guiInputCheck = 0;
            system("cls");
        }
        else if (guiInputKey[0] == 'f')
        {
            int flag_freq = 1;
            while (flag_freq)
            {
                printf("Now you may define your auto-Save freqency(0 to 10).\n");
                printf("If 0 is input, auto-Save function will be diasbled.\n");
                fflush(stdin);
                scanf("%d", &saveFreq);
                if (saveFreq >= 0 && saveFreq <= 10)
                {
                    if (saveFreq == 0)
                        printf("Auto-Save is disabled.\n");
                    else
                        printf("Your auto-Save freq has been successfully set to ONCE EVERY %d MOVE(S).\n", saveFreq);
                    flag_freq = 0;
                }
                else if (saveFreq != '\n')
                    printf("The freqency must be set from 1 to 10!\n");
            }
        }
        else if (guiInputKey[0] == 'p')
        {
            int flag_hist = 1;
            printf("Now select maximum history map record times:\n");
            printf("If 0 is input, recalling is disabled.\n");
            while (flag_hist)
            {
                fflush(stdin);
                scanf("%d", &histRecordMax);
                if (histRecordMax >= 0 && histRecordMax <= 10)
                {
                    if (histRecordMax == 0)
                        printf("Recalling function haas been disabled.\n");
                    else
                        printf("The maximum history map record time(s) has been set to %d.\n", histRecordMax);
                    flag_hist = 0;
                }
                else if (histRecordMax != '\n')
                    printf("Please input a number from 0 to 10.\n");
            }
        }
        else if (guiInputKey[0] == 'i')
        {
            printf("Here's the two parameters you want to check:\n");
            if (saveFreq != 0)
                printf("Your auto-save frequency: %d per step(s).\n", saveFreq);
            else
                printf("Auto-save has been disabled.\n");
            if (histRecordMax != 0)
                printf("Your maximum history record times has been set to %d.\n", histRecordMax);
            else
                printf("Recalling has been disabled.\n");
        }
        else if (guiInputKey[0] != '\n')
            printf("Please input an available key. If you need help, press 'h'.\n");
    }
}
void LvlSelection()
{
    printf("Now select a level to preview,or press 'q' to go back to main menu:\n");
    while (1)
    {
        fflush(stdin);
        scanf("%c", &level);
        if (level == '1' || level == '2' || level == '3' || level == '4' || level == '5' || level == '6' || level == '7' || level == '8' || level == '9')
        {
            NewMapFileOpen();
            MapRenderer();
            printf("Confirmation: Level %c?\nY or N:\n", level);
            guiInputCheck = 1;
            while (guiInputCheck)
            {
                fflush(stdin);
                scanf("%c", &guiInputKey[1]);
                if (guiInputKey[1] == 'y')
                    return;
                else if (guiInputKey[1] == 'n')
                {
                    system("cls");
                    printf("Then choose the levels again:\n");
                    guiInputCheck = 0;
                }
                else if (guiInputKey[1] == 'h')
                    HelpFileOpen();
                else if (guiInputKey[1] != '\n')
                    printf("Please input 'y' or 'n'.\n");
            }
        }
        else if (level == 's')
        {
            NewMapFileOpen();
            MapRenderer();
            printf("Confirmation: Saved level?\nY or N:\n");
            guiInputCheck = 1;
            while (guiInputCheck)
            {
                fflush(stdin);
                scanf("%c", &guiInputKey[1]);
                if (guiInputKey[1] == 'y')
                    return;
                else if (guiInputKey[1] == 'n')
                {
                    printf("Then choose the levels again:\n");
                    guiInputCheck = 0;
                }
                else if (guiInputKey[1] == 'h')
                    HelpFileOpen();
                else if (guiInputKey[1] != '\n')
                    printf("Please input an available key. If you need help, press 'h'.\n");
            }
        }
        else if (level == 'h')
            HelpFileOpen();
        else if (level == 'q')
        {
            system("cls");
            return;
        }
        else if (level != '\n')
            printf("Please input an available key. If you need help, press 'h'.\n");
    }
}
void Gameplay()
{
    system("cls");
    MapRenderer();
    int super_flag, recall_flag;
    int recall_order;
    availMoveCnt = 0;
    gameInputCnt = 0;
    timeCache = 0;
    for (int i = 0; i < column; i++)
    {
        for (int j = 0; j < row; j++)
        {
            histMap[i][j].serial[0] = map[i][j];
        }
    }
    if (level != 's')
    {
        printf("Mind that timing begins with your first available move!\n");
    }
    while (1)
    {
        //debugHistMap(0);
        //debugHistMap(1);
        for (int k = 1; k < X; k++)
            game[k] = '\0';
        super_flag = 1;
        if (chests == 0)
        {
            printf("You Win !\n ");
            if (level != 's')
            {
                gamePause = clock();
                timeCache += (gamePause - gameContinue);
                TimingHist();
                printf("Now, you may press 'm' to return to main menu,\npress 's' to return to level selection,\nor press 'r' to replay this level.\n");
                while (1)
                {
                    fflush(stdin);
                    scanf("%c", &game[0]);
                    if (game[0] == 'm' || game[0] == 's' || game[0] == 'r')
                    {
                        fflush(stdout);
                        system("cls");
                        return;
                    }
                    else if (game[0] != '\n')
                        printf("Please input an available key.\n");
                }
            }
            else
            {
                printf("Now, you may press 'm' to return to main menu,\nor press 's' to return to level selection.\n");
                while (1)
                {
                    fflush(stdin);
                    scanf("%c", &game[0]);
                    if (game[0] == 'm' || game[0] == 's')
                    {
                        fflush(stdout);
                        system("cls");
                        return;
                    }
                    else if (game[0] != '\n')
                        printf("Please input an available key.\n");
                }
            }
        }
        histGameInput[0] = game[0];
        fflush(stdin);
        gets(game);
        gameInputCnt++;
        Timing();
        if (game[0] == 'w')
        {
            system("cls");
            GoUp();
            MapRenderer();
            HistStore();
        }
        else if (game[0] == 's')
        {
            system("cls");
            GoDown();
            MapRenderer();
            HistStore();
        }
        else if (game[0] == 'a')
        {
            system("cls");
            GoLeft();
            MapRenderer();
            HistStore();
        }
        else if (game[0] == 'd')
        {
            system("cls");
            GoRight();
            MapRenderer();
            HistStore();
        }
        else if (game[0] == 'x')
        {
            if (histRecordMax == 0)
                printf("The recalling function has been closed.\nIf you wan to open it, please go back to main menu and press 'p', set the number to non-0 to open.\n");
            else if (availMoveCnt == 0)
            {
                printf("No recent available moves!\n");
            }
            else if (isEmptyCache(1))
                printf("Recall is unavailable because it has been out of memory capability.\n");
            else
            {
                system("cls");
                Recall(1);
                printf("Now bring you back 1 step:\n");
                MapRenderer();
                printf("Time travel complete!\n");
            }
        }
        else if (game[0] == 'e')
        {
            printf("Now navigating you to the main menu, WITHOUT SAVING...\n");
            Sleep(1000);
            fflush(stdout);
            system("cls");
            return;
        }
        else if (game[0] == 'r')
            Save();
        else if (game[0] == 'q')
        {
            Save();
            printf("Now navigating you to the main menu...\n");
            Sleep(1000);
            fflush(stdout);
            system("cls");
            return;
        }
        else if (game[0] == 'h')
            HelpFileOpen();
        else if (game[0] == 'm')
        {
            return;
        }
        else if (game[0] == 'z')
        {
            guiInputCheck = 1;
            while (guiInputCheck)
            {
                printf("Do you really want to restart this level?\nY or N :\n");
                fflush(stdin);
                scanf("%c", &game[0]);
                if (game[0] == 'y')
                {
                    printf("Navigating to initialization...\n");
                    Sleep(1000);
                    fflush(stdout);
                    system("cls");
                    return;
                }
                else if (game[0] == 'n')
                {
                    printf("You are now continuing this game.\n");
                    guiInputCheck = 0;
                }
                else if (game[0] != '\n')
                    printf("Please input 'y' or 'n'.\n");
            }
        }
        else if (game[0] == '/')
        {
            for (int j = 1; game[j] != '\0'; j++)
            {
                if (game[j] != 'w' && game[j] != 'a' && game[j] != 's' && game[j] != 'd' && game[1] != 'x')
                {
                    super_flag = 0;
                    printf("Your superman command contains unavailable input.The command cannot be carried out!\n");
                    break;
                }

                if (game[1] == 'x')
                {
                    super_flag = 0;
                    recall_flag = 1;
                    if (availMoveCnt == 0)
                        printf("No recent moves!\n");
                    else if (histRecordMax == 0)
                        printf("The recalling function has been closed.\nIf you want to open it, please back to main menu and press 'p' ,set the number to non-0 open.\n");
                    else
                    {
                        if (isEmptyCache(1))
                            printf("Out of memory.\n");
                        else if (availMoveCnt < histRecordMax)
                        {
                            printf("Your recent %d moves are listed below:\n", availMoveCnt);
                            for (int i = 1; i < availMoveCnt + 1; i++)
                            {
                                printf("%d move(s) ago you're:\n", i);
                                MapRenderer(i);
                            }
                        }
                        else
                        {
                            printf("Your recent %d moves are listed below:\n", histRecordMax);
                            for (int i = 1; i < histRecordMax + 1; i++)
                            {
                                printf("%d move(s) ago you're:\n", i);
                                MapRenderer(i);
                            }
                        }
                        while (recall_flag)
                        {
                            printf("Now please select which step you want to go back to:\n");
                            fflush(stdin);
                            scanf("%d", &recall_order);
                            if (recall_order != '\n')
                            {
                                if (recall_order > histRecordMax || recall_order > availMoveCnt || recall_order < 0)
                                    printf("Please input an availble POSITIVE INTEGAR below your set maximum record cache.\nOr your input contains illegal characters.\n");
                                else if (recall_order != '\n')
                                {
                                    system("cls");
                                    Recall(recall_order);
                                    printf("Now bringing you back %d steps:\n", recall_order);
                                    MapRenderer();
                                    printf("Time voyage complete!\n");
                                    availMoveCnt -= recall_order;
                                    recall_flag = 0;
                                    //debugHistMap(1);
                                }
                            }
                        }
                    }
                    break;
                }
                if (super_flag)
                {
                    if (chests == 0)
                        break;
                    else if (game[j] == 'w')
                    {
                        GoUp();
                    }
                    else if (game[j] == 'a')
                    {
                        GoLeft();
                    }
                    else if (game[j] == 's')
                    {
                        GoDown();
                    }
                    else if (game[j] == 'd')
                    {
                        GoRight();
                    }
                    //debug();
                    HistStore();
                }
                system("cls");
                MapRenderer();
            }
            //puts(game);
            //printf("%d\n",flag);
        }
        else if (game[0] == 'i')
        {
            system("cls");
            printf("Here's the two parameters you want to check:\n");
            if (saveFreq != 0)
                printf("Your auto-save frequency: %d per step(s).\n", saveFreq);
            else
                printf("Auto-save has been disabled.\n");
            if (histRecordMax != 0)
                printf("Your maximum history record times has been set to %d.\n", histRecordMax);
            else
                printf("Recalling has been disabled.\n");
            printf("And this, is your current map:\n");
            MapRenderer();
        }
        else if (game[0] != '\0')
            printf("Please input an available key. If you need help, press 'h'.\n");
        //printf("%d\n", availMoveCnt);
        AutoSave();
    }
}
int main()
{
    while (1)
    {
        if (game[0] == '-' || game[0] == 'm' || game[0] == 'q' || game[0] == 'e')
        {
            MainMenu();
            if (guiInputKey[0] == 'q')
                return 0;
            LvlSelection();
            if (level == 'q')
                guiInputCheck = 0;
            else
                guiInputCheck = 1;
            if (guiInputCheck)
            {
                PlayerInit();
                Gameplay();
            }
        }
        if (game[0] == 'y' || game[0] == 'r')
        {
            NewMapFileOpen();
            MapRenderer();
            PlayerInit();
            Gameplay();
        }
        if (game[0] == 's')
        {
            LvlSelection();
            if (level == 'q')
            {
                guiInputCheck = 0;
                game[0] = '-';
            }
            else
                guiInputCheck = 1;
            if (guiInputCheck)
            {
                PlayerInit();
                Gameplay();
            }
        }
    }
    system("pause");
}