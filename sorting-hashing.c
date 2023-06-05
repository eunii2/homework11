/*
 * sorting-hashing.c
 *
 *  Sorting & Hashing
 *  School of Computer Science at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ARRAY_SIZE			13	// 배열 최대 사이즈, 소수를 사용
#define MAX_HASH_TABLE_SIZE 	MAX_ARRAY_SIZE // 해시 테이블의 최대 사이즈는 배열의 최대 사이즈와 동일하게 설정

/* 필요에 따라 함수 추가 가능 */
int initialize(int **a); // 배열을 초기화하는 함수
int freeArray(int *a); // 배열에 할당된 메모리를 해제하는 함수
void printArray(int *a); // 배열의 내용을 출력하는 함수

int selectionSort(int *a); // 선택 정렬을 수행하는 함수
int insertionSort(int *a); // 삽입 정렬을 수행하는 함수
int bubbleSort(int *a); // 버블 정렬을 수행하는 함수
int shellSort(int *a); // 쉘 정렬을 수행하는 함수
/* recursive function으로 구현 */
int quickSort(int *a, int n); // 퀵 정렬을 수행하는 함수


/* hash code generator, key % MAX_HASH_TABLE_SIZE */
int hashCode(int key); // 해시 코드를 생성하는 함수

/* array a에대 한 hash table을 만든다. */
int hashing(int *a, int **ht); // 해시 테이블을 생성하는 함수

/* hash table에서 key를 찾아 hash table의 index return */
int search(int *ht, int key); // 해시 테이블에서 키를 검색하는 함수


int main()
{
    printf("[----- [Choi Gaeun] [2022078054] -----]\n");

	char command;
	int *array = NULL; // 정렬을 수행할 배열
	int *hashtable = NULL; // 해시 테이블
	int key = -1; // 컴색할 키
	int index = -1; // 키가 저장된 인덱스

	srand(time(NULL)); // 랜덤 함수의 seed 설정

	do{
		printf("----------------------------------------------------------------\n");
		printf("                        Sorting & Hashing                       \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize       = z           Quit             = q\n");
		printf(" Selection Sort   = s           Insertion Sort   = i\n");
		printf(" Bubble Sort      = b           Shell Sort       = l\n");
		printf(" Quick Sort       = k           Print Array      = p\n");
		printf(" Hashing          = h           Search(for Hash) = e\n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initialize(&array); // 배열 초기화
			break;
		case 'q': case 'Q':
			freeArray(array); // 배열 메모리 해제
			break;
		case 's': case 'S':
			selectionSort(array); // 선택 정렬 실행
			break;
		case 'i': case 'I':
			insertionSort(array); // 삽입 정렬 실행
			break;
		case 'b': case 'B':
			bubbleSort(array); // 버블 정렬 실행
			break;
		case 'l': case 'L':
			shellSort(array); // 쉘 정렬 실행
			break;
		case 'k': case 'K':
			printf("Quick Sort: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array); // 정렬 전 배열 출력
			quickSort(array, MAX_ARRAY_SIZE); // 퀵 정렬 실행
			printf("----------------------------------------------------------------\n");
			printArray(array); // 정렬 후 배열 출력

			break;

		case 'h': case 'H':
			printf("Hashing: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array); // 해시 전 배열 출력
			hashing(array, &hashtable); // 해싱 실행
			printArray(hashtable); // 해시 테이블 출력
			break;

		case 'e': case 'E':
			printf("Your Key = ");
			scanf("%d", &key); // 검색할 키 입력 받음
			printArray(hashtable); // 해시 테이블 출력
			index = search(hashtable, key); // 키 검색
			printf("key = %d, index = %d,  hashtable[%d] = %d\n", key, index, index, hashtable[index]);
			break;

		case 'p': case 'P':
			printArray(array); // 배열 출력
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initialize(int** a)
{
	int *temp = NULL;

	/* array가 NULL인 경우 메모리 할당 */
	if(*a == NULL) {
		temp = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE); // 메모리 할당
		*a = temp;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else
		temp = *a;

	/* 랜덤값을 배열의 값으로 저장 */
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		temp[i] = rand() % MAX_ARRAY_SIZE;

	return 0;
}

int freeArray(int *a)
{
	if(a != NULL)
		free(a); // 배열 메모리 해제
	return 0;
}

void printArray(int *a)
{
	if (a == NULL) {
		printf("nothing to print.\n");
		return;
	}
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		printf("a[%02d] ", i); // 인덱스 출력
	printf("\n");
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		printf("%5d ", a[i]); // 배열 원소 출력
	printf("\n");
}


int selectionSort(int *a)
{
	int min;
	int minindex;
	int i, j;

	printf("Selection Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); // 정렬 전 배열 출력

	for (i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		minindex = i;
		min = a[i];
		for(j = i+1; j < MAX_ARRAY_SIZE; j++)
		{
			if (min > a[j]) // 최소값 찾기
			{
				min = a[j];
				minindex = j;
			}
		}
		a[minindex] = a[i]; // 위치 변경
		a[i] = min;
	}

	printf("----------------------------------------------------------------\n");
	printArray(a); // 정렬 후 배열 출력
	return 0;
}

int insertionSort(int *a)
{
	int i, j, t;

	printf("Insertion Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); // 정렬 전 배열 출력

	for(i = 1; i < MAX_ARRAY_SIZE; i++)
	{
		t = a[i]; // 현재 값 저장
		j = i;
		while (a[j-1] > t && j > 0) // 이전 값들을 비교하며 현재 값이 작으면 위치 변경
		{
			a[j] = a[j-1]; // 위치 변경
			j--; // 이전 인덱스로 이동
		}
		a[j] = t; // 최종 위치에 현재 값 저장
	}

	printf("----------------------------------------------------------------\n");
	printArray(a); // 정렬 후 배열 출력

	return 0;
}

int bubbleSort(int *a)
{
	int i, j, t;

	printf("Bubble Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); // 정렬 전 배열 출력

	for(i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		for (j = 0; j < MAX_ARRAY_SIZE; j++) // j는 1부터 시작하고, 각 단계마다 마지막 원소는 확인하지 않아도 됨
		{
			if (a[j-1] > a[j]) // 이전 값이 현재 값보다 크면 위치 변경
			{
				t = a[j-1]; // 임시 변수에 이전 값 저장
				a[j-1] = a[j]; // 현재 값으로 이전 값 변경
				a[j] = t; // 임시 변수의 값을 현재 값으로 변경
			}
		}
	}

	printf("----------------------------------------------------------------\n");
	printArray(a); // 정렬 후 배열 출력

	return 0;
}

int shellSort(int *a)
{
	int i, j, k, h, v;

	printf("Shell Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); // 정렬 전 배열 출력

	for (h = MAX_ARRAY_SIZE/2; h > 0; h /= 2) // 갭을 반으로 줄이며 반ㅂㄱ
	{
		for (i = 0; i < h; i++) // 갭에 따른 그룹
		{
			for(j = i + h; j < MAX_ARRAY_SIZE; j += h) // 각 그룹에 대한 삽입 정렬
			{
				v = a[j]; // 현재 값 저장
				k = j;
				while (k > h-1 && a[k-h] > v) // 이전 값들을 비교하며 현재 값이 작으면 위치 변경
				{
					a[k] = a[k-h]; // 위치 변경
					k -= h; // 갭만큼 이전 인덱스로 이동
				}
				a[k] = v; // 최종 위치에 현재 값 저장
			}
		}
	}
	printf("----------------------------------------------------------------\n");
	printArray(a); // 정렬 후 배열 출력

	return 0;
}

int quickSort(int *a, int n)
{
	int v, t;
	int i, j;

	if (n > 1) // 배열 크기가 1 이상인 경우에만 정렬 수행
	{
		v = a[n-1]; // 피벗 값 설정
		i = -1;
		j = n - 1;

		while(1)
		{
			while(a[++i] < v); // 왼쪽부터 피벗보다 큰 값을 찾음
			while(a[--j] > v); // 오른쪽부터 피벗보다 작은 값 찾음

			if (i >= j) break; // 만약 i와 j가 교차하면 반복문 빠져나옴
			t = a[i]; // i와 j 위치의 값을 교환
			a[i] = a[j];
			a[j] = t;
		}
		t = a[i]; // i 위치의 값과 피벗 값을 교환
		a[i] = a[n-1];
		a[n-1] = t;

		quickSort(a, i); // 피벗을 기준으로 왼쪽 부분 배열에 대해 재귀적으로 퀵정렬 수행
		quickSort(a+i+1, n-i-1); // 피벗을 기준으로 오른쪽 부분 배열에 대해 재귀적으로 퀵정렬 수행
	}


	return 0;
}

int hashCode(int key) {
   return key % MAX_HASH_TABLE_SIZE; // 해시 함수 : 키를 해시 테이블 크기로 나눈 나머지
}

int hashing(int *a, int **ht)
{
	int *hashtable = NULL;

	/* hash table이 NULL인 경우 메모리 할당 */
	if(*ht == NULL) {
		hashtable = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);
		*ht = hashtable;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else {
		hashtable = *ht;	/* hash table이 NULL이 아닌경우, table 재활용, reset to -1 */
	}

	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		hashtable[i] = -1;

	int key = -1;
	int hashcode = -1;
	int index = -1;
	// 입력 배열의 모든 요소에 대해
	for (int i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		key = a[i]; // 현재 처리할 키 값을 가져옴
		hashcode = hashCode(key); // 키 값을 해시 코드로 변환

		if (hashtable[hashcode] == -1) // 해당 해시 코드 위치가 비어있으면 키 값을 저장
		{
			hashtable[hashcode] = key;
		} else 	{
			// 해당 해시 코드 위치가 이미 차있으면, 비어 있는 다음 위치를 찾아 키 값을 저장
			index = hashcode;

			while(hashtable[index] != -1)
			{
				index = (++index) % MAX_HASH_TABLE_SIZE;
			}
			hashtable[index] = key;
		}
	}

	return 0;
}

int search(int *ht, int key)
{
	int index = hashCode(key); // 검색할 키 값을 해시 코드로 변환

	if(ht[index] == key) // 해시 코드 위치에 찾는 키 값이 있으면 그 위치를 반환
		return index;

	// 해시 코드 위치에 찾는 키 값이 없으면, 그 다음 위치부터 찾아봄
	while(ht[++index] != key)
	{
		index = index % MAX_HASH_TABLE_SIZE;
	}
	return index; // 키 값을 찾으면 그 위치를 반환
}