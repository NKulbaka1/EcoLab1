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
    IEcoCalculatorX* pCalcX = 0;
    IEcoCalculatorY* pCalcY = 0;
    IEcoUnknown* pUnknown = 0;
    IEcoLab1* pLab1 = 0;

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

    /* Тестирование подключения интерфейсов */
    printf("=== Testing Interface Connectivity ===\n\n");
    
    /* Базовое получение интерфейсов от основного компонента */
    printf("1. Basic interface acquisition:\n");
    
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void**)&pCalcX);
    if (result == 0 && pCalcX != 0) {
        printf("   - IEcoCalculatorX acquired successfully\n");
        printf("     18 + 12 = %d\n", pCalcX->pVTbl->Addition(pCalcX, 18, 12));
    }
    
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void**)&pCalcY);
    if (result == 0 && pCalcY != 0) {
        printf("   - IEcoCalculatorY acquired successfully\n");
        printf("     9 * 8 = %d\n", pCalcY->pVTbl->Multiplication(pCalcY, 9, 8));
    }
    
    /* Проверка транзитивности через IEcoUnknown */
    printf("\n2. Interface transitiveness:\n");
    
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoUnknown, (void**)&pUnknown);
    if (result == 0 && pUnknown != 0) {
        printf("   - IEcoUnknown acquired from IEcoLab1\n");
        
        /* Из Unknown в CalculatorX */
        result = pUnknown->pVTbl->QueryInterface(pUnknown, &IID_IEcoCalculatorX, (void**)&pCalcX);
        if (result == 0 && pCalcX != 0) {
            printf("   - IEcoCalculatorX acquired from IEcoUnknown\n");
            printf("     45 - 15 = %d\n", pCalcX->pVTbl->Subtraction(pCalcX, 45, 15));
            pCalcX->pVTbl->Release(pCalcX);
        }
        
        pUnknown->pVTbl->Release(pUnknown);
    }
    
    /* Взаимные преобразования между интерфейсами */
    printf("\n3. Cross-interface conversion:\n");
    
    /* CalculatorX -> CalculatorY */
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void**)&pCalcX);
    if (result == 0 && pCalcX != 0) {
        result = pCalcX->pVTbl->QueryInterface(pCalcX, &IID_IEcoCalculatorY, (void**)&pCalcY);
        if (result == 0 && pCalcY != 0) {
            printf("   - IEcoCalculatorY acquired from IEcoCalculatorX\n");
            printf("     64 / 8 = %d\n", pCalcY->pVTbl->Division(pCalcY, 64, 8));
            pCalcY->pVTbl->Release(pCalcY);
        }
        
        /* CalculatorX -> IEcoLab1 */
        result = pCalcX->pVTbl->QueryInterface(pCalcX, &IID_IEcoLab1, (void**)&pLab1);
        if (result == 0 && pLab1 != 0) {
            printf("   - IEcoLab1 acquired from IEcoCalculatorX\n");
            pLab1->pVTbl->Release(pLab1);
        }
        
        pCalcX->pVTbl->Release(pCalcX);
    }
    
    /* CalculatorY -> CalculatorX */
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void**)&pCalcY);
    if (result == 0 && pCalcY != 0) {
        result = pCalcY->pVTbl->QueryInterface(pCalcY, &IID_IEcoCalculatorX, (void**)&pCalcX);
        if (result == 0 && pCalcX != 0) {
            printf("   - IEcoCalculatorX acquired from IEcoCalculatorY\n");
            printf("     27 + 19 = %d\n", pCalcX->pVTbl->Addition(pCalcX, 27, 19));
            pCalcX->pVTbl->Release(pCalcX);
        }
        pCalcY->pVTbl->Release(pCalcY);
    }

    /* Тестирование математических операций */
    printf("=== Testing Calculator Operations ===\n\n");
    
    /* Получаем интерфейсы для тестирования */
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void**)&pCalcX);
    if (result == 0 && pCalcX != 0) {
        result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void**)&pCalcY);
        if (result == 0 && pCalcY != 0) {
            /* Тестирование операций CalculatorX */
            printf("CalculatorX operations:\n");
            printf("  Addition:      23 + 17 = %d\n", pCalcX->pVTbl->Addition(pCalcX, 23, 17));
            printf("  Subtraction:   50 - 25 = %d\n", pCalcX->pVTbl->Subtraction(pCalcX, 50, 25));
            printf("  Addition:      100 + 200 = %d\n", pCalcX->pVTbl->Addition(pCalcX, 100, 200));
            printf("  Subtraction:   75 - 50 = %d\n", pCalcX->pVTbl->Subtraction(pCalcX, 75, 50));
            
            /* Тестирование операций CalculatorY */
            printf("\nCalculatorY operations:\n");
            printf("  Multiplication: 7 * 6 = %d\n", pCalcY->pVTbl->Multiplication(pCalcY, 7, 6));
            printf("  Division:       84 / 7 = %d\n", pCalcY->pVTbl->Division(pCalcY, 84, 7));
            printf("  Multiplication: 11 * 9 = %d\n", pCalcY->pVTbl->Multiplication(pCalcY, 11, 9));
            printf("  Division:       121 / 11 = %d\n", pCalcY->pVTbl->Division(pCalcY, 121, 11));
            
            /* Освобождение интерфейсов */
            pCalcX->pVTbl->Release(pCalcX);
            pCalcY->pVTbl->Release(pCalcY);
        }
    }

    /* Тестирование прямых компонентов */
    printf("=== Testing Direct Component Access ===\n\n");
    
    printf("Available CalculatorX implementations:\n");
    
    /* CalculatorA */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (void**)&pCalcX);
    if (result == 0 && pCalcX != 0) {
        printf("  CalculatorA: 15+8=%d, 40-12=%d\n", 
               pCalcX->pVTbl->Addition(pCalcX, 15, 8),
               pCalcX->pVTbl->Subtraction(pCalcX, 40, 12));
        pCalcX->pVTbl->Release(pCalcX);
    }
    
    /* CalculatorB */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorB, 0, &IID_IEcoCalculatorX, (void**)&pCalcX);
    if (result == 0 && pCalcX != 0) {
        printf("  CalculatorB: 22+18=%d, 60-25=%d\n", 
               pCalcX->pVTbl->Addition(pCalcX, 22, 18),
               pCalcX->pVTbl->Subtraction(pCalcX, 60, 25));
        pCalcX->pVTbl->Release(pCalcX);
    }
    
    printf("\nAvailable CalculatorY implementations:\n");
    
    /* CalculatorD */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (void**)&pCalcY);
    if (result == 0 && pCalcY != 0) {
        printf("  CalculatorD: 8*7=%d, 96/12=%d\n", 
               pCalcY->pVTbl->Multiplication(pCalcY, 8, 7),
               pCalcY->pVTbl->Division(pCalcY, 96, 12));
        pCalcY->pVTbl->Release(pCalcY);
    }
    
    /* CalculatorE */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorE, 0, &IID_IEcoCalculatorY, (void**)&pCalcY);
    if (result == 0 && pCalcY != 0) {
        printf("  CalculatorE: 13*5=%d, 65/13=%d\n", 
               pCalcY->pVTbl->Multiplication(pCalcY, 13, 5),
               pCalcY->pVTbl->Division(pCalcY, 65, 13));
        pCalcY->pVTbl->Release(pCalcY);
    }

    /* ТЕСТ: Тестирование сортировки */
    printf("\n--- Bubble Sort Performance ---\n");
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
    if (pCalcX != 0) {
        pCalcX->pVTbl->Release(pCalcX);
    }
    if (pCalcY != 0) {
        pCalcY->pVTbl->Release(pCalcY);
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