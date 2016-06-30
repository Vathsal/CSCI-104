#ifndef TWEET_H
#define TWEET_H

#include <iostream>
#include <string>
#include <string>
#include <vector>
#include <set>
#include "datetime.h"

using namespace std;

/* Forward declaration */
class User;

/* Models a tweet and provide comparison and output operators */
class Tweet
{
public:
	/* Default constructor */
	Tweet();

	/* Constructor */
	Tweet(User* user, DateTime& time, std::string& text);

	/* Gets the timestamp of this tweet
	   @return timestamp of the tweet */
	DateTime const & time() const;

	/* Gets the actual text of this tweet
	   @return text of the tweet */
	string const & text() const;

	/* Returns the hashtagged words without the '#' sign */
	set<string> hashTags() const;

	/* Return true if this Tweet's timestamp is less-than other's
	   @return result of less-than comparison of tweet's timestamp */
	bool operator<(const Tweet& other) const;

	/* Outputs the tweet to the given ostream in format:
	   YYYY-MM-DD HH::MM::SS username tweet_text
	   @return the ostream passed in as an argument */
	friend ostream& operator<<(ostream& os, const Tweet& t);

	/* Create any other public or private helper functions you deem necessary */
	User* user() const;

	/* Other member functions */
	void populateHashTags(string& text);

private:
	/* Other data members or helper functions */
	User* user_;
	DateTime dt_;
	string text_;
	set<string> hashTagsSet_;
};

struct TweetComp
{
	bool operator()(Tweet* t1, Tweet* t2)
	{
		return (*t2 < *t1);
	}
};

#endif
