//
// Created by afikloop on 22/05/2018.
//



#include <malloc.h>
#include <stdio.h>
#include <assert.h>
#include "user.h"
#include "mtm_ex3.h"
#include "mtmflix.h"
#include "set.h"
#include "series.h"
#include <string.h>

struct user{
    char* name;
    int age;
    List favorite_series;
    List friends;
};

User userCreate (char* name, int age){
    User user = malloc(sizeof(*user));
    if (user == NULL){
            printf("Memory allocation error\n");
            return NULL;
        }
    char* cpy_name = malloc(sizeof(char)*(strlen(name))+1);
    if (cpy_name == NULL){
        printf("Memory allocation error\n");
        return NULL;
    }
    strcpy(cpy_name, name);
    user->name=cpy_name;
    user->age=age;
    user->favorite_series = listCreate(copyFav, freeStr);
    user->friends = listCreate(copyFav, freeStr);
    return user;
}

void userDestroy (User user1){
    if (user1->name != NULL){
        free(user1->name);
    }
    if (user1->favorite_series!=NULL) {
        listDestroy(user1->favorite_series);
    }
    if (user1->friends!=NULL){
        listDestroy(user1->friends);
    }
    free(user1);
}

bool cmpUserByName (User user, char* username){
    if (strcmp(user->name,username) == 0) {
        return true;
    }
    return false;
}

ListResult removeFromFriendLists (Set users, char* username){
    User current_user = setGetFirst(users);
    ListResult result1 = LIST_SUCCESS;
        while (current_user!=NULL) {
            char* current_friend = listGetFirst(current_user->friends);
            while (current_friend!=NULL) {
                if (strcmp(username, current_friend) == 0) {
                    result1 = listRemoveCurrent(current_user->friends);
                    assert(result1 == LIST_SUCCESS);
                }
                current_friend = listGetNext(current_user->friends);
            }
        current_user = setGetNext(users);
    }
    return result1;
}

ListResult removeFavSeries (Set users, char* name) {
    if (users==NULL) {
        return LIST_NULL_ARGUMENT;
    }
    //User current_user = setGetFirst(users);
    ListResult result = LIST_SUCCESS;
    SET_FOREACH(User, cur_user, users){
        LIST_FOREACH(char*, cur_fav, cur_user->favorite_series){
            if (strcmp(name, cur_fav) == 0){
                result = listRemoveCurrent(cur_user->favorite_series);
                continue;
            }
        }
    }
    return result;
}

User userGet (Set users, char* username, UserStatus* status) {
    char* cpy_of_username = malloc(sizeof(char)*(strlen(username))+1);
    if (cpy_of_username == NULL){
        printf("Memory allocation error\n");
        *status = USER_MEMORY_ERROR;
        return NULL;
    }
    strcpy(cpy_of_username, username);
    User current_user = setGetFirst(users);
    while (current_user != NULL) {
        if (cmpUserByName(current_user, cpy_of_username)) {
            free (cpy_of_username);
            return current_user;
        }
        current_user = setGetNext(users);
    }
    free (cpy_of_username);
    return NULL;
}

ListResult insertSeriesToFav (User user, char* series_name){
    ListResult result;
    result = listInsertFirst(user->favorite_series, series_name);
    return result;
}

int getUserAge (User user){
    return user->age;
}

User checkIfUserExist (Set user_set, char* username,
                        UserStatus* status){
    User current_user = userGet(user_set, username, status);
    if (current_user == NULL) {
        *status = BAD_USER_INFO; // User Doesn't exist
        return NULL;
    } else {
        return current_user;
    }
}

ListResult seriesRemoveFromFav (User user, char* series_name){
    ListResult result = LIST_NULL_ARGUMENT;
    LIST_FOREACH(char*, fav_list, user->favorite_series){
        if (strcmp(series_name, fav_list)==0){
            result = listRemoveCurrent(user->favorite_series);
            return LIST_SUCCESS;
        }
    }
    return result;
}

bool checkIfFriendAlready (List friends, char* username) {
    LIST_FOREACH(char*, friend_list, friends) {
        if (strcmp(username, friend_list) == 0) {
            return true;
        }
    }
    return false;
}

ListResult insertUserToFriendList (User user, char* username){
    ListResult result;
    if (checkIfFriendAlready(user->friends, username)) return LIST_SUCCESS;
    result = listInsertFirst(user->friends, username);
    return result;
}

ListResult userRemoveFromFriends (User user, char* username){
    ListResult result = LIST_SUCCESS;
    LIST_FOREACH(char*, friend_list, user->friends){
        if (strcmp(username, friend_list)==0){
            result = listRemoveCurrent(user->friends);
            return LIST_SUCCESS;
        }
    }
    return result;
}

char* getUsername (User user){
    char* username = malloc(sizeof(char)*strlen(user->name)+1);
    if (username == NULL) {
        return NULL;
    }
    strcpy(username,user->name);
    return username;
}

List getFavList (User user){
    return user->favorite_series;
}

List getFriendList (User user){
    return user->friends;
}

double findNumFavSeriesOfGenre (Set series, User user, char* genre) {
    int count = 0;
    SeriesStatus status = SERIES_OK;
    LIST_FOREACH(char*, fav, user->favorite_series){
        char* fav_genre = getGenre(seriesGet(series, fav, &status));
        if (strcmp(genre, fav_genre) == 0) {
            count++;
        }
        free(fav_genre);
    }
    return count;
}

double findAverageLengthFavSeries(User user, Set series){
    double i=0, sum=0;
    SeriesStatus status = SERIES_OK;
    LIST_FOREACH(char*, fav, user->favorite_series){
        Series series1 = seriesGet(series,fav,&status);
        if (series1!=NULL) {
            sum += getSeriesLength(series1);
            i++;
        }
    }
    return sum/i;
}

int checkIfFriendsLikeToo (User user, Set users, char* series_name) {
    User user1;
    int count = 0;
    UserStatus status = USER_OK;
    LIST_FOREACH(char*, friend, user->friends) {
        user1 = userGet(users, friend, &status);
        LIST_FOREACH(char*, fav, user1->favorite_series) {
            if (strcmp(series_name, fav) == 0) {
                count++;
            }
        }
    }
    return count;
}

User copyUser (User source, UserStatusResult* statusResult){
    if (source == NULL){
        if (statusResult != NULL){
            *statusResult = USER_NULL_ARG;
        }
        return NULL;
    }
    User user1 = userCreate(source->name, source->age);
    if (statusResult!=NULL) {
        if (*statusResult != USER_RESULT_OK) {
            return NULL;
        }
    }
    if (statusResult!=NULL){
        *statusResult=USER_RESULT_OK;
    }
    return user1;
}

SetElement copyUserFun (SetElement user){
    if(user==NULL){
        return NULL;
    }
    UserStatusResult copy_result = USER_RESULT_OK;
    User cpy_user = copyUser(user, &copy_result);
    if (cpy_user==NULL || copy_result!=USER_RESULT_OK){
        return NULL;
    }
    return cpy_user;
}

void freeUser (SetElement user){
    if (user!=NULL){
        userDestroy(user);
    }
}

int cmpUser (User user1, User user2) {
    if (strcmp((user1->name), user2->name) == 0) {
        return 0;
    }
    if (strcmp((user1->name), user2->name) > 0) {
        return 1;
    } else {
        return -1;
    }
}

int cmpUserFun (SetElement user1, SetElement user2){
    return cmpUser(user1,user2);
}



SetElement copyFav (SetElement source){
    char* new = malloc(sizeof(char)*(strlen(source))+1);
    if (new==NULL) {
            return NULL;
        }
    strcpy(new, source);
    return new;
}

void freeStr (SetElement name){
    if (name!=NULL) {
        free(name);
    }
}

bool checkIfInFav (User user, char* series_name){
    LIST_FOREACH(char*, fav_list, user->favorite_series){
        if (strcmp(series_name, fav_list)==0){
            return true;
        }
    }
    return false;
}