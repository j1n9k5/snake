#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include <time.h>

/***** ���ݽṹ *****/

//�ߵĽڵ�
typedef struct _node
{
    int x; //x����
    int y; //y����
    struct _node *next; //����ṹָ����һ���ڵ�
}Node;

Node *head; //��ͷ
Node *index; //�α� ����ɾ����β
Node *oldTail; //��¼��һ���ƶ���β��λ�� �������߳Ե�ʳ��ʱ ���ӽڵ�

int food[2]; //�������� ���ڴ��ʳ������

int windowX = 100; //����Ŀ� ������Ϊż��
int windowY = 36; //����ĸ� ����С�ڿ�Ķ���֮һ Ϊ���ֽ��������ռ�
int point; //��Ϸ�÷ֳ�ʼΪ0
int level; //��Ϸ�ٶȼ��� ��ʼΪ1
int itval; //��ʼ��ʱ����� ��ʼΪ600
char direction; //��ʼ���� Ϊ��
int length = 0; //�ߵĳ���
int eatfood = 0; //�����ж��Ƿ�Ե�ʳ��

/***** ���� *****/
//��Ϸ��ʼ��
void InitWindow(void);
//���ÿ���̨����
void SetTitle(char *);
//���ÿ���̨ǰ���ͱ���ɫ
void SetColor(int, int);
//���ù��λ��
void SetPos(int, int);
//���ÿ���̨��С
void SetWindow(int, int);
//����
void Cls(void);
//���ع��
void HideCsr(void);

//��ʼ������ ǽ ��������
void InitItface(void);
//��ʼ���� ����Ϊ��β�ĳ�ʼλ�� Ĭ������+��ͷ����
void InitSnake(int, int);
//��ʼ��ʳ��
void InitFood(void);
//�������ʳ��
void RandomFood(void);
//ʳ�������ɫ
void RandomColor(void);
//��ԭ��ɫ
void ResetColor(void);

//���´�ӡ��
void RePrintSnake(void);
//���������α�ָ��
void ReIndex(void);

//��Ϸ��������
void Control(void);
//�����ƶ��������ı�x y�����ֵ ����Ϊx��y ��������
int XyChange(char);

//�����ж�
//����ǽ��
int TouchWall(void);
//�����Լ�
int TouchSelf(void);

//�ж��Ƿ�Ե�ʳ��
int EatFood(void);
//β������
void TailGain(void);
//β����ӡ
void PrintTail(void);

//��ʼ��Ϸ
void StartGame(void);
//��Ϸ����
void GameOver(void);
//��ͣ��Ϸ
void Pause(void);
//������Ϸ����
void ReInitAll(void);

//�Ե�ʳ�����10��
void GainPoint(void);
//�����ﵽ50�������� ���Ӽ�����ٶ�
void GainLevel(void);

int main()
{
    InitWindow();
    StartGame();
    Control();

    return 0;
}

//��ʼ������ ���� ��ɫ ����
void InitWindow(void)
{
    SetTitle("̰����");
    SetColor(10, 0);
    SetWindow(windowX, windowY);
    HideCsr();
}

//���ñ���
void SetTitle(char *title)
{
    SetConsoleTitle(title);
}

//����ǰ������ɫ
void SetColor(int fColor, int bColor)
{
    HANDLE  winHandle; //���
    winHandle = GetStdHandle(STD_OUTPUT_HANDLE); //����̨
    SetConsoleTextAttribute(winHandle, fColor + bColor * 0x10);
}

//����̨�ƶ����
void SetPos(int x, int y)
{
    HANDLE  winHandle; //���
    winHandle = GetStdHandle(STD_OUTPUT_HANDLE); //����̨
    COORD coord = {x, y}; //winapi �Դ�����ṹ��
    SetConsoleCursorPosition(winHandle, coord);
}

//���ô����С
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

//����
void Cls(void)
{
    system("cls");
}

//���ع��
void HideCsr(void)
{
    HANDLE  winHandle; //���
    winHandle = GetStdHandle(STD_OUTPUT_HANDLE); //����̨

    CONSOLE_CURSOR_INFO cinfo;
    cinfo.bVisible = 0;
    //���ù������
    cinfo.dwSize = 1;
    SetConsoleCursorInfo(winHandle, &cinfo);
}

//��ʼ������
void InitItface(void)
{
    //��ʼ������ ���� ��ʱ�����
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
    //���ֽ���
    SetColor(10, 0);
    SetPos(78, 8);
    printf("�÷֣�%-3d", point);
    SetPos(78, 10);
    printf("����%-3d", level);
    SetPos(76, 15);
    printf("������");
    SetPos(78, 17);
    printf("���򣺡� �� �� ��");
    SetPos(78, 19);
    printf("��ʼ/��ͣ��Space");
    SetPos(78, 21);
    printf("�˳���Esc");
    SetPos(76, 24);
    printf("���ߣ�jingks");
    SetPos(76, 26);
    printf(" Q Q��768603508");
}

//��ʼ����
void InitSnake(int x, int y)
{
    //��ʼ���ߵ��˶�����
    direction = 'r';

    //�����ߵ���ɫ
    SetColor(14, 0);

    Node *tail;
    int i;
    tail = (Node *)malloc(sizeof(Node));
    tail->x = x;
    tail->y = y;
    tail->next = NULL; //��ָ��

    for(i = 1; i < 3; i++)
    {
        head = (Node *)malloc(sizeof(Node));
        head->next = tail;
        head->x = x + 2 * i; //һ�����������ֽ�
        head->y = y; //Ĭ���ߵĳ�ʼ�˶�����Ϊ�� ����y����
        tail = head; //�ƶ�tailָ��Ϊ����ѭ����ֵ
    }

    while(tail != NULL)
    {
        SetPos(tail->x, tail->y);
        if(tail->x == head->x && tail->y == head->y) //��ͷ���ӡ�ɵ���ɫ
        {
            SetColor(12, 0);
        }
        printf("%c%c", 0xa8, 0x80);
        tail = tail->next;
        SetColor(14, 0);//��ԭ��ɫ
        //�����ߵĳ�ʼ������
        length++;
    }

    //��ʼ��oldTail
    oldTail = (Node *)malloc(sizeof(Node));

    //����ReIndex ��Indexָ�����ڶ�����β
    ReIndex();

}

//��ʼ��ʳ��
void InitFood(void)
{
    RandomFood();
}

//�������в���
void ReInitAll(void)
{
    point = 0;
    level = 1;
    itval = 500;

    direction = 'r'; //��ʼ���� Ϊ��
    length = 0; //�ߵĳ���
    eatfood = 0; //�����ж��Ƿ�Ե�ʳ��
}

//����ڵ�ͼ������ʳ�� ������ǽ�� ����������
void RandomFood(void)
{
    int flag = 0;//����flag����ͳ��ʳ�������Ƿ���������ͬ ��ͬ���1 ������ߵĳ�����Ƚ�
    int randX, randY;
    srand((unsigned int)time(NULL));

    //���ɲ��������غϵ�ʳ��
    Node *pointer; //һ����ָ��

    while(flag < length) //flag���߳��ȱȽ� ���С���߳� ˵���в�λ��֮�غ� ��������ʳ��
    {
        //ע��ָ�����Ҳ������
        pointer = head; //ͷָ���ַ����һ����ָ��
        flag = 0;//����flag

        randX = (rand() % (windowY - 2) + 1) * 2; //2-68֮���ż��
        randY = rand() % (windowY - 2) + 1; //1-34֮�����


        while(pointer != NULL)
        {
            if(randX != pointer->x && randY != pointer->y)//���ʳ�������뵱ǰ�ߵĲ������겻ͬ
            {
                flag++;
            }
            pointer = pointer->next;
        }

    }

    //�ڵ�ͼ�ϴ�ӡʳ��
    SetPos(randX, randY);
    RandomColor();
    printf("��");
    ResetColor();
    //�����긳��ʳ������
    food[0] = randX;
    food[1] = randY;

}

//ʳ�������ɫ����
void RandomColor(void)
{
    int randNum;
    srand((unsigned int)time(NULL));
    randNum = rand() % 14 + 1; //1-14�����
    SetColor(randNum, 0);
}

//��ԭ��ɫ
void ResetColor(void)
{
    SetColor(10, 0);
}

//���´�ӡ��
void RePrintSnake(void)
{

    //��ͷ
    Node *newHead;
    newHead = (Node *)malloc(sizeof(Node));
    newHead->x = head->x + XyChange('x'); //���ú����ı�����
    newHead->y = head->y + XyChange('y');
    newHead->next = head; //�����µ�ͷ��ָ��ɵ�ͷ
    head = newHead; //headָ��ָ����ͷ

    //�����ؾ�β ���ӡ��ͷ ���ȴ�ӡͷ������β��ͷ�˶���β��ʱ�ᱻ�ڸ�
    //���ؾ�β index->next ����β
    SetPos(index->next->x, index->next->y);
    printf("  ");

    //��ӡ��ͷ
    SetPos(head->x, head->y);

    SetColor(12, 0);//��һ�ε���ͷҪΪ��ɫ
    printf("%c%c", 0xa8, 0x80);
    SetPos(head->next->x, head->next->y);

    SetColor(14, 0);//�ɵ���ͷ��ԭΪ��ɫ
    printf("%c%c", 0xa8, 0x80);


    //����β��ֵ
    oldTail->x = index->next->x;
    oldTail->y = index->next->y;
    oldTail->next = NULL;
    //ȥβ �ͷ������е�β ����index���¶�λ���ڶ���β
    free(index->next);
    index->next = NULL;
    ReIndex();
}

//�ض����α�ָ��
void ReIndex(void)
{
    index = head;
    while(index->next->next != NULL) //ֱ��ȡ���ڶ�����β
    {
        index = index->next;
    }
}

//��Ϸ��������
void Control(void)
{
    while(1)
    {
        //��ʼ������ߺ�ʳ��
        Cls();
        InitItface();
        InitSnake(4, 4); //ע�ⲻҪ��������Խ��
        InitFood(); //ʳ���ʼ��

        //��ʼ��ϷΪ��ͣ״̬
        Pause();

        while(1)
        {
            //����ʳ��flag
            eatfood = 0;
            //�첽��������
            if(GetAsyncKeyState(VK_UP) && direction != 'd') //��Ϊ��ʱ����
            {
                direction = 'u';
            }
            else if(GetAsyncKeyState(VK_DOWN) && direction != 'u') //��Ϊ��ʱ����
            {
                direction = 'd';
            }
            else if(GetAsyncKeyState(VK_LEFT) && direction != 'r') //��Ϊ��ʱ����
            {
                direction = 'l';
            }
            else if(GetAsyncKeyState(VK_RIGHT) && direction != 'l') //��Ϊ��ʱ����
            {
                direction = 'r';
            }
            else if(GetAsyncKeyState(VK_SPACE)) //������˿ո����ͣ
            {
                Pause();
            }
            else if(GetAsyncKeyState(VK_ESCAPE))
            {
                exit(0);
            }
            RePrintSnake();

            //��ӡ���ߺ��������������ж�
            if(TouchWall() || TouchSelf())
            {
                GameOver();
                break;
            }

            //����Ե�ʳ������������β���������� ���ӷ���  �ж�������Ϸ���� �������µ�ʳ��
            if(EatFood())
            {
                eatfood = 1;
                ResetColor();//��ԭ��ɫ
                TailGain(); //����β������
                GainPoint(); //���ӷ���
                if(point % 50 == 0)//��һ�ν���ʱ�����Ѿ���10 ���Բ������
                {
                    GainLevel();
                }
                RandomFood(); //�������ʳ��
            }

            //��Ϸ��ʱ��
            Sleep(itval);

            //�Ե�ʳ�� �ƶ������ӡ��β��
            if(eatfood)
            {
                PrintTail();

            }

        }
    }
}

//x y����ı�
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

//����ǽ��
int TouchWall(void)
{
    int state = 0;
    if(head->x <= 0 || head->x >= (windowY - 1) * 2 || head->y <= 0 || head->y >= windowY - 1)
    {
        state = 1;
    }
    return state;
}

//�����Լ�
int TouchSelf(void)
{
    int state = 0;
    //����һ����ָ��
    Node *pointer;
    //���ͷ�ͳ�ͷ�����κ�һ������������ͬ ���ж�����
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

//ͷ���Ƿ�Ե�ʳ��
int EatFood(void)
{
    int state = 0;
    if(head->x == food[0] && head->y == food[1])
    {
        length++; //�Ե��ߵĳ��ȼ�1
        state = 1;
    }
    return state;
}

//β����������
void TailGain(void)
{
    //β�����ӽڵ�����
    Node *newTail;
    newTail = (Node *)malloc(sizeof(Node));
    newTail->x = oldTail->x;
    newTail->y = oldTail->y;
    newTail->next = NULL;
    index->next->next = newTail;

    //�ض����α�ָ��
    ReIndex();
}

//�Ե�ʳ�� ��ӡ��β��
void PrintTail(void)
{
    SetPos(oldTail->x, oldTail->y);
    printf("%c%c", 0xa8, 0x80);
}

//��ʼ��Ϸ
void StartGame(void)
{
    SetPos(44, 14);
    printf("��ʼ��Ϸ");
    SetPos(40, 16);
    system("pause");
}

//��Ϸ����
void GameOver(void)
{
    //����������������ӡ����
    Cls();
    ResetColor();
    SetPos(46, 14);
    printf("game over");
    SetPos(40, 18);
    printf("�������ˣ��÷�Ϊ��%d", point);
    SetPos(40, 19);
    system("pause");
    ReInitAll();
}

//��ͣ��Ϸ
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

//���ӷ���
void GainPoint(void)
{
    point += 10;
    SetPos(78, 8);
    printf("�÷֣�%-3d", point);
}

//���Ӽ��� �ٶ�
void GainLevel(void)
{
    if(itval > 50)
    {
        level++;
        SetPos(78, 10);
        printf("����%-3d", level);

        itval -= 50;
    }
}
