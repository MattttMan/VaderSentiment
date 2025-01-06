#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Function to count exclamation marks at the end of a sentence
int count_sentence_exclamation_marks(const char *sentence) {
    int count = 0;
    int len = strlen(sentence);
    // Start from the last character and move backwards
    for (int i = len - 1; i >= 0 && sentence[i] == '!'; i--) {
        count++;
    }
    return count;
}

// Check if a word is a positive intensifier
int is_positive_intensifier(const char *word) {
    const char *positive_intensifiers[] = {"very", "extremely", "absolutely", "completely", "incredibly", "really", "so", "totally", "particularly", "exceptionally", "remarkably", NULL};
    for (int i = 0; positive_intensifiers[i] != NULL; i++) {
        if (strcmp(word, positive_intensifiers[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Check if a word is a negative intensifier
int is_negative_intensifier(const char *word) {
    const char *negative_intensifiers[] = {"barely", "hardly", "scarcely", "somewhat", "mildly", "slightly", "partially", "fairly", "pretty much", NULL};
    for (int i = 0; negative_intensifiers[i] != NULL; i++) {
        if (strcmp(word, negative_intensifiers[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Check if a word is a negation
int is_negation(const char *word) {
    const char *negations[] = {"not", "isn’t", "doesn’t", "wasn’t", "shouldn’t", "won’t", "cannot", "can’t", "nor", "neither", "without", "lack", "missing", NULL};
    for (int i = 0; negations[i] != NULL; i++) {
        if (strcmp(word, negations[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to populate the lexicon data from a file into an array of WordData structures
int load_lexicon(const char *filename, WordData lexicon[], int max_entries) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open lexicon file");
        return 0;
    }

    char line[MAX_STRING_LENGTH * 2];
    int count = 0;

    while (fgets(line, sizeof(line), file) && count < max_entries) {
        WordData entry = {0};
        if (sscanf(line, "%s %f %f", entry.word, &entry.value1, &entry.value2) == 3) {
            lexicon[count++] = entry;
        }
    }

    fclose(file);
    return count;
}

// Function to convert a string to lowercase
void to_lower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Function to remove only periods, exclamations, and commas from a word
void remove_punctuation(char *word) {
    int i = 0, j = 0;
    while (word[i]) {
        // Keep characters that are alphabetic, digits, hyphens, or not ., !, or ,
        if (isalpha(word[i]) || word[i] == '-' || isdigit(word[i]) || (word[i] != '.' && word[i] != '!' && word[i] != ',')) {
            word[j++] = word[i];
        }
        i++;
    }
    word[j] = '\0'; // Null-terminate the modified string
}

// Function to check if a word exists in the lexicon and retrieve its sentiment score
float get_lexicon_score(const char *word, WordData lexicon[], int lexicon_size) {
    if (word == NULL)
        return 0.0;

    char cleaned_word[MAX_STRING_LENGTH];
    strncpy(cleaned_word, word, MAX_STRING_LENGTH - 1);
    cleaned_word[MAX_STRING_LENGTH - 1] = '\0';

    to_lower(cleaned_word);
    remove_punctuation(cleaned_word);

    for (int i = 0; i < lexicon_size; i++) {
        if (strcmp(lexicon[i].word, cleaned_word) == 0) {
            return lexicon[i].value1;
        }
    }
    return 0.0;
}

// Tokenize a sentence into words
char **tokenize_sentence(const char *sentence, int *word_count) {
    char **words = malloc(MAX_STRING_LENGTH * sizeof(char *));
    char sentence_copy[MAX_STRING_LENGTH * MAX_WORDS];
    strncpy(sentence_copy, sentence, sizeof(sentence_copy) - 1);
    sentence_copy[sizeof(sentence_copy) - 1] = '\0';

    char *token = strtok(sentence_copy, " ");
    *word_count = 0;

    while (token != NULL && *word_count < MAX_STRING_LENGTH) {
        words[*word_count] = strdup(token);
        (*word_count)++;
        token = strtok(NULL, " ");
    }
    return words;
}

// Calculate compound score
float calculate_compound_score(float score_total) {
    float compound = score_total / sqrt(score_total * score_total + ALPHA);
    return compound;
}

// Function to check if a word is composed entirely of uppercase letters
int is_all_caps(const char *word) {
    int i = 0;

    // Return 0 if the word is empty
    if (word[i] == '\0') {
        return 0;
    }

    // Check each character
    while (word[i] != '\0') {
        if (!isupper((unsigned char)word[i])) {
            return 0; // Not an uppercase letter, so it's not all caps
        }
        i++;
    }

    return 1; // All characters are uppercase letters
}

// Main function to calculate sentiment score based on rules
float calculate_score(const char *sentence, WordData lexicon[], int lexicon_size) {
    int word_count;
    float total_score = 0.0;
    char **words = tokenize_sentence(sentence, &word_count);

    for (int i = 0; i < word_count; i++) {
        if (strlen(words[i]) == 0) continue; // Skip empty tokens

        // Check if the original word is in ALLCAPS before any modification
        int is_allcaps = is_all_caps(words[i]);

        // Lowercase and remove punctuation
        to_lower(words[i]);
        remove_punctuation(words[i]);

        float score = get_lexicon_score(words[i], lexicon, lexicon_size);
        printf("Word: \"%s\", Base score: %.2f", words[i], score);

        float modifier = 1.0;

        // Check and apply modifiers
        if (i > 0 && is_positive_intensifier(words[i - 1])) {
            modifier *= (1 + BOOST_FACTOR);
            printf(", Positive intensifier \"%s\" applied", words[i - 1]);
        }

        if (i > 0 && is_negative_intensifier(words[i - 1])) {
            modifier *= (1 - BOOST_FACTOR);
            printf(", Negative intensifier \"%s\" applied", words[i - 1]);
        }

        if (i > 0 && is_negation(words[i - 1])) {
            modifier *= -0.5;
            printf(", Negation \"%s\" applied", words[i - 1]);
        }

        // Apply ALLCAPS emphasis if the original word was in ALLCAPS and has a lexicon score
        if (is_allcaps && score != 0) {
            modifier *= CAPS_MULTIPLIER;
            printf(", ALLCAPS emphasis applied");
        }

        // Apply the modifier to the score
        float final_score = score * modifier;
        total_score += final_score;
        printf(", Final score for word: %.4f, Updated sentence score: %.4f\n", final_score, total_score);
    }

    // Add exclamation emphasis
    int exclamation_count = count_sentence_exclamation_marks(sentence);
    float exclamation_boost = exclamation_count * EXCLAMATION_BOOST;
    total_score += exclamation_boost;
    printf("Exclamation marks found: %4d, Total exclamation boost: %.4f, Final sentence score: %.4f\n", exclamation_count, exclamation_boost, total_score);

    // Free allocated memory
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
    free(words);

    return total_score;
}

// Calculate compound score
float calculate_compound_score(float score_total) {
    float compound = score_total / sqrt(score_total * score_total + ALPHA);
    return compound;
}

// Function to check if a word is composed entirely of uppercase letters
int is_all_caps(const char *word) {
    int i = 0;

    // Return 0 if the word is empty
    if (word[i] == '\0') {
        return 0;
    }

    // Check each character
    while (word[i] != '\0') {
        if (!isupper((unsigned char)word[i])) {
            return 0; // Not an uppercase letter, so it's not all caps
        }
        i++;
    }

    return 1; // All characters are uppercase letters
}

// Main function to calculate sentiment score based on rules
float calculate_score(const char *sentence, WordData lexicon[], int lexicon_size) {
    int word_count;
    float total_score = 0.0;
    char **words = tokenize_sentence(sentence, &word_count);

    for (int i = 0; i < word_count; i++) {
        if (strlen(words[i]) == 0) continue; // Skip empty tokens

        // Check if the original word is in ALLCAPS before any modification
        int is_allcaps = is_all_caps(words[i]);

        // Lowercase and remove punctuation
        to_lower(words[i]);
        remove_punctuation(words[i]);

        float score = get_lexicon_score(words[i], lexicon, lexicon_size);
        printf("Word: \"%s\", Base score: %.2f", words[i], score);

        float modifier = 1.0;

        // Check and apply modifiers
        if (i > 0 && is_positive_intensifier(words[i - 1])) {
            modifier *= (1 + BOOST_FACTOR);
            printf(", Positive intensifier \"%s\" applied", words[i - 1]);
        }

        if (i > 0 && is_negative_intensifier(words[i - 1])) {
            modifier *= (1 - BOOST_FACTOR);
            printf(", Negative intensifier \"%s\" applied", words[i - 1]);
        }

        if (i > 0 && is_negation(words[i - 1])) {
            modifier *= -0.5;
            printf(", Negation \"%s\" applied", words[i - 1]);
        }

        // Apply ALLCAPS emphasis if the original word was in ALLCAPS and has a lexicon score
        if (is_allcaps && score != 0) {
            modifier *= CAPS_MULTIPLIER;
            printf(", ALLCAPS emphasis applied");
        }

        // Apply the modifier to the score
        float final_score = score * modifier;
        total_score += final_score;
        printf(", Final score for word: %.4f, Updated sentence score: %.4f\n", final_score, total_score);
    }

    // Add exclamation emphasis
    int exclamation_count = count_sentence_exclamation_marks(sentence);
    float exclamation_boost = exclamation_count * EXCLAMATION_BOOST;
    total_score += exclamation_boost;
    printf("Exclamation marks found: %4d, Total exclamation boost: %.4f, Final sentence score: %.4f\n", exclamation_count, exclamation_boost, total_score);

    // Free allocated memory
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
    free(words);

    return total_score;
}