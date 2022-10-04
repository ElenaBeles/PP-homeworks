#include <iostream>
#include <omp.h>
#include <chrono>
//////////////////////////////////////////

void home3() {
    int a = 24;
    int b = 24;

    printf("bedore 1 values is  a = %d  b = %d\n", a, b);
#pragma omp parallel num_threads(2) private(a) firstprivate(b)
    {
        int tNumber;
        tNumber = omp_get_thread_num();

        a += tNumber;
        b += tNumber;
        printf("in 1 thread values is %d, a = %d b = %d\n", tNumber, a, b);
    }
    printf("after 1 values is a = %d  b = %d\n", a, b);


    printf("\nbefore 2 values is a = %d  b = %d\n", a, b);
#pragma omp parallel num_threads(4) shared(a) private(b)
    {
        int tNumber;
        tNumber = omp_get_thread_num();

        a -= tNumber;
        b -= tNumber;
        printf("in 2 thread values is %d, a = %d b = %d\n", tNumber, a, b);
    }

    printf("after 2 values is a = %d  b = %d\n", a, b);
}

//////////////////////////////////////////

void home4() {
    int a[10];
    int b[10];
    int min = 1000;
    int max = -1000;

    for (int i = 0; i < 10; i++) {
        a[i] = i * 2;
        b[i] = i * 2;
    }

#pragma omp parallel shared(a, b, min, max) num_threads(2)
    {
        if (omp_get_thread_num() == 0) {
            for (int i : a) {
                if (i < min)
                    min = i;
            }
        }
        else {
            for (int i : b) {
                if (i > max)
                    max = i;
            }
        }
    }
    printf("Min value = %d, Max value = %d", min, max);
}

//////////////////////////////////////////

void home5() {
    int a[6][8];

    srand(std::chrono::steady_clock::now().time_since_epoch().count());

    for (auto& i : a) {
        for (int& j : i) {
            j = rand() % 20 + 1;
            printf("%d ", j);
        }
        printf("\n");
    }
#pragma omp parallel sections default(none) shared(a)
    {
#pragma omp section
        {
            int sum = 0;

            for (auto& i : a) {
                for (int& j : i) {
                    sum += j;
                }
            }
            printf("1 section ---- thread - %d, answer is %d \n", omp_get_thread_num(), sum / 48);
        }
#pragma omp section
        {
            int min = 1000;
            int max = -1000;

            for (auto& i : a) {
                for (int& j : i) {
                    if (j < min)
                        min = j;
                    if (j > max)
                        max = j;
                }
            }
            printf("2 section ---- thread - %d, answer is min = %d, max = %d  \n", omp_get_thread_num(), min, max);
        }
#pragma omp section
        {
            int count = 0;

            for (auto& i : a) {
                for (int& j : i) {
                    if (j % 3 == 0)
                        count++;
                }
            }
            printf("3 section ---- thread - %d, answer is %d  \n", omp_get_thread_num(), count);
        }
    }
}

//////////////////////////////////////////

void home6() {
    int a[100];
    int sum1 = 0, sum2 = 0;

    for (int i = 0; i < 100; i++)
        a[i] = i;

#pragma omp parallel for
    for (int i = 0; i < 100; i++)
        sum1 += a[i];

    printf("parallel for answer is %d\n", sum1 / 100);

#pragma omp parallel for reduction(+:sum2)
    for (int i = 0; i < 100; i++)
        sum2 += a[i];

    printf("parallel for(reduction) answer is %d", sum2 / 100);
}

//////////////////////////////////////////

void home7() {
    int a[12], b[12], c[12];

#pragma omp parallel for num_threads(3) default(none) shared(a, b) schedule (static, 4)
    for (int i = 0; i < 12; i++) {
        a[i] = i * 2;
        b[i] = i * 4;
        printf("static example, %d thread of %d threads. a[%d] = %d, b[%d] = %d\n", omp_get_thread_num(),
            omp_get_num_threads(), i, a[i], i, b[i]);
    }

#pragma omp parallel for num_threads(4) default(none) shared(a, b, c) schedule (dynamic, 2)
    for (int i = 0; i < 12; i++) {
        c[i] = a[i] + b[i];
        printf("dynamic example, %d thread of %d threads. c[%d] = %d + %d = %d\n", omp_get_thread_num(),
            omp_get_num_threads(), i, a[i], b[i], c[i]);
    }
}

const int n = 2000;
long matrix[n][n];
long vector[n];


//////////////////////////////////////////

void home9() {
   
    int parallelAnswer[n];
    int answer[n];
    int sum = 0;

    srand(std::chrono::steady_clock::now().time_since_epoch().count());

    for (int i = 0; i < n; i++) {
        vector[i] = rand() % 10000 + 1000;
        for (int j = 0; j < n; j++)
            matrix[i][j] = rand() % 15000 + 1000;
    }

    float startTimeForParallel = float(clock()) / CLOCKS_PER_SEC;

#pragma omp parallel for default(none) shared(n, matrix, vector, parallelAnswer) schedule(dynamic, 100) reduction(+:sum)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            sum += matrix[i][j] * vector[j];
        parallelAnswer[i] = sum;
        sum = 0;
    }

    float endTimeForParallel = float(clock()) / CLOCKS_PER_SEC;

    printf("parallelAnswer (with %f sec) \n", endTimeForParallel - startTimeForParallel);

    float startTime = float(clock()) / CLOCKS_PER_SEC;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            answer[i] = matrix[i][j] * vector[j];
        answer[i] = sum;
        sum = 0;
    }

    float endTime = float(clock()) / CLOCKS_PER_SEC;
    printf("answer (with %f sec): \n", endTime - startTime);
}

//////////////////////////////////////////

void home10() {
    const int N = 6;
    const int M = 8;

    int d[N][M];
    int max = INT_MIN;
    int min = INT_MAX;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            d[i][j] = rand() % 40 + 1;
        }
    }

#pragma omp parallel for num_threads(6)
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (d[i][j] > max)
#pragma omp critical
            {
                max = d[i][j];
            }

            if (d[i][j] < min)
#pragma omp critical
            {
                min = d[i][j];
            }

        }
    }
    printf("Max: %d; Min: %d", max, min);
}

//////////////////////////////////////////

void home11() {
    const int N = 30;
    int a[N];
    for (int i = 0; i < N; i++)
    {
        a[i] = rand() % 1000 + 10;
    }

    int result = 0;

#pragma omp parallel for num_threads(4)
    for (int i = 0; i < N; i++)
    {
        if (a[i] % 9 == 0)
        {
#pragma omp atomic
            result++;
        }
    }
    printf("answer is %d", result);
}

//////////////////////////////////////////

void home12() {
    const int N = 30;
    int a[N];
    int max = INT_MIN;

    for (int i = 0; i < N; i++)
    {
        a[i] = rand() % 1000 + 10;
    }

#pragma omp parallel for num_threads(6)
    for (int i = 0; i < N; i++)
    {
        if (a[i] % 7 == 0)
        {
#pragma omp critical
            {
                if (max < a[i])
                {
                    max = a[i];
                }
            }
        }
    }

    printf("answer is %d", max);
}

int main()
{
      // home3();
      // home4();
      // home5();
      // home6();
      // home7();
      // home9();
      // home10();
      // home11();
      // home12();
}
