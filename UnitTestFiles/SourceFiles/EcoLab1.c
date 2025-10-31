/*
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
#include <time.h>
#include <stdio.h>

/* Подключаем интерфейсы и идентификаторы всех калькуляторов */
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"

#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorC.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"

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

    /* Переменные для тестирования калькулятора */
    int32_t addResult;
    int16_t subResult;
    int32_t mulResult;
    int32_t divResult;

    /* Переменные для тестирования агрегирования */
    IEcoCalculatorX* pIX = 0;
    IEcoCalculatorY* pIY = 0;
    int32_t directAddResult;
    int16_t directSubResult;
    int32_t directMulResult;
    int32_t directDivResult;

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

    /* ТЕСТ 1: Агрегирование через QueryInterface внешнего компонента */
    printf("--- TEST 1: Aggregation through CEcoLab1 ---\n");

    /* Получение IEcoCalculatorX через агрегирование */
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void**)&pIX);
    if (result == 0 && pIX != 0) {
        
        directAddResult = pIX->pVTbl->Addition(pIX, 15, 7);
        printf("Direct call through aggregation: 15 + 7 = %d\n", directAddResult);
        
        directSubResult = pIX->pVTbl->Subtraction(pIX, 20, 8);
        printf("Direct call through aggregation: 20 - 8 = %d\n", directSubResult);
        
        pIX->pVTbl->Release(pIX);
        pIX = 0;
    } else {
        printf("FAILED: Cannot get IEcoCalculatorX through aggregation\n");
    }

    /* Получение IEcoCalculatorY через агрегирование */
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void**)&pIY);
    if (result == 0 && pIY != 0) {

        directMulResult = pIY->pVTbl->Multiplication(pIY, 6, 7);
        printf("Direct call through aggregation: 6 * 7 = %d\n", directMulResult);

        directDivResult = pIY->pVTbl->Division(pIY, 20, 4);
        printf("Direct call through aggregation: 20 / 4 = %d\n", directDivResult);

        pIY->pVTbl->Release(pIY);
        pIY = 0;
    } else {
        printf("FAILED: Cannot get IEcoCalculatorY through aggregation\n");
    }

    /* ТЕСТ 2: Демонстрация работы всех калькуляторов напрямую через интерфейсную шину */
    printf("\n--- TEST 2: Direct access to all calculators ---\n");

    /* Тестирование всех CalculatorX компонентов */
    printf("\nTesting CalculatorX components:\n");
    
    /* CalculatorA */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (void**)&pIX);
    if (result == 0 && pIX != 0) {
        printf("CalculatorA: 10 + 5 = %d, 10 - 5 = %d\n", 
               pIX->pVTbl->Addition(pIX, 10, 5),
               pIX->pVTbl->Subtraction(pIX, 10, 5));
        pIX->pVTbl->Release(pIX);
        pIX = 0;
    }

    /* CalculatorB */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorB, 0, &IID_IEcoCalculatorX, (void**)&pIX);
    if (result == 0 && pIX != 0) {
        printf("CalculatorB: 10 + 5 = %d, 10 - 5 = %d\n", 
               pIX->pVTbl->Addition(pIX, 10, 5),
               pIX->pVTbl->Subtraction(pIX, 10, 5));
        pIX->pVTbl->Release(pIX);
        pIX = 0;
    }

	/* CalculatorC */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorC, 0, &IID_IEcoCalculatorX, (void**)&pIX);
    if (result == 0 && pIX != 0) {
        printf("CalculatorC: 10 + 5 = %d, 10 - 5 = %d\n", 
               pIX->pVTbl->Addition(pIX, 10, 5),
               pIX->pVTbl->Subtraction(pIX, 10, 5));
        pIX->pVTbl->Release(pIX);
        pIX = 0;
    }

    /* Тестирование всех CalculatorY компонентов */
    printf("\nTesting CalculatorY components:\n");
    
    /* CalculatorC */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorC, 0, &IID_IEcoCalculatorY, (void**)&pIY);
    if (result == 0 && pIY != 0) {
        printf("CalculatorC: 6 * 7 = %d, 20 / 4 = %d\n", 
               pIY->pVTbl->Multiplication(pIY, 6, 7),
               pIY->pVTbl->Division(pIY, 20, 4));
        pIY->pVTbl->Release(pIY);
        pIY = 0;
    }

    /* CalculatorD */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (void**)&pIY);
    if (result == 0 && pIY != 0) {
        printf("CalculatorD: 6 * 7 = %d, 20 / 4 = %d\n", 
               pIY->pVTbl->Multiplication(pIY, 6, 7),
               pIY->pVTbl->Division(pIY, 20, 4));
        pIY->pVTbl->Release(pIY);
        pIY = 0;
    }

    /* CalculatorE */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorE, 0, &IID_IEcoCalculatorY, (void**)&pIY);
    if (result == 0 && pIY != 0) {
        printf("CalculatorE: 6 * 7 = %d, 20 / 4 = %d\n", 
               pIY->pVTbl->Multiplication(pIY, 6, 7),
               pIY->pVTbl->Division(pIY, 20, 4));
        pIY->pVTbl->Release(pIY);
        pIY = 0;
    }

    /* ТЕСТ 3: Демонстрация свойств интерфейсов - получение разных интерфейсов через QueryInterface */
    printf("\n--- TEST 3: Interface properties demonstration ---\n");

    /* Демонстрация: получение IEcoCalculatorX от компонента, реализующего IEcoCalculatorY */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorC, 0, &IID_IEcoCalculatorY, (void**)&pIY);
    if (result == 0 && pIY != 0) {
        printf("Obtained IEcoCalculatorY from CalculatorC\n");
        
        /* Пытаемся получить IEcoCalculatorX от того же компонента */
        result = pIY->pVTbl->QueryInterface(pIY, &IID_IEcoCalculatorX, (void**)&pIX);
        if (result == 0 && pIX != 0) {
            printf("CalculatorC through IEcoCalculatorX: 8 + 3 = %d\n", pIX->pVTbl->Addition(pIX, 8, 3));
            pIX->pVTbl->Release(pIX);
            pIX = 0;
        } else {
            printf("CalculatorC does not support IEcoCalculatorX interface\n");
        }
        pIY->pVTbl->Release(pIY);
        pIY = 0;
    }

    /* Аналогичный тест для CalculatorD */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (void**)&pIY);
    if (result == 0 && pIY != 0) {
        printf("Obtained IEcoCalculatorY from CalculatorD\n");
        
        result = pIY->pVTbl->QueryInterface(pIY, &IID_IEcoCalculatorX, (void**)&pIX);
        if (result == 0 && pIX != 0) {
            printf("CalculatorD through IEcoCalculatorX: 12 + 4 = %d\n", pIX->pVTbl->Addition(pIX, 12, 4));
            pIX->pVTbl->Release(pIX);
            pIX = 0;
        } else {
            printf("CalculatorD does not support IEcoCalculatorX interface\n");
        }
        pIY->pVTbl->Release(pIY);
        pIY = 0;
    }

    /* ТЕСТ 4: Тестирование функциональности внешнего компонента */
    printf("\n--- TEST 4: CEcoLab1 functionality ---\n");
    
    /* Тестирование калькулятора через методы CEcoLab1 */
    addResult = pIEcoLab1->pVTbl->CalcAdd(pIEcoLab1, 25, 17);
    printf("CEcoLab1 Addition: 25 + 17 = %d\n", addResult);
    
    subResult = pIEcoLab1->pVTbl->CalcSubtract(pIEcoLab1, 50, 23);
    printf("CEcoLab1 Subtraction: 50 - 23 = %d\n", subResult);

    mulResult = pIEcoLab1->pVTbl->CalcMultiply(pIEcoLab1, 8, 9);
    printf("CEcoLab1 Multiplication: 8 * 9 = %d\n", mulResult);

    divResult = pIEcoLab1->pVTbl->CalcDivide(pIEcoLab1, 45, 5);
    printf("CEcoLab1 Division: 45 / 5 = %d\n", divResult);

    /* ТЕСТ 5: Тестирование сортировки */
    printf("\n--- TEST 5: Bubble Sort Performance ---\n");
    for (j = 1; j <= 5; j++) {
        arrayLength = 5000 * j;
        result = pIEcoLab1->pVTbl->ArrayGen(pIEcoLab1, arrayLength, seed, &generatedArray);
        if (result != 0 || generatedArray == 0) {
            printf("Array generation error for length %u\n", arrayLength);
            continue;
        }

        start = clock();
        result = pIEcoLab1->pVTbl->BubbleSort(pIEcoLab1, generatedArray, arrayLength, &sortedArray);
        end = clock();
        if (result != 0 || sortedArray == 0) {
            printf("Sorting error for array length %u\n", arrayLength);
            pIMem->pVTbl->Free(pIMem, generatedArray);
            generatedArray = 0;
            continue;
        }

        workingTime = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("BubbleSort length = %u, time: %f s\n", arrayLength, workingTime);

        
        pIMem->pVTbl->Free(pIMem, generatedArray);
        pIMem->pVTbl->Free(pIMem, sortedArray);
        generatedArray = sortedArray = 0;
    }

    printf("\n=== ALL TESTS COMPLETED ===\n");

Release:

    /* Освобождение интерфейсов агрегирования */
    if (pIX != 0) {
        pIX->pVTbl->Release(pIX);
    }
    if (pIY != 0) {
        pIY->pVTbl->Release(pIY);
    }

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

    /* Освобождение интерфейса работы с памятью */
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