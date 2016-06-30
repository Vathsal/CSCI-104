#include <string>
#include <set>
#include <list>
#include <vector>
#include <algorithm>
#include <iterator>
#include "user.h"
#include "tweet.h"

using namespace std;

User::User(string name)
{
	name_ = name;
}

User::~User()
{
	// ??
}

std::string User::name() const
{
	return this->name_;
}

std::set<User*> User::followers() const
{
	return this->followersSet_;
}

std::set<User*> User::following() const
{
	return this->followingSet_;
}

std::list<Tweet*> User::tweets() const
{
	return this->tweetList_;
}

void User::addFollower(User* u)
{
	this->followersSet_.insert(u);
}

void User::addFollowing(User* u)
{
	this->followingSet_.insert(u);
}

void User::addTweet(Tweet* t)
{
	this->tweetList_.push_back(t);
}

void User::addAtMentionTweet(Tweet* t)
{
	this->atMentionTweetList_.push_back(t);
}

std::vector<Tweet*> User::getFeed()
{
	// create a vector of tweets 
	vector<Tweet*> allTweets;

	// retrive all tweets of this user
	list<Tweet*> utl = this->tweets();
	// push the tweets into allTweets
	for (list<Tweet*>::iterator it1 = utl.begin(); it1 != utl.end(); ++it1)
	{
		allTweets.push_back(*it1);
	}

	// 3. Get all the users that this user follows
	set<User*> fs = this->following();
	for (set<User*>::iterator it2 = fs.begin(); it2 != fs.end(); ++it2)
	{
		// retrive the list of tweets for this user
		list<Tweet*> ftl = (*it2)->tweets();
		// push the tweets into allTweets
		for (list<Tweet*>::iterator it3 = ftl.begin(); it3 != ftl.end(); ++it3)
		{
			allTweets.push_back(*it3);
		}
	}

	return allTweets;
}
