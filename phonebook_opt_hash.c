#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdint.h>

#include "phonebook_opt_hash.h"

hashEntry* hashTable[MAX_HASH_TABLE_SIZE];

/* use djb2 hash function */

static inline uint32_t hash(char lastname[])
{
    uint32_t hashVal = 5381;
    int c;
    do {
        c = *lastname++;
        hashVal = ((hashVal << 5) + hashVal) + c;
    } while(*lastname != '\0');

    return hashVal % MAX_HASH_TABLE_SIZE;
}

entry *findName(char lastname[], entry *pHead)
{
    /* find the bucket of lastname */
    int bucket = hash(lastname);
    if(hashTable[bucket] == NULL) {
        return NULL;
    }
    pHead = hashTable[bucket]->head;

    while (pHead != NULL) {
        if (strcasecmp(lastname, pHead->lastName) == 0)
            return pHead;
        pHead = pHead->pNext;
    }
    return NULL;
}

entry *append(char lastName[], entry *e)
{
    /* check whther entry is null or not */
    assert(e != NULL && "[error] entry is null");

    /* calculate the bucket number of the lastName */
    int bucket = hash(lastName);

    /* allocate memory for the new entry and put lastName */
    entry* newEntry = (entry *)malloc(sizeof(entry));
    newEntry->pNext = NULL;
    strcpy(newEntry->lastName, lastName);

    /* link the bucket and entries */
    if( hashTable[bucket] == NULL) {
        hashTable[bucket] = (hashEntry*)malloc(sizeof(hashEntry));
        hashTable[bucket] ->head = newEntry;
        hashTable[bucket] ->tail = newEntry;
    } else {
        hashTable[bucket]->tail->pNext = newEntry;
        hashTable[bucket]->tail = newEntry;
    }

    /* e is useless in hash version, keep it for formmat of API */
    return e;
}
