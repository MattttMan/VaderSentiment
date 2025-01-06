#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Constants used for sentiment analysis scoring and memory allocation
#define BOOST_FACTOR 0.293
#define CAPS_MULTIPLIER 1.5
#define EXCLAMATION_BOOST 0.292
#define MAX_STRING_LENGTH 50
#define MAX_WORDS 10000
#define ALPHA 15


// Data structure for lexicon words
typedef struct {
    char word[MAX_STRING_LENGTH];
    float value1;
    float value2;
} WordData;

// Function declarations

// Count exclamation marks at the end of a sentence
int count_sentence_exclamation_marks(const char *sentence);

// Check if a word is a positive intensifier
int is_positive_intensifier(const char *word);

// Check if a word is a negative intensifier
int is_negative_intensifier(const char *word);

// Check if a word is a negation
int is_negation(const char *word);

// Populate the lexicon data from a file
int load_lexicon(const char *filename, WordData lexicon[], int max_entries);

// Convert a string to lowercase
void to_lower(char *str);

// Remove only periods, exclamations, and commas from a word
void remove_punctuation(char *word);

// Check if a word exists in the lexicon and retrieve its sentiment score
float get_lexicon_score(const char *word, WordData lexicon[], int lexicon_size);

// Tokenize a sentence into words
char **tokenize_sentence(const char *sentence, int *word_count);

// Calculate compound score
float calculate_compound_score(float score_total);

// Check if a word is composed entirely of uppercase letters
int is_all_caps(const char *word);

// Calculate sentiment score based on rules
float calculate_score(const char *sentence, WordData lexicon[], int lexicon_size);

#endif // UTILITY_H