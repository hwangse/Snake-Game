#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <termios.h>
#include <sys/time.h>
#include <unistd.h>

const int N = 20, M = 20; // n, m seting 
int time_out, cx, cy, game_over,move=0,width=0,length=0,score=0,test=0;
char boardset[100][100];
int a[100][2];//snake's location 
int X,Y,x,y;


/* other user function */

int square_ten(int a){
	if (a==0)
		return 1;
	else 
		return 10*square_ten(a-1);
}

/* clear screen and draw board */
void drawBoard(int n, int m) {
     int i=0,j=0;

    system("clear"); // clear screen
	puts("");
	printf("  Snake Game |Score : %d|\n",score);
	puts("");
	/* user code */
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
			printf("%c ",boardset[i][j]);
		printf("\n");
	}

}


/* make random position x, y (call by reference) */
/* (x, y) empty place, 1 <= x <= n, 1 <= y <= m */
void makeFood(int n, int m, int *x, int *y) {
	/* user code */
	srand(time(NULL));
	*x=rand()%(m-2)+1;
	*y=rand()%(n-2)+1;
	if(boardset[*y][*x]=='@' || boardset[*y][*x]=='*')
	{
		*x=rand()%(m-2)+1;
		*y=rand()%(n-2)+1;
	}
	else
		boardset[*y][*x]='F';
}

/* init n x m board and other global variables */
void init(int n, int m) {

	cx = m / 2; /* start x */
	cy = n / 2; /* start y */	

	game_over = 0;
	time_out = 0;

	X=cx-1,Y=cy-1;

	/* user code */
	boardset[cy-1][cx-1]='@';
	srand(time(NULL));
	x=rand()%(m-2)+1;
	y=rand()%(n-2)+1;

	if(x==cx-1 && y==cy-1)
		makeFood(n,m,&x,&y);
	else
		boardset[y][x]='F';
}

/* call by ualarm( microsec ) */

int temp_do_count = 0;

void Do(int sig) {

	int i;

		/* user code */
	if(move==0)//Right 
	{	
		a[score][0]=X,a[score][1]=Y;
		if(boardset[Y][X+1]=='F')
		{
			boardset[Y][X+1]='@';
			a[score+1][0]=X+1,a[score+1][1]=Y;
			makeFood(length,width,&x,&y);
			score++;
		}
		else if(boardset[Y][X+1]=='@')
			game_over=1;
		else if(boardset[Y][X+1]=='*')
			game_over=1;
		else{
			boardset[Y][X+1]='@';
			a[score+1][0]=X+1,a[score+1][1]=Y;
			boardset[a[0][1]][a[0][0]]=' ';
		
		for(i=0;i<score+1;i++)
		{
			a[i][0]=a[i+1][0];
			a[i][1]=a[i+1][1];
		}
		}
		X++;

	}
	else if(move==1)//Up
	{
		a[score][0]=X,a[score][1]=Y;
		if(boardset[Y-1][X]=='F')
		{
			boardset[Y-1][X]='@';
			a[score+1][0]=X,a[score+1][1]=Y-1;
			makeFood(length,width,&x,&y);
			score++;
		}
		else if(boardset[Y-1][X]=='@')
			game_over=1;
		else if(boardset[Y-1][X]=='*')
			game_over=1;
		else{
			boardset[Y-1][X]='@';
			a[score+1][0]=X,a[score+1][1]=Y-1;
			boardset[a[0][1]][a[0][0]]=' ';
		
		for(i=0;i<score+1;i++)
		{
			a[i][0]=a[i+1][0];
			a[i][1]=a[i+1][1];
		}
		}
		Y--;
	}
	else if(move==2)//Left
	{
		a[score][0]=X,a[score][1]=Y;
		if(boardset[Y][X-1]=='F')
		{
			boardset[Y][X-1]='@';
			a[score+1][0]=X-1,a[score+1][1]=Y;
			makeFood(length,width,&x,&y);
			score++;
		}
		else if(boardset[Y][X-1]=='@')
			game_over=1;
		else if(boardset[Y][X-1]=='*')
			game_over=1;
		else{
			boardset[Y][X-1]='@';
			a[score+1][0]=X-1,a[score+1][1]=Y;
			boardset[a[0][1]][a[0][0]]=' ';
		
		for(i=0;i<score+1;i++)
		{
			a[i][0]=a[i+1][0];
			a[i][1]=a[i+1][1];
		}
		}
		X--;
	}
	else//Down
	{
		a[score][0]=X,a[score][1]=Y;
		if(boardset[Y+1][X]=='F')
		{
			boardset[Y+1][X]='@';
			a[score+1][0]=X,a[score+1][1]=Y+1;
			makeFood(length,width,&x,&y);
			score++;
		}
		else if(boardset[Y+1][X]=='@')
			game_over=1;
		else if(boardset[Y+1][X]=='*')
			game_over=1;
		else{
			boardset[Y+1][X]='@';
			a[score+1][0]=X,a[score+1][1]=Y+1;
			boardset[a[0][1]][a[0][0]]=' ';
		
		for(i=0;i<score+1;i++)
		{
			a[i][0]=a[i+1][0];
			a[i][1]=a[i+1][1];
		}
		}
		Y++;
	}


		
	// if game over
	//   game_over = 0;

	/* time out variable upadate */
	time_out = 0;
}

/* get key charater directly */
int getch(void)
{
	char   ch;
	int   error;
	static struct termios Otty, Ntty;

	fflush(stdout);
	tcgetattr(0, &Otty);
	Ntty = Otty;
	Ntty.c_iflag = 0;
	Ntty.c_oflag = 0;
	Ntty.c_lflag &= ~ICANON;
#if 1
	Ntty.c_lflag &= ~ECHO;
#else
	Ntty.c_lflag |= ECHO;
#endif
	Ntty.c_cc[VMIN] = 0;
	Ntty.c_cc[VTIME] = 1;

#if 1
#define FLAG TCSAFLUSH
#else
#define FLAG TCSANOW
#endif

	if (0 == (error = tcsetattr(0, FLAG, &Ntty)))
	{
		error = read(0, &ch, 1);
		error += tcsetattr(0, FLAG, &Otty);
	}

	return (error == 1 ? (int)ch : -1);
}


int GetCommand() {
	int ch = getch();

	switch (ch)
	{
	case 'd':
	case 'D': return 0; /* RIGHT */
	case 'w':
	case 'W': return 1; /* UP */
	case 'a':
	case 'A': return 2; /* LEFT */
	case 's':
	case 'S': return 3; /* DOWN */
	default: return -1; /* OTHER KEY */
	}

	return -1;
}


int main() {
	int n = N, m = M,command;
	int a=0;
	int i=0,j=0,num1=0,num2=0,enter=0;
	FILE *open=fopen("board.txt","r");
	FILE *write=fopen("copy.txt","w");
	char wid[4],leng[4],res,temp;

	fscanf(open,"%s %s\n",leng,wid);//determine int width and int length

	for(i=0;i<3;i++)
	{
		if(leng[i]=='\0')
			break;
		else
			num1++;
	}
	for(i=0;i<num1;i++)
		length += (leng[i]-48)*square_ten(num1-1-i);

	for(i=0;i<3;i++)
	{
		if(wid[i]=='\0')
			break;
		else
			num2++;
	}
	for(i=0;i<num2;i++)
		width += (wid[i]-48)*square_ten(num2-1-i);

	fclose(open);

	fopen("board.txt","r"); // set array's component

	while(1){
		res=fgetc(open);
		if(res==EOF) break;
		if(enter==0)
		{
			if(res=='\n') enter++;
		}
		else
		{
			if(res=='1'|| res=='0')
			fprintf(write,"%c",res);
		}

	}
	fclose(write);
	fopen("copy.txt","r");

	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			temp=fgetc(write);
			if(temp=='1')
				boardset[i][j]='*';
			else if(temp=='0')
				boardset[i][j]=' ';

		}
	}

	fclose(write);
	
	init(length,width);

	srand(time(NULL));
	static struct sigaction act, oact;

	/* if call alarm or ualarm function, then call Do function/ */
	act.sa_handler = Do; /*(signal.h header*/
	sigaction(SIGALRM,&act,NULL);

	do {

		if (time_out == 0) {
			//alarm(1); // 1 second
			ualarm(300000, 0); // 0.3 second ( unistd.h header )
			time_out = 1;
		}

		command = GetCommand();
		
		/* user code */
		if(command != -1)
		{
			test=command;
			if(test-move==2 || move-test==2)
			{}
			else
				move=test;
		}

		drawBoard(length,width);
	
	} while (!game_over);

	system("clear");
	printf("game over!\n");
}

