#pragma once

/*#ifndef __CTOOLS_H__
#define __CTOOLS_H__*/

#define __CT_MEMORY_TEST_CRUSH 0x4A
#define __CT_SIZE_TYPE_TAILLE__ unsigned long int
#define __CT_MEMORY_TAILLE_FIC 25

#define __CT_MALLOC(TAILLE)  __CT_malloc_alloc_lst(TAILLE, __LINE__, __FILE__)
#define __CT_FREE(PTR)  __CT_free_alloc_lst(PTR)

void __CT_free_alloc_lst(void* ptr);
void* __CT_malloc_alloc_lst(__CT_SIZE_TYPE_TAILLE__ taille, int ligne, const char* fic);

//#endif