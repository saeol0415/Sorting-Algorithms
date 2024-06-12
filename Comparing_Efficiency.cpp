#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define ARRAY_SIZE 10000
#define RANDOM_DATASETS 10
#define NEARLY_SORTED_DEVIATION_RATE 0.01
#define SWAP(x, y, temp) ((temp)=(x), (x)=(y), (y)=(temp))

void bubbleSort(int arr[], int n);
void bubbleSortOptimized(int arr[], int n);
void selectionSort(int arr[], int n);
void insertionSort(int arr[], int n);
void mergeSortAdapter(int arr[], int n);
void mergeSort(int arr[], int l, int r);
void mergeSortOptimizedAdapter(int arr[], int n);
void mergeSortOptimized(int arr[], int l, int r);
void merge(int arr[], int l, int m, int r);
void mergeOptimized(int arr[], int l, int m, int r);
void quickSortAdapter(int arr[], int n);
void quickSort(int arr[], int left, int right);
void quickSortOptimizedAdapter(int arr[], int n);
void quickSortOptimized(int arr[], int left, int right);
int medianOfThree(int arr[], int left, int right);

void fillArrayRandom(int arr[], int n);
void fillArrayNearlySorted(int arr[], int n);
void fillArrayReverseSorted(int arr[], int n);
void copyArray(int src[], int dest[], int n);
double getTime(void (*sortFunc)(int[], int), int arr[], int n);

int main() {
    int arrRandom[ARRAY_SIZE][RANDOM_DATASETS];
    int arrNearlySorted[ARRAY_SIZE];
    int arrReverseSorted[ARRAY_SIZE];
    int temp[ARRAY_SIZE];
    double time;
    srand((unsigned)::time(NULL));

    void (*sortFuncs[])(int[], int) = {
        bubbleSort,
        bubbleSortOptimized,
        selectionSort,
        insertionSort,
        mergeSortAdapter,
        mergeSortOptimizedAdapter,
        quickSortAdapter,
        quickSortOptimizedAdapter
    };

    const char *sortNames[] = {
        "Bubble Sort",
        "Bubble Sort Optimized",
        "Selection Sort",
        "Insertion Sort",
        "Merge Sort",
        "Merge Sort (Memory) Optimized",
        "Quick Sort",
        "Quick Sort Optimized"
    };

    int numSorts = sizeof(sortFuncs) / sizeof(sortFuncs[0]);

    for (int i = 0; i < RANDOM_DATASETS; i++) {
        fillArrayRandom(arrRandom[i], ARRAY_SIZE);
    }
    fillArrayNearlySorted(arrNearlySorted, ARRAY_SIZE);
    fillArrayReverseSorted(arrReverseSorted, ARRAY_SIZE);

    for (int i = 0; i < numSorts; i++) {
        printf("%s:\n", sortNames[i]);

        time = 0.0;
        for (int j = 0; j < RANDOM_DATASETS; j++) {
            copyArray(arrRandom[j], temp, ARRAY_SIZE);
            time += getTime(sortFuncs[i], temp, ARRAY_SIZE);
        }
        printf("  Random arrays: %.4f seconds (%.1f ms)\n", time / 10.0, time * 100);

        copyArray(arrNearlySorted, temp, ARRAY_SIZE);
        time = getTime(sortFuncs[i], temp, ARRAY_SIZE);
        printf("  Nearly sorted array(%.0f%% Deviation): %.4f seconds (%.1f ms)\n", NEARLY_SORTED_DEVIATION_RATE*100, time, time * 1000);

        copyArray(arrReverseSorted, temp, ARRAY_SIZE);
        time = getTime(sortFuncs[i], temp, ARRAY_SIZE);
        printf("  Reverse sorted array: %.4f seconds (%.1f ms)\n", time, time * 1000);
    }

    return 0;
}

void fillArrayRandom(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % n;
    }
}

void fillArrayNearlySorted(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }

    for (int i = 0; i < NEARLY_SORTED_DEVIATION_RATE * n; i++) {
        int index1 = rand() % n;
        int index2 = rand() % n;
        int temp;
        SWAP(arr[index1], arr[index2], temp);
    }
}

void fillArrayReverseSorted(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = n - i - 1;
    }
}

void copyArray(int src[], int dest[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

double getTime(void (*sortFunc)(int[], int), int arr[], int n) {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    sortFunc(arr, n);
    gettimeofday(&end, NULL);
    return ((end.tv_sec - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
}

void bubbleSort(int arr[], int n) {
    int temp;
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                SWAP(arr[j], arr[j+1], temp);
            }
        }
    }
}

void bubbleSortOptimized(int arr[], int n) {
    int temp, swapped;
    for (int i = 0; i < n-1; i++) {
        swapped = 0;
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                SWAP(arr[j], arr[j+1], temp);
                swapped = 1;
            }
        }
        if (!swapped) return;
    }
}

void selectionSort(int arr[], int n) {
    int temp;
    for (int i = 0; i < n-1; i++) {
        int minIndex = i;
        for (int j = i+1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            SWAP(arr[i], arr[minIndex], temp);
        }
    }
}

void insertionSort(int arr[], int n) {
    int key, j;
    for (int i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void mergeSortAdapter(int arr[], int n) {
    mergeSort(arr, 0, n - 1);
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void mergeSortOptimizedAdapter(int arr[], int n) {
    mergeSortOptimized(arr, 0, n - 1);
}

void mergeSortOptimized(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSortOptimized(arr, l, m);
        mergeSortOptimized(arr, m + 1, r);
        mergeOptimized(arr, l, m, r);
    }
}

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    int L[n1], R[n2];
    for (i = 0; i < n1; i++) L[i] = arr[l + i];
    for (j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    i = 0, j = 0, k = l;
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

void mergeOptimized(int arr[], int l, int m, int r) {
    int i = l;
    int j = m + 1;
    int k = 0;
    int* temp = (int*)malloc((r - l + 1) * sizeof(int));
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    while (i <= m && j <= r) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= m) {
        temp[k++] = arr[i++];
    }

    while (j <= r) {
        temp[k++] = arr[j++];
    }

    for (i = l, k = 0; i <= r; i++, k++) {
        arr[i] = temp[k];
    }

    free(temp);
}

void quickSortAdapter(int arr[], int n) {
    quickSort(arr, 0, n - 1);
}

void quickSort(int arr[], int left, int right) {
    if (left >= right) return;
    int pivot = arr[left];
    int i = left + 1;
    int temp;

    for (int j = i; j <= right; j++) {
        if (arr[j] < pivot) {
            SWAP(arr[i], arr[j], temp);
            i++;
        }
    }
    SWAP(arr[left], arr[i - 1], temp);
    int partition = i - 1;

    quickSort(arr, left, partition - 1);
    quickSort(arr, partition + 1, right);
}

void quickSortOptimizedAdapter(int arr[], int n) {
    quickSortOptimized(arr, 0, n - 1);
}

void quickSortOptimized(int arr[], int left, int right) {
    if (left >= right) return;
    int pivot = medianOfThree(arr, left, right);
    int i = left + 1;
    int temp;

    for (int j = i; j <= right; j++) {
        if (arr[j] < pivot) {
            SWAP(arr[i], arr[j], temp);
            i++;
        }
    }
    SWAP(arr[left], arr[i - 1], temp);
    int partition = i - 1;

    quickSortOptimized(arr, left, partition - 1);
    quickSortOptimized(arr, partition + 1, right);
}

int medianOfThree(int arr[], int left, int right) {
    int mid = left + (right - left) / 2;
    int temp;
    if (arr[left] > arr[mid]) SWAP(arr[left], arr[mid], temp);
    if (arr[left] > arr[right]) SWAP(arr[left], arr[right], temp);
    if (arr[mid] > arr[right]) SWAP(arr[mid], arr[right], temp);
    return arr[mid];
}