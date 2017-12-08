#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

typedef struct sorting { //char�� sort�� ���� �ӽñ���ü
	int serial_number;
	char *string;
} sorting;

typedef struct sorting_num{ //int�� sort�� ���� �ӽñ���ü
	int serial_number;
	int num;
} sorting_num;

typedef struct movie {
	int serial_number;
	char *title;
	char *genre;
	char *director;
	char *year;
	char *time;
	char *actors;
	struct director *d_link;
	struct actor *a_link;
	struct movie *next;
}movie;

typedef struct director {
	int serial_number;
	char *name;
	char *sex;
	char *birth;
	char *best_movies;
	struct movie *m_link;
	struct director *next;
}director;

typedef struct actor {
	int serial_number;
	char *name;
	char *sex;
	char *birth;
	char *best_movies;
	struct movie *m_link;
	struct actor *next;
}actor;

movie *root_movie = NULL, *m;	//movie*�� ����� �ٸ���
director *root_director = NULL, *d;	//director*�� ����� �ٸ���
actor *root_actor = NULL, *a;	//actor*�� ����� �ٸ���
int root_m_num = 0, root_d_num = 0, root_a_num = 0;	//������� �ƴ��� �Ǻ��� ����
int serial_m_num = 1, serial_d_num = 1, serial_a_num = 1;	//������ �ø��� �ѹ� ��������
int ctrl_c_num = 0;	//ctrl+c �޾����� ���� �Ÿ� �ٽ� ����ϰ� ���ִ� ��������(���� ���ȵ�)

char *colon_proc(char *);	//�ݷ� ó�� �Լ� ����
char *anti_colon_proc(char *);	//??;ó�� �Լ� ����
char *itoa(int value, char *str, int radix);	//itoa�Լ��� ��� ����
char *mystrcat(char *, char *);	//strcat�� ������ ���� mystrcat �Լ� ��� ����

char *mystrcat(char *dest, char *src) {
	while (*dest) dest++;
	while (*dest++ = *src++);
	return --dest;
}

void load_movie() {	//movie_log�� �о m ��ũ�� ����Ʈ�� ����� ���� �Լ�(�̿ϼ�)
	char *token, *line, *menu;	//��ū�� ������ �����ϴ� �Ͱ� �޴�(add, delete, update)�� ������ ������
	char *title, *genre, *director, *year, *m_time, *actors;	//movie�� ���� �ɰ����� ����� ������ ������
	char *full_file_name, *cp_file_name, *nt_year, *nt_mon, *nt_mday, *nt_hour, *nt_min;	//�ð����� ���ڿ��� ��ȯ�ؼ� ������ �����Ϳ� �� ��ĥ �κ�
	int serial_num;
	char ch;
	movie *m_load, *m_temp;
	time_t t = time(NULL);	//Ÿ�� ����ü ����
	struct tm tm = *localtime(&t);
	m_load = (movie *)malloc(sizeof(movie));

	full_file_name = (char *)malloc(sizeof(char) * 25);
	cp_file_name = (char *)malloc(sizeof(char) * 15);
	nt_year = (char *)malloc(sizeof(char) * 5);
	nt_mon = (char *)malloc(sizeof(char) * 3);
	nt_mday = (char *)malloc(sizeof(char) * 3);
	nt_hour = (char *)malloc(sizeof(char) * 3);
	nt_min = (char *)malloc(sizeof(char) * 3);

	sprintf(nt_year, "%d", tm.tm_year + 1900);
	printf("year : %s\n", nt_year);
	strcpy(cp_file_name, nt_year);
	sprintf(nt_mon, "%d", tm.tm_mon + 1);
	printf("month : %s\n", nt_mon);
	sprintf(nt_mday, "%d", tm.tm_mday);
	printf("day : %s\n", nt_mday);
	sprintf(nt_hour, "%d", tm.tm_hour);
	printf("hour : %s\n", nt_hour);
	sprintf(nt_min, "%d", tm.tm_min);
	printf("minute : %s\n", nt_min);

	token = (char *)malloc(sizeof(char) * 50);
	line = (char *)malloc(sizeof(char) * 200);

	FILE *fp, *fp_copy;	//movie_log�� ���ѰͰ� movie_list�� �ð��ٿ��� ������ ���� ������
	fp = fopen("movie_log", "r");
	if (fp == NULL)
		return;

	while (fgets(line, 200, fp) != NULL) {	//log������ ���� ���� ������ ����
		*(line + strlen(line) - 1) = 0;	//�������� �о�� \n�� ������
		printf("%s\n", line);

		token = strtok(line, ":");
		printf("token : %s\n", token);
		menu = (char *)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(menu, token);

		token = strtok(NULL, ":");
		printf("serial_num : %s\n", token);
		serial_num = atoi(token);	//���ڿ��� token�� ������ ��ȯ

		if(!strcmp(menu, "add"))
			serial_m_num = serial_num + 1;	//���߿� add m�� �� �� �ùٸ� �ø��� �ѹ��� ������


		if (strcmp(menu, "delete")) {
			token = strtok(NULL, ":");
			token = anti_colon_proc(token);	//??;�� :�� �ٲ���
			printf("title : %s\n", token);
			title = (char *)malloc(sizeof(char) * strlen(token) + 1);
			strcpy(title, token);

			token = strtok(NULL, ":");
			token = anti_colon_proc(token);
			printf("genre : %s\n", token);
			genre = (char *)malloc(sizeof(char) * strlen(token) + 1);
			strcpy(genre, token);

			token = strtok(NULL, ":");
			token = anti_colon_proc(token);
			printf("director : %s\n", token);
			director = (char *)malloc(sizeof(char) * strlen(token) + 1);
			strcpy(director, token);

			token = strtok(NULL, ":");
			token = anti_colon_proc(token);
			printf("year : %s\n", token);
			year = (char *)malloc(sizeof(char) * strlen(token) + 1);
			strcpy(year, token);

			token = strtok(NULL, ":");
			token = anti_colon_proc(token);
			printf("time : %s\n", token);
			m_time = (char *)malloc(sizeof(char) * strlen(token) + 1);
			strcpy(m_time, token);

			token = strtok(NULL, ":");
			token = anti_colon_proc(token);
			/*if (*token == '=') {
				//printf("= is comparamised\n");
				*(token + strlen(token) - 1) = 0;	//�������� �о�� �� �ǵ�(form feed?)�� ������
			}*/
			printf("actors : %s\n", token);
			actors = (char *)malloc(sizeof(char) * strlen(token) + 1);
			strcpy(actors, token);
		}


		if (!strcmp(menu, "add")) {	//tag�� add�̸� �����ϴ� ��
			if (root_m_num == 0) {	//��ũ�� ����Ʈ�� ó�� ����� ����
				root_movie = m_load;
				root_m_num = 1;
			}
			m_load->serial_number = serial_num;
			m_load->title = (char *)malloc(sizeof(char) * strlen(title) + 1);
			strcpy(m_load->title, title);
			m_load->genre = (char *)malloc(sizeof(char) * strlen(genre) + 1);
			strcpy(m_load->genre, genre);
			m_load->director = (char *)malloc(sizeof(char) * strlen(director) + 1);
			strcpy(m_load->director, director);
			m_load->year = (char *)malloc(sizeof(char) * strlen(year) + 1);
			strcpy(m_load->year, year);
			m_load->time = (char *)malloc(sizeof(char) * strlen(m_time) + 1);
			strcpy(m_load->time, m_time);
			m_load->actors = (char *)malloc(sizeof(char) * strlen(actors) + 1);
			strcpy(m_load->actors, actors);

			m_load->next = (movie *)malloc(sizeof(movie));	//m_load�� next�����͸� �����Ҵ�
			m_load = m_load->next;	//m_load�� ���� m_load�� next�� �ٲ�
			m_temp = m_load;	//��ũ�� ����Ʈ�� �������� m_temp���ٰ� ����
			m_load->next = NULL;	//������ m_load�� next�� null�� ����
		}
		else if (!strcmp(menu, "update")) {	//tag�� update�̸� �����ϴ� ��
			m_load = root_movie;
			while (1) {
				if (m_load->serial_number == serial_num) {	//update�ϰ��� �ϴ� �ø��� �ѹ��� ��ġ�ϸ� ����
					if (strcmp(title, "="))
						strcpy(m_load->title, title);
					if (strcmp(genre, "="))
						strcpy(m_load->genre, genre);
					if (strcmp(director, "="))
						strcpy(m_load->director, director );
					if (strcmp(year, "="))
						strcpy(m_load->year, year);
					if (strcmp(m_time, "="))
						strcpy(m_load->time, m_time);
					if (strcmp(actors, "="))
						strcpy(m_load->actors, actors);

					break;
				}
				if (m_load->next == NULL)
					break;
				m_load = m_load->next;
			}
			m_load = m_temp;
		}
		else if (!strcmp(menu, "delete")) {	//tag�� delete�̸� �����ϴ� ��
			m_load = root_movie;
			while (1) {
				if (m_load->serial_number == serial_num) {	//delete�ϰ��� �ϴ� �ø��� �ѹ��� ��ġ�ϸ� ����
					m_load->title = NULL;
					m_load->genre = NULL;
					m_load->director = NULL;
					m_load->year = NULL;
					m_load->time = NULL;
					m_load->actors = NULL;

					break;
				}
				m_load = m_load->next;
			}
			if (m_load->next == NULL)
				break;
			m_load = m_temp;
		}
		printf("\n");
	}
	m = m_load;

	strcat(cp_file_name, nt_mon);	//movie_list������ ���� ������ �� Ȯ���ڸ� ��¥�� �ϰ� �ϴ� �κ�
	if (atoi(nt_mday) < 10) {
		mystrcat(cp_file_name, "0");
		mystrcat(cp_file_name, nt_mday);
	}
	else {
		mystrcat(cp_file_name, nt_mday);
	}

	if (atoi(nt_hour) < 10) {
		mystrcat(cp_file_name, "0");
		mystrcat(cp_file_name, nt_hour);
	}
	else {
		mystrcat(cp_file_name, nt_hour);
	}

	if (atoi(nt_min) < 10) {
		mystrcat(cp_file_name, "0");
		mystrcat(cp_file_name, nt_min);
	}
	else {
		mystrcat(cp_file_name, nt_min);
	}
	printf("cp_file_name : %s\n", cp_file_name);
	strcpy(full_file_name, "movie_list.");
	mystrcat(full_file_name, cp_file_name);
	printf("full_file_name : %s\n", full_file_name);

	fp_copy = fopen(full_file_name, "wt");
	rewind(fp);	//movie_list�� �� ������ ���� �����͸� �ű�
	while ((ch = fgetc(fp)) != EOF)
		fputc(ch, fp_copy);

	fclose(fp);
	fclose(fp_copy);
	printf("\n");
}

void load_director() {	//director_log�� �о d ��ũ�� ����Ʈ�� ����� ���� �Լ�
	char *token, *line, *menu;	//��ū�� ������ �����ϴ� �Ͱ� �޴�(add, delete, update)�� ������ ������
	char *name, *sex, *birth, *best_movies;	//director�� ���� �ɰ����� ����� ������ ������
	char *full_file_name, *cp_file_name, *nt_year, *nt_mon, *nt_mday, *nt_hour, *nt_min;	//�ð����� ���ڿ��� ��ȯ�ؼ� ������ �����Ϳ� �� ��ĥ �κ�
	int serial_num;
	char ch;
	director *d_load, *d_temp;
	time_t t = time(NULL);	//Ÿ�� ����ü ����
	struct tm tm = *localtime(&t);
	d_load = (director *)malloc(sizeof(director));

	full_file_name = (char *)malloc(sizeof(char) * 25);
	cp_file_name = (char *)malloc(sizeof(char) * 15);
	nt_year = (char *)malloc(sizeof(char) * 5);
	nt_mon = (char *)malloc(sizeof(char) * 3);
	nt_mday = (char *)malloc(sizeof(char) * 3);
	nt_hour = (char *)malloc(sizeof(char) * 3);
	nt_min = (char *)malloc(sizeof(char) * 3);

	sprintf(nt_year, "%d", tm.tm_year + 1900);
	//printf("year : %s\n", nt_year);
	strcpy(cp_file_name, nt_year);
	sprintf(nt_mon, "%d", tm.tm_mon + 1);
	//printf("month : %s\n", nt_mon);
	sprintf(nt_mday, "%d", tm.tm_mday);
	//printf("day : %s\n", nt_mday);
	sprintf(nt_hour, "%d", tm.tm_hour);
	//printf("hour : %s\n", nt_hour);
	sprintf(nt_min, "%d", tm.tm_min);
	//printf("minute : %s\n", nt_min);

	token = (char *)malloc(sizeof(char) * 50);
	line = (char *)malloc(sizeof(char) * 200);

	FILE *fp, *fp_copy;	//director_log�� ���ѰͰ� director_list�� �ð��ٿ��� ������ ���� ������
	fp = fopen("director_log", "r");
	if (fp == NULL)
		return;

	while (fgets(line, 200, fp) != NULL) {	//log������ ���� ���� ������ ����
		*(line + strlen(line) - 1) = 0;	//�������� �о�� \n�� ������
		//printf("%s\n", line);

		token = strtok(line, ":");
		//printf("token : %s\n", token);
		menu = (char *)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(menu, token);

		token = strtok(NULL, ":");
		//printf("serial_num : %s\n", token);
		serial_num = atoi(token);	//���ڿ��� token�� ������ ��ȯ

		if (!strcmp(menu, "add"))
			serial_d_num = serial_num + 1;	//���߿� add d�� �� �� �ùٸ� �ø��� �ѹ��� ������


		if (strcmp(menu, "delete")) {
			token = strtok(NULL, ":");
			token = anti_colon_proc(token);	//??;�� :�� �ٲ���
			//printf("name : %s\n", token);
			name = (char *)malloc(sizeof(char) * strlen(token) + 1);
			strcpy(name, token);

			token = strtok(NULL, ":");
			token = anti_colon_proc(token);
			//printf("sex : %s\n", token);
			sex = (char *)malloc(sizeof(char) * strlen(token) + 1);
			strcpy(sex, token);

			token = strtok(NULL, ":");
			token = anti_colon_proc(token);
			//printf("birth : %s\n", token);
			birth = (char *)malloc(sizeof(char) * strlen(token) + 1);
			strcpy(birth, token);

			token = strtok(NULL, ":");
			token = anti_colon_proc(token);
			/*if (*token == '=') {
				//printf("= is comparamised\n");
				*(token + strlen(token) - 1) = 0;	//�������� �о�� �� �ǵ�(form feed?)�� ������
			}
			//printf("best_movies : %s\n", token);*/
			best_movies = (char *)malloc(sizeof(char) * strlen(token) + 1);
			strcpy(best_movies, token);
		}


		if (!strcmp(menu, "add")) {	//tag�� add�̸� �����ϴ� ��
			if (root_d_num == 0) {	//��ũ�� ����Ʈ�� ó�� ����� ����
				root_director = d_load;
				root_d_num = 1;
			}
			d_load->serial_number = serial_num;
			d_load->name = (char *)malloc(sizeof(char) * strlen(name) + 1);
			strcpy(d_load->name, name);
			d_load->sex = (char *)malloc(sizeof(char) * strlen(sex) + 1);
			strcpy(d_load->sex, sex);
			d_load->birth = (char *)malloc(sizeof(char) * strlen(birth) + 1);
			strcpy(d_load->birth, birth);
			d_load->best_movies = (char *)malloc(sizeof(char) * strlen(best_movies) + 1);
			strcpy(d_load->best_movies, best_movies);

			d_load->next = (director *)malloc(sizeof(director));	//d_load�� next�����͸� �����Ҵ�
			d_load = d_load->next;	//d_load�� ���� d_load�� next�� �ٲ�
			d_temp = d_load;	//��ũ�� ����Ʈ�� �������� d_temp���ٰ� ����
			d_load->next = NULL;	//������ d_load�� next�� null�� ����
		}
		else if (!strcmp(menu, "update")) {	//tag�� update�̸� �����ϴ� ��
			d_load = root_director;
			while (1) {
				if (d_load->serial_number == serial_num) {	//update�ϰ��� �ϴ� �ø��� �ѹ��� ��ġ�ϸ� ����
					if (strcmp(name, "="))
						strcpy(d_load->name, name);
					if (strcmp(sex, "="))
						strcpy(d_load->sex, sex);
					if (strcmp(birth, "="))
						strcpy(d_load->birth, birth);
					if (strcmp(best_movies, "="))
						strcpy(d_load->best_movies, best_movies);

					break;
				}
				if (d_load->next == NULL)
					break;
				d_load = d_load->next;
			}
			d_load = d_temp;
		}
		else if (!strcmp(menu, "delete")) {	//tag�� delete�̸� �����ϴ� ��
			d_load = root_director;
			while (1) {
				if (d_load->serial_number == serial_num) {	//delete�ϰ��� �ϴ� �ø��� �ѹ��� ��ġ�ϸ� ����
					d_load->name = NULL;
					d_load->sex = NULL;
					d_load->birth = NULL;
					d_load->best_movies = NULL;

					break;
				}
				d_load = d_load->next;
			}
			if (d_load->next == NULL)
				break;
			d_load = d_temp;
		}
		printf("\n");
	}
	d = d_load;

	strcat(cp_file_name, nt_mon);	//movie_list������ ���� ������ �� Ȯ���ڸ� ��¥�� �ϰ� �ϴ� �κ�
	if (atoi(nt_mday) < 10) {
		mystrcat(cp_file_name, "0");
		mystrcat(cp_file_name, nt_mday);
	}
	else {
		mystrcat(cp_file_name, nt_mday);
	}

	if (atoi(nt_hour) < 10) {
		mystrcat(cp_file_name, "0");
		mystrcat(cp_file_name, nt_hour);
	}
	else {
		mystrcat(cp_file_name, nt_hour);
	}

	if (atoi(nt_min) < 10) {
		mystrcat(cp_file_name, "0");
		mystrcat(cp_file_name, nt_min);
	}
	else {
		mystrcat(cp_file_name, nt_min);
	}
	//printf("cp_file_name : %s\n", cp_file_name);
	strcpy(full_file_name, "director_list.");
	mystrcat(full_file_name, cp_file_name);
	//printf("full_file_name : %s\n", full_file_name);

	fp_copy = fopen(full_file_name, "wt");
	rewind(fp);	//movie_list�� �� ������ ���� �����͸� �ű�
	while ((ch = fgetc(fp)) != EOF)
		fputc(ch, fp_copy);

	fclose(fp);
	fclose(fp_copy);
	printf("\n");
}

void load_actor() {	//actor_log�� �о a ��ũ�� ����Ʈ�� ����� ���� �Լ�
	char *token, *line, *menu;	//��ū�� ������ �����ϴ� �Ͱ� �޴�(add, delete, update)�� ������ ������
	char *name, *sex, *birth, *best_movies;	//actor�� ���� �ɰ����� ����� ������ ������
	char *full_file_name, *cp_file_name, *nt_year, *nt_mon, *nt_mday, *nt_hour, *nt_min;	//�ð����� ���ڿ��� ��ȯ�ؼ� ������ �����Ϳ� �� ��ĥ �κ�
	int serial_num;
	char ch;
	actor *a_load, *a_temp;
	time_t t = time(NULL);	//Ÿ�� ����ü ����
	struct tm tm = *localtime(&t);
	a_load = (actor *)malloc(sizeof(actor));

	full_file_name = (char *)malloc(sizeof(char) * 25);
	cp_file_name = (char *)malloc(sizeof(char) * 15);
	nt_year = (char *)malloc(sizeof(char) * 5);
	nt_mon = (char *)malloc(sizeof(char) * 3);
	nt_mday = (char *)malloc(sizeof(char) * 3);
	nt_hour = (char *)malloc(sizeof(char) * 3);
	nt_min = (char *)malloc(sizeof(char) * 3);

	sprintf(nt_year, "%d", tm.tm_year + 1900);
	//printf("year : %s\n", nt_year);
	strcpy(cp_file_name, nt_year);
	sprintf(nt_mon, "%d", tm.tm_mon + 1);
	//printf("month : %s\n", nt_mon);
	sprintf(nt_mday, "%d", tm.tm_mday);
	//printf("day : %s\n", nt_mday);
	sprintf(nt_hour, "%d", tm.tm_hour);
	//printf("hour : %s\n", nt_hour);
	sprintf(nt_min, "%d", tm.tm_min);
	//printf("minute : %s\n", nt_min);

	token = (char *)malloc(sizeof(char) * 50);
	line = (char *)malloc(sizeof(char) * 200);

	FILE *fp, *fp_copy;	//actor_log�� ���ѰͰ� actor_list�� �ð��ٿ��� ������ ���� ������
	fp = fopen("actor_log", "r");
	if (fp == NULL)
		return;

	while (fgets(line, 200, fp) != NULL) {	//log������ ���� ���� ������ ����
		*(line + strlen(line) - 1) = 0;	//�������� �о�� \n�� ������
		//printf("%s\n", line);

		token = strtok(line, ":");
		//printf("token : %s\n", token);
		menu = (char *)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(menu, token);

		token = strtok(NULL, ":");
		//printf("serial_num : %s\n", token);
		serial_num = atoi(token);	//���ڿ��� token�� ������ ��ȯ

		if (!strcmp(menu, "add"))
			serial_a_num = serial_num + 1;	//���߿� add a�� �� �� �ùٸ� �ø��� �ѹ��� ������


		if (strcmp(menu, "delete")) {
			token = strtok(NULL, ":");
			token = anti_colon_proc(token);	//??;�� :�� �ٲ���
			//printf("name : %s\n", token);
			name = (char *)malloc(sizeof(char) * strlen(token) + 1);
			strcpy(name, token);

			token = strtok(NULL, ":");
			token = anti_colon_proc(token);
			//printf("sex : %s\n", token);
			sex = (char *)malloc(sizeof(char) * strlen(token) + 1);
			strcpy(sex, token);

			token = strtok(NULL, ":");
			token = anti_colon_proc(token);
			//printf("birth : %s\n", token);
			birth = (char *)malloc(sizeof(char) * strlen(token) + 1);
			strcpy(birth, token);

			token = strtok(NULL, ":");
			token = anti_colon_proc(token);
			/*if (*token == '=') {
				//printf("= is comparamised\n");
				*(token + strlen(token) - 1) = 0;	//�������� �о�� �� �ǵ�(form feed?)�� ������
			}*/
			//printf("best_movies : %s\n", token);
			best_movies = (char *)malloc(sizeof(char) * strlen(token) + 1);
			strcpy(best_movies, token);
		}


		if (!strcmp(menu, "add")) {	//tag�� add�̸� �����ϴ� ��
			if (root_a_num == 0) {	//��ũ�� ����Ʈ�� ó�� ����� ����
				root_actor = a_load;
				root_a_num = 1;
			}
			a_load->serial_number = serial_num;
			a_load->name = (char *)malloc(sizeof(char) * strlen(name) + 1);
			strcpy(a_load->name, name);
			a_load->sex = (char *)malloc(sizeof(char) * strlen(sex) + 1);
			strcpy(a_load->sex, sex);
			a_load->birth = (char *)malloc(sizeof(char) * strlen(birth) + 1);
			strcpy(a_load->birth, birth);
			a_load->best_movies = (char *)malloc(sizeof(char) * strlen(best_movies) + 1);
			strcpy(a_load->best_movies, best_movies);

			a_load->next = (actor *)malloc(sizeof(actor));	//a_load�� next�����͸� �����Ҵ�
			a_load = a_load->next;	//a_load�� ���� a_load�� next�� �ٲ�
			a_temp = a_load;	//��ũ�� ����Ʈ�� �������� a_temp���ٰ� ����
			a_load->next = NULL;	//������ a_load�� next�� null�� ����
		}
		else if (!strcmp(menu, "update")) {	//tag�� update�̸� �����ϴ� ��
			a_load = root_actor;
			while (1) {
				if (a_load->serial_number == serial_num) {	//update�ϰ��� �ϴ� �ø��� �ѹ��� ��ġ�ϸ� ����
					if (strcmp(name, "="))
						strcpy(a_load->name, name);
					if (strcmp(sex, "="))
						strcpy(a_load->sex, sex);
					if (strcmp(birth, "="))
						strcpy(a_load->birth, birth);
					if (strcmp(best_movies, "="))
						strcpy(a_load->best_movies, best_movies);

					break;
				}
				if (a_load->next == NULL)
					break;
				a_load = a_load->next;
			}
			a_load = a_temp;
		}
		else if (!strcmp(menu, "delete")) {	//tag�� delete�̸� �����ϴ� ��
			a_load = root_actor;
			while (1) {
				if (a_load->serial_number == serial_num) {	//delete�ϰ��� �ϴ� �ø��� �ѹ��� ��ġ�ϸ� ����
					a_load->name = NULL;
					a_load->sex = NULL;
					a_load->birth = NULL;
					a_load->best_movies = NULL;

					break;
				}
				a_load = a_load->next;
			}
			if (a_load->next == NULL)
				break;
			a_load = a_temp;
		}
		printf("\n");
	}
	a = a_load;

	strcat(cp_file_name, nt_mon);	//movie_list������ ���� ������ �� Ȯ���ڸ� ��¥�� �ϰ� �ϴ� �κ�
	if (atoi(nt_mday) < 10) {
		mystrcat(cp_file_name, "0");
		mystrcat(cp_file_name, nt_mday);
	}
	else {
		mystrcat(cp_file_name, nt_mday);
	}

	if (atoi(nt_hour) < 10) {
		mystrcat(cp_file_name, "0");
		mystrcat(cp_file_name, nt_hour);
	}
	else {
		mystrcat(cp_file_name, nt_hour);
	}

	if (atoi(nt_min) < 10) {
		mystrcat(cp_file_name, "0");
		mystrcat(cp_file_name, nt_min);
	}
	else {
		mystrcat(cp_file_name, nt_min);
	}
	//printf("cp_file_name : %s\n", cp_file_name);
	strcpy(full_file_name, "actor_list.");
	mystrcat(full_file_name, cp_file_name);
	//printf("full_file_name : %s\n", full_file_name);

	fp_copy = fopen(full_file_name, "wt");
	rewind(fp);	//movie_list�� �� ������ ���� �����͸� �ű�
	while ((ch = fgetc(fp)) != EOF)
		fputc(ch, fp_copy);

	fclose(fp);
	fclose(fp_copy);
	printf("\n");
}

void link_director(){
	char *director_name;
	movie *m_p;
	director *d_p;
	m_p = root_movie;
	int i = 1;

	if(m_p == NULL)
		return;

	while(1){
		m_p->d_link = NULL;	//�ϴ� NULL�� ������༭ ã�Ƽ� ������ ��� NULL�̰� ��
		if(m_p->title == NULL){
			if(m_p->next == NULL)
				break;
			m_p = m_p->next;
			continue;
		}
		director_name = (char *)malloc(sizeof(char) * strlen(m_p->director) + 1);
		strcpy(director_name, m_p->director);
		printf("director name : %s\n", director_name);
		d_p = root_director;
		printf("start to enter\n");
		while(1){
			if(d_p->name == NULL){
				if(d_p->next == NULL)
					break;
				d_p = d_p->next;
				printf("skip deleted...\n");
				continue;
			}

			if(!strcmp(director_name, d_p->name)) {
				m_p->d_link = d_p;	//movie����ü�� d_link�� director �̸��� �´� �ɷ� ����Ű�� ��
				printf("movie linked director : %s\n", director_name);
				break;
			}
			if(d_p->next == NULL)
				break;
			d_p = d_p->next;
			printf("finding director...\n");
		}
		printf("one time did\n");
		if(m_p->next == NULL){
			printf("what is problem?\n");
			break;
		}
		printf("%d movie struct passed\n", i++);
		m_p = m_p->next;
		printf("is it here?\n\n");
	}
}

void handler(int sig) {	//Ctrl + c ������ ������� �ʰ� ����� �ϴ� �Լ�
	char *answer;
	answer = (char *)malloc(sizeof(char) * 5);
	printf("\nControl+c\n");
	printf("Get Interrupt Signal.\n");
	printf("Do you want to exit myMOVIE program? (Y/N) ");
	scanf("%s", answer);
	//printf("answer : %s\n", answer);
	//printf("%d, %d\n", !strcmp(answer, "y"), !strcmp(answer, "Y"));
	if (!strcmp(answer, "y") || !strcmp(answer, "Y"))
		exit(1);
}

void add_movie(){	//movie ���� �Է¹޴� �Լ�
	char *temp;	//���ڸ� �Է¹��� �ӽ� ������
	FILE *fp;
	fp = fopen("movie_log", "at");

	temp = (char *)malloc(sizeof(char) * 200);	//�ӽ� ������ �����Ҵ�

	if (root_m_num == 0) {	//��ũ�� ����Ʈ ó�� ����� root_movie�� ����
		root_movie = m;
		root_m_num = 1;
	}
	m->serial_number = serial_m_num++;

	printf("title > ");
	gets(temp);	//title �Է�
	m->title = (char *)malloc(sizeof(char) * strlen(temp) + 1);	//�Է¹��� ������ ũ�⸸ŭ �����Ҵ� ����(+1�� �ǵڿ� null�� ���� ����)
	strcpy(m->title, temp);	//temp�� ����ü ����� �ű�

	printf("genre > ");
	gets(temp);
	m->genre = (char *)malloc(sizeof(char) * strlen(temp) + 1);
	strcpy(m->genre, temp);

	printf("director > ");
	gets(temp);
	m->director = (char *)malloc(sizeof(char) * strlen(temp) + 1);
	strcpy(m->director, temp);

	printf("year > ");
	scanf("%[^\n]s", temp);
	m->year = (char *)malloc(sizeof(char) * strlen(temp) + 1);
	strcpy(m->year, temp);

	printf("time > ");
	scanf("%s", temp);
	getchar();
	m->time = (char *)malloc(sizeof(char) * strlen(temp) + 1);
	strcpy(m->time, temp);

	printf("actors > ");
	gets(temp);
	m->actors = (char *)malloc(sizeof(char) * strlen(temp) + 1);
	strcpy(m->actors, temp);

	fprintf(fp, "add:%d:%s:%s:%s:%s:%s:%s\n", m->serial_number, colon_proc(m->title), colon_proc(m->genre), colon_proc(m->director), colon_proc(m->year), colon_proc(m->time), colon_proc(m->actors));

	m->next = (movie *)malloc(sizeof(movie));	//m�� next�����͸� �����Ҵ�
	m = m->next;	//m�� ���� m�� next�� �ٲ�
	m->next = NULL;	//������ m�� next�� null�� ����

	fclose(fp);
	printf("@@ Done\n\n");
}

void add_director() {	//director ���� �Է¹޴� �Լ�
	char *temp;
	FILE *fp;
	temp = (char *)malloc(sizeof(char) * 200);
	fp = fopen("director_log", "at");

	if (root_d_num == 0) {	//��ũ�� ����Ʈ ó�� ����� root_director�� ����
		root_director = d;
		root_d_num = 1;
	}
	d->serial_number = serial_d_num++;
	printf("name > ");
	gets(temp);	//name �Է�
	d->name = (char *)malloc(sizeof(char) * strlen(temp) + 1);	//�Է¹��� ������ ũ�⸸ŭ �����Ҵ� ����(+1�� �ǵڿ� null�� ���� ����)
	strcpy(d->name, temp);	//temp�� ����ü ����� �ű�

	printf("sex > ");
	scanf("%s", temp);
	d->sex = (char *)malloc(sizeof(char) * strlen(temp) + 1);
	strcpy(d->sex, temp);

	printf("birth > ");
	scanf("%s", temp);
	getchar();
	d->birth = (char *)malloc(sizeof(char) * strlen(temp) + 1);
	strcpy(d->birth, temp);

	printf("best_movies > ");
	gets(temp);
	d->best_movies = (char *)malloc(sizeof(char) * strlen(temp) + 1);
	strcpy(d->best_movies, temp);

	fprintf(fp, "add:%d:%s:%s:%s:%s\n", d->serial_number, colon_proc(d->name), colon_proc(d->sex), colon_proc(d->birth), colon_proc(d->best_movies));

	d->next = (director *)malloc(sizeof(director));	//d�� next�����͸� �����Ҵ�
	d = d->next;	//d�� ���� d�� next�� �ٲ�
	d->next = NULL;	//������ d�� next�� null�� ����

	fclose(fp);
	printf("@@ Done\n\n");
}

void add_actor() {	//actor�� ������ �Է¹޴� �Լ�
	char *temp;
	FILE *fp;
	temp = (char *)malloc(sizeof(char) * 200);
	fp = fopen("actor_log", "at");

	if (root_a_num == 0) {	//��ũ�� ����Ʈ ó�� ����� root_actor�� ����
		root_actor = a;
		root_a_num = 1;
	}
	a->serial_number = serial_a_num++;

	printf("name > ");
	gets(temp);
	a->name = (char *)malloc(sizeof(char) * strlen(temp) + 1);
	strcpy(a->name, temp);

	printf("sex > ");
	scanf("%s", temp);
	a->sex = (char *)malloc(sizeof(char) * strlen(temp) + 1);
	strcpy(a->sex, temp);

	printf("birth > ");
	scanf("%s", temp);
	getchar();
	a->birth = (char *)malloc(sizeof(char) * strlen(temp) + 1);
	strcpy(a->birth, temp);

	printf("best_movies > ");
	gets(temp);
	a->best_movies = (char *)malloc(sizeof(char) * strlen(temp) + 1);
	strcpy(a->best_movies, temp);

	fprintf(fp, "add:%d:%s:%s:%s:%s\n", a->serial_number, colon_proc(a->name), colon_proc(a->sex), colon_proc(a->birth), colon_proc(a->best_movies));

	a->next = (actor *)malloc(sizeof(actor));	//a�� next�����͸� �����Ҵ�
	a = a->next;	//a�� ���� a�� next�� �ٲ�
	a->next = NULL;	//������ a�� next�� null�� ����

	fclose(fp);
	printf("@@ Done\n\n");
}

void save_director() {
	FILE *fp;
	fp = fopen("director_list", "wt");
	d = root_director;
	while (d->next != NULL) {
		if (d->name == NULL) {
			d = d->next;
			continue;
		}
		fprintf(fp, "%d:%s:%s:%s:%s\n", d->serial_number, colon_proc(d->name), colon_proc(d->sex), colon_proc(d->birth), colon_proc(d->best_movies));
		d = d->next;
	}
	fclose(fp);
	printf("@@ Done\n\n");
}

void save_movie() {
	printf("error1\n");
	FILE *fp;
	fp = fopen("movie_list", "wt");
	printf("error2\n");
	m = root_movie;
	printf("error3\n");
	while (m->next != NULL) {
		if (m->title == NULL) {
			m = m->next;
			continue;
		}
		fprintf(fp, "%d:%s:%s:%s:%s:%s:%s\n", m->serial_number, colon_proc(m->title), colon_proc(m->genre), colon_proc(m->director), colon_proc(m->year), colon_proc(m->time), colon_proc(m->actors));
		m = m->next;
	}
	printf("error5\n");
	fclose(fp);
	printf("@@ Done\n\n");
}

void save_actor() {
	FILE *fp;
	fp = fopen("actor_list", "wt");
	a = root_actor;
	while (a->next != NULL) {
		if (a->name == NULL) {
			a = a->next;
			continue;
		}
		fprintf(fp, "%d:%s:%s:%s:%s\n", a->serial_number, colon_proc(a->name), colon_proc(a->sex), colon_proc(a->birth), colon_proc(a->best_movies));
		a = a->next;
	}
	fclose(fp);
	printf("@@ Done\n\n");
}

char *colon_proc(char *s) {	//':'�� "??;"�� ġȯ�ϴ� �Լ�
	char *result, *sr;	//result�� ġȯ�� �������, sr�� ����ؼ� �����ָ鼭 �ٲ�� ���ڿ�
	char *colon, *newstr;	//�ݷ��� ������ ���� ??;�� ������ ��
	colon = (char *)malloc(sizeof(char) * 2);	//�ݷ� ���� �Ҵ�
	newstr = (char *)malloc(sizeof(char) * 4);	//??; �����Ҵ�
	colon = ":";
	newstr = "??;";

	size_t i, count = 0;	//����� ���� ���� i ����
	size_t colonlen = strlen(colon);
	size_t newlen = strlen(newstr);

	for (i = 0; s[i] != '\0';) {	//:�� ??;�� �ٲٸ鼭 �ʿ��� �޸𸮸� ����ϴ� �ݺ���
		if (memcmp(&s[i], colon, colonlen) == 0) {
			count++;
			i += colonlen;
		}
		else i++;
	}

	result = (char *)malloc(i + 1 + count * (newlen - colonlen));	//�ʿ��� �޸𸮸�ŭ ���� �����Ҵ�

	sr = result;
	while (*s) {	//�ݷ��� ??;�� �ٲ��ִ� �ݺ���
		if (memcmp(s, colon, colonlen) == 0) {
			memcpy(sr, newstr, newlen);
			sr += newlen;
			s += colonlen;
		}
		else *sr++ = *s++;
	}
	*sr = '\0';	//�������� NULL ����

	return result;
}

char *anti_colon_proc(char *s) {	//'??;'�� ":"�� ġȯ�ϴ� �Լ�
	char *result, *sr;	//result�� ġȯ�� �������, sr�� ����ؼ� �����ָ鼭 �ٲ�� ���ڿ�
	char *anti_colon, *newstr;	//??;�� ������ ���� �ݷ��� ������ ��
	anti_colon = (char *)malloc(sizeof(char) * 2);	//"??;" ���� �Ҵ�
	newstr = (char *)malloc(sizeof(char) * 4);	//':' �����Ҵ�
	anti_colon = "??;";
	newstr = ":";

	size_t i, count = 0;	//����� ���� ���� i ����
	size_t anti_colonlen = strlen(anti_colon);
	size_t newlen = strlen(newstr);

	for (i = 0; s[i] != '\0';) {	//??;�� ':'�� �ٲٸ鼭 �ʿ��� �޸𸮸� ����ϴ� �ݺ���
		if (memcmp(&s[i], anti_colon, anti_colonlen) == 0) {
			count++;
			i += anti_colonlen;
		}
		else i++;
	}

	result = (char *)malloc(i + 1 + count * (newlen - anti_colonlen));	//�ʿ��� �޸𸮸�ŭ ���� �����Ҵ�

	sr = result;
	while (*s) {	//??;�� �ݷ����� �ٲ��ִ� �ݺ���
		if (memcmp(s, anti_colon, anti_colonlen) == 0) {
			memcpy(sr, newstr, newlen);
			sr += newlen;
			s += anti_colonlen;
		}
		else *sr++ = *s++;
	}
	*sr = '\0';	//�������� NULL ����

	return result;
}


void print_m(int sn){ //movie print �Լ�
	movie *m_p;
	director *d_p;
	actor *a_p;
   m_p = root_movie;
   d_p = root_director;
   a_p = root_actor;
   while(m_p->serial_number != sn){ //�Է��� serial_number�� ���� ��� �޽��� ���
      if(m_p->next == NULL){
         printf("serial number is not found\n\n");
         return;
      }
      m_p = m_p->next;
   }
	if(m_p->title == NULL){ //delete�� ���ڵ��� ���
	 	printf("*deleted record*\n\n", m_p->serial_number);
		return;
	}

  printf("%d, %s, %s\n", m_p->serial_number, m_p->title == NULL ? "-" : m_p->title, m_p->genre==NULL ? "-" : m_p->genre);
	if(root_d_num == 0){ //director�� �ƹ��͵� ������ �� ���ִ� ���¿����� ������ ���ϱ� ���� �ڵ�
		printf("(-)\n");
	}

	else{ //director ��ũ�� ����Ʈ�� �о� �ش��ϴ� director�� �ִ��� �˻�
		while(1){
			if(d_p->next == NULL){
				break;
			}
			if(d_p->name == NULL){
				d_p = d_p->next;
				continue;
			}
			if(m_p->director == NULL){
				d_p = d_p->next;
				continue;
			}
			if(!strcmp(m_p->director, d_p->name)){ //�ش��ϴ� movie ���ڵ忡�� ����ϴ� director�� director ���ڵ忡 ���� ���
				break;
			}
   	   d_p = d_p->next;
   	}
		printf("D : %s(%s)\n", m_p->director==NULL? "-":m_p->director, d_p->next==NULL ? "-" : d_p->birth); //director�� ���ڵ忡 ���� ��쿣 �ش� ���ڵ忡�� �о���� ���� ��쿣 "-" ���.
	}
	char *a_name = NULL;
	char *string = NULL;

	int i = 1; //��� ���� ����ϱ� ���� ����
	if(m_p->actors != NULL){
		a_name = (char *)malloc(sizeof(char)*strlen(m_p->actors)+1); //��찡 �� ������ ��Ȯ�� �� �� �����Ƿ� �ִ밪 �Ҵ�.
		string = (char *)malloc(sizeof(char)*strlen(m_p->actors)+1);
		strcpy(string, m_p->actors); //movie ���ڵ��� actors ��°�� ����
		a_name = strtok(string, ","); //�ϳ��ϳ� ����

	if(root_a_num == 0){ //��� ���ڵ忡 �ƹ��͵� ����Ǿ����� ���� ��� ������ ���ϱ� ���� �ڵ�
	//	printf("A%d : %s(-)\n", i++, a_name);
	}
	else{  //actors ��ũ�� ����Ʈ�� �о� �ش� actor�� �ִ��� �˻�
		while(1){
			if(a_p->next == NULL)
				break;
			if(a_p->name == NULL){
				a_p = a_p ->next;
				continue;
			}
			if(*(a_name + strlen(a_name) - 1) == 13){
				*(a_name + strlen(a_name) - 1) = 0;
			}
			if(*(a_name + strlen(a_name) - 1) == 13){
				*(a_name + strlen(a_name) - 1) = 0;
			}
			if(!strncmp(a_name, a_p->name, strlen(a_name)))
				break;
			a_p = a_p->next;
		}

		printf("A%d : %s(%s)\n", i++, a_name==NULL? "-": a_name, (a_p->next == NULL) ? "-" : a_p->birth); //ã�� ��쿣 actor ���ڵ忡�� ������ �о���� ���� ��쿣 "-" ���
	}

		a_name = strtok(NULL, ",");
		while(1){ //�ι�° actor������ �ݺ���(strtok �Լ��� �ι�° ������� ������ �ٲ�� ������ ��������)
			if(a_name == NULL){
				printf("\n");
				return;
			}
			if(*a_name == ' '){ //��� �̸��� ', '�� �ԷµǾ��� ��� strtok�� ������ ���� ����� �̸��� ù���ڰ� �����̹Ƿ� ������ �����ִ� �ڵ�
				a_name = a_name+sizeof(char);
			}
			a_p = root_actor;
			while(1){
				if(a_p->next == NULL)
					break;
				if(a_p->name == NULL){
					a_p = a_p->next;
					continue;
				}
				if(*(a_name + strlen(a_name) - 1) == 13){
					*(a_name + strlen(a_name) - 1) = 0;
				}
				if(*(a_name + strlen(a_name) - 1) == 13){
					*(a_name + strlen(a_name) - 1) = 0;
				}
				if(!strncmp(a_name, a_p->name,strlen(a_name)))
					break;
				a_p = a_p->next;
			}

			printf("A%d : %s(%s)\n", i++, a_name, a_p->next == NULL ? "-" : a_p->birth);
			a_name = strtok(NULL, ",");
		}
	}
	else{
		printf("A%d : -(-)\n\n", i++);
		return;
	}



}
void print_d(int sn){ //director ���ڵ��� print �Լ�
	movie *m_p;
	director *d_p;
	actor *a_p;
   m_p = root_movie;
   d_p = root_director;
   a_p = root_actor;
   while(d_p->serial_number != sn){ //serial_number�� ���� ����� �����޽��� ���
      if(root_d_num == 0 || d_p->next == NULL){
         printf("serial number is not found\n\n");
         return;
      }
      d_p = d_p->next;
   }
	 if(d_p->name == NULL){
		 printf("*deleted record*\n");
		 return;
	 }

	char *a_best_movie = (char *)malloc(sizeof(char)*strlen(d_p->best_movies)+1); //best_movies���� �� ��ǰ�� ���� �����ϱ� ���� ������
	char *string = (char *)malloc(sizeof(char)*strlen(d_p->best_movies)+1); //best_movies�� ��°�� �����ϱ� ���� ������

  printf("%d, %s, %s, %s\n", d_p->serial_number, d_p->name, d_p->sex, d_p->birth); //director�� ���� ���
	strcpy(string, d_p->best_movies);
	a_best_movie = strtok(string, ",");

	while(1){ //a_best_movie�� movie ���ڵ忡 �ִ��� �˻�
		if(a_best_movie == NULL){//best movie�� �Էµ��� �ʾ��� ��
			printf("\n");
			return;
		}
		if(root_m_num == 0 || m_p->next == NULL){ //movie ���ڵ忡 �ƹ��͵� �Էµ��� �ʾƼ� �ߴ� ������ ���ϱ� ���� �ڵ� + �� ã���� ��� break
			break;
		}
		if(m_p->title == NULL){
			m_p = m_p->next;
			continue;
		}
		if(*(a_best_movie + strlen(a_best_movie) - 1) == 13){
			*(a_best_movie + strlen(a_best_movie) - 1) = 0;
		}
		if(*(a_best_movie + strlen(a_best_movie) - 1) == 13){
			*(a_best_movie + strlen(a_best_movie) - 1) = 0;
		}
		if(!strncmp(a_best_movie, m_p->title, strlen(a_best_movie))){ //ã���� break�� ��������
			break;
		}
		m_p = m_p->next;
	}

	printf("%s, %s, %s\n", a_best_movie, m_p->next==NULL ? "-" : m_p->year, m_p->next==NULL ? "-" : m_p->time);
	a_best_movie = strtok(NULL, ",");
	while(1){		//�ι�° best_movies������ �ݺ���
		if(a_best_movie == NULL){
			printf("\n");
			return;
		}
		if(*a_best_movie == ' ')
			a_best_movie = a_best_movie+sizeof(char);
		m_p = root_movie;
		while(1){
				if(m_p->next == NULL)
					break;
				if(m_p->title == NULL){
					m_p=m_p->next;
					continue;
				}
				if(*(a_best_movie + strlen(a_best_movie) - 1) == 13){
					*(a_best_movie + strlen(a_best_movie) - 1) = 0;
				}
				if(*(a_best_movie + strlen(a_best_movie) - 1) == 13){
					*(a_best_movie + strlen(a_best_movie) - 1) = 0;
				}
				if(!strncmp(a_best_movie, m_p->title, strlen(a_best_movie)))
					break;
				m_p = m_p->next;
		}
		printf("%s, %s, %s\n", a_best_movie, m_p->next==NULL? "-" : m_p->year, m_p->next==NULL? "-" : m_p->time);
		a_best_movie = strtok(NULL, ",");
	}

	printf("\n");


}


void print_a(int sn){
	movie *m_p;
	director *d_p;
	actor *a_p;
   m_p = root_movie;
   d_p = root_director;
   a_p = root_actor;
   while(a_p->serial_number != sn){
      if(a_p->next == NULL){
         printf("serial number is not found\n\n");
         return;
      }
      a_p = a_p->next;
   }
	 if(a_p->name==NULL){
		 printf("*deleted record*\n");
		 return;
	 }
	char *a_best_movie = (char *)malloc(sizeof(char)*strlen(a_p->best_movies)+1);
	char *string = (char *)malloc(sizeof(char)*strlen(a_p->best_movies)+1);

  printf("%d, %s, %s, %s\n", a_p->serial_number, a_p->name, a_p->sex, a_p->birth);

	strcpy(string, a_p->best_movies);
	a_best_movie = strtok(string, ",");
	while(1){
		if(a_best_movie == NULL){
			printf("\n"); //best movie�� �ƿ� �Էµ��� �ʾ��� ��
			return;
		}
		if(m_p->next==NULL){ //best movie�� ��Ͽ� ���� ��
			break;
		}
		if(m_p->title == NULL){
			m_p = m_p->next;
			continue;
		}
		if(*(a_best_movie + strlen(a_best_movie) - 1) == 13){
			*(a_best_movie + strlen(a_best_movie) - 1) = 0;
		}
		if(*(a_best_movie + strlen(a_best_movie) - 1) == 13){
			*(a_best_movie + strlen(a_best_movie) - 1) = 0;
		}
		if(!strncmp(a_best_movie, m_p->title, strlen(a_best_movie)))
			break;
		m_p = m_p->next;
	}

	printf("%s, %s, %s\n", a_best_movie, m_p->next == NULL ? "-" : m_p->year, m_p->next == NULL ? "-" : m_p->time);
	a_best_movie = strtok(NULL, ",");
	while(1){	//�ι�° ��ǥ�ۺ����� �ݺ���
		if(a_best_movie == NULL){
			printf("\n");
			return;
		}
		if(*a_best_movie == ' ')
			a_best_movie = a_best_movie+sizeof(char);
		m_p = root_movie;
		while(1){
				if(m_p->next == NULL){
					break;
				}
				if(m_p->title == NULL){
					m_p = m_p->next;
					continue;
				}
				if(*(a_best_movie + strlen(a_best_movie) - 1) == 13){
					*(a_best_movie + strlen(a_best_movie) - 1) = 0;
				}
				if(*(a_best_movie + strlen(a_best_movie) - 1) == 13){
					*(a_best_movie + strlen(a_best_movie) - 1) = 0;
				}
				if(!strncmp(a_best_movie, m_p->title, strlen(a_best_movie)-2))
					break;
				m_p = m_p->next;
			}
		printf("%s, %s, %s\n", a_best_movie, m_p->next==NULL ? "-" : m_p->year, m_p->next==NULL ? "-" : m_p->time);
		a_best_movie = strtok(NULL, ",");
	}

}
void print_m_file(int sn, char *fn){ //sort�� ���� file ��� �Լ�(print�Լ��� ���� ����)
	movie *m_p;
	director *d_p;
	actor *a_p;
   m_p = root_movie;
   d_p = root_director;
   a_p = root_actor;
	 FILE *ofp = fopen(fn, "a");
   while(m_p->serial_number != sn){
      if(m_p->next == NULL){
         printf("serial number is not found\n\n");
         return;
      }
      m_p = m_p->next;
   }
	 if(m_p->title==NULL){
		 printf("*deleted record*\n");
		 return;
	 }
  fprintf(ofp, "%d, %s, %s\n", m_p->serial_number, m_p->title == NULL ? "-" : m_p->title, m_p->genre == NULL? "-" : m_p->genre);
	if(root_d_num == 0){
		fprintf(ofp, "(-)\n");
	}
	else{
   	while(1){
			if(d_p->next == NULL){
					break;
			}
			if(d_p->name == NULL){
				d_p=d_p->next;
				continue;
			}
			if(!strcmp(m_p->director, d_p->name)){
				break;
			}
   	   d_p = d_p->next;
   	}
		fprintf(ofp, "D : %s(%s)\n", m_p->director == NULL? "-" : m_p->director, d_p->next==NULL ? "-" : d_p->birth);
	}

	char *a_name = (char *)malloc(sizeof(char)*strlen(m_p->actors)+1);
	char *string = (char *)malloc(sizeof(char)*strlen(m_p->actors)+1);

	strcpy(string, m_p->actors);
	a_name = strtok(string, ",");
	int i = 1;

	if(root_a_num == 0){
		fprintf(ofp, "A%d : %s(-)\n", i++, a_name);
	}
	else{
		while(1){
			if(a_p->next == NULL)
				break;
			if(a_p->name == NULL){
				a_p = a_p->next;
				continue;
			}
			if(*(a_name + strlen(a_name) - 1) == 13){
				*(a_name + strlen(a_name) - 1) = 0;
			}
			if(*(a_name + strlen(a_name) - 1) == 13){
				*(a_name + strlen(a_name) - 1) = 0;
			}
			if(!strncmp(a_name, a_p->name, strlen(a_name)-1))
				break;
			a_p = a_p->next;
		}

		fprintf(ofp, "A%d : %s(%s)\n", i++, a_name, a_p->next == NULL ? "-" : a_p->birth);
	}

	a_name = strtok(NULL, ",");
	while(1){ //�ι�° actor������ �ݺ���
		if(a_name == NULL){
			fprintf(ofp, "\n");
			fclose(ofp);
			return;
		}
		if(*a_name == ' '){
			a_name = a_name+sizeof(char);
		}
		a_p = root_actor;
		while(1){
			if(a_p->next == NULL)
				break;
			if(a_p->name == NULL){
				a_p = a_p->next;
				continue;
			}
			if(*(a_name + strlen(a_name) - 1) == 13){
				*(a_name + strlen(a_name) - 1) = 0;
			}
			if(*(a_name + strlen(a_name) - 1) == 13){
				*(a_name + strlen(a_name) - 1) = 0;
			}
			if(!strncmp(a_name, a_p->name, strlen(a_name)-1))
				break;
			a_p = a_p->next;
		}


		fprintf(ofp, "A%d : %s(%s)\n", i++, a_name, a_p->next == NULL ? "-" : a_p->birth);
		a_name = strtok(NULL, ",");
	}

}

void print_d_file(int sn, char *fn){
	movie *m_p;
	director *d_p;
	actor *a_p;
   m_p = root_movie;
   d_p = root_director;
   a_p = root_actor;
	 FILE *ofp = fopen(fn, "a");
   while(d_p->serial_number != sn){
      if(root_d_num == 0 || d_p->next == NULL){
         printf("serial number is not found\n\n");
         return;
      }
      d_p = d_p->next;
   }
	 if (d_p->name==NULL){
		 printf("*deleted record*\n");
		 return;
	 }

	char *a_best_movie = (char *)malloc(sizeof(char)*strlen(d_p->best_movies)+1);
	char *string = (char *)malloc(sizeof(char)*strlen(d_p->best_movies)+1);

  fprintf(ofp, "%d, %s, %s, %s\n", d_p->serial_number, d_p->name==NULL? "-" : d_p->name, d_p->sex==NULL? "-" : d_p->sex, d_p->birth==NULL?"-":d_p->birth);
	strcpy(string, d_p->best_movies);
	a_best_movie = strtok(string, ",");

	while(1){
		if(a_best_movie == NULL){//best movie�� �Էµ��� �ʾ��� ��
			fprintf(ofp, "\n");
			return;
		}
		if(root_m_num == 0 || m_p->next == NULL){
			break;
		}
		if(m_p->title ==NULL){
			m_p=m_p->next;
			continue;
		}
		if(*(a_best_movie + strlen(a_best_movie) - 1) == 13){
			*(a_best_movie + strlen(a_best_movie) - 1) = 0;
		}
		if(*(a_best_movie + strlen(a_best_movie) - 1) == 13){
			*(a_best_movie + strlen(a_best_movie) - 1) = 0;
		}
		if(!strncmp(a_best_movie, m_p->title, strlen(a_best_movie)-1)){
			break;
		}
		m_p = m_p->next;
	}

	fprintf(ofp, "%s, %s, %s\n", a_best_movie, m_p->next==NULL ? "-" : m_p->year, m_p->next==NULL ? "-" : m_p->time);
	a_best_movie = strtok(NULL, ",");
	while(1){		//�ι�° ��ǥ�ۺ����� �ݺ���
		if(a_best_movie == NULL){
			fprintf(ofp, "\n");
			fclose(ofp);
			return;
		}
		if(*a_best_movie == ' ')
			a_best_movie = a_best_movie+sizeof(char);
		m_p = root_movie;
		while(1){
				if(m_p->next == NULL)
					break;
				if(m_p->title == NULL){
					m_p=m_p->next;
					continue;
				}
				if(*(a_best_movie + strlen(a_best_movie) - 1) == 13){
					*(a_best_movie + strlen(a_best_movie) - 1) = 0;
				}
				if(*(a_best_movie + strlen(a_best_movie) - 1) == 13){
					*(a_best_movie + strlen(a_best_movie) - 1) = 0;
				}
				if(!strncmp(a_best_movie, m_p->title, strlen(a_best_movie)-1))
					break;
				m_p = m_p->next;
		}
		fprintf(ofp, "%s, %s, %s\n", a_best_movie, m_p->next==NULL? "-" : m_p->year, m_p->next==NULL? "-" : m_p->time);
		a_best_movie = strtok(NULL, ",");
	}
}

void print_a_file(int sn, char *fn){
	movie *m_p;
	director *d_p;
	actor *a_p;
   m_p = root_movie;
   d_p = root_director;
   a_p = root_actor;
	 FILE *ofp = fopen(fn, "a");
   while(a_p->serial_number != sn){
      if(a_p->next == NULL){
         printf("serial number is not found\n\n");
         return;
      }
      a_p = a_p->next;
   }
	 if(a_p->name == NULL){
		 printf("*deleted record*\n");
		 return;
	 }

	char *a_best_movie = (char *)malloc(sizeof(char)*strlen(a_p->best_movies)+1);
	char *string = (char *)malloc(sizeof(char)*strlen(a_p->best_movies)+1);

  fprintf(ofp, "%d, %s, %s, %s\n", a_p->serial_number, a_p->name==NULL? "-" : a_p->name, a_p->sex==NULL? "-" : a_p->sex, a_p->birth==NULL? "-" : a_p->birth);

	strcpy(string, a_p->best_movies);
	a_best_movie = strtok(string, ",");
	while(1){
		if(a_best_movie == NULL){
			fprintf(ofp, "\n"); //best movie�� �ƿ� �Էµ��� �ʾ��� ��
			return;
		}
		if(m_p->next==NULL){ //best movie�� ��Ͽ� ���� ��
			break;
		}
		if(m_p->title == NULL){
			m_p=m_p->next;
			continue;
		}
		if(*(a_best_movie + strlen(a_best_movie) - 1) == 13){
			*(a_best_movie + strlen(a_best_movie) - 1) = 0;
		}
		if(*(a_best_movie + strlen(a_best_movie) - 1) == 13){
			*(a_best_movie + strlen(a_best_movie) - 1) = 0;
		}
		if(!strncmp(a_best_movie, m_p->title, strlen(a_best_movie)-1))
			break;
		m_p = m_p->next;
	}

	fprintf(ofp, "%s, %s, %s\n", a_best_movie, m_p->next == NULL ? "-" : m_p->year, m_p->next == NULL ? "-" : m_p->time);
	a_best_movie = strtok(NULL, ",");
	while(1){	//�ι�° ��ǥ�ۺ����� �ݺ���
		if(a_best_movie == NULL){
			fprintf(ofp, "\n");
			fclose(ofp);
			return;
		}
		if(*a_best_movie == ' ')
			a_best_movie = a_best_movie+sizeof(char);
		m_p = root_movie;
		while(1){
			if(m_p->next == NULL){
				break;
			}
			if(m_p->title ==NULL){
				m_p=m_p->next;
				continue;
			}
			if(*(a_best_movie + strlen(a_best_movie) - 1) == 13){
				*(a_best_movie + strlen(a_best_movie) - 1) = 0;
			}
			if(*(a_best_movie + strlen(a_best_movie) - 1) == 13){
				*(a_best_movie + strlen(a_best_movie) - 1) = 0;
			}
			if(!strncmp(a_best_movie, m_p->title, strlen(a_best_movie)-1))
				break;
			m_p = m_p->next;
		}

		fprintf(ofp, "%s, %s, %s\n", a_best_movie, m_p->next==NULL ? "-" : m_p->year, m_p->next==NULL ? "-" : m_p->time);
		a_best_movie = strtok(NULL, ",");
	}

}

void sort(char *factor, char *option, char *file_name){//����ü ũ�� 64
	if(strcmp(file_name, "NULL")){//filename�� ���������� �� ���� ���� ����
		FILE *fp = fopen(file_name, "w");
		fclose(fp);
	}

	if(!strcmp(factor, "m")){ // factor == m
		movie *m_p; //��ũ�帮��Ʈ ������
		sorting *m_string; //movie�� ����ü ������� �ֱ� ���� ����ü
		sorting m_tmp; //������ ���� tmp
		sorting_num *m_num; //movie�� ����ü ������� �ֱ� ���� ����ü
		sorting_num m_tmp_n; //int�� ������ ���� tmp
		m_p = root_movie; //������������ ��������
		int total = 0; //�� ���ڵ� ������ ���� ���� ����
		while(1){
			if(m_p->next == NULL){
				break;
			}
			total++;
			m_p = m_p->next;
		}
		m_string = (sorting *)malloc(sizeof(sorting)*total); //���ڵ� ����ŭ ����ü �����Ϳ� �޸� �Ҵ�


		if(!strcmp(option, "NULL") || !strcmp(option, "t")){ //option == NULL or t�� �� (��� ����)
			m_p = root_movie;
			for(int i=0; i<total; i++){ //movie ���ڵ��� title�� ��� �ӽ� ����ü�� �Űܿ�
				if(m_p->title == NULL){
					m_p=m_p->next;
					total--;
					i--;
					continue;
				}
				(*(m_string+i)).string = (char *)malloc(sizeof(char)*strlen(m_p->title)+1);
				strcpy((*(m_string+i)).string, m_p->title);
				(*(m_string+i)).serial_number = m_p->serial_number;
				m_p=m_p->next;

		}
			for(int i=total-1; i>0; i--){ //��������
				for(int j=0; j<i; j++){
					if((strcmp((*(m_string+j)).string, (*(m_string+j+1)).string)) > 0){
						m_tmp = *(m_string+j);
						*(m_string+j) = *(m_string+j+1);
					*(m_string+j+1) = m_tmp;
					}
				}

			}

			if(!strcmp(file_name, "NULL")){ //ȭ�� ����� ��
				for(int i=0; i<total; i++){
					print_m((*(m_string+i)).serial_number);
				}
			}
			else{		//filename ������ ����� ��
				for(int i=0; i<total; i++){
					print_m_file((*(m_string+i)).serial_number, file_name);
				}
			}
		}
		else if(!strcmp(option, "g")){ //factor == m, option == g
			m_p = root_movie;
			for(int i=0; i<total; i++){
				if(m_p->title == NULL){
					m_p=m_p->next;
					total--;
					i--;
					continue;
				}
				(*(m_string+i)).string = (char *)malloc(sizeof(char)*strlen(m_p->genre)+1);
				strcpy((*(m_string+i)).string, m_p->genre);
				(*(m_string+i)).serial_number = m_p->serial_number;
				m_p=m_p->next;
			}
			for(int i=total-1; i>0; i--){
				for(int j=0; j<i; j++){
					if((strcmp((*(m_string+j)).string, (*(m_string+j+1)).string)) > 0){
						m_tmp = *(m_string+j);
						*(m_string+j) = *(m_string+j+1);
						*(m_string+j+1) = m_tmp;
					}
				}
			}
			if(!strcmp(file_name, "NULL")){ //ȭ�� ����� ��
				for(int i=0; i<total; i++){
					print_m((*(m_string+i)).serial_number);
				}
			}
			else{		//filename ������ ����� ��
				for(int i=0; i<total; i++){
					print_m_file((*(m_string+i)).serial_number, file_name);
				}
			}
		}
		else if(!strcmp(option, "d")){ //factor == m, option == d
			m_p = root_movie;
			for(int i=0; i<total; i++){
				if(m_p->title == NULL){
					m_p=m_p->next;
					total--;
					i--;
					continue;
				}
				(*(m_string+i)).string = (char *)malloc(sizeof(char)*strlen(m_p->director)+1);
				strcpy((*(m_string+i)).string, m_p->director);
				(*(m_string+i)).serial_number = m_p->serial_number;
				m_p=m_p->next;
			}
			for(int i=total-1; i>0; i--){
				for(int j=0; j<i; j++){
					if((strcmp((*(m_string+j)).string, (*(m_string+j+1)).string)) > 0){
						m_tmp = *(m_string+j);
						*(m_string+j) = *(m_string+j+1);
						*(m_string+j+1) = m_tmp;
					}
				}
			}
			if(!strcmp(file_name, "NULL")){ //ȭ�� ����� ��
				for(int i=0; i<total; i++){
					print_m((*(m_string+i)).serial_number);
				}
			}
			else{		//filename ������ ����� ��
				for(int i=0; i<total; i++){
					print_m_file((*(m_string+i)).serial_number, file_name);
				}
			}
		}

		else if(!strcmp(option, "y")){ //option == y (������ int������ ��)
			m_p = root_movie;
			m_num = (sorting_num *)malloc(sizeof(sorting_num)*total);
			for(int i=0; i<total; i++){
				if(m_p->title == NULL){
					m_p=m_p->next;
					total--;
					i--;
					continue;
				}
				(*(m_num+i)).num = atoi(m_p->year);
				(*(m_num+i)).serial_number = m_p->serial_number;
				m_p=m_p->next;
			}
			for(int i=total-1; i>0; i--){
				for(int j=0; j<i; j++){
					if((*(m_num+j)).num > (*(m_num+j+1)).num){
						m_tmp_n = *(m_num+j);
						*(m_num+j) = *(m_num+j+1);
						*(m_num+j+1) = m_tmp_n;
					}
				}
			}
			if(!strcmp(file_name, "NULL")){ //ȭ�� ����� ��
				for(int i=0; i<total; i++){
					print_m((*(m_num+i)).serial_number);
				}
			}
			else{		//filename ������ ����� ��
				for(int i=0; i<total; i++){
					print_m_file((*(m_num+i)).serial_number, file_name);
				}
			}
		}
		else if(!strcmp(option, "r")){ //option == t (�ð��� int������ ��)
			m_p = root_movie;
			m_num = (sorting_num *)malloc(sizeof(sorting_num)*total);
			for(int i=0; i<total; i++){
				if(m_p->title == NULL){
					m_p=m_p->next;
					total--;
					i--;
					continue;
				}
				(*(m_num+i)).num = atoi(m_p->time);
				(*(m_num+i)).serial_number = m_p->serial_number;
				m_p=m_p->next;
			}

			for(int i=total-1; i>0; i--){
				for(int j=0; j<i; j++){
					if((*(m_num+j)).num > (*(m_num+j+1)).num){
						m_tmp_n = *(m_num+j);
						*(m_num+j) = *(m_num+j+1);
						*(m_num+j+1) = m_tmp_n;
					}
				}
			}
			if(!strcmp(file_name, "NULL")){ //ȭ�� ����� ��
				for(int i=0; i<total; i++){
					print_m((*(m_num+i)).serial_number);
				}
			}
			else{		//filename ������ ����� ��
				for(int i=0; i<total; i++){
					print_m_file((*(m_num+i)).serial_number, file_name);
				}
			}
		}
		else if(!strcmp(option, "a")){ //factor == m, option == a //�� ù��° ��� �̸����� ��.
			m_p = root_movie;
			char *token;
			char *st; //�ӽ÷� actors�� ��°�� ���� ������
			for(int i=0; i<total; i++){
				if(m_p->title == NULL){
					m_p=m_p->next;
					total--;
					i--;
					continue;
				}
				st = (char *)malloc(sizeof(char)+strlen(m_p->actors)+1);
				strcpy(st, m_p->actors);
				token = strtok(st, ","); //token���� ù��° ��츸 �߶�.
				(*(m_string+i)).string = (char *)malloc(sizeof(char)*strlen(token)+1);
				strcpy((*(m_string+i)).string, token); //ù��° ��츦 ����ü�� ����.
				(*(m_string+i)).serial_number = m_p->serial_number;
				m_p=m_p->next;
			}
			for(int i=total-1; i>0; i--){
				for(int j=0; j<i; j++){
					if((strcmp((*(m_string+j)).string, (*(m_string+j+1)).string)) > 0){
						m_tmp = *(m_string+j);
						*(m_string+j) = *(m_string+j+1);
						*(m_string+j+1) = m_tmp;
					}
				}
			}
			if(!strcmp(file_name, "NULL")){ //ȭ�� ����� ��
				for(int i=0; i<total; i++){
					print_m((*(m_string+i)).serial_number);
				}
			}
			else{		//filename ������ ����� ��
				for(int i=0; i<total; i++){
					print_m_file((*(m_string+i)).serial_number, file_name);
				}
			}
		}
	}
	else if(!strcmp(factor, "d")){ // factor == d
		director *d_p; //director ��ũ�� ����Ʈ ������
		sorting *d_string; //director�� char*��, serial number �ֱ� ���� ����ü
		sorting d_tmp; //������ ���� tmp
		sorting_num *d_num; //director�� int��, serial number �ֱ� ���� ����ü
		sorting_num d_tmp_n; //int�� ������ ���� tmp
		d_p = root_director;
		int total = 0;
		while(1){
			if(d_p->next == NULL){
				break;
			}
			total++;
			d_p = d_p->next;
		}
		d_string = (sorting *)malloc(sizeof(sorting)*total);

		if(!strcmp(option, "NULL") || !strcmp(option, "n")){ //option ==NULL or n
			d_p = root_director;
			for(int i=0; i<total; i++){
				if(d_p->name == NULL){
					d_p=d_p->next;
					total--;
					i--;
					continue;
				}
				(*(d_string+i)).string = (char *)malloc(sizeof(char)*strlen(d_p->name)+1);
				strcpy((*(d_string+i)).string, d_p->name);
				(*(d_string+i)).serial_number = d_p->serial_number;
				d_p=d_p->next;
			}
			for(int i=total-1; i>0; i--){
				for(int j=0; j<i; j++){
					if((strcmp((*(d_string+j)).string, (*(d_string+j+1)).string)) > 0){
						d_tmp = *(d_string+j);
						*(d_string+j) = *(d_string+j+1);
						*(d_string+j+1) = d_tmp;
					}
				}
			}
			if(!strcmp(file_name, "NULL")){ //ȭ�� ����� ��
				for(int i=0; i<total; i++){
					print_d((*(d_string+i)).serial_number);
				}
			}
			else{		//filename ������ ����� ��
				for(int i=0; i<total; i++){
					print_d_file((*(d_string+i)).serial_number, file_name);
				}
			}
		}
		else if(!strcmp(option, "s")){ //option ==s
			d_p = root_director;
			for(int i=0; i<total; i++){
				if(d_p->name == NULL){
					d_p=d_p->next;
					total--;
					i--;
					continue;
				}
				(*(d_string+i)).string = (char *)malloc(sizeof(char)*strlen(d_p->sex)+1);
				strcpy((*(d_string+i)).string, d_p->sex);
				(*(d_string+i)).serial_number = d_p->serial_number;
				d_p=d_p->next;
			}
			for(int i=total-1; i>0; i--){
				for(int j=0; j<i; j++){
					if((strcmp((*(d_string+j)).string, (*(d_string+j+1)).string)) > 0){
						d_tmp = *(d_string+j);
						*(d_string+j) = *(d_string+j+1);
						*(d_string+j+1) = d_tmp;
					}
				}
			}
			if(!strcmp(file_name, "NULL")){ //ȭ�� ����� ��
				for(int i=0; i<total; i++){
					print_d((*(d_string+i)).serial_number);
				}
			}
			else{		//filename ������ ����� ��
				for(int i=0; i<total; i++){
					print_d_file((*(d_string+i)).serial_number, file_name);
				}
			}
		}
		else if(!strcmp(option, "b")){ //option == b (��������� int������ ��)//� ������ ���ĵ�.
			d_p = root_director;
			d_num = (sorting_num *)malloc(sizeof(sorting_num)*total);
			for(int i=0; i<total; i++){
				if(d_p->name == NULL){
					d_p=d_p->next;
					total--;
					i--;
					continue;
				}
				(*(d_num+i)).num = atoi(d_p->birth);
				(*(d_num+i)).serial_number = d_p->serial_number;
				d_p=d_p->next;
			}
			for(int i=total-1; i>0; i--){
				for(int j=0; j<i; j++){
					if((*(d_num+j)).num > (*(d_num+j+1)).num){
						d_tmp_n = *(d_num+j);
						*(d_num+j) = *(d_num+j+1);
						*(d_num+j+1) = d_tmp_n;
					}
				}
			}
			if(!strcmp(file_name, "NULL")){ //ȭ�� ����� ��
				for(int i=0; i<total; i++){
					print_d((*(d_num+i)).serial_number);
				}
			}
			else{		//filename ������ ����� ��
				for(int i=0; i<total; i++){
					print_d_file((*(d_num+i)).serial_number, file_name);
				}
			}
		}
		else if(!strcmp(option, "m")){ //option == m ù��° ��ȭ �������� ��.
			d_p = root_director;
			char *token;
			char *st; //�ӽ÷� best_movies�� ��°�� ���� ������
			for(int i=0; i<total; i++){
				if(d_p->name == NULL){
					d_p=d_p->next;
					total--;
					i--;
					continue;
				}
				st = (char *)malloc(sizeof(char)+strlen(d_p->best_movies)+1);
				strcpy(st, d_p->best_movies);
				token = strtok(st, ",");
				(*(d_string+i)).string = (char *)malloc(sizeof(char)*strlen(token)+1);
				strcpy((*(d_string+i)).string, token);
				(*(d_string+i)).serial_number = d_p->serial_number;
				d_p=d_p->next;
			}
			for(int i=total-1; i>0; i--){
				for(int j=0; j<i; j++){
					if((strcmp((*(d_string+j)).string, (*(d_string+j+1)).string)) > 0){
						d_tmp = *(d_string+j);
						*(d_string+j) = *(d_string+j+1);
						*(d_string+j+1) = d_tmp;
					}
				}
			}
			if(!strcmp(file_name, "NULL")){ //ȭ�� ����� ��
				for(int i=0; i<total; i++){
					print_d((*(d_string+i)).serial_number);
				}
			}
			else{		//filename ������ ����� ��
				for(int i=0; i<total; i++){
					print_d_file((*(d_string+i)).serial_number, file_name);
				}
			}
		}
	}
	else if(!strcmp(factor, "a")){ // factor == a
		actor *a_p;
		sorting *a_string; //director�� char*��, serial number �ֱ� ���� ����ü
		sorting a_tmp; //������ ���� tmp
		sorting_num *a_num; //director�� int��, serial number �ֱ� ���� ����ü
		sorting_num a_tmp_n; //int�� ������ ���� tmp
		a_p = root_actor;
		int total = 0;
		while(1){
			if(a_p->next == NULL){
				break;
			}
			total++;
			a_p = a_p->next;
		}
		a_string = (sorting *)malloc(sizeof(sorting)*total);

		if(!strcmp(option, "NULL") || !strcmp(option, "n")){ //option ==NULL or n
			a_p = root_actor;
			for(int i=0; i<total; i++){
				if(a_p->name == NULL){
					a_p=a_p->next;
					total--;
					i--;
					continue;
				}
				(*(a_string+i)).string = (char *)malloc(sizeof(char)*strlen(a_p->name)+1);
				strcpy((*(a_string+i)).string, a_p->name);
				(*(a_string+i)).serial_number = a_p->serial_number;
				a_p=a_p->next;
			}
			for(int i=total-1; i>0; i--){
				for(int j=0; j<i; j++){
					if((strcmp((*(a_string+j)).string, (*(a_string+j+1)).string)) > 0){
						a_tmp = *(a_string+j);
						*(a_string+j) = *(a_string+j+1);
						*(a_string+j+1) = a_tmp;
					}
				}
			}
			if(!strcmp(file_name, "NULL")){ //ȭ�� ����� ��
				for(int i=0; i<total; i++){
					print_a((*(a_string+i)).serial_number);
				}
			}
			else{		//filename ������ ����� ��
				for(int i=0; i<total; i++){
					print_a_file((*(a_string+i)).serial_number, file_name);
				}
			}
		}
		if(!strcmp(option, "s")){ //option ==s
			a_p = root_actor;
			for(int i=0; i<total; i++){
				if(a_p->name == NULL){
					a_p=a_p->next;
					total--;
					i--;
					continue;
				}
				(*(a_string+i)).string = (char *)malloc(sizeof(char)*strlen(a_p->sex)+1);
				strcpy((*(a_string+i)).string, a_p->sex);
				(*(a_string+i)).serial_number = a_p->serial_number;
				a_p=a_p->next;
			}
			for(int i=total-1; i>0; i--){
				for(int j=0; j<i; j++){
					if((strcmp((*(a_string+j)).string, (*(a_string+j+1)).string)) > 0){
						a_tmp = *(a_string+j);
						*(a_string+j) = *(a_string+j+1);
						*(a_string+j+1) = a_tmp;
					}
				}
			}
			if(!strcmp(file_name, "NULL")){ //ȭ�� ����� ��
				for(int i=0; i<total; i++){
					print_a((*(a_string+i)).serial_number);
				}
			}
			else{		//filename ������ ����� ��
				for(int i=0; i<total; i++){
					print_a_file((*(a_string+i)).serial_number, file_name);
				}
			}
		}
		else if(!strcmp(option, "b")){ //option == b (��������� int������ ��)//� ������ ���ĵ�.
			a_p = root_actor;
			a_num = (sorting_num *)malloc(sizeof(sorting_num)*total);
			for(int i=0; i<total; i++){
				if(a_p->name == NULL){
					a_p=a_p->next;
					total--;
					i--;
					continue;
				}
				(*(a_num+i)).num = atoi(a_p->birth);
				(*(a_num+i)).serial_number = a_p->serial_number;
				a_p=a_p->next;
			}
			for(int i=total-1; i>0; i--){
				for(int j=0; j<i; j++){
					if((*(a_num+j)).num > (*(a_num+j+1)).num){
						a_tmp_n = *(a_num+j);
						*(a_num+j) = *(a_num+j+1);
						*(a_num+j+1) = a_tmp_n;
					}
				}
			}
			if(!strcmp(file_name, "NULL")){ //ȭ�� ����� ��
				for(int i=0; i<total; i++){
					print_a((*(a_num+i)).serial_number);
				}
			}
			else{		//filename ������ ����� ��
				for(int i=0; i<total; i++){
					print_a_file((*(a_num+i)).serial_number, file_name);
				}
			}
		}
		else if(!strcmp(option, "m")){ //option == m ù��° ��ȭ �������� ��.
			a_p = root_actor;
			char *token;
			char *st; //�ӽ÷� best_movies�� ��°�� ���� ������
			for(int i=0; i<total; i++){
				if(a_p->name == NULL){
					a_p=a_p->next;
					total--;
					i--;
					continue;
				}
				st = (char *)malloc(sizeof(char)+strlen(a_p->best_movies)+1);
				strcpy(st, a_p->best_movies);
				token = strtok(st, ",");
				(*(a_string+i)).string = (char *)malloc(sizeof(char)*strlen(token)+1);
				strcpy((*(a_string+i)).string, token);
				(*(a_string+i)).serial_number = a_p->serial_number;
				a_p=a_p->next;
			}
			for(int i=total-1; i>0; i--){
				for(int j=0; j<i; j++){
					if((strcmp((*(a_string+j)).string, (*(a_string+j+1)).string)) > 0){
						a_tmp = *(a_string+j);
						*(a_string+j) = *(a_string+j+1);
						*(a_string+j+1) = a_tmp;
					}
				}
			}
			if(!strcmp(file_name, "NULL")){ //ȭ�� ����� ��
				for(int i=0; i<total; i++){
					print_a((*(a_string+i)).serial_number);
				}
			}
			else{		//filename ������ ����� ��
				for(int i=0; i<total; i++){
					print_a_file((*(a_string+i)).serial_number, file_name);
				}
			}
		}
	}

}

int menu_func(char *input) {	//��ɾ� �Է��Ѱ� �����ϴ°�, ���Ŀ� ���� �������� �߰��ϼ���
	char *temp;	//input�޴� �ӽ� ����, input�� �ٲٴ� ���°� �Ͼ�� �ʰ� ����
	char *token;	//��ɾ� �ɰ��� �����ϴ� ��ū
	char *cut;	//��ɾ� �ɰ��� ������ ��� ������
	char *menu;	//��� �κ��� ��� ������(��: update)
	char *factor;	//���� �κ��� ��� ������(��: m, d, a�� �ϳ���)
	char *option;	//�ɼ� �κ��� ��� ������(��: tdy)
	char *string;	//search��ɿ� ���̴� ã�� ���ڿ�
	char *file_name;	//���� �̸��� ��� ������(��: my_list)
	int get_serial_num;	//print�� delete��ɿ� ���̴� ���� ��ȣ
	int i;	//for���� ������ ����, �� for�� ���µ� �����߳� �����Ŵٸ� ��� ���ϴ°� ��! ��! ��! ��!

	temp = (char *)malloc(sizeof(char) * 50);	//temp ���� �Ҵ�
	token = (char *)malloc(sizeof(char) * 50);	//��ū ���� �Ҵ�
	cut = (char *)malloc(sizeof(char) * 10);	//cut ���� �Ҵ�
	strcpy(temp, input);	//temp�� input ����
	cut = " ";	//�������� ��ɾ� �ɰ���

	token = strtok(temp, cut);
	if (token == NULL)	//�ƹ��͵� �Է¾��ϸ� �� �ѱ��
		return 1;
	menu = (char *)malloc(sizeof(char) * strlen(token) + 1);
	strcpy(menu, token);
	printf("menu : %s\n", menu);

	if (!strcmp(menu, "end")) {
		return 0;	//quit_num�� 0���� ����� �����ϱ�
	}
	else if (!strcmp(menu, "add")) {	//add ��ɾ� ó��
		token = strtok(NULL, cut);
		if (token == NULL)	//���ڸ� �Է¾��ϸ� �� �ѱ��
			return 1;
		factor = (char *)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(factor, token);
		printf("factor : %s\n", factor);	//factor Ȯ��

		if (!strcmp(factor, "m"))
			add_movie();
		else if (!strcmp(factor, "d"))
			add_director();
		else if (!strcmp(factor, "a"))
			add_actor();
	}
	else if (!strcmp(menu, "print")) {  //print ��ɾ� ó��, ��Ͽ� ���� �� ����ؾ� �� �� ������.
		token = strtok(NULL, cut);
		factor = (char *)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(factor, token);
		printf("factor : %s\n", factor); //factor Ȯ��

		token = strtok(NULL, cut);
		if(token == NULL){            //serial number ���� ��(���߿� ��������)
			 if (!strcmp(factor, "m")) {
			if(root_m_num == 0){
				printf("no movie.\n\n");
				return 1;
			}
					m = root_movie;
					while (m->next != NULL) {
						printf("%d:%s:%s:%s:%s:%s:%s\n", m->serial_number, m->title, m->genre, m->director, m->year, m->time, m->actors);
						if(m->d_link != NULL){
							printf("%s's birthday : %s\n", m->d_link->name, m->d_link->birth);
						}
						m = m->next;
					}
					printf("\n");
			 }
			 else if (!strcmp(factor, "d")) {
			if(root_d_num == 0){
				printf("no director.\n\n");
				return 1;
			}
					d = root_director;
					while (d->next != NULL) {
						 printf("add:%d:%s:%s:%s:%s\n", d->serial_number, d->name, d->sex, d->birth, d->best_movies);
						 d = d->next;
					}
					printf("\n");
			 }
			 else if (!strcmp(factor, "a")) {
			if(root_a_num == 0){
				printf("no actor.\n\n");
				return 1;
			}
					a = root_actor;
					while (1) {
						if(a->next == NULL)
							break;
						 printf("%d:%s\n", a->serial_number, a->name);
						 a = a->next;
					}
					printf("\n");
			 }
		}
		else{ 				//serial number ���� ��
			 get_serial_num = atoi(token); //���ڿ��� ���ڷ� ��ȯ
			 printf("num : %d\n", get_serial_num);  //get_serial_num Ȯ��
			 if(!strcmp(factor, "m")){
			if(root_m_num == 0){
				printf("no movie.\n\n");
				return 1;
			}
					print_m(get_serial_num);
		}
			 else if(!strcmp(factor, "d")){
			if(root_d_num == 0){
				printf("no director.\n\n");
				return 1;
			}
					print_d(get_serial_num);
		}
			 else if(!strcmp(factor, "a")){
			if(root_a_num == 0){
				printf("no actor.\n\n");
				return 1;
			}
					print_a(get_serial_num);
		}
		}

		}
	else if (!strcmp(menu, "delete")) {	//delete ��ɾ� ó��
		token = strtok(NULL, cut);
		if (token == NULL)	//���ڸ� �Է¾��ϸ� �� �ѱ��
			return 1;
		factor = (char *)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(factor, token);
		printf("factor : %s\n", factor);	//factor Ȯ��

		token = strtok(NULL, cut);
		if (token == NULL)	//�ø��� �ѹ��� �Է¾��ϸ� �� �ѱ��
			return 1;
		get_serial_num = atoi(token);
		printf("num : %d\n", get_serial_num);	//get_serial_num Ȯ��

		if (!strcmp(factor, "m"))
			;	//moive �����ϴ� �Լ�
		else if (!strcmp(factor, "d"))
			;	//director �����ϴ� �Լ�
		else if (!strcmp(factor, "a"))
			;	//actor �����ϴ� �Լ�
	}
	else if (!strcmp(menu, "search")) {	//search ��ɾ� ó��
		token = strtok(NULL, cut);
		if (token == NULL)	//���ڸ� �Է¾��ϸ� �� �ѱ��
			return 1;

		if (*token == '-') {
			option = (char *)malloc(sizeof(char) * strlen(token) + 1);
			strcpy(option, token);
			printf("option : %s\n", option);	//option Ȯ��
			token = strtok(NULL, cut);
		}
		else{
			option = NULL;
		}
		string = (char *)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(string, token);
		printf("string : %s\n", string);	//string Ȯ��
	}
	else if (!strcmp(menu, "update")) {	//update ��ɾ� ó��
		token = strtok(NULL, cut);
		if (token == NULL)	//���ڸ� �Է¾��ϸ� �� �ѱ��
			return 1;
		factor = (char *)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(factor, token);
		printf("factor : %s\n", factor);	//factor Ȯ��

		token = strtok(NULL, cut);
		if (token == NULL)	//�ڿ� �� ������ �� �ѱ��
			return 1;
		if (*token >= '9') {	//���ڰ� �ƴ� ���ĺ��̸� option���� �ֱ�
			option = (char *)malloc(sizeof(char) * strlen(token) + 1);
			strcpy(option, token);
			printf("option : %s\n", option);	//option Ȯ��
			token = strtok(NULL, cut);
			if (token == NULL)	//�ø��� �ѹ� ������ �� �ѱ��
				return 1;
		}
		get_serial_num = atoi(token);
		printf("num : %d\n", get_serial_num);	//get_serial_num Ȯ��
	}
	else if (!strcmp(menu, "sort")) {	//sort ��ɾ� ó��
		token = strtok(NULL, cut);
		factor = (char *)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(factor, token);
		printf("factor : %s\n", factor);	//factor Ȯ��
		if ((token = strtok(NULL, cut)) != NULL) {	//�ڿ� ���� �� �ִ��� Ȯ��
			if (!strcmp(token, "-f")) {	//�ڿ� �ִ°� -f�̸�
				token = strtok(NULL, cut);	//-f �ǳʶٱ�
				file_name = (char *)malloc(sizeof(char) * strlen(token) + 1);
				strcpy(file_name, token);
				printf("file_name : %s\n", file_name);	//file_name Ȯ��
				sort(factor, "NULL", file_name);
			}
			else {	//�ڿ� �ִ°� �ɼ��̸�
				option = (char *)malloc(sizeof(char) * strlen(token) + 1);
				strcpy(option, token);
				printf("option : %s\n", option);	//option Ȯ��

				if ((token = strtok(NULL, cut)) != NULL) {	//�ڿ� ���� �� �ִ��� Ȯ��
					token = strtok(NULL, cut);	//�ִٸ� -f���״� �ǳʶٱ�
					file_name = (char *)malloc(sizeof(char) * strlen(token) + 1);
					strcpy(file_name, token);
					printf("file_name : %s\n", file_name);	//file_name Ȯ��
					sort(factor, option, file_name);
				}
				else{//�ڿ� �ƹ��͵� ���ٸ�
					sort(factor, option, "NULL");
				}
			}
		}
		else{ 			//factor �ڿ� �ƹ��͵� ����.
			sort(factor, "NULL", "NULL");
		}
		printf("\n");
	}
	else if (!strcmp(menu, "save")) {	//save ��ɾ� ó��
		token = strtok(NULL, cut);
		if (token == NULL)	//���ڸ� �Է¾��ϸ� �� �ѱ��
			return 1;
		factor = (char *)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(factor, token);
		printf("factor : %s\n", factor);	//factor Ȯ��

		if ((token = strtok(NULL, cut)) != NULL) {	//�ڿ� ���� �� �ִ��� Ȯ��
			if (!strcmp(token, "-f")) {	//�ڿ� �ִ°� -f�̸�
				token = strtok(NULL, cut);	//-f �ǳʶٱ�
				if (token == NULL)	//�����̸��� �Է¾��ϸ� �� �ѱ��
					return 1;
				file_name = (char *)malloc(sizeof(char) * strlen(token) + 1);
				strcpy(file_name, token);
				printf("file_name : %s\n", file_name);	//file_name Ȯ��
			}
			else {	//�ڿ� �ִ°� �ɼ��̸�
				option = (char *)malloc(sizeof(char) * strlen(token) + 1);
				strcpy(option, token);
				printf("option : %s\n", option);	//option Ȯ��

				if ((token = strtok(NULL, cut)) != NULL) {	//�ڿ� ���� �� �ִ��� Ȯ��
					token = strtok(NULL, cut);	//�ִٸ� -f���״� �ǳʶٱ�
					if (token == NULL)	//���� �̸��� �Է¾��ϸ� �� �ѱ��
						return 1;
					file_name = (char *)malloc(sizeof(char) * strlen(token) + 1);
					strcpy(file_name, token);
					printf("file_name : %s\n", file_name);	//file_name Ȯ��
				}
			}
		}
		printf("\n");
	}

	if (!strcmp(input, "save m")) {	//�ӽ� movie ���̺�
		save_movie();
	}
	else if (!strcmp(input, "save d")) {	//�ӽ� director ���̺�
		save_director();
	}
	else if (!strcmp(input, "save a")) {	//�ӽ� actor ���̺�
		save_actor();
	}

	return 1;
}



int main(void) {
	m = (movie *)malloc(sizeof(movie));	//movie *m ���� ����ü �����Ҵ�
	d = (director *)malloc(sizeof(director));	//director *d ���� ����ü �����Ҵ�
	a = (actor *)malloc(sizeof(actor));	//actor *a ���� ����ü �����Ҵ�
	int quit_num = 1;	//���α׷� ������ ����
	char *input_words;
	input_words = (char *)malloc(sizeof(char) * 50);

	printf(">> Welcome to My Movie <<\n");
	printf("File Loading.....\n");
	load_movie();
	load_director();
	load_actor();
	link_director();
	printf("You can use add, update, delete, search, sort, save, end commands.\n");
	signal(SIGINT, handler);	//Ctrl + c�� �������� �ٷ� ������� �ʰ� �����

	while (quit_num) {
		printf("(movie) ");
		gets(input_words);
		quit_num = menu_func(input_words);
	}

	return 0;
}

