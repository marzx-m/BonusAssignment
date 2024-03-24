/*
Mariam Befekadu
COP 3502
Bonus Programming Assignment
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int extraMemoryAllocated = 0;

void *Alloc(size_t sz) {
  extraMemoryAllocated += sz;
  size_t *ret = malloc(sizeof(size_t) + sz);
  *ret = sz;
  printf("Extra memory allocated, size: %ld\n", sz);
  return &ret[1];
}

void DeAlloc(void *ptr) {
  size_t *pSz = (size_t *)ptr - 1;
  extraMemoryAllocated -= *pSz;
  printf("Extra memory deallocated, size: %ld\n", *pSz);
  free(pSz);
}

//Sorting algorithm implementations and helper functions

void swap(int *xp, int *yp) {
  int temp = *xp;
  *xp = *yp;
  *yp = temp;
}

void selectionSort(int *pData, int n) {
  int i, j, min_idx;
  for (i = 0; i < n - 1; i++) {
    min_idx = i;
    for (j = i + 1; j < n; j++)
      if (pData[j] < pData[min_idx])
        min_idx = j;
    swap(&pData[min_idx], &pData[i]);
  }
}

void insertionSort(int *pData, int n) {
  int i, key, j;
  for (i = 1; i < n; i++) {
    key = pData[i];
    j = i - 1;
    while (j >= 0 && pData[j] > key) {
      pData[j + 1] = pData[j];
      j = j - 1;
    }
    pData[j + 1] = key;
  }
}

void bubbleSort(int *pData, int n) {
  int i, j;
  for (i = 0; i < n - 1; i++)
    for (j = 0; j < n - i - 1; j++)
      if (pData[j] > pData[j + 1])
        swap(&pData[j], &pData[j + 1]);
}

void merge(int arr[], int l, int m, int r) {
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;
  int L[n1], R[n2];
  for (i = 0; i < n1; i++)
    L[i] = arr[l + i];
  for (j = 0; j < n2; j++)
    R[j] = arr[m + 1 + j];
  i = 0;
  j = 0;
  k = l;
  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }
  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }
}

void mergeSort(int arr[], int l, int r) {
  if (l < r) {
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
  }
}

void heapify(int arr[], int n, int i) {
  int largest = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;
  if (left < n && arr[left] > arr[largest])
    largest = left;
  if (right < n && arr[right] > arr[largest])
    largest = right;
  if (largest != i) {
    swap(&arr[i], &arr[largest]);
    heapify(arr, n, largest);
  }
}

void heapSort(int arr[], int n) {
  for (int i = n / 2 - 1; i >= 0; i--)
    heapify(arr, n, i);
  for (int i = n - 1; i >= 0; i--) {
    swap(&arr[0], &arr[i]);
    heapify(arr, i, 0);
  }
}

//Parses through input file to an integer array
int parseData(char *inputFileName, int **ppData) {
  FILE *inFile = fopen(inputFileName, "r");
  int dataSz = 0;
  if (inFile) {
    fscanf(inFile, "%d", &dataSz);
    *ppData = (int *)Alloc(sizeof(int) * dataSz);
    for (int i = 0; i < dataSz; ++i) {
      fscanf(inFile, "%d", (*ppData) + i);
    }
    fclose(inFile);
  }
  return dataSz;
}

//Prints first and last 100 items in the data array
void printArray(int pData[], int dataSz) {
  int i, sz = dataSz < 100 ? dataSz : 100;
  printf("\tData:\n\t");
  for (i = 0; i < sz; ++i) {
    printf("%d ", pData[i]);
  }
  if (dataSz > 200) {
    printf("\n\t... Skipping ...\n\t");
    for (i = dataSz - sz; i < dataSz; ++i) {
      printf("%d ", pData[i]);
    }
  } else if (dataSz > 100) {
    for (i = 100; i < dataSz; ++i) {
      printf("%d ", pData[i]);
    }
  }
  printf("\n\n");
}

int main(void) {
  clock_t start, end;
  double cpu_time_used;
  char *fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};

  for (int i = 0; i < 3; ++i) {
    int *pDataSrc, *pDataCopy;
    int dataSz = parseData(fileNames[i], &pDataSrc);

    if (dataSz <= 0)
      continue;

    pDataCopy = (int *)Alloc(sizeof(int) * dataSz);

    printf("---------------------------\n");
    printf("Dataset Size : %d\n", dataSz);
    printf("---------------------------\n");

    // Execute each sort algorithm
    // Example with Selection Sort
    memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
    extraMemoryAllocated = 0; //Reset before sort
    start = clock();
    selectionSort(pDataCopy, dataSz);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Selection Sort:\n\truntime\t\t\t: %.2f seconds\n", cpu_time_used);
    printf("\textra memory allocated\t: %d bytes\n", extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    //Make sure to DeAlloc and reset extraMemoryAllocated as needed

    //Similar for insertionSort, bubbleSort, mergeSort, and heapSort

    DeAlloc(pDataCopy);
    DeAlloc(pDataSrc);
  }

  return 0;
}
