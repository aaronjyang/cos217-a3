/*--------------------------------------------------------------------*/
/* symtable.h                                                         */
/* Author: Aaron Yang                                                 */
/*--------------------------------------------------------------------*/
#include <stdio.h> 
#include <stddef.h>
#ifndef SYMTABLE_INCLUDED
#define SYMTABLE_INCLUDED

typedef struct SymTable* SymTable_T;

/* Creates new SymTable object and returns a pointer to the created 
   object */
SymTable_T SymTable_new(void);

/* Creates new SymTable object oSymTable and returns a pointer to the 
   created object */
void SymTable_free(SymTable_T oSymTable);

/* Accepts a pointer to a SymTable oSymTable and returns the number of 
   bindings */
size_t SymTable_getLength(SymTable_T oSymTable);

/* Accepts a SymTable pointer oSymTable, string pcKey, and void 
   pointer pvValue. If the SymTable does not contain a binding with
   key pvKey, a new binding is added with key pvKey and value pvValue 
   and 1 is returned. Else, SymTable remains unchanged and 0 is 
   returned. */
int SymTable_put(SymTable_T oSymTable,
    const char *pcKey, const void *pvValue);

/* Accepts a SymTable pointer oSymTable, string pcKey, and void 
   pointer pvValue. If SymTable has a binding with the key pcKey, 
   replace the binding's value with pvValue and return a pointer to 
   the previous value. Else, return NULL. */
void *SymTable_replace(SymTable_T oSymTable,
    const char *pcKey, const void *pvValue);

/* Accepts a SymTable pointer oSymTable and string pcKey. If the 
   SymTable contains a binding with key pcKey, return 1. Else return 
   0. */
int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/* Accepts a SymTable pointer oSymTable and string pcKey. If oSymtable 
   contains a binding with a key pcKey, return the corresponding value. 
   Else, return NULL. */
void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/* Accepts a SymTable pointer oSymTable and string pcKey. If oSymTable 
   contains a binding with a key matching pcKey, remove the binding and 
   return the removed binding's value. Else, return NULL. */
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/* Accepts a SymTable pointer oSymTable; pointer to function pfApply
   accepting a string pcKey and two void pointers pvValue and pvExtra;
   and a void pointer. For every binding in oSymTable, calls pfApply by 
   passing the binding's key, binding's value, and specified void pointer. */
void SymTable_map(SymTable_T oSymTable,
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra);

#endif