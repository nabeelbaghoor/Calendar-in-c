#include<iostream>
#include<string.h>
#include<conio.h>
#include<windows.h>
#include<fstream>
#include<ios>      
#include<limits>  
#include<sstream>
using namespace std;
struct activity {
	char *title;
	char *userid;
	int duration;
	float priority;
};
int  orig_width, orig_height;
bool break_flag = 0;
int option_x = 0, option_y = 0, options_key = 0;
void setnull(activity *****&calendar, int count[12][31][24]);
bool clash_check(int s1, int s2, int e1, int e2);
void options(activity*****&calendar, int count[12][31][24]);
bool isCursorKeyPressed(int& whickKey);
bool isCursorKeyPressednew2(int& whickKey);
void myRect(int x1, int y1, int x2, int y2, int R, int G, int B, int BR, int BG, int BB);
void GetDesktopResolution(int& horizontal, int& vertical);
void gotoxy(int x, int y);
void menu_print();
void expand_new(float *&prio, int z);
void msg();
void print_s(activity*****calendar, int count[12][31][24], int i, int j, int k, int l);
void expand(activity *****&calendar, int count[12][31][24], int i, int j, int k);
void read_s(activity *****&calendar, int count[12][31][24]);
void add_activity(activity *****&calendar, int count[12][31][24], int start, int day, int month, int duration_s, char userid_s[], char actid_s[], char title_s[], float priority_s);
void print_month(activity *****&calendar, int count[12][31][24]);
void save(activity *****&calendar, int count[12][31][24]);
void remove_user(activity *****&calendar, int count[12][31][24]);
void calendar_stats_year(activity *****&calendar, int count[12][31][24]);
void activity_stats_month(activity *****&calendar, int count[12][31][24]);
void free_slots(activity *****&calendar, int count[12][31][24]);
void clashes(activity *****&calendar, int count[12][31][24]);
void vocation(activity *****&calendar, int count[12][31][24]);
void five_most_imp(activity *****&calendar, int count[12][31][24]);
void all_activities(activity *****&calendar, int count[12][31][24]);
void remove_scrollbar();
void setFontSize(int FontSize);
int main()
{
	GetDesktopResolution(orig_width, orig_height);
	menu_print();
	int len;
	activity *****calendar = nullptr;
	int count[12][31][24];
	for (int i = 0; i < 12; i++)
		for (int j = 0; j < 31; j++)
			for (int k = 0; k < 24; k++)
				count[i][j][k] = 0;
	calendar = new activity****[12];//months
	for (int i = 0; i < 12; i++)//days
	{
		if (i + 1 == 1 || i + 1 == 3 || i + 1 == 5 || i + 1 == 7 || i + 1 == 8 || i + 1 == 10 || i + 1 == 12)
			len = 31;
		else if (i + 1 == 2)
			len = 28;
		else
			len = 30;
		calendar[i] = new activity***[len];
		for (int j = 0; j < len; j++)//hours
		{
			calendar[i][j] = new activity**[24];
		}
	}
	setnull(calendar, count);
	read_s(calendar, count);
	options(calendar, count);
	if (option_y == 2)
		all_activities(calendar, count);
	else if (option_y == 4)
		five_most_imp(calendar, count);
	else if (option_y == 6)
		vocation(calendar, count);
	else if (option_y == 8)
		clashes(calendar, count);
	else if (option_y == 10)
		free_slots(calendar, count);
	else if (option_y == 12)
		activity_stats_month(calendar, count);
	else if (option_y == 14)
		calendar_stats_year(calendar, count);
	else if (option_y == 16)
		remove_user(calendar, count);
	else if (option_y == 18)
		save(calendar, count);
	else if (option_y == 20)
		print_month(calendar, count);
	//deallocation
	for (int i = 0; i<12; i++)
	{
		if (i + 1 == 1 || i + 1 == 3 || i + 1 == 5 || i + 1 == 7 || i + 1 == 8 || i + 1 == 10 || i + 1 == 12)
			len = 31;
		else if (i + 1 == 2)
			len = 28;
		else
			len = 30;
		for (int j = 0; j<len; j++)
		{
			for (int k = 0; k<24; k++)
			{
				for (int l = 0; l<count[i][j][k]; l++)
				{
					if (calendar[i][j][k][l] != nullptr)
					{
						delete[]calendar[i][j][k][l]->userid;
						delete[]calendar[i][j][k][l]->title;
						calendar[i][j][k][l]->userid = nullptr;
						calendar[i][j][k][l]->title = nullptr;
						delete[]calendar[i][j][k][l];
						calendar[i][j][k][l] = nullptr;
					}
				}
				delete[]calendar[i][j][k];
				calendar[i][j][k] = nullptr;
			}

			delete[]calendar[i][j];
			calendar[i][j] = nullptr;
		}

		delete[]calendar[i];
		calendar[i] = nullptr;
	}
	delete[]calendar;
	calendar = nullptr;
	system("pause");
}
void read_s(activity *****&calendar, int count[12][31][24])
{
	char junk;
	char userid_s[10], actid_s[20], title_s[40];
	int start, end, day, month, duration_s;
	float priority_s;
	ifstream fin;
	fin.open("calendar2.dat");
	if (fin.is_open())
	{
		while (!fin.eof())
		{
			fin >> day >> junk >> month >> junk >> start >> junk >> end >> junk;
			fin.getline(userid_s, 10, ',');
			fin.getline(actid_s, 20, ',');
			fin.getline(title_s, 40, ',');
			fin >> priority_s;
			duration_s = end - start;
			add_activity(calendar, count, start, day, month, duration_s, userid_s, actid_s, title_s, priority_s);
		}
	}
	else
		cout << "currupted\n";
}
void add_activity(activity *****&calendar, int count[12][31][24], int start, int day, int month, int duration_s, char userid_s[], char actid_s[], char title_s[], float priority_s)
{
	int len, p = 0;
	bool flag = 1;
	for (int i = 0; i<12 && flag; i++)
	{
		if (i + 1 == 1 || i + 1 == 3 || i + 1 == 5 || i + 1 == 7 || i + 1 == 8 || i + 1 == 10 || i + 1 == 12)
			len = 31;
		else if (i + 1 == 2)
			len = 28;
		else
			len = 30;
		for (int j = 0; j<len && flag; j++)
		{
			for (int k = 0; k<24 && flag; k++)
			{
				expand(calendar, count, i, j, k);
				for (int l = 0; l<count[i][j][k] + 1 && flag; l++)
				{
					if (i == month - 1 && j == day - 1 && k == start - 1 && l == count[i][j][k])
					{
						calendar[i][j][k][l] = new activity;
						calendar[i][j][k][l]->title = new char[40];
						calendar[i][j][k][l]->userid = new char[10];
#pragma warning(disable : 4996)
						strcpy(calendar[i][j][k][l]->title, title_s);
						strcpy(calendar[i][j][k][l]->userid, userid_s);
						calendar[i][j][k][l]->duration = duration_s;
						calendar[i][j][k][l]->priority = priority_s;
						count[i][j][k]++;
						//print_s(calendar, count, i, j, k, l);
						flag = 0;
					}
				}
			}
		}
	}
}
void print_s(activity*****calendar, int count[12][31][24], int i, int j, int k, int l)
{
	cout << j + 1 << "/" << i + 1 << "," << k + 1 << "," << k + 1 + calendar[i][j][k][l]->duration << "," << calendar[i][j][k][l]->userid << "," << calendar[i][j][k][l]->title << "," << calendar[i][j][k][l]->priority << endl;
}
void expand(activity *****&calendar, int count[12][31][24], int i, int j, int k)
{

	if (count[i][j][k] == 0)
	{
		calendar[i][j][k] = new activity*[count[i][j][k] + 1];
	}
	else
	{
		activity**temp = nullptr;
		temp = new activity*[count[i][j][k] + 1];
		for (int p = 0; p < count[i][j][k] + 1; p++)
			temp[p] = calendar[i][j][k][p];
		delete[]calendar[i][j][k];
		calendar[i][j][k] = temp;
		temp = nullptr;
		delete[]temp;
	}
}
//10
void print_month(activity *****&calendar, int count[12][31][24])//with calendar ui
{
	cout << "Following function would Print a month of the calendar";
	int month, len;
	bool flag = 1;
	msg();
	cout << "enter month:\t";
	cin >> month;
	if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
		len = 31;
	else if (month == 2)
		len = 28;
	else
		len = 30;
	setFontSize(36);
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);
	remove_scrollbar();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");
	for (int j = 0; j<31; j++)
	{
		flag = 0;
		for (int k = 0; k<24 && flag == 0; k++)
		{
			if (count[month - 1][j][k] != 0)
				flag = 1;
		}
		if (flag == 1)
		{
			SetConsoleTextAttribute(hConsole, 15);//15//7
												  //system("color 0F");//F//7
			cout << j + 1 << "\t";//white//activity present
		}
		if (flag == 0)
		{
			SetConsoleTextAttribute(hConsole, 14);//14//6
												  //system("color 0E");//E//6
			cout << j + 1 << "\t";//yellow//n o activities
		}
	}
	cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;

}
//9
void save(activity *****&calendar, int count[12][31][24])///i think we would have to rewrite or update the file........
{
	cout << "Following function would Save the calendar\n";
	ofstream fout;
	fout.open("calendar2.dat");
	if (fout.is_open())
	{
		int len, act = 0;
		string actid;
		for (int i = 0; i < 12; i++)
		{
			if (i + 1 == 1 || i + 1 == 3 || i + 1 == 5 || i + 1 == 7 || i + 1 == 8 || i + 1 == 10 || i + 1 == 12)
				len = 31;
			else if (i + 1 == 2)
				len = 28;
			else
				len = 30;
			for (int j = 0; j < len; j++)
			{
				for (int k = 0; k < 24; k++)
				{
					for (int l = 0; l < count[i][j][k]; l++)
					{
						if (calendar[i][j][k][l] != nullptr)
						{
							actid = "act";
							string str = to_string(act);
							actid = actid + str;
							fout << j + 1 << "/" << i + 1 << "," << k + 1 << "," << k + 1 + calendar[i][j][k][l]->duration << "," << calendar[i][j][k][l]->userid << "," << actid << "," << calendar[i][j][k][l]->title << "," << calendar[i][j][k][l]->priority << endl;
							act++;
						}
					}
				}
			}
		}
		cout << "FILE HAS BEEN SAVED\n";
	}
	else
		cout << "corrupted\n";

}
//8
void remove_user(activity *****&calendar, int count[12][31][24])
{
	cout << "Following function would Remove a user from the calendar (remove all activities of this user)\n";
	char userid_s[10];
	msg();
	cout << "enter userid:\n";
	gets_s(userid_s);
	int len;
	for (int i = 0; i<12; i++)
	{
		if (i + 1 == 1 || i + 1 == 3 || i + 1 == 5 || i + 1 == 7 || i + 1 == 8 || i + 1 == 10 || i + 1 == 12)
			len = 31;
		else if (i + 1 == 2)
			len = 28;
		else
			len = 30;
		for (int j = 0; j<len; j++)
		{
			for (int k = 0; k<24; k++)
			{
				for (int l = 0; l<count[i][j][k]; l++)
				{
					if (calendar[i][j][k][l] != nullptr)
					{
						if (!strcmp(calendar[i][j][k][l]->userid, userid_s))
						{
							delete[]calendar[i][j][k][l]->userid;
							delete[]calendar[i][j][k][l]->title;
							calendar[i][j][k][l]->userid = nullptr;
							calendar[i][j][k][l]->title = nullptr;
							delete[]calendar[i][j][k][l];
							calendar[i][j][k][l] = nullptr;
						}
					}

				}

			}
		}
	}

}
//7
void calendar_stats_year(activity *****&calendar, int count[12][31][24])//for whole year..
{

	cout << "Following function would Print Calendar Stats (for the whole year)\n";
	int total_act = 0;
	int len, act_month[12];
	for (int i = 0; i < 12; i++)
	{
		if (i + 1 == 1 || i + 1 == 3 || i + 1 == 5 || i + 1 == 7 || i + 1 == 8 || i + 1 == 10 || i + 1 == 12)
			len = 31;
		else if (i + 1 == 2)
			len = 28;
		else
			len = 30;
		act_month[i] = 0;
		for (int j = 0; j < len; j++)
		{
			for (int k = 0; k < 24; k++)
			{
				if (count[i][j][k])
					total_act += count[i][j][k];
				act_month[i] += count[i][j][k];
			}
		}
	}
	int max = act_month[0], max_index = 0;
	for (int a = 1; a < 12; a++)
	{
		if (max < act_month[a])
		{
			max = act_month[a];
			max_index = a;
		}
	}
	cout << "\nTotal activities in year:\t" << total_act;
	cout << "\nAverage number of activities per month:\t" << (float)total_act / 12;
	cout << "\nBusiest month of year:\t" << max_index + 1;//mb -1
	cout << "\nNumber of activities in busiest month:\t" << act_month[max_index];
	cout << endl;

}
//6
void activity_stats_month(activity *****&calendar, int count[12][31][24])//for a given month....
{
	cout << "Following function would Print activity stats for a given month\n";
	int month, total_act = 0, busiest_day_act = 0;
	float avg_act = 0, avg_priority = 0;
	msg();
	cout << "enter month:\t";
	cin >> month;
	int len;
	if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
		len = 31;
	else if (month == 2)
		len = 28;
	else
		len = 30;
	float *sum_prio, *avg_prio;
	sum_prio = new float[len];
	avg_prio = new float[len];
	int *act_day = new int[len];
	for (int j = 0; j < len; j++)
	{
		act_day[j] = 0;
		sum_prio[j] = 0;
		avg_prio[j] = 0;
		for (int k = 0; k < 24; k++)
		{
			total_act += count[month - 1][j][k];
			act_day[j] += count[month - 1][j][k];
			for (int l = 0; l < count[month - 1][j][k]; l++)
			{
				if (calendar[month - 1][j][k][l] != nullptr)
				{
					sum_prio[j] += calendar[month - 1][j][k][l]->priority;
				}
			}
		}
		avg_prio[j] = sum_prio[j] / act_day[j];
	}
	int max = act_day[0], max_index = 0;
	for (int a = 1; a < len; a++)
	{
		if (max < act_day[a])
		{
			max = act_day[a];
			max_index = a;
		}
	}
	for (int i = 0; i < 24; i++)
		busiest_day_act += count[month - 1][max_index][i];
	float max_avg_prio = avg_prio[0];
	int max_index_prio = 0;
	for (int a = 1; a < len; a++)
	{
		if (max_avg_prio < avg_prio[a])
		{
			max_avg_prio = avg_prio[a];
			max_index_prio = a;
		}
	}
	cout << "\nTotal activities in month:\t" << total_act;
	cout << "\nAverage number of activities per day:\t" << (float)total_act / len;
	cout << "\nBusiest day of month:\t" << max_index + 1;//mb -1
	cout << "\nNumber of activities in busiest day:\t" << busiest_day_act;
	cout << "\nDay with highest average priority of activities(most important day):\t" << max_index_prio + 1;//mb -1
	cout << "\nNumber of activities in the most important day:\t" << act_day[max_index_prio];//mb no+1  //-1
	cout << endl;
}
//5
void free_slots(activity *****&calendar, int count[12][31][24])
{
	cout << "Folowing function would list all hourly slots that are free for all the entered users in entered time period :\n";
	bool flag = 0;
	int size = 0, start_day, start_month, end_day, end_month;
	cout << "Enter the number of userid's which you want to enter:\n";
	cin >> size;
	char **userid_s = new char*[size];
	for (int i = 0; i < size; i++)
		userid_s[i] = new char[10];
	int *free = new int[size];
	msg();
	for (int i = 0; i < size; i++)
	{
		cout << "enter userid:\n";
		cin>>userid_s[i];
	}
	cout << "enter start date\n";
	cout << "enter day:\t";
	cin >> start_day;
	cout << "enter month:\t";
	cin >> start_month;
	cout << "enter end date\n";
	cout << "enter day:\t";
	cin >> end_day;
	cout << "enter month:\t";
	cin >> end_month;
	int len;
	bool freeall = 1,flag2=0;
	cout << "All of the entered users are free at :\n";
	for (int i = 0; i<12; i++)
	{
		if (i + 1 == 1 || i + 1 == 3 || i + 1 == 5 || i + 1 == 7 || i + 1 == 8 || i + 1 == 10 || i + 1 == 12)
			len = 31;
		else if (i + 1 == 2)
			len = 28;
		else
			len = 30;
		for (int j = 0; j<len; j++)
		{
			for (int k = 0; k<24; k++)
			{
				freeall = 1;
				for (int q = 0; q<size; q++)
					free[q] = 1;
				for (int l = 0; l<count[i][j][k]; l++)
				{
					if (calendar[i][j][k][l] != nullptr)
					{
						if ((i >= start_month - 1 && i <= end_month - 1))
						{
							if (start_month - 1 != end_month - 1)
							{
								if (i == start_month - 1 && j <= len - 1 && j >= start_day - 1)
									flag = 1;
								else if (i == end_month - 1 && j >= 0 && j <= end_day - 1)
									flag = 1;
								else if (i > start_month - 1 && i < end_month - 1)
									flag = 1;
							}
							else
							{
								if (j <= end_day - 1 && j >= start_day - 1)
									flag = 1;
							}

						}
						if (flag)
						{
							for (int q = 0; q < size; q++)
							{
								if (!strcmp(calendar[i][j][k][l]->userid, userid_s[q]))
									free[q] = 0;
							}
							flag = 0;
						}
					}
				}
				if ((i >= start_month - 1 && i <= end_month - 1))
				{
					if (start_month - 1 != end_month - 1)
					{
						if (i == start_month - 1 && j <= len - 1 && j >= start_day - 1)
							flag2 = 1;
						else if (i == end_month - 1 && j >= 0 && j <= end_day - 1)
							flag2 = 1;
						else if (i > start_month - 1 && i < end_month - 1)
							flag2 = 1;
					}
					else
					{
						if (j <= end_day - 1 && j >= start_day - 1)
							flag2 = 1;
					}

				}
				if (flag2)
				{
					for (int p = 0; p < size; p++)
						if (free[i] == 0)
							freeall = 0;
					if (freeall)
					{
						cout << "Date:\t" << j + 1 << "/" << i + 1 << "\tTime:\t" << k + 1 << ":00" << endl;
					}
					flag2 = 0;
				}
			}
		}
	}

}
//4
void clashes(activity *****&calendar, int count[12][31][24])//bt start and end time
{
	cout << "Following function would List all the clashing activities of a pair of users, during a time period\n";
	int user = 0;
	bool flag = 0, off = 0;
	int start_day, start_month, end_day, end_month, s1, e1, s2, e2, count1, count2,hour1,hour2;
	char userid_1[10] = { 0 }, userid_2[10] = { 0 };
	msg();
	cout << "enter userid of user 1:\n";
	gets_s(userid_2);
	cout << "enter userid of user 2:\n";
	gets_s(userid_1);
	cout << "enter start date\n";
	cout << "enter day:\t";
	cin >> start_day;
	cout << "enter month:\t";
	cin >> start_month;
	cout << "enter end date\n";
	cout << "enter day:\t";
	cin >> end_day;
	cout << "enter month:\t";
	cin >> end_month;
	int len;
	bool user1 = 0, user2 = 0;
	for (int i = 0; i<12; i++)
	{
		if (i + 1 == 1 || i + 1 == 3 || i + 1 == 5 || i + 1 == 7 || i + 1 == 8 || i + 1 == 10 || i + 1 == 12)
			len = 31;
		else if (i + 1 == 2)
			len = 28;
		else
			len = 30;
		for (int j = 0; j<len; j++)
		{
			user1 = 0, user2 = 0;
			for (int k = 0; k<24; k++)
			{
				for (int l = 0; l<count[i][j][k]; l++)
				{
					if (calendar[i][j][k][l] != nullptr)
					{
						if ((i >= start_month - 1 && i <= end_month - 1))
						{
							if (start_month - 1 != end_month - 1)
							{
								if (i == start_month - 1 && j <= len - 1 && j >= start_day - 1)
									flag = 1;
								else if (i == end_month - 1 && j >= 0 && j <= end_day - 1)
									flag = 1;
								else if (i > start_month - 1 && i < end_month - 1)
									flag = 1;
							}
							else
							{
								if (j <= end_day - 1 && j >= start_day - 1)
									flag = 1;
							}

						}
						if (flag)
						{
							if (!strcmp(calendar[i][j][k][l]->userid, userid_1))
							{
								user1 = 1;
								count1 = l;
								hour1 = k;
								s1 = k + 1;//-1
								e1 = s1 + calendar[i][j][k][l]->duration;
							}
							if (!strcmp(calendar[i][j][k][l]->userid, userid_2))
							{

								user2 = 1;
								hour2 = k;
								count2 = l;
								s2 = k + 1;//-1
								e2 = s2 + calendar[i][j][k][l]->duration;
							}
							flag = 0;
						}
					}
				}
			}
			if (user1 && user2)
			{
				if (clash_check(s1, s2, e1, e2))
				{
					print_s(calendar, count,i, j, hour1, count1);
					print_s(calendar, count, i, j, hour2, count2);
				}
			}
		}
	}
}
bool clash_check(int s1, int s2, int e1, int e2)
{
	bool flag = 0;
	if((s2<=s1 && e2>=e1) || (s2>=s1 && e2<=e1) || (s2<=s1 && e2>=s1 && e2<=e1) || (s2>=s1 && s2<=e1 && e2>=e1))
		flag = 1;
	return flag;
}
//3
void vocation(activity *****&calendar, int count[12][31][24])//longest free period.........should be named
{
	cout << "Following function would print the longest free period; that is, the longest consecutive number of days in which the user has no activity\n";
	int voc[1000] = { 0 };
	int p = 0;
	int start_day[1000] = { 0 }, start_month[1000] = { 0 }, end_day[1000] = { 0 }, end_month[1000] = { 0 };
	bool user = 0, freep = 0, freeday = 1, one_time = false;
	char userid_s[10] = { 0 };
	msg();
	cout << "enter userid:\n";
	gets_s(userid_s);
	int len, i, j, k, l, length;
	for (i = 0; i < 12; i++)
	{
		if (i + 1 == 1 || i + 1 == 3 || i + 1 == 5 || i + 1 == 7 || i + 1 == 8 || i + 1 == 10 || i + 1 == 12)
			len = 31;
		else if (i + 1 == 2)
			len = 28;
		else
			len = 30;
		for (j = 0; j < len; j++)
		{
			freeday = 1;
			user = 0;
			for (k = 0; k < 24 && freeday && user == 0; k++)
			{
				if (count[i][j][k] != 0)
				{
					for (l = 0; l < count[i][j][k] && freeday && user == 0; l++)
					{
						if (calendar[i][j][k][l] != nullptr)
						{
							if (!strcmp(calendar[i][j][k][l]->userid, userid_s))
							{
								freeday = 0;
								user = 1;
							}
						}
					}
				}
			}
			if (freeday && one_time)
			{
				start_day[p] = j + 1;
				start_month[p] = i + 1;
				one_time = false;
			}
			if (freeday)
				voc[p]++;
			else if (user)
			{
				end_day[p] = (j + 1) - 1;
				end_month[p] = (i + 1);
				if (end_day[p] == 0)
				{
					end_month[p] --;
					if (end_month[p] - 1 == 1 || end_month[p] - 1 == 3 || end_month[p] - 1 == 5 || end_month[p] - 1 == 7 || end_month[p] - 1 == 8 || end_month[p] - 1 == 10 || end_month[p] - 1 == 12)
						length = 31;
					else if (end_month[p] - 1 == 2)
						length = 28;
					else
						length = 30;
					end_day[p] = length;
				}
				one_time = true;
				p++;

			}
		}

	}
	if (freeday)
	{
		end_day[p] = j;
		end_month[p] = i;
	}
	int max = voc[0], max_index = 0;
	for (int a = 1; a < p + 1; a++)
	{
		if (max < voc[a])
		{
			max = voc[a];
			max_index = a;
		}
	}
	cout << "\nnumber of days:\t" << max << "\nstart date:\t" << start_day[max_index] << " / " << start_month[max_index] << "\nend date:\t" << end_day[max_index] << " / " << end_month[max_index];
	cout << endl;
}
//2
void five_most_imp(activity *****&calendar, int count[12][31][24])//during  a time
{

	cout << "Folowing function would List the 5 most important activities of a given user during a time\n";
	float *prio = new float[1];
	int z = 0;
	bool flag = 0, off = 0;
	int start_day, start_month, end_day, end_month;
	char userid_s[10] = { 0 };
	msg();
	cout << "enter userid:\n";
	gets_s(userid_s);
	cout << "enter start date\n";
	cout << "enter day:\t";
	cin >> start_day;
	cout << "enter month:\t";
	cin >> start_month;
	cout << "enter end date\n";
	cout << "enter day:\t";
	cin >> end_day;
	cout << "enter month:\t";
	cin >> end_month;
	int len;
	for (int i = 0; i < 12; i++)
	{
		if (i + 1 == 1 || i + 1 == 3 || i + 1 == 5 || i + 1 == 7 || i + 1 == 8 || i + 1 == 10 || i + 1 == 12)
			len = 31;
		else if (i + 1 == 2)
			len = 28;
		else
			len = 30;
		for (int j = 0; j < len; j++)
		{
			for (int k = 0; k < 24; k++)
			{
				for (int l = 0; l < count[i][j][k]; l++)
				{
					if (calendar[i][j][k][l] != nullptr)
					{
						if ((i >= start_month - 1 && i <= end_month - 1) && !strcmp(calendar[i][j][k][l]->userid, userid_s))
						{
							if (start_month - 1 != end_month - 1)
							{
								if (i == start_month - 1 && j <= len - 1 && j >= start_day - 1)
									flag = 1;
								else if (i == end_month - 1 && j >= 0 && j <= end_day - 1)
									flag = 1;
								else if (i > start_month - 1 && i < end_month - 1)
									flag = 1;
							}
							else
							{
								if (j <= end_day - 1 && j >= start_day - 1)
									flag = 1;
							}

						}
						if (flag && z != 0)
							expand_new(prio, z + 1);
						if (flag)
						{
							prio[z] = calendar[i][j][k][l]->priority;
							z++;
							flag = 0;
						}
					}
				}
			}
		}
	}
	for (int a = 0; a < z; a++)
		for (int b = a + 1; b < z; b++)
			if (prio[a] < prio[b])
				swap(prio[a], prio[b]);
	flag = 0;
	bool flag2 = 1;
	for (int p = 0; p<5;)
	{
		flag2 = 1;
		for (int i = 0; i < 12 && flag2; i++)
		{
			if (i + 1 == 1 || i + 1 == 3 || i + 1 == 5 || i + 1 == 7 || i + 1 == 8 || i + 1 == 10 || i + 1 == 12)
				len = 31;
			else if (i + 1 == 2)
				len = 28;
			else
				len = 30;
			for (int j = 0; j < len && flag2; j++)
			{
				for (int k = 0; k < 24 && flag2; k++)
				{
					for (int l = 0; l < count[i][j][k] && flag2; l++)
					{
						if (calendar[i][j][k][l] != nullptr)
						{
							if ((i >= start_month - 1 && i <= end_month - 1) && !strcmp(calendar[i][j][k][l]->userid, userid_s) && calendar[i][j][k][l]->priority == prio[p])
							{
								if (start_month - 1 != end_month - 1)
								{
									if (i == start_month - 1 && j <= len - 1 && j >= start_day - 1)
										flag = 1;
									else if (i == end_month - 1 && j >= 0 && j <= end_day - 1)
										flag = 1;
									else if (i > start_month - 1 && i < end_month - 1)
										flag = 1;
								}
								else
								{
									if (j <= end_day - 1 && j >= start_day - 1)
										flag = 1;
								}

							}
							if (flag)
							{
								print_s(calendar, count, i, j, k, l);
								flag = 0;
								p++;
								flag2 = 0;

							}
						}


					}
				}
			}
		}
	}
}
void expand_new(float *&prio, int z)
{
	float*temp = nullptr;
	temp = new float[z];
	for (int i = 0; i < z; i++)
		temp[i] = prio[i];
	delete[]prio;
	prio = temp;
	temp = nullptr;
	delete[]temp;
}
//1
void all_activities(activity *****&calendar, int count[12][31][24])//during a time
{
	cout << "Folowing function would List all activities of a given user during a time period:\n";
	bool flag = 0;
	int start_day, start_month, end_day, end_month;
	char userid_s[10] = { 0 };
	msg();
	cout << "enter userid:\n";
	gets_s(userid_s);
	cout << "enter start date\n";
	cout << "enter day:\t";
	cin >> start_day;
	cout << "enter month:\t";
	cin >> start_month;
	cout << "enter end date\n";
	cout << "enter day:\t";
	cin >> end_day;
	cout << "enter month:\t";
	cin >> end_month;
	int len;
	for (int i = 0; i<12; i++)
	{
		if (i + 1 == 1 || i + 1 == 3 || i + 1 == 5 || i + 1 == 7 || i + 1 == 8 || i + 1 == 10 || i + 1 == 12)
			len = 31;
		else if (i + 1 == 2)
			len = 28;
		else
			len = 30;
		for (int j = 0; j<len; j++)
		{
			for (int k = 0; k<24; k++)
			{
				for (int l = 0; l<count[i][j][k]; l++)
				{
					if (calendar[i][j][k][l] != nullptr)
					{
						if ((i >= start_month - 1 && i <= end_month-1) && !strcmp(calendar[i][j][k][l]->userid, userid_s))
						{
							if (start_month-1 != end_month-1)
							{
								if (i == start_month - 1 && j <= len - 1 && j >= start_day - 1)
									flag = 1;
								else if (i == end_month - 1 && j >= 0 && j <= end_day - 1)
									flag = 1;
								else if (i > start_month - 1 && i < end_month - 1)
									flag = 1;
							}
							else
							{
								if (j <= end_day-1 && j >= start_day - 1)
									flag = 1;
							}

						}
						if (flag) {
							print_s(calendar, count, i, j, k, l);
							flag = 0;
						}
					}
				}
			}
		}
	}
}
void msg()
{
	cout << "enter the following\n";
}
void remove_scrollbar()
{
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hstdout, &csbi);

	csbi.dwSize.X = csbi.dwMaximumWindowSize.X;
	csbi.dwSize.Y = csbi.dwMaximumWindowSize.Y;
	SetConsoleScreenBufferSize(hstdout, csbi.dwSize);

	HWND x_x = GetConsoleWindow();
	ShowScrollBar(x_x, SB_BOTH, FALSE);
}
void setFontSize(int FontSize)
{
	CONSOLE_FONT_INFOEX info = { 0 };
	info.cbSize = sizeof(info);
	info.dwFontSize.Y = FontSize; // leave X as zero
	info.FontWeight = FW_NORMAL;
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
}
void menu_print()
{
	setFontSize(36);
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);
	//remove_scrollbar();
	myRect(0, 0, orig_width, orig_height, 255, 255, 255, 255, 255, 255);
	gotoxy(23, 10);
	cout << "WELCOME TO NULL STUDIOS";
	_getch();
	system("cls");
	//myRect(0, 0, orig_width, orig_height, 255, 255, 255, 255, 255, 255);
	gotoxy(23, 10);
	cout << "Reading File...............";
}
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

}
void myRect(int x1, int y1, int x2, int y2, int R, int G, int B, int BR, int BG, int BB)
{
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);

	//change the color by changing the values in RGB (from 0-255)
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
	SelectObject(device_context, pen);
	HBRUSH brush = ::CreateSolidBrush(RGB(BR, BG, BB)); //Fill color is passed as parameter to the function!!!

	SelectObject(device_context, brush);

	Rectangle(device_context, x1, y1, x2, y2);
	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);
	DeleteObject(brush);
}
void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}
void options(activity*****&calendar, int count[12][31][24])
{
	setFontSize(34);
	system("cls");
	option_x = 23;
	system("color F0");
	myRect(0, 0, orig_width, orig_height, 255, 255, 255, 255, 255, 255);
	gotoxy(option_x, 2);
	cout << "      ACTIVITIES";
	gotoxy(option_x, 4);
	cout << "MOST IMPORTANT ACTIVITIES";
	gotoxy(option_x, 6);
	cout << "   LONGEST FREE PERIOD";
	gotoxy(option_x, 8);
	cout << "   CLASHING ACTIVITIES";
	gotoxy(option_x, 10);
	cout << "      FREE SLOTS";
	gotoxy(option_x, 12);
	cout << "    ACTIVITY STATS";
	gotoxy(option_x, 14);
	cout << "    CALENDAR STATS";
	gotoxy(option_x, 16);
	cout << "     REMOVE USER";
	gotoxy(option_x, 18);
	cout << "    SAVE CALENDAR";
	gotoxy(option_x, 20);
	cout << "     PRINT MONTH";
	option_y = 2;
	//gotoxy(option_x, 6);
	//	option_y = 6;
	int junk;
	isCursorKeyPressed(junk);
	while (!break_flag)
	{

		isCursorKeyPressednew2(options_key);
		switch (options_key)
		{
		case 2://up
			if (option_y >= 4)
				option_y -= 2;
			break;
		case 1://enter
			break_flag = 1;
			break;
		case 3://backspace
			break;
		case 4://down
			if (option_y <= 20)
				option_y += 2;
			break;
		case 5://escape
			break_flag = 1;
			break;
		}

		options_key = 0;
		gotoxy(option_x, option_y);
	}
	setFontSize(30);
	system("color 0F");
	system("cls");
	myRect(0, 0, orig_width, orig_height, 0, 0, 0, 0, 0, 0);
	system("pause");
	system("cls");
}
bool isCursorKeyPressed(int& whickKey)   //whichKey passed as reference.... 
{
	char key;
	key = GetAsyncKeyState(37);
	if (key == 1)
	{
		whickKey = 1;		// 1 if left key is pressed 
		return true;
	}
	key = GetAsyncKeyState(38);
	if (key == 1)
	{

		whickKey = 2;		// 2 if up key is pressed
		return true;
	}

	key = GetAsyncKeyState(39);
	if (key == 1)
	{

		whickKey = 3; // 3 if right key is pressed
		return true;
	}
	key = GetAsyncKeyState(40);
	if (key == 1)
	{

		whickKey = 4;   // 4 if down key is pressed
		return true;
	}
	key = GetAsyncKeyState(13);
	if (key == 1)
	{

		whickKey = 5;   // 4 if down key is pressed
		return true;
	}
	return false;
}
bool isCursorKeyPressednew2(int& whickKey)   //whichKey passed as reference.... 
{
	char key;
	key = GetAsyncKeyState(13);
	if (key == 1)
	{
		whickKey = 1;		// 1 if enter key is pressed 
		return true;
	}
	key = GetAsyncKeyState(38);
	if (key == 1)
	{

		whickKey = 2;		// 2 if up key is pressed
		return true;
	}

	key = GetAsyncKeyState(8);
	if (key == 1)
	{

		whickKey = 3; // 3 if backspace is pressed
		return true;
	}
	key = GetAsyncKeyState(40);
	if (key == 1)
	{

		whickKey = 4;   // 4 if down key is pressed
		return true;
	}
	key = GetAsyncKeyState(27);
	if (key == 1)
	{

		whickKey = 5;   // 5 if escape is pressed
		return true;
	}
	return false;
}
void setnull(activity *****&calendar, int count[12][31][24])
{
	int len;
	for (int i = 0; i < 12; i++)
	{
		if (i + 1 == 1 || i + 1 == 3 || i + 1 == 5 || i + 1 == 7 || i + 1 == 8 || i + 1 == 10 || i + 1 == 12)
			len = 31;
		else if (i + 1 == 2)
			len = 28;
		else
			len = 30;
		for (int j = 0; j < len; j++)
		{
			for (int k = 0; k < 24; k++)
			{

				calendar[i][j][k] = nullptr;

			}
		}
	}
}