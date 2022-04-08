/*
개발자 : 2112 윤서연
*/

#include <iostream>
#include <time.h>
#include <cstdlib>
#include <stdlib.h>
#include <iomanip>
#include <windows.h> 
#include <conio.h>
#include<fstream>
#include <string>
// wav 플레이어 헤더파일
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define CARD_NUM 52
#define RED 4
#define BLACK 0
#define WHITE 15
#define GRAY 7

using namespace std;

int select_num;		//1, 2, 3중 선택
int option_num;		//옵션 1, 2, 3 중 선택
char ch;					//1, 2, 3중 선택하면 화면 넘기기

void Game_start();				//게임 시작
void Game_option();			//게임 옵션
void Game_option_o1();		//게임 옵션-1
void Game_option_o2();		//게임 옵션-2
void Game_option_o3();		//게임 옵션-3
void Shuffle(int* Card_num, int num);		//카드 섞기
void textcolor(int foreground, int background);	//색깔
void gotoxy(int x, int y); //좌표
void CursorView(char cursor);			//커서

typedef struct Dealer {				 //딜러
	int dealer_card[20] = { 0, };	 //딜러 카드 담을 것
	int dealer_shape_card[20] = { 0, };  ////K, Q, J, A 카드만 담을 것
	string shape[20] = { };				 //딜러 카드 모양
	int dealer_sum = 0;					//카드의 합
	int dealer_money = 100000;				//딜러 금액
	int d_win = 0;	//딜러 이긴 횟수 합산
	int d_gamewin = 0;	//딜러 미니 게임 이긴 횟수
	int d_gnr = 0;		   //딜러 랜덤 수 돌리기
	int d_gns = 0;		   //딜러 카드 값 저장
};

typedef struct Player {					//플레이어
	int player_card[20] = { 0, };	   //플레이어 카드 담을 것
	int player_shape_card[20] = { 0, };	//K, Q, J, A 카드만 담을 것 
	string shape[20] = { };				   //플레이어 카드 모양
	int player_sum = 0;						//카드의 합	  
	int player_money;					//플레이어 기존 금액
	int player_betting;								//플레이어 배팅 금액
	int p_win = 0;	//플레이어 이긴 횟수 합산
	int Game_cnt = 0;  //처음에만 배팅 금액 나오게 하는 것
	int p_gamewin = 0;	//플레이어 미니 게임 이긴 횟수
	int p_gnr = 0;	  //플레이어 랜덤 수 돌리기
	int p_gns = 0;	  //플레이어 카드 값 저장
	int MiniGame_Card[4];								   //미니게임 카드 숫자 저장
	int MiniGame_Card2[4];								   //미니게임 카드 숫자 저장 2
	string MiniGame_shape[4];							   //미니게임 카드 모양 저장
	string p_inputbetting;
};

void Black_Jack(Player& player, Dealer& dealer);				//본 게임
void P_Card_Random(Player &player, Dealer& delaer, int Card_num[]);		//플레이어에게 카드 랜덤으로 나눠주기, Hit 눌렀을 때 카드 추가
void D_Card_Random(Dealer& dealer, Player& player, int Card_num[]);	//딜러에게 카드 랜덤으로 나눠주기
void PD_Card_Sum(Dealer& dealer, Player& player, char ch);   //딜러 카드 합해서 패배 결정, Stand를 눌렀을 때 무승부 확인하고 카드 합해서 패배 결정
void Surrender(Player& player);	//서렌더(Surrender) : 처음 받은 2장의 카드를 확인하고 "이길 수 없다"라고 판단될 경우 "항복" 하여 베팅의 절반을 돌려받을 수 있습니다.
void H_S_Select();		//Hit와 Stand중 고르기
void D_Card_Add();	   //딜러 카드 뽑기
void P_D_Reset(Player& player, Dealer& dealer);		//게임 종료 시 플레이어/딜러 카드 모양/숫자 초기화
void UpandDown(Player &player, Dealer& dealer);		  //UP&Down 게임
void MiniGame_Card(Player& player, Dealer& dealer, int m, int o);	//미니게임 카드 정해주기
void ShowCard(Player& palyer, Dealer& dealer);	//카드 보여주기
void Show_DCard(Dealer& dealer); //딜러 카드 보여주기
void SRP(Player & player, Dealer & delaer);	  //가위바위보 게임 srp
void Kwangsoo_Game(Player& player, Dealer& dealer);	//이광수 게임
void Dice_Game(Player& player, Dealer& dealer); //주사위 게임
  
struct Dealer dealer;
struct Player player;
int Card_num[52];
ofstream pb_outfile;
ifstream pb_inputfile;

void Black_Jack(Player& player, Dealer& dealer) {			//본 게임
	srand(time(NULL));
	CursorView(false);

	 
	if (player.Game_cnt == 0) {
		
		pb_inputfile.open("black_jack_betting.txt");
		if (!pb_inputfile) {
			cout << "파일이 존재하지 않습니다!" << endl;
		}

		else {
			while (getline(pb_inputfile, player.p_inputbetting)) {
				player.player_money = stoi(player.p_inputbetting);
				gotoxy(55, 20);
				cout << "당신이 현재 가지고 있는 돈 : " << player.player_money << "원";

				Sleep(3000);
				system("cls");
			}
		}

		while (true) {
			textcolor(15, BLACK);
			cout << endl << endl << endl <<
				"\t\t\t\t\t" << ".______" << "     _______" << "  .__________." << " .__________." << "  __" << "  .__    __." << "    _______" << endl <<
				"\t\t\t\t\t" << "|   _  ＼" << "  |  _____|" << " |          |" << " |          |" << " |  |" << " |  ＼ |  |" << "   /  _____|" << endl;
			textcolor(10, BLACK);
			cout <<
				"\t\t\t\t\t" << "|  | )   |" << " |  |__" << "     ---|  |---" << "   ---|  |---" << "  |  |" << " |   ＼|  |" << "  |  |  __" << endl <<
				"\t\t\t\t\t" << "|   _  <" << "   |   __|" << "       |  |" << "         |  |" << "     |  |" << " |        |" << "  |  | |_ |" << endl;
			textcolor(9, BLACK);
			cout << 
				"\t\t\t\t\t" << "|  | )   |" << " |  |____" << "      |  |" << "         |  |" << "     |  |" << " |  |＼   |" << "  |  |__| |" << endl <<
				"\t\t\t\t\t" << "|_______/" << "  |_______|" << "     |__|" << "         |__|" << "     |__|" << " |__| ＼__|" << "  ＼______|";

			textcolor(WHITE, BLACK);
			gotoxy(62, 20);
			cout << "배팅 할 금액을 입력하세요";

			textcolor(15, BLACK);
			gotoxy(66, 21);
			cout << "._____________.";
			textcolor(10, BLACK);
			gotoxy(66, 22);
			cout << "|             |";
			gotoxy(66, 23);
			cout << "|             |";
			gotoxy(66, 24);
			cout << "|             |";
			textcolor(9, BLACK);
			gotoxy(66, 25);
			cout << " ------------- ";
			gotoxy(70, 23);
			cin >> player.player_betting;
			PlaySound(L"coin.wav", 0, SND_FILENAME | SND_ASYNC);

			textcolor(WHITE, BLACK);
			if (player.player_betting < player.player_money) {		//내 베팅 금액과 기존 가지고 있던 돈 비교
				system("cls");
				player.player_money - player.player_betting;
				gotoxy(65, 19);
				textcolor(9, BLACK);
				cout << player.player_betting;
				textcolor(WHITE, BLACK);
				cout << "원을 배팅하셨습니다!";

				textcolor(WHITE, BLACK);
				gotoxy(65, 20);
				cout << "현재 남은 금액 : ";
				textcolor(9, BLACK);
				cout << player.player_money - player.player_betting;
				textcolor(WHITE, BLACK);
				cout << "원";

				gotoxy(65, 22);
				cout << "3초 뒤 게임이 시작됩니다.";

				for (int i = 3; i >= 1; i--) {
					textcolor(RED, BLACK);
					gotoxy(76, 25);
					cout << i;
					Sleep(1000);
				}
				gotoxy(76, 25);
				cout << "    ";

				textcolor(WHITE, BLACK);

				system("cls");
				break;
			}
			else {
				gotoxy(52, 29);
				cout << "돈이 부족합니다. 다시 배팅금액을 입력해주세요." << endl;
				Sleep(2000);
				system("cls");
			}
		}
	}
	
	
	system("cls");

	PlaySound(L"cardShuffle.wav", 0, SND_FILENAME | SND_ASYNC);
	gotoxy(65, 20);
	cout << "카드 나눠주는 중..";
	Sleep(4000);

	PlaySound(L"bgm.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
	
	gotoxy(65, 20);
	cout << "                     ";
	system("cls");

	textcolor(RED, BLACK);
	gotoxy(70, 1);
	cout << "BLACK JACK" << endl;
	textcolor(WHITE, BLACK);

	for (int i = 0; i < CARD_NUM; i++) {  //카드 배열에 0~51까지의 수 넣기
		Card_num[i] = i;
	}

	cout << endl << endl;

	gotoxy(70, 5);
	cout << "┌       ┐";
	gotoxy(71, 7);
	cout << "플레이어";
	gotoxy(70, 9);
	cout << "└       ┘";

	gotoxy(70, 41);
	cout << "┌       ┐";
	gotoxy(73, 43);
	cout << "딜러";
	gotoxy(70, 45);
	cout << "└       ┘";

	char pd_char = 'a';

	while (true) {
		Shuffle(Card_num, CARD_NUM);   //카드 섞기
		P_Card_Random(player, dealer, Card_num);   //플레이어 카드 배열에 랜덤으로 카드 넣어주기, 카드 모양&&숫자 정하기
		D_Card_Random(dealer, player, Card_num);	   //딜러 카드 배열에 랜덤으로 카드 넣어주기, 카드 모양&&숫자 정하기 
		PD_Card_Sum(dealer, player, pd_char);	    //플레이어, 딜러 카드 합해서 패배 결정
		Surrender(player);	//항복 확인

		while (true) {
			player.p_gamewin = 0;
			dealer.d_gamewin = 0;
			H_S_Select();		//플레이어 - Hit와 Stand중 고르기
			D_Card_Add();   //딜러 카드 뽑기
			PD_Card_Sum(dealer, player, pd_char); //딜러 카드 뽑은 후 패배 결정
		}
	}
}

void P_Card_Random(Player& player, Dealer& dealer, int Card_num[]) {	//플레이어 카드 랜덤 나눠주기 && 카드 모양, Hit 눌렀을 때 카드 추가
	textcolor(RED, BLACK);
	gotoxy(45, 10);
	cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓";

	gotoxy(45, 11);
	cout << "┃				                              ┃";

	gotoxy(45, 12);
	cout << "┃				                              ┃";

	gotoxy(45, 13);
	cout << "┃				                              ┃";

	gotoxy(45, 14);
	cout << "┃				                              ┃";

	gotoxy(45, 15);
	cout << "┃				                              ┃";

	gotoxy(45, 16);
	cout << "┃				                              ┃";

	gotoxy(45, 17);
	cout << "┃				                              ┃";

	gotoxy(45, 18);
	cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";

	textcolor(WHITE, BLACK);
	if (player.player_card[0] == 0 && player.player_card[1] == 0) {  //플레이어 카드 배열[0]방과 [1]방이 비어있다면 아래 for문 실행	
		for (int i = 0; i < 2; i++) {
			player.player_card[i] = Card_num[i];				//[0]=[0]	[1]=[1]

			Card_num[i] = -1;	//0번방과 1번방 -1으로 초기화

			switch ((int)(player.player_card[i] / 13)) {
			case 0:
				//다이아
				player.shape[i] = "◆";
				break;

			case 1:
				//하트
				player.shape[i] = "♥";
				break;

			case 2:
				//스페이스
				player.shape[i] = "♠";
				break;

			case 3:
				//클로버
				player.shape[i] = "♣";
				break;
			}


			if (player.player_card[i] > 12) {
				while (player.player_card[i] > 12) {
					player.player_card[i] = (player.player_card[i] - 13);
				}
				player.player_card[i] = player.player_card[i] + 2;	  //+2해서 카드 숫자 최종 확정
			}

			else {
				player.player_card[i] = player.player_card[i] + 2;	  //+2해서 카드 숫자 최종 확정
			}
		}
	}
	else {		   //플레이어 카드 배열[0]방과 [1]방이 비어있지 않고 차있다면 아래 for문 실행
		gotoxy(45, 24);
		cout << "                                                                                 ";

		gotoxy(70, 25);
		cout << "                        ";

		for (int i = 0; i < 20; i++) {
			if (player.player_card[i] == 0) {
				Sleep(1000);
				srand((unsigned)time(0));

				MiniGame_Card(player, dealer, 0, i); //뽑을 수 있는 카드 보여주기

				textcolor(WHITE, BLACK);
				system("cls");

				//int minigame_random = rand() % 4 + 1;
				int minigame_random = 2;

				for (int i = 4; i < 34; i += 8) {
					gotoxy(59, i);
					textcolor(RED, BLACK);
					cout << "。";
					textcolor(WHITE, BLACK);
					cout << "_______________________________";
					textcolor(RED, BLACK);
					cout << "。";
					textcolor(WHITE, BLACK);
					gotoxy(59, i + 1);
					cout << "|                               |";
					gotoxy(59, i + 2);
					cout << "|                               |";
					gotoxy(59, i + 3);
					cout << "|                               |";
					gotoxy(59, i + 4);
					cout << "|                               |";
					gotoxy(59, i + 5);
					textcolor(RED, BLACK);
					cout << "。";
					textcolor(WHITE, BLACK);
					cout << "-------------------------------";
					textcolor(RED, BLACK);
					cout << "。";
				}


				textcolor(WHITE, BLACK);
				gotoxy(67, 7);
				cout << "1번 : Up&Down 게임";

				gotoxy(61, 15);
				cout << "2번 : Rock-Paper-Scissors 게임";

				gotoxy(65, 23);
				cout << "3번 Lee Kwang Soo 게임";

				gotoxy(68, 31);
				cout << "4번 : Dice 게임";

				gotoxy(20, 35);
				cout << "총 4개의 MiniGame 중 하나의 게임이 ";
				textcolor(6, BLACK);
				cout << "랜덤";
				textcolor(WHITE, BLACK);
				cout << "으로 선택됩니다! 게임에서 이기신다면 카드를 선택할 수 있는 ";
				textcolor(6, BLACK);
				cout << "기회";
				textcolor(WHITE, BLACK);
				cout << "를 드립니다!";

				Sleep(4000);

				switch (minigame_random) {
				case 1:
					UpandDown(player, dealer);		//up&down 게임
					break;

				case 2:
					SRP(player, dealer);		//가위바위보 게임
					break;

				case 3:
					Kwangsoo_Game(player, dealer); //이광수 게임
					break;

				case 4:
					Dice_Game(player, dealer);		 //주사위 게임
					break;
				}

				system("cls");

				PlaySound(L"bgm.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
				MiniGame_Card(player, dealer, 1, i);	 //플레이어와 딜러 이긴 횟수로 카드 정하기

				system("cls");

				ShowCard(player, dealer);

				if (player.player_card[i] == 11) {
					if (player.Game_cnt == 0) {
						gotoxy(35, 24);
						cout << "A는 1 또는 11 중에 선택이 가능하며 자신에게 유리한 쪽으로 선택하면 됩니다";

						Sleep(3500);
						gotoxy(35, 24);
						cout << "                                                                                                              ";
					}

					gotoxy(45, 24);
					cout << "1과 11 중 어느 숫자를 선택하시겠습니까?(1이면 o, 11이면 e)";
					gotoxy(70, 25);
					textcolor(6, BLACK);
					cout << "1 or 11";
					textcolor(WHITE, BLACK);

					while (1) {
						ch = _getch();
						if (ch == 'o') {
							player.player_card[i] = 1;
							Sleep(1000);
							gotoxy(45, 24);
							cout << "                                                                                        ";
							gotoxy(70, 25);
							cout << "              ";
							break;
						}
						else if (ch == 'e') {
							player.player_card[i] = 65;
							Sleep(1000);
							gotoxy(45, 24);
							cout << "                                                                                        ";
							gotoxy(70, 25);
							cout << "              ";
							break;
						}
					}
				}

				break;
			}
		}
	}
}

void D_Card_Random(Dealer& dealer, Player& player, int Card_num[]) {   //딜러 카드 랜덤 나눠주기
	textcolor(RED, BLACK);
	gotoxy(45, 31);
	cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓";

	gotoxy(45, 32);
	cout << "┃				                              ┃";

	gotoxy(45, 33);
	cout << "┃				                              ┃";

	gotoxy(45, 34);
	cout << "┃				                              ┃";

	gotoxy(45, 35);
	cout << "┃				                              ┃";

	gotoxy(45, 36);
	cout << "┃				                              ┃";

	gotoxy(45, 37);
	cout << "┃				                              ┃";

	gotoxy(45, 38);
	cout << "┃				                              ┃";

	gotoxy(45, 39);
	cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";

	textcolor(WHITE, BLACK);
	if (dealer.dealer_card[0] == 0 && dealer.dealer_card[1] == 0) {  //플레이어 카드 배열[0]방과 [1]방이 비어있다면 아래 for문 실행
		for (int i = 0; i < 2; i++) {
			dealer.dealer_card[i] = Card_num[i + 2];				//[0]=[2]	[1]=[3]
			Card_num[i + 2] = -1;	//초기화

			switch ((int)(dealer.dealer_card[i] / 13)) {
			case 0:
				//다이아
				dealer.shape[i] = "◆";
				break;

			case 1:
				//하트
				dealer.shape[i] = "♥";
				break;

			case 2:
				//스페이스
				dealer.shape[i] = "♠";
				break;

			case 3:
				//클로버
				dealer.shape[i] = "♣";
				break;
			}

			while (dealer.dealer_card[i] > 12) {
				dealer.dealer_card[i] = (dealer.dealer_card[i] - 13);
			}
			dealer.dealer_card[i] = dealer.dealer_card[i] + 2;	  //+2해서 카드 숫자 최종 확정

			ShowCard(player, dealer);
		}
	}

	else {
		for (int i = 2; i < 20; i++) {
			if (dealer.dealer_card[i] == 0) {
				if (dealer.d_gamewin > player.p_gamewin || dealer.d_gamewin < player.p_gamewin) {
					for (int j = 4; j < CARD_NUM; j++) {
						if (Card_num[j] != -1) {
							dealer.dealer_card[i] = Card_num[j];
							switch ((int)(dealer.dealer_card[i] / 13)) {
							case 0:
								//다이아
								dealer.shape[i] = "◆";
								break;

							case 1:
								//하트
								dealer.shape[i] = "♥";
								break;

							case 2:
								//스페이스
								dealer.shape[i] = "♠";
								break;

							case 3:
								//클로버
								dealer.shape[i] = "♣";
								break;
							}

							if (dealer.dealer_card[i] > 12) {
								while (dealer.dealer_card[i] > 12) {
									dealer.dealer_card[i] = (dealer.dealer_card[i] - 13);
								}
								dealer.dealer_card[i] = dealer.dealer_card[i] + 2;	  //+2해서 카드 숫자 최종 확정
							}

							else {
								dealer.dealer_card[i] = dealer.dealer_card[i] + 2;	  //+2해서 카드 숫자 최종 확정
							}
							Card_num[j] = -1;
							break;
						}
					}
				}

				ShowCard(player, dealer);
				break;
			}
		}
	}
}

void MiniGame_Card(Player& player, Dealer& dealer, int m, int o) {		//Card_num방이 -1이 아니면 MiniGame_Card방에 넣어주고 그건 4번 반복, 그 4개의 카드 숫자/모양 정해주기, 
	textcolor(WHITE, BLACK);
	if (m == 0) {
		system("cls");

		gotoxy(42, 15);
		cout << "미니게임에서 승리하시면 ";
		textcolor(6, BLACK);
		cout << "4개";
		textcolor(WHITE, BLACK);
		cout << "의 카드 중 원하는 카드를 고를 수 있습니다!";
		gotoxy(53, 16);
		cout << "패배하시면 ";
		textcolor(6, BLACK);
		cout << "4개";
		textcolor(WHITE, BLACK);
		cout << "의 카드 중 ";
		textcolor(6, BLACK);
		cout << "랜덤";
		textcolor(WHITE, BLACK);
		cout << "으로 배정됩니다!";

		for (int i = 0; i < 4; i++) {	//미니게임 카드, 모양 초기화
			player.MiniGame_Card[i] = -1;
			player.MiniGame_shape[i] = " ";
		}

		srand((unsigned)time(0));
		int Card_num_random[4];
		int cnt = 0;

		for (int i = 0; i < 4; i++) {
			Card_num_random[i] = rand() % 48 + 4;
			for (int j = 0; j < i; j++) {
				if (((Card_num_random[i]) == (Card_num_random[j])) || (Card_num[Card_num_random[i]] == -1)) {
					i--;
					break;
				}
			}
		}

		for (int k = 0; k < 4; k++) {
			if (player.MiniGame_Card[k] == -1) {
				player.MiniGame_Card[k] = Card_num[Card_num_random[k]];
				player.MiniGame_Card2[k] = Card_num[Card_num_random[k]];
				cnt++;
			}
		}

		for (int j = 0; j < 4; j++) {
			switch ((int)(player.MiniGame_Card[j] / 13)) {
			case 0:
				//다이아
				player.MiniGame_shape[j] = "◆";
				break;

			case 1:
				//하트
				player.MiniGame_shape[j] = "♥";
				break;

			case 2:
				//스페이스
				player.MiniGame_shape[j] = "♠";
				break;

			case 3:
				//클로버
				player.MiniGame_shape[j] = "♣";
				break;
			}

			if (player.MiniGame_Card[j] > 12) {
				while (player.MiniGame_Card[j] > 12) {
					player.MiniGame_Card[j] = (player.MiniGame_Card[j] - 13);
				}
				player.MiniGame_Card[j] = player.MiniGame_Card[j] + 2;	  //+2해서 카드 숫자 최종 확정
			}

			else {
				player.MiniGame_Card[j] = player.MiniGame_Card[j] + 2;	  //+2해서 카드 숫자 최종 확정
			}
		}
	}

	if (m == 0 || m == 1) {
		textcolor(BLACK, BLACK);
		for (int j = 0; j < 4; j++) {
			gotoxy(55 + (j * 10), 21);
			textcolor(WHITE, WHITE);
			cout << "       ";

			for (int i = 21; i < 24; i++) {
				textcolor(WHITE, WHITE);
				gotoxy(55 + (j * 10), i + 1);
				cout << "       ";
			}

			gotoxy(57 + (j * 10), 23);
			if ((player.MiniGame_shape[j] == "◆") || (player.MiniGame_shape[j] == "♥")) {	  //red
				textcolor(RED, WHITE);
				cout << player.MiniGame_shape[j];
			}
			else {
				textcolor(BLACK, WHITE);
				cout << player.MiniGame_shape[j];
			}
			textcolor(BLACK, WHITE);
			if (player.MiniGame_Card[j] == 11) {
				cout << "A";
			}
			else if (player.MiniGame_Card[j] == 12) {
				cout << "K";
			}
			else if (player.MiniGame_Card[j] == 13) {
				cout << "Q";
			}
			else if (player.MiniGame_Card[j] == 14) {
				cout << "J";
			}
			else {
				cout << player.MiniGame_Card[j];
			}
			textcolor(WHITE, WHITE);
			gotoxy(55 + (j * 10), 25);
			cout << "       ";
		}
	}

	if (m == 1) {
		textcolor(BLACK, BLACK);
		gotoxy(45, 19);
		cout << "                                                                                                 ";
		gotoxy(50, 20);
		cout << "                                                                    ";
		textcolor(WHITE, BLACK);
		int p_card_select;
		if (player.p_gamewin > dealer.d_gamewin) {	//플레이어가 딜러를 이겼을 때
			gotoxy(45, 16);
			cout << "미니 게임에서 이기셨으므로 카드 우선 선택권을 얻으셨습니다!";
			gotoxy(50, 17);
			textcolor(6, BLACK);
			cout << "1, 2, 3, 4";
			textcolor(WHITE, BLACK);
			cout << "의 수 중 하나를 선택해 카드를 고르세요";
			gotoxy(60, 18);
			cout << "플레이어의 카드 합 : " << player.player_sum;
			gotoxy(62, 19);
			cout << "카드 선택 :   ";
			textcolor(6, BLACK);
			cin >> p_card_select;

			switch (p_card_select) {
			case 1:
				player.player_card[o] = player.MiniGame_Card[0];
				player.shape[o] = player.MiniGame_shape[0];
				player.p_gns = player.MiniGame_Card2[0];
				for (int i = 4; i < CARD_NUM; i++) {
					if (player.p_gns == Card_num[i]) {
						for (int j = 0; j < 4; j++) {
							player.MiniGame_Card[j] = -1;
						}
						
						Card_num[i] = -1;

						Sleep(2000);
						break;
					}
				}
				break;

			case 2:
				player.player_card[o] = player.MiniGame_Card[1];
				player.shape[o] = player.MiniGame_shape[1];
				player.p_gns = player.MiniGame_Card2[1];
				for (int i = 4; i < CARD_NUM; i++) {
					if (player.p_gns == Card_num[i]) {
						for (int j = 0; j < 4; j++) {
							player.MiniGame_Card[j] = -1;
						}
						Card_num[i] = -1;

						Sleep(2000);
						break;
					}
				}
				break;

			case 3:
				player.player_card[o] = player.MiniGame_Card[2];
				player.shape[o] = player.MiniGame_shape[2];
				player.p_gns = player.MiniGame_Card2[2];
				for (int i = 4; i < CARD_NUM; i++) {
					if (player.p_gns == Card_num[i]) {
						for (int j = 0; j < 4; j++) {
							player.MiniGame_Card[j] = -1;
						}
						Card_num[i] = -1;

						Sleep(2000);
						break;
					}
				}
				break;

			case 4:
				player.player_card[o] = player.MiniGame_Card[3];
				player.shape[o] = player.MiniGame_shape[3];
				player.p_gns = player.MiniGame_Card2[3];
				for (int i = 4; i < CARD_NUM; i++) {
					if (player.p_gns == Card_num[i]) {
						for (int j = 0; j < 4; j++) {
							player.MiniGame_Card[j] = -1;
						}
						Card_num[i] = -1;

						Sleep(2000);
						break;
					}
				}
				break;

			}
		}

		else if (player.p_gamewin < dealer.d_gamewin) {		 //딜러가 플레이어를 이겼을 때
			srand((unsigned)time(0));

			player.p_gnr = rand() % 4;	 //0~3

			gotoxy(45, 17);
			cout << "게임에서 패하셨으므로 4개의 카드 중 ";
			textcolor(6, BLACK);
			cout << "랜덤";
			textcolor(WHITE, BLACK);
			cout << "으로 배정됩니다!";

			Sleep(3000);
			gotoxy(45, 19);
			cout << "                                                                               ";

			PlaySound(L"cardShuffle.wav", 0, SND_FILENAME | SND_ASYNC);
			gotoxy(65, 19);
			cout << "카드 배정하는 중..";
			Sleep(4000);

			player.player_card[o] = player.MiniGame_Card[player.p_gnr];
			player.shape[o] = player.MiniGame_shape[player.p_gnr];
			player.p_gns = player.MiniGame_Card2[player.p_gnr];
			for (int i = 4; i < CARD_NUM; i++) {
				if (player.p_gns == Card_num[i]) {
					for (int j = 0; j < 4; j++) {
						player.MiniGame_Card[j] = -1;
					}
					Card_num[i] = -1;
					break;
				}
			}
		}
	}
	
	if (m == 0) {
		Sleep(5000);
	}
}

void UpandDown(Player& player, Dealer& dealer) {
	system("cls");
	srand((unsigned)time(0));

	textcolor(11, BLACK);
	cout <<
		"\t\t\t\t  " << " __     __                    ____         _____" << endl <<
		"\t\t\t\t  " << "|  |   |  |                  /  _ ＼      (____ ＼" << endl;
	textcolor(9, BLACK);
	cout <<
		"\t\t\t\t  " << "|  |   |  |     ____        (  (_)  )      __  ＼ ＼    ___     __ __ __      ____" << endl <<
		"\t\t\t\t  " << "|  |   |  |    |  _ ＼       )  _  (      |  |   | |  /  _ ＼   |  |  |  |   |  _ ＼" << endl;
	textcolor(1, BLACK);
	cout <<
		"\t\t\t\t  " << "|  |___|  |    | | |  |     (  (/  ＼     |  |__/ /  |  |_|  |  |  |  |  |   | | | |" << endl <<
		"\t\t\t\t  " << "＼_______ |    | | |_/       ＼__/＼_)    |______/    ＼___ /   ＼_______|   |_| |_|" << endl <<
		"\t\t\t\t  " << "               |_|";

	textcolor(WHITE, BLACK);
	gotoxy(68, 12);
	cout << "숫자 범위 : 1~10" << endl;
	gotoxy(70, 14);
	cout << "총 3번의 기회" << endl;

	int com_num, p_num;
	int cnt = 3;
	com_num = rand() % 10 + 1;

	while (1) {
		gotoxy(60, 16);
		cout << "1~10 사이의 수를 입력해 주세요 : ";
		cin >> p_num;

		if (p_num >= 1 && p_num <= 10) {
			if (p_num > com_num) {
				textcolor(11, BLACK);
				gotoxy(75, 18);
				cout << "Down!";
				cnt--;
				textcolor(WHITE, BLACK);
				gotoxy(64, 20);
				cout << cnt << "번의 기회가 남았습니다!";

				Sleep(2500);
				gotoxy(75, 18);
				cout << "          ";
				gotoxy(64, 20);
				cout << "      " << "                                  ";
			}
			else if (p_num < com_num) {
				textcolor(11, BLACK);
				gotoxy(75, 18);
				cout << "Up!";
				cnt--;
				textcolor(WHITE, BLACK);
				gotoxy(64, 20);
				cout << cnt << "번의 기회가 남았습니다!";

				Sleep(2500);
				gotoxy(75, 18);
				cout << "          ";
				gotoxy(64, 20);
				cout << "      " << "                                  ";
			}
			else if (p_num == com_num) {
				gotoxy(60, 20);
				if (cnt == 3) {
					cout << "정답입니다! " << cnt - 2 << "번만에 맞추셨습니다!!";
				}
				else if (cnt == 2) {
					cout << "정답입니다! " << cnt << "번만에 맞추셨습니다!!";
				}
				else {
					cout << "정답입니다! " << cnt + 2 << "번만에 맞추셨습니다!!";
				}

				++player.p_gamewin;

				Sleep(2500);
				gotoxy(60, 20);
				cout << "                    " << "      " << "                                    ";
				break;
			}
			if (cnt == 0) {
				PlaySound(L"wrong.wav", 0, SND_FILENAME | SND_ASYNC);
				gotoxy(63, 20);
				cout << "3번의 기회를 모두 쓰셨습니다!";
				gotoxy(70, 21);
				cout << "답은 : " << com_num << "입니다!";
				++dealer.d_gamewin;

				Sleep(2500);
				break;
			}
		}
		else {
			gotoxy(62, 20);
			cout << "범위 내의 숫자를 입력해주세요!";
			Sleep(2000);
			gotoxy(62, 20);
			cout << "                                            ";
		}
	}
}

void SRP(Player& player, Dealer& delaer) {				   //가위바위보 게임
	system("cls");
	srand((unsigned)time(0));

	string rock[][8] = { " ━━━━━━━━━━━━━━━", "┃   ┃   ┃   ┃   ┃",  "┃   ┃   ┃   ┃   ┃", "┃     __________", "┃   ┃           ┃", "┃     ━━━━━━━━━ ┃", "┃               ┃", "┃_______________┃" };
	string sissor[][11] = { "         ━━     ━━", "       ┃    ┃ ┃    ┃", "       ┃    ┃ ┃    ┃", "       ┃    ┃ ┃    ┃", "  ━━━━       ━     ┃","┃           _______┃",  "┃    ┃   ┃ ┃       ┃", "┃    ┃   ┃  -------┃", " ---- ----         ┃", "┃                  ┃", "┃__________________┃" };
	string paper[][12] = { "              ━━", "        ━━  ┃    ┃  ━━", "      ┃    ┃┃    ┃┃    ┃", "  ━━  ┃    ┃┃    ┃┃    ┃", "┃    ┃┃    ┃┃    ┃┃    ┃", "┃     -     -     -      ━━", "┃                      ┃    ┃", "┃                      ┃    ┃", "＼                          /", "  ＼                       /", "    ＼                    /","      ＼_________________/" };

	int rock_length = sizeof(rock) / sizeof(rock[0]);
	int sissor_length = sizeof(sissor) / sizeof(sissor[0]);
	int paper_length = sizeof(paper) / sizeof(paper[0]);

	textcolor(6, BLACK);
	cout <<
		"\t\t\t\t\t" << "_____                       ____                       _____" << endl <<
		"\t\t\t\t\t" << "|  _  ＼                   / ___|                      |  _ ＼" << endl;
	textcolor(13, BLACK);
	cout <<
		"\t\t\t\t\t" << "| |_)  |        _____      ＼___＼          _____      | |_)  |" << endl <<
		"\t\t\t\t\t" << "|  _  <        |_____|       ___) |        |_____|     |   __／ " << endl;
	textcolor(4, BLACK);
	cout <<
		"\t\t\t\t\t" << "|_| ＼_＼ock                |____/ issors              |_ |    aper " << endl;

	textcolor(WHITE, BLACK);
	int d_random, p_random;

	gotoxy(60, 12);
	cout << "가위바위보 게임입니다!";
	gotoxy(54, 13);
	cout << "1 ~ 3중 하나의 숫자를 선택해주세요!";
	gotoxy(56, 14);
	textcolor(14, BLACK);
	cout << "1";
	textcolor(WHITE, BLACK);
	cout << "은 ";
	textcolor(14, BLACK);
	cout << "바위";
	textcolor(6, BLACK);
	cout << " 2";
	textcolor(WHITE, BLACK);
	cout << "는 ";
	textcolor(6, BLACK);
	cout << "가위 ";
	textcolor(12, BLACK);
	cout << "3";
	textcolor(WHITE, BLACK);
	cout << "은 ";
	textcolor(12, BLACK);
	cout << "보";
	textcolor(WHITE, BLACK);
	cout << "입니다!";

	while (1) {
		d_random = rand() % 3 + 1;

		gotoxy(62, 16);
		cout << "플레이어의 선택 : ";
		cin >> p_random;

		if (p_random >= 1 && p_random <= 3) { //p_random이 1~3안의 숫자일 때
			if (p_random == d_random) {				 //비겼을 때
				gotoxy(65, 20);
				cout << "무승부입니다!";
				if ((p_random == 1) && (d_random == 1)) {  //바위
					for (int i = 0; i < rock_length; i++) {
						for (int j = 0; j < 8; j++) {
							gotoxy(40, 30 + (j * 1));
							cout << rock[i][j] << endl;
						}
					}

					for (int i = 0; i < rock_length; i++) {
						for (int j = 0; j < 8; j++) {
							gotoxy(90, 30 + (j * 1));
							cout << rock[i][j] << endl;
						}
					}

					textcolor(RED, BLACK);
					gotoxy(45, 42);
					cout << "<딜러>";

					gotoxy(93, 42);
					cout << "<플레이어>";

					textcolor(WHITE, BLACK);

					Sleep(2000);
					for (int i = 0; i < rock_length; i++) {
						for (int j = 0; j < 8; j++) {
							gotoxy(40, 30 + (j * 1));
							cout << "                                                                " << endl;
						}
					}

					for (int i = 0; i < rock_length; i++) {
						for (int j = 0; j < 8; j++) {
							gotoxy(90, 30 + (j * 1));
							cout << "                                                                " << endl;
						}
					}

					gotoxy(45, 42);
					cout << "            ";

					gotoxy(93, 42);
					cout << "                 ";
				}

				else if ((p_random == 2) && (d_random == 2)) {	  //가위
					for (int i = 0; i < sissor_length; i++) {
						for (int j = 0; j < 11; j++) {
							gotoxy(30, 30 + (j * 1));
							cout << sissor[i][j] << endl;
						}
					}

					for (int i = 0; i < sissor_length; i++) {
						for (int j = 0; j < 11; j++) {
							gotoxy(90, 30 + (j * 1));
							cout << sissor[i][j] << endl;
						}
					}

					textcolor(RED, BLACK);
					gotoxy(38, 45);
					cout << "<딜러>";

					gotoxy(95, 45);
					cout << "<플레이어>";

					textcolor(WHITE, BLACK);

					//그림 글 지우기
					Sleep(2000);
					for (int i = 0; i < sissor_length; i++) {
						for (int j = 0; j < 11; j++) {
							gotoxy(30, 30 + (j * 1));
							cout << "                                                      " << endl;
						}
					}

					for (int i = 0; i < sissor_length; i++) {
						for (int j = 0; j < 11; j++) {
							gotoxy(90, 30 + (j * 1));
							cout << "                                                      " << endl;
						}
					}

					gotoxy(38, 45);
					cout << "               ";

					gotoxy(95, 45);
					cout << "                 ";
				}

				else {	   //보
					for (int i = 0; i < paper_length; i++) {
						for (int j = 0; j < 12; j++) {
							gotoxy(30, 30 + (j * 1));
							cout << paper[i][j] << endl;
						}
					}

					for (int i = 0; i < paper_length; i++) {
						for (int j = 0; j < 12; j++) {
							gotoxy(90, 30 + (j * 1));
							cout << paper[i][j] << endl;
						}
					}

					textcolor(RED, BLACK);
					gotoxy(42, 45);
					cout << "<딜러>";

					gotoxy(100, 45);
					cout << "<플레이어>";

					textcolor(WHITE, BLACK);

					Sleep(2000);
					for (int i = 0; i < paper_length; i++) {
						for (int j = 0; j < 12; j++) {
							gotoxy(30, 30 + (j * 1));
							cout << "                                                                              " << endl;
						}
					}

					for (int i = 0; i < paper_length; i++) {
						for (int j = 0; j < 12; j++) {
							gotoxy(90, 30 + (j * 1));
							cout << "                                                                              " << endl;
						}
					}

					gotoxy(42, 45);
					cout << "            ";

					gotoxy(100, 45);
					cout << "                  ";
				}

				Sleep(2000);
				gotoxy(65, 20);
				cout << "                    ";
				continue;
			}

			else if (((d_random == 1) && (p_random == 2)) || ((d_random == 2) && (p_random == 3)) || ((d_random == 3) && (p_random == 1))) {	 //졌을 때의 경우의 수
				PlaySound(L"wrong.wav", 0, SND_FILENAME | SND_ASYNC);
				if ((d_random == 1) && (p_random == 2)) {	   //딜러 = 바위, 플레이어= 가위
					for (int i = 0; i < rock_length; i++) {
						for (int j = 0; j < 8; j++) {
							gotoxy(30, 32 + (j * 1));
							cout << rock[i][j] << endl;
						}
					}

					for (int i = 0; i < sissor_length; i++) {
						for (int j = 0; j < 11; j++) {
							gotoxy(90, 30 + (j * 1));
							cout << sissor[i][j] << endl;
						}
					}

					textcolor(RED, BLACK);
					gotoxy(36, 45);
					cout << "<딜러>";

					gotoxy(95, 45);
					cout << "<플레이어>";

					textcolor(WHITE, BLACK);

					gotoxy(45, 25);
					cout << "<딜러 : ";
					textcolor(14, BLACK);
					cout << "바위";
					textcolor(WHITE, BLACK);
					cout << ", 플레이어 : ";
					textcolor(6, BLACK);
					cout << "가위";
					textcolor(WHITE, BLACK);
					cout << " > 로 플레이어가 패했습니다..";

					Sleep(2000);
				}
				else if ((d_random == 2) && (p_random == 3)) {	 //딜러 = 가위, 플레이어 = 보
					for (int i = 0; i < sissor_length; i++) {
						for (int j = 0; j < 11; j++) {
							gotoxy(30, 32 + (j * 1));
							cout << sissor[i][j] << endl;
						}
					}

					for (int i = 0; i < paper_length; i++) {
						for (int j = 0; j < 12; j++) {
							gotoxy(90, 30 + (j * 1));
							cout << paper[i][j] << endl;
						}
					}

					textcolor(RED, BLACK);
					gotoxy(38, 45);
					cout << "<딜러>";

					gotoxy(101, 45);
					cout << "<플레이어>";

					textcolor(WHITE, BLACK);

					gotoxy(45, 25);
					cout << "<딜러 : ";
					textcolor(6, BLACK);
					cout << "가위";
					textcolor(WHITE, BLACK);
					cout << ", 플레이어 : ";
					textcolor(12, BLACK);
					cout << "보 ";
					textcolor(WHITE, BLACK);
					cout << "> 로 플레이어가 패했습니다..";

					Sleep(2000);
				}
				else {	 //딜러 = 보, 플레이어 = 바위
					for (int i = 0; i < paper_length; i++) {
						for (int j = 0; j < 12; j++) {
							gotoxy(30, 30 + (j * 1));
							cout << paper[i][j] << endl;
						}
					}

					for (int i = 0; i < rock_length; i++) {
						for (int j = 0; j < 8; j++) {
							gotoxy(90, 32 + (j * 1));
							cout << rock[i][j] << endl;
						}
					}

					textcolor(RED, BLACK);
					gotoxy(42, 45);
					cout << "<딜러>";

					gotoxy(95, 45);
					cout << "<플레이어>";

					textcolor(WHITE, BLACK);

					gotoxy(45, 25);
					cout << "<딜러 : ";
					textcolor(12, BLACK);
					cout << "보";
					textcolor(WHITE, BLACK);
					cout << ", 플레이어 : ";
					textcolor(14, BLACK);
					cout << "바위 ";
					textcolor(WHITE, BLACK);
					cout << "> 로 플레이어가 패했습니다..";

					Sleep(2000);
				}

				++dealer.d_gamewin;
				break;
			}

			else {			 //플레이어가 이겼을 때
				if ((p_random == 1) && (d_random == 2)) {	 //플레이어 : 바위, 딜러 : 가위
					for (int i = 0; i < sissor_length; i++) {  //딜러 : 가위
						for (int j = 0; j < 11; j++) {
							gotoxy(30, 30 + (j * 1));
							cout << sissor[i][j] << endl;
						}
					}

					for (int i = 0; i < rock_length; i++) {		  //플레이어 : 바위
						for (int j = 0; j < 8; j++) {
							gotoxy(90, 32 + (j * 1));
							cout << rock[i][j] << endl;
						}
					}

					textcolor(RED, BLACK);
					gotoxy(38, 45);
					cout << "<딜러>";

					gotoxy(95, 45);
					cout << "<플레이어>";

					textcolor(WHITE, BLACK);

					gotoxy(45, 25);
					cout << "<플레이어 : ";
					textcolor(14, BLACK);
					cout << "바위";
					textcolor(WHITE, BLACK);
					cout << ", 딜러 : ";
					textcolor(6, BLACK);
					cout << "가위 ";
					textcolor(WHITE, BLACK);
					cout << "> 로 플레이어가 승리하였습니다!!";

					Sleep(2000);
				}
				else if ((p_random == 2) && (d_random == 3)) { //플레이어 : 가위, 딜러 : 보
					for (int i = 0; i < paper_length; i++) {	  //딜러 : 보
						for (int j = 0; j < 12; j++) {
							gotoxy(30, 30 + (j * 1));
							cout << paper[i][j] << endl;
						}
					}

					for (int i = 0; i < sissor_length; i++) {		 //플레이어 : 가위
						for (int j = 0; j < 11; j++) {
							gotoxy(90, 30 + (j * 1));
							cout << sissor[i][j] << endl;
						}
					}

					textcolor(RED, BLACK);
					gotoxy(42, 45);
					cout << "<딜러>";

					gotoxy(95, 45);
					cout << "<플레이어>";

					textcolor(WHITE, BLACK);

					gotoxy(45, 25);
					cout << "<플레이어 : ";
					textcolor(6, BLACK);
					cout << "가위";
					textcolor(WHITE, BLACK);
					cout << ", 딜러 : ";
					textcolor(12, BLACK);
					cout << "보 ";
					textcolor(WHITE, BLACK);
					cout << "> 로 플레이어가 승리하였습니다!!";

					Sleep(2000);
				}
				else {				  //플레이어 : 보, 딜러 : 바위
					for (int i = 0; i < rock_length; i++) {	   //딜러 : 바위
						for (int j = 0; j < 8; j++) {
							gotoxy(30, 32 + (j * 1));
							cout << rock[i][j] << endl;
						}
					}


					for (int i = 0; i < paper_length; i++) {  //플레이어 : 보
						for (int j = 0; j < 12; j++) {
							gotoxy(90, 30 + (j * 1));
							cout << paper[i][j] << endl;
						}
					}

					textcolor(RED, BLACK);
					gotoxy(35, 45);
					cout << "<딜러>";

					gotoxy(101, 45);
					cout << "<플레이어>";

					textcolor(WHITE, BLACK);

					gotoxy(47, 25);
					cout << "<플레이어 : ";
					textcolor(12, BLACK);
					cout << "보";
					textcolor(WHITE, BLACK);
					cout << ", 딜러 : ";
					textcolor(14, BLACK);
					cout << "바위 ";
					textcolor(WHITE, BLACK);
					cout << "> 로 플레이어가 승리하였습니다!!";

					Sleep(2000);
				}

				++player.p_gamewin;
				break;
			}
		}

		else {
			gotoxy(45, 20);
			cout << "1~3에 벗어나는 수를 입력하셨습니다! 다시 입력해주세요!";
			Sleep(2000);
			gotoxy(45, 20);
			cout << "                                                                               ";
		}
	}

	Sleep(1500);
}

void Kwangsoo_Game(Player& player, Dealer& dealer) {	//이광수 게임
	system("cls");
	srand((unsigned)time(0));

	textcolor(7, BLACK);
	cout <<
		"\t\t\t" << " _                            _   __                                            ____" << endl <<
		"\t\t\t" << "| |        ___      ___     |  |/  / __        __   __ _    _ __     __ _     / ___|      ___      ___" << endl;
	textcolor(14, BLACK);
	cout <<
		"\t\t\t" << "| |       / _ ＼   / _ ＼   |  '  / ＼ ＼ /＼ / /  / _` |  | '_ ＼  / _`  |   ＼___ ＼   /  _＼   / _ ＼" << endl <<
		"\t\t\t" << "| |___   |   __/  |   __/   |  .  ＼  ＼ V   V /  | (_| |  | | | | | (_|  |     ___) |  |  (_) | | (_)  |" << endl;
	textcolor(6, BLACK);
	cout <<
		"\t\t\t" << "|_____|   ＼___|   ＼___|   |_ |＼_＼  ＼_/＼_/   ＼__,_|  |_| |_| ＼__,  |    |____/    ＼___/   ＼___/" << endl <<
		"\t\t\t" << "                                                                    |____/";

	textcolor(WHITE, BLACK);
	gotoxy(65, 11);
	cout << "이광수 게임입니다!!";
	gotoxy(50, 12);
	cout << "1. 랜덤으로 나오는 두 글자의 단어 중 한글자를 선택!";
	gotoxy(45, 13);
	cout << "2. 딜러와 같은 글자를 말하면 실패!!    딜러와 다른 글자를 말하면 성공!!";
	gotoxy(35, 14);
	cout << "예) 제시어 : 악어 || 딜러: 악, 플레이어: 악 => 실패! || 딜러: 어, 플레이어: 악 => 성공!!";

	ifstream kwangsoo_game;
	string word[66];

	kwangsoo_game.open("kwangsoo_game.txt");

	while (!kwangsoo_game.eof()) {
		for (int i = 0; i < 66; i++) {
			kwangsoo_game >> word[i];
		}
	}

	int kwangsoo_random = rand() % 66;

	gotoxy(53, 16);
	cout << "첫번째 글자 선택 : ";
	textcolor(6, BLACK);
	cout << "1";
	textcolor(WHITE, BLACK);
	cout << "   두번째 글자 선택 : ";
	textcolor(6, BLACK);
	cout << "2";
	gotoxy(60, 17);
	cout << "1";
	textcolor(WHITE, BLACK);
	cout << " 과 ";
	textcolor(6, BLACK);
	cout << "2";
	textcolor(WHITE, BLACK);
	cout << "중 하나를 선택해 주세요!";

	int d_random = rand() % 2 + 1, p_select;
	string d_word, p_word;

	gotoxy(66, 22);
	cout << "플레이어 선택 : ";

	if (kwangsoo_random % 2 == 0) {
		gotoxy(67, 20);
		cout << "제시어 : ";
		textcolor(6, BLACK);
		cout << word[kwangsoo_random] << word[kwangsoo_random + 1];

		gotoxy(82, 22);
		cin >> p_select;

		if (d_random == 1) {
			d_word = word[kwangsoo_random];
		}
		else if (d_random == 2) {
			d_word = word[kwangsoo_random + 1];
		}

		if (p_select == 1) {
			p_word = word[kwangsoo_random];
		}
		else if (p_select == 2) {
			p_word = word[kwangsoo_random + 1];
		}
	}
	else {
		gotoxy(67, 20);
		cout << "제시어 : ";
		textcolor(6, BLACK);
		cout << word[kwangsoo_random - 1] << word[kwangsoo_random];

		gotoxy(82, 22);
		cin >> p_select;

		if (d_random == 1) {
			d_word = word[kwangsoo_random - 1];
		}
		else if (d_random == 2) {
			d_word = word[kwangsoo_random];
		}

		if (p_select == 1) {
			p_word = word[kwangsoo_random - 1];
		}
		else if (p_select == 2) {
			p_word = word[kwangsoo_random];
		}
	}

	Sleep(1000);

	gotoxy(50, 27);
	textcolor(WHITE, BLACK);
	cout << "딜러 : ";
	textcolor(6, BLACK);
	cout << d_word;

	gotoxy(88, 27);
	textcolor(WHITE, BLACK);
	cout << "플레이어 : ";
	textcolor(6, BLACK);
	cout << p_word;

	textcolor(WHITE, BLACK);

	if (d_word == p_word) {
		PlaySound(L"wrong.wav", 0, SND_FILENAME | SND_ASYNC);
		gotoxy(49, 25);
		cout << "딜러와 같은 글자를 말하셨으므로 플레이어의 ";
		textcolor(6, BLACK);
		cout << "패";
		textcolor(WHITE, BLACK);
		cout << "입니다..";
		++dealer.d_gamewin;
	}
	else {
		gotoxy(49, 25);
		cout << "딜러와 다른 글자를 말하셨으므로 플레이어의 ";
		textcolor(6, BLACK);
		cout << "승";
		textcolor(WHITE, BLACK);
		cout << "입니다!!";
		++player.p_gamewin;
	}

	kwangsoo_game.close();
	Sleep(3500);
}

void Dice_Game(Player& player, Dealer& dealer) {		//주사위 게임
	system("cls");
	srand((unsigned)time(0));

	textcolor(WHITE, BLACK);
	cout <<
		"\t\t\t\t\t\t      " << " _______      __       ______   _______" << endl <<
		"\t\t\t\t\t\t      " << "|       ＼   |   |    /      |  |  ____|" << endl;
	textcolor(10, BLACK);
	cout <<
		"\t\t\t\t\t\t      " << "|  .--.   |  |   |   |  ,----'  |  |__" << endl <<
		"\t\t\t\t\t\t      " << "|  |  |   |  |   |   |  |       |   __|" << endl;
	textcolor(2, BLACK);
	cout <<
		"\t\t\t\t\t\t      " << "|  '--'   |  |   |   |  `----.  |  |____" << endl <<
		"\t\t\t\t\t\t      " << "|_______/    |__ |   ＼______|  |_______|";

	textcolor(WHITE, BLACK);
	gotoxy(63, 8);
	cout << "주사위 게임입니다!!";
	gotoxy(45, 12);
	textcolor(12, BLACK);
	cout << "빨간";
	textcolor(WHITE, BLACK);
	cout << "주사위와 ";
	textcolor(9, BLACK);
	cout << "파란";
	textcolor(WHITE, BLACK);
	cout << "주사위의 합이 9보다 크면 ";
	textcolor(6, BLACK);
	cout << "승! ";
	textcolor(WHITE, BLACK);
	cout << "아니면 ";
	textcolor(6, BLACK);
	cout << "패!";
	gotoxy(30, 13);
	textcolor(12, BLACK);
	cout << "빨간";
	textcolor(WHITE, BLACK);
	cout << "주사위는 이미 랜덤으로 정해져있습니다! ";
	textcolor(9, BLACK);
	cout << "파란";
	textcolor(WHITE, BLACK);
	cout << "주사위의 숫자를 1 ~ 6 사이의 수 중에 선택해 주세요!!";

	int red_random = rand() % 6 + 1;
	int blue_random, dice_sum;

	while (1) {
		gotoxy(65, 16);
		textcolor(9, BLACK);
		cout << "파란";
		textcolor(WHITE, BLACK);
		cout << "주사위 : ";
		textcolor(9, BLACK);
		cin >> blue_random;

		if (blue_random >= 1 && blue_random <= 6) {
			dice_sum = red_random + blue_random;
			if (dice_sum >= 9) {
				textcolor(WHITE, BLACK);
				gotoxy(50, 18);
				cout << "두 주사위의 합이 9보다 큽니다!! 플레이어의 ";
				textcolor(6, BLACK);
				cout << "승!!";
				textcolor(WHITE, BLACK);
				gotoxy(65, 19);
				cout << "두 주사위의 합 : ";
				textcolor(6, BLACK);
				cout << dice_sum;

				++player.p_gamewin;
				break;
			}
			else {
				PlaySound(L"wrong.wav", 0, SND_FILENAME | SND_ASYNC);
				textcolor(WHITE, BLACK);
				gotoxy(50, 18);
				cout << "두 주사위의 합이 9보다 작습니다.. 플레이어의 ";
				textcolor(6, BLACK);
				cout << "패..";
				textcolor(WHITE, BLACK);
				gotoxy(65, 19);
				cout << "두 주사위의 합 : ";
				textcolor(6, BLACK);
				cout << dice_sum;

				++dealer.d_gamewin;
				break;
			}
		}
		else {
			textcolor(WHITE, BLACK);
			gotoxy(45, 18);
			cout << "범위를 벗어난 숫자를 입력하셨습니다! 다시 입력해주세요!";
			Sleep(2000);
			gotoxy(45, 18);
			cout << "                                                                                 ";
		}
	}

	textcolor(WHITE, BLACK);
	Sleep(3000);
}

void Show_DCard(Dealer& dealer) {	 //딜러 카드 보여주기
	for (int i = 0; i < 10; i++) {
		if (dealer.dealer_card[i] != 0) {
			gotoxy(50 + (i * 10), 33); 
			textcolor(WHITE, WHITE);
			cout << "       ";

			for (int j = 33; j < 36; j++) {
				textcolor(WHITE, WHITE);
				gotoxy(50 + (i * 10), j + 1);
				cout << "       ";
			}
			gotoxy(52 + (i * 10), 35);
			if ((dealer.shape[i] == "◆") || (dealer.shape[i] == "♥")) {	  //red
				textcolor(RED, WHITE);
				cout << dealer.shape[i];
			}
			else {
				textcolor(BLACK, WHITE);
				cout << dealer.shape[i];
			}
			textcolor(BLACK, WHITE);
			if (dealer.dealer_card[i] == 11) {
				cout << "A";
			}
			else if (dealer.dealer_card[i] == 12) {
				cout << "K";
			}
			else if (dealer.dealer_card[i] == 13) {
				cout << "Q";
			}
			else if (dealer.dealer_card[i] == 14) {
				cout << "J";
			}
			else {
				cout << dealer.dealer_card[i];
			}
			textcolor(WHITE, WHITE);
			gotoxy(50 + (i * 10), 37);
			cout << "       ";
		}
	}
	textcolor(WHITE, BLACK);
}

void ShowCard(Player& player, Dealer& dealer) {
	textcolor(WHITE, BLACK);
	gotoxy(30, 7);
	cout << "플레이어 승리 횟수 : " << player.p_win;
	gotoxy(30, 43);
	cout << "딜러 승리 횟수 : " << dealer.d_win;

	gotoxy(70, 5);
	cout << "┌       ┐";
	gotoxy(71, 7);
	cout << "플레이어";
	gotoxy(70, 9);
	cout << "└       ┘";

	gotoxy(70, 41);
	cout << "┌       ┐";
	gotoxy(73, 43);
	cout << "딜러";
	gotoxy(70, 45);
	cout << "└       ┘";

	textcolor(RED, BLACK);

	//플레이어 칸
	gotoxy(45, 10);
	cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓";

	gotoxy(45, 11);
	cout << "┃				                              ┃";

	gotoxy(45, 12);
	cout << "┃				                              ┃";

	gotoxy(45, 13);
	cout << "┃				                              ┃";

	gotoxy(45, 14);
	cout << "┃				                              ┃";

	gotoxy(45, 15);
	cout << "┃				                              ┃";

	gotoxy(45, 16);
	cout << "┃				                              ┃";

	gotoxy(45, 17);
	cout << "┃				                              ┃";

	gotoxy(45, 18);
	cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";

	//딜러 칸
	gotoxy(45, 31);
	cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓";

	gotoxy(45, 32);
	cout << "┃				                              ┃";

	gotoxy(45, 33);
	cout << "┃				                              ┃";

	gotoxy(45, 34);
	cout << "┃				                              ┃";

	gotoxy(45, 35);
	cout << "┃				                              ┃";

	gotoxy(45, 36);
	cout << "┃				                              ┃";

	gotoxy(45, 37);
	cout << "┃				                              ┃";

	gotoxy(45, 38);
	cout << "┃				                              ┃";

	gotoxy(45, 39);
	cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";

	textcolor(WHITE, BLACK);

	for (int i = 0; i < 10; i++) {
		if (player.player_card[i] != 0) {
			textcolor(WHITE, BLACK);

			gotoxy(50 + (i * 10), 12);
			textcolor(WHITE, WHITE);
			cout << "       ";
			
			for (int j = 12; j < 15; j++) {
				textcolor(WHITE, WHITE);
				gotoxy(50 + (i * 10), j+1);
				cout << "       ";
			}
			gotoxy(52 + (i * 10), 14);
			if ((player.shape[i] == "◆") || (player.shape[i] == "♥")) {	  //red
				textcolor(RED, WHITE);
				cout << player.shape[i];
			}
			else {
				textcolor(BLACK, WHITE);
				cout << player.shape[i];
			}
			textcolor(BLACK, WHITE);
			if (player.player_card[i] == 11 || player.player_card[i] == 65 || player.player_card[i] == 1) {
				cout << "A";
			}
			else if (player.player_card[i] == 12) {
				cout << "K";
			}
			else if (player.player_card[i] == 13) {
				cout << "Q";
			}
			else if (player.player_card[i] == 14) {
				cout << "J";
			}
			else {
				cout << player.player_card[i];
			}
			textcolor(WHITE, WHITE);
			gotoxy(50 + (i * 10), 16);
			cout << "       ";
		}
	}

	textcolor(WHITE, BLACK);
}

void H_S_Select() {		   //Hit = 카드 추가 O,  Stand = 카드 추가 X 
	CursorView(false);

	if (player.Game_cnt == 0) {
		gotoxy(65, 24);
		textcolor(6, BLACK);
		cout << "Hit";
		textcolor(WHITE, BLACK);
		cout << "(카드 한 장 추가)";

		gotoxy(48, 25);
		textcolor(6, BLACK);
		cout << "Stand";
		textcolor(WHITE, BLACK);
		cout << "(카드를 추가 하지 않고 현재 카드 합으로 딜러와 승부)";

		Sleep(5000);
		gotoxy(65, 24);
		cout << "                           ";
		gotoxy(48, 25);
		cout << "                                                                                ";
	}
	
	gotoxy(45, 24);
	cout << "Hit과 Stand 중 하나를 선택해 주세요!(h, s로 선택해 주세요)";

	gotoxy(70, 25);
	textcolor(6, BLACK);
	cout << "Hit / Stand";

	textcolor(WHITE, BLACK);
	while (1) {
		ch = _getch();
		char ch2 = ch;
		if (ch == 'h') {			//h를 선택했다면		
			P_Card_Random(player, dealer, Card_num);
			PD_Card_Sum(dealer, player, ch2);	    //플레이어, 딜러 카드 합해서 패배 결정
			break;
		}

		else if(ch == 's') {                      //s를 선택했다면
			PD_Card_Sum(dealer, player, ch);
			break;
		}
	}
}

void P_D_Reset(Player& player, Dealer& dealer) {	//게임 종료 시 플레이어/딜러 카드 모양/숫자 초기화
	PlaySound(L"reset.wav", 0, SND_FILENAME | SND_ASYNC);
	if (player.p_win < 2 && dealer.d_win < 2) {
		for (int i = 0; i < 20; i++) {
			if (player.player_card[i] != 0 || dealer.dealer_card[i] != 0) {
				player.player_card[i] = 0;
				dealer.dealer_card[i] = 0;
				player.shape[i] = " ";
				dealer.shape[i] = " ";
				player.Game_cnt++;

				//플레이어 카드 자리 리셋
				textcolor(BLACK, BLACK);
				for (int j = 11; j < 17; j++) {
					gotoxy(50 + (i * 10), j+1);
					cout << "               ";
				}
				

				//딜러 카드 자리 리셋
				for (int j = 32; j < 37; j++) {
					gotoxy(50 + (i * 10), j+1);
					cout << "               ";
				}
			}
		}

		Sleep(1000);
		textcolor(WHITE, BLACK);
		gotoxy(60, 24);
		cout << "5초 후 게임을 다시 시작합니다..";

		for (int i = 5; i >= 1; i--) {
			textcolor(RED, BLACK);	   
			gotoxy(74, 26);
			cout << i;
			Sleep(1000);
		}

		gotoxy(74, 26);
		cout << "   ";
		
		textcolor(WHITE, BLACK);
		gotoxy(60, 24);
		cout << "                                                ";

		Black_Jack(player, dealer);
	}

	else if (player.p_win == 2 || dealer.d_win == 2) {
		textcolor(BLACK, BLACK);
	  	if (player.p_win == 2 || dealer.d_win == 2) {		//플레이어가 2점 확보
			if (player.p_win == 2) {
				gotoxy(45, 24);
				cout << "플레이어가 2점을 먼저 확보하였으므로 게임이 종료됩니다!!";
			}
			else if (dealer.d_win == 2) {
				gotoxy(45, 24);
				cout << "딜러가 2점을 먼저 확보하였으므로 게임이 종료됩니다!!";
			}

			for (int i = 0; i < 20; i++) {
				if (player.player_card[i] != 0) {
					player.player_card[i] = 0;
					dealer.dealer_card[i] = 0;
					player.shape[i] = " ";
					dealer.shape[i] = " ";
					player.Game_cnt=0;

					//플레이어 카드 자리 리셋
					textcolor(BLACK, BLACK);
					for (int j = 11; j < 17; j++) {
						gotoxy(50 + (i * 10), j + 1);
						cout << "               ";
					}


					//딜러 카드 자리 리셋
					for (int j = 32; j < 37; j++) {
						gotoxy(50 + (i * 10), j + 1);
						cout << "               ";
					}
				}
			}

	  		Sleep(4000);
	  		gotoxy(45, 24);
	  		cout << "                                                                                  ";
			textcolor(WHITE, BLACK);
			gotoxy(55, 24);
	  		cout << "5초 후 게임 메인 화면으로 넘어갑니다!";

			for (int i = 5; i >= 1; i--) {
				textcolor(RED, BLACK);
				gotoxy(74, 26);
				cout  << i;
				Sleep(1000);
			}

			gotoxy(74, 26);
			cout << "        ";
	  		gotoxy(55, 24);
	  		cout << "                                                             ";

			textcolor(WHITE, BLACK);
			pb_outfile.close();
			pb_inputfile.close();
			Sleep(2000);
	  		Game_start();
			//exit(0);
	  	}
	}
}

void D_Card_Add() {
	if (dealer.dealer_sum > 17) {		  //딜러의 카드 합이 17이 초과했다면
		gotoxy(52, 24);
		cout << "딜러는 더이상 카드를 추가하지 않습니다..";

		Sleep(2000);
		gotoxy(50, 24);
		cout << "                                             ";
	}

	else if(dealer.dealer_sum <= 17) {  //딜러의 카드 합이 17을 초과하지 않았다면
		gotoxy(65, 24);
		cout << "딜러의 차례입니다";

		Sleep(2000);   
		gotoxy(65, 24);
		cout << "                           ";

		gotoxy(65, 24);
		cout << "딜러 카드 뽑는 중..";

		Sleep(3000);
		gotoxy(60, 24);
		cout << "                               ";

		D_Card_Random(dealer, player, Card_num);
	}
}

void Surrender(Player& player) {		//서렌더(Surrender) : 처음 받은 2장의 카드를 확인하고 "이길 수 없다"라고 판단될 경우 "항복" 하여 베팅의 절반을 돌려받을 수 있습니다.
	CursorView(false);

	if (player.Game_cnt == 0) {
		gotoxy(60, 24);
		cout << "2장의 카드를 확인하셨나요?";
		Sleep(3000);

		gotoxy(55, 24);
		cout << "'이길 수 없다'고 판단 되시나요?";
		gotoxy(45, 25);
		cout << "지금 항복하시면 배팅액의 절반을 돌려받으실 수 있습니다.";
		Sleep(4000);
	}

	gotoxy(55, 24);
	cout << "항복하시겠습니까?(y, n으로 선택해주세요)";
	gotoxy(45, 25);
	cout << "                                                                                         ";
	gotoxy(70, 25);
	textcolor(6, BLACK);
	cout << "YES / NO";
	textcolor(WHITE, BLACK);

	while (1) {
		ch = _getch();
		if (ch == 'y') {
			system("cls");
			gotoxy(55, 24);
			cout << "항복하셨으므로 배팅액의 절반을 얻으셨습니다!";

			pb_outfile.open("black_jack_betting.txt", ios::trunc);
			pb_outfile << player.player_money + (player.player_betting / 2);

			Sleep(3000);
			gotoxy(55, 24);
			cout << "                                               ";

			P_D_Reset(player, dealer);
		}
		else if (ch == 'n') {
			gotoxy(70, 25);
			cout << "               ";

			gotoxy(55, 24);
			cout << "                                                           ";

			gotoxy(65, 24);
			cout << "게임을 계속 진행합니다";
			Sleep(2000);

			gotoxy(65, 24);
			cout << "                                    ";
			break;
		}
	}
}

void PD_Card_Sum(Dealer& dealer, Player& player, char ch) {  //플레이어, 딜러 카드 합해서 패배 결정, Stand를 눌렀을 때 무승부 확인하고 카드 합해서 패배 결정
	Sleep(1000);
	gotoxy(45, 24);
	cout << "                                                                                 ";
	gotoxy(70, 25);
	cout << "                  ";

	CursorView(false);

	player.player_sum = 0;		//플레이어 카드 합 초기화

	for (int i = 0; i < 20; i++) {	
		if (player.player_card[i] == 11 || player.player_card[i] == 12 || player.player_card[i] == 13 || player.player_card[i] == 14) {	    //플레이어 A, K, Q, J 모두 10
			player.player_shape_card[i] = player.player_card[i];
			player.player_shape_card[i] = 10;
		}

		else {
			player.player_shape_card[i] = player.player_card[i];
		}

		if (ch == 'h') {	 //Hit을 누르고 나온 카드가 A 일 때 실행
			if (player.player_card[i] == 65) {	 //플레이어 카드[i]가 65 즉, A일 때 11로 바꾸기
				player.player_shape_card[i] = player.player_card[i];
				player.player_shape_card[i] = 11;
			}
		}

		player.player_sum += player.player_shape_card[i];
	}

	dealer.dealer_sum = 0;		  //딜러 카드 합 초기화
	for (int i = 0; i < 20; i++) {	
		if (dealer.dealer_card[i] == 11 || dealer.dealer_card[i] == 12 || dealer.dealer_card[i] == 13 || dealer.dealer_card[i] == 14) {		//딜러 A, K, Q, J 모두 10
			dealer.dealer_shape_card[i] = dealer.dealer_card[i];
			dealer.dealer_shape_card[i] = 10;
		}
		else {
			dealer.dealer_shape_card[i] = dealer.dealer_card[i];
		}
		
		dealer.dealer_sum += dealer.dealer_shape_card[i];
	}

	if (ch == 's') {      //ch가 's'일때만 실행 - 스탠드(stand), 둘 다 21이 넘지 않은 상태에서 대결
		if (dealer.dealer_sum < 21 && player.player_sum < 21) {	    //플레이어의 카드 합과 딜러의 카드 합이 모두 21을 넘지 않을 때
			if (dealer.dealer_sum == player.player_sum) {		   //딜러의 카드 합과 플레이어의 카드 합이 같을 때
				Show_DCard(dealer);
				gotoxy(45, 24);
				cout << "                                                                                  ";

				gotoxy(62, 23);
				cout << "플레이어 카드 합 : " << player.player_sum;

				gotoxy(63, 24);
				cout << "PUSH(무승부)입니다!";

				gotoxy(62, 25);
				cout << "딜러 카드 합 : " << dealer.dealer_sum;

				Sleep(5000);
				gotoxy(62, 23);
				cout << "                                "; 
				gotoxy(63, 24);
				cout << "                                       ";
				gotoxy(62, 25);
				cout << "                        ";

				gotoxy(30, 7);
				cout << "플레이어 승리 횟수 : " << player.p_win;
				gotoxy(30, 43);
				cout << "딜러 승리 횟수 : " << dealer.d_win;

				P_D_Reset(player, dealer); //게임 종료 후 플레이어, 딜러 카드 숫자, 모양 초기화
			}

			else if (dealer.dealer_sum > player.player_sum) {			 //딜러 카드 합 > 플레이어 카드 합 = 딜러 승리
				Show_DCard(dealer);
				PlaySound(L"p_lose.wav", 0, SND_FILENAME | SND_ASYNC);
				gotoxy(62, 23);
				cout << "플레이어 카드 합 : ";
				textcolor(6, BLACK);
				cout << player.player_sum;
				
				PlaySound(L"p_lose.wav", 0, SND_FILENAME | SND_ASYNC);

				textcolor(WHITE, BLACK);
				gotoxy(35, 24);
				cout << "딜러의 카드 숫자 합이 플레이어의 카드 숫자 합보다 21에 더 가까으므로 ";
				textcolor(6, BLACK);
				cout << "딜러의 승리";
				textcolor(WHITE, BLACK);
				cout << "입니다!!";
				
				gotoxy(62, 25);
				cout << "딜러 카드 합 : ";
				textcolor(6, BLACK); 
				cout << dealer.dealer_sum;

				textcolor(WHITE, BLACK);

				Sleep(5000);
				gotoxy(62, 23);
				cout << "                             ";
				gotoxy(35, 24);
				cout << "                                                                                                                                      ";
				gotoxy(62, 25);
				cout << "                             ";

				++dealer.d_win;

				gotoxy(30, 7);
				cout << "플레이어 승리 횟수 : " << player.p_win;
				gotoxy(30, 43);
				cout << "딜러 승리 횟수 : " << dealer.d_win;

				pb_outfile.open("black_jack_betting.txt", ios::trunc);
				pb_outfile << player.player_money - (player.player_betting * 0.5);

				P_D_Reset(player, dealer);	//게임 종료 후 플레이어, 딜러 카드 숫자, 모양 초기화
			}
			else if(dealer.dealer_sum < player.player_sum){		//딜러 카드 합 < 플레이어 카드 합 = 플레이어 승리
				Show_DCard(dealer);
				gotoxy(62, 23);
				cout << "플레이어 카드 합 : ";
				textcolor(6, BLACK);
				cout << player.player_sum;
				
				PlaySound(L"p_win.wav", 0, SND_FILENAME | SND_ASYNC);

				textcolor(WHITE, BLACK);
				gotoxy(35, 24);
				cout << "플레이어의 카드 숫자 합이 딜러의 카드 숫자 합보다 21에 더 가까으므로 ";
				textcolor(6, BLACK);
				cout << "플레이어의 승리";
				textcolor(WHITE, BLACK);
				cout << "입니다!!";
			
				gotoxy(62, 25);
				cout << "딜러 카드 합 : ";
				textcolor(6, BLACK);
				cout << dealer.dealer_sum;

				textcolor(WHITE, BLACK);

				Sleep(5000);
				gotoxy(62, 23);
				cout << "                             ";
				gotoxy(35, 24);
				cout << "                                                                                                                                      ";
				gotoxy(62, 25);
				cout << "                             ";

				++player.p_win;

				gotoxy(30, 7);
				cout << "플레이어 승리 횟수 : " << player.p_win;
				gotoxy(30, 43);
				cout << "딜러 승리 횟수 : " << dealer.d_win;

				pb_outfile.open("black_jack_betting.txt", ios::trunc);
				pb_outfile << player.player_money + (player.player_betting * 0.5);

				P_D_Reset(player, dealer);	//게임 종료 후 플레이어, 딜러 카드 숫자, 모양 초기화
			}
		}
	}

	else {   
		if (dealer.dealer_sum == 21 && player.player_sum == 21) {			//딜러와 플레이어의 카드 합이 모두 21이 된다면
			Show_DCard(dealer);
			CursorView(false);
			gotoxy(62, 23);
			cout << "플레이어 카드 합 : ";
			textcolor(6, BLACK);
			cout << player.player_sum;

			textcolor(WHITE, BLACK);
			gotoxy(48, 24);
			cout << "플레이어와 딜러 모두 카드 합이 21이 되었으므로 ";
			textcolor(6, BLACK);
			cout << "무승부";
			textcolor(WHITE, BLACK); 
			cout << "입니다!!";

			gotoxy(62, 25);
			cout << "딜러 카드 합 : "; 
			textcolor(6, BLACK);
			cout << dealer.dealer_sum;

			textcolor(WHITE, BLACK);

			Sleep(5000);
			gotoxy(62, 23);
			cout << "                             ";
			gotoxy(48, 24);
			cout << "                                                                                                ";
			gotoxy(62, 25);
			cout << "                             ";

			gotoxy(30, 7);
			cout << "플레이어 승리 횟수 : " << player.p_win;
			gotoxy(30, 43);
			cout << "딜러 승리 횟수 : " << dealer.d_win;

			P_D_Reset(player, dealer);	//게임 종료 후 플레이어, 딜러 카드 숫자, 모양 초기화
		}

		else if (player.player_sum > 21 && dealer.dealer_sum > 21) {			//딜러와 플레이어의 카드 합이 모두 21이 넘는다면
			CursorView(false);
			Show_DCard(dealer);
			gotoxy(62, 23);
			cout << "플레이어 카드 합 : "; 
			textcolor(6, BLACK);
			cout << player.player_sum;

			textcolor(WHITE, BLACK);
			gotoxy(48, 24);
			cout << "플레이어와 딜러 모두 카드 합이 21이 넘었으므로 ";
			textcolor(6, BLACK);
			cout << "무승부";
			textcolor(WHITE, BLACK);
			cout << "입니다!!";

			gotoxy(62, 25);
			cout << "딜러 카드 합 : ";
			textcolor(6, BLACK);
			cout << dealer.dealer_sum;

			textcolor(WHITE, BLACK);

			Sleep(3000);
			gotoxy(62, 23);
			cout << "                               ";
			gotoxy(48, 24);
			cout << "                                                                                        ";
			gotoxy(62, 25);
			cout << "                        ";

			gotoxy(30, 7);
			cout << "플레이어 승리 횟수 : " << player.p_win;
			gotoxy(30, 43);
			cout << "딜러 승리 횟수 : " << dealer.d_win;

			P_D_Reset(player, dealer);	//게임 종료 후 플레이어, 딜러 카드 숫자, 모양 초기화
		}

		else if (player.player_sum > 21) {		 //플레이어의 카드 합이 21이 넘는다면 
			CursorView(false);
			Show_DCard(dealer);
			gotoxy(62, 25);
			cout << "플레이어 카드 합 : ";
			textcolor(6, BLACK);
			cout << player.player_sum;

			textcolor(WHITE, BLACK);

			PlaySound(L"p_lose.wav", 0, SND_FILENAME | SND_ASYNC);

			gotoxy(50, 24);
			cout << "카드의 합이 21이 넘었으므로 ";
			textcolor(6, BLACK);
			cout << "플레이어의 패배";
			textcolor(WHITE, BLACK);
			cout << "입니다.";

			Sleep(4000);
			gotoxy(50, 24);
			cout << "                                                                          ";
			gotoxy(62, 25);
			cout << "                            ";				 //플레이어 카드 합 2초 뒤 없어지게 하는 문장

			++dealer.d_win;

			gotoxy(30, 7);
			cout << "플레이어 승리 횟수 : " << player.p_win;
			gotoxy(30, 43);
			cout << "딜러 승리 횟수 : " << dealer.d_win;

			pb_outfile.open("black_jack_betting.txt", ios::trunc);
			pb_outfile << player.player_money - (player.player_betting * 0.7);

			P_D_Reset(player, dealer);	//게임 종료 후 플레이어, 딜러 카드 숫자, 모양 초기화
		}

		else if (dealer.dealer_sum > 21) {		 //딜러의 카드 합이 21이 넘는다면 
			CursorView(false);
			Show_DCard(dealer);
			gotoxy(62, 25);
			cout << "딜러 카드 합 : ";
			textcolor(6, BLACK);
			cout << dealer.dealer_sum;

			textcolor(WHITE, BLACK);
			gotoxy(50, 24);
			cout << "카드의 합이 21이 넘었으므로  ";
			textcolor(6, BLACK); 
			cout << "딜러의 패배";
			textcolor(WHITE, BLACK); 
			cout << "입니다.";

			Sleep(3000);
			gotoxy(62, 25);
			cout << "                        ";
			gotoxy(50, 24);
			cout << "                                                                  ";

			gotoxy(50, 24);
			cout << "딜러의 패배로 플레이어는 배팅액의 1.5배를 얻게됩니다!";
			player.player_betting = player.player_betting * 1.5;	 //플레이어 배팅액 1.5배
			Sleep(4000);
			gotoxy(50, 24);
			cout << "                                                          ";

			++player.p_win;

			gotoxy(30, 7);
			cout << "플레이어 승리 횟수 : " << player.p_win;
			gotoxy(30, 43);
			cout << "딜러 승리 횟수 : " << dealer.d_win;

			pb_outfile.open("black_jack_betting.txt", ios::trunc);
			pb_outfile << player.player_money + (player.player_betting * 0.7);

			P_D_Reset(player, dealer);	//게임 종료 후 플레이어, 딜러 카드 숫자, 모양 초기화
		}

		else if (player.player_sum == 21) {				 //플레이어의 카드 합이 21이 된다면 승리
			CursorView(false);
			Show_DCard(dealer);
			gotoxy(62, 25);
			cout << "플레이어 카드 합 : ";
			textcolor(6, BLACK); 
			cout << player.player_sum;

			Sleep(2000);
			gotoxy(62, 25);
			cout << "                            ";				 //플레이어 카드 합 2초 뒤 없어지게 하는 문장

			PlaySound(L"p_win.wav", 0, SND_FILENAME | SND_ASYNC);

			gotoxy(33, 22);
			textcolor(13, BLACK);
			cout << "  ____    _          _        ____   _  __           _      _        ____    _  __";
			gotoxy(33, 23);
			cout << " | __ )  | |        / ＼     / ___| | |/ /          | |    / ＼     / ___|  | |/ /";

			gotoxy(33, 24);
			textcolor(12, BLACK);
			cout << " |  _ ＼ | |       / _ ＼   | |     | ' /       _   | |   / _ ＼   | |      | ' / ";
			gotoxy(33, 25);
			cout << " | |_) | | |___   / ___ ＼  | |___  | . ＼     | |_ | |  / ___ ＼  | |___   | . ＼";

			gotoxy(33, 26);
			textcolor(RED, BLACK);
			cout << " |____/  |_____| /_/  ＼_＼ ＼____| |_|＼_＼    ＼___/  /_/  ＼_＼  ＼____| |_|＼_＼";

			Sleep(3000);

			for (int i = 22; i < 27; i++) {
				gotoxy(33, i);
				cout << "                                                                                                                                    ";
			}

			gotoxy(48, 24);
			textcolor(WHITE, BLACK);
			cout << "플레이어의 카드 합이 21이 되었으므로 플레이어의 승리!!";
			Sleep(4000);
			gotoxy(48, 24);
			cout << "                                                          ";

			++player.p_win;

			gotoxy(30, 7);
			cout << "플레이어 승리 횟수 : " << player.p_win;
			gotoxy(30, 43);
			cout << "딜러 승리 횟수 : " << dealer.d_win;

			pb_outfile.open("black_jack_betting.txt", ios::trunc);
			pb_outfile << player.player_money + (player.player_betting * 1.5);

			P_D_Reset(player, dealer);	//게임 종료 후 플레이어, 딜러 카드 숫자, 모양 초기화
		}

		else if (dealer.dealer_sum == 21) {				 //딜러의 카드 합이 21이 된다면 승리
			CursorView(false);
			Show_DCard(dealer);
			gotoxy(62, 25);
			cout << "딜러 카드 합 : " << dealer.dealer_sum;

			Sleep(2000);
			gotoxy(62, 25);
			cout << "                            ";				 //딜러 카드 합 2초 뒤 없어지게 하는 문장


			PlaySound(L"p_lose.wav", 0, SND_FILENAME | SND_ASYNC);
			gotoxy(69, 24);
			textcolor(RED, BLACK);
			cout << "BLACK JACK";
			Sleep(2000);

			gotoxy(52, 24);
			textcolor(WHITE, BLACK);
			cout << "딜러의 카드 합이 21이 되었으므로 딜러의 승리!!";
			Sleep(2000);
			gotoxy(52, 24);
			cout << "                                                ";

			++dealer.d_win;

			gotoxy(30, 7);
			cout << "플레이어 승리 횟수 : " << player.p_win;
			gotoxy(30, 43);
			cout << "딜러 승리 횟수 : " << dealer.d_win;

			pb_outfile.open("black_jack_betting.txt", ios::trunc);
			pb_outfile << player.player_money - (player.player_betting * 1.5);

			P_D_Reset(player, dealer);	//게임 종료 후 플레이어, 딜러 카드 숫자, 모양 초기화
		}

		else if (player.player_sum < 21) {
			Sleep(1000);
			gotoxy(62, 25);
			cout << "플레이어 카드 합 : " << player.player_sum;

			Sleep(2000);

			gotoxy(62, 25);
			cout << "                             ";
		}
	}
}

void Shuffle(int* Card_num, int num) {	 //카드 섞어주기
	srand(time(NULL)); 
	int temp; 
	int rn; 
	for (int i = 0; i < num - 1; i++) {
		rn = rand() % (num - i) + i; 
		temp = Card_num[i];
		Card_num[i] = Card_num[rn];
		Card_num[rn] = temp; 
	}
}

void textcolor(int foreground, int background)	//글씨 색깔
{
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoxy(int x, int y) {				//좌표
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void CursorView(char cursor) {					//마우스 커서 안보이게
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = cursor;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void Game_start() {			//첫 화면(게임, 옵션, 종료)
	system("cls");
	CursorView(false);
	PlaySound(TEXT("background_m.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

	
	textcolor(13, BLACK);
	gotoxy(25, 7);
	cout << " /$$$$$$$" << "  /$$" << "        /$$$$$$" << "   /$$$$$$" << "  /$$   /$$" << "            " << "/$$$$$" << "  /$$$$$$" << "   /$$$$$$" << "  /$$   /$$";
	
	gotoxy(25, 8);
	cout << "| $$__  $$" << "| $$" << "       /$$__  $$" << " /$$__  $$" << "| $$  /$$/" << "           " << "|__  $$" << " /$$__  $$" << " /$$__  $$" << "| $$  /$$/";
	
	textcolor(12, BLACK);
	gotoxy(25, 9);
	cout << "| $$  | $$" << "| $$" << "      | $$  | $$" << "| $$  |__/" << "| $$ /$$/" << "               " << "| $$" << "| $$  | $$" << "| $$  |__/" << "| $$ /$$/";
	
	gotoxy(25, 10);
	cout << "| $$$$$$$" << " | $$" << "      | $$$$$$$$" << "| $$" << "      | $$$$$/" << "                " << "| $$" << "| $$$$$$$$" << "| $$" << "      | $$$$$/";
	
	textcolor(RED, BLACK);
	gotoxy(25, 11);
	cout << "| $$__  $$" << "| $$" << "      | $$__  $$" << "| $$" << "      | $$  $$" << "           " << "/$$  | $$" << "| $$__  $$" << "| $$" << "      | $$  $$";
	
	gotoxy(25, 12);
	cout << "| $$  | $$" << "| $$" << "      | $$  | $$" << "| $$    $$" << "| $$|  $$" << "         " << "| $$  | $$" << "| $$  | $$" << "| $$    $$" << "| $$|  $$";
	
	gotoxy(25, 13);
	cout << "| $$$$$$$/" << "| $$$$$$$$" << "| $$  | $$" << "|  $$$$$$/" << "| $$ |  $$" << "        " << "|  $$$$$$/" << "| $$  | $$" << "|  $$$$$$/" << "| $$ |  $$";
	
	gotoxy(25, 14);
	cout << "|_______/" << " |________/" << "|__/  |__/" << " |______/" << " |__/  |__/" << "         " << "|______/" << " |__/  |__/" << " |______/" << " |__/  |__/";
	textcolor(WHITE, BLACK);

	for (int i = 20; i < 37; i += 6) {
		gotoxy(59, i);
		textcolor(RED, BLACK);
		cout << "。";
		textcolor(WHITE, BLACK);
		cout << "______________________________";
		textcolor(RED, BLACK);
		cout << "。";
		textcolor(WHITE, BLACK);
		gotoxy(59, i + 1);
		cout << "|                               |";
		gotoxy(59, i + 2);
		cout << "|                               |";
		gotoxy(59, i + 3);
		cout << "|                               |";
		gotoxy(59, i + 4);
		textcolor(RED, BLACK);
		cout << "。";
		textcolor(WHITE, BLACK);
		cout << "------------------------------";
		textcolor(RED, BLACK);
		cout << "。";
	}

	textcolor(WHITE, BLACK);
	gotoxy(72, 22);
	cout << "1. 게임";
	gotoxy(72, 28);
	cout << "2.  옵션";
	gotoxy(72, 34);
	cout << "3. 종료";
	
	gotoxy(62, 38);
	cout << "1, 2, 3 중 하나를 선택하세요!";

	while (1)
	{
		ch = _getch();
		switch (ch) {
			case '1':
				system("cls");
				Sleep(300); //딜레이 0.3
				Black_Jack(player, dealer);
				break;

			case '2':
				PlaySound(L"bbok.wav", 0, SND_FILENAME | SND_ASYNC);
				system("cls");
				Sleep(300); //딜레이 0.3
				Game_option();
				break;

			case '3':
				PlaySound(L"bbok.wav", 0, SND_FILENAME | SND_ASYNC);
				system("cls");
				Sleep(300); //딜레이 0.3

				textcolor(RED, BLACK);
				gotoxy(45, 2);
				cout << "■■■    ■■" << "    ■■   ■" << "    ■■■■" << "    ■■■■";
				gotoxy(45, 3);
				cout << "    ■■■■■" << "  ■    ■ ■" << "       ■" << "             ■";
				gotoxy(45, 4);
				cout << "    ■    ■■" << "    ■■   ■" << "     ■  ■" << "     ■■■■";
				gotoxy(45, 5);
				cout << "    ■    ■■" << "     ■■■■" << "       ■" << "       ■";
				gotoxy(45, 6);
				cout << "                   ■    ■" << "   ■■■■■" << "   ■■■■";
				gotoxy(45, 7);
				cout << "                   ■■■■" << "     ■■■" << "       ■■";
				gotoxy(45, 8);
				cout << "                              ■      ■" << "     ■■";
				gotoxy(45, 9);
				cout << "                                ■■■" << "    ■■■■■";
	
				textcolor(WHITE, BLACK);
				gotoxy(60, 16);
				cout << "게임이 종료되었습니다!!";
				gotoxy(54, 18);
				cout << "3초 뒤 게임 메인 화면으로 넘어갑니다!";
				Sleep(3000);
				Game_start();
				break;
			}
	}
}

void  Game_option() {	//옵션
	system("cls");
	CursorView(false);

	textcolor(WHITE, BLACK);

	gotoxy(60, 2);
	cout << "┏            ㉠㉧㉦㉤          ┓";
	gotoxy(60, 3);
	cout << "┃                              ┃";
	gotoxy(60, 4);
	cout << "┃                              ┃";
	gotoxy(60, 5);
	cout << "┃                              ┃";
	gotoxy(60, 6);
	cout << "┃                              ┃";
	gotoxy(60, 7);
	cout << "┃                              ┃";
	gotoxy(60, 8);
	cout << "┃                              ┃";
	gotoxy(60, 9);
	cout << "┃                              ┃";
	gotoxy(60, 10);
	cout << " ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━";
	gotoxy(70, 6);
	cout << "1. 게임 설명";

	gotoxy(60, 14);
	cout << "┏            ㉠㉧㉥㉥          ┓";
	gotoxy(60, 15);
	cout << "┃                              ┃";
	gotoxy(60, 16);
	cout << "┃                              ┃";
	gotoxy(60, 17);
	cout << "┃                              ┃";
	gotoxy(60, 18);
	cout << "┃                              ┃";
	gotoxy(60, 19);
	cout << "┃                              ┃";
	gotoxy(60, 20);
	cout << "┃                              ┃";
	gotoxy(60, 21);
	cout << "┃                              ┃";
	gotoxy(60, 22);
	cout << " ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━";
	gotoxy(70, 18);
	cout << "2. 게임 방법";

	gotoxy(60, 26);
	cout << "┏            ㉠㉥㉨            ┓";
	gotoxy(60, 27);
	cout << "┃                              ┃";
	gotoxy(60, 28);
	cout << "┃                              ┃";
	gotoxy(60, 29);
	cout << "┃                              ┃";
	gotoxy(60, 30);
	cout << "┃                              ┃";
	gotoxy(60, 31);
	cout << "┃                              ┃";
	gotoxy(60, 32);
	cout << "┃                              ┃";
	gotoxy(60, 33);
	cout << "┃                              ┃";
	gotoxy(60, 34);
	cout << " ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━";
	gotoxy(72, 30);
	cout << "3. 개발자";

	gotoxy(62, 38);
	cout << "1, 2, 3 중 하나를 선택하세요. ";

	while (1) {
		ch = _getch();
		if (ch == 's') {
			PlaySound(L"bbok.wav", 0, SND_FILENAME | SND_ASYNC);
			Game_start();
		}
		else if (ch == '1' || ch == '2' || ch == '3') {
			switch (ch) {
			case '1':
				PlaySound(L"bbok.wav", 0, SND_FILENAME | SND_ASYNC);
				Sleep(300); //딜레이 0.3
				Game_option_o1();  //옵션1
				break;

			case '2':
				PlaySound(L"bbok.wav", 0, SND_FILENAME | SND_ASYNC);
				Sleep(300); //딜레이 0.3
				Game_option_o2();	  //옵션2
				break;

			case '3':
				PlaySound(L"bbok.wav", 0, SND_FILENAME | SND_ASYNC);
				Sleep(300); //딜레이 0.3
				Game_option_o3();	 //옵션3
				break;

			default:
				cout << setw(195) << "잘못된 번호를 입력하셨습니다 다시 입력해주세요!" << endl;
				break;
			}
		}
	}
}

void Game_option_o1() {		//옵션 자세히01
	system("cls");
	cout.setf(ios::right);
	CursorView(false);

	textcolor(15, BLACK);
	gotoxy(40, 2);
	cout << "♤♤♤    ♤♤" << "    ♤♤   ♤" << "       ♤     ♤" << "    ♤♤♤♤     ♤";
	gotoxy(40, 3);
	cout << "    ♤♤♤♤♤" << "  ♤    ♤ ♤" << "     ♤♤   ♤♤" << "    ♤    ♤ ♤♤♤";
	textcolor(14, BLACK);
	gotoxy(40, 4);
	cout << "    ♡    ♡♡" << "    ♡♡   ♡" << "   ♡    ♡   ♡" << "    ♡    ♡ ♡♡♡";
	gotoxy(40, 5);
	cout << "    ♡    ♡♡" << "     ♡♡♡♡" << "        ♡♡♡♡" << "    ♡♡♡♡     ♡";
	gotoxy(40, 6);
	cout << "                   ♧    ♧" << "              ♧" << "            ♧♧♧♧";
	textcolor(6, BLACK);
	gotoxy(40, 7);
	cout << "                   ♧♧♧♧" << "        ♧♧♧♧" << "          ♧      ♧";
	gotoxy(40, 8);
	cout << "                                   ◇" << "                ◇      ◇";
	gotoxy(40, 9);
	cout << "                                   ◇◇◇◇" << "            ◇◇◇";

	//cout << endl << endl << endl << endl << endl << endl;
	textcolor(WHITE, BLACK);
	gotoxy(28, 13);
	cout << "◇이 게임은 트럼프 카드로 하는 BLACK JACK 게임입니다.";
	gotoxy(28, 14);
	cout << "♡딜러와 플레이어 중 카드의 합이 21 또는 21에 가장 가까운 숫자를 가지는 쪽이 이기는 게임입니다.";
	gotoxy(28, 15);
	cout << "♤자세한 게임 설명은 <2번. 게임 방법>을 선택해 주세요";
	gotoxy(28, 16);
	cout << "♧되돌아가려면 s를 눌러주세요";
	/*gotoxy(37, 17);
	cout << ">>";*/

	for (int i = 12; i < 18; i++) {
		textcolor(6, BLACK);
		gotoxy(25, i);
		cout << ":";

		for (int j = 12; j < 18; j++) {
			gotoxy(125, j);
			cout << "：";
		}

		for (int k = 26; k < 126; k++) {
			gotoxy(k, 18);
			cout << ".";
		}
	}

	while (1) {
		ch = _getch();
		if (ch == 's') {
			PlaySound(L"bbok.wav", 0, SND_FILENAME | SND_ASYNC);
			Sleep(300);			//딜레이 0.3
			Game_option();		//옵션
		}
	}
}

void Game_option_o2() {		//옵션 자세히02
	system("cls");
	cout.setf(ios::right);
	CursorView(false);

	textcolor(15, BLACK);
	gotoxy(40, 2);
	cout << "◇◇◇    ◇◇" << "    ◇◇   ◇" << "    ◇    ◇  ◇" << "      ◇    ◇   ◇";
	gotoxy(40, 3);
	cout << "    ◇◇◇◇◇" << "  ◇    ◇ ◇" << "    ◇◇◇◇  ◇◇" << "    ◇◇◇◇ ◇◇";
	textcolor(10, BLACK);
	gotoxy(40, 4);
	cout << "    ♤    ♤" << "      ♤♤   ♤" << "    ♤    ♤  ♤" << "      ♤    ♤   ♤";
	gotoxy(40, 5);
	cout << "    ♤    ♤♤" << "     ♤♤♤♤" << "    ♤♤♤♤  ♤" << "      ♤♤♤♤   ♤";
	gotoxy(40, 6);
	cout << "                   ♡    ♡" << "          ♡♡♡" << "          ♡    ♡";
	textcolor(11, BLACK);
	gotoxy(40, 7);
	cout << "                   ♡♡♡♡" << "        ♡      ♡" << "        ♡♡♡♡";
	gotoxy(40, 8);
	cout << "                                   ♧      ♧" << "        ♧    ♧";
	gotoxy(40, 9);
	cout << "                                     ♧♧♧" << "          ♧♧♧♧";

	textcolor(WHITE, BLACK);
	gotoxy(20, 15);
	cout << "1. 카드를 받기 전 먼저 베팅을 합니다.";

	gotoxy(20, 16);
	cout << "2. 딜러와 플레이어는 2장의 카드를 랜덤으로 받습니다.";

	gotoxy(20, 17);
	cout << "3. 플레이어는 처음 받은 두 장의 카드를 확인하고  이길 수 없다고 판단될 경우 Surrender(항복)하여 베팅의 절반을 돌려받을 수 있습니다";

	gotoxy(20, 18);
	cout << "4. 자신이 갖고 있는 카드의 합이 21에 가까워 지도록 카드를 Hit(추가)하거나 Stand(그대로)를 선택할 수 있습니다.";

	gotoxy(20, 19);
	cout << "5. 플레이어는 21을 초과하지 않는 한 원하는만큼 카드를 Hit(추가) 할 수 있습니다.";

	gotoxy(20, 20);
	cout << "6. 딜러는 카드의 합이 17을 초과할 때까지 카드를 Hit(추가)합니다.";

	gotoxy(20, 21);
	cout << "7. 카드의 합이 21을 넘어버린 시점 Bust에서 패배가 결정됩니다.";

	textcolor(6, BLACK);
	gotoxy(20, 22);
	cout << "<<베팅금액 계산법>>";
	textcolor(WHITE, BLACK);
	gotoxy(20, 23);
	cout << "1. Surrender(항복)한다면 베팅금액의 절반++";
	gotoxy(20, 24);
	cout << "2. Stand했을 때 딜러가 이긴다면 베팅금액의 절반 --";
	gotoxy(20, 25);
	cout << "3. Stand했을 때 플레이어가 이긴다면 베팅금액의 절반++";
	gotoxy(20, 26);
	cout << "4. 딜러가 black_jack 이 나올 경우 베팅금액의 1.5배 --";
	gotoxy(20, 27);
	cout << "5. 플레이어가 black_jack이 나올 경우 베팅금액의 1.5배++";
	gotoxy(20, 27);
	cout << "6. Hit 했을 때 딜러가 이긴다면 베팅금액의 0.7배 --";
	gotoxy(20, 27);
	cout << "7. Hit 했을 때 플레이어가 이긴다면 베팅금액의 0.7배++";
	gotoxy(20, 27);
	cout << "8. 무승부라면 베팅금액 다시 돌려줌";

	textcolor(BLACK, WHITE);
	gotoxy(55, 30);
	cout << "게임 이해가 다 되셨다면 되돌아가겠습니까?";

	gotoxy(60, 32);
	cout << "되돌아가려면 s를 눌러주세요";

	textcolor(WHITE, BLACK);
	while (1) {
		ch = _getch();
		if (ch == 's') {
			PlaySound(L"bbok.wav", 0, SND_FILENAME | SND_ASYNC);
			Sleep(300); //딜레이 0.3
			Game_option();	 //옵션
		}
	}
}

void Game_option_o3() {		//옵션 자세히02
	system("cls");
	cout.setf(ios::right);
	CursorView(false);
	cout << setw(235) << "개발자는 [윤서연]입니다." << endl;
	cout << setw(236) << "되돌아가려면 s를 눌러주세요" << endl;
	cout << setw(220) << ">>";
	while (1) {
		ch = _getch();
		if (ch == 's') {
			PlaySound(L"bbok.wav", 0, SND_FILENAME | SND_ASYNC);
			Sleep(300); //딜레이 0.3
			Game_option();	 //옵션
		}
	}
}

int main() {
	system("mode con: cols=150 lines=47 | title BLACK JACK"); //lines = 45
	Game_start();

	return 0;
}