#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>


#define PIPE_NAME "my_named_pipe"
#define PIPE_NAME2 "my_named_pipe2"
#define PIPE_NAME_CREATE "my_named_create"
#define PIPE_NAME_DELETE "my_named_delete"
#define PIPE_NAME_EXIT "my_named_exit"
#define PIPE_NAME_WRITE "my_named_write"
#define BUFFER_SIZE 1024
//char array size return;
int size_char(char *array);
void remove_newline_char(char *str);
// parse string to words by using delimiter
void splitP(char *str, char **parsed, char *delim);
//char elemanı boş mu değil mi buna göre return vermekte boş ise 1 dolu ise 0 dönmektedir.
int charIsEmpty(char *str);


pthread_mutex_t mutex;
int fd;
int fd2;
int fd_write;
int fd_create;
int fd_delete;
int fd_exit;

 char* buffer ;
void *create_file(void *arg)
{   
  printf("create file komutu dinlemeye başladi\n");
   // Read from the named pipe and print the data to the console
  while (1) {
    
    ssize_t bytes_read = read(fd_create, buffer, BUFFER_SIZE);
      pthread_mutex_lock(&mutex); 
       
    
    if (bytes_read < 0) {
      perror("read");
      break;
    }
    if (bytes_read == 0) {
      break;
    }
    char command[BUFFER_SIZE];
    //printf("buffer: %s",buffer);
   char *splitBuffer[200] = {NULL};
        
    splitP(buffer,splitBuffer," ");
    sscanf(buffer, "%s", command);
    //printf("buffer: %s %d",buffer,bytes_read);


    char *fileName="yeni.txt";
     if(strcmp(command,"create")==0){
       
       int i=0;
       while(splitBuffer[i]!=NULL){
         // printf("create command %s \n",splitBuffer[i]);
          i++;
          if(i==1){
            fileName=splitBuffer[i];
          }
       }

       printf("filename: %s", fileName);
       FILE *fp;
       fp = fopen(fileName, "a+");
       fclose(fp);
      
       sprintf(buffer, "%s adli file_olusturuldu", fileName);

       // cevap gönderme başlangıç
       printf("buray akadar geldi\n");
       int bufferSize = size_char(buffer);
       printf("buffer : %s  buffer boyu %d \n", buffer, bufferSize);
       //fwrite(buffer, 1, bufferSize, stdout);
        fd2 = open(PIPE_NAME2, O_WRONLY);
          if (fd2 < 0) {
            perror("open");
            return 0;
          } 
       ssize_t bytes_written = write(fd2, buffer, bufferSize);
       if (bytes_written < 0)
       {
          perror("write");
          break;
       } // cevap gönderme bitiş
       close(fd2);

       

     }

     pthread_mutex_unlock(&mutex);
  }

  // dosya oluşturma işlemini kilitleyin
    printf("create file\n");
    // dosya oluşturma işlemleri burada gerçekleştirin

   // dosya oluşturma işlemini kilidi açın
}

void *delete_file(void *arg)
{
    printf("delete file komutu dinlemeye başladi\n");
   // Read from the named pipe and print the data to the console
  while (1) {
    
    ssize_t bytes_read = read(fd_delete, buffer, BUFFER_SIZE);
      pthread_mutex_lock(&mutex); 
       
    
    if (bytes_read < 0) {
      perror("read");
      break;
    }
    if (bytes_read == 0) {
      break;
    }
    char command[BUFFER_SIZE];
    //printf("buffer: %s",buffer);
   char *splitBuffer[200] = {NULL};
        
    splitP(buffer,splitBuffer," ");
    sscanf(buffer, "%s", command);
    //printf("buffer: %s %d",buffer,bytes_read);


    char *fileName="yeni.txt";
     if(strcmp(command,"delete")==0){
       
       int i=0;
       while(splitBuffer[i]!=NULL){
         // printf("create command %s \n",splitBuffer[i]);
          i++;
          if(i==1){
            fileName=splitBuffer[i];
          }
       }

       printf("filename: %s", fileName);
       FILE *fp;
       fp = fopen(fileName, "a+");
       fclose(fp);
      
       sprintf(buffer, "%s adli file_olusturuldu", fileName);

       // cevap gönderme başlangıç
       printf("buray akadar geldi\n");
       int bufferSize = size_char(buffer);
       printf("buffer : %s  buffer boyu %d \n", buffer, bufferSize);
       //fwrite(buffer, 1, bufferSize, stdout);
        fd2 = open(PIPE_NAME2, O_WRONLY);
          if (fd2 < 0) {
            perror("open");
            return 0;
          } 
       ssize_t bytes_written = write(fd2, buffer, bufferSize);
       if (bytes_written < 0)
       {
          perror("write");
          break;
       } // cevap gönderme bitiş
       close(fd2);

       
      printf("delete file komutundan cikis yapildi\n");
     }
 
     pthread_mutex_unlock(&mutex);
  }

  // dosya oluşturma işlemini kilitleyin
    printf("delete file\n");
   
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
  // if(strcmp(command,"exit")==0){
  //       printf("Proggress exit : worked exit command");
  //       break;
  //   }
    pthread_mutex_unlock(&mutex); // çıkış işlemini kilidi açın
}


int main(int argc, char** argv) {


// mutex'i başlatın
    pthread_mutex_init(&mutex, NULL);

 

  



    char * myfifo = PIPE_NAME;
    char * myfifo2 = PIPE_NAME2;
    char * myfifocreate = PIPE_NAME_CREATE;
    char * myfifodelete = PIPE_NAME_DELETE;
    char * myfifoexit= PIPE_NAME_EXIT;
    char * myfifowrite = PIPE_NAME_WRITE;

    mkfifo(myfifo, 0666);
    mkfifo(myfifo2, 0666);
    mkfifo(myfifocreate, 0666);
    mkfifo(myfifodelete, 0666);
    mkfifo(myfifoexit, 0666);
    mkfifo(myfifowrite, 0666);
  // Open the named pipe for reading
  fd = open(PIPE_NAME, O_RDONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }
   fd_create = open(PIPE_NAME_CREATE, O_RDONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }
   fd_delete = open(PIPE_NAME_DELETE, O_RDONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }
   fd_exit = open(PIPE_NAME_EXIT, O_RDONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }
   fd_write= open(PIPE_NAME_WRITE, O_RDONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }

  fd2 = open(PIPE_NAME2, O_WRONLY);
  if (fd2 < 0) {
    perror("open");
    return 1;
  }
  printf("all pipe open");
  // Allocate a buffer for reading from the named pipe
     buffer = malloc(BUFFER_SIZE);
  if (buffer == NULL) {
    perror("malloc");
    return 1;
  }
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
  // Clean up resources
  free(buffer);
  close(fd);
  close(fd2);
  printf("read main bitti");
  return 0;
}

void splitP(char *str, char **parsed, char *delim)
{
    int i = 0; // parsed index
    while ((parsed[i] = strsep(&str, delim)) != NULL)
    {
        if (charIsEmpty(parsed[i])) 
        {
            continue;
        }
        i++;
    }
}
int charIsEmpty(char *str)
{
    // str value is not null
    while (*str != '\0')
    {
        // if str value is not empty
        if (*str != ' ')
        {
            return 0; // False: it is not empty
        }
        str++; // increse str pointer
    }
    return 1; // True: str is empty
}
int size_char(char *array){
    int a=0; 
   while(array[a]!='\0'){
          a++;
   }
   
    return a;
}
void remove_newline_char(char *str)
{
    while (*str != '\0') // str is not null
    {
        if (*str == '\n') // str is newline character
        {
            *str = '\0'; // str assigned null character
            break;
        }
        str++; // increse str pointer
    }
}