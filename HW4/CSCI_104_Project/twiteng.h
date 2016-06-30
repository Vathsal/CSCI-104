#ifndef TWITENG_H
#define TWITENG_H

#include <map>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include "user.h"
#include "datetime.h"

class TwitEng
{
public:
	TwitEng();
	~TwitEng();
	/**
	* Parses the Twitter database and populates internal structures
	* @param filename of the database file
	* @return true if there is an error, false if successful
	*/
	bool parse(char* filename);

	/**
	* Allocates a tweet from its parts and adds it to internal structures
	* @param username of the user who made the tweet
	* @param timestamp of the tweet
	* @param text is the actual text of the tweet as a single string
	*/
	void addTweet(string& username, DateTime& time, string& text);

	/**
	* Searches for tweets with the given words and strategy
	* @param words is the hashtag terms in any case without the '#'
	* @param strategy 0=AND, 1=OR
	* @return the tweets that match the search
	*/
	set<Tweet*> search(vector<string>& terms, int strategy);

	/* Dump feeds of each user to their own file */
	void dumpFeeds();

	/* You may add other member functions */
	struct Comparator
	{
		bool operator() (const string &s1, const string &s2) const
		{
			string str1(s1.length(), ' ');
			string str2(s2.length(), ' ');
			transform(s1.begin(), s1.end(), str1.begin(), ::tolower);
			transform(s2.begin(), s2.end(), str2.begin(), ::tolower);
			return str1 < str2;
		}
	};

	void addNewTweet(string& myStr);

private:
	/* Add any other data members or helper functions here  */
	map<string, User*> userMap;
	// create a map to hold the hashtags and the associated tweets
	map<string, set<Tweet*>, Comparator> hashTagMap;
};

#endif
