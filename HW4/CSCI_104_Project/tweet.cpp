#include <iostream>
#include <string>
#include <string>
#include <vector>
#include <set>
#include "user.h"
#include "datetime.h"
#include "tweet.h"

using namespace std;

Tweet::Tweet()
{
	user_ = new User("DummyUser");

	dt_.year_ = 0;
	dt_.month_ = 0;
	dt_.day_ = 0;
	dt_.hour_ = 0; 
	dt_.min_ = 0; 
	dt_.sec_ = 0;

	text_ = "";
}

Tweet::Tweet(User* user, DateTime& time, string& text)
{
	user_ = user;
	dt_ = time;
	text_ = text;
}

DateTime const& Tweet::time() const
{
	return this->dt_;
}

string const & Tweet::text() const
{
	return this->text_;
}

set<string> Tweet::hashTags() const
{
	return this->hashTagsSet_;
}

User* Tweet::user() const
{
	return this->user_;
}

bool Tweet::operator<(const Tweet& other) const
{
	if (this->dt_.year_ != other.dt_.year_)
		return this->dt_.year_ < other.dt_.year_;
	if (this->dt_.month_ != other.dt_.month_)
		return this->dt_.month_ < other.dt_.month_;
	if (this->dt_.day_ != other.dt_.day_)
		return this->dt_.day_ < other.dt_.day_;
	if (this->dt_.hour_ != other.dt_.hour_)
		return this->dt_.hour_ < other.dt_.hour_;
	if (this->dt_.min_ != other.dt_.min_)
		return this->dt_.min_ < other.dt_.min_;
	if (this->dt_.sec_ != other.dt_.sec_)
		return this->dt_.sec_ < other.dt_.sec_;
}

ostream& operator<<(ostream& os, const Tweet& t)
{
	return os << t.dt_.year_ << "-" << t.dt_.month_ << "-" << t.dt_.day_ << " " << t.dt_.hour_ << ":" << t.dt_.min_ << ":" << t.dt_.sec_;
}

void Tweet::populateHashTags(string& text)
{
	// put the 'text' into a string stream
	stringstream ss(text);
	string oneWord;

	// extract each word
	while (ss >> oneWord)
	{
		// check if its a hashTag
		if (oneWord.substr(0, 1) == "#")
		{
			// removed the "#" for the hashTag
			oneWord = oneWord.substr(1, oneWord.length());
			hashTagsSet_.insert(oneWord);
		}
	}
}

