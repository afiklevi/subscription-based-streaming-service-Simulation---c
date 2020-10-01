//
// Created by afikloop on 21/05/2018.
//


#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "mtmflix.h"
#include "set.h"
#include "user.h"
#include "aux_f.h"
#include "series.h"

// The function sorts an array of (char*) series according to a given genre,
// by the ABC

char** sortedSeriesForGenre (MtmFlix mtmflix, const char* genre);

struct mtmFlix_t{
    Set user_set;
    Set series_set;
};

MtmFlix mtmFlixCreate(){
    MtmFlix mtmflix = malloc(sizeof(*mtmflix));
    if (mtmflix == NULL){
        printf("Memory allocation error.\n");
        return NULL;
    }
    mtmflix->user_set = setCreate(copyUserFun, freeUser, cmpUserFun);
    mtmflix->series_set = setCreate(copySeriesFun, freeSeries, cmpSeriesFun);
    return mtmflix;
}

void mtmFlixDestroy (MtmFlix mtmflix){
    if (!mtmflix) return;
    if (mtmflix->user_set) {
        setDestroy(mtmflix->user_set);
    }
    if (mtmflix->series_set) {
        setDestroy(mtmflix->series_set);
    }
    free(mtmflix);
}

MtmFlixResult mtmFlixAddUser(MtmFlix mtmflix, const char* username, int age){
    if (mtmflix == NULL || username == NULL) return MTMFLIX_NULL_ARGUMENT;
    char* cpy_of_username = malloc(sizeof(char)*(strlen(username))+1);
    strcpy(cpy_of_username, username);
    if (!legalName(cpy_of_username)) {
        free(cpy_of_username);
        return MTMFLIX_ILLEGAL_USERNAME;
    }
    UserStatus status = USER_OK;
    User user = checkIfUserExist(mtmflix->user_set, cpy_of_username, &status);
    if (user!=NULL){
        free(cpy_of_username);
        return MTMFLIX_USERNAME_ALREADY_USED;
    }
    if (age < MTM_MIN_AGE || age > MTM_MAX_AGE) {
        free(cpy_of_username);
        return MTMFLIX_ILLEGAL_AGE;
    }
    char* cpy_of_username1 = malloc(sizeof(char)*(strlen(username))+1);
    strcpy(cpy_of_username1, username);
    user = userCreate(cpy_of_username1, age);
    if (user == NULL){
        free(cpy_of_username);
        free(cpy_of_username1);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    SetResult set_status;
    set_status = setAdd(mtmflix->user_set, user);
    userDestroy(user);
    free(cpy_of_username);
    free(cpy_of_username1);
    if (set_status == SET_NULL_ARGUMENT) return MTMFLIX_NULL_ARGUMENT;
    if (set_status == SET_OUT_OF_MEMORY) return MTMFLIX_OUT_OF_MEMORY;

    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixRemoveUser (MtmFlix mtmflix, const char* username){
    if (mtmflix == NULL || username == NULL) return MTMFLIX_NULL_ARGUMENT;
    UserStatus user_status = USER_OK;
    char* cpy_of_username = malloc(sizeof(char)*(strlen(username))+1);
    strcpy(cpy_of_username, username);
    User current_user=userGet(mtmflix->user_set,cpy_of_username,&user_status);
    SetResult set_status = SET_NULL_ARGUMENT;
    ListResult remove_result = LIST_NULL_ARGUMENT;
    if (current_user != NULL){
        remove_result=removeFromFriendLists
                (mtmflix->user_set,cpy_of_username);
        set_status=setRemove(mtmflix->user_set, current_user);
        }
    free(cpy_of_username);
    if (set_status == SET_SUCCESS && remove_result == LIST_SUCCESS) {
        return MTMFLIX_SUCCESS;
    } else {
        return MTMFLIX_USER_DOES_NOT_EXIST;
    }
}

MtmFlixResult mtmFlixAddSeries(MtmFlix mtmflix, const char* name,
               int episodesNum, Genre genre, int* ages, int episodesDuration){
    if (mtmflix == NULL) return MTMFLIX_NULL_ARGUMENT;
    if (name == NULL) return MTMFLIX_NULL_ARGUMENT;
    char* cpy_of_series_name = malloc(sizeof(char)*(strlen(name))+1);
    strcpy(cpy_of_series_name, name);
    if (!legalName(cpy_of_series_name)) {
        free(cpy_of_series_name);
        return MTMFLIX_ILLEGAL_SERIES_NAME;
    }
    Series series = seriesCreate(cpy_of_series_name, episodesNum, genre,
                                 ages, episodesDuration);
    if (series == NULL){
        free(cpy_of_series_name);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    SeriesStatus status = SERIES_OK;
    if (seriesGet(mtmflix->series_set, cpy_of_series_name, &status) != NULL){
        free(cpy_of_series_name);
        seriesDestroy(series);
        return MTMFLIX_SERIES_ALREADY_EXISTS;
    }
    if (episodesNum<=0) {
        free(cpy_of_series_name);
        seriesDestroy(series);
        return MTMFLIX_ILLEGAL_EPISODES_NUM;
    }
    if (episodesDuration<=0){
        free(cpy_of_series_name);
        seriesDestroy(series);
        return MTMFLIX_ILLEGAL_EPISODES_DURATION;
    }
    SetResult set_status;
    set_status = setAdd(mtmflix->series_set, series);
    seriesDestroy(series);
    //free(cpy_of_series_name);
    //if (set_status == SET_SUCCESS) return MTMFLIX_SUCCESS;
    if (set_status == SET_NULL_ARGUMENT){
        free(cpy_of_series_name);
        seriesDestroy(series);
        return MTMFLIX_NULL_ARGUMENT;
    }
    if (set_status == SET_OUT_OF_MEMORY){
        free(cpy_of_series_name);
        seriesDestroy(series);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    free(cpy_of_series_name);
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixRemoveSeries(MtmFlix mtmflix, const char* name) {
    if (mtmflix == NULL || name == NULL) return MTMFLIX_NULL_ARGUMENT;
    SeriesStatus status = SERIES_OK;
    char *cpy_of_name = malloc((sizeof(char)) * (strlen(name))+1);
    strcpy(cpy_of_name, name);
    Series current_series = seriesGet(mtmflix->series_set, cpy_of_name,
                                      &status);
    if (current_series == NULL) {
        free(cpy_of_name);
        return MTMFLIX_SERIES_DOES_NOT_EXIST;
    }
    SetResult set_status = SET_NULL_ARGUMENT;
    ListResult remove_result = LIST_NULL_ARGUMENT;
    remove_result = removeFavSeries(mtmflix->user_set, cpy_of_name);
    set_status = setRemove(mtmflix->series_set, current_series);
    free(cpy_of_name);
    if (set_status == SET_SUCCESS && remove_result == LIST_SUCCESS) {
        return MTMFLIX_SUCCESS;
    } else {
        return MTMFLIX_USER_DOES_NOT_EXIST;
    }
}

MtmFlixResult mtmFlixSeriesJoin(MtmFlix mtmflix, const char* username,
                                const char* seriesName) {
    if (mtmflix == NULL || username == NULL || seriesName == NULL)
        return MTMFLIX_NULL_ARGUMENT;
    UserStatus user_status = USER_OK;
    User current_user;
    char* cpy_of_username = malloc((sizeof(char))*(strlen(username)+1));
    if (cpy_of_username == NULL){
        printf("Memory allocation error\n");
        user_status = USER_MEMORY_ERROR;
        return MTMFLIX_OUT_OF_MEMORY;
    }
    strcpy(cpy_of_username, username);
    current_user = checkIfUserExist(mtmflix->user_set, cpy_of_username,
                                 &user_status);
    if (current_user == NULL){
        free (cpy_of_username);
        return MTMFLIX_USER_DOES_NOT_EXIST;
    }
    if (checkIfInFav(current_user,cpy_of_username)) {
        free(cpy_of_username);
        return MTMFLIX_SUCCESS;
    }
    SeriesStatus series_status = SERIES_OK;
    Series fav_series;
    char* cpy_of_name = malloc(sizeof(char)*(strlen(seriesName))+1);
    if (cpy_of_name == NULL){
        free (cpy_of_username);
        printf("Memory allocation error\n");
        series_status = SERIES_MEMORY_ERROR;
        return MTMFLIX_OUT_OF_MEMORY;
    }
    strcpy(cpy_of_name, seriesName);
    fav_series = checkIfSeriesExist(mtmflix->series_set, cpy_of_name,
                                 &series_status);
    if (fav_series == NULL) {
        free (cpy_of_username);
        return MTMFLIX_SERIES_DOES_NOT_EXIST;
    }
    if (!checksRightAge(fav_series, current_user)){
        free (cpy_of_name);
        free (cpy_of_username);
        return MTMFLIX_USER_NOT_IN_THE_RIGHT_AGE;
    }
    ListResult result = insertSeriesToFav(current_user, cpy_of_name);
    free (cpy_of_name);
    free (cpy_of_username);
    if (result == LIST_OUT_OF_MEMORY) {
        return MTMFLIX_OUT_OF_MEMORY;
    }
    if (result == LIST_SUCCESS) {
        return MTMFLIX_SUCCESS;
    }
    return MTMFLIX_OUT_OF_MEMORY; // not supposed to get here
}

MtmFlixResult mtmFlixSeriesLeave(MtmFlix mtmflix,
                             const char* username, const char* seriesName){
    if (mtmflix == NULL || username == NULL ||
            seriesName == NULL) return MTMFLIX_NULL_ARGUMENT;

    UserStatus user_status = USER_OK;
    User current_user;
    char* cpy_of_username = malloc(sizeof(char)*(strlen(username))+1);
    if (cpy_of_username == NULL){
        printf("Memory allocation error\n");
        user_status = USER_MEMORY_ERROR;
        return MTMFLIX_OUT_OF_MEMORY;
    }
    strcpy(cpy_of_username, username);
    current_user = checkIfUserExist(mtmflix->user_set, cpy_of_username,
                                 &user_status);
    if (current_user == NULL) {
        return MTMFLIX_USER_DOES_NOT_EXIST;
    }
    SeriesStatus series_status = SERIES_OK;
    Series fav_series;
    char* cpy_of_name = malloc(sizeof(char)*(strlen(seriesName))+1);
    if (cpy_of_name == NULL){
        printf("Memory allocation error\n");
        series_status = SERIES_MEMORY_ERROR;
        return MTMFLIX_OUT_OF_MEMORY;
    }
    strcpy(cpy_of_name, seriesName);
    fav_series = checkIfSeriesExist(mtmflix->series_set, cpy_of_name,
                                 &series_status);
    if (fav_series == NULL) {
        return MTMFLIX_SERIES_DOES_NOT_EXIST;
    }
    ListResult result = seriesRemoveFromFav(current_user, cpy_of_name);
    if (result == LIST_SUCCESS) {
        free (cpy_of_name);
        free (cpy_of_username);
        return MTMFLIX_SUCCESS;
    }
    if (result == LIST_OUT_OF_MEMORY) {
        free (cpy_of_name);
        free (cpy_of_username);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    printf("if you see this there is a problem on mtmFlixSeriesLeave \n");
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixAddFriend(MtmFlix mtmflix,
                               const char* username1, const char* username2){
    if (mtmflix == NULL || username1 == NULL || username2 == NULL)
        return MTMFLIX_NULL_ARGUMENT;
    if (strcmp(username1,username2)==0) return MTMFLIX_SUCCESS;
    UserStatus user1_status = USER_OK, user2_status = USER_OK;
    User user1, user2;
    char* cpy_of_username1 = malloc(sizeof(char)*(strlen(username1))+1);
    if (!cpy_of_username1){
        printf("Memory allocation error\n");
        user1_status = USER_MEMORY_ERROR;
        user2_status = USER_MEMORY_ERROR;
        return MTMFLIX_OUT_OF_MEMORY;
    }
    char* cpy_of_username2 = malloc(sizeof(char)*(strlen(username2))+1);
    if (cpy_of_username2 == NULL){
        printf("Memory allocation error\n");
        free(cpy_of_username1);
        user1_status = USER_MEMORY_ERROR;
        user2_status = USER_MEMORY_ERROR;
        return MTMFLIX_OUT_OF_MEMORY;
    }
    strcpy(cpy_of_username1, username1);
    strcpy(cpy_of_username2, username2);
    user1 = checkIfUserExist(mtmflix->user_set, cpy_of_username1,
                                 &user1_status);
    user2 = checkIfUserExist(mtmflix->user_set, cpy_of_username2,
                          &user2_status);
    if (user1 == NULL || user2 == NULL) {
        free(cpy_of_username1);
        free(cpy_of_username2);
        return MTMFLIX_USER_DOES_NOT_EXIST;
    }
    ListResult result = insertUserToFriendList(user1, cpy_of_username2);
    if (result == LIST_SUCCESS) {
        free (cpy_of_username1);
        free (cpy_of_username2);
        return MTMFLIX_SUCCESS;
    }
    if (result == LIST_OUT_OF_MEMORY) {
        free(cpy_of_username1);
        free(cpy_of_username2);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixRemoveFriend(MtmFlix mtmflix,
                              const char* username1, const char* username2){
    if (mtmflix == NULL || username1 == NULL || username2 == NULL){
        return MTMFLIX_NULL_ARGUMENT;
    }
    UserStatus user1_status = USER_OK, user2_status = USER_OK;
    User user1, user2;
    char* cpy_of_username1 = malloc(sizeof(char)*(strlen(username1))+1);
    if (cpy_of_username1 == NULL){
        printf("Memory allocation error\n");
        user1_status = USER_MEMORY_ERROR;
        user2_status = USER_MEMORY_ERROR;
        return MTMFLIX_OUT_OF_MEMORY;
    }
    char* cpy_of_username2 = malloc(sizeof(char)*(strlen(username2))+1);
    if (cpy_of_username2 == NULL){
        free(cpy_of_username1);
        printf("Memory allocation error\n");
        user1_status = USER_MEMORY_ERROR;
        user2_status = USER_MEMORY_ERROR;
        return MTMFLIX_OUT_OF_MEMORY;
    }
    strcpy(cpy_of_username1, username1);
    strcpy(cpy_of_username2, username2);
    user1 = checkIfUserExist(mtmflix->user_set, cpy_of_username1,
                          &user1_status);
    user2 = checkIfUserExist(mtmflix->user_set, cpy_of_username2,
                          &user2_status);
    if (user1 == NULL || user2 == NULL) {
        free(cpy_of_username1);
        free(cpy_of_username2);
        return MTMFLIX_USER_DOES_NOT_EXIST;
    }
    ListResult result = userRemoveFromFriends(user1, cpy_of_username2);
    if (result == LIST_SUCCESS) {
        free (cpy_of_username1);
        free (cpy_of_username2);
        return MTMFLIX_SUCCESS;
    }
    if (result == LIST_OUT_OF_MEMORY) {
        free (cpy_of_username1);
        free (cpy_of_username2);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    return MTMFLIX_OUT_OF_MEMORY; // not supposed to get here
}

MtmFlixResult mtmFlixGetRecommendations(MtmFlix mtmflix,
                    const char* username, int count, FILE* outputStream){
    if (mtmflix == NULL || username == NULL || outputStream == NULL) {
        return MTMFLIX_NULL_ARGUMENT;
    }
    UserStatus user_status = USER_OK;
    User user;
    char* cpy_of_username = malloc(sizeof(char)*(strlen(username))+1);
    if (cpy_of_username == NULL){
        printf("Memory allocation error\n");
        user_status = USER_MEMORY_ERROR;
        return MTMFLIX_OUT_OF_MEMORY;
    }
    strcpy(cpy_of_username, username);
    user = checkIfUserExist(mtmflix->user_set,cpy_of_username,&user_status);
    if (user == NULL) {
        free(cpy_of_username);
        return MTMFLIX_USER_DOES_NOT_EXIST;
    }
    if (count<0){
        free(cpy_of_username);
        return MTMFLIX_ILLEGAL_NUMBER;
    }
    double avg_len_fav=findAverageLengthFavSeries(user, mtmflix->series_set),
            rank,fav_from_genre, length_of_series, friends_like_too;
    int num=0,num_of_series = setGetSize(mtmflix->series_set);
    Series* series_array = seriesArray(mtmflix->series_set);
    for (int i=0; i<num_of_series; i++){
        if ((checkIfInFav(user,seriesGetName(series_array[i])))||
        (!checksRightAge(series_array[i], user))){
            insertRank (series_array[i],0);
            continue;
        }
        char * genre = getGenre(series_array[i]);
        fav_from_genre=findNumFavSeriesOfGenre(mtmflix->series_set,user,genre);
        free(genre);
        length_of_series = getSeriesLength (series_array[i]);
        friends_like_too=checkIfFriendsLikeToo(user, mtmflix->user_set,
                           seriesGetName(series_array[i]));
        rank = (fav_from_genre*friends_like_too)/
                (1+absolute(length_of_series,avg_len_fav));
        insertRank (series_array[i],rank);
    }
    Series* sorted_series_array=seriesSortArray(series_array,num_of_series);
    if ((count == 0) || (count>num_of_series)) {
        num = num_of_series;
    } else {
        num = count;
    }
    for (int i = 0; i < num; i++) {
        if (!getRank(sorted_series_array[i])==0) {
            char * genre = getGenre(series_array[i]);
            fprintf(outputStream, mtmPrintSeries(
                    seriesGetName(sorted_series_array[i]),genre));
            free(genre);
            }
        }
    freeSeriesArray (series_array,cpy_of_username);
    return MTMFLIX_SUCCESS;
}

char** sortedSeriesForGenre (MtmFlix mtmflix, const char* genre) {
    int i=0, series_num = setGetSize(mtmflix->series_set);
    char** series_for_genre_array = malloc(sizeof(char*)*(series_num+1));
    if (series_for_genre_array == NULL){
        printf("Memory allocation error\n");
        return NULL;
    }
    SET_FOREACH(Series , iterator, mtmflix->series_set) {
        series_for_genre_array[i] = checkIfSeriesThisGenre(iterator, genre);
        if (series_for_genre_array[i]!=NULL){
            i++;
        }
    }
    series_for_genre_array[i] = NULL;
    return series_for_genre_array;
}

MtmFlixResult mtmFlixReportSeries
        (MtmFlix mtmflix, int seriesNum, FILE* outputStream){
    if (mtmflix == NULL || outputStream == NULL) return MTMFLIX_NULL_ARGUMENT;
    if ((setGetFirst(mtmflix->series_set) == NULL)) return MTMFLIX_NO_SERIES;
    char** genres1 = genresCharArray();
    stringsSortedArray(genres1, NUM_OF_GENRES);
    int count = 0;
    for (int i = 0; i<NUM_OF_GENRES; i++) {
        char** sorted_series_by_genre =
                sortedSeriesForGenre(mtmflix, genres1[i]);
        while ((sorted_series_by_genre[count] != NULL) &&
                ((count < seriesNum)||(seriesNum == 0))) {
            const char * to_print =
                    mtmPrintSeries(sorted_series_by_genre[count], genres1[i]);
            fprintf(outputStream,"%s", to_print);
            count++;
        }
        free(sorted_series_by_genre);
        count = 0;
    }
    for (int i=0;i<NUM_OF_GENRES;i++){
        free(genres1[i]);
    }
    free(genres1);
    return MTMFLIX_SUCCESS;
}

char** usersCharArray (MtmFlix mtmflix){
    int number_of_users = setGetSize(mtmflix->user_set), i=0;
    char **usernames = malloc(sizeof(char *) * (number_of_users));
    SET_FOREACH(User ,iterator, mtmflix->user_set) {
        usernames[i] = getUsername (iterator);
        i++;
        }
    return usernames;
}


MtmFlixResult mtmFlixReportUsers
        (MtmFlix mtmflix, FILE* outputStream){
    if (mtmflix == NULL || outputStream == NULL) return MTMFLIX_NULL_ARGUMENT;
    if (setGetFirst(mtmflix->user_set) == NULL) return MTMFLIX_NO_USERS;
    User user;
    UserStatus status = USER_OK;
    int number_of_users = setGetSize(mtmflix->user_set);
    char** all_usernames = usersCharArray (mtmflix);
    all_usernames = stringsSortedArray(all_usernames, number_of_users);
    for (int i = 0; i<number_of_users; i++){
        user = userGet(mtmflix->user_set,all_usernames[i],&status);
        fprintf(outputStream,
                mtmPrintUser(all_usernames[i], getUserAge(user),
                getFriendList(user), getFavList(user)));
        }
    for (int i=0;i<number_of_users;i++){
        free(all_usernames[i]);
    }
    free(all_usernames);
    return MTMFLIX_SUCCESS;
}
