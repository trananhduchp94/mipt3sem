/*Есть большое количество экспериментальных 
данных (нужно самим сгенерировать)
вещественных чисел. Необходимо с 
помощью 
N 
потоков найти их среднее и дисперсию*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>

#define Elements 1000000
#define N 5

long double *array;

long double sum[N];
long double disp[N];
long double average, dispersion;

struct Task
{
  int b, a, index;
};

struct Thread
{
  pthread_t id;
};

void* suma(void* task) 
{
    long int i;
    
    long double sum1 = 0;
    pthread_t my_thread_id;
    for(i = ((struct Task*)task) -> a; i < ((struct Task*)task) -> b; i++)
    {
        sum1 = sum1 + array[i]; 
    } 
    sum[((struct Task*)task)->index] = sum1;
}

void* dispersion1(void* task) 
{
    long int i;
    long double sum2 = 0;
    pthread_t my_thread_id;
    for (i = ((struct Task*)task) -> a; i < ((struct Task*)task) -> b; i++)
    {
        sum2 = sum2 + (array[i] - average) * (array[i] - average);
    } 
    disp[((struct Task*)task)->index] = sum2;
    return NULL;
}

int main()
{
    int i;
    
    struct Task *tasks;
    struct Thread *threads;
    /*
     * У вас же число задач должно совпадать с числом нитей.
     * А вы выделяете память не под N элементов, а под Elements.
     */
    tasks = (struct Task *)malloc(Elements * sizeof(struct Task));
    threads = (struct Thread *)malloc(N * sizeof(struct Thread));

    for(i = 0; i < N; i++)
    {
        tasks[i].index = i;
        tasks[i].a =  i * Elements / N;
        tasks[i].b = (i + 1) * Elements / N;
    }

    if (Elements % N != 0)
    {
        tasks[N - 1].b = Elements;
    }

    array = (long double *)malloc(Elements * sizeof(long double));
    
    for (i = 0; i < Elements; i++)
    array[i] = 1 + rand() % 2;  
    
    for(i = 0; i < N; i++)
    {
        sum[i] = pthread_create(&(threads[i].id) ,
                                     (pthread_attr_t *)NULL ,
                                      suma,
                                      &tasks[i]);
    }  
     
    for(i = 0; i < N; i++)
    {
        pthread_join(threads[i].id , (void **) NULL);
    }
      
    for (i = 0; i < N; i++)
    {
       average = average + sum[i];
    } 
    
    average = average / Elements;
    
    for(i = 0; i < N; i++)
    {
        disp[i] = pthread_create(&(threads[i].id) ,
                                     (pthread_attr_t *)NULL ,
                                      dispersion1 ,
                                      &tasks[i]);
    }  
     
    for(i = 0; i < N; i++)
    {
        pthread_join(threads[i].id , (void **) NULL);
    }
   
    /*
     * У вас цикл был почему-то до N - 1.
     * Из-за это дисперсия вычислялась неверно.
     */
    for (i = 0; i < N; i++)
    {
        dispersion = dispersion + disp[i];
    }
    
    dispersion = dispersion / Elements;
    
    printf("Average = %Lf\n" , average);
    printf("Dispersion = %Lf\n" , dispersion);
    return 0;
}

