#ifndef	_CDATE_H_
class CDate
{
public:
	enum	EWeekDay
	{
		Sunday = 0,
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday
	};
	static	bool	is_leap_year(int year);
	static	int	diff(const CDate& left,const CDate& right);			//从日期left到日期right之间相隔的天数，如果日期right比日期left早，则返回负数；
	static	bool	offset(const CDate& d,int days,CDate* result);	//计算从日期d开始，偏移days天数之后的日期，保存在result中，days为正数表示向后偏移，为负数表示向前偏移；
	static	int	compare(const CDate& left,const CDate& right);		//比较日期left和right，如果left比right早返回负数，left比right晚返回正数，相同返回0；
	static	EWeekDay	weekday(const CDate& date);					//计算date的星期
	static	int	month_days(bool is_leap_year,int month)	{return	s_month_days[is_leap_year][month%12];}
private:
	static	int	align_left(int year,int align);
	static	int	align_right(int year,int align);
	static	int	calc_align_dist(int year_left,int year_right,int align);
	static	int	calc_leap_days(int year_left,int year_right);
	static	bool	calc_month(int days,bool is_leap_year,bool after,int * result_month,int * result_day);
public:
	CDate():_year(2009),_month(1),_day(0){}
	CDate(int year,int month,int day):_year(year),_month(month),_day(day)
	{
		adjust();
	}
	CDate(const CDate& date){*this = date;	adjust();}
	bool	is_leap_year()	const	{return	is_leap_year(this->_year);}
	bool	offset(int days)
	{
		CDate temp = *this;
		return	offset(temp,days,this);
	}
	int	get_year()	const	{return	_year;}
	int	get_month()	const	{return	_month;}
	int	get_day()	const	{return	_day;}
	void	adjust();
	void	set_year(int year)	{_year = year;	adjust();}
	void	set_month(int month)	{_month = month;	adjust();}
	void	set_day(int day)	{_day = day;	adjust();}
	void	set(int year,int month,int day)	{_year=year;_month=month;_day=day;	adjust();}
	void	set(const CDate&	date)	{*this = date;	adjust();}
	EWeekDay	weekday()	const	{return	weekday(*this);}

	CDate&	operator-=(int days);
	CDate&	operator+=(int days);
	CDate&	operator++();
	CDate	operator++(int);
	CDate&	operator--();
	CDate	operator--(int);
private:
	int	_year;	//为方便计算，年份的负数和零表示公元前，0表示公元前1年，-1表示公元前2年
	int	_month;	//为方便计算，月份以0为开始，0表示1月，1表示2月
	int	_day;	//为方便计算，日期以0为开始，0表示1号，1表示2号
private:
	static	int	const	s_month_days[2][12];		//每个月份的天数
	static	int	const	s_month_days_before[2][12];	//每个月份之前的天数
	static	int	const	s_month_days_after[2][12];	//每个月份之后的天数
};

bool	operator>(const CDate& left,const CDate& right);
bool	operator>=(const CDate& left,const CDate& right);
bool	operator<(const CDate& left,const CDate& right);
bool	operator<=(const CDate& left,const CDate& right);
bool	operator==(const CDate& left,const CDate& right);
bool	operator!=(const CDate& left,const CDate& right);
int		operator-(const CDate& left,const CDate& right);
CDate	operator-(const CDate& date,int days);
CDate	operator+(const CDate& date,int days);
CDate	operator+(int days,const CDate& date);

#endif
