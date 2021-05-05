#include "Digits.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

Digits* createDigits(unsigned size) {
    Digits *l = calloc(1, sizeof(Digits));
    if (l == NULL)
        checkErrors(OUT_OF_MEMORY_EXCEPTION);
    while (size > 0) {
        checkErrors(pushFront(l, 0));
        size--;
    }
    return l;
}

Digits* stringToDigits(char *str) {
    Digits *l = createDigits(0);
    int len = (int ) strlen(str), i;
    if (len != 0 && str[len - 1] == '\n')
        len--;
    for (i = 0; i < len; i++) {
        if (str[i] < 48 || str[i] > 57)
            return createDigits(1);
        checkErrors(pushFront(l,  (char) ((char) str[i] - 0x30))); // 0х30 == 48
    }
    return l;
}

Digits* digitsCpy(Digits *original) {
    Digits *newDigits = createDigits(0);
    Node *head = original->HEAD;
    while (head) {
        pushBack(newDigits, head->data);
        head = head->next;
    }
    return newDigits;
}

Errors pushFront(Digits *digits, char data) {
    Node *new = malloc(sizeof(Node));
    if (new == NULL)
        return OUT_OF_MEMORY_EXCEPTION;
    new->data = data;
    new->next = digits->HEAD;
    digits->HEAD = new;
    digits->count++;
    return NO_EXCEPTION;
}

Errors pushBack(Digits *digits, char data) {
    Node **Head = &(digits->HEAD);
    while (*Head)
        Head = &(*Head)->next;
    *Head = malloc(sizeof(**Head));
    (*Head)->data = data;
    (*Head)->data = 0;
    digits->count++;
    return NO_EXCEPTION;
}

char popFront(Digits *digits) {
    Node *tmp = digits->HEAD;
    char value;
    if (!tmp)
        return DIGITS_EMPTY_EXCEPTION;
    digits->HEAD = digits->HEAD->next;
    value = tmp->data;
    free(tmp);
    digits->count--;
    return value;
}

Node* getByIndex(Digits *digits, unsigned index) {
    Node* result = digits->HEAD;
    int i = 0;
    if (index > digits->count-1 || digits->count == 0 || index < 0) {
        checkErrors(INDEX_OUT_OF_RANGE_EXCEPTION);
        return NULL;
    }
    while (i != index && result) {
        result = result->next;
        i++;
    }
    return result;
}

char* digitsToString (Digits *digits, char optional) {
    char *str;
    int i = 0;
    if (optional != '-')
        str = malloc((digits->count + 1) * sizeof(char));
    else {
        str = malloc((digits->count + 2) * sizeof(char));
        i++;
        str[0] = optional;
    }
    if (str == NULL) {
        checkErrors(OUT_OF_MEMORY_EXCEPTION);
        return NULL;
    }
    Node **HEAD = &(digits->HEAD);
    while (*HEAD) {
        str[i] = (char) ((char) (*HEAD)->data + 0x30);
        HEAD = &(*HEAD)->next;
        i++;
    }
    str[i] = '\0';
    reverseDigits(digits);
    return str;
}

void freeDigits(Digits *digits) {
    Node *head = digits->HEAD, *tmp = NULL;
    while (head) {
        tmp = head->next;
        free(head);
        head = tmp;
    }
    free(digits);
}

void checkErrors(Errors code)  {
    if (code)
        exit(code);
}

Digits* getLonger(Digits *a, Digits *b, int *length) {
    *length = MAX(a->count, b->count);
    return (a->count == *length) ? a : b;
}

Digits* getShorter(Digits *a, Digits *b, int *length) {
    *length = MIN(a->count, b->count);
    return (b->count == *length) ? b : a;
}

Digits* reverseDigits(Digits *digits) {
    Node *head = digits->HEAD;
    Node *new_head = NULL;
    while (head) {
        Node* next = head->next;
        head->next = new_head;
        new_head = head;
        head = next;
    }
    digits->HEAD = new_head;
    return digits;
}

bool isEmpty(Digits a) {
    return a.count == 0 || a.HEAD == NULL;
}

void printDigits(Digits a) {
    Node *HEAD = a.HEAD;
    while (HEAD) {
        putchar(HEAD->data);
        HEAD = HEAD->next;
    }
}