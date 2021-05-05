#ifndef BIGINTEGER_DIGITS_H
#define BIGINTEGER_DIGITS_H

#include "stdbool.h"
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef enum {
    NO_EXCEPTION = 0,
    OUT_OF_MEMORY_EXCEPTION = -1,
    DIGITS_EMPTY_EXCEPTION = -2,
    INDEX_OUT_OF_RANGE_EXCEPTION = -3
} Errors;

typedef struct Node {
    char data;
    struct Node *next;
} Node;

typedef struct Digits {
    Node *HEAD;
    unsigned count;
} Digits;

Errors pushFront(Digits*, char);
Errors pushBack(Digits*, char);
void checkErrors(Errors code);
char popFront(Digits*);

Digits* stringToDigits(char*);
char* digitsToString(Digits*, char);
void printDigits(Digits);
Digits* createDigits(unsigned int size);

Digits* digitsCpy(Digits*);
Digits* reverseDigits(Digits *Digits);
Node* getByIndex(Digits*, unsigned index);
void freeDigits(Digits*);
bool isEmpty(Digits);

Digits* getLonger(Digits *a, Digits *b, int *length);
Digits* getShorter(Digits *a, Digits *b, int *length);

#endif //BIGINTEGER_Digits_H
