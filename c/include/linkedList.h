#pragma once
/*#ifndef LINKED_LIST_H_INCLUDED
#define LINKED_LIST_H_INCLUDED*/

#include "ctools.h"
#include <stddef.h>

typedef struct __CT_LBL_VOID
{
    struct __CT_LBL_VOID *suivant;
    struct __CT_LBL_VOID *precedent;
} __CT_TYP_VOID;

void __CT_libererUnElement(void *);
void __CT_libererElements(void *);
void *__CT_creerElement(size_t , void *);


//#endif // LINKED_LIST_H_INCLUDED