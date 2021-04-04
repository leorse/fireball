#include <stddef.h>

#include "linkedList.h"
#include "ctools.h"

void *__CT_creerElement(size_t tailleElt, void *lst)
{
    __CT_TYP_VOID *elt = NULL;
    __CT_TYP_VOID *ptr = NULL;

    elt = (__CT_TYP_VOID *)__CT_MALLOC(tailleElt);
    if (elt == NULL)
    {
        return NULL;
    }
    //elt->suivant
    ptr = (__CT_TYP_VOID *)elt;
    ptr->suivant = NULL;
    if (lst != NULL)
    {
        //lst->suivant
        ptr = (__CT_TYP_VOID *)lst;
        ptr->suivant = elt;
        //elt->precedent
        elt->precedent = (__CT_TYP_VOID *)lst;
    }
    else
    {
        //elt->precedent
        elt->precedent = NULL;
    }

    return elt;
}

void __CT_libererUnElement(void *elmt)
{
    __CT_TYP_VOID *ptr = NULL;
    __CT_TYP_VOID *suiv = NULL;
    __CT_TYP_VOID *prec = NULL;

    if (elmt == NULL)
    {
        return;
    }

    ptr = (__CT_TYP_VOID *)elmt;

    suiv = ptr->suivant;
    if (suiv != NULL)
    {
        suiv->precedent = ptr->precedent;
    }
    prec = ptr->precedent;
    if (prec != NULL)
    {
        prec->suivant = ptr->suivant;
    }

    __CT_FREE(ptr);
}

void __CT_libererElements(void *premierElmt)
{
    __CT_TYP_VOID *ptr = NULL;
    __CT_TYP_VOID *suiv = NULL;

    if (premierElmt == NULL)
    {
        return;
    }

    ptr = (__CT_TYP_VOID *)premierElmt;

    do
    {
        suiv = ptr->suivant;
        __CT_FREE(ptr);
        ptr = suiv;
    } while (suiv != NULL);
}

