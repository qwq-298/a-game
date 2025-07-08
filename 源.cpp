#define _CRT_SECURE_NO_WARNINGS 
#include<iostream>
#include<graphics.h>
#include <conio.h>
#include<windows.h>
#include<vector>
#include<time.h>
#include<math.h>
#include<string>
#include<fstream>
#define FILENAME "游戏.text"
using namespace std;
int map[25][25];
int ifground[25][25] = {0};//检查是否为地面
int enemyexistmap[25][25] = { 0 };//检查敌人位置
int checkbuffer[25][25] = { 0 };//检验buffer
int checkbuffer1[25][25] = { 0 };
int checkbuffer2[25][25] = { 0 };
int enemygetbuffer[25][25] = { 0 };
int canbeshovle[25][25] = { 0 };
int canbeshovle1[25][25] = { 0 };
//int magicbuffer[25][25] = { 0 };
int ifhavemagic[25][25] = { 0 };
int money = 0;
int deathnumber = 0;
int luckynum = 10;//精英怪出现数字
int shovlecost = 50;
bool changeimage = false;
int eggtime = 60;//小推车再生时间
int endpositionx = 14;
int endpositiony = 0;
int bornpositionx = 0;
int bornpositiony = 14;
bool jumpsign = false;
int retw = 0;
int stime = 1;
TCHAR s1[] = _T("金币:");
TCHAR s2[] = _T("击杀敌人:");
TCHAR s3[] = _T("/");
IMAGE image1;
IMAGE image2;
IMAGE image5;
IMAGE image51;
IMAGE image6;
IMAGE image7;
IMAGE image8;
IMAGE image81;
IMAGE image9;
IMAGE image10;
IMAGE enemyimage[2];
IMAGE image11;
IMAGE image12;
IMAGE image13;
IMAGE image14;
IMAGE image15;
IMAGE image16;
IMAGE image17;
IMAGE image18;
IMAGE image19;
IMAGE image20;
IMAGE image21;
IMAGE image22;
IMAGE image23;
IMAGE image24;
IMAGE image25;
IMAGE image26;
IMAGE image27;
IMAGE image28;
IMAGE image29;
IMAGE image30;
IMAGE image31;
IMAGE image32;
IMAGE image33;
IMAGE image34;
IMAGE image35;
IMAGE image36;
IMAGE image37;
IMAGE image38;
IMAGE image39;
IMAGE image40;
IMAGE image41;
IMAGE image42;
IMAGE image43;//
IMAGE image44;//
IMAGE image45;//
IMAGE image46;//
IMAGE image47;//
IMAGE image48;//medcinebuffer
IMAGE image49;
IMAGE image50;
IMAGE image52;
IMAGE image53;
IMAGE image54;
IMAGE image55;
IMAGE image56;
IMAGE image57;
IMAGE image58;
IMAGE image59;
IMAGE image60;
IMAGE image61;
void transparentimage(int x, int y, IMAGE img) {
	IMAGE img1;
	DWORD* d1;
	img1 = img;
	d1 = GetImageBuffer(&img1);
	float h, s, l;
	for (int i = 0; i < img1.getheight() * img1.getwidth(); i++) {
		RGBtoHSL(BGR(d1[i]), &h, &s, &l);
		if (l < 0.03) {
			d1[i] = BGR(WHITE);
		}
		if (d1[i] != BGR(WHITE)) {
			d1[i] = 0;
		}
	}
	putimage(x, y, &img1, SRCAND);
	putimage(x, y, &img, SRCPAINT);
}
#pragma comment (lib,"MSIMG32.LIB")
void putimage_1(int x, int y, IMAGE* img)
{
	int h = img->getheight();
	int w = img->getwidth();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}
class Animation {
	int interval_ms = 0;
	vector<IMAGE*>framelist;
	int timer = 0;
	int indexframe = 0;
public:
	Animation(LPCTSTR path, int num, int interval) {
		interval_ms = interval;
		TCHAR path_file[600];
		for (int i = 0; i < num; i++) {
			_stprintf_s(path_file, path, i);
			IMAGE* p = new IMAGE;
			loadimage(p, path_file,25,25);
			framelist.push_back(p);
		}
	}
	~Animation() {
		for (int i = 0; i < framelist.size(); i++) {
			delete framelist[i];
		}
	}
	void play(int x, int y, int dur) {
		timer += dur;
		if (timer >= interval_ms) {
			indexframe = (indexframe + 1) % framelist.size();
			timer = 0;
		}
		putimage_1(x,y,framelist[indexframe]);
	}
};
int counttime() {
	static unsigned long long lastTime = 0;
	unsigned long long currentTime = GetTickCount();
	if (lastTime == 0) {
		lastTime = currentTime;
		return 0;
	}
	else {
		int ret = currentTime - lastTime;
		lastTime = currentTime;
		return ret;
	}
}
class Role {
public:
	int x;
	int y;
	int attack=5;
	int attackcircle=3;
	double attainterval = 1.0;//攻击间隔，需要很短
	int cost = 12;
	Animation* role2left = new Animation(_T("role2right%d.png"), 4, 4);
	void draw(int dur) {
		role2left->play(x * 25, y * 25, dur);
	}
	bool ifattack = false;
	bool ifdrawback = false;
};
Role Rolemode;
vector<Role*>vecRole;
int f(int a, int b) {
	if (a - b >= 0) {
		return a - b;
	}
	else {
		return 0;
	}
}
class Enemy {
private:
	Animation* enemyleft;
public:
	int x;
	int y;
	int movex;
	int movey;
	int blood;
	int speed;
	int listnum;
	int enemypower;
	double attackinterval2 = 1.0;
	/*int blood = 170;//170=一个远程塔，circle=3；
	int speed=25;//5左右*/
	Animation* enemyattack;
	Animation* enemyjump;
	Animation* enemyattack1;
	Animation* enemyshow;
	bool ifhaveshow=false;
	int visit[25][25] = { 0 };
	int money;
	bool live = true;
	/* = new Animation(_T("Enemy1left%d.png"), 4, 4);*/
	int ondeath = 0;
	int ondeath1 = 0;
	int ondeath2 = 0;
	int ondeath3 = 0;
	int ifdrop;
	int ifdrop1;
	int ifdrop2 = 0;
	bool ifbeblocked = false;
	bool ifbeblockedandbekilled = false;
	bool ifbekiiledatstone = false;
	bool checked = false;
	bool checked1 = false;
	bool checked2 = false;
	bool getbuffer = false;
	bool getbuffer1 = false;
	bool getbuffer2 = false;
	bool ifjump = false;
	bool jumpsignal = false;
	bool ifattacky = false;
	int improve = 0;
	int armor;
	int countnumber;
	int onshow = 0;
	int onattack = 0;
	int initialblood;
	bool ifcanbeblocked = true;
	int attacknum = 0;
	bool changeattack = false;
	bool startwalk;
	int rt = 0;
	int shoot = 0;
	vector<int>vec;
	vector<int>vec1;
	vector<int>vec2;
	void beattack(Role* p,double timerx,bool&flagx) {
		if (p->ifdrawback == false) {
			if (timerx >= p->attainterval) {
				double distance = 0;
				distance = pow(fabs(p->x - movex) * fabs(p->x - movex) + fabs(p->y - movey) * fabs(p->y - movey), 0.5);
				if (distance <= p->attackcircle && live == true) {
					blood = blood - f(p->attack,armor);
					p->ifattack = true;
					setlinecolor(LIGHTRED);
					line(p->x * 25 + 12.5, p->y * 25 + 12.5, x, y);
					putimage_1(x, y, &image7);
					p->draw(1);
				}
				flagx = true;
			}
		}
	};
	void draw(int dur) {
		enemyleft->play(x-12.5, y-12.5, dur);
	}
	void drawattack(int dur) {
		enemyattack->play(x - 12.5, y - 12.5, dur);
	}
	void drawattack1(int dur) {
		enemyattack1->play(x - 12.5, y - 12.5, dur);
	}
	void drawjump(int dur) {
		enemyjump->play(x - 12.5, y - 12.5, dur);
	}
	void drawshow(int dur) {
		enemyshow->play(x - 12.5, y - 12.5, dur);
		ifhaveshow = true;
	}
	Enemy(LPCTSTR path,int num,int interval,int eblood,int espeed,int elistnum,int emoney,int einitialblood,int eenemypower,int earmor) {
		enemyleft = new Animation(path, num, interval);
		blood = eblood;
		speed = espeed;
		listnum = elistnum;
		money = emoney;
		initialblood = einitialblood;
		enemypower = eenemypower;
		armor = earmor;
	}
};
vector<Enemy*>vecEnemy;
class Role1 {
public:
	int x;
	int y;
	int rolepower = 15;
	int blockmaxnum = 1;
	int blocknum = 0;
	double attainterval1 = 1.0;
	int blood=150;
	int initialblood = blood;
	bool live = true;
	int ondeath = 0;
	int ondeath1 = 0;
	int cost = 10;
	bool ifdrawback = false;
	int ifdrop = 0;
	int whichbuffer;
	bool checked = false;
	vector<int>vec;
	vector<Enemy*>jumpenemy;
	Animation* roleleft = new Animation(_T("role1right%d.png"), 4, 4);
	void beatenemy(Enemy*p) {
		int sign = 0;
		for (int i = 0; i < jumpenemy.size(); i++) {
			if (jumpenemy[i]->countnumber == p->countnumber) {
				sign = 1;
			}
		}
		if (sign == 0) {
			if (p->ifcanbeblocked == true) {
				if (ifdrawback == false) {
					if ((p->movex == x - 1 && p->movey == y) || (p->movex == x + 1 && p->movey == y) || (p->movex == x && p->movey == y - 1) || (p->movex == x && p->movey == y + 1)) {
						if (p->listnum == 3 && p->ifjump == false) {
							p->ifjump = true;
							jumpsign = true;
							jumpenemy.push_back(p);
						}
						else {
							p->ifbeblocked = true;
							ifattack = true;
							enemyattacked.push_back(p);
							blocknum++;
						}
					}
				}
			}
		}
	}
	void beat(Enemy*p,double timery,bool&flagz) {
		if (ifdrawback==false) {
			if (timery >= attainterval1) {
				p->blood = p->blood - f(rolepower,p->armor);
				if (p->listnum == 4 && p->attacknum < 5) {
					p->attacknum++;
					blood = blood - p->enemypower;
				}
				else if (p->listnum == 4 && p->attacknum == 5) {
					p->attacknum = 0;
					p->changeattack = true;
					blood = blood - p->enemypower * 10;
				}
				else { blood = blood - p->enemypower; }//假设敌我方攻击速度相同
				flagz = true;
			}
		}
	}
	bool ifattack = false;
	vector<Enemy*>enemyattacked;
	void draw(int dur) {
		roleleft->play(x*25, y*25, dur);
	}
};
Role1 Role1mode;
vector<Role1*>vecRole1;
class Role2 {
public:
	int x;
	int y;
	int cost=5;
	void makemoney() {
		money++;
	}
	bool ifdrawback = false;
};
Role2 Role2mode;
vector<Role2*>vecRole2;
class Redoilbarrel {
private:
	Animation* boom=new Animation(_T("boom%d.png"),9,10);
public:
	int x;
	int y;
	void draw(int dur) {
		boom->play(x*25, y*25, dur);
	}
	bool ifboom = false;
	bool beused = false;
	int boomtime = 0;
};
vector<Redoilbarrel*>vecRedoilbarrel;
class Rolestone {
public:
	int x;
	int y;
	int cost = 30;
	bool ifdrawback = false;
	bool live = true;
	int blood = 500;
	int initialblood = 500;
	int blockmaxnum = 5;
	int blocknum = 0;
	void block(Enemy* p, double timery, bool& flagv) {
		if (ifdrawback == false) {
			if (timery >= p->attackinterval2) {
				if (p->listnum == 4 && p->attacknum < 5) {
					p->attacknum++;
					blood = blood - p->enemypower;
				}
				else if (p->listnum == 4 && p->attacknum == 5) {
					p->changeattack = true;
					p->attacknum = 0;
					blood = blood - p->enemypower * 10;
				}
				else { blood = blood - p->enemypower; }//假设敌我方攻击速度相同
				flagv = true;
			}
		}
	}
	vector<Enemy*>enemybeblocked;
	vector<Enemy*>enemyjump;
};
Rolestone Rolestonemode;
vector<Rolestone*>vecRolestone;
class Duck {
public:
	int x;
	int y;
	int visit[25][25] = { 0 };
	int speed = 1;
	bool iftotheend = false;
	Animation* move = new Animation(_T("duckmove%d.png"), 4, 4);
	void draw(int dur) {
		move->play(x * 25, y * 25, dur);
	}
};
vector<Duck*>vecduck;
class Bullet {
public:
	int x;
	int y;
	bool ifcatch=false;
	int endx;
	int endy;
	int damage;
};
vector<Bullet*>vecbullet;
void drawmap() {
	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 25; j++) {
			if (i == 14 && j <= 14) {
				map[i][j] = 1;
			}
			else if (j == 14 && i <= 14) {
				map[i][j] = 1;
			}
			else {
				map[i][j] = 0;
			}
			canbeshovle[i][j] = 0;
		}
	}
	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 25; j++) {
			if (map[i][j] == 0) {
				putimage(25 * i, 25 * j, &image1);
			}
			if (map[i][j] == 1) {
				putimage(25 * i, 25 * j, &image2);
			}
		}
	}
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 13; j++) {
			putimage_1(i*25, j*25, &image13);
			canbeshovle[i][j] = 1;
		}
	}
	for (int i = 15; i < 25; i++) {
		for (int j = 15; j < 25; j++) {
			putimage_1(i * 25, j * 25, &image13);
			canbeshovle[i][j] = 1;
		}
	}
	for (int i = 5; i < 12; i++) {
		putimage_1(i*25, 500, &image14);
	}
	for (int i = 3; i < 14; i++) {
		putimage_1(i * 25, 525, &image14);
	}
	for (int i = 2; i < 15; i++) {
		putimage_1(i * 25, 550, &image14);
	}
	for (int i = 0; i < 15; i++) {
		putimage_1(i * 25, 575, &image14);
		putimage_1(i * 25, 600, &image14);
	}
	for (int i = 0; i < 2; i++) {
		putimage_1(i * 25, 550, &image15);
		ifground[i][22] = 1;
	}
	for (int i = 1; i < 3; i++) {
		putimage_1(i * 25, 525, &image15);
		ifground[i][21] = 1;
	}
	for (int i = 2; i < 5; i++) {
		putimage_1(i * 25, 500, &image15);
		ifground[i][20] = 1;
	}
	for (int i = 4; i < 13; i++) {
		putimage_1(i * 25, 475, &image15);
		ifground[i][19] = 1;
	}
	for (int i = 12; i < 15; i++) {
		putimage_1(i * 25, 500, &image15);
		ifground[i][20] = 1;
	}
	for (int i = 0; i < 15; i++) {
		putimage_1(i * 25, 450, &image13);
		canbeshovle[i][450/25] = 1;
	}
	for (int i = 0; i < 4; i++) {
		putimage_1(i * 25, 475, &image16);
	}
	for (int i = 0; i < 6; i++) {
		putimage_1(i * 25, 375, &image13);
		canbeshovle[i][375/25] = 1;
	}
	for (int i = 0; i < 9; i++) {
		putimage_1(i * 25, 400, &image13);
		canbeshovle[i][400/25] = 1;
	}
	for (int i = 0; i < 13; i++) {
		putimage_1(i * 25, 425, &image13);
		canbeshovle[i][425/25] = 1;
	}
	for (int i = 15; i < 25; i++) {
		for (int j = 0; j < 15; j++) {
			putimage_1(i* 25, j*25, &image13);
			canbeshovle[i][j] = 1;
		}
	}
	for (int j = 0; j < 5; j++) {
		putimage_1(13* 25,j * 25, &image13);
		canbeshovle[13][j] = 1;
	}
	for (int j = 0; j < 5; j++) {
		putimage_1(100+j * 25, 13* 25, &image13);
		canbeshovle[4+j][13] = 1;
	}
	for (int j = 0; j < 5; j++) {
		putimage_1(j * 25, 13 * 25, &image13);
		canbeshovle[j][13] = 1;
	}
	putimage_1(225, 13 * 25, &image13);
	canbeshovle[225/25][13] = 1;
	putimage_1(250, 13 * 25, &image13);
	canbeshovle[10][13] = 1;
	putimage_1(13 * 25, 6 * 25, &image13);
	canbeshovle[13][6] = 1;
	putimage_1(13 * 25, 7* 25, &image13);
	canbeshovle[13][7] = 1;
	putimage_1(13 * 25, 325, &image22);
	putimage_1(13 * 25, 300, &image22);
	putimage_1(13 * 25, 275, &image22);
	putimage_1(13 * 25, 250, &image22);
	putimage_1(13 * 25, 225, &image22);
	putimage_1(13* 25, 400, &image18);
	putimage_1(13 * 25, 375, &image20);
	putimage_1(12 * 25, 400, &image20);
	putimage_1(12 * 25, 375, &image21);
	putimage_1(14 * 25, 375, &image21);
	putimage_1(11* 25, 375, &image21);
	putimage_1(10 * 25, 375, &image21);
	putimage_1(9 * 25, 375, &image21);
	putimage_1(14 * 25, 400, &image18);
	putimage_1(13 * 25, 425, &image18);
	putimage_1(14 * 25, 425, &image18);
	putimage_1(0, 500, &image16);
	putimage_1(0, 525, &image16);
	putimage_1(25, 500, &image16);
	putimage_1(50, 500, &image16);
	putimage_1(14 * 25, 525, &image15);
	putimage_1(14 * 25, 500, &image17);
	putimage_1(14 * 25, 475, &image17);
	putimage_1(13 * 25, 475, &image17);
	if (money >= Rolemode.cost) { putimage(0, 625, &image44); }
	else { putimage(0, 625, &image5); }
	if (money >= Role1mode.cost) { putimage(50, 625, &image43); }
	else { putimage(50, 625, &image8); }
	if (money >= Role2mode.cost) { putimage(100, 625, &image45); }
	else { putimage(100, 625, &image23); }
	putimage(150, 625, &image29);//background
	putimage(150+9*25, 625, &image27);
	putimage_1(0, 0, &image40);
	/*if (money >= shovlecost) { putimage(150, 625, &image46); }
	else { putimage(150, 625, &image41); }*/
	putimage(200, 625, &image42);
	if (money >= Rolestonemode.cost) { putimage(250, 625, &image55); }
	else { putimage(250, 625, &image53); }
	if (changeimage == false) {
		putimage(300, 625, &image56);
	}
	else if (changeimage == true) {
		putimage(300, 625, &image57);
	}
}
void loadpicture() {
	loadimage(&image1, L"picture1.png", 25, 25);
	loadimage(&image2, L"picture2.png", 25, 25);
	loadimage(&image5, L"picture5.png", 50, 50);
	loadimage(&image51, L"picture5.png", 25, 25);
	loadimage(&image6, L"tombpicture.png", 25, 25);
	loadimage(&image7, L"beattackeffect.png", 15, 15);
	loadimage(&image8, L"picturerole.png", 50, 50);
	loadimage(&image81, L"roleidle.png", 25, 25);
	loadimage(&image9, L"Enemy1left0.png", 25, 25);
	loadimage(&image10, L"Enemy2left0.png", 25, 25);
	loadimage(&image11, L"rolebekilled.png", 25, 25);
	loadimage(&image12, L"roleidle1.png", 25, 25);
	loadimage(&image13, L"forest.png", 25, 25);
	loadimage(&image14, L"water.png", 25, 25);
	loadimage(&image15, L"ground.png", 25, 25);
	loadimage(&image16, L"stone.png", 25, 25);
	loadimage(&image17, L"redgrass.png", 25, 25);
	loadimage(&image18, L"cuttree.png", 25, 25);
	loadimage(&image19, L"downtree.png", 25, 25);
	loadimage(&image20, L"chicken0.png", 25, 25);
	loadimage(&image21, L"greengrass.png", 25, 25);
	loadimage(&image22, L"greengrass1.png", 25, 25);
	loadimage(&image23, L"pickaxe.png", 50, 50);
	loadimage(&image24, L"house.png", 25, 25);
	loadimage(&image25, L"heart.png", 25, 25);
	loadimage(&image26, L"speedup.png", 25, 25);
	loadimage(&image27, L"drawback.png", 50, 50);
	loadimage(&image28, L"Frame.png", 60, 60);
	loadimage(&image29, L"background.png", 475, 50);
	loadimage(&image30, L"improve.png", 25, 25);
	loadimage(&image31, L"improve1.png", 25, 25);
	loadimage(&image32, L"friend100%.png", 25, 4);
	loadimage(&image33, L"friend80%.png", 20, 4);
	loadimage(&image34, L"friend60%.png", 15, 4);
	loadimage(&image35, L"friend40%.png", 15, 4);
	loadimage(&image36, L"friend20%.png", 15, 4);
	loadimage(&image37, L"uibackground.png", 625, 675);
	loadimage(&image38, L"button.png", 200, 100);
	loadimage(&image39, L"filebutton.png", 200, 100);
	loadimage(&image40, L"keepbutton.png", 50, 50);
	loadimage(&image41, L"shovel.png", 50, 50);
	loadimage(&image42, L"potion.png", 50, 50);
	loadimage(&image43, L"picturerole1.png", 50, 50);
	loadimage(&image44, L"picture51.png", 50, 50);
	loadimage(&image45, L"pichaxe1.png", 50, 50);
	loadimage(&image46, L"shovel1.png", 50, 50);
	loadimage(&image47, L"potion1.png", 50, 50);
	loadimage(&image48, L"medcinebuffer.png", 25, 25);
	loadimage(&image49, L"potion(1).png", 50, 50);
	loadimage(&image50, L"potion(2).png", 50, 50);
	loadimage(&image52, L"redoilbarrel.png", 25, 25);
	loadimage(&image53, L"stonebarrier.png", 50, 50);
	loadimage(&image54, L"stoneimage.png", 25, 25);
	loadimage(&image55, L"stonebarrier1.png", 50, 50);
	loadimage(&image56, L"egg1.png", 50, 50);
	loadimage(&image57, L"egg.png", 50, 50);
	loadimage(&image58, L"eggimage.png", 25, 25);
	loadimage(&image59, L"ghostidle.png", 25, 25);
	loadimage(&image60, L"bullet.png", 25, 25);
	loadimage(&image61, L"skull.png", 25, 25);
}
void createEnemy(int maxEnemy) {
	static int count = 0;
	count++;
	int ret = rand() % 2+1;
	int ret1 = rand() % 10 + 1;
	static int counter = 0;
	if (count > 5&&maxEnemy>vecEnemy.size()) {
		count = 0;
		counter++;
		if (counter % luckynum!= 0||deathnumber==0) {
			if (ret == 1 && !(ret1 == 9 && counter >= 40) && !(ret1 == 10 && counter >= 60)&&!(counter >= 80 && retw <= 5)&& !(ret1 <= 2 && counter >= 20)) {
				Enemy* p = new Enemy(_T("Enemy1left%d.png"), 4, 4, 170, 25, 0, 10, 170,5,0);
				p->x = bornpositionx*25+12.5;
				p->y = bornpositiony*25+12.5;
				p->movex = p->x / 25;
				p->movey = p->y / 25;
				int ret1 = rand() % 100 + 1;
				int ret2 = rand() % 100 + 1;
				if (ret1 <= 90 && 60 <= ret1) {
					p->ifdrop = 1;
				}
				else {
					p->ifdrop = 0;
				}
				if (ret2 <= 30 && 0 <= ret2) {
					p->ifdrop1 = 1;
				}
				else {
					p->ifdrop1 = 0;
				}
				p->countnumber = counter;
				p->startwalk = true;
				vecEnemy.push_back(p);
			}
			if (ret==2&& !(ret1 == 9 && counter >= 40) && !(ret1 == 10 && counter >= 60)&&!(counter >= 80 && retw <= 5)&& !(ret1 <= 2 && counter >= 20)) {
				Enemy* p = new Enemy(_T("Enemy2left%d.png"), 4, 4, 370, 25, 1, 15, 370,10,0);
				p->x = bornpositionx * 25 + 12.5;
				p->y = bornpositiony * 25 + 12.5;
				p->movex = p->x / 25;
				p->movey = p->y / 25;
				p->enemyattack = new Animation(_T("Enemy2attack%d.png"), 4, 4);
				int ret1 = rand() % 100 + 1;
				int ret2 = rand() % 100 + 1;
				if (ret1 <= 90 && 70 <= ret1) {
					p->ifdrop = 1;
				}
				else {
					p->ifdrop = 0;
				}
				if (ret2 <= 40 && 4 <= ret2) {
					p->ifdrop1 = 1;
				}
				else {
					p->ifdrop1 = 0;
				}
				p->countnumber = counter;
				p->startwalk = true;
				vecEnemy.push_back(p);
			}
			if (ret1 <= 2 && counter >= 20 && !(counter >= 80 && retw <= 5)) {
				Enemy* p = new Enemy(_T("Enemy3left%d.png"), 4, 4, 900, 25, 2, 0, 900, 20, 0);//注意攻击
				p->x = bornpositionx * 25 + 12.5;
				p->y = bornpositiony * 25 + 12.5;
				p->movex = p->x / 25;
				p->movey = p->y / 25;
				p->enemyattack = new Animation(_T("Enemy3attack%d.png"), 4, 4);
				p->ifdrop2 = 1;
				p->countnumber = counter;
				p->startwalk = true;
				vecEnemy.push_back(p);
			}
			if (ret1==9&&counter>=40&&!(counter >= 80 && retw <= 5)) {
				Enemy* p = new Enemy(_T("Enemy4left%d.png"), 4, 4, 1200, 25, 3, 200, 1000, 15,10);
				p->x = bornpositionx * 25 + 12.5;
				p->y = bornpositiony * 25 + 12.5;
				p->movex = p->x / 25;
				p->movey = p->y / 25;
				p->enemyattack = new Animation(_T("Enemy4attack%d.png"), 4, 4);//注意逗号
				p->enemyjump = new Animation(_T("Enemy4jump%d.png"), 4, 4);
				p->countnumber = counter;
				p->startwalk = true;
				vecEnemy.push_back(p);
			}
			if (ret1 == 10&&counter>=60&& !(counter >= 80 && retw <= 5)) {
				Enemy* p = new Enemy(_T("Enemy5left%d.png"), 4, 4, 1500, 25, 4, 0, 1500, 40, 20);
				p->x = bornpositionx * 25 + 12.5;
				p->y = bornpositiony * 25 + 12.5;
				p->movex = p->x / 25;
				p->movey = p->y / 25;
				p->enemyattack = new Animation(_T("Enemy5attack%d.png"), 4, 4);
				p->enemyattack1 = new Animation(_T("Enemy5eattack%d.png"), 4, 4);
				p->countnumber = counter;
				p->startwalk = true;
				vecEnemy.push_back(p);
			}
			if (counter>=80&&retw<=5) {
				Enemy* p = new Enemy(_T("Enemy6left%d.png"), 2, 2, 1000, 25, 5, 0, 3000, 5, 15);//attackpower
				p->x = (bornpositionx+retw) * 25 + 12.5;
				p->y = bornpositiony * 25 + 12.5;//////////////////////////attation!!!!!!!!!!!!!!!!!!!!!!!!!!!
				p->movex = p->x / 25;
				p->movey = p->y / 25;
				p->enemyattack = new Animation(_T("Enemy6attack%d.png"), 7, 7);
				p->enemyshow = new Animation(_T("Enemy6show%d.png"), 4, 4);
				p->countnumber = counter;
				p->startwalk = false;
				vecEnemy.push_back(p);
				if (retw <= 5) {
					retw++;
				}
			}
		}
		else if(counter%luckynum==0&&deathnumber!=0){
			Enemy* p = new Enemy(_T("Enemy3left%d.png"), 4, 4, 900, 25, 2, 0, 900,20,0);//注意攻击
			p->x = bornpositionx * 25 + 12.5;
			p->y = bornpositiony * 25 + 12.5;
			p->movex = p->x / 25;
			p->movey = p->y / 25;
			p->enemyattack = new Animation(_T("Enemy3attack%d.png"), 4, 4);
			p->ifdrop2 = 1;
			p->countnumber = counter;
			p->startwalk = true;
			vecEnemy.push_back(p);
		}
	}// 创建前两个怪物
}
void showenemy(int maxEnemy) {
	int flag1 = 0;
	int timer = 0;
	timer += counttime();
	int ret1 = rand() % 1000 + 1;//控制生成快慢
	if (timer > ret1) {
		timer = 0;
		flag1 = 1;
	}
	if (flag1 == 1 && maxEnemy >= vecEnemy.size()) {
		flag1 = 0;
		createEnemy(maxEnemy);
	}
}
void moveEnemy(Enemy* p) {
	if (p->ifbeblocked == false&&p->startwalk==true) {
		int endx = endpositionx*25;
		int endy = endpositiony*25;
		int movex = p->movex;
		int movey = p->movey;
		if (movex != endx / 25 || movey != endy / 25) {
			if (movex == 0 && movey == 0) {
				p->visit[movex][movey] = 1;
				if (map[movex + 1][movey] == 1 && p->visit[movex + 1][movey] == 0) {
					p->x = p->x + p->speed;
				}
				else if (map[movex][movey + 1] == 1 && p->visit[movex][movey + 1] == 0) {
					p->y = p->y + p->speed;
				}
				p->movex = p->x / 25;
				p->movey = p->y / 25;
				if (enemygetbuffer[p->movex][p->movey] == 1) {
					p->getbuffer = true;
					for (int i = 0; i < vecRole1.size(); i++) {
						if (vecRole1[i]->ifdrawback == false) {
							if (vecRole1[i]->live == false && vecRole1[i]->checked == false) {
								if (vecRole1[i]->vec.size() == 3) {
									if (vecRole1[i]->vec[0] == p->movex && vecRole1[i]->vec[1] == p->movey) {
										vecRole1[i]->checked = true;
										if (vecRole1[i]->vec[2] == 0) {
											p->getbuffer1 = true;
										}
										else if(vecRole1[i]->vec[2] == 1){
											p->getbuffer2 = true;
										}
									}
								}
							}
						}
					}
					enemygetbuffer[p->movex][p->movey] = 0;
				}
			}
			else if (movex == 0 && movey != 0) {
				p->visit[movex][movey] = 1;
				if (map[movex + 1][movey] == 1 && p->visit[movex + 1][movey] == 0) {
					p->x = p->x + p->speed;
				}
				else if (map[movex][movey + 1] == 1 && p->visit[movex][movey + 1] == 0) {
					p->y = p->y + p->speed;
				}
				else if (map[movex][movey - 1] == 1 && p->visit[movex][movey - 1] == 0) {
					p->y = p->y - p->speed;
				}
				p->movex = p->x / 25;
				p->movey = p->y / 25;
				if (enemygetbuffer[p->movex][p->movey] == 1) {
					p->getbuffer = true;
					for (int i = 0; i < vecRole1.size(); i++) {
						if (vecRole1[i]->ifdrawback == false) {
							if (vecRole1[i]->live == false && vecRole1[i]->checked == false) {
								if (vecRole1[i]->vec.size() == 3) {
									if (vecRole1[i]->vec[0] == p->movex && vecRole1[i]->vec[1] == p->movey) {
										vecRole1[i]->checked = true;
										if (vecRole1[i]->vec[2] == 0) {
											p->getbuffer1 = true;
										}
										else if (vecRole1[i]->vec[2] == 1) {
											p->getbuffer2 = true;
										}
									}
								}
							}
						}
					}
					enemygetbuffer[p->movex][p->movey] = 0;
				}
			}
			else if (movex != 0 && movey == 0) {
				p->visit[movex][movey] = 1;
				if (map[movex + 1][movey] == 1 && p->visit[movex + 1][movey] == 0) {
					p->x = p->x + p->speed;
				}
				else if (map[movex][movey + 1] == 1 && p->visit[movex][movey + 1] == 0) {
					p->y = p->y + p->speed;
				}
				else if (map[movex - 1][movey] == 1 && p->visit[movex - 1][movey] == 0) {
					p->x = p->x - p->speed;
				}
				p->movex = p->x / 25;
				p->movey = p->y / 25;
				if (enemygetbuffer[p->movex][p->movey] == 1) {
					p->getbuffer = true;
					for (int i = 0; i < vecRole1.size(); i++) {
						if (vecRole1[i]->ifdrawback == false) {
							if (vecRole1[i]->live == false && vecRole1[i]->checked == false) {
								if (vecRole1[i]->vec.size() == 3) {
									if (vecRole1[i]->vec[0] == p->movex && vecRole1[i]->vec[1] == p->movey) {
										vecRole1[i]->checked = true;
										if (vecRole1[i]->vec[2] == 0) {
											p->getbuffer1 = true;
										}
										else if (vecRole1[i]->vec[2] == 1) {
											p->getbuffer2 = true;
										}
									}
								}
							}
						}
					}
					enemygetbuffer[p->movex][p->movey] = 0;
				}
			}
			else if (movex != 0 && movey != 0) {
				p->visit[movex][movey] = 1;
				if (map[movex + 1][movey] == 1 && p->visit[movex + 1][movey] == 0) {
					p->x = p->x + p->speed;
				}
				else if (map[movex][movey + 1] == 1 && p->visit[movex][movey + 1] == 0) {
					p->y = p->y + p->speed;
				}
				else if (map[movex][movey - 1] == 1 && p->visit[movex][movey - 1] == 0) {
					p->y = p->y - p->speed;
				}
				else if (map[movex - 1][movey] == 1 && p->visit[movex - 1][movey] == 0) {
					p->x = p->x - p->speed;
				}
				p->movex = p->x / 25;
				p->movey = p->y / 25;
				if (enemygetbuffer[p->movex][p->movey] == 1) {
					p->getbuffer = true;
					for (int i = 0; i < vecRole1.size(); i++) {
						if (vecRole1[i]->ifdrawback == false) {
							if (vecRole1[i]->live == false && vecRole1[i]->checked == false) {
								if (vecRole1[i]->vec.size() == 3) {
									if (vecRole1[i]->vec[0] == p->movex && vecRole1[i]->vec[1] == p->movey) {
										vecRole1[i]->checked = true;
										if (vecRole1[i]->vec[2] == 0) {
											p->getbuffer1 = true;
										}
										else if (vecRole1[i]->vec[2] == 1) {
											p->getbuffer2 = true;
										}
									}
								}
							}
						}
					}
					enemygetbuffer[p->movex][p->movey] = 0;
				}
			}
		}
		else if (movex == endx / 25 && movey == endy / 25) {
			p->live = false;
		}
	}
}
/*void drawEnemy(Enemy* p) {
	if (p->live == true) {
		setfillcolor(RED);
		solidrectangle(p->movex * 25, p->movey * 25, p->movex * 25 + 25, p->movey * 25 + 25);
	}
}*/
void moveBullet(Bullet* p) {
	if (p->x != p->endx || p->y != p->endy) {
		if (p->ifcatch == false) {
			if (p->x == p->endx) {
				if (p->endy > p->y) {
					p->y++;
				}
				else if (p->endy < p->y) {
					p->y--;
				}
			}
			if (p->y == p->endy) {
				if (p->endx > p->x) {
					p->x++;
				}
				else if (p->endx < p->x) {
					p->x--;
				}
			}
		}
	}
}
void moveDuck(Duck* p) {
	int endx = bornpositionx * 25;
	int endy = bornpositiony * 25;
	int movex = p->x;
	int movey = p->y;
	if (movex != endx / 25 || movey != endy / 25) {
		if (movex == 0 && movey == 0) {
			p->visit[movex][movey] = 1;
			if (map[movex + 1][movey] == 1 && p->visit[movex + 1][movey] == 0) {
				p->x = p->x + p->speed;
			}
			else if (map[movex][movey + 1] == 1 && p->visit[movex][movey + 1] == 0) {
				p->y = p->y + p->speed;
			}
		}
		else if (movex == 0 && movey != 0) {
			p->visit[movex][movey] = 1;
			if (map[movex + 1][movey] == 1 && p->visit[movex + 1][movey] == 0) {
				p->x = p->x + p->speed;
			}
			else if (map[movex][movey + 1] == 1 && p->visit[movex][movey + 1] == 0) {
				p->y = p->y + p->speed;
			}
			else if (map[movex][movey - 1] == 1 && p->visit[movex][movey - 1] == 0) {
				p->y = p->y - p->speed;
			}
		}
		else if (movex != 0 && movey == 0) {
			p->visit[movex][movey] = 1;
			if (map[movex + 1][movey] == 1 && p->visit[movex + 1][movey] == 0) {
				p->x = p->x + p->speed;
			}
			else if (map[movex][movey + 1] == 1 && p->visit[movex][movey + 1] == 0) {
				p->y = p->y + p->speed;
			}
			else if (map[movex - 1][movey] == 1 && p->visit[movex - 1][movey] == 0) {
				p->x = p->x - p->speed;
			}
		}
		else if (movex != 0 && movey != 0) {
			p->visit[movex][movey] = 1;
			if (map[movex + 1][movey] == 1 && p->visit[movex + 1][movey] == 0) {
				p->x = p->x + p->speed;
			}
			else if (map[movex][movey + 1] == 1 && p->visit[movex][movey + 1] == 0) {
				p->y = p->y + p->speed;
			}
			else if (map[movex][movey - 1] == 1 && p->visit[movex][movey - 1] == 0) {
				p->y = p->y - p->speed;
			}
			else if (map[movex - 1][movey] == 1 && p->visit[movex - 1][movey] == 0) {
				p->x = p->x - p->speed;
			}
		}
	}
	else if (movex == endx / 25 && movey == endy / 25) {
		p->iftotheend= true;
	}
}
void ifexistenemy(Role* p) {
	bool flagq = true;
	for (int i = 0; i < vecEnemy.size(); i++) {
		if (vecEnemy[i]->live == true) {
			double distance1 = 0;
			distance1 = pow(fabs(vecEnemy[i]->movex - p->x) * fabs(vecEnemy[i]->movex - p->x) + fabs(vecEnemy[i]->movey - p->y) * fabs(vecEnemy[i]->movey - p->y), 0.5);
			if (distance1 <= p->attackcircle) {
				flagq = false;
			}
		}
	}
	if (flagq == true) {
		p->ifattack = false;
	}
}
void actionattack(double timerx,bool&flagx) {
	for (int i = 0; i < vecEnemy.size(); i++) {
		if (vecEnemy[i]->live) {
			for (int j = 0; j < vecRole.size(); j++) {
				vecEnemy[i]->beattack(vecRole[j],timerx,flagx);
			}
			if (vecEnemy[i]->blood <= 0) {
				vecEnemy[i]->live = false;
				deathnumber++;
			}
		}
	}
}
void twoactionattack(double timery, bool& flagz) {
	for (int i = 0; i < vecRole1.size(); i++) {
		if (vecRole1[i]->ifdrawback == false) {
			if (vecRole1[i]->live == true) {
				bool flagy = false;
				if ((enemyexistmap[vecRole1[i]->x + 1][vecRole1[i]->y] == 1) || (enemyexistmap[vecRole1[i]->x - 1][vecRole1[i]->y] == 1) || (enemyexistmap[vecRole1[i]->x][vecRole1[i]->y - 1] == 1) || (enemyexistmap[vecRole1[i]->x][vecRole1[i]->y + 1] == 1)) {
					flagy = true;
				}
				if ((vecRole1[i]->blocknum < vecRole1[i]->blockmaxnum) && flagy == true) {
					for (int j = 0; j < vecEnemy.size(); j++) {
						if (vecEnemy[j]->live == true) {//!!!!
							vecRole1[i]->beatenemy(vecEnemy[j]);
						}
					}
				}
				if (vecRole1[i]->ifattack == true) {
					for (int k = 0; k < vecRole1[i]->enemyattacked.size(); k++) {
						if (vecRole1[i]->enemyattacked[k]->live == true && vecRole1[i]->enemyattacked[k]->ifbeblockedandbekilled == false) {
							vecRole1[i]->beat(vecRole1[i]->enemyattacked[k], timery, flagz);
							if (vecRole1[i]->enemyattacked[k]->blood <= 0) {
								vecRole1[i]->enemyattacked[k]->live = false;
								deathnumber++;
								vecRole1[i]->enemyattacked[k]->ifbeblockedandbekilled = true;
								vecRole1[i]->blocknum--;
								if (vecRole1[i]->blocknum == 0) {
									vecRole1[i]->ifattack = false;
								}
							}
							if (vecRole1[i]->blood <= 0) {
								vecRole1[i]->live = false;
								vecRole1[i]->enemyattacked[k]->ifbeblocked = false;
							}
						}
					}
				}
				if (vecRole1[i]->live == true) {
					int num = 0;
					for (int k = 0; k < vecRole1[i]->enemyattacked.size(); k++) {
						if (vecRole1[i]->enemyattacked[k]->live == true) {
							num++;
						}
					}
					if (num != vecRole1[i]->blocknum) {
						vecRole1[i]->blocknum = num;
					}
				}
			}
		}
	}
}
void createredoilbarrel(int x, int y) {
	Redoilbarrel* p = new Redoilbarrel;
	p->x = x;
	p->y = y;
	vecRedoilbarrel.push_back(p);
}
void checkboom() {
	for (int i = 0; i < vecRedoilbarrel.size(); i++) {
		if (vecRedoilbarrel[i]->ifboom == false) {
			int px = vecRedoilbarrel[i]->x;
			int py = vecRedoilbarrel[i]->y;
			int pxfront = px - 1;
			int pxback = px + 1;
			int pyfront = py - 1;
			int pyback = py + 1;
			int signalv = 0;
			for (int j = 0; j < vecEnemy.size(); j++) {
				if (vecEnemy[j]->live == true) {
					if (vecEnemy[j]->movex == px && vecEnemy[j]->movey == py) {
						vecEnemy[j]->blood = 0;
						vecEnemy[j]->live = false;
						signalv = 1;
					}
					else if (vecEnemy[j]->movex == px && vecEnemy[j]->movey == pyfront) {
						vecEnemy[j]->blood = 0;
						vecEnemy[j]->live = false;
						signalv = 1;
					}
					else if (vecEnemy[j]->movex == px && vecEnemy[j]->movey == pyback) {
						vecEnemy[j]->blood = 0;
						vecEnemy[j]->live = false;
						signalv = 1;
					}
					else if (vecEnemy[j]->movex == pxfront && vecEnemy[j]->movey == py) {
						vecEnemy[j]->blood = 0;
						vecEnemy[j]->live = false;
						signalv = 1;
					}
					else if (vecEnemy[j]->movex == pxback && vecEnemy[j]->movey == py) {
						vecEnemy[j]->blood = 0;
						vecEnemy[j]->live = false;
						signalv = 1;
					}
					else if (vecEnemy[j]->movex == pxfront && vecEnemy[j]->movey == pyfront) {
						vecEnemy[j]->blood = 0;
						vecEnemy[j]->live = false;
						signalv = 1;
					}
					else if (vecEnemy[j]->movex == pxfront && vecEnemy[j]->movey == pyback) {
						vecEnemy[j]->blood = 0;
						vecEnemy[j]->live = false;
						signalv = 1;
					}
					else if (vecEnemy[j]->movex == pxback && vecEnemy[j]->movey == pyback) {
						vecEnemy[j]->blood = 0;
						vecEnemy[j]->live = false;
						signalv = 1;
					}
					else if (vecEnemy[j]->movex == pxback && vecEnemy[j]->movey == pyfront) {
						vecEnemy[j]->blood = 0;
						vecEnemy[j]->live = false;
						signalv = 1;
					}
				}
			}
			if (signalv == 1) {
				vecRedoilbarrel[i]->ifboom = true;
				if (canbeshovle[px][py] == 1) {
					canbeshovle1[px][py] = 1;
				}
				if (canbeshovle[px][pyfront] == 1) {
					canbeshovle1[px][pyfront] = 1;
				}
				if (canbeshovle[px][pyback] == 1) {
					canbeshovle1[px][pyback] = 1;
				}
				if (canbeshovle[pxfront][py] == 1) {
					canbeshovle1[pxfront][py] = 1;
				}
				if (canbeshovle[pxback][py] == 1) {
					canbeshovle1[pxback][py] = 1;
				}
				if (canbeshovle[pxfront][pyfront] == 1) {
					canbeshovle1[pxfront][pyfront] = 1;
				}
				if (canbeshovle[pxfront][pyback] == 1) {
					canbeshovle1[pxfront][pyback] = 1;
				}
				if (canbeshovle[pxback][pyfront] == 1) {
					canbeshovle1[pxback][pyfront] = 1;
				}
				if (canbeshovle[pxback][pyback] == 1) {
					canbeshovle1[pxback][pyback] = 1;
				}
			}
		}
	}
}
void checkduckdamage() {
	for (int i = 0; i < vecduck.size(); i++) {
		if (vecduck[i]->iftotheend== false) {
			int px = vecduck[i]->x;
			int py = vecduck[i]->y;
			int pxfront = px - 1;
			int pxback = px + 1;
			int pyfront = py - 1;
			int pyback = py + 1;
			for (int j = 0; j < vecEnemy.size(); j++) {
				if (vecEnemy[j]->live == true) {
					if (vecEnemy[j]->movex == px && vecEnemy[j]->movey == py) {
						vecEnemy[j]->blood = 0;
						vecEnemy[j]->live = false;
					}
					else if (vecEnemy[j]->movex == px && vecEnemy[j]->movey == pyfront) {
						vecEnemy[j]->blood = 0;
						vecEnemy[j]->live = false;
					}
					else if (vecEnemy[j]->movex == px && vecEnemy[j]->movey == pyback) {
						vecEnemy[j]->blood = 0;
						vecEnemy[j]->live = false;
					}
					else if (vecEnemy[j]->movex == pxfront && vecEnemy[j]->movey == py) {
						vecEnemy[j]->blood = 0;
						vecEnemy[j]->live = false;
					}
					else if (vecEnemy[j]->movex == pxback && vecEnemy[j]->movey == py) {
						vecEnemy[j]->blood = 0;
						vecEnemy[j]->live = false;
					}
					else if (vecEnemy[j]->movex == pxfront && vecEnemy[j]->movey == pyfront) {
						vecEnemy[j]->blood = 0;
						vecEnemy[j]->live = false;
					}
					else if (vecEnemy[j]->movex == pxfront && vecEnemy[j]->movey == pyback) {
						vecEnemy[j]->blood = 0;
						vecEnemy[j]->live = false;
					}
					else if (vecEnemy[j]->movex == pxback && vecEnemy[j]->movey == pyback) {
						vecEnemy[j]->blood = 0;
						vecEnemy[j]->live = false;
					}
					else if (vecEnemy[j]->movex == pxback && vecEnemy[j]->movey == pyfront) {
						vecEnemy[j]->blood = 0;
						vecEnemy[j]->live = false;
					}
				}
			}
		}
	}
}
void oneactionattack(double timery, bool&flagv) {
	for (int i = 0; i < vecRolestone.size(); i++) {
		if (vecRolestone[i]->live && vecRolestone[i]->ifdrawback == false) {
			for (int j = 0; j < vecEnemy.size(); j++) {
				if (vecEnemy[j]->live) {
					if (vecRolestone[i]->blocknum < vecRolestone[i]->blockmaxnum) {
						int x = vecEnemy[j]->movex;
						int y = vecEnemy[j]->movey;
						int sx = vecRolestone[i]->x;
						int sy = vecRolestone[i]->y;
						int xfront = sx - 1;
						int xback = sx + 1;
						int yfront = sy - 1;
						int yback = sy + 1;
						int sign = 0;
						for (int y = 0; y < vecRolestone[i]->enemyjump.size(); y++) {
							if (vecRolestone[i]->enemyjump[y]->countnumber == vecEnemy[j]->countnumber) {
								sign = 1;
							}
						}
						if (sign == 0) {
							if (vecEnemy[j]->ifbeblocked == false) {
								if ((xfront == x && sy == y) || (xback == x && sy == y) || (sx == x && yfront == y) || (sx == x && yback == y)) {
									if (vecEnemy[j]->listnum == 3 && vecEnemy[j]->ifjump == false) {
										vecEnemy[j]->ifjump = true;
										jumpsign = true;
										vecRolestone[i]->enemyjump.push_back(vecEnemy[j]);
									}
									else {
										vecEnemy[j]->ifbeblocked = true;
										vecRolestone[i]->blocknum++;
										vecRolestone[i]->enemybeblocked.push_back(vecEnemy[j]);
									}
								}
							}
						}
					}
				}
			}
			for (int k = 0; k < vecRolestone[i]->enemybeblocked.size(); k++) {
				if (vecRolestone[i]->enemybeblocked[k]->live) {
					vecRolestone[i]->block(vecRolestone[i]->enemybeblocked[k], timery, flagv);
				}
				else if (vecRolestone[i]->enemybeblocked[k]->live == false && vecRolestone[i]->enemybeblocked[k]->ifbekiiledatstone == false) {
					vecRolestone[i]->enemybeblocked[k]->ifbekiiledatstone = true;
					vecRolestone[i]->blocknum--;
				}
			}
			if (vecRolestone[i]->blood <= 0 || vecRolestone[i]->ifdrawback == true) {
				for (int k = 0; k < vecRolestone[i]->enemybeblocked.size(); k++) {
					if (vecRolestone[i]->enemybeblocked[k]->live) {
						vecRolestone[i]->enemybeblocked[k]->ifbeblocked = false;
					}
				}
				vecRolestone[i]->live = false;
			}
		}
	}
}
void ghostaction(double timery,bool&flagr) {
	for (int i = 0; i < vecEnemy.size(); i++) {
		if (vecEnemy[i]->live) {
			int signal = 0;
			if (vecEnemy[i]->listnum == 5) {
				int x = vecEnemy[i]->movex;
				int y = vecEnemy[i]->movey;
				for (int j = 0; j < vecRole1.size(); j++) {
					if (vecRole1[j]->live&&vecRole1[j]->ifdrawback==false) {
						int rx = vecRole1[j]->x;
						int ry = vecRole1[j]->y;
						if ((rx == x - 1 && ry == y) || (rx == x - 2 && ry == y)) {
							vecEnemy[i]->shoot = 2;
							vecEnemy[i]->startwalk = false;
							signal = 1;
						}
						if ((rx == x + 1 && ry == y) || (rx == x + 2 && ry == y)) {
							vecEnemy[i]->shoot = 1;
							vecEnemy[i]->startwalk = false;
							signal = 1;
						}
						if ((rx == x && ry == y - 1) || (rx == x && ry == y - 2)) {
							vecEnemy[i]->shoot = 3;
							vecEnemy[i]->startwalk = false;
							signal = 1;
						}
						if ((rx == x && ry == y + 1) || (rx == x && ry == y + 2)) {
							vecEnemy[i]->shoot = 4;
							vecEnemy[i]->startwalk = false;
							signal = 1;
						}
					}
				}
				for (int k = 0; k < vecRolestone.size(); k++) {
					if (vecRolestone[k]->live&&vecRolestone[k]->ifdrawback==false) {
						int rx = vecRolestone[k]->x;
						int ry = vecRolestone[k]->y;
						if ((rx == x - 1 && ry == y) || (rx == x - 2 && ry == y)) {
							vecEnemy[i]->shoot = 2;
							vecEnemy[i]->startwalk = false;
							signal = 1;
						}
						if ((rx == x + 1 && ry == y) || (rx == x + 2 && ry == y)) {
							vecEnemy[i]->shoot = 1;
							vecEnemy[i]->startwalk = false;
							signal = 1;
						}
						if ((rx == x && ry == y - 1) || (rx == x && ry == y - 2)) {
							vecEnemy[i]->shoot = 3;
							vecEnemy[i]->startwalk = false;
							signal = 1;
						}
						if ((rx == x && ry == y + 1) || (rx == x && ry == y + 2)) {
							vecEnemy[i]->shoot = 4;
							vecEnemy[i]->startwalk = false;
							signal = 1;
						}
					}
				}
				if (signal == 0) {
					vecEnemy[i]->ifattacky = false;
				}
				else if (signal == 1) {
					vecEnemy[i]->ifattacky = true;
				}
			}
		}
	}
	for (int i = 0; i < vecEnemy.size(); i++) {
		if (vecEnemy[i]->live) {
			if (vecEnemy[i]->listnum == 5) {
				if (vecEnemy[i]->ifattacky == true) {
					vecEnemy[i]->ifbeblocked = true;
				}
				else if(vecEnemy[i]->ifattacky == false){
					vecEnemy[i]->ifbeblocked = false;
				}
			}
		}
	}
	for (int i = 0; i < vecEnemy.size(); i++) {
		if (vecEnemy[i]->live) {
			if (vecEnemy[i]->listnum == 5) {
				if(vecEnemy[i]->ifbeblocked ){
					if (timery >= stime) {
						Bullet* p = new Bullet;
						p->x = vecEnemy[i]->movex;
						p->y = vecEnemy[i]->movey;
						if (vecEnemy[i]->shoot == 1) {
							p->endx = p->x + 2;
							p->endy = p->y;
						}
						if (vecEnemy[i]->shoot == 2) {
							p->endx = p->x - 2;
							p->endy = p->y;
						}
						if (vecEnemy[i]->shoot == 3) {
							p->endx = p->x;
							p->endy = p->y - 2;
						}
						if (vecEnemy[i]->shoot == 4) {
							p->endx = p->x;
							p->endy = p->y + 2;
						}
						p->damage = vecEnemy[i]->enemypower;
						vecbullet.push_back(p);
						flagr = true;
					}
				}
			}
		}
	}
}
void ghostattack(double timery,bool&flagq) {
	for (int i = 0; i < vecbullet.size(); i++) {
		int x = vecbullet[i]->x;
		int y = vecbullet[i]->y;
		for (int j = 0; j < vecRole1.size(); j++) {
			if (vecRole1[j]->live && vecRole1[j]->ifdrawback == false) {
				int rx = vecRole1[j]->x;
				int ry = vecRole1[j]->y;
				if ((rx == x && ry == y) || (rx == x - 1 && ry == y) || (rx == x + 1 && ry == y) || (rx == x && ry == y - 1) || (rx == x && ry == y + 1)) {
					if (timery >= stime) {
						vecRole1[j]->blood = vecRole1[j]->blood - vecbullet[i]->damage;
						if (vecRole1[j]->blood <= 0) {
							vecRole1[j]->live = false;
						}
						vecbullet[i]->ifcatch = true;
						flagq = true;
					}
				}
			}
		}
		for (int k = 0; k < vecRolestone.size(); k++) {
			if (vecRolestone[k]->live && vecRolestone[k]->ifdrawback == false) {
				int rx = vecRolestone[k]->x;
				int ry = vecRolestone[k]->y;
				if ((rx == x && ry == y) || (rx == x - 1 && ry == y) || (rx == x + 1 && ry == y) || (rx == x && ry == y - 1) || (rx == x && ry == y + 1)) {
					if (timery >= stime) {
						vecRolestone[k]->blood = vecRolestone[k]->blood - vecbullet[i]->damage;
						if (vecRolestone[k]->blood <= 0) {
							vecRolestone[k]->live = false;
						}
						vecbullet[i]->ifcatch = true;
						flagq = true;
					}
				}
			}
		}
	}
}
bool wincheck(int maxEnemy,int barriercheck) {
	if (vecEnemy.size() == maxEnemy) {
		int counter = 0;
		for (int i = 0; i < vecEnemy.size(); i++) {
			if (vecEnemy[i]->blood > 0 && vecEnemy[i]->movex == endpositionx && vecEnemy[i]->movey == endpositiony) {
				counter++;
			}
		}
		if (counter < barriercheck&&vecEnemy[vecEnemy.size() - 1]->blood<=0) {
			MessageBox(NULL,L"恭喜",L"你过关", MB_OK);
			return false;
		}
		else if(counter>=barriercheck){
			MessageBox(NULL, L"很遗憾", L"未过关", MB_OK);
			return false;
		}
	}
	else {
		int counter = 0;
		for (int i = 0; i < vecEnemy.size(); i++) {
			if (vecEnemy[i]->blood > 0&& vecEnemy[i]->movex==endpositionx&& vecEnemy[i]->movey==endpositiony) {
				counter++;
			}
		}
		if (counter >= barriercheck) {
			MessageBox(NULL, L"很遗憾", L"未过关", MB_OK);
			return false;
		}
	}
}
struct base {
	int money;
};
void  read() {
	ifstream file;
	file.open("person1.dat", std::ios::binary); 
	if (!file.is_open()) {
		cout << "打开失败";
	}
	base p;
	file.read(reinterpret_cast<char*>(&p), sizeof(base));
	money = p.money;
	file.close();
}
void save() {
	ofstream file;
    file.open("person1.dat", std::ios::binary); 
	base p = { money };
	file.write(reinterpret_cast<const char*>(&p), sizeof(base));
	file.close();
}
int main() {
	double duringtime1,duringtime2,duringtime3,duringtime4,duringtime5,duringtime6,durbefore,duringtime7,duringtime8,duringtime9,duringtime10,duringtime11;
	clock_t startbefore,start, end,start1,end1,start2,end2,start3,end3,start4,end4,start5,end5,endhinter,start6,end6,start7,end7,start8,end8,start9,end9,start10,end10;
	startbefore = clock();
	start = clock();
	start1 = clock();
	start2 = clock();
	start3 = clock();
	start4 = clock();
	start5 = clock();
	start6 = clock();
	start7 = clock();
	start8 = clock();
	start9 = clock();
	start10 = clock();
	int maxEnemy = 100;//最大出怪量
	int barriercheck = 3;//关卡生命
	int initalmoney = 10;
	Animation* chicken = new Animation(_T("chicken%d.png"), 4, 4);
	money = initalmoney;
	Role* virtualrole=new Role;
	Role1* virtualrole1=new Role1;
	Role2* virtualrole2 = new Role2;
	Rolestone* virtualrole3 = new Rolestone;
	initgraph(625, 675);
	loadpicture();
	enemyimage[0] = image9;
	enemyimage[1] = image10;
	srand((unsigned int)time(NULL));
	bool running=false;
	ExMessage m;
	int x = 0;
	int y = 0;
	int flag = 0;
	int flag1 = 0;
	int flag2 = 0;
	int flag3 = 0;
	int heartbuffer = 0;
	int speedup = 0;
	int medcinebuffer = 0;//可以调整
	bool iftick = false;
	bool iftickshovel = false;
	bool iftickmagic = false;
	bool iftickegg = false;
	bool ifhaveegg = false;
	bool ifexistegg = false;
	bool timesign = false;
	bool changeimagex = false;
	int clickegg = 0;
	//bool flagx = false;
	putimage(0, 0, &image37);
	putimage_1(200, 400, &image38);
	putimage_1(200, 200, &image39);
	while (1) {
		m = getmessage(EX_MOUSE);
		if (m.message == WM_LBUTTONDOWN) {
			if (200<= m.x && m.x <= 400 && m.y >= 400 && m.y <= 500) {
				running = true;
				break;
			}
			if (200 <= m.x && m.x <= 400 && m.y >= 200 && m.y <= 300) {
				read();
				running = true;
				break;
			}
		}
	}
	BeginBatchDraw();
	endhinter = clock();
	durbefore = double(endhinter - startbefore);
	durbefore = durbefore / CLOCKS_PER_SEC;
	while (running) {
		DWORD starttime = GetTickCount();
		for (int i = 0; i < vecEnemy.size(); i++) {
			if (vecEnemy[i]->live == true) {
				enemyexistmap[vecEnemy[i]->movex][vecEnemy[i]->movey] = 1;
			}
		}
		bool flagx = false;
		bool flagz = false;
		bool flagv = false;
		bool flagq = false;
		bool flagr = false;
		bool carsign = false;
		bool duckshow = false;
		jumpsign = false;
		while (peekmessage(&m)) {
			if (m.message == WM_LBUTTONDOWN) {
				if ((m.x <= 50) && (m.y <= 675) && (625 <= m.y)) {
					x = m.x;
					y = m.y;
					flag =1;
				}
				if ((50 < m.x) && (m.x <= 100) && (m.y <= 675) && (625 <= m.y)) {
					x = m.x;
					y = m.y;
					flag1 = 1;
				}
				if ((100 < m.x) && (m.x <= 150) && (m.y <= 675) && (625 <= m.y)) {
					x = m.x;
					y = m.y;
					flag2 = 1;
				}
				if ((250 < m.x) && (m.x <= 300) && (m.y <= 675) && (625 <= m.y)) {
					x = m.x;
					y = m.y;
					flag3 = 1;
				}
				if (endpositionx * 25 < m.x && m.x <= endpositionx * 25 + 25 && endpositiony * 25 < m.y && m.y <= endpositiony * 25 + 25&&ifexistegg&&clickegg<=3) {
					clickegg++;
					if (clickegg == 3) {
						ifexistegg = false;
						duckshow = true;
						clickegg = 0;
					}
				}
				if ((375 < m.x) && (m.x <= 425) && (m.y <= 675) && (625 <= m.y)) {
					iftick = true;
				}//撤退图标
				if (150 < m.x && m.x <= 200 && (m.y <= 675) && (625 <= m.y)) {
					iftickshovel = true;
				}//铲子
				if (200 < m.x && m.x <= 250 && (m.y <= 675) && (625 <= m.y)&&medcinebuffer==3) {
					iftickmagic = true;
				}
				if (300 < m.x && m.x <= 350 && (m.y <= 675) && (625 <= m.y) && ifhaveegg == true) {
					iftickegg = true;
				}
				if (checkbuffer[m.x / 25][m.y/25] == 1) {
					heartbuffer++;
					for (int i = 0; i < vecEnemy.size(); i++) {
						if (vecEnemy[i]->live ==false&& vecEnemy[i]->checked==false) {
							if (vecEnemy[i]->vec.size() == 2) {
								if (vecEnemy[i]->vec[0] == m.x / 25 && vecEnemy[i]->vec[1] == m.y / 25) {
									vecEnemy[i]->checked = true;
								}
							}
						}
					}
					checkbuffer[m.x / 25][m.y / 25] = 0;
				}
				if (checkbuffer1[m.x / 25][m.y / 25] == 1) {
					speedup++;
					for (int i = 0; i < vecEnemy.size(); i++) {
						if (vecEnemy[i]->live == false && vecEnemy[i]->checked1 == false) {
							if (vecEnemy[i]->vec1.size() == 2) {
								if (vecEnemy[i]->vec1[0] == m.x / 25 && vecEnemy[i]->vec1[1] == m.y / 25) {
									vecEnemy[i]->checked1 = true;
								}
						    }
						}
					}
					checkbuffer1[m.x / 25][m.y / 25] = 0;
				}
				if (checkbuffer2[m.x / 25][m.y / 25] == 1&& medcinebuffer < 3) {
					medcinebuffer++;
					for (int i = 0; i < vecEnemy.size(); i++) {
						if (vecEnemy[i]->live == false && vecEnemy[i]->checked2 == false) {
							if (vecEnemy[i]->vec2.size() == 2) {
								if (vecEnemy[i]->vec2[0] == m.x / 25 && vecEnemy[i]->vec2[1] == m.y / 25) {
									vecEnemy[i]->checked2 = true;
								}
							}
						}
					}
					checkbuffer2[m.x / 25][m.y / 25] = 0;
				}
				if (m.x <= 50 && m.y <= 50) {
					save();
				}//存取标志
			}
			else if (m.message == WM_MOUSEMOVE) {
				    x = m.x;
					y = m.y;
			}
			else if (m.message == WM_LBUTTONUP&&flag==1&&map[x/25][y/25]!=1&&y<=625&&money>=virtualrole->cost) {
				if (((x / 25 == 13 && y / 25 == 5)||(x/25==13&&y/25==8)||(x/25==12&&y/25==13)||(x/25==11&&y/25==13)
					||(x/25==6&&y/25==15)||(x/25==7&&y/25==15)||(x/25==8&&y/25==15)||(x/25==9&&y/25==16)||(x/25==10&&y/25==16)||(x/25==11&&y/25==16))||canbeshovle1[x/25][y/25]==1) {
					flag = 0;
					int judgesign = 0;
					for (int i = 0; i < vecRole.size(); i++) {
						if (vecRole[i]->ifdrawback == false) {
							if (x / 25 == vecRole[i]->x && y / 25 == vecRole[i]->y) {
								judgesign = 1;
							}
						}
					}
					if (judgesign == 0) {
						money = money - virtualrole->cost;
						Role* p = new Role;
						p->x = x / 25;
						p->y = y / 25;
						p->attack=p->attack+ 2 * speedup;
						vecRole.push_back(p);
					}
				}
			}//放置role
			else if (m.message== WM_LBUTTONUP && flag1 == 1 && map[x / 25][y / 25] == 1 && y <= 625&& money >= virtualrole1->cost){
				flag1 = 0;
				int judgesign1 = 0;
				for (int i = 0; i < vecRole1.size(); i++) {
					if (vecRole1[i]->ifdrawback == false) {
						if (vecRole1[i]->live) {
							if (x / 25 == vecRole1[i]->x && y / 25 == vecRole1[i]->y) {
								judgesign1 = 1;
							}
						}
					}
				}
				if (judgesign1 == 0) {
					money = money - virtualrole1->cost;
					Role1* p = new Role1;
					p->x = x / 25;
					p->y = y / 25;
					p->rolepower= p->rolepower+2* speedup;
					p->blood = p->blood + 10 * heartbuffer;
					int ret1 = rand() % 100 + 1;
					if (67 <= ret1 && ret1 <= 80) {
						p->ifdrop = 1;
						if (ret1 <= 75) {
							p->whichbuffer = 0;
						}
						else {
							p->whichbuffer = 1;
						}
					}
					else {
						p->ifdrop = 0;
					}
					vecRole1.push_back(p);
				}
			}//放置role1
			else if (m.message == WM_LBUTTONUP && flag3 == 1 && map[x / 25][y / 25] == 1 && y <= 625 && money >= virtualrole3->cost) {
				flag3 = 0;
				int judgesign3 = 0;
				for (int i = 0; i < vecRolestone.size(); i++) {
					if (vecRolestone[i]->ifdrawback == false) {
						if (vecRolestone[i]->live) {
							if (x / 25 == vecRolestone[i]->x && y / 25 == vecRolestone[i]->y) {
								judgesign3 = 1;
							}
						}
					}
				}
				if (judgesign3 == 0) {
					money = money - virtualrole3->cost;
					Rolestone* p = new Rolestone;
					p->x = x / 25;
					p->y = y / 25;
					vecRolestone.push_back(p);
				}
			}//放置stone
			else if (m.message == WM_LBUTTONUP && flag2 == 1&&money>=virtualrole2->cost&&(ifground[x/25][y/25]==1|| canbeshovle1[x / 25][y / 25] == 1)) {
				flag2 = 0;
				int judgesign2 =0;
				for (int i = 0; i < vecRole2.size(); i++) {
					if (vecRole2[i]->ifdrawback == false) {
						if (x / 25 == vecRole2[i]->x && y / 25 == vecRole2[i]->y) {
							judgesign2 = 1;
						}
					}
				}
				if (judgesign2 == 0) {
					money = money - virtualrole2->cost;
					Role2* p = new Role2;
					p->x = x / 25;
					p->y = y / 25;
					vecRole2.push_back(p);
				}
			}//放置role2
			else if (m.message == WM_LBUTTONUP && iftick == true) {
				for (int i = 0; i < vecRole.size(); i++) {
					if (x / 25 == vecRole[i]->x && y / 25 == vecRole[i]->y) {
						vecRole[i]->ifdrawback = true;
					}
				}
				for (int i = 0; i < vecRole1.size(); i++) {
					if (x / 25 == vecRole1[i]->x && y / 25 == vecRole1[i]->y) {
						if (vecRole1[i]->blocknum == 0) {
							vecRole1[i]->ifdrawback = true;
						}
						else if(vecRole1[i]->blocknum > 0){
							for (int k = 0; k < vecRole1[i]->enemyattacked.size(); k++) {
								vecRole1[i]->enemyattacked[k]->ifbeblocked = false;
							}
							vecRole1[i]->ifdrawback = true;
						}
					}
				}
				for (int i = 0; i < vecRole2.size(); i++) {
					if (x / 25 == vecRole2[i]->x && y / 25 == vecRole2[i]->y) {
						vecRole2[i]->ifdrawback = true;
					}
				}
				for (int i = 0; i < vecRolestone.size(); i++) {
					if (x / 25 == vecRolestone[i]->x && y / 25 == vecRolestone[i]->y) {
						vecRolestone[i]->ifdrawback = true;
					}
				}
				iftick = false;
			}//撤退图标
			else if (m.message == WM_LBUTTONUP && iftickshovel==true) {
				if (x / 25 >= 25 || y / 25 >= 25) {
					iftickshovel = false;
				}
				else if (canbeshovle[x / 25][y / 25]==1 && money>=shovlecost) {
					canbeshovle[x / 25][y / 25] = 0;
					canbeshovle1[x / 25][y / 25] = 1;
					money = money - shovlecost;
				}
				iftickshovel = false;
			}
			else if (m.message == WM_LBUTTONUP && iftickmagic == true) {
				if (x / 25 >= 25 || y / 25 >= 25) {
					iftickmagic= false;
				}
				else if (map[x / 25][y / 25] == 1&&ifhavemagic[x/25][y/25]==0) {
					ifhavemagic[x / 25][y / 25] = 1;
					medcinebuffer = 0;
					createredoilbarrel(x / 25, y / 25);
				}
				iftickmagic = false;
			}
			else if (m.message == WM_LBUTTONUP && iftickegg == true) {
				if (x / 25 >= 25 || y / 25 >= 25) {
					iftickegg = false;
				}
				else if (x / 25 == endpositionx&& y / 25 == endpositiony&&carsign==false&&ifexistegg==false) {
					carsign = true;
					ifexistegg = true;
				}
				iftickegg = false;
			}
		}
		showenemy(maxEnemy);
		/*end1 = clock();
		duringtime2 = double(end1 - start1);
		actionattack(duringtime2 / CLOCKS_PER_SEC,flagx);
		if (flagx == true){
			start1 = clock();
		}*///画图逻辑要放在后面
		if (duckshow) {
			Duck* p = new Duck;
			p->x = endpositionx;
			p->y = endpositiony;
			vecduck.push_back(p);
		}
		end6 = clock();
		duringtime7 = double(end6- start6);
		for (int i = 0; i < vecduck.size(); i++) {
			if (vecduck[i]->iftotheend == false) {
				if (duringtime7 / CLOCKS_PER_SEC >= 1) {
					moveDuck(vecduck[i]);
					start6 = clock();
				}
			}
		}
		end = clock();
		duringtime1 = double(end - start);
		for (int i = 0; i < vecEnemy.size(); i++) {
			if (duringtime1 / CLOCKS_PER_SEC >= 1) {
				moveEnemy(vecEnemy[i]);
				start = clock();
			}
		}
		/*end8 = clock();
		duringtime9 = double(end8 - start8);
		for (int i = 0; i < vecbullet.size(); i++) {
			if (vecbullet[i]->ifcatch == false) {
				if (duringtime9 / CLOCKS_PER_SEC >= 1) {
					moveBullet(vecbullet[i]);
					start8 = clock();
				}
			}
		}*/
		for (int i = 0; i < vecbullet.size(); i++) {
			if (vecbullet[i]->ifcatch == false) {
				moveBullet(vecbullet[i]);
			}
		}
		end3 = clock();
		duringtime4 = double(end3 - start3);
		for (int i = 0; i < vecRole2.size(); i++) {
			if (vecRole2[i]->ifdrawback == false) {
				if (duringtime4 / CLOCKS_PER_SEC >= 1) {
					vecRole2[i]->makemoney();
					start3 = clock();
				}
			}
		}
		checkboom();
		checkduckdamage();
		end5 = clock();
		duringtime6 = double(end5 - start5);
		double duringtimeq=0;
		if (timesign == false) { duringtimeq = duringtime6 / CLOCKS_PER_SEC - durbefore; }
		else { duringtimeq = duringtime6 / CLOCKS_PER_SEC; }
		if (duringtimeq>= eggtime) {
			timesign = true;
			if (carsign) {
				start5 = clock();
			}
			changeimage = true; 
			ifhaveegg = true;
		}///egg
		else {
			changeimage = false;
		}//egg计时器
		end7 = clock();
		duringtime8 = double(end7 - start7);
		double duringtimeq1 = 0;
		duringtimeq1 = duringtime8 / CLOCKS_PER_SEC - durbefore;
		if (duringtimeq1 >= 10) {
			for (int i = 0; i < vecEnemy.size(); i++) {
				if (vecEnemy[i]->listnum == 5) {
					vecEnemy[i]->startwalk = true;
				}
			}
			start7 = clock();
		}
		cleardevice();
		drawmap();
		if (ifexistegg) {
			putimage_1(endpositionx * 25, endpositiony * 25, &image58);
		}
		if (iftick) {
			putimage_1(370, 620, &image28);
			putimage_1(375, 625, &image27);
		}
		else {
			putimage_1(375, 625, &image27);
		}//drawback
		if (iftickshovel) {
			if (money >= shovlecost) { 
				putimage_1(145, 620, &image28);
				putimage(150, 625, &image46); 
			}
			else { putimage(150, 625, &image41); }
		}
		else {
			if (money >= shovlecost) {
				putimage(150, 625, &image46);
			}
			else { putimage(150, 625, &image41); }
		}//铲子
		if (changeimage == true) {
			if (iftickegg) {
				putimage_1(295, 620, &image28);
				putimage(300, 625, &image57);
			}
			else {
				putimage(300, 625, &image57);
			}
		}
		else {
			putimage(300, 625, &image56);
		}
		for (int i = 0; i < 25; i++) {
			for (int j = 0; j < 25; j++) {
				if (canbeshovle1[i][j] == 1) {
					putimage_1(i * 25, j * 25, &image15);
					canbeshovle[i][j] = 0;
				}
			}
		}
		if (medcinebuffer == 0) {
			putimage(200, 625, &image42);
		}
		else if (medcinebuffer == 1) {
			putimage(200, 625, &image49);
		}
		else if (medcinebuffer == 2) {
			putimage(200, 625, &image50);
		}
		else if (medcinebuffer == 3) {
			putimage(200, 625, &image47);
		}
		if (iftickmagic) {
			putimage_1(195, 620, &image28);
			putimage(200, 625, &image47);
		}
		end1 = clock();
		duringtime2 = double(end1 - start1);
		actionattack(duringtime2 / CLOCKS_PER_SEC, flagx);
		if (flagx == true) {
			start1 = clock();
		}
		end2 = clock();
		duringtime3 = double(end2 - start2);
		twoactionattack(duringtime3 / CLOCKS_PER_SEC, flagz);
		if (flagz == true) {
			start2 = clock();
		}
		end4 = clock();
		duringtime5 = double(end4 - start4);
		oneactionattack(duringtime5 / CLOCKS_PER_SEC, flagv);
		if (flagv == true) {
			start4 = clock();
		}
		end10 = clock();
		duringtime11 = double(end10 - start10);
		ghostaction(duringtime11 / CLOCKS_PER_SEC, flagr);
		if (flagr == true) {
			start10 = clock();
		}
		end9 = clock();
		duringtime10 = double(end9 - start9);
		ghostattack(duringtime10 / CLOCKS_PER_SEC, flagq);
		if (flagq == true) {
			start9 = clock();
		}
		for (int i = 0; i < vecbullet.size(); i++) {
			if (vecbullet[i]->x == vecbullet[i]->endx && vecbullet[i]->y == vecbullet[i]->endy) {
				vecbullet[i]->ifcatch = true;
				putimage_1(vecbullet[i]->x * 25, vecbullet[i]->y * 25, &image61);
			}
			if (vecbullet[i]->ifcatch == false) {
				putimage_1(vecbullet[i]->x*25, vecbullet[i]->y*25, &image60);
			}
		}///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////bullet
		for (int i = 0; i < vecduck.size(); i++) {
			if (vecduck[i]->iftotheend == false) {
				vecduck[i]->draw(1);
			}
		}
		for (int i = 0; i < vecRolestone.size(); i++) {
			if (vecRolestone[i]->live && vecRolestone[i]->ifdrawback == false) {
				putimage_1(vecRolestone[i]->x * 25, vecRolestone[i]->y * 25, &image54);
			}
		}
		for (int i = 0; i < vecRedoilbarrel.size(); i++) {
			if (vecRedoilbarrel[i]->ifboom == false&&vecRedoilbarrel[i]->beused==false) {
				putimage_1(vecRedoilbarrel[i]->x * 25, vecRedoilbarrel[i]->y * 25, &image52);
			}
			if (vecRedoilbarrel[i]->ifboom == true&&vecRedoilbarrel[i]->beused==false&&vecRedoilbarrel[i]->boomtime<=80) {
				vecRedoilbarrel[i]->draw(1);
				ifhavemagic[vecRedoilbarrel[i]->x][vecRedoilbarrel[i]->y] = 0;
				vecRedoilbarrel[i]->boomtime++;
				if (vecRedoilbarrel[i]->boomtime == 80) {
					vecRedoilbarrel[i]->beused = true;
				}
			}
		}
		for (int i = 0; i < vecEnemy.size(); i++) {
			if (vecEnemy[i]->live) {
				double bloodrate = vecEnemy[i]->blood / vecEnemy[i]->initialblood;
				if (bloodrate >= 0.8) {
					putimage_1(vecEnemy[i]->movex*25, vecEnemy[i]->movey*25, &image32);
				}
				else if (bloodrate < 0.8 && bloodrate >= 0.6) {
					putimage_1(vecEnemy[i]->movex*25, vecEnemy[i]->movey * 25, &image33);
				}
				else if (bloodrate < 0.6 && bloodrate >= 0.4) {
					putimage_1(vecEnemy[i]->movex * 25, vecEnemy[i]->movey * 25, &image34);
				}
				else if (bloodrate < 0.4 && bloodrate >= 0.2) {
					putimage_1(vecEnemy[i]->movex * 25, vecEnemy[i]->movey * 25, &image35);
				}
				else if (bloodrate < 0.2 && bloodrate >= 0) {
					putimage_1(vecEnemy[i]->movex * 25, vecEnemy[i]->movey * 25, &image36);
				}
			}
		}
		for (int i = 0; i < vecRole1.size(); i++) {
			if (vecRole1[i]->live&&vecRole1[i]->ifdrawback==false) {
				double bloodrate = vecRole1[i]->blood / vecRole1[i]->initialblood;
				if (bloodrate >= 0.8) {
					putimage_1(vecRole1[i]->x * 25, vecRole1[i]->y * 25, &image32);
				}
				else if (bloodrate < 0.8 && bloodrate >= 0.6) {
					putimage_1(vecRole1[i]->x * 25, vecRole1[i]->y * 25, &image33);
				}
				else if (bloodrate < 0.6 && bloodrate >= 0.4) {
					putimage_1(vecRole1[i]->x * 25, vecRole1[i]->y * 25, &image34);
				}
				else if (bloodrate < 0.4 && bloodrate >= 0.2) {
					putimage_1(vecRole1[i]->x * 25, vecRole1[i]->y * 25, &image35);
				}
				else if (bloodrate < 0.2 && bloodrate >= 0) {
					putimage_1(vecRole1[i]->x * 25, vecRole1[i]->y * 25, &image36);
				}
			}
		}
		for (int i = 0; i < vecRolestone.size(); i++) {
			if (vecRolestone[i]->live && vecRolestone[i]->ifdrawback == false) {
				double bloodrate = vecRolestone[i]->blood / vecRolestone[i]->initialblood;
				if (bloodrate >= 0.8) {
					putimage_1(vecRolestone[i]->x * 25, vecRolestone[i]->y * 25, &image32);
				}
				else if (bloodrate < 0.8 && bloodrate >= 0.6) {
					putimage_1(vecRolestone[i]->x * 25, vecRolestone[i]->y * 25, &image33);
				}
				else if (bloodrate < 0.6 && bloodrate >= 0.4) {
					putimage_1(vecRolestone[i]->x * 25, vecRolestone[i]->y * 25, &image34);
				}
				else if (bloodrate < 0.4 && bloodrate >= 0.2) {
					putimage_1(vecRolestone[i]->x * 25, vecRolestone[i]->y * 25, &image35);
				}
				else if (bloodrate < 0.2 && bloodrate >= 0) {
					putimage_1(vecRolestone[i]->x * 25, vecRolestone[i]->y * 25, &image36);
				}
			}
		}
		for (int i = 0; i < vecEnemy.size(); i++) {
			int flagf = 0;
			if (vecEnemy[i]->live) {
				for (int g = 0; g < vecRolestone.size(); g++) {
					if (vecRolestone[g]->live && vecRolestone[g]->ifdrawback == false) {
						for (int w = 0; w < vecRolestone[g]->enemybeblocked.size(); w++) {
							if (vecRolestone[g]->enemybeblocked[w]->countnumber == vecEnemy[i]->countnumber) {
								flagf = 1;
							}
						}
					}
				}
				for (int k = 0; k < vecRole1.size(); k++) {
					if (vecRole1[k]->ifdrawback == false) {
						if (vecRole1[k]->live == true) {
							for (int j = 0; j < vecRole1[k]->enemyattacked.size(); j++) {
								if (vecRole1[k]->enemyattacked[j]->countnumber == vecEnemy[i]->countnumber) {
									flagf = 1;
								}
							}
						}
					}
				}
				if (flagf == 0) {
					vecEnemy[i]->ifbeblocked = false;
				}
			}
		}
		for (int i = 0; i < vecEnemy.size(); i++) {
			if (vecEnemy[i]->live && vecEnemy[i]->getbuffer) {
				vecEnemy[i]->improve++;
				if (vecEnemy[i]->getbuffer1==true) {
					vecEnemy[i]->armor = vecEnemy[i]->armor + vecEnemy[i]->improve * 2;//获得护甲
					vecEnemy[i]->getbuffer1 = false;
				}
				else if (vecEnemy[i]->getbuffer2 == true) {
					vecEnemy[i]->enemypower = vecEnemy[i]->enemypower + vecEnemy[i]->improve * 2;//提高攻击
					vecEnemy[i]->getbuffer2 = false;
				}
				vecEnemy[i]->getbuffer = false;
			}
		}
		for (int i = 0; i < vecRole2.size(); i++) {
			if (vecRole2[i]->ifdrawback == false) {
				putimage_1(vecRole2[i]->x * 25, vecRole2[i]->y * 25, &image24);
			}
		}
		for (int j = 0; j < vecRole.size(); j++) {
			if (vecRole[j]->ifdrawback == false) {
				ifexistenemy(vecRole[j]);
			}
		}
		for (int i = 0; i < vecRole.size(); i++) {
			if (vecRole[i]->ifdrawback == false) {
				if (vecRole[i]->ifattack == false) {
					putimage_1(vecRole[i]->x * 25, vecRole[i]->y * 25, &image12);
				}
				else if (vecRole[i]->ifattack == true) {
					vecRole[i]->draw(1);
				}
			}
		}
		bool chickenfly = false;
		for (int i = 0; i < vecRole1.size(); i++) {
			//buffer暂时没有持续时间（也可设置）；增加单位血量，加快攻击速度
			if (vecRole1[i]->ifdrawback == false) {
				if (vecRole1[i]->blocknum == 0 && vecRole1[i]->live == true) {
					putimage_1(vecRole1[i]->x * 25, vecRole1[i]->y * 25, &image81);
				}
				else if (vecRole1[i]->blocknum > 0 && vecRole1[i]->live == true) {
					vecRole1[i]->draw(1);
					chickenfly = true;
				}
				else if (vecRole1[i]->live == false && vecRole1[i]->ondeath <= 10) {
					putimage_1(vecRole1[i]->x * 25, vecRole1[i]->y * 25, &image11);
					vecRole1[i]->ondeath++;
				}
				else if (vecRole1[i]->live == false && vecRole1[i]->ondeath1 <= 200 && vecRole1[i]->ifdrop == 1&&vecRole1[i]->checked==false) {
					if (vecRole1[i]->ondeath1 == 0) {
						vecRole1[i]->vec.push_back(vecRole1[i]->x);
						vecRole1[i]->vec.push_back(vecRole1[i]->y);
						vecRole1[i]->vec.push_back(vecRole1[i]->whichbuffer);
					}
					int x = vecRole1[i]->vec[0];
					int y = vecRole1[i]->vec[1];
					enemygetbuffer[x][y] = 1;
					if (vecRole1[i]->whichbuffer == 0) {
						putimage_1(x * 25, y * 25, &image30);
					}
					else if (vecRole1[i]->whichbuffer == 1) {
						putimage_1(x * 25, y * 25, &image31);
					}
					vecRole1[i]->ondeath1++;
				}
			}
		}
		if (chickenfly) {
			chicken->play(13 * 25, 375, 1);
			chicken->play(12 * 25, 400, 1);
		}
		for (int i = 0; i < vecEnemy.size(); i++) {
			if (vecEnemy[i]->live == true&&vecEnemy[i]->ifbeblocked==false) {
				if (vecEnemy[i]->listnum == 5) {
					if (vecEnemy[i]->startwalk == true) {
						vecEnemy[i]->draw(1);
					}
					else {
						if (vecEnemy[i]->onshow <20) {
							vecEnemy[i]->drawshow(1);
							vecEnemy[i]->onshow++;
						}
						if (vecEnemy[i]->onshow >= 20) {
							putimage_1(vecEnemy[i]->x - 12.5, vecEnemy[i]->y - 12.5, &image59);
						}
					}
				}
				else {
					if (vecEnemy[i]->ifjump == false) {
						vecEnemy[i]->draw(1);
					}
					else if (vecEnemy[i]->ifjump == true && vecEnemy[i]->jumpsignal == false) {
						vecEnemy[i]->drawjump(1);
						vecEnemy[i]->jumpsignal = true;
					}
					else if (vecEnemy[i]->ifjump == true && vecEnemy[i]->jumpsignal == true) {
						vecEnemy[i]->draw(1);
					}
				}
			}
			else if (vecEnemy[i]->live == false&&vecEnemy[i]->ondeath<=5&&!((vecEnemy[i]->movex==endpositionx)&&(vecEnemy[i]->movey==endpositiony))) {
				putimage_1(vecEnemy[i]->x-12.5, vecEnemy[i]->y-12.5, &image6);
				vecEnemy[i]->ondeath++;
			}
			else if (vecEnemy[i]->live == false && vecEnemy[i]->ondeath1<= 200&&vecEnemy[i]->ifdrop==1 && !((vecEnemy[i]->movex == endpositionx) && (vecEnemy[i]->movey == endpositiony))&&vecEnemy[i]->checked==false) {
				if (vecEnemy[i]->ondeath1 == 0) {
					vecEnemy[i]->vec.push_back(vecEnemy[i]->movex);
					vecEnemy[i]->vec.push_back(vecEnemy[i]->movey);
				}
				int x = vecEnemy[i]->vec[0];
				int y = vecEnemy[i]->vec[1];
				checkbuffer[x][y] = 1;
				putimage_1(x*25, y*25, &image25);
				vecEnemy[i]->ondeath1++;
			}
			else if (vecEnemy[i]->live == false&& vecEnemy[i]->ondeath2 <= 200&&vecEnemy[i]->ifdrop1==1&& !((vecEnemy[i]->movex == endpositionx) && (vecEnemy[i]->movey == endpositiony)) && vecEnemy[i]->checked1 == false) {
				if (vecEnemy[i]->ondeath2 == 0) {
					vecEnemy[i]->vec1.push_back(vecEnemy[i]->movex);
					vecEnemy[i]->vec1.push_back(vecEnemy[i]->movey);
				}
				int x = vecEnemy[i]->vec1[0];
				int y = vecEnemy[i]->vec1[1];
				checkbuffer1[x][y] = 1;
				putimage_1(x * 25, y * 25, &image26);
				vecEnemy[i]->ondeath2++;
				
			}
			else if (vecEnemy[i]->live == false && vecEnemy[i]->ondeath3 <= 200 && vecEnemy[i]->ifdrop2 == 1 && !((vecEnemy[i]->movex == endpositionx) && (vecEnemy[i]->movey == endpositiony)) && vecEnemy[i]->checked2== false) {
				if (vecEnemy[i]->ondeath3 == 0) {
					vecEnemy[i]->vec2.push_back(vecEnemy[i]->movex);
					vecEnemy[i]->vec2.push_back(vecEnemy[i]->movey);
				}
				int x = vecEnemy[i]->vec2[0];
				int y = vecEnemy[i]->vec2[1];
				checkbuffer2[x][y] = 1;
				putimage_1(x * 25, y * 25, &image48);
				vecEnemy[i]->ondeath3++;
			}
			else if (vecEnemy[i]->live == true && vecEnemy[i]->ifbeblocked == true) {
				if (vecEnemy[i]->listnum == 0) {
					vecEnemy[i]->draw(1);
				}
				else if (vecEnemy[i]->listnum == 1) {
					vecEnemy[i]->drawattack(1);
				}
				else if (vecEnemy[i]->listnum == 2) {
					vecEnemy[i]->drawattack(1);
				}
				else if (vecEnemy[i]->listnum == 3) {
					vecEnemy[i]->drawattack(1);
				}
				else if (vecEnemy[i]->listnum == 4) {
					if (vecEnemy[i]->changeattack) {
						if (vecEnemy[i]->rt < 100) {
							vecEnemy[i]->drawattack1(1);
							vecEnemy[i]->rt++;
						}
						if (vecEnemy[i]->rt == 100) {
							vecEnemy[i]->changeattack = false;
							vecEnemy[i]->rt = 0;
						}
					}
					else {
						vecEnemy[i]->drawattack(1);
					}
				}
				else if (vecEnemy[i]->listnum == 5) {
					vecEnemy[i]->drawattack(10);
				}
			}
			else if (vecEnemy[i]->live == false) {
				money = money + vecEnemy[i]->money;
				vecEnemy[i]->money = 0;
			}
		}
		outtextxy(450,630, s1);
		outtextxy(450, 645, s2);
		outtextxy(550, 645, s3);
		TCHAR s[100];
		_stprintf_s(s, _T("%d"), money);
		TCHAR f[100];
		_stprintf_s(f, _T("%d"), deathnumber);
		TCHAR e[100];
		_stprintf_s(e, _T("%d"), maxEnemy);
		outtextxy(500,630, s);
		outtextxy(530,645, f);
		outtextxy(565, 645, e);
		FlushBatchDraw();
		for (int i = 0; i < 25; i++) {
			for (int j = 0; j < 25; j++) {
				enemyexistmap[i][j] = 0;
			}
		}
		running=wincheck(maxEnemy, barriercheck);
		DWORD endtime = GetTickCount();
		DWORD duringtime = endtime - starttime;
		if (duringtime < 1000 / 60) {
			Sleep(1000 / 60 - duringtime);
		}
	}
	EndBatchDraw();
	save();
}