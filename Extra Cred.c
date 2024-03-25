#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated; // Tracks extra memory allocated

void *AllocateMemory(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeallocateMemory(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

void Heapify(int arr[], int n, int i) {
    // Find the largest among root, left child, and right child
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
  
    if (left < n && arr[left] > arr[largest])
      largest = left;
  
    if (right < n && arr[right] > arr[largest])
      largest = right;
  
    // Swap and continue heapifying if the root is not the largest
    if (largest != i) {
      	int tmp = arr[i];
    	arr[i] = arr[largest];
    	arr[largest] = tmp;

      	Heapify(arr, n, largest);
    }
}

// Implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void HeapSort(int arr[], int N) {
    for (int i = N / 2 - 1; i >= 0; i--) {
        Heapify(arr, N, i);
    }

    for (int i = N - 1; i > 0; i--) {
        int tmp = arr[0];
        arr[0] = arr[i];
        arr[i] = tmp;

        Heapify(arr, i, 0);
    }
}

// Implements merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void MergeSort(int pData[], int l, int r){
	int m = 0;
	if(l < r){
		m = (r + l)/2;
		MergeSort(pData,l,m);
		MergeSort(pData,m + 1, r);

		int i, j, k;
		int tmpLeftSize = m - l + 1;
		int tmpRightSize = r - m;
		

		int *tmpLeftArray = AllocateMemory(sizeof(int) * tmpLeftSize);
		int *tmpRightArray = AllocateMemory(sizeof(int) * tmpRightSize);


		for (i = 0; i < tmpLeftSize; i++){
			tmpLeftArray[i] = pData[l + i];
		}
		for (j = 0; j < tmpRightSize; j++){
			tmpRightArray[j] = pData[m + 1+ j];
		}
		
		i = 0;
		j = 0; 
		k = l; 


		while (i < tmpLeftSize && j < tmpRightSize)
		{
			if (tmpLeftArray[i] <= tmpRightArray[j])
			{
				pData[k] = tmpLeftArray[i];
				i++;
			}
			else
			{
				pData[k] = tmpRightArray[j];
				j++;
			}
			k++;
		}


		while (i < tmpLeftSize)
		{
			pData[k] = tmpLeftArray[i];
			i++;
			k++;
		}


		while (j < tmpRightSize)
		{
			pData[k] = tmpRightArray[j];
			j++;
			k++;
		}


		DeallocateMemory(tmpLeftArray);
		DeallocateMemory(tmpRightArray);
	}

}

// Implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void InsertionSort(int* pData, int n)
{
	int i, key, j;
    for (i = 1; i < n; i++) {
        key = pData[i];
        j = i - 1;
 
        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
        while (j >= 0 && pData[j] > key) {
            pData[j + 1] = pData[j];
            j = j - 1;
        }
        pData[j + 1] = key;
    }
}

// Implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void BubbleSort(int* pData, int n)
{
    while(n > 0){
        int tmpIndex = 0;
        for(int b = 1;b < n;++b){

            if(pData[tmpIndex]>pData[b]){
				
				
				int tmp = pData[tmpIndex];
    			pData[tmpIndex] = pData[b];
    			pData[b] = tmp;
                tmpIndex = b;
            }
            else{
                tmpIndex = b;
            }
        }
       
        --n;
	}
}

// Implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void SelectionSort(int* pData, int n)
{
    int i = n - 1;
    int baseIndex = 0;
    while(i >= 0){
        int tmpIndex = 0;
        int minIndex = baseIndex;
        for(int b = baseIndex;b < n ;++b){
            if(pData[tmpIndex]>pData[b] && pData[b]<pData[minIndex]){
                minIndex = b;
                tmpIndex = b;
            }
            else{
                tmpIndex = b;
            }
        }
        if(minIndex != baseIndex){
            int tmp = pData[minIndex];
    		pData[minIndex] = pData[baseIndex];
    		pData[baseIndex] = tmp;
            
        }
        ++baseIndex;
        --i;
    }
}

// Parses input file to an integer array
int ParseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	*ppData = NULL;
	int i, n, *data;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)AllocateMemory(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// Prints first and last 100 items in the data array
void PrintArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = ParseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)AllocateMemory(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		SelectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		PrintArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		InsertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		PrintArray(pDataCopy, dataSz);
		
		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		BubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		PrintArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		MergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		PrintArray(pDataCopy, dataSz);
		
        printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		HeapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		PrintArray(pDataCopy, dataSz);
	
		DeallocateMemory(pDataCopy);
		DeallocateMemory(pDataSrc);
	}
	
}