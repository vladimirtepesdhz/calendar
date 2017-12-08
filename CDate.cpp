#include "CDate.h"

int	const	CDate::s_month_days[2][12]=
{
	{
		31,
		28,
		31,
		30,
		31,
		30,
		31,
		31,
		30,
		31,
		30,
		31
	}
	,
	//leap year
	{
		31,
		29,
		31,
		30,
		31,
		30,
		31,
		31,
		30,
		31,
		30,
		31,
	}
};

int	const	CDate::s_month_days_before[2][12]=
{
	{
		0,
		31,
		31+28,
		31+28+31,
		31+28+31+30,
		31+28+31+30+31,
		31+28+31+30+31+30,
		31+28+31+30+31+30+31,
		31+28+31+30+31+30+31+31,
		31+28+31+30+31+30+31+31+30,
		31+28+31+30+31+30+31+31+30+31,
		31+28+31+30+31+30+31+31+30+31+30
	}
	,
	//leap year
	{
		0,
		31,
		31+29,
		31+29+31,
		31+29+31+30,
		31+29+31+30+31,
		31+29+31+30+31+30,
		31+29+31+30+31+30+31,
		31+29+31+30+31+30+31+31,
		31+29+31+30+31+30+31+31+30,
		31+29+31+30+31+30+31+31+30+31,
		31+29+31+30+31+30+31+31+30+31+30
	}
};

int	const	CDate::s_month_days_after[2][12]=
{
	{
		28+31+30+31+30+31+31+30+31+30+31,
		31+30+31+30+31+31+30+31+30+31,
		30+31+30+31+31+30+31+30+31,
		31+30+31+31+30+31+30+31,
		30+31+31+30+31+30+31,
		31+31+30+31+30+31,
		31+30+31+30+31,
		30+31+30+31,
		31+30+31,
		30+31,
		31,
		0
	}
	,
	//leap year
	{
		29+31+30+31+30+31+31+30+31+30+31,
		31+30+31+30+31+31+30+31+30+31,
		30+31+30+31+31+30+31+30+31,
		31+30+31+31+30+31+30+31,
		30+31+31+30+31+30+31,
		31+31+30+31+30+31,
		31+30+31+30+31,
		30+31+30+31,
		31+30+31,
		30+31,
		31,
		0
	}
};

bool	CDate::is_leap_year(int year)
{
	return	((year%4==0)&&(year%100!=0))||((year%100==0)&&(year%400==0));
}

int	CDate::compare(const CDate& left,const CDate& right)
{
	if(left.get_year() != right.get_year())
		return	left.get_year() - right.get_year();
	if(left.get_month() != right.get_month())
		return	left.get_month() - right.get_month();
	return	left.get_day() - right.get_day();
}

void	CDate::adjust()
{
	if(_month < 0)
		_month = 0;
	if(_month >= 12)
		_month = 11;
	if(_day < 0)
		_day = 0;
	if(_day >= s_month_days[(int)(is_leap_year())][_month])
		_day = s_month_days[(int)(is_leap_year())][_month] -1;
}

int	CDate::align_left(int year,int align)
{
	if(year%align == 0)
		return	year;
	if(year>=0)
	{
		return	year - year%align;
	}
	else
	{
		year=-year;
		return	-(year + (align - year%align));
	}
}

int	CDate::align_right(int year,int align)
{
	if(year%align == 0)
		return	year;
	if(year>=0)
	{
		return	year + (align - year%align);
	}
	else
	{
		year = -year;
		return	-(year - year%align);
	}
}

int	CDate::calc_align_dist(int year_left,int year_right,int align)
{
	year_left = align_right(year_left,align);
	year_right = align_right(year_right,align);
	if(year_left>year_right)
		return	0;
	return	(year_right - year_left)/align;
}

//从年份year_left到年份year_right之间所有的闰天数目(其实也就是闰年数目)，不包括year_right在内
int	CDate::calc_leap_days(int year_left,int year_right)
{
	return	calc_align_dist(year_left,year_right,4)-
			calc_align_dist(year_left,year_right,100)+
			calc_align_dist(year_left,year_right,400);
}

//根据天数days的偏移计算日期
bool	CDate::calc_month(int days,bool is_leap_year,bool after,int * result_month,int * result_day)
{
	if(result_month==0||result_day==0)
		return	false;
	if(after)
		days = (is_leap_year?366:365) - days;
	for(int m=0;m<12;++m)
	{
		if(s_month_days_before[is_leap_year][m]+s_month_days[is_leap_year][m]>days)
		{
			*result_month = m;
			*result_day = days - s_month_days_before[is_leap_year][*result_month];
			return	true;
		}
	}
	return	false;
}

//计算日期l到日期r之间间隔的天数，日期r比日期l早的话就返回负数；
int	CDate::diff(const CDate &l, const CDate &r)
{
	CDate left(l),right(r),temp;
	if(l>r)
	{
		temp = left;
		left = right;
		right = temp;
	}
	int	rd_left =	s_month_days_after[(int)(left.is_leap_year())][left.get_month()] +
					s_month_days[(int)(left.is_leap_year())][left.get_month()] - left.get_day();
	int	rd_right=	s_month_days_before[(int)(right.is_leap_year())][right.get_month()] + right.get_day();
	int	rd_total = 0;
	if(left.get_year() == right.get_year())
	{
		rd_total = rd_left + rd_right - (left.is_leap_year()?366:365);
	}
	else
	{
		rd_total = (right.get_year() - (left.get_year() + 1))*365 + calc_leap_days(left.get_year()+1,right.get_year());
		rd_total += rd_left + rd_right;
	}
	return	(l>r)?(-rd_total):rd_total;
}

//有了计算日期天数间隔的函数diff，计算星期就简单多了，只要知道随便一个日期的星期，
//然后以此日期为标尺ruler，用diff函数求出间隔的天数，再求余，就得到了
CDate::EWeekDay	CDate::weekday(const CDate& date)
{
	CDate	ruler(2009,1,0);	//比如，我们知道2009年2月1日是星期日，就以此为标尺；其实其它日期也可以，比如2008年6月1日也可以；
	int diff_days = 0;
	if(date>=ruler)	//当前日期在ruler日期之后
	{
		diff_days = diff(ruler,date);	//求出当前日期到标尺日期之间的天数
		return	(CDate::EWeekDay)(diff_days%7);	//再求余就得到了星期
	}
	else	//当前日期在ruler日期之前
	{
		diff_days = diff(date,ruler);	//求出当前日期到标尺日期之间的天数
		return	(CDate::EWeekDay)(6-(diff_days-1)%7);	//再求余就得到了星期
	}
}

bool	CDate::offset(const CDate &d, int days, CDate *result)
{
	static	int	const	s_level_align_year[3] = {400,100,4};
	static	int const	s_level_align_days[3] = {146097,36525,1461};

	int	cur_level = 0;
	if(result == 0)
		return	false;

	*result = d;
	if(days>0)
	{
		int	rd_days_before = s_month_days_before[(int)(result->is_leap_year())][result->get_month()] + result->get_day();
		int	rd_days_after = (result->is_leap_year()?366:365)-rd_days_before;
		if(rd_days_after>days)
		{
			int	month,day;
			if(!calc_month(rd_days_before+days,result->is_leap_year(),false,&month,&day))
				return	false;
			result->set(result->get_year(),month,day);
		}
		else
		{
			days -= rd_days_after;
			result->set(result->get_year()+1,0,0);
			for(int iter=0;iter<sizeof(s_level_align_year)/sizeof(s_level_align_year[0]);++iter)
			{
				int	ay = align_right(result->get_year(),s_level_align_year[iter]);
				int diff_days = diff(*result,CDate(ay,0,0));
				if(days >= diff_days)
				{
					days -= diff_days;
					result->set(ay,0,0);
					break;
				}
			}
			//400
			if(days >= s_level_align_days[0])
			{
				int step = days/s_level_align_days[0];
				days %= s_level_align_days[0];
				result->set(result->get_year()+step*400,0,0);
			}
			//100,4
			for(int iter=1;iter<sizeof(s_level_align_days)/sizeof(s_level_align_days[0]);++iter)
			{
				for(;;)
				{
					int align_days = s_level_align_days[iter] - (int)(!result->is_leap_year());
					if(days<align_days)
						break;
					days -= align_days;
					result->set_year(result->get_year()+s_level_align_year[iter]);
				}
			}
			while(days>(result->is_leap_year()?366:365))
			{
				days -= (result->is_leap_year()?366:365);
				result->set_year(result->get_year()+1);
			}
			int	month,day;
			if(!calc_month(days,result->is_leap_year(),false,&month,&day))
				return	false;
			result->set(result->get_year(),month,day);
		}
	}
	else if(days<0)
	{
		days=-days;
		int	rd_days_before = s_month_days_before[(int)(result->is_leap_year())][result->get_month()] + result->get_day();
		int	rd_days_after = (result->is_leap_year()?366:365)-rd_days_before;
		if(rd_days_before>=days)
		{
			int	month,day;
			if(!calc_month(rd_days_before-days,result->is_leap_year(),false,&month,&day))
				return	false;
			result->set(result->get_year(),month,day);
		}
		else
		{
			days -= rd_days_before;
			result->set(result->get_year(),0,0);
			for(int iter=0;iter<sizeof(s_level_align_year)/sizeof(s_level_align_year[0]);++iter)
			{
				int	ay = align_left(result->get_year(),s_level_align_year[iter]);
				int diff_days = diff(CDate(ay,0,0),*result);
				if(days >= diff_days)
				{
					days -= diff_days;
					result->set(ay,0,0);
					break;
				}
			}
			//400
			if(days >= s_level_align_days[0])
			{
				int step = days/s_level_align_days[0];
				days %= s_level_align_days[0];
				result->set(result->get_year()-step*400,0,0);
			}
			for(int iter=1;iter<sizeof(s_level_align_days)/sizeof(s_level_align_days[0]);++iter)
			{
				for(;;)
				{
					int align_days = s_level_align_days[iter] - (int)(!is_leap_year(result->get_year()-s_level_align_year[iter]));
					if(days<align_days)
						break;
					days -= align_days;
					result->set_year(result->get_year()-s_level_align_year[iter]);
				}
			}
			while(days>(is_leap_year(result->get_year()-1)?366:365))
			{
				days -= (is_leap_year(result->get_year()-1)?366:365);
				result->set_year(result->get_year()-1);
			}
			if(days)
			{
				int	month,day;
				result->set_year(result->get_year()-1);
				if(!calc_month(days,result->is_leap_year(),true,&month,&day))
					return	false;
				result->set(result->get_year(),month,day);
			}
		}
	}
	return	true;
}

bool	operator>(const CDate& left,const CDate& right)
{
	return	CDate::compare(left,right)>0;
}
bool	operator>=(const CDate& left,const CDate& right)
{
	return	CDate::compare(left,right)>=0;
}
bool	operator<(const CDate& left,const CDate& right)
{
	return	CDate::compare(left,right)<0;
}
bool	operator<=(const CDate& left,const CDate& right)
{
	return	CDate::compare(left,right)<=0;
}
bool	operator==(const CDate& left,const CDate& right)
{
	return	CDate::compare(left,right)==0;
}
bool	operator!=(const CDate& left,const CDate& right)
{
	return	CDate::compare(left,right)!=0;
}

int		operator-(const CDate& left,const CDate& right)
{
	return	CDate::diff(right,left);
}

CDate	operator-(const CDate& date,int days)
{
	CDate	result;
	CDate::offset(date,-days,&result);
	return	result;
}
CDate	operator+(const CDate& date,int days)
{
	CDate	result;
	CDate::offset(date,days,&result);
	return	result;
}
CDate	operator+(int days,const CDate& date)
{
	CDate	result;
	CDate::offset(date,days,&result);
	return	result;
}

CDate&	CDate::operator -=(int days)
{
	offset(-days);
	return	*this;
}
CDate&	CDate::operator+=(int days)
{
	offset(days);
	return	*this;
}
CDate&	CDate::operator++()
{
	offset(1);
	return	*this;
}
CDate	CDate::operator++(int)
{
	CDate	temp = *this;
	offset(1);
	return	temp;
}
CDate&	CDate::operator--()
{
	offset(-1);
	return	*this;
}
CDate	CDate::operator--(int)
{
	CDate	temp = *this;
	offset(-1);
	return	temp;
}

