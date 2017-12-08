// Date.cpp : 定义控制台应用程序的入口点。
//

#include "CDate.h"
#include <string>

#include <iostream>
#include <fstream>

using	namespace	std;

#define	Out_put(v)	do{cout<<(#v##"==\t")<<(v)<<'\n';}while(0)
#define	Num_of_array(a)	(sizeof(a)/sizeof((a)[0]))

char const *	g_weekdays[7] =
{
	"星期日",
	"星期一",
	"星期二",
	"星期三",
	"星期四",
	"星期五",
	"星期六",
};
char const *	g_week_sign[7]=
{
	"日",
	"一",
	"二",
	"三",
	"四",
	"五",
	"六",
};
ostream&	operator<<(ostream& o,const CDate& date)
{
	if(date.get_year()>0)
		o<<"公元"<<date.get_year()<<"年";
	else if(date.get_year()<=0)
	{
		o<<"公元前"<<(1-date.get_year())<<"年";
	}
	o<<(date.get_month()+1)<<"月"<<(date.get_day()+1)<<"日 "<<g_weekdays[(int)date.weekday()];
	return	o;
}

void	print_date_month(ostream& out,const CDate& d)
{
	CDate	date = d;
	for(int iter=0;iter<8*7;++iter)
		out<<'-';
	out<<'\n';
	out<<"\t\t\t"<<(date.get_month()+1)<<"月\n";
	for(int iter=0;iter<8*7;++iter)
		out<<'=';
	out<<'\n';
	for(int iter=0;iter<Num_of_array(g_week_sign);++iter)
		out<<g_week_sign[iter]<<'\t';
	out<<'\n';
	date.set_day(0);
	int	wd = date.weekday();
	for(int iter=0;iter<wd;++iter)
		out<<'\t';
	for(int iter=0;iter<CDate::month_days(date.is_leap_year(),date.get_month());++iter)
	{
		date.set_day(iter);
		if(iter!=0&&date.weekday()%7==0)
			out<<'\n';
		out<<(date.get_day()+1)<<'\t';
	}
	out<<'\n';
	for(int iter=0;iter<8*7;++iter)
		out<<'=';
	out<<'\n';
}

int main(int argc, char * argv[])
{
	CDate	date,result;
	int	year,month,day;
	string	date_file_name;
/*	
	int od;
	for(;;)
	{
		cout<<"输入日期:";
		cin>>year>>month>>day;
		date.set(year,month-1,day-1);
		cout<<"输入偏移:";
		cin>>od;
		result = date + od;
		cout<<date<<(od>=0?" + ":" ")<<od<<"天 == "<<result<<'\n';
		cout<<"验算:(result - date)=="<<(result-date)<<'\n';
	}
	*/
	for(;;)
	{
		cout<<"日历输出文件:";
		cin>>date_file_name;

		cout<<"输入年份:";
		cin>>year;
		date.set(year,0,0);

		fstream	file(date_file_name.c_str(),ios_base::in|ios_base::out|ios_base::trunc);
		if(!file)
			cout<<"没有文件\n";
		else
		{
			if(date.get_year()>0)
				file<<"公元"<<date.get_year()<<"年\n";
			else
				file<<"公元前"<<(1-date.get_year())<<"年\n";
			for(int m=0;m<12;++m)
			{
				date.set_month(m);
				print_date_month(file,date);
				print_date_month(cout,date);
			}
			cout<<"成功建立日历文件.\n";
		}
	}
	return 0;
}

