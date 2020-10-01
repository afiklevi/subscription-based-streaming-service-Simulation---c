//
// Created by afikloop on 22/05/2018.
//

#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include "mtm_ex3.h"
#include "series.h"
#include "user.h"

#define LARGE_GENRE_NAME 100



bool legalName (char* name){
    if (name[0] == '\0') return false;
    for (int i=0; i<strlen(name); i++){
        if (!((name[i] >= 'a' && name[i] <= 'z') ||
              (name[i] >= 'A' && name[i] <= 'Z') ||
                ((name[i]>='0') && (name[i]<='9')))){
            return false;
        }
    }
    return true;
}

char* genreToString (Genre genre){
    const char* genres[] = { "SCIENCE_FICTION", "DRAMA", "COMEDY", "CRIME",
                             "MYSTERY", "DOCUMENTARY", "ROMANCE", "HORROR"};
    char* genre_name = malloc(LARGE_GENRE_NAME);
    if (genre_name == NULL){
        printf("Memory allocation error.\n");
        return NULL;
    }
    strcpy(genre_name,genres[genre]);
    return genre_name;
}

char** genresCharArray (){
    Genre genreEnum = FIRST_GENRE;
    char** genres = malloc(sizeof(char*)*NUM_OF_GENRES);
    if (genres == NULL){
        printf("Memory allocation error.\n");
        return NULL;
    }
    for (int i=0; i<NUM_OF_GENRES; i++) {
        genres[i] = genreToString(genreEnum + i);
        if (genres[i] == NULL) {
            return NULL;
        }
    }
    return genres;
}

int findIndexOfMax (char** genres, int length) {
    int i = 0, i_max = 0;
    for (i = 1; i < length; i++) {
        if (strcmp(genres[i_max], genres[i]) < 0) {
            i_max = i;
        }
    }
    return i_max;
}

void swap (char** a, char** b){
    char *temp = *a;
    *a = *b;
    *b = temp;
}

char** stringsSortedArray (char** strings, int length){
    int i = length;
    for (;i>1; i--) {
        int i_max = findIndexOfMax(strings, i);
        swap(&strings[i_max], &strings[i-1]);
        }
    return strings;
    }

double absolute (double a, double b){
    double c = a-b;
    if (c<0){
        return (c*(-1));
    }
    else{
        return c;
    }
}

int findSeriesIndexOfMin (Series* series, int num_of_series) {
    int i = 0, i_min = 0;
    for (i = 1; i < num_of_series; i++) {
        if ((getRank(series[i]))<(getRank(series[i_min]))) {
            i_min = i;
        }
        if ((getRank(series[i]))==(getRank(series[i_min])) &&
                (getRank(series[i])!= 0)){
            if ((strcmp(seriesGetName(series[i]),
                        seriesGetName(series[i_min])))>0){
                i_min = i;
            }
        }
    }
    return i_min;
}

void seriesSwap (Series* series1, Series* series2){
    Series temp = *series2;
    *series2 = *series1;
    *series1 = temp;
}

Series* seriesSortArray (Series* series, int length){
    for (int i = length; i>1; i--) {
        int i_min = findSeriesIndexOfMin(series, i);
        seriesSwap(&series[i_min], &series[i-1]);
    }
    return series;
}

bool checksRightAge (Series fav_series, User current_user) {
    int min_age = getMinAge(fav_series);
    int max_age = getMaxAge(fav_series);
    int user_age = getUserAge(current_user);
    if (user_age < min_age || user_age > max_age) {
        return false;
    }
    return true;
}

void freeSeriesArray (Series* series_array, char* cpy_username){
    free(series_array);
    free(cpy_username);
}