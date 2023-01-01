#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
pthread_mutex_t mutex;

void *create_file(void *arg)
{   
    pthread_mutex_lock(&mutex); // dosya oluşturma işlemini kilitleyin
    printf("create file\n");
    // dosya oluşturma işlemleri burada gerçekleştirin

    pthread_mutex_unlock(&mutex); // dosya oluşturma işlemini kilidi açın
}

void *delete_file(void *arg)
{
    pthread_mutex_lock(&mutex); // dosya silme işlemini kilitleyin
 printf("delete file\n");
    // dosya silme işlemleri burada gerçekleştirin

    pthread_mutex_unlock(&mutex); // dosya silme işlemini kilidi açın
}

void *write_file(void *arg)
{
    pthread_mutex_lock(&mutex); // dosya yazma işlemini kilitleyin
 printf("write file\n");
    // dosya yazma işlemleri burada gerçekleştirin

    pthread_mutex_unlock(&mutex); // dosya yazma işlemini kilidi açın
}

void *exit_program(void *arg)
{
    pthread_mutex_lock(&mutex); // çıkış işlemini kilitleyin
          printf("exit file\n");
    // çıkış işlemleri burada gerçekleştirin

    pthread_mutex_unlock(&mutex); // çıkış işlemini kilidi açın
}

int main(int argc, char *argv[])
{
    // mutex'i başlatın
    pthread_mutex_init(&mutex, NULL);

    // thread'leri oluşturun
    pthread_t thread1, thread2, thread3, thread4;
    pthread_create(&thread1, NULL, create_file, NULL);
    pthread_create(&thread2, NULL, delete_file, NULL);
    pthread_create(&thread3, NULL, write_file, NULL);
    pthread_create(&thread4, NULL, exit_program, NULL);

    // thread'lerin bitmesini bekleyin
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);

    // mutex'i temizleyin
    pthread_mutex_destroy(&mutex);

    return 0;
}