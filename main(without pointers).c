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

void print_mat(int N, int M, int mat[M][N]) {
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

void print_arr(int n, int* arr) {
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

void print_sub_arrs(int N, int M, Sub_arr sub_arrs[M][N]) {
    for(int i = 0; i < M; i++) {
        int changed = 0;
        for(int j = 0; j < N; j++) {
            if(sub_arrs[i][j].arr != NULL) {
                if(!changed) {
                    printf("R%i subarrs: \n", i+1);
                }
                Sub_arr* s = &sub_arrs[i][j];
                printf("a[%i,%i] to a[%i,%i] --> ", i, s->start, i, s->end);
                print_arr(N, s->arr);
                printf("\n");
                changed++;
            }
        }
        if(changed) {
            printf("\n");
        }
    }
}

int sum(int n, int* arr) {
    int sum = 0;
    for(int i = 0; i < n; i++) {
        if(arr[i] != INT_MIN)
        sum += arr[i];
    }
    return sum;
}

void copy(int n, int* source, int* target) {
    for(int i = 0; i < n; i++) {
        target[i] = source[i];
    }
}

void clear(int n, int* arr) {
    for(int i = 0; i < n; i++) {
        arr[i] = INT_MIN;
    }
}

int len(int n, int* arr) {
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

void zero_sub_arrs(int N, int M, int mat[M][N], Sub_arr sub_arrs[M][N]) {
    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++) {
            sub_arrs[i][j].arr = NULL;
        }
    }
    for(int i = 0; i < M; i++) {
        int main_idx = 0;
        for(int j = 0; j < N; j++) {
            int* sub_arr = (int*)malloc(sizeof(int) * N);
            int* temp = (int*)malloc(sizeof(int) * N);
            clear(N, sub_arr);
            clear(N, temp);
            int idx = 0;
            for(int k = j; k < N; k++) {
                temp[idx++] = mat[i][k];
                if(!sum(N, temp)) {
                    copy(N, temp, sub_arr);
                }
            }
            if(sub_arr[0] != INT_MIN && len(N, sub_arr) > 1) {
                sub_arrs[i][main_idx].row = i;
                sub_arrs[i][main_idx].start = j;
                sub_arrs[i][main_idx].end = j + len(N, sub_arr) - 1;
                sub_arrs[i][main_idx].arr = sub_arr;
                main_idx++;
            }
            else {
                free(sub_arr);
            }
            free(temp);
        }
    }
}

void counting_sort(int n, int* arr, int asc) {
    int min = arr[0];
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < min)
            min = arr[i];
        if (arr[i] > max)
            max = arr[i];
    }
    int range = max - min + 1;
    int index[range];
    int result[n];

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
}

void counting_sort_radix(int n, int* arr, int exp, int asc) {
    int min = arr[0];
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < min)
            min = arr[i];
        if (arr[i] > max)
            max = arr[i];
    }
    int range = max - min + 1;
    int index[range];
    int result[n];

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
}

void radix_sort(int n, int* arr, int asc) {
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max)
            max = arr[i];
    }

    for (int exp = 1; max / exp > 0; exp *= 10) {
        counting_sort_radix(n, arr, exp, asc);
    }
}

void heapify(int n, int arr[], int i, int asc) {
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

        heapify(n, arr, largest, asc);
    }
}

void heap_sort(int n, int arr[], int asc) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(n, arr, i, asc);

    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        heapify(i, arr, 0, asc);
    }
}

void comb_sort(int n, int arr[], int asc) {
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

void even_counting_odd_heap(int N, int M, int mat[M][N], Sub_arr sub_arrs[M][N], int alg) {
    int number_of_elements = 0;
    int rows[M];
    clear(N, rows);
    int idx = 0;
    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++) {
            if(sub_arrs[i][j].arr != NULL) {
                Sub_arr* s = &sub_arrs[i][j];
                rows[idx++] = i;
                number_of_elements += len(N, s->arr);
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
                counting_sort(N, mat[rows[i]], 0);
            } else {
                comb_sort(N, mat[rows[i]], 0);
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
                heap_sort(N, mat[rows[i]], 0);
            } else {
                radix_sort(N, mat[rows[i]], 0);
            }
        }
    }
}

int main() {
    srand(time(NULL));
    int N, M;
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
    int mat[M][N];
    for(int i = 0; i < M; i++) {
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
    print_mat(N, M, mat);
    Sub_arr sub_arrs[M][N];
    zero_sub_arrs(N, M, mat, sub_arrs);
    printf("\nSubarrays:\n\n");
    print_sub_arrs(N, M, sub_arrs);
    printf("Choose the sorting algorithms: \n");
    printf("0- Counting(even), Heap(odd).\n");
    printf("1- Comb(even), Radix(odd).\n");
    printf("Your choice: ");
    int choice;
    scanf("%i", &choice);
    even_counting_odd_heap(N, M, mat, sub_arrs, choice);
    printf("\nYour Matrix:\n");
    print_mat(N, M, mat);

    return 0;
}