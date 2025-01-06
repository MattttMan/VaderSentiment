#include "utility.h"
#include <stdio.h>

int main() {
    const char *filename = "vader_lexicon.txt";  // Path to your lexicon file
    WordData lexicon[MAX_WORDS];
    int lexicon_size = load_lexicon(filename, lexicon, MAX_WORDS);

    if (lexicon_size == 0) {
        printf("Failed to load lexicon data.\n");
        return 1;
    }

    // Array of sentences to process
    const char *sentences[] = {
        "VADER is smart, handsome, and funny.",
        "VADER is smart, handsome, and funny!",
        "VADER is very smart, handsome, and funny.",
        "VADER is VERY SMART, handsome, and FUNNY.",
        "VADER is VERY SMART, handsome, and FUNNY!!!",
        "VADER is VERY SMART, uber handsome, and FRIGGIN FUNNY!!!",
        "VADER is not smart, handsome, nor funny.",
        "At least it isn't a horrible book.",
        "The plot was good, but the characters are uncompelling and the dialog is not great.",
        "Make sure you :) or :D today!",
        "Not bad at all"
    };
    int num_sentences = sizeof(sentences) / sizeof(sentences[0]);

    for (int i = 0; i < num_sentences; i++) {
        printf("\nProcessing sentence: \"%s\"\n", sentences[i]);
        
        // Calculate the total and compound score using the lexicon
        float total_score = calculate_score(sentences[i], lexicon, lexicon_size);
        float compound_score = calculate_compound_score(total_score);
        
        //printf("Total Score: %.6f\n", total_score);
        printf("Compound Score: %.4f\n", compound_score);
    }

    return 0;
}