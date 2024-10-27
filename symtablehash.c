/*--------------------------------------------------------------------*/
/* symtablehash.c                                                     */
/* Author: Aaron Yang                                                 */
/*--------------------------------------------------------------------*/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "symtable.h"

struct SymTableNode {
    /* String representing the binding's key */
    char* key;

    /* Void pointer to the binding's value */
    const void* value;

    /* Pointer to the next binding in the SymTable */
    struct SymTableNode* nextNode;
};

struct SymTable {
    struct SymTableNode** startNodes;
    size_t numBuckets;
    size_t size; 
};

/* Return a hash code for pcKey that is between 0 and uBucketCount-1,
   inclusive. */

static size_t SymTable_hash(const char *pcKey, size_t uBucketCount)
{
    const size_t HASH_MULTIPLIER = 65599;
    size_t u;
    size_t uHash = 0;

    assert(pcKey != NULL);

    for (u = 0; pcKey[u] != '\0'; u++)
        uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];
    
    return uHash % uBucketCount;
}

SymTable_T SymTable_new(void) {
    SymTable_T symtable;
    int i;
    symtable = (SymTable_T) malloc(sizeof(struct SymTable));
    if (symtable == NULL) {
        return NULL;
    }
    
    symtable -> startNodes = (struct SymTableNode**) calloc(509, sizeof(struct SymTableNode *));

    if (symtable -> startNodes == NULL) {
        return NULL;
    }
    for (i = 0; i < 509; i++) {
        (symtable -> startNodes)[i] = NULL;
    }

    symtable -> size = 0;
    symtable -> numBuckets = 509;
    return symtable;
}

void SymTable_free(SymTable_T oSymTable) {
    size_t i;
    assert(oSymTable != NULL);
    for (i = 0; i < oSymTable -> numBuckets; i++) {
        if ((oSymTable -> startNodes)[i] != NULL) {    
            struct SymTableNode* cur = (oSymTable -> startNodes)[i];
            struct SymTableNode* next = cur -> nextNode;

            free((char*)cur -> key);
            free(cur);
        
            while(next != NULL) {
                cur = next;
                next = cur -> nextNode;
                free((char*)cur -> key);
                free(cur);
            }
        }
    }
    free(oSymTable -> startNodes);
    free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable) {
    return oSymTable -> size;
}

int SymTable_put(SymTable_T oSymTable,
    const char *pcKey, const void *pvValue) {
    struct SymTableNode* cur;
    struct SymTableNode* last;
    struct SymTableNode* newNode;
    size_t hash;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    
    hash = SymTable_hash(pcKey, oSymTable -> numBuckets);

    

    cur = (oSymTable -> startNodes)[hash];
    
    last = cur;

    while (cur != NULL) {
        if (strcmp(cur -> key, pcKey) == 0) {
            return 0;
        }
        last = cur;
        cur = cur -> nextNode;
    }

    newNode = (struct SymTableNode*) malloc(sizeof(struct SymTableNode));

    if (newNode == NULL) {
        return 0;
    }
    
    newNode -> key = (char *) malloc(sizeof(char) * (strlen(pcKey) + 1));
    if (newNode -> key == NULL) {
        return 0;
    }
    strcpy(newNode -> key, pcKey);

    newNode -> value = pvValue;
    newNode -> nextNode = NULL;

    if ((oSymTable -> startNodes)[hash] == NULL) {
        (oSymTable -> startNodes)[hash] = newNode;
        (oSymTable -> size)++;
        return 1;
    }
    last -> nextNode = newNode;
    (oSymTable -> size)++;

    return 1;
}

void *SymTable_replace(SymTable_T oSymTable,
    const char *pcKey, const void *pvValue) {

    struct SymTableNode* cur;
    size_t hash = SymTable_hash(pcKey, oSymTable -> numBuckets);


    assert (oSymTable != NULL);
    assert (pcKey != NULL);

    cur = (oSymTable -> startNodes)[hash];

    while (cur != NULL) {
        if (strcmp(cur -> key, pcKey) == 0) {
            void* oldValue = (void*)cur -> value;
            cur -> value = pvValue;
            return oldValue;
        }
        cur = cur -> nextNode;
    }
    return NULL;

}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
    struct SymTableNode* cur;
    size_t hash = SymTable_hash(pcKey, oSymTable -> numBuckets);
    assert (pcKey != NULL);

    assert (oSymTable != NULL);

    cur = (oSymTable -> startNodes)[hash];

    while (cur != NULL) {
        if(strcmp(cur -> key, pcKey) == 0) {
            return 1;
        }
        cur = cur -> nextNode;
    }
    return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    struct SymTableNode* cur;
    size_t hash = SymTable_hash(pcKey, oSymTable -> numBuckets);

    assert (pcKey != NULL);
    assert (oSymTable != NULL);

    cur = (oSymTable -> startNodes)[hash];

    while (cur != NULL) {
        if (strcmp(cur -> key, pcKey) == 0) {
            return (void*) cur -> value;
        }
        cur = cur -> nextNode;
    }
    return NULL;
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
    struct SymTableNode* cur;
    struct SymTableNode* last;
    void* returnVal;
    size_t hash;
    assert (oSymTable != NULL);
    assert (pcKey != NULL);
    
    hash = SymTable_hash(pcKey, oSymTable -> numBuckets);

    cur = (oSymTable -> startNodes)[hash];
    last = NULL;
    

    while (cur != NULL) {
        if (strcmp(cur -> key, pcKey) == 0) {
            (oSymTable -> size)--;
            returnVal = (void*) cur -> value;
            free(cur -> key); 
            if (last == NULL) {
                (oSymTable -> startNodes)[hash] = cur -> nextNode;
            } else {
                last -> nextNode = cur -> nextNode;
            }
            free(cur);
            return returnVal;
        }
        last = cur;
        cur = cur -> nextNode;
    }

    return NULL;
}

void SymTable_map(SymTable_T oSymTable,
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra) {
    size_t i;
    struct SymTableNode* cur;

    assert (oSymTable != NULL);
    assert (pfApply != NULL);

    for (i = 0; i < oSymTable -> numBuckets; i++) {
        cur = (oSymTable -> startNodes)[i];

        while (cur != NULL) {
            (*pfApply)(cur -> key, (void*) cur-> value, (void*)pvExtra);
            cur = cur -> nextNode;
        }
    }
}