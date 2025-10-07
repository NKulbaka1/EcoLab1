﻿/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   EcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный файл является точкой входа
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

/* Eco OS */
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoLab1.h"

/*
 *
 * <сводка>
 *   Функция EcoMain
 * </сводка>
 *
 * <описание>
 *   Функция EcoMain - точка входа
 * </описание>
 *
 */
int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    /* Указатель на системный интерфейс */
    IEcoSystem1* pISys = 0;
    /* Указатель на интерфейс работы с системной интерфейсной шиной */
    IEcoInterfaceBus1* pIBus = 0;
    /* Указатель на интерфейс работы с памятью */
    IEcoMemoryAllocator1* pIMem = 0;
    /* Указатель на тестируемый интерфейс */
    IEcoLab1* pIEcoLab1 = 0;

    /* Переменные для тестирования */
    int32_t* generatedArray = 0;
    int32_t* sortedArray = 0;
    uint32_t arrayLength;
    uint32_t seed = 257;
    uint32_t i;
	uint32_t j;
	clock_t start;
	clock_t end;
	double workingTime;

    /* Проверка и создание системного интерфейса */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
        if (result != 0 && pISys == 0) {
            /* Освобождение системного интерфейса в случае ошибки */
            goto Release;
        }
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }

#ifdef ECO_LIB
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_1F5DF16EE1BF43B999A434ED38FE8F3A);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#endif

    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 || pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        goto Release;
    }

    /* Получение тестируемого интерфейса */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1);
    if (result != 0 || pIEcoLab1 == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }

    /* Тестирование сортировки пузырьком */
	for (j = 1; j <= 10; j++) {
		arrayLength = 10000 * j;
		result = pIEcoLab1->pVTbl->ArrayGen(pIEcoLab1, arrayLength, seed, &generatedArray);
		if (result != 0 || generatedArray == 0) {
			/* Ошибка генерации массива */
			goto Release;
		}

		start = clock();
		result = pIEcoLab1->pVTbl->BubbleSort(pIEcoLab1, generatedArray, arrayLength, &sortedArray);
		end = clock();
		if (result != 0 || sortedArray == 0) {
			/* Ошибка сортировки */
			pIMem->pVTbl->Free(pIMem, generatedArray);
			generatedArray = 0;
			goto Release;
		}

		workingTime = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("BubbleSort length = %u, time: %f s\n", arrayLength, workingTime);

		/* Проверка корректности сортировки */
		for (i = 0; i < arrayLength - 1; i++) {
			if (sortedArray[i] > sortedArray[i + 1]) {
				/* Ошибка сортировки - массив не отсортирован */
				result = -1;
				pIMem->pVTbl->Free(pIMem, generatedArray);
				pIMem->pVTbl->Free(pIMem, sortedArray);
				generatedArray = sortedArray = 0;
				goto Release;
			}
		}

		pIMem->pVTbl->Free(pIMem, generatedArray);
		pIMem->pVTbl->Free(pIMem, sortedArray);
		generatedArray = sortedArray = 0;

		/* Если дошли сюда - сортировка успешна */
		result = 0;
	}

Release:

	/* Освобождение памяти массивов */
    if (generatedArray != 0 && pIMem != 0) {
        pIMem->pVTbl->Free(pIMem, generatedArray);
    }
    if (sortedArray != 0 && pIMem != 0) {
        pIMem->pVTbl->Free(pIMem, sortedArray);
    }
    
    /* Освобождение интерфейса для работы с интерфейсной шиной */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Освобождение интерфейса работы с памятью	*/
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Освобождение тестируемого интерфейса */
    if (pIEcoLab1 != 0) {
        pIEcoLab1->pVTbl->Release(pIEcoLab1);
    }

    /* Освобождение системного интерфейса */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }

    return result;
}