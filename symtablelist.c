/*--------------------------------------------------------------------*/
/* symtablelist.c                                                     */
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
    struct SymTableNode* startNode;
    size_t size; 
};

SymTable_T SymTable_new(void) {
    SymTable_T symtable;
    symtable = (SymTable_T) malloc(sizeof(struct SymTable));
    if (symtable == NULL) {
        return NULL;
    }
    symtable -> startNode = NULL;
    symtable -> size = 0;
    return symtable;
}

void SymTable_free(SymTable_T oSymTable) {
    assert(oSymTable != NULL);
    if (oSymTable -> startNode != NULL) {    
        struct SymTableNode* cur = oSymTable -> startNode;
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

    assert (oSymTable != NULL);
    assert (pcKey != NULL);

    cur = oSymTable -> startNode;
    last = cur;

    while (cur != NULL) {
        if (strcmp(cur -> key, pcKey) == 0) {
            return 0;
        }
        last = cur;
        cur = cur -> nextNode;
    }

    newNode = (struct SymTableNode*)malloc(sizeof(struct SymTableNode));
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
    
    if (oSymTable -> startNode == NULL) {
        oSymTable -> startNode = newNode;
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

    assert (oSymTable != NULL);
    assert (pcKey != NULL);


    cur = oSymTable -> startNode;

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
    assert (pcKey != NULL);

    assert (oSymTable != NULL);

    cur = oSymTable -> startNode;

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
    assert (pcKey != NULL);

    assert (oSymTable != NULL);

    cur = oSymTable -> startNode;

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

    assert (oSymTable != NULL);
    assert (pcKey != NULL);

    cur = oSymTable -> startNode;
    last = NULL;
    

    while (cur != NULL) {
        if (strcmp(cur -> key, pcKey) == 0) {
            (oSymTable -> size)--;
            returnVal = (void*) cur -> value;
            free(cur -> key); 
            if (last == NULL) {
                oSymTable -> startNode = cur -> nextNode;
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
    struct SymTableNode* cur;

    assert (oSymTable != NULL);
    assert (pfApply != NULL);

    cur = oSymTable -> startNode;

    while (cur != NULL) {
        (*pfApply)(cur -> key, (void*) cur-> value, (void*)pvExtra);
        cur = cur -> nextNode;
    }
}
