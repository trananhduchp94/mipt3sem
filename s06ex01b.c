/*написать две программы, осуществляющие взаимодействие через разделяемую память. Первая программа должна создать сегмент разделяемой памяти и скопировать туда свой собственный код
исходный код. Вторая должна брать оттуда этот текст, печатать его на экране и удалять сегмент разделяемой памяти из системы.
*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define size 11

int main ()
{
   char     *array;
   int     shmid;
   int     new = 1 ;
   const char pathname[] = "in.txt";
   key_t   key;

   if((key = ftok(pathname , 0)) < 0){ 
     printf("Can\'t generate key\n");
     exit(-1);
   }
   if((shmid = shmget(key, size * sizeof(char), 0666 | IPC_CREAT | IPC_EXCL)) < 0){ 
     if(errno != EEXIST) {
            printf("Can't create shared memory\n");
            exit(-1);
     } else {
      if((shmid = shmget(key, size * sizeof(char), 0)) < 0){
            printf("Can't find shared memory\n");
            exit(-1);
         }
         new = 0;
       }
   }
   if((array = (char*)shmat(shmid, NULL, 0)) == (char*)(-1))
   {
      printf("Can't attach shared memory\n");
      exit(-1);
   }
   
   printf("%s\n",array);
   
   
   if(shmdt(array) < 0)
   {
      printf("Can't detach shared memory\n");
      exit(-1);
   }
   return 0;
}













