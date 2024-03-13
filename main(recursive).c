#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

typedef struct {
    int row;
    int start;
    int end;
    int* arr;
} Sub_arr;

void print_mat(int** mat, int N, int M) {
    printf("\n*");
    for(int i = 0; i < N; i++) {
        printf("\t%i:", i);
    }
    printf("\n");
    for(int i = 0; i < M; i++) {
        printf("%i:", i);
        for(int j = 0; j < N; j++) {
            int val = mat[i][j];
            if(val != INT_MIN) {
                printf("\t%i", val);
            }
        }
        printf("\n");
    }
}

void print_arr(int* arr, int n) {
    if(n <= 0 || arr[0] == INT_MIN) {
        printf("{}");
        return;
    }
    printf("{%i", arr[0]);
    for(int i = 1; i < n-1; i++) {
        if(arr[i] != INT_MIN)
            printf(", %i", arr[i]);
    }
    if(arr[n-1] == INT_MIN) {
        printf("}");
    } else {
        printf(", %i}", arr[n-1]);
    }

}

void print_sub_arrs(Sub_arr*** sub_arrs, int N, int M) {
    for(int i = 0; i < M; i++) {
        int changed = 0;
        for(int j = 0; j < N; j++) {
            if(sub_arrs[i][j] != NULL) {
                if(!changed) {
                    printf("R%i subarrs: \n", i+1);
                }
                Sub_arr* s = sub_arrs[i][j];
                printf("a[%i,%i] to a[%i,%i] --> ", i, s->start, i, s->end);
                print_arr(s->arr, N);
                printf("\n");
                changed++;
            }
        }
        if(changed) {
            printf("\n");
        }
    }
}



int sum(int* arr, int n) {
    int sum = 0;
    for(int i = 0; i < n; i++) {
        if(arr[i] != INT_MIN)
        sum += arr[i];
    }
    return sum;
}

void copy(int* source, int* target, int n) {
    for(int i = 0; i < n; i++) {
        target[i] = source[i];
    }
}

void clear(int* arr, int n) {
    for(int i = 0; i < n; i++) {
        arr[i] = INT_MIN;
    }
}

int len(int* arr, int n) {
    int len = 0;
    for(int i = 0; i < n; i++) {
        if(arr[i] != INT_MIN) {
            len++;
        } else {
            return len;
        }
    }
    return len;
}

void recursive_zero_sub_arrs(int** mat, int N, int M, int i, int j, Sub_arr*** sub_arrs) {
    if (i < M) {
        if (j < N) {
            int* sub_arr = (int*)malloc(sizeof(int) * N);
            int* temp = (int*)malloc(sizeof(int) * N);
            clear(sub_arr, N);
            clear(temp, N);
            int idx = 0;
            for (int k = j; k < N; k++) {
                temp[idx++] = mat[i][k];
                if (!sum(temp, N)) {
                    copy(temp, sub_arr, N);
                }
            }
            if (sub_arr[0] != INT_MIN && len(sub_arr, N) > 1) {
                Sub_arr* res = (Sub_arr*)malloc(sizeof(Sub_arr));
                res->row = i;
                res->start = j;
                res->end = j + len(sub_arr, N) - 1;
                res->arr = sub_arr;
                sub_arrs[i][j] = res;
            } else {
                free(sub_arr);
            }
            free(temp);

            // Recursive call for the next column
            recursive_zero_sub_arrs(mat, N, M, i, j + 1, sub_arrs);
        } else {
            // Move to the next row
            recursive_zero_sub_arrs(mat, N, M, i + 1, 0, sub_arrs);
        }
    }
}

Sub_arr*** zero_sub_arrs(int** mat, int N, int M) {
    Sub_arr*** sub_arrs = (Sub_arr***)malloc(sizeof(Sub_arr**) * M);
    for (int i = 0; i < M; i++) {
        sub_arrs[i] = (Sub_arr**)malloc(sizeof(Sub_arr*) * N);
        for (int j = 0; j < N; j++) {
            sub_arrs[i][j] = NULL;
        }
    }

    recursive_zero_sub_arrs(mat, N, M, 0, 0, sub_arrs);

    return sub_arrs;
}


void counting_sort(int* arr, int n, int asc) {
    int min = arr[0];
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < min)
            min = arr[i];
        if (arr[i] > max)
            max = arr[i];
    }
    int range = max - min + 1;
    int* index = (int*)malloc(range * sizeof(int));
    int* result = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < range; i++) index[i] = 0;
    for (int i = 0; i < n; i++) index[arr[i] - min]++;
    if (asc) {
        for (int i = 1; i < range; i++)
            index[i] += index[i - 1];
    } else {
        for (int i = range - 1; i > 0; i--)
            index[i - 1] += index[i];
    }
    for (int i = n - 1; i >= 0; i--) result[--index[arr[i] - min]] = arr[i];
    for (int i = 0; i < n; i++) arr[i] = result[i];

    free(index);
    free(result);
}

void counting_sort_radix(int* arr, int n, int exp, int asc) {
    int min = arr[0];
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < min)
            min = arr[i];
        if (arr[i] > max)
            max = arr[i];
    }
    int range = max - min + 1;
    int* index = (int*)malloc(range * sizeof(int));
    int* result = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < range; i++)
        index[i] = 0;
    for (int i = 0; i < n; i++)
        index[arr[i] - min]++;
    if (asc) {
        for (int i = 1; i < range; i++)
            index[i] += index[i - 1];
    } else {
        for (int i = range - 1; i > 0; i--)
            index[i - 1] += index[i];
    }
    for (int i = n - 1; i >= 0; i--)
        result[--index[arr[i] - min]] = arr[i];
    for (int i = 0; i < n; i++)
        arr[i] = result[i];

    free(index);
    free(result);
}

void radix_sort(int* arr, int n, int asc) {
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max)
            max = arr[i];
    }

    for (int exp = 1; max / exp > 0; exp *= 10) {
        counting_sort_radix(arr, n, exp, asc);
    }
}

void heapify(int arr[], int n, int i, int asc) {
    int largest = i;
    int left_child = 2 * i + 1;
    int right_child = 2 * i + 2;

    if (asc) {
        if (left_child < n && arr[left_child] > arr[largest])
            largest = left_child;

        if (right_child < n && arr[right_child] > arr[largest])
            largest = right_child;
    } else {
        if (left_child < n && arr[left_child] < arr[largest])
            largest = left_child;

        if (right_child < n && arr[right_child] < arr[largest])
            largest = right_child;
    }

    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        heapify(arr, n, largest, asc);
    }
}

void heap_sort(int arr[], int n, int asc) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, asc);

    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        heapify(arr, i, 0, asc);
    }
}

void comb_sort(int arr[], int n, int asc) {
    const float shrink_factor = 1.3;
    int gap = n;
    int swapped = 1;

    while (gap > 1 || swapped) {
        gap = (int)(gap / shrink_factor);
        if (gap < 1) {
            gap = 1;
        }
        swapped = 0;
        for (int i = 0; i < n - gap; i++) {
            int comparison = (asc) ? (arr[i] > arr[i + gap]) : (arr[i] < arr[i + gap]);
            if (comparison) {
                int temp = arr[i];
                arr[i] = arr[i + gap];
                arr[i + gap] = temp;
                swapped = 1;
            }
        }
    }
}

void even_counting_odd_heap(int** mat, Sub_arr*** sub_arrs, int N, int M, int alg) {
    int number_of_elements = 0;
    int* rows = (int*)malloc(sizeof(int)*M);
    clear(rows, N);
    int idx = 0;
    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++) {
            if(sub_arrs[i][j] != NULL) {
                Sub_arr* s = sub_arrs[i][j];
                rows[idx++] = i;
                number_of_elements += len(s->arr, N);
            }
        }
    }
    printf("\nNumber of elements: %i", number_of_elements);
    if(number_of_elements <= 0) {
        printf("\nNot enough elements...\n");
        return;
    }
    if(number_of_elements%2 == 0) {
        printf("(even)\n");
        if(!alg) {
            printf("Applying counting sort to the rows containing the subarrays...\n");
        } else {
            printf("Applying comb sort to the rows containing the subarrays...\n");
        }
        int i = -1;
        while(rows[++i] != INT_MIN) {
            if(!alg) {
                counting_sort(mat[rows[i]], N, 0);
            } else {
                comb_sort(mat[rows[i]], N, 0);
            }
        }
    } else {
        printf("(odd)\n");
        if(!alg) {
            printf("Applying heap sort to the rows containing the subarrays...\n");
        } else {
            printf("Applying radix sort to the rows containing the subarrays...\n");
        }
        int i = -1;
        while(rows[++i] != INT_MIN) {
            if(!alg) {
                heap_sort(mat[rows[i]], N, 0);
            } else {
                radix_sort(mat[rows[i]], N, 0);
            }
        }
    }
}

int main()
{
    srand(time(NULL));
    int N,M;
    printf("Enter N:");
    scanf("%i", &N);
    printf("Enter M:");
    scanf("%i", &M);

    printf("Manual or Random?\n");
    printf("0- Manual.\n");
    printf("1- Random.\n");
    int random;
    scanf("%i", &random);
    if(!random) {
        printf("Enter the elements: \n");
    }
    int** mat = (int**)malloc(sizeof(int*)*M);
    for(int i = 0; i < M; i++) {
        mat[i] = (int*)malloc(sizeof(int)*N);
        for(int j = 0; j < N; j++) {
            if(random) {
                mat[i][j] = rand()%51-25;
            } else {
                int e;
                scanf("%i", &e);
                mat[i][j] = e;
            }
        }
    }
    printf("\nYour Matrix:\n");
    print_mat(mat, N, M);
    Sub_arr*** sub_arrs = zero_sub_arrs(mat, N, M);
    printf("\nSubarrays:\n\n");
    print_sub_arrs(sub_arrs, N, M);
    printf("Choose the sorting algorithms: \n");
    printf("0- Counting(even), Heap(odd).\n");
    printf("1- Comb(even), Radix(odd).\n");
    printf("Your choice: ");
    int choice;
    scanf("%i", &choice);
    even_counting_odd_heap(mat, sub_arrs, N, M, choice);
    printf("\nYour Matrix:\n");
    print_mat(mat, N, M);
}
