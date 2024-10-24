// CHECK IMPORTS
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"

struct SymTableNode {
    const char* key;
    const void* value;
    struct SymTableNode* nextNode;
};

struct SymTable {
    struct SymTableNode* startNode;
};

SymTable_T SymTable_new(void) {
    SymTable_T symtable;
    symtable = (SymTable_T) malloc(sizeof(struct SymTable));
    if (symtable == NULL) {
        return NULL;
    }
    symtable -> startNode = NULL;
    return symtable;
};

void SymTable_free(SymTable_T oSymTable) {
    assert(oSymTable != NULL);
    if (oSymTable -> startNode != NULL) {    
        struct SymTableNode* cur = oSymTable -> startNode;
        struct SymTableNode* next = cur -> nextNode;

        free((char*)cur -> key);
        free((void*)cur -> value);
        free(cur -> nextNode);

        while(next != NULL) {
            cur = next;
            next = cur -> nextNode;
            free((char*)cur -> key);
            free((void*)cur -> value);
            free(cur -> nextNode);
        }
    }
}

size_t SymTable_getLength(SymTable_T oSymTable) {
    assert (oSymTable != NULL);

    struct SymTableNode* cur = oSymTable -> startNode;
    size_t size = 0;

    while (cur != NULL) {
        size++;
        cur = cur -> nextNode;
    }

    return size;
}

int SymTable_put(SymTable_T oSymTable,
    const char *pcKey, const void *pvValue) {
    
    assert (oSymTable != NULL);

    struct SymTableNode* cur = oSymTable -> startNode;
    struct SymTableNode* last = cur;

    while (cur != NULL) {
        if (*(cur -> key) == *pcKey) {
            return 0;
        }
        last = cur;
        cur = cur -> nextNode;
    }

    struct SymTableNode* newNode;

    newNode = (struct SymTableNode*)malloc(sizeof(struct SymTableNode));

    newNode -> key = (const char *) malloc(sizeof(strlen(pcKey)));
    strcpy(newNode -> key, pcKey);

    newNode -> value = pvValue;
    newNode -> nextNode = NULL;

    
    strcpy(newNode -> value, pvValue);
    last -> nextNode = newNode;

}

void *SymTable_replace(SymTable_T oSymTable,
    const char *pcKey, const void *pvValue) {
    assert (oSymTable != NULL);

    struct SymTableNode* cur = oSymTable -> startNode;

    while (cur != NULL) {
        if (*(cur -> key) == *pcKey) {
            void* oldValue = cur -> value;
            cur -> value = pvValue;
            return oldValue;
        }
        cur = cur -> nextNode;
    }
    return NULL;
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
    assert (oSymTable != NULL);

    struct SymTableNode* cur = oSymTable -> startNode;

    while (cur != NULL) {
        if (*(cur -> key) == *pcKey) {
            return 1;
        }
        cur = cur -> nextNode;
    }
    return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    assert (oSymTable != NULL);

    struct SymTableNode* cur = oSymTable -> startNode;

    while (cur != NULL) {
        if (*(cur -> key) == *pcKey) {
            return cur -> value;
        }
        cur = cur -> nextNode;
    }
    return NULL;
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
    assert (oSymTable != NULL);

    struct SymTableNode* cur = oSymTable -> startNode;
    struct SymTableNode* last = cur;

    while (cur != NULL) {
        if (*(cur -> key) == *pcKey) {
            free(cur -> value);
            free(cur -> key);
            last -> nextNode = cur -> nextNode;
            free(cur);
        }
        last = cur;
        cur = cur -> nextNode;
    }
    return NULL;
}

void SymTable_map(SymTable_T oSymTable,
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra) {
    assert (oSymTable != NULL);

    struct SymTableNode* cur = oSymTable -> startNode;

    while (cur != NULL) {
        (*pfApply)(cur -> key, (void*) cur-> value, (void*)pvExtra);
        cur = cur -> nextNode;
    }
}
