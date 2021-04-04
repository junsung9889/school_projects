#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "stackqueue.h"
#define DAYS 31                              //Day입니다
#define SPEED (500.0)  // 비행기의 속력입니다 500km/hour



typedef struct point {
	char name;
	int x, y;
	int distance, needTime, departureTime;        // 기존의 distance이외에 needtime과 departureTime, paths[26], pathsNum이 추가되었습니다.
	struct point* next;                        //distance는 adjency list의 edge부분에서만 활성화되며, default값은 -1이고, addpath로 등록시 두 city간 거리가 기록됩니다.
	char paths[26];                           //needTime은 다이스트라 알고리즘의 해당 city까지 필요한 cost로서, 경로상의 city와 departureTime과 distance에 의해 산출됩니다.
	int pathsNum;                           //paths[26]은 해당 city까지 최단거리로 오는 city의 리스트입니다. pathsNum은 paths[26]의 길이를 나타냅니다
}City;

typedef struct a {
	int num;
	City** heads;

}Graph;

typedef struct {
	char name[20], source, destination;
	int departureTime, departureDate, arrivalTime, arrivalDate, flightTime;
	int level;
	char flightPath[30];

}Passenger;

typedef struct Node {
	int key;//= reservation number
	int color;
	struct Node* parent;
	struct Node* leftc;
	struct Node* rightc;
	Passenger* passenger;
}node;
node* root;
node* nil;


//RBT==============================================
node* makenode(int key);
void initTree();
node* isexist(node* root, int key);
void Lrotate(node* cur);
void Rrotate(node* cur);
void insert_fixup(node* cur);
node* insert(node* nod);
node* minnode(node* cur);
node* successor(node* cur);
void delete_fixup(node* cur);
node* delet(node* nod);
void printRBT(node* root, int level);
void printreserve(node* root);
int height(node* root);

//Graph============================================

Graph* CreateGraph(int num);                  // num개의 city가 있는 기본 그래프를 생성합니다.
void DestroyGraph(Graph* pgraph);               // 그래프 하나를 전부 메모리에 반환합니다.
void AddPath(Graph* pgraph, char src, char dest);   //pgraph내의 2개 city간에 bidriectional edge를 1개 생성합니다
void AddPaths(Graph* pgraph, int num);            //pgraph내에 num개만큼 랜덤하게 AddPath를 실행합니다.
int PathExists(Graph* pgraph, char src, char dst);  // AddPath에만 활용되는 함수입니다. 기존 그래프에 동일 path가 생성되어있는지 확인합니다.
Graph** CreateTimeTable(Graph* pgraph);            //기본 그래프 pgraph에 departure time을 할당한 31가지 그래프를 만듭니다.
void PrintGraph(Graph* pgraph);                  //1개 graph의 내용을 출력합니다.
City* ShortestPath(Graph** pgraphs, int depDate, int src, int dst);   // pgraph내에 내용을 기록한 후 최종 목적도시를 반환합니다. 
//여기서 needTime을 참조하면 도착예정시간, paths를 참조하면 경로상city가 담긴array, pathsNum은 pathsArray의 길이를 나타냅니다.
City* ExtractMin(City** Q, int* qNum);            // ShortestPath내에서만 쓰이는 함수로서 수도코드의 ExtractMin과 동일합니다.
void SetDepartureTime(Graph** pgraphs, int nowDate);
void PrintArray(char* arr);         // char array인쇄.
void PrintResult(Passenger* values);
Passenger* module(char* name, char source, char destination, int date, int level);

Graph* pgraph, ** pgraphs;


int main() {
	srand(time(NULL));

	int num, day;
	char name[20] = { '\0' };
	char src, dst;
	int reservationNumber;
	int seat, level;
	int h = 0;
	int status = 1;

	node* temp = NULL;
	Passenger* values = NULL;
	node** latest = (node * *)malloc(sizeof(node*));

	initTree();

	pgraph = CreateGraph(26);         //26개 city를 생성합니다.
	AddPaths(pgraph, 100);                  //해당 26개 city간에 path를 무작위로 100개를 생성합니다.
	pgraphs = CreateTimeTable(pgraph);   //위에 생성된 path에 무작위 departureTime을 부여한 graph 31개를 생성합니다.

	int length;

	printf("Added 500 reservations\n");
	printf("Latest 10 Reservations------------------------\n");

	for (num = 0; num < 500; num++) {
		for (int i = 0; i < 20; i++) name[i] = '\0';

		length = rand() % 5 + 3;      //length of name
		name[0] = rand() % 26 + 65;      //first character of name(Cap)
		for (int i = 1; i < length; i++) {
			name[i] = rand() % 26 + 97;      //orthrt characters of name
		}

		day = rand() % 31 + 1;
		src = rand() % 26 + 97;
		dst = rand() % 26 + 97;
		while (dst == src) dst = rand() % 26 + 97;

		//확률에 따라 grade 배정
		seat = rand() % 100;
		if (seat < 4) level = 1;
		else if (seat < 11) level = 2;
		else level = 3;


		values = module(name, src, dst, day, level);   //여기에 name, source, destination, date를 입력하면!!      
		if (values == NULL) {
			num--;
			continue;
		}
		while (1) {
			temp = makenode(rand() % 10000);
			temp->passenger = values;
			if (NULL != insert(temp)) break;
		}
		if (num >= 490) printreserve(temp);
	}


	//input
	int rnum;
	int choice = 0;
	char yn;
	while (status) {

		printf("\nWhat do you want to do?\n");
		printf("1. Reservation\n");
		printf("2. Cancel\n");
		printf("3. Search\n");
		printf("4. Exit\n");


		scanf("%d", &choice);

		switch (choice) {
		case 1:
			printf("\n==================== Reservation ======================= \n");
			printf("Enter the name: "); scanf("%s", &name); getchar();
			printf("Enter the day(1~31): "); scanf("%d", &day); getchar();
			while (!(day >= 1 && day <= 31)) {
				printf("Wrong Input! Please input number of 1 ~ 31.\n");
				printf("Enter the day: "); scanf("%d", &day); getchar();
			}
			printf("Enter the source city (a~z): "); scanf("%c", &src); getchar();
			while (!(src >= 'a' && src <= 'z')) {
				printf("Wrong Input! Please input a ~ z.\n");
				printf("Enter the source city: "); scanf("%c", &src); getchar();
			}
			printf("Enter the destination city (a~z): "); scanf("%c", &dst); getchar();
			while (!(dst >= 'a' && dst <= 'z')) {
				printf("Wrong Input! Please input a ~ z.\n");
				printf("Enter the destination city: "); scanf("%c", &dst); getchar();
			}
			printf("Enter the level of seat(1~3): "); scanf("%d", &level); getchar();
			while (!(level >= 1 && level <= 3)) {
				printf("Wrong Input! Please input number of 1 ~ 3.\n");
				printf("Enter the level of seat: "); scanf("%d", &level); getchar();
			}
			num++;

			values = module(name, src, dst, day, level);
			if (values == NULL) {
				printf("Paths not found!\n");
				break;
			}
			while (1) {
				temp = makenode(rand() % 10000);
				temp->passenger = values;
				if (NULL != insert(temp)) break;
			}
			printf("Reservated-------------------------------------------- \n");
			printreserve(temp);

			h = height(root);
			printf("Number of reservation: %d\n", num);
			printf("Height is %d\n", h);
			break;

		case 2:
			printf("\n====================== Cancel ========================== \n");
			printf("Enter the reservation number for cancel: ");
			scanf("%d", &rnum);
			temp = isexist(root, rnum);
			while (temp == NULL || temp->key == -1) {
				printf("There are no reservation!\n");
				printf("Pleasse enter the number correctly.\n");
				scanf("%d", &rnum);
				temp = isexist(root, rnum);
			}

			printreserve(temp);

			delet(isexist(root, rnum));
			num--;

			printf("The reservation is canceled.\n");

			h = height(root);
			printf("Number of reservation: %d\n", num);
			printf("Height is %d\n", h);

			break;
		case 3:
			printf("\n====================== Search ========================== \n");
			printf("Enter the reservation number for search: ");
			scanf("%d", &rnum);
			temp = isexist(root, rnum);
			while (temp == NULL || temp->key == -1) {
				printf("There are no reservation!\n");
				printf("Pleasse enter the number correctly.\n");
				scanf("%d", &rnum);
				temp = isexist(root, rnum);
			}
			printreserve(temp);
			break;
		case 4:
			status = 0;
			break;

		default:
			printf("Wrong number.\nPlease input number correctly\n");
			break;
		}
	}                                          //to here//

	DestroyGraph(pgraph);                  //메모리해제
	for (int i = 0; i < DAYS; i++) DestroyGraph(pgraphs[i]);
	return 0;
}
Passenger* module(char* name, char source, char destination, int date, int level) {
	Passenger* result = (Passenger*)malloc(sizeof(Passenger));
	char src = source, dst = destination;
	int day = date;
	int lev = level;
	result->level = lev;
	for (int i = 0; i < 20; i++) result->name[i] = name[i];

	City* srcCity = pgraphs[31]->heads[src - 'a'];
	City* dstCity = ShortestPath(pgraphs, date, src, dst);
	if (dstCity->needTime > 0) {            //제대로 찾음 ㅎ
		result->source = src;
		result->destination = dst;
		for (int i = 0; i < dstCity->pathsNum; i++) result->flightPath[i] = dstCity->paths[i];
		result->flightPath[dstCity->pathsNum] = '\0';
		SetDepartureTime(pgraphs, date);         //해당날짜의 비행기 departureTIme을 불러온후그래프에적용
		City* adj = pgraphs[31]->heads[src - 'a'];   // src시작city의 인접(adj) city들에 대한 반복문
		while (adj->next != NULL) {            //이것을 수행하는 목적은? 첫 비행기탑승시간의 파악.
			adj = adj->next;
			if (adj->name == dstCity->paths[1]) {
				result->departureTime = adj->departureTime % (24 * 60);   //시간을 구하긴위해선 하루=24*60
				//의나머지를구하여한다.
				result->departureDate = adj->departureTime / (24 * 60) + 1;//나눗셈의 몫+1값이 date가 될것이다.
				break;
			}
		}
		result->arrivalTime = dstCity->needTime % (24 * 60);   //위의 departureTIme,date구하는과정과 동일
		result->arrivalDate = dstCity->needTime / (24 * 60) + 1;   //Time은 분기준,date하나는 24*60분
		result->flightTime = (dstCity->needTime) - (adj->departureTime);
		/*if (result->arrivalTime / 60 >= 24) {
		result->arrivalTime -= 24 * 60;
		result->arrivalDate++;
		}*/// arrivaltime은 modulus를 취하므로 24시를 초과할일 없다.=>주석처리

	}
	else result = NULL;

	return result;
}
void PrintArray(char* arr) {
	printf("(");
	int i = 0;
	while (1) {
		printf("%c", arr[i++]);
		if (arr[i] != '\0') printf(" ");
		else break;
	}
	printf(")\n");
}
Graph* CreateGraph(int num) {
	Graph* pgraph = (Graph*)malloc(sizeof(Graph));
	City* head;
	if (NULL == pgraph) return NULL;
	pgraph->num = num;
	pgraph->heads = (City * *)malloc(sizeof(City*) * num * 2);
	if (NULL == pgraph->heads) return NULL;
	for (int i = 0; i < num; i++) {
		head = (City*)malloc(sizeof(City));
		if (NULL == pgraph->heads || NULL == head) return NULL;
		(pgraph->heads)[i] = head;
		head->name = 'a' + i;
		head->next = NULL;
		head->x = (rand() % 6001) - 3000;
		head->y = (rand() % 6001) - 3000;
		head->distance = -1;
		head->needTime = -1;
		head->departureTime = -1;
	}
	return pgraph;
}
void DestroyGraph(Graph* pgraph) {
	City* cur, *tmp;
	for (int i = 0; i < pgraph->num; i++) {
		cur = pgraph->heads[i];
		while (cur != NULL) {
			tmp = cur;
			cur = cur->next;
			free(tmp);
		}
	}
	free(pgraph->heads);
	free(pgraph);
	return;
}
void AddPath(Graph* pgraph, char src, char dest) {
	if (PathExists(pgraph, src, dest) == 1) {
		//printf("Path exists!\n");
		return;
	}
	City* newCity1, *newCity2, *head1, *head2, *cur;
	int distance;
	head1 = pgraph->heads[dest - 'a'];
	head2 = pgraph->heads[src - 'a'];
	distance = (head1->x - head2->x) * (head1->x - head2->x) +
		(head1->y - head2->y) * (head1->y - head2->y);//blabla
	distance = (int)(sqrt(distance));

	newCity1 = (City*)malloc(sizeof(City));
	newCity2 = (City*)malloc(sizeof(City));
	if (NULL == newCity1 || NULL == newCity2) return;
	*newCity1 = *head1;
	*newCity2 = *head2;
	newCity1->next = NULL;
	newCity2->next = NULL;
	newCity1->distance = distance;
	newCity2->distance = distance;
	newCity1->needTime = -1;
	newCity2->needTime = -1;

	cur = head1;
	while (cur->next != NULL) cur = cur->next;
	cur->next = newCity2;
	cur = head2;
	while (cur->next != NULL) cur = cur->next;
	cur->next = newCity1;
}
void AddPaths(Graph* pgraph, int num) {
	City* cur;
	int src, dst;
	for (int i = 0; i < num; i++) {
		src = rand() % 26 + 'a';
		dst = rand() % 26 + 'a';
		while (src == dst) {
			dst = rand() % 26 + 'a';
		}
		AddPath(pgraph, src, dst);
	}
}
int PathExists(Graph* pgraph, char src, char dst) {
	int result = 0;
	City** heads = pgraph->heads, *cur;
	cur = heads[src - 'a'];
	while (cur->next != NULL) {
		cur = cur->next;
		if (cur->name == dst) {
			result = 1;
			break;
		}
	}
	//printf("\n");
	return result;
}
Graph** CreateTimeTable(Graph* pgraph) {
	Graph** graphs = (Graph * *)malloc(sizeof(Graph*) * (DAYS + 1));
	City** heads1, ** heads2, *cur1, *cur2, *new;
	//
	for (int i = 0; i < DAYS + 1; i++) {
		heads1 = pgraph->heads;
		heads2 = (City * *)malloc(sizeof(City*) * (pgraph->num));
		if (NULL == graphs) return 0;
		graphs[i] = (Graph*)malloc(sizeof(Graph));
		graphs[i]->heads = heads2;
		graphs[i]->num = pgraph->num;
		for (int j = 0; j < pgraph->num; j++) {
			cur1 = heads1[j];
			cur2 = (City*)malloc(sizeof(City));
			heads2[j] = cur2;
			*(cur2) = *(cur1);
			cur2->next = NULL;
			while (cur1->next != NULL) {
				cur1 = cur1->next;
				cur2->next = (City*)malloc(sizeof(City));
				cur2 = cur2->next;
				*(cur2) = *(cur1);
				cur2->next = NULL;
				cur2->departureTime = (rand() % (24 * 60)) + (60 * 24 * i);
			}
		}
	}
	return graphs;
}
void PrintGraph(Graph* pgraph) {
	printf("City|x-cor|y-cor|distance|departureTime|arriveTime\n");
	City* cur, ** heads = pgraph->heads;
	int arrive;
	for (int i = 0; i < 26; i++) {
		printf("==================================================\n");
		cur = heads[i];
		printf("%3c |%5d|%5d|\n\n", cur->name, cur->x, cur->y);
		while (cur->next != NULL) {
			cur = cur->next;
			arrive = cur->departureTime + (int)(cur->distance / SPEED * 60);
			printf("%3c |%5d|%5d|%8d| %4dh %4dm | %3dh %3dm\n", cur->name, cur->x, cur->y,
				cur->distance, cur->departureTime / 60, cur->departureTime % 60, arrive / 60, arrive % 60);
		}
	}
	printf("==================================================\n");
	return;
}
City* ExtractMin(City** Q, int* qNum) {   //모든 City중에서 NeedTime이 가장적게소요되는 city찾기
	City* result = NULL;            //무한대인city는 선택될 수없음(needtime==-1은불가)
	int min = (1 << 30) - 1, mindex = -1;
	for (int i = 0; i < *qNum; i++) {
		if (Q[i]->needTime == -1) continue;
		if (min >(Q[i]->needTime)) {
			min = Q[i]->needTime;
			mindex = i;
		}
	}
	if (mindex == -1) return NULL;
	//deleting
	for (int i = 0; i < *qNum; i++) {
		if (i == mindex) {
			result = Q[i];
			for (int j = i; j < (*qNum) - 1; j++) {
				Q[j] = Q[j + 1];
			}
			break;
		}
	}
	//printf("%c ", result->name);
	(*qNum)--;
	return result;
}
void SetDepartureTime(Graph** pgraphs, int nowDate) {   //해당날짜의 departureTime으로 설정
	//printf("hihi%d\n", nowDate);
	City** heads1 = pgraphs[nowDate - 1]->heads, ** heads2 = pgraphs[31]->heads, *adj, *adj2;
	//heads2 = pgraphs[nowDate]->heads;
	for (int i = 0; i < pgraphs[31]->num; i++) {
		adj = heads1[i];
		adj2 = heads2[i];
		while (adj->next != NULL) {
			adj = adj->next;
			adj2 = adj2->next;
			adj2->departureTime = adj->departureTime;
		}
	}
}
City* ShortestPath(Graph** pgraphs, int depDate, int src, int dst) {
	if (src == dst) {                  //예외처리, 출발도시와 도착도시가 같은경우
		printf("source can't equal destination.");
		return -1;
	}
	Graph* pgraph = pgraphs[31];         //pgraph는 항상 pgraph[31]로 고정, 추후에 departuretime만
	SetDepartureTime(pgraphs, depDate);      //해당날짜로 바꿔서 계속씀
	char oldPaths[26] = { 0, };            //paths[26]복원용
	City** heads = pgraph->heads, ** Q, *u = NULL, *adj, *v;
	Q = (City * *)malloc(sizeof(City*) * pgraph->num);
	//알고리즘에 따른 City들의 집합 Q구현
	int qNum = pgraph->num, oldNeedTime, oldPathsNum, arrive, nowDate, oldnowDate;
	for (int i = 0; i < pgraphs[31]->num; i++) {
		if (i == src - 'a') {
			heads[i]->needTime = 60 * 24 * (depDate - 1);//해당출발날짜의 최소시간
			heads[i]->pathsNum = 1;         //paths설정
			heads[i]->paths[0] = src;      //paths설정
		}
		else {
			heads[i]->needTime = -1;      //-1은 다이스트라 알고리즘에서의 무한대를 의미
			heads[i]->pathsNum = 0;         //paths설정
		}
	}

	Queue* S = CreateQueue();         //다이스트라 알고리즘에 따른 S구현 뭔지는모름
	for (int i = 0; i < pgraph->num; i++) Q[i] = heads[i];   //Q에모든city들을 집어넣습니다.
	while (qNum > 0) {
		u = ExtractMin(Q, &qNum);            //현재 relaxing할 city를 추출합니다
		if (u == NULL) break;               //더이상 extract할것이없다?끝
		nowDate = u->needTime / (60 * 24) + 1;      //그 도시에 따른 현재날짜 계산
		if (nowDate > 31 || nowDate - depDate >= 3) break;//날짜가 31일 넘어가면 비행기없음,departureTIme테이블없음
		//아니면 3일자 여행 반복중이면 곤란하다.
		if (u->name == dst) break;            //dst도시의 최솟값을 확보했다..
		SetDepartureTime(pgraphs, nowDate);      //날짜에 따른 departureTime반영
		EnQueue(S, u);                     //뭔지모르겠다 ,일단 S에넣어서저장해줍니다
		adj = u;
		oldnowDate = nowDate;
		while (adj->next != NULL) {            //u의 모든 adjecncy한city 근접city에대하여 for문
			adj = adj->next;
			v = heads[adj->name - 'a'];         //도착지 v city에대한 정보
			oldNeedTime = v->needTime;         //backtracking(복원)을 위한 oldvalue생성
			for (int i = 0; i < v->pathsNum; i++) {
				oldPaths[i] = v->paths[i];      //이하같음..
			}
			oldPathsNum = v->pathsNum;         //..
			while (1) {
				arrive = adj->departureTime + (int)(adj->distance / SPEED * 60);//거기에 도착하면 몇시지? 도착시간예측
				if (v->needTime == -1 || v->needTime > arrive) {
					//v->needtime이 무한대이거나, 아니면 새로예상되는
					//도착시간이 기존정보보다 빠르다, 정보의 업데이트필요!
					v->needTime = arrive;         //v 도착시 예상시간업데이트
					for (int i = 0; i < u->pathsNum; i++) v->paths[i] = u->paths[i];
					v->paths[u->pathsNum] = v->name;//이하 path, pathsNum또한 업데이트해줍니다
					v->pathsNum = u->pathsNum + 1;
				}
				if (u->needTime > adj->departureTime) {//그런데?출발지 u에서의 시간보다 v로의 비행기출발시간 암튼비행기놓침
					nowDate++;                     //다음날비행기를알아보자..
					v->needTime = oldNeedTime;         //adj->depratureTime이 더 빠르다?
					for (int i = 0; i < v->pathsNum; i++) v->paths[i] = oldPaths[i];
					v->pathsNum = oldPathsNum;      //그러면 못타는 비행기였으므로 다시 원래대로 backtracking
					if (nowDate > 31) {
						break;         //날짜가 31일 넘어가면 비행기없음,dep
					}
					SetDepartureTime(pgraphs, nowDate);      //날짜에 따른 departureTime반영 이제 왠만하면 다음에는 else로 빠질것이다..
				}
				else {
					SetDepartureTime(pgraphs, oldnowDate);      //날짜에 따른 departureTime반영
					nowDate = oldnowDate;                  // nowdate복구
					//if(oldnowDate!=2) printf("hihi%d", oldnowDate);
					break;
				}
			}
		}
	}
	free(S);
	heads[dst - 'a']->paths[heads[dst - 'a']->pathsNum] = '\0';
	return heads[dst - 'a'];
}

//=============================RBT======================================

node* makenode(int key) {
	node* temp = (node*)malloc(sizeof(node));
	temp->parent = nil;
	temp->leftc = nil;
	temp->rightc = nil;
	temp->passenger = NULL;
	temp->key = key;
	temp->color = 0;

	return temp;
}

void printreserve(node* root) {
	if (root == NULL || root->passenger == NULL) return;
	int hour, min;

	//printf("Reservated---------------------------------------\n");
	printf("Reservation Number: %d\n", root->key);
	printf("Name of passenger : %s\n", root->passenger->name);
	if (root->passenger->level == 1) printf("Grade of seat : First class\n");
	else if (root->passenger->level == 2) printf("Grade of seat : Business class\n");
	else if (root->passenger->level == 3) printf("Grade of seat : Economy class\n");
	printf("From : %c\n", root->passenger->source);
	hour = root->passenger->departureTime / 60;
	min = root->passenger->departureTime % 60;
	printf("Departure Date & time : %d / %d : %d\n", root->passenger->departureDate, hour, min);
	printf("To : %c\n", root->passenger->destination);
	hour = root->passenger->arrivalTime / 60;
	min = root->passenger->arrivalTime % 60;
	printf("Arrival Date & time : %d / %d : %d\n", root->passenger->arrivalDate, hour, min);
	hour = root->passenger->flightTime / 60;
	min = root->passenger->flightTime % 60;
	printf("Flight time : %d : %d\n", hour, min);
	printf("Flight path : ");
	PrintArray(root->passenger->flightPath);
	printf("\n");
}



//이 아래로는 기본적인 RBT의 기능 구현을 위한 함수들입니다.

node* isexist(node* root, int key) {
	if (root->key < 0 || root->key == key) return root;
	else if (root->key > key)
		return isexist(root->leftc, key);
	else
		return isexist(root->rightc, key);
}



void initTree() {
	nil = (node*)malloc(sizeof(node));
	nil->parent = nil;
	nil->leftc = nil;
	nil->rightc = nil;
	nil->key = -1;
	nil->color = 0;
}

void Lrotate(node* cur) {
	node* par = cur->rightc;

	cur->rightc = par->leftc;
	if (par->leftc->key >= 0)
		par->leftc->parent = cur;
	par->parent = cur->parent;
	if (cur->parent->key < 0)
		root = par;
	else {
		if (cur == cur->parent->leftc) cur->parent->leftc = par;
		else cur->parent->rightc = par;
	}
	par->leftc = cur;
	cur->parent = par;
}

void Rrotate(node* cur) {
	node* par = cur->leftc;

	cur->leftc = par->rightc;
	if (par->rightc->key >= 0)
		par->rightc->parent = cur;
	par->parent = cur->parent;
	if (cur->parent->key < 0)
		root = par;
	else {
		if (cur == cur->parent->rightc) cur->parent->rightc = par;
		else cur->parent->leftc = par;
	}
	par->rightc = cur;
	cur->parent = par;
}

void insert_fixup(node* cur) {
	node* y = NULL;

	while (cur != root && cur->parent->color == 1) {
		if (cur->parent == cur->parent->parent->leftc) {
			y = cur->parent->parent->rightc;
			if (y->key >= 0 && y->color == 1) {
				cur->parent->color = 0;
				y->color = 0;
				cur->parent->parent->color = 1;
				cur = cur->parent->parent;
			}
			else {
				if (cur == cur->parent->rightc) {
					cur = cur->parent;
					Lrotate(cur);
				}
				cur->parent->color = 0;
				cur->parent->parent->color = 1;
				Rrotate(cur->parent->parent);
			}
		}
		else {
			y = cur->parent->parent->leftc;
			if (y->key >= 0 && y->color == 1) {
				cur->parent->color = 0;
				y->color = 0;
				cur->parent->parent->color = 1;
				cur = cur->parent->parent;

			}
			else {
				if (cur == cur->parent->leftc) {
					cur = cur->parent;
					Rrotate(cur);
				}
				cur->parent->color = 0;
				cur->parent->parent->color = 1;
				Lrotate(cur->parent->parent);
			}
		}
	}

	root->color = 0;
}


node* insert(node* nod) {
	node* par = nil;
	node* cur = root;

	while (cur != NULL && cur->key >= 0) {
		par = cur;
		if (nod->key == cur->key) {
			return NULL;
		}
		if (nod->key < cur->key) {
			cur = cur->leftc;
		}
		else {
			cur = cur->rightc;
		}
	}
	nod->parent = par;
	if (par->key < 0) {
		root = nod;
	}
	else if (nod->key < par->key) {
		par->leftc = nod;
	}
	else par->rightc = nod;

	nod->leftc = nil;
	nod->rightc = nil;
	nod->color = 1;
	insert_fixup(nod);

	while (root->parent->key >= 0)
		root = root->parent;

	return root;
}

node* minnode(node* cur) {
	node* temp = cur;

	while (temp->leftc->key >= 0) {
		temp = temp->leftc;
	}
	return temp;
}

node* successor(node* cur) {
	node* par = cur->parent;

	if (cur->rightc->key >= 0)
		return minnode(cur->rightc);
	while (par->key >= 0 && cur == par->rightc) {
		cur = par;
		par = par->parent;
	}
	return par;
}

void delete_fixup(node* cur) {

	node* temp = NULL;

	while (cur->parent->key > 0 && cur->color == 0) {
		if (cur == cur->parent->leftc) {
			temp = cur->parent->rightc;
			if (temp->color == 1) {
				temp->color = 0;
				cur->parent->color = 1;
				Lrotate(cur->parent);
				temp = cur->parent->rightc;
			}
			if (temp->leftc->color == 0 && temp->rightc->color == 0) {
				temp->color = 1;
				cur = cur->parent;
			}
			else {
				if (temp->rightc->color == 0) {
					temp->leftc->color = 0;
					temp->color = 1;
					Rrotate(temp);
					temp = cur->parent->rightc;
				}
				temp->color = cur->parent->color;
				cur->parent->color = 0;
				temp->rightc->color = 0;
				Lrotate(cur->parent);
				cur = root;
			}
		}
		else {
			temp = cur->parent->leftc;
			if (temp->color == 1) {
				temp->color = 0;
				cur->parent->color = 1;
				Rrotate(cur->parent);
				temp = cur->parent->leftc;
			}
			if (temp->rightc->color == 0 && temp->leftc->color == 0) {
				temp->color = 1;
				cur = cur->parent;
			}
			else {
				if (temp->leftc->color == 0) {
					temp->rightc->color = 0;
					temp->color = 1;
					Lrotate(temp);
					temp = cur->parent->leftc;
				}
				temp->color = cur->parent->color;
				cur->parent->color = 0;
				temp->leftc->color = 0;
				Rrotate(cur->parent);
				cur = root;
			}
		}

	}
	if (cur->key > 0)
		cur->color = 0;
}

node* delet(node* nod) {
	node* par = NULL;
	node* cur = NULL;

	if (nod->leftc->key < 0 || nod->rightc->key < 0) {
		par = nod;
	}
	else par = successor(nod);

	if (par->leftc->key >= 0) {
		cur = par->leftc;
	}
	else cur = par->rightc;
	if (cur->key >= 0)
		cur->parent = par->parent;

	if (par->parent->key < 0) {
		root = cur;
	}
	else {
		if (par == par->parent->leftc) {
			par->parent->leftc = cur;
		}
		else par->parent->rightc = cur;
	}

	if (par != nod) {
		nod->key = par->key;
	}
	if (par->color == 0) {
		delete_fixup(cur);
	}

	return root;
}

void printRBT(node* root, int level) {

	if (root->key < 0) {
		for (int i = 0; i < level; i++)
			putchar('\t');
		puts("~");
	}
	else {
		printRBT(root->rightc, level + 1);
		for (int i = 0; i < level; i++)
			putchar('\t');
		if (root->color) {
			printf("%d[R]\n", root->key);
		}
		else printf("%d[B]\n", root->key);
		printRBT(root->leftc, level + 1);
	}
}

int height(node* root) {
	int h;
	if (root == NULL || root->key < 0)
		return 0;
	else {
		int left = height(root->leftc);
		int right = height(root->rightc);
		h = (left > right ? left : right) + 1;
		return h;
	}
}