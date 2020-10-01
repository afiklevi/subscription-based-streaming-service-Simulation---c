//
// Created by afikloop on 22/05/2018.
//

#ifndef EX3_WET_PART2_AUX_F_H
#define EX3_WET_PART2_AUX_F_H

#include "mtm_ex3.h"
#include "series.h"

//The function receives a name checks if the name is legal
// returns true is it's legal
bool legalName (char* name);

//The function converts a genre type to a string
// returns a string of the genre
char* genreToString (Genre genre);

// the function swaps the two given params
void swap (char** a, char** b);

//The function returns an Array of Char* genres
char** genresCharArray ();

//The function sorts an array of strings according to the A-Z \ 1-9
// receives an array of genres and its length, and returns a pointer
// to the sorted genres array
char** stringsSortedArray (char** genres, int length);

//The function returns the absolute difference of 2 numbers
double absolute (double a, double b);

// The function finds the index of the minimal value of a series rank
// receive an array of series and the number of the series in it
// returns the index of the series with the minimal rank
int findSeriesIndexOfMin (Series* series, int num_of_series);

// The function swaps between 2 series
void seriesSwap (Series* series1, Series* series2);

//The function find the index of the latest genre name according to the A-Z
// receive an array of genres and its length
// returns the index of the genre that appears last (by a lexicographical
// order)
int findIndexOfMax (char** genres, int length);

// The function sorts an array of series according to its rank
// receive an array of series and its length
// returns an array sorted by the rank of the series (top to bottom)
Series* seriesSortArray (Series* series, int length);

// The function checks if the user is at the right age for watching a given
// series
// receives an option for a favorite series and a user
// return whether he's in the proper age or not
bool checksRightAge (Series fav_series, User current_user);

void freeSeriesArray (Series* series_array, char* cpy_username);

#endif //EX3_WET_PART2_AUX_F_H
