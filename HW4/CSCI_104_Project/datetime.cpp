#include "datetime.h"

using namespace std;

DateTime::DateTime() 
{
	hour_ = 0;
	min_ = 0;
	sec_ = 0;
	year_ = 0;
	month_ = 0;
	day_ = 0;
}

DateTime::DateTime(int hh, int mm, int ss, int yr, int mn, int dy) 
{
	hour_ = hh;
	min_ = mm;
	sec_ = ss;
	year_ = yr;
	month_ = mn;
	day_ = dy;
}

bool DateTime::operator<(const DateTime& other) const 
{
	if (this->year_ != other.year_)
		return this->year_ < other.year_;
	if (this->month_ != other.month_)
		return this->month_ < other.month_;
	if (this->day_ != other.day_)
		return this->day_ < other.day_;
	if (this->hour_ != other.hour_)
		return this->hour_ < other.hour_;
	if (this->min_ != other.min_)
		return this->min_ < other.min_;
	if (this->sec_ != other.sec_)
		return this->sec_ < other.sec_;
}

ostream& operator<<(std::ostream& os, const DateTime& other) 
{
	return os << other.year_ << "-" << other.month_ << "-" << other.day_ << " " << other.hour_ << "::" << other.min_ << "::" << other.sec_;
}

istream& operator >> (std::istream& is, DateTime& dt) 
{
	string phrase;
	string partOfPhrase;

	getline(is, phrase);
	stringstream ss(phrase);

	getline(ss, partOfPhrase, '-');
	dt.year_ = stoi(partOfPhrase);
	getline(ss, partOfPhrase, '-');
	dt.month_ = stoi(partOfPhrase);
	getline(ss, partOfPhrase, ' ');
	dt.day_ = stoi(partOfPhrase);
	getline(ss, partOfPhrase, ':');
	dt.hour_ = stoi(partOfPhrase);
	getline(ss, partOfPhrase, ':');
	dt.min_= stoi(partOfPhrase);
	getline(ss, partOfPhrase, ' ');
	dt.sec_ = stoi(partOfPhrase);

	return is;
}