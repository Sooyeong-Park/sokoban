#include <stdio.h>
#include <termio.h>
#include <stdlib.h>

#define LEVEL 5
#define X 30
#define Y 30

char username[11];
char map[LEVEL][Y][X];
int level=-1;
int player_x[LEVEL], player_y[LEVEL];
int boxCount[LEVEL]={0}, storeCount[LEVEL]={0};
char undo;
char command;

void User(void);
int getch(void);			// 명령 입력
void Display_help(void);	// 명령 d
void Command(void);
void read_map(void);
void clear_map(void);
void write_map(void);
void player_move(void); 

int main()
{
	read_map();
	write_map();
	Command();

	return 0;
}

void User()	// player 이름 받기
{
	printf("Start....\n");

	// username을 공백으로 초기화
	for (int i = 0; i < 11; i++)
		username[i] = ' ';

	printf("input Name : ");
	scanf("%s", username);
}

void read_map() // map.txt에서 맵 배열에 넣기
{
	FILE *write_map;
	write_map=fopen("map.txt","r");
	
	char c;

	int x=0, y=0;
	
	while ((c = getc(write_map)) != EOF){
		if (c=='e'){
			break;
		}
		if ((49 <= c)&&(c <= 53)){	// c가 1~5 사이의 수면 레벨 증가
			level++;
			y=-1;
			continue;
		}

		if (c=='@'){	// 플레이어 위치 저장
			player_x[level]=x;
			player_y[level]=y;
		}

		if (c=='$'){	// 상자 수 세기
			boxCount[level]++;
		}

		if (c=='O'){	// 보관 장소 수 세기
			storeCount[level]++;
		}

		if (c=='\n'){
			y++;
			x=0;
		}

		else {
			map[level][y][x] = c;
			x++;
		}
	}

	// $개수와 O개수가 다르면 종료
	for (int i = 0; i < LEVEL; i++){
		if (boxCount[i] != storeCount[i]){
			printf("오류| 박스의 개수와 보관장소의 개수가 다릅니다!");
			return;
		}
	}

	level = 0;

	fclose(write_map);
}

void clear_map()	// 맵 지우기
{
	system("clear");
}

void write_map()	// 맵 그리기
{
	for (int i = 0; i < Y; i++){
		printf("%s\n", map[level][i]);
	}
}

void Command()
{

	while (command != 'e')
	{
		command = getch();

		switch (command){
			case 'h' :	// 왼쪽이동
			case 'j' :	// 아래이동
			case 'k' :	// 위이동
			case 'l' :	// 오른쪽이동
				player_move();
				break;
			case 'u' :	// 되돌리기(undo)

				break;
			case 'r' :	// 현재 맵 다시 시작, 움직임 횟수 유지

				break;
			case 'n' :	// 첫째 맵부터 다시 시작, 움직임 횟수 삭제
				
				break;
			case 'e' :	// 게임 종료, 종료 전 저장

				break;
			case 's' :	// 현재 상태 sokoban이라는 파일에 저장

				break;
			case 'f' :	// sokoban 파일에서 불러오기

				break;
			case 'd' :	// 명령 내용 출력
				Display_help();
				break;
		}
	}
}

int getch(void){
	int ch;

	struct termios buf;
	struct termios save;

	tcgetattr(0, &save);
	buf = save;

	buf.c_lflag&=~(ICANON|ECHO);
	buf.c_cc[VMIN]=1;
	buf.c_cc[VTIME]=0;

	tcsetattr(0, TCSAFLUSH, &buf);

	ch=getchar();
	tcsetattr(0, TCSAFLUSH, &save);
	
	return ch;
}

void player_move(){	// 플레이어 상하좌우 이동
	switch (command){
		case 'h' : // 왼쪽 이동
			map[level][player_y[level]][player_x[level]--] = '.';
			map[level][player_y[level]][player_x[level]] = '@';
			undo = command;
			break;
		case 'j' : // 아래 이동
			map[level][player_y[level]++][player_x[level]] = '.';
			map[level][player_y[level]][player_x[level]] = '@';
			undo = command;
			break;
		case 'k' : // 위 이동
			map[level][player_y[level]--][player_x[level]] = '.';
			map[level][player_y[level]][player_x[level]] = '@';
			undo = command;
			break;
		case 'l' : // 오른쪽 이동
			map[level][player_y[level]][player_x[level]++] = '.';
			map[level][player_y[level]][player_x[level]] = '@';
			undo = command;
			break;
	}

	clear_map();
	write_map();
}

void Display_help(void){
	printf("- h(왼쪽), j(아래), k(위), l(오른쪽)\n");
	printf("- u(undo)\n");
	printf("- r(replay)\n");
	printf("- n(new)\n");
	printf("- e(exit)\n");
	printf("- s(save)\n");
	printf("- f(file load)\n");
	printf("- d(display help)\n");
	printf("- t(top)\n");
}

