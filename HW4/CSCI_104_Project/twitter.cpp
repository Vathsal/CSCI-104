#include "twiteng.h"
#include "tweet.h"
#include "datetime.h"
#include "user.h"
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>

using namespace std;

void displayTweets(set<Tweet*>& hits)
{
	for (set<Tweet*>::iterator it = hits.begin(); it != hits.end(); ++it)
		cout << (*it)->time() << " " << (*it)->user()->name() << " " << (*it)->text() << endl;
}

vector<string>& parseHashTags(string &myStr , vector<string>& terms)
{
	string temp;
	stringstream ss(myStr);
	bool isFirst = true;

	while (ss >> temp)
	{
		if (isFirst)
		{
			isFirst = false;
			continue;
		}
		terms.push_back(temp);
	}
	return terms;
}

void parseInput(TwitEng& twit)
{
	string myStr;
	string temp;
	set<Tweet*> hits;
	vector<string> terms;

	// Assuming all inputs are in 1 line 
	while (myStr != "QUIT")
	{
		// read 1 line of input
		getline(cin, myStr);

		// skip blank lines
		if (myStr == "")
			continue;

		stringstream ss(myStr);
		ss >> temp;

		// clear the vector and set
		terms.clear();
		hits.clear();

		if (temp == "AND")
		{
			terms = parseHashTags(myStr, terms);
			hits = twit.search(terms, 0);
		}
		else if (temp == "OR")
		{
			terms = parseHashTags(myStr, terms);
			hits = twit.search(terms, 1);
		}
		else if (temp == "TWEET")
		{
			twit.addNewTweet(myStr);
		}
		else if (temp == "QUIT")
		{
			break;
		}
		displayTweets(hits);
	}
}

int main(int argc, char* argv[])
{
	if (argc < 2) {
		cerr << "Please provide the twitter data file" << endl;
		return 1;
	}

	TwitEng twit;

	if (!(twit.parse(argv[1]))) {
		cerr << "Unable to parse " << argv[1] << endl;
		return 1;
	}
	
	cout << "=====================================" << endl;
	cout << "Menu: " << endl;
	cout << "  AND term term ...                  " << endl;
	cout << "  OR term term ...                   " << endl;
	cout << "  TWEET username tweet_text " << endl;
	cout << "  QUIT (and write feed files)        " << endl;
	cout << "=====================================" << endl;
	
	// pass the input as it is entered by  the user
	//parseInput(twit);
	
	// dump feeds on Quit
	twit.dumpFeeds();

	//system("pause");

	return 0;
}