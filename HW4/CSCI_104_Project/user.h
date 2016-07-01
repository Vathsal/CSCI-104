#ifndef USER_H
#define USER_H

#include <string>
#include <set>
#include <list>
#include <vector>
#include <map>

using namespace std;

/* Forward Declaration to avoid #include dependencies */
class Tweet;

class User
{
public:
	/*  Constructor */
	User(string name);
	/*  Destructor */
	~User();

	/* Gets the name of the user
	* @return name of the user */
	string name() const;

	/* Gets all the followers of this user
	* @return Set of Users who follow this user */
	set<User*> followers() const;

	/* Gets all the users whom this user follows
	* @return Set of Users whom this user follows */
	set<User*> following() const;

	/* Gets all the tweets this user has posted
	* @return List of tweets this user has posted */
	list<Tweet*> tweets() const;

	/* Adds a follower to this users set of followers 
	* @param u User to add as a follower */
	void addFollower(User* u);

	/* Adds another user to the set whom this User follows 
	* @param u User that the user will now follow */
	void addFollowing(User* u);

	/* Adds the given tweet as a post from this user
	* @param t new Tweet posted by this user */
	void addTweet(Tweet* t);

	/* Adds the given tweet as a @Mention Tweet of this user
	* @param t : Tweet containing this user's @mention */
	void addAtMentionTweet(Tweet* t);

	/*Produces the list of Tweets that represent this users feed/timeline
	*  It should contain in timestamp order all the tweets from
	*  this user and all the tweets from all the users whom this user follows
	* @return vector of pointers to all the tweets from this user and those they follow in timestamp order */
	vector<Tweet*> getFeed(map<string , User*> &uMap);

	/* Gets all the @mention tweets of this user */
    list<Tweet*> getAtMentionTweet() const;

private:
	/* Other data members or helper functions */
	string name_;
	set<User*> followersSet_;
	set<User*> followingSet_;
	list<Tweet*> tweetList_;
	list<Tweet*> atMentionTweetList_;
};

#endif
