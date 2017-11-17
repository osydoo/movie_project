#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#define EMPTY 0

struct director{//포인터 포현이 안되서 그냥 배열로 함 수정 필요, 감독 목록
	int serial_number;//감독 시리얼 번호
	char director[20];//감독 이름
	char sex[3];//성
	char birth[10];//생일//왜 int형은 경고가 뜨는데 char형은 경고아 안뜨는가?
	char title[3][20];//대표작 제목
	struct director *next_director;//다음 항목으로
};
typedef struct director linklist_d;

struct movie{//포인터 포현이 안되서 그냥 배열로 함 수정 필요, 영화 목록
	int serial_number;//영화 시리얼번호
	char title[20];//영화 이름
	char genre[10];//영화 장르
	char director[20];//감독이름
	char year[10];//제작 년도
	char time[10];//총 상영시간
	char actor[3][20];//배우이름
	struct movie *next_movie;//다음 항목으로
};
typedef struct movie linklist_m;

struct actor{//포인터 포현이 안되서 그냥 배열로 함 수정 필요, 배우목록
	int serial_number;//배우 시리얼 번호
	char actor[20];//배우 이름
	char sex[3];//성
	char birth[10];//생일-왜 int형은 경고가 뜨는데 char형은 경고아 안뜨는가?
	char title[3][20];//대표작 제목
	struct actor *next_actor;//다음 항목으로
};
typedef struct actor linklist_a;

linklist_d *get_director(){//동적 할당-감독
	linklist_d *tmp;
	tmp = (linklist_d*)malloc(sizeof(linklist_d));
	tmp -> next_director = EMPTY;
	return tmp;
}

linklist_m *get_movie(){//동적 할당-영화
	linklist_m *tmp;
	tmp = (linklist_m*)malloc(sizeof(linklist_m));
	tmp -> next_movie = EMPTY;
	return tmp;
}

linklist_a *get_actor(){//동적 할당-배우
	linklist_a *tmp;
	tmp = (linklist_a*)malloc(sizeof(linklist_a));
	tmp -> next_actor = EMPTY;
	return tmp;
}
// 배열 표현을 포인터로 먼저 고치고 search 만들자!



void search(linklist_d **first_d, linklist_m **first_m, linklist_a **first_a){
	//linklist_d *first_d;
	linklist_d **tmp_d;
	tmp_d = first_d;
	//linklist_m *first_m;
	linklist_m **tmp_m;
	tmp_m = first_m;
	//linklist_a *first_a;
	linklist_a **tmp_a;//전부다 구조체 불러옴
	tmp_a = first_a;
	char option[3];//확인을 위한 임시적인 변수
	char string[3];

	printf("옵션을 입력하세요 : ");
	scanf("%s", option);
	printf("찾을 항목을 입력하새요 : ");
	scanf("%s", string);

	for(int i=0 ; i<3 ; i++){//option이 dma3개다 들어갈 수 있으므로 3
		if(option[i]=='d'){
			while(1){//모든 항목을 뒤지기 위해서 사용
				if (*first_d == NULL) {//감독 내에서 비교
					printf("director에 저장 된 값이 없습니다");
					break;
				}
				if(strcmp(string, (*first_d)->director) == 0)//감독 이름에잇는가?
					printf("%d:%s:%s:%s:%s, %s, %s\n", (*first_d)->serial_number, (*first_d)->director, (*first_d)->sex, (*first_d)->birth, (*first_d)->title[0], (*first_d)->title[1], (*first_d)->title[2]);
				else if(strcmp(string, (*first_d)->sex) == 0)//성에 잇는가?
					printf("%d:%s:%s:%s:%s, %s, %s\n", (*first_d)->serial_number, (*first_d)->director, (*first_d)->sex, (*first_d)->birth, (*first_d)->title[0], (*first_d)->title[1], (*first_d)->title[2]);
				else if(strcmp(string, (*first_d)->birth) == 0)//생일에 있는가?
					printf("%d:%s:%s:%s:%s, %s, %s\n", (*first_d)->serial_number, (*first_d)->director, (*first_d)->sex, (*first_d)->birth, (*first_d)->title[0], (*first_d)->title[1], (*first_d)->title[2]);
				for(int i = 0; i < 3;i++){//대표작들에 있는가?
					if(strcmp(string, (*first_d)->title[i]) == 0)
						printf("%d:%s:%s:%s:%s, %s, %s\n", (*first_d)->serial_number, (*first_d)->director, (*first_d)->sex, (*first_d)->birth, (*first_d)->title[0], (*first_d)->title[1], (*first_d)->title[2]);
				}
				*first_d = (*first_d)->next_director;//다음 주소 값을 뒤져보자!
			}
			*first_d = *tmp_d;//첫번째 주소값을 보존하기 위한 코드
		}
		else if(option[i] == 'm'){//영화 내에서 비교
			while(1){//모든 항목을 뒤지기 위해서 사용
				if (*first_m == 0) {
					printf("movie에 저장 된 값이 없습니다");
					break;
				}
				if(strcmp(string, (*first_m)->title) == 0)//영화 제목에 있는가?
					printf("%d:%s:%s:%s:%s, %s, %s\n", (*first_m)->serial_number, (*first_m)->title, (*first_m)->genre, (*first_m)->director, (*first_m)->actor[0], (*first_m)->actor[1], (*first_m)->actor[2]);
				else if(strcmp(string, (*first_m)->genre) == 0)//장르에 있는가?
					printf("%d:%s:%s:%s:%s, %s, %s\n", (*first_m)->serial_number, (*first_m)->title, (*first_m)->genre, (*first_m)->director, (*first_m)->actor[0], (*first_m)->actor[1], (*first_m)->actor[2]);
				else if(strcmp(string, (*first_m)->director) == 0)//감독이름에 있는가?
					printf("%d:%s:%s:%s:%s, %s, %s\n", (*first_m)->serial_number, (*first_m)->title, (*first_m)->genre, (*first_m)->director, (*first_m)->actor[0], (*first_m)->actor[1], (*first_m)->actor[2]);
				for(int i = 0; i < 3;i++){//배우 이름에 있는가?
					if(strcmp(string, (*first_m)->actor[i]) == 0)
						printf("%d:%s:%s:%s:%s, %s, %s\n", (*first_m)->serial_number, (*first_m)->title, (*first_m)->genre, (*first_m)->director, (*first_m)->actor[0], (*first_m)->actor[1], (*first_m)->actor[2]);
				}
				*first_m = (*first_m)->next_movie;//다음값으로 넘어가자
			}
			*first_m = *tmp_m;//첫번째 주소값을 보존하기 위한 코드
		}
		else if(option[i] == 'a'){
			while(1){//모든 항목을 뒤지기 위해서 사용
				if (*first_a == 0) {
					printf("actor에 저장 된 값이 없습니다");
					break;
				}
				if(strcmp(string, (*first_a)->actor) == 0)//영화 제목에 있는가?
					printf("%d:%s:%s:%s:%s, %s, %s\n", (*first_a)->serial_number, (*first_a)->actor, (*first_a)->sex, (*first_a)->birth, (*first_a)->title[0], (*first_a)->title[1], (*first_a)->title[2]);
				else if(strcmp(string, (*first_a)->sex) == 0)//성에 있는가?
					printf("%d:%s:%s:%s:%s, %s, %s\n", (*first_a)->serial_number, (*first_a)->actor, (*first_a)->sex, (*first_a)->birth, (*first_a)->title[0], (*first_a)->title[1], (*first_a)->title[2]);
				else if(strcmp(string, (*first_a)->birth) == 0)//생일에 있는가?
					printf("%d:%s:%s:%s:%s, %s, %s\n", (*first_a)->serial_number, (*first_a)->actor, (*first_a)->sex, (*first_a)->birth, (*first_a)->title[0], (*first_a)->title[1], (*first_a)->title[2]);
				for(int i = 0; i < 3;i++){
					if(strcmp(string, (*first_a)->title[i]) == 0)//대표작이름에 있는가?
						printf("%d:%s:%s:%s:%s, %s, %s\n", (*first_a)->serial_number, (*first_a)->actor, (*first_a)->sex, (*first_a)->birth, (*first_a)->title[0], (*first_a)->title[1], (*first_a)->title[2]);
				}
				*first_a = (*first_a)->next_actor;//다음값으로 넘어가자
			}
			*first_a = *tmp_a;//초기값을 보존하자
		}
	}
}

int add_director(linklist_d **first_d){//감독 입력하기
	/*linklist_d *tmp;
	tmp = *first_d;*/// 다시보니 왜 들어가 있는지 모르겠어서 빼봄 -> 정상실행 -> 모든add항목에서 뺌 혹시몰라서 주석 처리
	char director[20];//감독 이름
	char sex[3];//성
	char birth[10];//생일
	char title[3][20];//제목
	char stop;//중단 확인

FILE *fp;//파일 입출력

fp = fopen("director_log.txt", "at");
if (fp == NULL){
	printf("실패 - 종료\n");
	return 1;
}
	if(*first_d==NULL){ //처음입력할 때 저장, 입출력은 데이터를 저장한후 따로 할 예정
		*first_d = get_director();
		printf("director > ");//감독 입력
		scanf("%s", (*first_d)->director);
		getchar();
		printf("sex > ");//성별입력
		scanf("%s", (*first_d)->sex);
		getchar();
		printf("birth > ");//생일입력
		scanf("%s", (*first_d)->birth);
		getchar();
		for(int i = 0; i < 3 ; i++){//대표작 입력
			printf("title%d > ", i+1);
			scanf("%s", (*first_d)->title[i]);
			getchar();
		}
		fprintf(fp, "%s:%d:%s:%s:%s:%s, %s, %s\n", "add", (*first_d)->serial_number, (*first_d)->director, (*first_d)->sex, (*first_d)->birth, (*first_d)->title[0], (*first_d)->title[1], (*first_d)->title[2]);
		//director_log에 입력하기 - 만약 대표작이 3건이 안된다면 어쩔지에 대한것이 처리가 안되었음!
		fclose(fp);//director_log닫기

		fp = fopen("director_list.txt", "at");
		if (fp == NULL){
			printf("실패 - 종료\n");
			return 1;
		}
		fprintf(fp, "%d:%s:%s:%s:%s,%s,%s\n", (*first_d)->serial_number, (*first_d)->director, (*first_d)->sex, (*first_d)->birth, (*first_d)->title[0], (*first_d)->title[1], (*first_d)->title[2]);
		fclose(fp);//movie_list닫기
	}
	printf("입력을 계속 하시겠습니까? y or n\n");
	scanf("%c", &stop);
	getchar();
	if(stop == 'n'){

		return 0;
	}
	else{
		add_director(&(*first_d)->next_director);
	}
}

int add_movie(linklist_m **first_m){//영화 입력하기
	/*linklist_m *tmp;
	tmp = *first_m;*/
	char title[20];//영화 이름
	char genre[10];//영화 장르
	char director[20];//감독이름
	char year[10];//제작 년도
	char time[10];//총 상영시간
	char actor[3][20];//배우이름
	char stop;//중단 확인

FILE *fp;//파일 입출력

fp = fopen("movie_log.txt", "at");
if (fp == NULL){
	printf("실패 - 종료\n");
	return 1;
}
	if(*first_m == NULL){ //처음입력할 때 저장, 입출력은 데이터를 저장한후 따로 할 예정
		*first_m = get_movie();
		printf("title > ");//감독 입력
		scanf("%s", (*first_m)->title);
		getchar();
		printf("genre > ");//성별입력
		scanf("%s", (*first_m)->genre);
		getchar();
		printf("director > ");//생일입력
		scanf("%s", (*first_m)->director);
		getchar();
		for(int i = 0; i < 3 ; i++){//대표작 입력
			printf("actor%d > ", i+1);
			scanf("%s", (*first_m)->actor[i]);
			getchar();
		}
		fprintf(fp, "%s:%d:%s:%s:%s:%s, %s, %s\n", "add", (*first_m)->serial_number, (*first_m)->title, (*first_m)->genre, (*first_m)->director, (*first_m)->actor[0], (*first_m)->actor[1], (*first_m)->actor[2]);
		//director_log에 입력하기 - 만약 대표작이 3건이 안된다면 처리가안되었음
		fclose(fp);//movie_log닫기

		fp = fopen("movie_list.txt", "at");
		if (fp == NULL){
			printf("실패 - 종료\n");
			return 1;
		}
		fprintf(fp, "%d:%s:%s:%s:%s, %s, %s\n", (*first_m)->serial_number, (*first_m)->title, (*first_m)->genre, (*first_m)->director, (*first_m)->actor[0], (*first_m)->actor[1], (*first_m)->actor[2]);
		fclose(fp);//movie_list닫기
	}
	printf("입력을 계속 하시겠습니까? y or n\n");
	scanf("%c", &stop);
	getchar();
	if(stop == 'n'){

		return 0;
	}
	else{
		add_movie(&(*first_m)->next_movie);
	}
}

int add_actor(linklist_a **first_a){//배우 입력하기
	/*linklist_a *tmp;
	tmp = *first_a;*/
	char actor[20];//감독 이름
	char sex[3];//성
	char birth[10];//생일
	char title[3][20];//제목
	char stop;//중단 확인

FILE *fp;//파일 입출력

fp = fopen("actor_log.txt", "at");
if (fp == NULL){
	printf("실패 - 종료\n");
	return 1;
}

	if(*first_a==NULL){ //처음입력할 때 저장, 입출력은 데이터를 저장한후 따로 할 예정
		*first_a = get_actor();
		printf("actor > ");//배우 입력
		scanf("%s", (*first_a)->actor);
		getchar();
		printf("sex > ");//성별입력
		scanf("%s", (*first_a)->sex);
		getchar();
		printf("birth > ");//생일입력
		scanf("%s", (*first_a)->birth);
		getchar();
		for(int i = 0; i < 3 ; i++){//대표작 입력
			printf("title%d > ", i+1);
			scanf("%s", (*first_a)->title[i]);
			getchar();
		}
		fprintf(fp, "%s:%d:%s:%s:%s:%s, %s, %s\n", "add", (*first_a)->serial_number, (*first_a)->actor, (*first_a)->sex, (*first_a)->birth, (*first_a)->title[0], (*first_a)->title[1], (*first_a)->title[2]);
		//actor_log에 입력하기 - 만약 대표작이 3건이 안된다면 어쩔지에 대한것이 처리가 안되었음!
		fclose(fp);//actor_log닫기

		fp = fopen("actor_list.txt", "at");
		if (fp == NULL){
			printf("실패 - 종료\n");
			return 1;
		}
		fprintf(fp, "%d:%s:%s:%s:%s, %s, %s\n", (*first_a)->serial_number, (*first_a)->actor, (*first_a)->sex, (*first_a)->birth, (*first_a)->title[0], (*first_a)->title[1], (*first_a)->title[2]);
		fclose(fp);//actor_list닫기
	}
	printf("입력을 계속 하시겠습니까? y or n\n");
	scanf("%c", &stop);
	getchar();
	if(stop == 'n'){

		return 0;
	}
	else{
		add_actor(&(*first_a)->next_actor);
	}
}

int main(){
	int i = 0;
	linklist_d *first_d = EMPTY;
	linklist_m *first_m = EMPTY;
	linklist_a *first_a = EMPTY;
	//char *option;
	//char *string;
	//*option = "m";
	//*string = "1";

	while(1){
		system("clear");
		printf("1.찾기 2.감독입력 3.영화입력 4.배우입력 5.종료 - ");
		scanf("%d", &i);
		system("clear");
		switch(i){
			case 1:
				search(&first_d, &first_m, &first_a);
				break;
			case 2:
				add_director(&first_d);
				break;
			case 3:
				add_movie(&first_m);
				break;
			case 4:
				add_actor(&first_a);
				break;
			case 5:
				printf("종료합니다");
				break;
		}
		if(i == 5){
				break;
		}
	}
	return 0;
}
