//
// Created by afikloop on 21/05/2018.
//

#ifndef EX3_WET_PART2_SERIES_H
#define EX3_WET_PART2_SERIES_H
#define NEW_BORN 0
#define ELDERLY 999
#define NUM_OF_GENRES 8
#define FIRST_GENRE SCIENCE_FICTION

#include "mtm_ex3.h"
#include "set.h"

typedef struct series* Series;

typedef enum seriesStatusResult{
    SERIES_RESULT_OK,
    SERIES_RESULT_MEMORY_ERROR,
    BAD_SERIES_RESULT_INFO,
    SERIES_NULL_ARG} SeriesStatusResult;


typedef enum seriesStatus {
    SERIES_OK,
    SERIES_MEMORY_ERROR,
    BAD_SERIES_INFO,
    SERIES_NULL_PTR} SeriesStatus;


/**
* seriesCreate: Allocates a new series.
*
* @param name - a pointer of the name of the series
* @param episodes_num - the number of episode of the series
* 		the set
* @param genre - the genre of the series
* @param ages - a pointer for an array contains the age limits recommended
* @param episode_duration - average duration of an episode of the series

* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new Series pointer in case of success.
*/
Series seriesCreate (char* name, int episodes_num, Genre genre,
                     const int* ages, int episode_duration);

//The function compares between a series and a name of a series, and returns
// 'true' if the series has the name that is given
// receives a series and a name
// return his true if exists
bool checkIfSeries (Series series, char* name);

// The function returns a pointer to a series by a name, NULL if not exists
// receives a series, a name and a status
// return the series if exists, or NULL
Series seriesGet (Set series, char* name, SeriesStatus* status);

// The function returns the minimal age allowed for a series
// receives a series
// return its minimal age
int getMinAge (Series series);

// The function returns the maximal age recommended for a series
// receives a series
// return its maximal age
int getMaxAge (Series series);

// The function checks if the series exists according to a given name
// receives a set of series, series name and a status
// return the series if exists, or NULL
Series checkIfSeriesExist(Set series_set,char* series_name,
                          SeriesStatus* status);

// The function check if the series is from a given genre
// receives series and a genre
// returns the series name if so, else - NULL
char* checkIfSeriesThisGenre(Series series, const char* genre);

// receives series
// The function return (char*) the series genre
char* getGenre (Series series);

// The function creates an array of pointers to series
// receives series
// returns array of series
Series* seriesArray (Set series);

// receives a series
// The function returns the length of a series
int getSeriesLength (Series series);

// receives a series
// The function returns the name of a series
char* seriesGetName (Series series1);

// The function updates the current rank of a series
// receives a series and a rank
void insertRank (Series series1, double rank);

void seriesDestroy (Series series1);

// receives series
// returns the series current rank
int getRank (Series series1);

Series copySeries (Series source, SeriesStatusResult* statusResult);

SetElement copySeriesFun (SetElement series);

// free all allocated memoroy of series
void freeSeries (SetElement series);

SetElement freeSeriesFun (Series series1);

// Receives two series and compares between them
// return 0 if equal, 1 if the first is first by lexicographical
// order, and -1 if the second is
int cmpSeries (Series series1, Series series2);

int cmpSeriesFun (SetElement series1, SetElement series2);

#endif //EX3_WET_PART2_SERIES_H
