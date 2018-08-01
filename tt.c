#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>

#define HEIGHT 10
#define WIDTH  20

#define CTRL_C 3
#define QIANGTUI 0
#define ZHENGCHANG 1

#define WINW 62
#define TYPEWINH   16
#define STATUSWINH 3
#define LINES (TYPEWINH/2-1)
#define COUNTS (WINW/5*5)

void mvprinthanzi(int y,int x,int fontmatrix[HEIGHT][WIDTH])
{
  
  int i,j;

  for(i=0;i<HEIGHT;i++)
    {
      for(j=0;j<WIDTH;j++)
	{
	  if(fontmatrix[i][j]==1)  attron(A_REVERSE);
	  else   attroff(A_REVERSE);
	  mvprintw(y+i,x+j," ");
	}

    }
  attroff(A_REVERSE);
}

void printmenu()
{
  int  row,col;
  int  da[HEIGHT][WIDTH]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			  {0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			  {0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0},
			  {0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0},
			  {0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
			  {0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0},
			  {0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
			  {0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
			  {0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
			  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
  
  int  zi[HEIGHT][WIDTH]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			  {0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
			  {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
			  {0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0},
			  {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
			  {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
			  {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
			  {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
			  {0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
			  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

  char menu[][40]={"***欢迎使用tt.c!!!***\n",
		   "",
		   "",
		   "",
		   "1,0123456789",
		   "2,asdfghjkl",
		   "3,zxcvbnm",
		   "4,qwertyuiop",
		   "5,`-=[]\\;',./",
		   "6,~!@#$%^&*()_+{}|:\"<>?",
		   "7,ABCDEFGHIJKLMNOPQRSTUVWXYZ",
		   "8,退出",
		    "选择： "};

  clear();
  getmaxyx(stdscr,row,col);
  attron(COLOR_PAIR(3));
  mvprinthanzi(row/2-HEIGHT,2,da);
  attroff(COLOR_PAIR(3));
  attron(COLOR_PAIR(2));
  mvprinthanzi(row/2,2,zi);
  attroff(COLOR_PAIR(2));  
  
  attron(A_BOLD|COLOR_PAIR(3));
  for(int i=0;i<13;i++) mvprintw(row/2-5+i,WIDTH+5,"%s",menu[i]);
  
  attroff(COLOR_PAIR(3));   
}


int practice(int select)
{
  time_t start,end;
  int hour,minute,second;
  int t;
  WINDOW *typewin;
  WINDOW *statuswin;
  int status=ZHENGCHANG;
  char strtotype[LINES][COUNTS];
  char usertyped[LINES][COUNTS];
  char charset[][40]={"0123456789",
	  	      "asdfghjkl",
		      "zxcvbnm",
		      "qwertyuiop",
		      "`-=[]\\;',./",
		      "~!@#$%^&*()_+{}|:\"<>?",
		      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"};

  int a;
  int ch;
  int zq,zql,zong;
  int i,j;
  
  for(int i=0;i<LINES;i++)
    {
      for(int j=0;j<COUNTS;j++)
	{
	  if(j%5==4) strtotype[i][j]=' ';
	  else
	    {
	      a=rand()%strlen(charset[select-1]);
	      strtotype[i][j]=charset[select-1][a];
	    }
	}
      strtotype[i][COUNTS-1]=0;
    }
  
  clear();
  refresh();
  typewin = newwin(TYPEWINH,WINW,5,10);
  keypad(typewin,TRUE);
  wtimeout(typewin,500);
  statuswin = newwin(STATUSWINH,WINW,5+1+TYPEWINH,10);
  wattron(typewin,COLOR_PAIR(2));
  box(typewin,'*','+');
  wattroff(typewin,COLOR_PAIR(2));
  box(statuswin,0,0);

  mvprintw(3,27,"开始练习%d！！！",select);
  refresh();

  for(int i=0;i<LINES;i++)
    {
      mvwprintw(typewin,1+i*2,1,"%s",strtotype[i]);
    }
  start=time(NULL);
  mvwprintw(statuswin,1,1,"时间: 00:00:00    0字/分  正确率:100%%");
  mvwprintw(statuswin,1,45,"按F2返回主菜单");
  
  wrefresh(typewin);
  wrefresh(statuswin);

  i=0;
  j=0;
  wmove(typewin,2,1);
  
  zq=0;
  
  while(1)
    {
      ch=wgetch(typewin);
      end=time(NULL);
      t=difftime(end,start);
      second=t%60;
      minute=t/60;
      minute=minute%60;
      hour=t/3600;
      zong=i*(COUNTS-1)+j;
      if(zong==0) zql=100;
      else zql=zq*100/zong;
      mvwprintw(statuswin,1,1,"时间：%02d:%02d:%02d  %3d字/分  正确率：%3d%%",hour,minute,second,zong*60/(t+1),zql);
      wmove(typewin,2+i*2,j+1);
      wrefresh(statuswin);
      if(isprint(ch))
	{
	  usertyped[i][j]=ch;
	  if(ch==strtotype[i][j])
	    {
	      wattroff(typewin,COLOR_PAIR(4));
	      zq++;
	    }
	  else if (isprint(ch)) wattron(typewin,COLOR_PAIR(4));
	  mvwprintw(typewin,i*2+2,j+1,"%c",ch);
	  if(j==COUNTS-2)
	    {
	      i++;
	      j=0;
	      wmove(typewin,i*2+2,1);
	      if(i==LINES)
		{
		  wclear(typewin);
		  clear();
		  refresh();
		  wattroff(typewin,COLOR_PAIR(4));
		  mvwprintw(typewin,TYPEWINH/2-1,1,"恭喜你完成任务！！！你的成绩是：");
		  mvwprintw(typewin,TYPEWINH/2+2,1,"时间：%02d:%02d:%02d  %3d字/分  正确率：%3d%%",hour,minute,second,zong*60/(t+1),zql);
		  wrefresh(typewin);
		  sleep(5);
		  break;
		}  
	    }
	  else j++;
	}
      else
	{

	  if(ch==KEY_BACKSPACE)
	    {
	      if(j!=0)
		{
		  if(usertyped[i][j-1]==strtotype[i][j-1]) zq--;
		  mvwprintw(typewin,i*2+2,j," ");
		  wmove(typewin,i*2+2,j);
		  j--;
		}
	    }
	  else if(ch==KEY_F(2))
	    {
	      break;
	    }
	  else if(ch==CTRL_C)
	    {
	      status=QIANGTUI;
	      break;
	    }
	}
    }
  delwin(typewin);
  delwin(statuswin);
  return status;
}

int main(void)
{
  int myselect,exitstatus;
  srand((unsigned int)time(NULL));
  setlocale(LC_ALL,"");
  initscr();
  raw();
  //halfdelay(5);
  noecho();
  start_color();
  init_pair(2,COLOR_BLACK,COLOR_BLUE|COLOR_RED);
  init_pair(3,COLOR_GREEN,COLOR_BLACK);
  init_pair(4,COLOR_RED,COLOR_BLACK);

  do{
    
    printmenu();
    echo();
    scanw("%d",&myselect);
    noecho();
    if(myselect==8) break;
    else if(myselect>0&&myselect<8)
    {
      exitstatus=practice(myselect);
      if(exitstatus==QIANGTUI) break;
    }
  }while(1);
  
  endwin();

  return 0;
}
