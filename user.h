//
// Created by afikloop on 21/05/2018.
//

#ifndef EX3_WET_PART2_USER_H
#define EX3_WET_PART2_USER_H

typedef struct user* User;

#include <stdbool.h>
#include "set.h"
#include "list.h"

typedef enum userStatusResult{
    USER_RESULT_OK,
    USER_RESULT_MEMORY_ERROR,
    BAD_USER_RESULT_INFO,
    USER_NULL_ARG} UserStatusResult;

typedef enum userStatus {
    USER_OK,
    USER_MEMORY_ERROR,
    BAD_USER_INFO,
    USER_NULL_PTR} UserStatus;


/**
* userCreate: Allocates a new user.
*
* @param name - a pointer of the name of the series
* @param age - user's age

* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new User pointer in case of success.
*/
User userCreate (char* name, int age);

// The function returns true if a given username is
// an actual username in the system (also given)
// recevies a user and a user name
// returns whether equal or not
bool cmpUserByName (User user, char* username);

// The function removes a username from all users's friend list
// receives a user set and a username
// return LIST_SUCCESS
ListResult removeFromFriendLists (Set users, char* username);

// The function removes a series from all users' favorite series lists
// receives a user set and a name of a series
// return LIST_SUCCESS if okay
ListResult removeFavSeries (Set users, char* name);

// receives a user set, a status and a username
// return the user if okay or NULL if not
User userGet (Set users, char* username, UserStatus* status);

// The function insert a series name to a user's favorite
// series list
// receives a user set and a series name
// return LIST_SUCCESS if inserted
ListResult insertSeriesToFav (User user, char* series_name);

// receives user and returns his age
int getUserAge (User user);

// The function returns the pointer of a user according to a given name
// if no such user exists - returns NULL
User checkIfUserExist (Set user_set, char* username,
                     UserStatus* status);

// The function removes a series from all users' favorite series lists
// receives a user set and a series name
// return LIST_SUCCESS if removed or NULL
ListResult seriesRemoveFromFav (User user, char* series_name);

// receives a user set and a username
// return LIST_SUCCESS if inserted or NULL
ListResult insertUserToFriendList (User user, char* username);

// receives a user set and a username
// return LIST_SUCCESS if removed or NULL
ListResult userRemoveFromFriends (User user, char* username);

// receives a user
// return his age
char* getUsername (User user);

// receives a user
// return his favorite list
List getFavList (User user);

// receives a user
// return his friends list
List getFriendList (User user);

// The function returns 'true' if a given username is already in a
// user's friend list
// receives a friends list a user name
// true if the user is in it
bool checkIfFriendAlready (List friends, char* username);

// The function sums the amount of series which are like the same genre
// as given, from a user's favorite series list
double findNumFavSeriesOfGenre (Set series, User user, char* genre);

// The function calculates the average length of a user's
// favorite series
double findAverageLengthFavSeries(User user, Set series);

//The function sums the number of friends that like the certain series
int checkIfFriendsLikeToo (User user, Set users, char* series_name);

// Receive a user
// returns a copy of the user (new memory allocated)
User copyUser (User source, UserStatusResult* statusResult);

SetElement copyUserFun (SetElement user);

// Receives two users and compares between the names
// return 0 if equal, 1 if the first is first by lexicographical
// order, and -1 if the second is
int cmpUser (User user1, User user2);

int cmpUserFun (SetElement user1, SetElement user2);

// free all allocated memoroy of user
// including his name
// using userDestroy
void freeUser (SetElement user);

SetElement copyFav (SetElement source);

// free all allocated memoroy of user
// including his name
void userDestroy (User user1);

// free all allocated memoroy of a name
void freeStr (SetElement name);

// The function returns 'true' if the series givens is favored by a user
bool checkIfInFav (User user, char* series_name);

#endif //EX3_WET_PART2_USER_H
