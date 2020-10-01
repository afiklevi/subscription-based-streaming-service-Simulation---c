//
// Created by afikloop on 22/05/2018.
//

#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "series.h"
#include "mtm_ex3.h"
#include "set.h"
#include "user.h"
#include "mtmflix.h"
#include "aux_f.h"

struct series{
    char* name;
    int number_of_episodes;
    Genre genre;
    int min_age;
    int max_age;
    int episode_duration;
    int current_rank;
};




Series seriesCreate (char* name, int episodes_num, Genre genre,
                     const int* ages, int episode_duration){
    Series series= malloc(sizeof(*series));
    if (series == NULL){
        printf("Memory allocation error\n");
        return NULL;
    }
    char* cpy_name = malloc(sizeof(char)*(strlen(name))+1);
    if (cpy_name == NULL){
        printf("Memory allocation error\n");
        return NULL;
    }
    strcpy(cpy_name, name);
    series->name=cpy_name;
    series->episode_duration=episode_duration;
    series->number_of_episodes=episodes_num;
    Genre cpy_genre = genre;
    series->genre = cpy_genre;
    series->current_rank = 0;
    if (ages == NULL){
        series->min_age = NEW_BORN;
        series->max_age = ELDERLY;
    } else {
        series->min_age = ages[0];
        series->max_age = ages[1];
    }
    return series;
}

void seriesDestroy (Series series1){
    if(series1){
        free(series1->name);
    }
    free(series1);
}

bool checkIfSeries (Series series, char* name){
    if (strcmp(series->name,name) == 0) {
        return true;
    }
    return false;
}

Series seriesGet (Set series, char* name, SeriesStatus* status) {
    char *cpy_of_name = malloc(sizeof(char)*(strlen(name))+1);
    if (cpy_of_name == NULL){
        printf("Memory allocation error\n");
        *status = SERIES_MEMORY_ERROR;
        return NULL;
    }
    strcpy(cpy_of_name, name);
    Series current_series = setGetFirst(series);
    while (current_series != NULL) {
        if (checkIfSeries(current_series, cpy_of_name)) {
            free(cpy_of_name);
            return current_series;
        }
        current_series = setGetNext(series);
    }
    free(cpy_of_name);
    return NULL;
}

int getMinAge (Series series){
    return series->min_age;
}

int getMaxAge (Series series){
    return series->max_age;
}

Series checkIfSeriesExist (Set series_set, char* series_name,
                        SeriesStatus* status){
    Series fav_series = seriesGet(series_set, series_name, status);
    if (fav_series == NULL) {
        free (series_name);
        *status = BAD_SERIES_INFO; // Series Doesn't exist
        return NULL;
    } else {
        return fav_series;
    }
}

char* checkIfSeriesThisGenre(Series series, const char* genre){
    if (series == NULL){
        return NULL;
    }
    char* string= genreToString(series->genre);
    if (strcmp(string,genre)==0){
        free(string);
        return series->name;
        }
    free(string);
    return NULL;
}

char* getGenre (Series series){
    if (series == NULL){
        return NULL;
    }
    char* genre = genreToString(series->genre);
    return genre;
}

Series* seriesArray (Set series){
    int i=0, num_of_series = setGetSize(series);
    Series* seriesArray = malloc(sizeof(Series)*num_of_series);
    if (seriesArray == NULL) {
        return NULL;
    }
    SET_FOREACH(Series, iterator, series){
        seriesArray[i] = iterator;
        i++;
    }
    return seriesArray;
}

int getSeriesLength (Series series){
    return series->episode_duration;
}

char* seriesGetName (Series series1){
    return series1->name;
}

void insertRank (Series series1, double rank){
    series1->current_rank = (int)rank;
}

int getRank (Series series1){
    return series1->current_rank;
}

Series copySeries (Series source, SeriesStatusResult* statusResult){
    if (source == NULL){
        if (statusResult != NULL){
            *statusResult = SERIES_NULL_ARG;
        }
        return NULL;
    }
    int ages[2];
    ages[0] = source->min_age;
    ages[1] = source->max_age;
    Series series1 = seriesCreate(source->name, source->number_of_episodes,
                              source->genre, ages, source->episode_duration);
    if (statusResult!=NULL) {
        if (*statusResult != SERIES_RESULT_OK) {
            return NULL;
        }
    }
    if (statusResult!=NULL){
        *statusResult=SERIES_RESULT_OK;
    }
    return series1;
}

SetElement copySeriesFun (SetElement series){
    if(series==NULL){
        return NULL;
    }
    SeriesStatusResult copy_result = SERIES_RESULT_OK;
    Series cpy_series = copySeries(series, &copy_result);
    if (cpy_series==NULL || copy_result!=SERIES_RESULT_OK){
        return NULL;
    }
    return cpy_series;
}

void freeSeries (SetElement series){
    if (series!=NULL){
        seriesDestroy(series);
    }
}

int cmpSeries (Series series1, Series series2) {
    if (strcmp((series1->name), series2->name) == 0) {
        return 0;
    }
    if (strcmp((series1->name), series2->name) > 0) {
        return 1;
    } else {
        return -1;
    }
}

int cmpSeriesFun (SetElement series1, SetElement series2){
    return cmpSeries(series1,series2);
}