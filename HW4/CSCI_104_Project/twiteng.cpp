#include <map>
#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <ctime>
#include "ctype.h"
#include "user.h"
#include "tweet.h"
#include "datetime.h"
#include "twiteng.h"

using namespace std;

TwitEng::TwitEng()
{
	// ??
}

TwitEng::~TwitEng()
{
	// ??
}

bool is_number(const string &s)
{
	string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it))
	{
		++it;
	}
	return !s.empty() && it == s.end();
}

bool TwitEng::parse(char* filename)
{
	// 1. read 1st line - number of users (4)
	// 2. create 4 "user" objects : user1 , user2, user3, user4
	//			 4 "tweet" objects : tweet1 ,tweet2, tweet3 ,tweet4
	//    - for each user extract:
	//			1. name 
	//			2. followers 
	//			3. following 
	// 3. 5th line is first tweet
	//    - for each line extract:
	//			1. timestamp
	//		    2. name of the person who has tweeted this 
	//			3. text ( tweet itself)
	//          4. hashtags

	string oneLine;
	string oneWord;
	string partOfPhrase;
	string currentUser_name;
	int numUsers = 0;
	bool isFirstWord = true;
	DateTime dt;

	ifstream inFile(filename);

	// -- 1. Get number of users -- 
	getline(inFile, oneLine); // extract 1st line 
	if (is_number(oneLine))
	{
		numUsers = stoi(oneLine);
	}
	else
	{
		cout << " Error : Invalid first line !" << endl;
		return false;
	}

	// -- 2. 4 lines of users -- 
	for (int i = 0; i < numUsers; i++)
	{
		getline(inFile, oneLine); // extract 1 line 
		stringstream ss(oneLine);
		isFirstWord = true; // reset to true everytime it reads a new line 
		while (ss >> oneWord) // extract a word 
		{
			if (isFirstWord)
			// Its first word
			{
				// 1. Set isFirst word to false 
				isFirstWord = false; 
				// 2. Update the name of current user 
				currentUser_name = oneWord;
				// 3. Search if user is already presetn in the "usermap"
				//    if     present : Do nothing !
				//    if not present : add username and object to "userMap"
				if ((userMap.find(oneWord) == userMap.end()))
				{
					// create new userObject 
					User* newUser = new User(oneWord);
					// add the user and userObject to the userMap
					userMap[oneWord] = newUser;
				}
			}
			else
			// Its not first word
			{
				if (!(userMap.find(oneWord) == userMap.end()))
				// user already present in userMap 
				{
					// add this userObject to the "following" list of current user 
					User* currentUser_obj = userMap.find(currentUser_name)->second;
					User* followingUser_obj = userMap.find(oneWord)->second;
					currentUser_obj->addFollowing(followingUser_obj);

					// add this currrentUserObject to the "follower" list of Following user 
					followingUser_obj->addFollower(currentUser_obj);
				}
				else
				// user not present in userMap
				{
					// create new userObject 
					User* newUser = new User(oneWord);
					// add the user and userObject to the userMap
					userMap[oneWord] = newUser; // userMap.insert(pair<string, User*>(oneWord ,newUser)); 

					// add this userObject to the "following" list of current user 
					User* currentUser_obj = userMap.find(currentUser_name)->second;
					User* followingUser_obj = userMap.find(oneWord)->second;
					currentUser_obj->addFollowing(followingUser_obj);

					// add this currrentUserObject to the "follower" list of Following user 
					followingUser_obj->addFollower(currentUser_obj);
				}
			}
		}// end of while 
	}// end of for 

	// -- 3. 'x' Lines of tweet -- //
	while (getline(inFile, oneLine))
	{
		// 1. Parse the date and time 
		// 2. Parse the name of the user who is tweeting 
		// 3. Parse the text and add to the User's tweet list 

		stringstream ss(oneLine);

		// 1 Parse the date and time 
		getline(ss, partOfPhrase, '-');
		if (is_number(partOfPhrase))
			dt.year_ = stoi(partOfPhrase);
		else
			return false;
		getline(ss, partOfPhrase, '-');
		if (is_number(partOfPhrase))
			dt.month_ = stoi(partOfPhrase);
		else
			return false;
		getline(ss, partOfPhrase, ' ');
		if (is_number(partOfPhrase))
			dt.day_ = stoi(partOfPhrase);
		else
			return false;
		getline(ss, partOfPhrase, ':');
		if (is_number(partOfPhrase))
			dt.hour_ = stoi(partOfPhrase);
		else
			return false;
		getline(ss, partOfPhrase, ':');
		if (is_number(partOfPhrase))
			dt.min_ = stoi(partOfPhrase);
		else
			return false;
		getline(ss, partOfPhrase, ' ');
		if (is_number(partOfPhrase))
			dt.sec_ = stoi(partOfPhrase);
		else
			return false;

		// 2. Parse the user
		getline(ss, oneWord, ' ');
		string currentTweetUserName = oneWord;

		// 3. Parse the text
		getline(ss, oneWord, '\n');
		string currentTweet = oneWord;
		// add this tweet to the list of the user 
		addTweet(currentTweetUserName, dt, currentTweet);
	}

	return true; // default return
}

void TwitEng::addTweet(string& username, DateTime& time, string& text)
{
	if (userMap.find(username) == userMap.end())
	// user NOT present in userMap 
	{
		// create new userObject 
		User* newUser = new User(username);
		// add the user and userObject to the userMap
		userMap[username] = newUser; // userMap.insert(pair<string, User*>(oneWord ,newUser)); 
	}
	// ----- user present / NOT present in userMap : do the below operations -----
	// retrieve the userObject for that user
	User* currentTweetUser_obj = userMap.find(username)->second;
	// wrap this text in a tweet object
	Tweet* currentTweet_obj = new Tweet(currentTweetUser_obj, time, text);
	// add it to the list of tweets
	currentTweetUser_obj->addTweet(currentTweet_obj);

	// populate the hashTags for this tweet
	currentTweet_obj->populateHashTags(text);
	// retrieve all the hashTags for this tweet
	set<string> ht = currentTweet_obj->hashTags();
	// insert these hashTags into the hashTagMap
	for (set<string>::iterator it3 = ht.begin(); it3 != ht.end(); ++it3)
	{
		if (hashTagMap.find(*it3) == hashTagMap.end())
			// if hashTag is not present in the hashTagMap - add hashTag + add tweet
			hashTagMap[*it3].insert(currentTweet_obj);
		else
			// if hashTag is present in the hashTagMap - find hashTag + add tweet
			hashTagMap.find(*it3)->second.insert(currentTweet_obj);
	}
}

void TwitEng::addNewTweet(string& myStr)
{
	string oneWord;
	DateTime dt;

	stringstream ss(myStr);

	// 1. Parse the user
	ss >> oneWord;// ignore the first word 'TWEET'
	ss >> oneWord;
	string currentTweetUserName = oneWord;

	//2. Query for system date and time 
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	dt.year_ = timeinfo->tm_year + 1900;
	dt.month_ = timeinfo->tm_mon + 1;
	dt.day_ = timeinfo->tm_mday;
	dt.hour_ = timeinfo->tm_hour;
	dt.min_ = timeinfo->tm_min;
	dt.sec_ = timeinfo->tm_sec;

	// 3. Parse the text
	getline(ss, oneWord, '\n');
	string currentTweet = oneWord;
	// add this tweet to the list of the user 
	addTweet(currentTweetUserName, dt, currentTweet);
}

set<Tweet*> TwitEng::search(vector<string>& terms, int strategy)
{
	// create a new set to hold the tweets 
	set<Tweet*> mySet;
	set<Tweet*> set1;
	bool isFirst = true;
	int count = 0;

	if (strategy == 0) // AND
	{
		// for all the 'terms'
		for (vector<string>::iterator it1 = terms.begin(); it1 != terms.end(); ++it1)
		{
			// check if its present in the hashTagMap
			if (!(hashTagMap.find(*it1) == hashTagMap.end()))
			{
				count++;
				if (isFirst)// Set1
				{
					isFirst = false;
					for (set<Tweet*>::iterator it2 = hashTagMap.find(*it1)->second.begin(); it2 != hashTagMap.find(*it1)->second.end(); ++it2)
					{
						set1.insert(*it2);
					}
				}
				else //Set2
				{
					set_intersection(set1.begin(), set1.end(), hashTagMap.find(*it1)->second.begin(), hashTagMap.find(*it1)->second.end(), inserter(mySet, mySet.begin()));
					set1 = mySet;
				}
			}
			// if not present in hashTagMap , clear the set
			else
			{
				cout << "No matches." << endl;
				mySet.clear();
				return mySet;
			}
		}
		if (count == 1) //only 1 hashtag provided
			mySet = set1;
	}
	else if (strategy == 1) // OR
	{
		// for all the 'terms'
		for (vector<string>::iterator it1 = terms.begin(); it1 != terms.end(); ++it1)
		{
			count++;
			// check if its present in the hashTagMap
			if (!(hashTagMap.find(*it1) == hashTagMap.end()))
			{
				if (isFirst)// Set1
				{
					isFirst = false;
					for (set<Tweet*>::iterator it2 = hashTagMap.find(*it1)->second.begin(); it2 != hashTagMap.find(*it1)->second.end(); ++it2)
					{
						set1.insert(*it2);
					}
				}
				else //Set2
				{
					set_union(set1.begin(), set1.end(), hashTagMap.find(*it1)->second.begin(), hashTagMap.find(*it1)->second.end(), inserter(mySet, mySet.begin()));
					set1 = mySet;
				}
			}
			else
			{
				mySet = set1;
			}
		}
		if (count == 1) //only 1 hashtag provided
			mySet = set1;
		
		/* -- OLD CODE !
		// for all the terms
		for (vector<string>::iterator it1 = terms.begin(); it1 != terms.end(); ++it1)
		{
			// check if its present in the hashTagMap
			if (!(hashTagMap.find(*it1) == hashTagMap.end()))
			{
				//if Present : push all the tweets into mt
				for (set<Tweet*>::iterator it2 = hashTagMap.find(*it1)->second.begin(); it2 != hashTagMap.find(*it1)->second.end(); ++it2)
				{
					mySet.insert(*it2);
				}
			}
		}
		*/
	}
	else
		cout << " ERROR : Invalid Operator !" << endl;

	// print a message if there are no matches
	if (mySet.empty())
		cout << "NO matches. " << endl;

	return mySet;
}

void TwitEng::dumpFeeds()
{
	// --------- PRINT FEEDS  --------
	// Iterate through all users int the UserMap 
	for (map<string, User*>::iterator it1 = userMap.begin(); it1 != userMap.end(); ++it1)
	{
		// 1. Create the user's .feed file 
		ofstream outFile(it1->first + ".feed");
		// 2. Print the user name 
		outFile <<  it1->first << endl;
		// 3. Print his tweets
			// retrive the userObject for this user
			User* cu = userMap.find(it1->first)->second;
			// retrive all his tweetsfrom getFeed()
			vector<Tweet*> myFeedVec = cu->getFeed();
			// sort the tweets
			sort(myFeedVec.begin(), myFeedVec.end(), TweetComp());
			// print the tweets
			for (vector<Tweet*>::iterator it2 = myFeedVec.begin(); it2 != myFeedVec.end(); ++it2)
			{
				outFile << (*it2)->time() << " " << (*it2)->user()->name() << " " << (*it2)->text() << endl;
			}
	}
	
	//-------- PRINT HASHTAGS ---------
	/*cout << endl << "HashTags : " << endl << endl;
	for (map<string, set<Tweet*> >::iterator it3 = hashTagMap.begin(); it3 != hashTagMap.end(); ++it3)
	{
		cout << it3->first << " : " <<  endl;

		for (set<Tweet*>::iterator it4 = it3->second.begin(); it4 != it3->second.end(); ++it4)
		{
			cout << (*it4)->text() << endl;
		}
		cout << " ------------------------------------------------------------------------" << endl << endl;
	}*/

}


