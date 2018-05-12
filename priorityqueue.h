#ifndef S02P01_PRIORITYQUEUE_H
#define S02P01_PRIORITYQUEUE_H

#include <stdio.h>
#include <malloc.h>
/*
 * Created by marek.hasselder on 04/27/2018
 */

struct priorityqueue_t {
    struct pqentry_t *first;
    struct pqentry_t *last;
    int entries;
};

struct pqentry_t {
    char *value;
    float priority;
    struct pqentry_t *next_entry;
};

/**
 * Logs an error
 * @param message
 */
void logError(char message[]) {
    printf("ERROR: %s", message);
}

/**
 * Creates an empty queue
 * @return struct priorityqueue_t
 */
struct priorityqueue_t *pqueue_create() {
    struct priorityqueue_t *q = malloc(sizeof(struct priorityqueue_t));

    if (q) {
        q->first = NULL;
        q->last = NULL;
        q->entries = 0;
        return q;
    }

    logError("could not allocate memory for queue");
}

struct pqentry_t *create_entry(char *value, float p) {
    struct pqentry_t *newEntry = (struct pqentry_t *) malloc(sizeof(struct pqentry_t));

    if (!newEntry) {
        logError("malloc error!\n");
        return 0;
    }
    newEntry->value = value;
    newEntry->priority = p;
    newEntry->next_entry = NULL;
    return newEntry;
}

/**
 * inserts a new element into the queue
 * @param pq
 * @param value
 * @param p
 */
void pqueue_insert(struct priorityqueue_t *pq, char *value, float p) {
    struct pqentry_t *newEntry = create_entry(value, p);

    if (pq->entries == 0) {
        pq->first = newEntry;
        pq->last = newEntry;
        pq->entries = 1;
        return;
    }

    if (newEntry->priority >= pq->last->priority) {
        struct pqentry_t *lastEntry = pq->last;
        lastEntry->next_entry = newEntry;
        pq->last = newEntry;
    } else if (newEntry->priority <= pq->first->priority) {
        struct pqentry_t *firstStoredEvent = pq->first;
        pq->first = newEntry;
        newEntry->next_entry = firstStoredEvent;
    } else {
        struct pqentry_t *currentEvent = pq->first;
        while (currentEvent->next_entry != NULL) {
            if (newEntry->priority <= currentEvent->next_entry->priority) {
                if (newEntry->priority >= currentEvent->priority) {
                    struct pqentry_t *next = currentEvent->next_entry;
                    currentEvent->next_entry = newEntry;
                    newEntry->next_entry = next;
                    break;
                }
            }
            currentEvent = currentEvent->next_entry;
        }
    }
    pq->entries++;
}

/**
 * returns the element value with the highest priority
 * @param pq
 * @return
 */
char *pqueue_extractMin(struct priorityqueue_t *pq) {
    if (pq->entries == 0) {
        logError("No entries found.\n");
        return NULL;
    }
    struct pqentry_t *currentEntry = pq->first;
    pq->first = currentEntry->next_entry;
    char *val = currentEntry->value;

    if (pq->entries == 1) {
        pq->first = NULL;
        pq->last = NULL;
    }
    //Debugging values
//    float priority = currentEntry->priority;
//    printf("Value: %d Priority: %f\n", val, priority);
    pqueue_remove(pq, val);
    return val;
}

/**
 * changes the priority of an element within the queue
 * @param pq
 * @param value
 * @param p
 */
void pqueue_decreaseKey(struct priorityqueue_t *pq, char *value, float p) {
    //TODO improve? -> dont delete, change value and pointer in queue
    pqueue_remove(pq, value);
    pqueue_insert(pq, value, p);
}

/**
 * removes an element of the queue by the given value
 * @param pq
 * @param value
 */
void pqueue_remove(struct priorityqueue_t *pq, char *value) {
    struct pqentry_t *currentEntry = pq->first;
    struct pqentry_t *previousEntry = NULL;
    if (currentEntry == NULL) {
        return;
    }

    while (currentEntry->next_entry != NULL) {
        if (currentEntry->value == value) {
            if (previousEntry == NULL) {
                pq->first = currentEntry->next_entry;
                free(currentEntry);
                break;
            }
            previousEntry->next_entry = currentEntry->next_entry;
            free(currentEntry);
            break;
        }
        previousEntry = currentEntry;
        currentEntry = currentEntry->next_entry;
    }
    pq->entries--;
}

/**
 * destroy's the queue
 * @param pq
 */
void pqueue_destroy(struct priorityqueue_t *pq) {
    if (pq) {
        if (pq->first) { //free all elements
            struct pqentry_t *current = pq->first;
            while (current->next_entry) {
                struct pqentry_t *element = current;
                current = current->next_entry;
                free(element);
            }
        }
        pq = NULL; //TODO check if needed
        free(pq);
    }
}

#endif //S02P01_PRIORITYQUEUE_H
