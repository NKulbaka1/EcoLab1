/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoLab1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoLab1.h"

#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorC.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoLab1
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1_QueryInterface(/* in */ IEcoLab1Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoLab1) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
	else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        if (pCMe->m_pInnerUnknown != 0) {
            return pCMe->m_pInnerUnknown->pVTbl->QueryInterface(pCMe->m_pInnerUnknown, riid, ppv);
        }
        else {
            *ppv = &pCMe->m_pVTblIX;
            pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
        }
    }
	else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &pCMe->m_pVTblIY;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESES;
}

static int16_t ECOCALLMETHOD CEcoLab1_X_QueryInterface(/* in */ IEcoCalculatorXPtr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint8_t*)me - sizeof(IEcoLab1VTbl*));
    
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Если есть агрегированный компонент - делегируем ему */
    if (pCMe->m_pInnerUnknown != 0) {
        return pCMe->m_pInnerUnknown->pVTbl->QueryInterface(pCMe->m_pInnerUnknown, riid, ppv);
    }

    /* Стандартная логика QueryInterface */
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorX) || IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->m_pVTblIX;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if (IsEqualUGUID(riid, &IID_IEcoLab1)) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &pCMe->m_pVTblIY;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESES;
}

static int16_t ECOCALLMETHOD CEcoLab1_Y_QueryInterface(/* in */ IEcoCalculatorYPtr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint8_t*)me - sizeof(IEcoLab1VTbl*) - sizeof(IEcoCalculatorXVTbl*));
    
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    if (IsEqualUGUID(riid, &IID_IEcoCalculatorY) || IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->m_pVTblIY;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if (IsEqualUGUID(riid, &IID_IEcoLab1)) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &pCMe->m_pVTblIX;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoLab1
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoLab1_AddRef(/* in */ IEcoLab1Ptr_t me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return ++pCMe->m_cRef;
}

static uint32_t ECOCALLMETHOD CEcoLab1_X_AddRef(/* in */ IEcoCalculatorXPtr_t me) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint8_t*)me - sizeof(IEcoLab1VTbl*));
    return CEcoLab1_AddRef((IEcoLab1Ptr_t)pCMe);
}

static uint32_t ECOCALLMETHOD CEcoLab1_Y_AddRef(/* in */ IEcoCalculatorYPtr_t me) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint8_t*)me - sizeof(IEcoLab1VTbl*) - sizeof(IEcoCalculatorXVTbl*));
    return CEcoLab1_AddRef((IEcoLab1Ptr_t)pCMe);
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoLab1
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoLab1_Release(/* in */ IEcoLab1Ptr_t me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLab1((IEcoLab1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

static uint32_t ECOCALLMETHOD CEcoLab1_X_Release(/* in */ IEcoCalculatorXPtr_t me) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint8_t*)me - sizeof(IEcoLab1VTbl*));
    return CEcoLab1_Release((IEcoLab1Ptr_t)pCMe);
}

static uint32_t ECOCALLMETHOD CEcoLab1_Y_Release(/* in */ IEcoCalculatorYPtr_t me) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint8_t*)me - sizeof(IEcoLab1VTbl*) - sizeof(IEcoCalculatorXVTbl*));
    return CEcoLab1_Release((IEcoLab1Ptr_t)pCMe);
}

static int16_t ECOCALLMETHOD CEcoLab1_ArrayGen(
    /* in */ IEcoLab1Ptr_t me,
    /* in */ uint32_t length,
    /* in */ uint32_t seed,
    /* out */ int32_t **generatedArray)
{
    CEcoLab1 *pCMe = (CEcoLab1 *)me;
    int32_t *outputArray;
    int32_t i;
    if (me == 0 || length == 0)
    {
        return ERR_ECO_POINTER;
    }
    outputArray = (uint32_t *)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, length * sizeof(uint32_t));
    if (outputArray == 0)
    {
        return ERR_ECO_OUTOFMEMORY;
    }
    for (i = 0; i < length; i++)
    {
        outputArray[i] = ((i + 1) * seed * 42 + 228) % 1007;
    }

    *generatedArray = outputArray;

    return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция MyFunction
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1_BubbleSort(/* in */ IEcoLab1Ptr_t me, /* in */ int32_t *inputArray, /* in */ uint32_t length, /* out */ int32_t **sortedArray) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int32_t *outputArray;
    uint32_t i, j;
    int32_t temp;
	int swapped;

    /* Проверка указателей */
    if (me == 0 || inputArray == 0 || sortedArray == 0) {
        return ERR_ECO_POINTER;
    }

    /* Выделяем память для отсортированного массива */
    outputArray = (int32_t *)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, length * sizeof(int32_t));
    if (outputArray == 0) {
        return ERR_ECO_OUTOFMEMORY;
    }

    /* Копируем исходный массив */
    for (i = 0; i < length; i++) {
        outputArray[i] = inputArray[i];
    }

    /* Сортировка пузырьком */
    for (i = 0; i < length - 1; i++) {
		swapped = 0;
        for (j = 0; j < length - i - 1; j++) {
            if (outputArray[j] > outputArray[j + 1]) {
                temp = outputArray[j];
                outputArray[j] = outputArray[j + 1];
                outputArray[j + 1] = temp;
				swapped = 1;
            }
        }

		if (!swapped)
			break;
    }

    *sortedArray = outputArray;
    return ERR_ECO_SUCCESES;
}

static int32_t ECOCALLMETHOD CEcoLab1_X_Addition(/* in */ IEcoCalculatorXPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint8_t*)me - sizeof(IEcoLab1VTbl*));
    
    /* Приоритет: агрегированный компонент */
    if (pCMe->m_pInnerUnknown != 0) {
        IEcoCalculatorX* pIX = 0;
        if (pCMe->m_pInnerUnknown->pVTbl->QueryInterface(pCMe->m_pInnerUnknown, &IID_IEcoCalculatorX, (void**)&pIX) == 0) {
            int32_t result = pIX->pVTbl->Addition(pIX, a, b);
            pIX->pVTbl->Release(pIX);
            return result;
        }
    }
    
    /* Резерв: включенный компонент */
    if (pCMe->m_pIX != 0) {
        return pCMe->m_pIX->pVTbl->Addition(pCMe->m_pIX, a, b);
    }
    
    return -1; // Ошибка - нет доступного калькулятора
}

static int16_t ECOCALLMETHOD CEcoLab1_X_Subtraction(/* in */ IEcoCalculatorXPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint8_t*)me - sizeof(IEcoLab1VTbl*));
    
    if (pCMe->m_pInnerUnknown != 0) {
        IEcoCalculatorX* pIX = 0;
        if (pCMe->m_pInnerUnknown->pVTbl->QueryInterface(pCMe->m_pInnerUnknown, &IID_IEcoCalculatorX, (void**)&pIX) == 0) {
            int32_t result = pIX->pVTbl->Subtraction(pIX, a, b);
            pIX->pVTbl->Release(pIX);
            return result;
        }
    }
    
    if (pCMe->m_pIX != 0) {
        return pCMe->m_pIX->pVTbl->Subtraction(pCMe->m_pIX, a, b);
    }
    
    return -1;
}

static int32_t ECOCALLMETHOD CEcoLab1_Y_Multiplication(/* in */ IEcoCalculatorYPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint8_t*)me - sizeof(IEcoLab1VTbl*) - sizeof(IEcoCalculatorXVTbl*));
    
    if (pCMe->m_pIY != 0) {
        return pCMe->m_pIY->pVTbl->Multiplication(pCMe->m_pIY, a, b);
    }
    
    return -1;
}

static int16_t ECOCALLMETHOD CEcoLab1_Y_Division(/* in */ IEcoCalculatorYPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint8_t*)me - sizeof(IEcoLab1VTbl*) - sizeof(IEcoCalculatorXVTbl*));
    
    if (pCMe->m_pIY != 0) {
        return pCMe->m_pIY->pVTbl->Division(pCMe->m_pIY, a, b);
    }
    
    return -1;
}

/*
 *
 * <сводка>
 *   Функция Init
 * </сводка>
 *
 * <описание>
 *   Функция инициализации экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD initCEcoLab1(/*in*/ IEcoLab1Ptr_t me, /* in */ struct IEcoUnknown *pIUnkSystem) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    IEcoInterfaceBus1* pIBus = 0;
    int16_t result = -1;
	IEcoUnknown* pOuterUnknown = (IEcoUnknown*)me;	

    /* Проверка указателей */
    if (me == 0 ) {
        return result;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        return result;
    }

	 /* первый приоритет: CalculatorB*/
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorB, pOuterUnknown, &IID_IEcoUnknown, (void**)&pCMe->m_pInnerUnknown);
    if (result == 0 && pCMe->m_pInnerUnknown != 0) {
    } else {
        printf("CalculatorB not available, trying simple inclusion...\n");
        
        /* второй приоритет: CalculatorA */
        result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (void**)&pCMe->m_pIX);
        if (result == 0) {
        } else {
            /* третий приоритет: CalculatorC */
            result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorC, 0, &IID_IEcoCalculatorX, (void**)&pCMe->m_pIX);
            if (result == 0) {
            } else {
                printf("WARNING: No CalculatorX components available\n");
                pCMe->m_pIX = 0;
            }
        }
    }

    /* Загрузка CalculatorY компонентов (простое включение) */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorC, 0, &IID_IEcoCalculatorY, (void**)&pCMe->m_pIY);
    if (result != 0) {
        result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (void**)&pCMe->m_pIY);
    }
    if (result != 0) {
        result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorE, 0, &IID_IEcoCalculatorY, (void**)&pCMe->m_pIY);
    }

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

    return ERR_ECO_SUCCESES;
}

/* Create Virtual Table IEcoLab1 */
IEcoLab1VTbl g_x277FC00C35624096AFCFC125B94EEC90VTbl = {
    CEcoLab1_QueryInterface,
    CEcoLab1_AddRef,
    CEcoLab1_Release,
    CEcoLab1_BubbleSort,
	CEcoLab1_ArrayGen
};

IEcoCalculatorXVTbl g_X_x8A1ED3A6B4454B86B7C1D1E9F0A2B3C4VTbl = {
    CEcoLab1_X_QueryInterface,
    CEcoLab1_X_AddRef,
    CEcoLab1_X_Release,
    CEcoLab1_X_Addition,
    CEcoLab1_X_Subtraction
};

IEcoCalculatorYVTbl g_Y_x5F2B9C8D6E1A49479A8B6C5D4E3F2A1BVTbl = {
    CEcoLab1_Y_QueryInterface,
    CEcoLab1_Y_AddRef,
    CEcoLab1_Y_Release,
    CEcoLab1_Y_Multiplication,
    CEcoLab1_Y_Division
};

/*
 *
 * <сводка>
 *   Функция Create
 * </сводка>
 *
 * <описание>
 *   Функция создания экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD createCEcoLab1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoLab1** ppIEcoLab1) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoLab1* pCMe = 0;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;

    /* Проверка указателей */
    if (ppIEcoLab1 == 0 || pIUnkSystem == 0) {
        return result;
    }

    /* Получение системного интерфейса приложения */
    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem, (void **)&pISys);

    /* Проверка */
    if (result != 0 && pISys == 0) {
        return result;
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Получение идентификатора компонента для работы с памятью */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Получение интерфейса распределителя памяти */
    pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 && pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        pISys->pVTbl->Release(pISys);
        return result;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoLab1*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab1));

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = pISys;

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoLab1 */
    pCMe->m_pVTblIEcoLab1 = &g_x277FC00C35624096AFCFC125B94EEC90VTbl;

	/* Создание таблицы функций интерфейса IEcoCalculatorX */
    pCMe->m_pVTblIX = &g_X_x8A1ED3A6B4454B86B7C1D1E9F0A2B3C4VTbl;

    /* Создание таблицы функций интерфейса IEcoCalculatorY */
    pCMe->m_pVTblIY = &g_Y_x5F2B9C8D6E1A49479A8B6C5D4E3F2A1BVTbl;

	pCMe->m_pInnerUnknown = 0;

	pCMe->m_pIX = 0;

    pCMe->m_pIY = 0;

    /* Инициализация данных */
    pCMe->m_Name = 0;

    /* Возврат указателя на интерфейс */
    *ppIEcoLab1 = (IEcoLab1*)pCMe;

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

    return 0;
}

/*
 *
 * <сводка>
 *   Функция Delete
 * </сводка>
 *
 * <описание>
 *   Функция освобождения экземпляра
 * </описание>
 *
 */
void ECOCALLMETHOD deleteCEcoLab1(/* in */ IEcoLab1* pIEcoLab1) {
    CEcoLab1* pCMe = (CEcoLab1*)pIEcoLab1;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoLab1 != 0 ) {
        pIMem = pCMe->m_pIMem;

		/* Освобождение агрегированного компонента */
        if (pCMe->m_pInnerUnknown != 0) {
            pCMe->m_pInnerUnknown->pVTbl->Release(pCMe->m_pInnerUnknown);
        }
		
        /* Освобождение включенных компонентов */
        if (pCMe->m_pIX != 0) {
            pCMe->m_pIX->pVTbl->Release(pCMe->m_pIX);
        }
        if (pCMe->m_pIY != 0) {
            pCMe->m_pIY->pVTbl->Release(pCMe->m_pIY);
        }

        /* Освобождение */
        if ( pCMe->m_Name != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}
