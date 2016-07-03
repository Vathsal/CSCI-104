#include <string>
#include <set>
#include <list>
#include <vector>
#include <algorithm>
#include <iterator>
#include "user.h"
#include "tweet.h"
#include "twiteng.h"

using namespace std;

User::User(string name)
{
	name_ = name;
}

User::~User()
{
	// ??
}

string User::name() const
{
	return this->name_;
}

set<User*> User::followers() const
{
	return this->followersSet_;
}

set<User*> User::following() const
{
	return this->followingSet_;
}

list<Tweet*> User::tweets() const
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

vector<Tweet*> User::getFeed(map<string, User*> &uMap)
{
	string firstWordOfTweet;
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
			// ---- @ MENTION -------
			//extract the first word from tweet
			stringstream ss((*it3)->text());
			ss >> firstWordOfTweet;
			//check if its starts with '@'
			if (firstWordOfTweet.substr(0, 1) == "@")
			{
				firstWordOfTweet = firstWordOfTweet.substr(1, firstWordOfTweet.length());
				// retrieve the userObject for that @mention user
				User* atMentionUser_obj = uMap.find(firstWordOfTweet)->second;
				// access his following list
				set<User*> temp2 = this->following();
				set<User*>::const_iterator temp3 = temp2.find(atMentionUser_obj);
				if (temp3 != temp2.end())
				{
					allTweets.push_back(*it3);
				}
			}
			else
			{
				allTweets.push_back(*it3);
			}
			
		//	allTweets.push_back(*it3);
		}
	}

	return allTweets;
}

list<Tweet*> User::getAtMentionTweet() const
{
	return this->atMentionTweetList_;
}