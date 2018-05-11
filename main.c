#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include <time.h>

/***** 数据结构 *****/

//蛇的节点
typedef struct _node
{
    int x; //x坐标
    int y; //y坐标
    struct _node *next; //链表结构指向下一个节点
}Node;

Node *head; //蛇头
Node *index; //游标 用于删除蛇尾
Node *oldTail; //记录上一次移动蛇尾的位置 用于在蛇吃到食物时 增加节点

int food[2]; //整形数组 用于存放食物坐标

int windowX = 100; //窗体的宽 必须设为偶数
int windowY = 36; //窗体的高 必须小于宽的二分之一 为文字界面留出空间
int point; //游戏得分初始为0
int level; //游戏速度级别 初始为1
int itval; //初始定时器间隔 初始为600
char direction; //初始方向 为右
int length = 0; //蛇的长度
int eatfood = 0; //用于判定是否吃到食物

/***** 函数 *****/
//游戏初始化
void InitWindow(void);
//设置控制台标题
void SetTitle(char *);
//设置控制台前景和背景色
void SetColor(int, int);
//设置光标位置
void SetPos(int, int);
//设置控制台大小
void SetWindow(int, int);
//清屏
void Cls(void);
//隐藏光标
void HideCsr(void);

//初始化界面 墙 文字面板等
void InitItface(void);
//初始化蛇 参数为蛇尾的初始位置 默认蛇身+蛇头三节
void InitSnake(int, int);
//初始化食物
void InitFood(void);
//生成随机食物
void RandomFood(void);
//食物随机颜色
void RandomColor(void);
//还原颜色
void ResetColor(void);

//重新打印蛇
void RePrintSnake(void);
//重新设置游标指针
void ReIndex(void);

//游戏控制引擎
void Control(void);
//根据移动方向来改变x y坐标的值 参数为x或y 返回整型
int XyChange(char);

//死亡判定
//碰到墙壁
int TouchWall(void);
//碰到自己
int TouchSelf(void);

//判断是否吃到食物
int EatFood(void);
//尾部增加
void TailGain(void);
//尾部打印
void PrintTail(void);

//开始游戏
void StartGame(void);
//游戏结束
void GameOver(void);
//暂停游戏
void Pause(void);
//重置游戏参数
void ReInitAll(void);

//吃到食物则加10分
void GainPoint(void);
//分数达到50的整数倍 增加级别和速度
void GainLevel(void);

int main()
{
    InitWindow();
    StartGame();
    Control();

    return 0;
}

//初始化函数 窗体 颜色 标题
void InitWindow(void)
{
    SetTitle("贪吃蛇");
    SetColor(10, 0);
    SetWindow(windowX, windowY);
    HideCsr();
}

//设置标题
void SetTitle(char *title)
{
    SetConsoleTitle(title);
}

//设置前景背景色
void SetColor(int fColor, int bColor)
{
    HANDLE  winHandle; //句柄
    winHandle = GetStdHandle(STD_OUTPUT_HANDLE); //控制台
    SetConsoleTextAttribute(winHandle, fColor + bColor * 0x10);
}

//控制台移动光标
void SetPos(int x, int y)
{
    HANDLE  winHandle; //句柄
    winHandle = GetStdHandle(STD_OUTPUT_HANDLE); //控制台
    COORD coord = {x, y}; //winapi 自带坐标结构体
    SetConsoleCursorPosition(winHandle, coord);
}

//设置窗体大小
void SetWindow(int x, int y)
{
    char str[100] = "mode con cols=";
    char str2[8] = " lines=";
    char strX[10], strY[10];
    itoa(x, strX, 10);
    itoa(y, strY, 10);
    strcat(str, strX);
    strcat(str, str2);
    strcat(str, strY);
    system(str);
}

//清屏
void Cls(void)
{
    system("cls");
}

//隐藏光标
void HideCsr(void)
{
    HANDLE  winHandle; //句柄
    winHandle = GetStdHandle(STD_OUTPUT_HANDLE); //控制台

    CONSOLE_CURSOR_INFO cinfo;
    cinfo.bVisible = 0;
    //设置光标隐藏
    cinfo.dwSize = 1;
    SetConsoleCursorInfo(winHandle, &cinfo);
}

//初始化界面
void InitItface(void)
{
    //初始化分数 级别 定时器间隔
    point = 0;
    level = 1;
    itval = 500;

    int i, j;
    for(i = 0; i < windowY; i++)
    {
        for(j = 0; j < windowX / 2; j++)
        {
            if(i == 0 || i == windowY - 1 || j == 0 || j == windowX / 2 -1 || j == windowY - 1)
            {
                SetColor(6, 15);
                printf("%c%c", 0xa8, 0x80);
            }
            else
            {
                SetColor(10, 0);
                printf("  ");
            }
        }
        if(i != windowY - 1)
        {
            printf("\n");
        }
    }
    //文字界面
    SetColor(10, 0);
    SetPos(78, 8);
    printf("得分：%-3d", point);
    SetPos(78, 10);
    printf("级别：%-3d", level);
    SetPos(76, 15);
    printf("操作：");
    SetPos(78, 17);
    printf("方向：↑ → ↓ ←");
    SetPos(78, 19);
    printf("开始/暂停：Space");
    SetPos(78, 21);
    printf("退出：Esc");
    SetPos(76, 24);
    printf("作者：jingks");
    SetPos(76, 26);
    printf(" Q Q：768603508");
}

//初始化蛇
void InitSnake(int x, int y)
{
    //初始化蛇的运动方向
    direction = 'r';

    //设置蛇的颜色
    SetColor(14, 0);

    Node *tail;
    int i;
    tail = (Node *)malloc(sizeof(Node));
    tail->x = x;
    tail->y = y;
    tail->next = NULL; //空指针

    for(i = 1; i < 3; i++)
    {
        head = (Node *)malloc(sizeof(Node));
        head->next = tail;
        head->x = x + 2 * i; //一个方块俩个字节
        head->y = y; //默认蛇的初始运动方向为右 蛇身y不变
        tail = head; //移动tail指针为链表循环赋值
    }

    while(tail != NULL)
    {
        SetPos(tail->x, tail->y);
        if(tail->x == head->x && tail->y == head->y) //蛇头则打印成淡红色
        {
            SetColor(12, 0);
        }
        printf("%c%c", 0xa8, 0x80);
        tail = tail->next;
        SetColor(14, 0);//还原颜色
        //计算蛇的初始化长度
        length++;
    }

    //初始化oldTail
    oldTail = (Node *)malloc(sizeof(Node));

    //调用ReIndex 把Index指向倒数第二节蛇尾
    ReIndex();

}

//初始化食物
void InitFood(void)
{
    RandomFood();
}

//重置所有参数
void ReInitAll(void)
{
    point = 0;
    level = 1;
    itval = 500;

    direction = 'r'; //初始方向 为右
    length = 0; //蛇的长度
    eatfood = 0; //用于判定是否吃到食物
}

//随机在地图上生成食物 避免在墙壁 蛇身上生成
void RandomFood(void)
{
    int flag = 0;//定义flag用来统计食物坐标是否与蛇身相同 不同则加1 最后与蛇的长度相比较
    int randX, randY;
    srand((unsigned int)time(NULL));

    //生成不与蛇身重合的食物
    Node *pointer; //一次性指针

    while(flag < length) //flag与蛇长度比较 如果小于蛇长 说明有部位与之重合 重新生成食物
    {
        //注意指针必须也得重置
        pointer = head; //头指针地址赋给一次性指针
        flag = 0;//重置flag

        randX = (rand() % (windowY - 2) + 1) * 2; //2-68之间的偶数
        randY = rand() % (windowY - 2) + 1; //1-34之间的数


        while(pointer != NULL)
        {
            if(randX != pointer->x && randY != pointer->y)//如果食物坐标与当前蛇的部分坐标不同
            {
                flag++;
            }
            pointer = pointer->next;
        }

    }

    //在地图上打印食物
    SetPos(randX, randY);
    RandomColor();
    printf("●");
    ResetColor();
    //把坐标赋给食物数组
    food[0] = randX;
    food[1] = randY;

}

//食物随机颜色函数
void RandomColor(void)
{
    int randNum;
    srand((unsigned int)time(NULL));
    randNum = rand() % 14 + 1; //1-14随机数
    SetColor(randNum, 0);
}

//还原颜色
void ResetColor(void)
{
    SetColor(10, 0);
}

//重新打印蛇
void RePrintSnake(void)
{

    //生头
    Node *newHead;
    newHead = (Node *)malloc(sizeof(Node));
    newHead->x = head->x + XyChange('x'); //调用函数改变坐标
    newHead->y = head->y + XyChange('y');
    newHead->next = head; //生成新的头并指向旧的头
    head = newHead; //head指针指向新头

    //先隐藏旧尾 后打印新头 若先打印头后隐藏尾当头运动到尾部时会被掩盖
    //隐藏旧尾 index->next 就是尾
    SetPos(index->next->x, index->next->y);
    printf("  ");

    //打印新头
    SetPos(head->x, head->y);

    SetColor(12, 0);//下一次的蛇头要为红色
    printf("%c%c", 0xa8, 0x80);
    SetPos(head->next->x, head->next->y);

    SetColor(14, 0);//旧的蛇头还原为黄色
    printf("%c%c", 0xa8, 0x80);


    //给旧尾赋值
    oldTail->x = index->next->x;
    oldTail->y = index->next->y;
    oldTail->next = NULL;
    //去尾 释放链表中的尾 并把index重新定位到第二节尾
    free(index->next);
    index->next = NULL;
    ReIndex();
}

//重定向游标指针
void ReIndex(void)
{
    index = head;
    while(index->next->next != NULL) //直到取出第二节蛇尾
    {
        index = index->next;
    }
}

//游戏控制引擎
void Control(void)
{
    while(1)
    {
        //初始界面和蛇和食物
        Cls();
        InitItface();
        InitSnake(4, 4); //注意不要让蛇身子越界
        InitFood(); //食物初始化

        //初始游戏为暂停状态
        Pause();

        while(1)
        {
            //重置食物flag
            eatfood = 0;
            //异步监听按键
            if(GetAsyncKeyState(VK_UP) && direction != 'd') //不为下时向上
            {
                direction = 'u';
            }
            else if(GetAsyncKeyState(VK_DOWN) && direction != 'u') //不为上时向下
            {
                direction = 'd';
            }
            else if(GetAsyncKeyState(VK_LEFT) && direction != 'r') //不为右时向左
            {
                direction = 'l';
            }
            else if(GetAsyncKeyState(VK_RIGHT) && direction != 'l') //不为左时向右
            {
                direction = 'r';
            }
            else if(GetAsyncKeyState(VK_SPACE)) //如果按了空格就暂停
            {
                Pause();
            }
            else if(GetAsyncKeyState(VK_ESCAPE))
            {
                exit(0);
            }
            RePrintSnake();

            //打印完蛇后立即进行死亡判定
            if(TouchWall() || TouchSelf())
            {
                GameOver();
                break;
            }

            //如果吃到食物立即给链表尾部增加数据 增加分数  判断增加游戏级别 并生成新的食物
            if(EatFood())
            {
                eatfood = 1;
                ResetColor();//还原颜色
                TailGain(); //增加尾部数据
                GainPoint(); //增加分数
                if(point % 50 == 0)//第一次进来时分数已经是10 所以不会加速
                {
                    GainLevel();
                }
                RandomFood(); //随机生成食物
            }

            //游戏定时器
            Sleep(itval);

            //吃到食物 移动过后打印新尾部
            if(eatfood)
            {
                PrintTail();

            }

        }
    }
}

//x y坐标改变
int XyChange(char xy)
{
    int num;
    if(xy == 'x')
    {
        switch(direction)
        {
        case 'r':
            num = 2;
            break;
        case 'l':
            num = -2;
            break;
        case 'u':
            num = 0;
            break;
        case 'd':
            num = 0;
            break;
        }
    }
    else if(xy == 'y')
    {
        switch(direction)
        {
        case 'r':
            num = 0;
            break;
        case 'l':
            num = 0;
            break;
        case 'u':
            num = -1;
            break;
        case 'd':
            num = 1;
            break;
        }
    }
    return num;
}

//碰到墙壁
int TouchWall(void)
{
    int state = 0;
    if(head->x <= 0 || head->x >= (windowY - 1) * 2 || head->y <= 0 || head->y >= windowY - 1)
    {
        state = 1;
    }
    return state;
}

//碰到自己
int TouchSelf(void)
{
    int state = 0;
    //定义一次性指针
    Node *pointer;
    //如果头和除头以外任何一个身子坐标相同 则判定死亡
    pointer = head->next;
    while(pointer != NULL)
    {
        if(head->x == pointer->x && head->y == pointer->y)
        {
            state = 1;
        }
        pointer = pointer->next;
    }
    return state;
}

//头部是否吃到食物
int EatFood(void)
{
    int state = 0;
    if(head->x == food[0] && head->y == food[1])
    {
        length++; //吃到蛇的长度加1
        state = 1;
    }
    return state;
}

//尾部数据增加
void TailGain(void)
{
    //尾部增加节点数据
    Node *newTail;
    newTail = (Node *)malloc(sizeof(Node));
    newTail->x = oldTail->x;
    newTail->y = oldTail->y;
    newTail->next = NULL;
    index->next->next = newTail;

    //重定向游标指针
    ReIndex();
}

//吃到食物 打印新尾部
void PrintTail(void)
{
    SetPos(oldTail->x, oldTail->y);
    printf("%c%c", 0xa8, 0x80);
}

//开始游戏
void StartGame(void)
{
    SetPos(44, 14);
    printf("开始游戏");
    SetPos(40, 16);
    system("pause");
}

//游戏结束
void GameOver(void)
{
    //调用清屏函数并打印分数
    Cls();
    ResetColor();
    SetPos(46, 14);
    printf("game over");
    SetPos(40, 18);
    printf("你死亡了，得分为：%d", point);
    SetPos(40, 19);
    system("pause");
    ReInitAll();
}

//暂停游戏
void Pause()
{
    while(1)
    {
        Sleep(300);
        if(GetAsyncKeyState(VK_SPACE))
        {
            break;
        }
        else if(GetAsyncKeyState(VK_ESCAPE))
        {
            exit(0);
        }
    }
}

//增加分数
void GainPoint(void)
{
    point += 10;
    SetPos(78, 8);
    printf("得分：%-3d", point);
}

//增加级别 速度
void GainLevel(void)
{
    if(itval > 50)
    {
        level++;
        SetPos(78, 10);
        printf("级别：%-3d", level);

        itval -= 50;
    }
}
