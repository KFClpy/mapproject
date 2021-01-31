#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<stack>
#include<easyx.h>
#include<graphics.h>
#include<conio.h>
#include<tchar.h>
using namespace std;
#define maxint 32767
IMAGE img1;
typedef struct {
	char vexs[25][100];
	float arcs[25][25];
	int vexnum, arcnum;
}AMGraph;
RECT r = { 760, 50, 1060, 600 };
RECT r1 = { 760,115,1060,600 };
RECT r2 = { 760,350,1060,600 };
void CharToTchar(const char* _char, TCHAR* tchar)
{
	int iLength;
	iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
}
int Locatevex(AMGraph G, char v[])
{
	int i;
	for (i = 0; i < G.vexnum; i++)
	{
		if (strcmp(G.vexs[i], v) == 0)break;
	}
	return i;
}
int CreateUDN(AMGraph& G,FILE *in)
{
	float w;
	char c1[100], c2[100];
	int i, j, k; 
	fscanf(in,"%d %d", &G.vexnum, &G.arcnum);
	for (i = 0; i < G.vexnum; i++)
	{
		fscanf(in,"%s", G.vexs[i]);
	}
	for (i = 0; i < G.vexnum; i++)
		for (j = 0; j < G.vexnum; j++)
		{
			if (i != j)G.arcs[i][j] = maxint; else G.arcs[i][j] = 0;
		}
	for (k = 0; k < G.arcnum; k++)
	{
		fscanf(in,"%s", c1);
		fscanf(in,"%s", c2);
		fscanf(in,"%f", &w);
		i = Locatevex(G, c1);
		j = Locatevex(G, c2);
		G.arcs[i][j] = w;
		G.arcs[j][i] = G.arcs[i][j];
	}
	return 1;
}
void Floyd(AMGraph G, int path[25][25],float shortest[25][25])
{
	int k = 0, i = 0, j = 0;
	for (i = 0; i < G.vexnum; i++)
		for (j = 0; j < G.vexnum; j++)
			shortest[i][j] = G.arcs[i][j];
	for (i = 0; i < G.vexnum; i++)
		for (j = 0; j < G.vexnum; j++)
		{
			if (shortest[i][j] == maxint)path[i][j] = -1;
			else path[i][j] = j;
		}
	for(k=0;k<G.vexnum;k++)
		for(i=0;i<G.vexnum;i++)
			for(j=0;j<G.vexnum;j++)
				if (shortest[i][j] > shortest[i][k] + shortest[k][j]&&shortest[i][k]<maxint&&shortest[k][j]<maxint) {
					shortest[i][j] = shortest[i][k] + shortest[k][j];
					path[i][j] = path[i][k];
				}
}
void change_num(AMGraph &un,float speed)
{
	int i = 0;
	int j = 0;
	for(i=0;i<un.vexnum;i++)
		for (j = 0; j < un.vexnum; j++)
		{
			if (un.arcs[i][j] == maxint)continue;
			else un.arcs[i][j] = un.arcs[i][j] / speed;
		}
}
void set_new_web(AMGraph& web, AMGraph un, AMGraph bus)
{
	int i;
	for (i = 0; i < un.vexnum; i++)
		strcpy(web.vexs[i], un.vexs[i]);
	web.vexnum = un.vexnum;
	int j;
	web.arcnum = un.arcnum;
	for (i = 0; i < web.vexnum; i++)
		for (j = 0; j < web.vexnum; j++)
		{
			if (i != j)web.arcs[i][j] = maxint; else web.arcs[i][j] = 0;
		}

	for(i=0;i<un.vexnum;i++)
		for (j = 0; j < un.vexnum; j++)
		{
			if (un.arcs[i][j] == 0 || bus.arcs[i][j] == 0||un.arcs[i][j]==bus.arcs[i][j])continue;
			if (un.arcs[i][j] < bus.arcs[i][j]) {
				web.arcs[i][j] = un.arcs[i][j]; web.arcs[j][i] = web.arcs[i][j];
			}
			else {
				web.arcs[i][j] = bus.arcs[i][j]; web.arcs[j][i] = web.arcs[i][j];
			}
		}
}
void Way(AMGraph U, AMGraph B, int way[25][25])
{
	int i;
	int j;
	for(i=0;i<U.vexnum;i++)
		for (j = 0; j < U.vexnum; j++)
		{
			if (i == j)way[i][j] = 0;
			if (U.arcs[i][j] != 32767&& U.arcs[i][j] != 0)way[i][j] += 1;
			if (B.arcs[i][j] != 32767&&B.arcs[i][j] != 0)way[i][j] += 2;
		}
	for (i = 0; i < U.vexnum; i++)
	{
		for (j = 0; j < U.vexnum; j++)
		{
			printf("%d ", way[i][j]);
		}
		printf("\n");
	}
}
void tominute(AMGraph& G)
{
	int i, j;
	for(i=0;i<G.vexnum;i++)
		for (j = 0; j < G.vexnum; j++)
		{
			G.arcs[i][j] = G.arcs[i][j] / 120;
		}
}
int done[25];
int path_num = 0;
void swap_way(int dway[10000],int slength[10000],int flag)
{
	int i = 0;
	int length = 0;
	for (i = 0; i < 100; i++)
	{
		
		if (dway[i] < 0)break;
		length++;
	}
	slength[flag] = length;
	int temp;
	for (i = 0; i < length/2; i++)
	{
		temp = dway[i];
		dway[i] = dway[length - 1 - i];
		dway[length - 1 - i] = temp;
	}
}
int slength[10000];
void save_way(stack<int>q,int sway[10000][10000])
{
	int i = 0;
	while (!q.empty()) {
		sway[path_num][i] = q.top();
		q.pop();
		i++;
	}
	swap_way(sway[path_num],slength,path_num);
}
void findway(int start,int end,int way[25][25],AMGraph G,int sway[10000][10000],stack<int>q)
{
	int i;
	for (i = 0; i < G.vexnum; i++)
	{
		if (start==end) {
			save_way(q,sway); path_num++;
			break;
			}
		if (way[start][i] != 0&&done[i]!=1) {
			q.push(i);
			done[start] = 1;
			findway(i, end, way,G,sway,q);
			done[start] = 0;
			q.pop();
		}
	}
}
void set_times(int sway[10000][10000], int times[10000], int way[25][25], int start, int end)
{
	int i, j;
	int front = start;
	int goway;
	for (i = 0; i < path_num; i++)
	{
		for (j = 0;; j++)
		{
			if (front == end) {
				front = start; break;
			}
			if (j != 0) {
				if (goway != way[front][sway[i][j]] && goway != 3 && way[front][sway[i][j]] != 3)times[i]++;
			}
			goway = way[front][sway[i][j]];
			front = sway[i][j];
		}
	}
}
void print_lesttimesway(int times[10000], int sway[10000][10000], int start, int end,AMGraph G)
{
	int i;
	int shortest=times[0];
	for (i = 0; i < path_num; i++)
	{
		if (shortest > times[i])shortest = times[i];
	}
	int j;
	int lest_times = 10000;
	for (i = 0; i < path_num; i++)
	{
		if (times[i] == shortest)
		{
			if (lest_times > slength[i])lest_times = slength[i];
		}
	}
	char theway[500] = { "" };
	for (i = 0; i < path_num; i++)
	{
		if (times[i] == shortest&&lest_times==slength[i]) {
			printf("%s-->", G.vexs[start]);
			strcat(theway, G.vexs[start]);
			strcat(theway, "-->");
			for (j = 0;; j++)
			{
				if (sway[i][j] == end) {
					printf("%s", G.vexs[end]); strcat(theway, G.vexs[end]); break;
				}
				printf("%s-->", G.vexs[sway[i][j]]);
				strcat(theway, G.vexs[sway[i][j]]);
				strcat(theway, "-->");
			}
			char pices[50];
			itoa(shortest, pices, 10);
			strcat(theway, "换乘");
			strcat(theway, pices);
			strcat(theway, "次");
			printf(" 换乘%d次", shortest);
			printf("\n");
			strcat(theway, "\n");
			
		}
	}
	settextstyle(26, 0, _T("黑体"));
	TCHAR ctr[500];
	setbkmode(TRANSPARENT);
	CharToTchar(theway, ctr);
	drawtext(ctr, &r2, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_EDITCONTROL);
	
}
void set_price(int sway[10000][10000], int prices[10000], int way[25][25], int start, int end)
{
	int i, j;
	int front = start;
	int goway;
	int underway = 0;
	for (i = 0; i < path_num; i++)
	{
		for (j = 0;; j++)
		{
			if (sway[i][0]==end)
			{
				if (goway == 1||goway==3)prices[i] = prices[i] + 2;
				if (goway == 2)
				{
					if (underway < 7 && underway >= 0)prices[i] += 3;
					else if (underway >= 7 && underway < 14)prices[i] += 5;
					else prices[i] += 8;
				}
				front = start;
				break;

			}
			if (j != 0)
			{
				if (goway != way[front][sway[i][j]] && goway != 3 && way[front][sway[i][j]] != 3)
				{
					if (goway == 1)prices[i] = prices[i] + 2;
					if (goway == 2)
					{
						if (underway < 7 && underway >= 0)prices[i] += 3;
						else if (underway >= 7 && underway < 14)prices[i] += 5;
						else prices[i] += 8;
						underway = 0;
					}
				}
			}
			if (front == end)
			{
				front = start; break;
			}
			if (way[front][sway[i][j]] == 2)underway++;
			goway = way[front][sway[i][j]];
			front = sway[i][j];

		}
	}
}
void print_lestpricesway(int prices[10000], int sway[10000][10000], int start, int end, AMGraph G)
{
	int i;
	int shortest = prices[0];
	for (i = 0; i < path_num; i++)
	{
		if (shortest > prices[i])shortest = prices[i];
	}
	int j;
	int lest_times = 10000;
	for (i = 0; i < path_num; i++)
	{
		if (prices[i] == shortest) {
			if (slength[i] < lest_times)lest_times = slength[i];
		}
	}
	char theway[500] = { "" };
	for (i = 0; i < path_num; i++)
	{
		if (prices[i] == shortest&&slength[i]==lest_times) {
			strcat(theway, G.vexs[start]);
			strcat(theway, "-->");
			printf("%s-->", G.vexs[start]);
			for (j = 0;; j++)
			{
				if (sway[i][j] == end) {
					printf("%s", G.vexs[end]); strcat(theway, G.vexs[end]); break;
				}
				printf("%s-->", G.vexs[sway[i][j]]);
				strcat(theway, G.vexs[sway[i][j]]);
				strcat(theway, "-->");
			}
			char pices[50];
			itoa(prices[i], pices, 10);
			strcat(theway, "花费为");
			strcat(theway, pices);
			strcat(theway, "元");
			printf(" 花费为%d元", prices[i]);
			settextstyle(26, 0, _T("黑体"));
			printf("\n");
			strcat(theway, "\n");
		}
	}
	settextstyle(26, 0, _T("黑体"));
	setbkmode(TRANSPARENT);
	TCHAR ctr[500];
	CharToTchar(theway, ctr);
	drawtext(ctr, &r1, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_EDITCONTROL);
}
int sway[10000][10000];
void init_sway(int sway[10000][10000])
{
	int i;
	int j;
	for (i = 0; i < 10000; i++)
	{
		for (j = 0; j < 10000; j++)
		{
			sway[i][j] = -2;
		}
	}
}
int prices[10000];
int times[10000];
int optimes1 = 0;
int optimes2 = 0;
int optimes3 = 0;
int main()
{
	FILE* in;
	in = fopen("input.txt", "r");
	int i, j;
	float bus_speed, un_speed;
	init_sway(sway);
	AMGraph G;
	AMGraph B;
	AMGraph U;
	CreateUDN(U,in);
	CreateUDN(B,in);
	fscanf(in,"%f %f", &bus_speed, &un_speed);
	change_num(U, un_speed);
	change_num(B, bus_speed);
	set_new_web(G, U, B);
	tominute(G);
	for (i = 0; i < G.vexnum; i++)
	{
		for (j = 0; j < G.vexnum; j++)
		{
			if (G.arcs[i][j] != maxint)
			{
				printf("%f", G.arcs[i][j]);
				printf(" ");
			}
			else {
				printf("∞");
				printf(" ");
			}
		}
		printf("\n");
	}
	printf("\n");
	float shortest[25][25];
	int path[25][25];
	for (i = 0; i < G.vexnum; i++)
		for (j = 0; j < G.vexnum; j++)
			path[i][j] = 0;
	Floyd(G, path, shortest);
	for (i = 0; i < G.vexnum; i++)
	{
		for (j = 0; j < G.vexnum; j++)
		{
			if (shortest[i][j] != maxint)
			{
				printf("%f", shortest[i][j]);
				printf(" ");
			}
			else {
				printf("∞");
				printf(" ");
			}
		}
		printf("\n");
	}
	printf("\n");
	for (i = 0; i < G.vexnum; i++)
		done[i] = 0;
	int way[25][25];
	for (i = 0; i < G.vexnum; i++)
		for (j = 0; j < G.vexnum; j++)
			way[i][j] = 0;
	Way(U, B, way);
	char Start[100];
	char End[100];
	int start, end;
	/*
	start = Locatevex(G, Start);
	end = Locatevex(G, End);
	*/
	/*
	findway(start, end, way, G,sway,q);
	set_times(sway, times, way, start, end);
	set_price(sway, prices, way, start, end);
	print_lesttimesway(times, sway, start, end, G);
	print_lestpricesway(prices, sway, start, end, G);
	int temp = start;
	printf("%s", G.vexs[start]);
	float whole_time = 0;
	int goway = way[temp][path[temp][end]];
	int underway = 0;
	int price = 0;
	int front = start;
	while (temp != end)
	{
		front = path[front][end];
		
			if (goway != way[front][path[front][end]] && goway != 3 && way[front][path[front][end]] != 3)
			{
				if (goway == 1)price = price + 2;
				if (goway == 2)
				{
					if (underway < 7 && underway >= 0)price += 3;
					else if (underway >= 7 && underway < 14)price+= 5;
					else price+= 8;
					underway = 0;
				}
			}
		if (way[front][path[front][end]] == 2)underway++;
		printf("-->");
		printf("%s", G.vexs[path[temp][end]]);
		goway = way[front][path[front][end]];
		whole_time += G.arcs[temp][path[temp][end]];
		temp = path[temp][end];
	}
	if (goway == 1 || goway == 3)price = price+ 2;
	if (goway == 2)
	{
		if (underway < 7 && underway >= 0)price += 3;
		else if (underway >= 7 && underway < 14)price += 5;
		else price += 8;
	}
	printf(" %.0f分钟 花费为%d元", whole_time, price);
	*/
	HWND hwnd;
	hwnd = FindWindow(_T("ConsoleWindowClass"), NULL);
	if (hwnd)
	{
		ShowWindow(hwnd, SW_HIDE);//设置指定窗口的显示状态
	}
	initgraph(1152, 648, EW_SHOWCONSOLE);
	cleardevice();
	loadimage(&img1, _T("D:\\截图\\图形界面.jpg"));
	putimage(0, 0, &img1);
	setlinecolor(BLACK);
	setfillcolor(BLACK);
	fillcircle(350, 63, 5);
	int x0 = 350, y0 = 63;
	fillcircle(182, 117, 5);
	int x1 = 182, y1 = 117;
	fillcircle(163, 225, 5);
	int x2 = 163, y2 = 225;
	fillcircle(110, 362, 5);
	int x3 = 110, y3 = 362;
	fillcircle(233, 495, 5);
	int x4 = 233, y4 = 495;
	fillcircle(403, 475, 5);
	int x5 = 403, y5 = 475;
	fillcircle(460, 380, 5);
	int x6 = 460, y6 = 380;
	fillcircle(485, 253, 5);
	int x7 = 485, y7 = 253;
	fillcircle(468, 158, 5);
	int x8 = 468, y8 = 158;
	fillcircle(300, 113, 5);
	int x9 = 300, y9 = 113;
	fillcircle(290, 149, 5);
	int x10 = 290, y10 = 149;
	fillcircle(268, 180, 5);
	int x11 = 268, y11 = 180;
	fillcircle(375, 122, 5);
	int x12 = 375, y12 = 122;
	fillcircle(412, 150, 5);
	int x13 = 412, y13 = 150;
	fillcircle(418, 200, 5);
	int x14 = 418, y14 = 200;
	fillcircle(448, 223, 5);
	int x15 = 448, y15 = 223;
	fillcircle(425, 270, 5);
	int x16 = 425, y16 = 270;
	fillcircle(412, 398, 5);
	int x17 = 412, y17 = 398;
	fillcircle(278, 240, 5);
	int x18 = 278, y18 = 240;
	fillcircle(268, 307, 5);
	int x19 = 268, y19 = 307;
	fillcircle(250, 365, 5);
	int x20 = 250, y20 = 365;
	fillcircle(225, 400, 5);
	int x21 = 225, y21 = 400;
	fillcircle(343, 378, 5);
	int x22 = 343, y22 = 378;
	MOUSEMSG msg;
	FlushMouseMsgBuffer();
	int step = 1;
	while (true)
	{
		while (MouseHit())
		{
			
			msg = GetMouseMsg();
			if (WM_LBUTTONDOWN == msg.uMsg)
			{
				if (msg.x >= x0 - 5 && msg.x <= x0 + 5 && msg.y >= y0 - 5 && msg.y <= y0 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x0, y0, 5);
						strcpy(Start, "a0");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x0, y0, 5);
						strcpy(End, "a0");
					}
					step++;
				}
				if (msg.x >= x1 - 5 && msg.x <= x1 + 5 && msg.y >= y1 - 5 && msg.y <= y1 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x1, y1, 5);
						strcpy(Start, "a1");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x1, y1, 5);
						strcpy(End, "a1");
					}
					step++;
				}
				if (msg.x >= x2 - 5 && msg.x <= x2 + 5 && msg.y >= y2 - 5 && msg.y <= y2 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x2, y2, 5);
						strcpy(Start, "a2");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x2, y2, 5);
						strcpy(End, "a2");
					}
					step++;
				}
				if (msg.x >= x3 - 5 && msg.x <= x3 + 5 && msg.y >= y3 - 5 && msg.y <= y3 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x3, y3, 5);
						strcpy(Start, "a3");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x3, y3, 5);
						strcpy(End, "a3");
					}
					step++;
				}
				if (msg.x >= x4 - 5 && msg.x <= x4 + 5 && msg.y >= y4 - 5 && msg.y <= y4 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x4, y4, 5);
						strcpy(Start, "a4");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x4, y4, 5);
						strcpy(End, "a4");
					}
					step++;
				}
				if (msg.x >= x5 - 5 && msg.x <= x5 + 5 && msg.y >=y5 - 5 && msg.y <= y5 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x5, y5, 5);
						strcpy(Start, "a5");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x5, y5, 5);
						strcpy(End, "a5");
					}
					step++;
				}
				if (msg.x >= x6 - 5 && msg.x <= x6 + 5 && msg.y >= y6 - 5 && msg.y <= y6 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x6, y6, 5);
						strcpy(Start, "a6");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x6, y6, 5);
						strcpy(End, "a6");
					}
					step++;
				}
				if (msg.x >= x7 - 5 && msg.x <= x7 + 5 && msg.y >= y7 - 5 && msg.y <= y7 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x7, y7, 5);
						strcpy(Start, "a7");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x7, y7, 5);
						strcpy(End, "a7");
					}
					step++;
				}
				if (msg.x >= x8 - 5 && msg.x <= x8 + 5 && msg.y >= y8 - 5 && msg.y <= y8 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x8, y8, 5);
						strcpy(Start, "a8");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x8, y8, 5);
						strcpy(End, "a8");
					}
					step++;
				}
				if (msg.x >= x9 - 5 && msg.x <= x9 + 5 && msg.y >= y9 - 5 && msg.y <= y9 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x9, y9, 5);
						strcpy(Start, "a9");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x9, y9, 5);
						strcpy(End, "a9");
					}
					step++;
				}
				if (msg.x >= x10 - 5 && msg.x <= x10 + 5 && msg.y >= y10 - 5 && msg.y <= y10 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x10, y10, 5);
						strcpy(Start, "a10");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x10, y10, 5);
						strcpy(End, "a10");
					}
					step++;
				}
				if (msg.x >= x11 - 5 && msg.x <= x11 + 5 && msg.y >= y11 - 5 && msg.y <= y11 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x11, y11, 5);
						strcpy(Start, "a11");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x11, y11, 5);
						strcpy(End, "a11");
					}
					step++;
				}
				if (msg.x >= x12 - 5 && msg.x <= x12 + 5 && msg.y >= y12 - 5 && msg.y <= y12 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x12, y12, 5);
						strcpy(Start, "a12");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x12, y12, 5);
						strcpy(End, "a12");
					}
					step++;
				}
				if (msg.x >= x13 - 5 && msg.x <= x13 + 5 && msg.y >= y13 - 5 && msg.y <= y13 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x13, y13, 5);
						strcpy(Start, "a13");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x13, y13, 5);
						strcpy(End, "a13");
					}
					step++;
				}
				if (msg.x >= x14 - 5 && msg.x <= x14 + 5 && msg.y >= y14 - 5 && msg.y <= y14 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x14, y14, 5);
						strcpy(Start, "a14");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x14, y14, 5);
						strcpy(End, "a14");
					}
					step++;
				}
				if (msg.x >= x15 - 5 && msg.x <= x15 + 5 && msg.y >= y15 - 5 && msg.y <= y15 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x15, y15, 5);
						strcpy(Start, "a15");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x15, y15, 5);
						strcpy(End, "a15");
					}
					step++;
				}
				if (msg.x >= x16 - 5 && msg.x <= x16 + 5 && msg.y >= y16 - 5 && msg.y <= y16 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x16, y16, 5);
						strcpy(Start, "a16");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x16, y16, 5);
						strcpy(End, "a16");
					}
					step++;
				}if (msg.x >= x17 - 5 && msg.x <= x17 + 5 && msg.y >= y17 - 5 && msg.y <= y17 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x17, y17, 5);
						strcpy(Start, "a17");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x17, y17, 5);
						strcpy(End, "a17");
					}
					step++;
				}
				if (msg.x >= x18 - 5 && msg.x <= x18 + 5 && msg.y >= y18 - 5 && msg.y <= y18 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x18, y18, 5);
						strcpy(Start, "a18");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x18, y18, 5);
						strcpy(End, "a18");
					}
					step++;
				}if (msg.x >= x19 - 5 && msg.x <= x19 + 5 && msg.y >= y19 - 5 && msg.y <= y19 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x19, y19, 5);
						strcpy(Start, "a19");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x19, y19, 5);
						strcpy(End, "a19");
					}
					step++;
				}
				if (msg.x >= x20 - 5 && msg.x <= x20 + 5 && msg.y >= y20 - 5 && msg.y <= y20 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x20, y20, 5);
						strcpy(Start, "a20");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x20, y20, 5);
						strcpy(End, "a20");
					}
					step++;
				}
				if (msg.x >= x21 - 5 && msg.x <= x21 + 5 && msg.y >= y21 - 5 && msg.y <= y21 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x21, y21, 5);
						strcpy(Start, "a21");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x21, y21, 5);
						strcpy(End, "a21");
					}
					step++;
				}
				if (msg.x >= x22 - 5 && msg.x <= x22 + 5 && msg.y >= y22 - 5 && msg.y <= y22 + 5)
				{
					if (step == 1) {
						setlinecolor(GREEN);
						setfillcolor(GREEN);
						fillcircle(x22, y22, 5);
						strcpy(Start, "a22");
					}
					if (step == 2)
					{
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x22, y22, 5);
						strcpy(End, "a22");
					}
					step++;
				}
				if (msg.x >= 38 && msg.x <= 171 && msg.y >= 555 && msg.y <= 620&&step>=2&&optimes1<1)
				{
					char str[100];
					start = Locatevex(G, Start);
					end = Locatevex(G, End);
					int temp = start;
					printf("%s", G.vexs[start]);
					strcpy(str, G.vexs[start]);
					float whole_time = 0;
					int goway = way[temp][path[temp][end]];
					int underway = 0;
					int price = 0;
					int front = start;
					while (temp != end)
					{
						front = path[front][end];

						if (goway != way[front][path[front][end]] && goway != 3 && way[front][path[front][end]] != 3)
						{
							if (goway == 1)price = price + 2;
							if (goway == 2)
							{
								if (underway < 7 && underway >= 0)price += 3;
								else if (underway >= 7 && underway < 14)price += 5;
								else price += 8;
								underway = 0;
							}
						}
						if (way[front][path[front][end]] == 2)underway++;
						printf("-->");
						strcat(str, "-->");
						printf("%s", G.vexs[path[temp][end]]);
						strcat(str, G.vexs[path[temp][end]]);
						goway = way[front][path[front][end]];
						whole_time += G.arcs[temp][path[temp][end]];
						temp = path[temp][end];
					}
					if (goway == 1 || goway == 3)price = price + 2;
					if (goway == 2)
					{
						if (underway < 7 && underway >= 0)price += 3;
						else if (underway >= 7 && underway < 14)price += 5;
						else price += 8;
					}
					printf(" %.0f分钟 花费为%d元\n", whole_time, price);
					char wholetime[100];
					char prices[100];
					int wholeinttime;
					wholeinttime = int(whole_time);
					itoa(wholeinttime, wholetime, 10);
					itoa(price, prices,10);
					strcat(str, " ");
					strcat(str, wholetime);
					strcat(str, "分钟 花费为");
					strcat(str, prices);
					strcat(str, "元");
					setbkmode(TRANSPARENT);
					settextstyle(26, 0, _T("黑体"));
					TCHAR ctr[100];
					CharToTchar(str, ctr);
					drawtext(ctr,&r, DT_LEFT| DT_TOP| DT_WORDBREAK | DT_EDITCONTROL);
					
					optimes1++;
				}
				if (msg.x >= 232 && msg.x <= 350 && msg.y >= 553 && msg.y <= 622&&step>=2&&optimes2<1)
				{
					start = Locatevex(G, Start);
					end = Locatevex(G, End);
					stack<int>q;
					findway(start, end, way, G, sway, q);
					set_price(sway, prices, way, start, end);
					print_lestpricesway(prices, sway, start, end, G);
					optimes2++;
				}
				if (msg.x >= 393 && msg.x <= 535 && msg.y >= 555 && msg.y <= 620&&step>=2&&optimes3<1)
				{
					start = Locatevex(G, Start);
					end = Locatevex(G, End);
					stack<int>q;
					findway(start, end, way, G, sway, q);
					set_times(sway, prices, way, start, end);
					print_lesttimesway(prices, sway, start, end, G);
					optimes3++;
				}
				if (msg.x >= 565 && msg.x <= 680 && msg.y >= 555 && msg.y <= 620)
				{
					step = 0;
					optimes1 = 0;
					optimes2 = 0;
					optimes3 = 0;
					putimage(0,0,&img1);
					setlinecolor(BLACK);
					setfillcolor(BLACK);
					fillcircle(350, 63, 5);
					fillcircle(182, 117, 5);
					fillcircle(163, 225, 5);
					fillcircle(110, 362, 5);
					fillcircle(233, 495, 5);
					fillcircle(403, 475, 5);
					fillcircle(460, 380, 5);
					fillcircle(485, 253, 5);
					fillcircle(468, 158, 5);
					fillcircle(300, 113, 5);
					fillcircle(290, 149, 5);
					fillcircle(268, 180, 5);
					fillcircle(375, 122, 5);
					fillcircle(412, 150, 5);
					fillcircle(418, 200, 5);
					fillcircle(448, 223, 5);
					fillcircle(425, 270, 5);
					fillcircle(412, 398, 5);
					fillcircle(278, 240, 5);
					fillcircle(268, 307, 5);
					fillcircle(250, 365, 5);
					fillcircle(225, 400, 5);
					fillcircle(343, 378, 5);
				}
			}
		}
	}

	}



