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
#define PIPE_NAME_READ "my_named_read"
#define PIPE_NAME_WRITE "my_named_write"
#define PIPE_NAME_EXIT "my_named_exit"

#define BUFFER_SIZE 1024
//char array size return;
int size_char(char *array);
void remove_newline_char(char *str);
// parse string to words by using delimiter
void splitP(char *str, char **parsed, char *delim);
//char elemanı boş mu değil mi buna göre return vermekte boş ise 1 dolu ise 0 dönmektedir.
int charIsEmpty(char *str);
//eger char degeri içeriyorsa 1 dönüyor.


pthread_mutex_t mutex;
int fd;
int fd2;
int fd_create;
int fd_delete;
int fd_write;
int fd_read;
int fd_exit;
 char* buffer ;
 char* buffer_create ;
 char* buffer_delete ;
 char* buffer_read ;
 char* buffer_write ;
 char* buffer_exit ;

void *create_file(void *arg)
{   
  printf("create file komutu dinlemeye başladi\n");
   // Read from the named pipe and print the data to the console
  while (1) {
    memset(buffer_create, 0, sizeof(buffer_create));
    ssize_t bytes_read_create = read(fd_create, buffer_create, BUFFER_SIZE);
      pthread_mutex_lock(&mutex); 
       
    
    if (bytes_read_create < 0) {
      perror("read");
      break;
    }
    if (bytes_read_create == 0) {
      break;
    }
    char command[BUFFER_SIZE];
    //printf("buffer: %s",buffer);
   char *splitBuffer[200] = {NULL};
        
    splitP(buffer_create,splitBuffer," ");
    sscanf(buffer_create, "%s", command);
    //printf("buffer: %s %d",buffer,bytes_read);

     printf("buraya geldi");
    char *fileName="yeni.txt";
     if(strcmp(command,"create")==0){
       
       int i=0;
       while(splitBuffer[i]!=NULL){
         // printf("create command %s \n",splitBuffer[i]);
        
          if(strstr(splitBuffer[i],".txt")!=NULL){
            fileName=splitBuffer[i];
          }
          i++;
          
       }

       printf("filename: %s", fileName);
       FILE *fp;
       fp = fopen(fileName, "a+");
       fclose(fp);
   
       
      

       // cevap gönderme başlangıç
       
       int bufferSize = size_char(buffer_create);
       printf("buffer : %s  buffer boyu %d \n", buffer_create, bufferSize);
       //fwrite(buffer, 1, bufferSize, stdout);
      
       ssize_t bytes_written_create = write(fd2, buffer_create, bufferSize);
       if (bytes_written_create < 0)
       {
          perror("write");
          break;
       } // cevap gönderme bitiş

       close(fd2);
       fd2 = open(PIPE_NAME2, O_WRONLY);
          if (fd2 < 0) {
            perror("open");
            return 1;
          } 
       

     }
     printf("create cikis yapti\n");
     pthread_mutex_unlock(&mutex);
  }
  close(fd_create);
 
  // dosya oluşturma işlemini kilitleyin
    printf("create file\n");
    // dosya oluşturma işlemleri burada gerçekleştirin

   // dosya oluşturma işlemini kilidi açın
}

void *delete_file(void *arg){
    printf("delete file komutu dinlemeye başladi\n");
  while (1) {
    
    ssize_t bytes_read_delete = read(fd_delete, buffer_delete, BUFFER_SIZE);
      pthread_mutex_lock(&mutex); 
       
    
    if (bytes_read_delete < 0) {
      perror("read");
      break;
    }
    if (bytes_read_delete == 0) {
      break;
    }
    char command[BUFFER_SIZE];
    //printf("buffer: %s",buffer);
   char *splitBuffer[200] = {NULL};
        
    splitP(buffer_delete,splitBuffer," ");
    sscanf(buffer_delete, "%s", command);
    //printf("buffer: %s %d",buffer,bytes_read);

    
    char *fileName="yeni.txt";
    char *fileNameTemp="yeni.txt";
     if(strcmp(command,"delete")==0){
       
       int i=0;
       while(splitBuffer[i]!=NULL){
         // printf("create command %s \n",splitBuffer[i]);
         
          if(i==1){
            printf("splitBuffer i: %s \n",splitBuffer[i]);
            fileName=splitBuffer[i];
            fileNameTemp=splitBuffer[i];
          }
           i++;
       }
       char *temp;
      // printf("filename: %s\n", fileName);
      //  printf("filenameTemp: %s\n", fileNameTemp);
       if (remove(fileName) ==0) {
        
          printf("filenameTemp: %s\n", fileNameTemp);
         
          
         //sprintf(buffer, "%s adli dosya basariyla silindi", fileName);
        
       } else {
       
        
          
       }

       remove_newline_char(buffer_delete);
       // cevap gönderme başlangıç
      
      
      
       printf("buffer : %s  buffer boyu %d \n", buffer_delete, size_char(buffer_delete));
       //fwrite(buffer, 1, bufferSize, stdout);
      
       ssize_t bytes_written_delete = write(fd2, buffer_delete, size_char(buffer_delete));
       if (bytes_written_delete < 0)
       {
          perror("write");
          break;
       } // cevap gönderme bitiş
       close(fd2);
     
       fd2 = open(PIPE_NAME2, O_WRONLY);
          if (fd2 < 0) {
            perror("open");
            return 1;
          } 
       

     }
     printf("delete cikis yapti\n");
    
     pthread_mutex_unlock(&mutex);
  }
  close(fd_delete);

  // dosya oluşturma işlemini kilitleyin
    printf("delete file\n\n");
   
}

void *write_file(void *arg)
{
   printf("write file komutu dinlemeye başladi\n");
  while (1) {
    
    ssize_t bytes_read_write = read(fd_write, buffer_write, BUFFER_SIZE);
      pthread_mutex_lock(&mutex); // dosya yazma işlemini kilitle
       remove_newline_char(buffer);
       printf("buffer ilk hal %s",buffer);
    
    if (bytes_read_write < 0) {
      perror("read");
      break;
    }
    if (bytes_read_write == 0) {
      break;
    }
    char command[BUFFER_SIZE];
    //printf("buffer: %s",buffer);
   char *splitBuffer[200] = {NULL};
        
    splitP(buffer_write,splitBuffer," ");
    sscanf(buffer_write, "%s", command);
    //printf("buffer: %s %d",buffer,bytes_read);

    
    char *fileName="yeni.txt";
    char *fileNameTemp="yeni.txt";
     if(strcmp(command,"write")==0){
       
       int i=0;
  
       int satir=0;
       while(splitBuffer[i]!=NULL){
         // printf("create command %s \n",splitBuffer[i]);
        
          if(strstr(splitBuffer[i],".txt")!=NULL){
            fileName=splitBuffer[i];
            satir=i+1;
          }
          i++;
          
       }
       printf("filename %s /n",fileName);
       FILE *fptr;
       fptr=fopen(fileName,"a+");
      if(fptr == NULL)
      {
          printf("Error!");   
                       
      }else{
        for(int j=satir;splitBuffer[j] !=NULL ;j++){
          fputs(splitBuffer[j], fptr);
          fputs(" ", fptr);
        }
         fputs("\n", fptr);
      }
        fclose(fptr);

      
       char *temp;
      // printf("filename: %s\n", fileName);
      //  printf("filenameTemp: %s\n", fileNameTemp);
   

       remove_newline_char(buffer_write);
       // cevap gönderme başlangıç
      
      
      
       printf("buffer : %s  buffer boyu %d \n", buffer_write, size_char(buffer_write));
       //fwrite(buffer, 1, bufferSize, stdout);
      
       ssize_t bytes_written_write = write(fd2, buffer_write, size_char(buffer_write));
       if (bytes_written_write < 0)
       {
          perror("write");
          break;
       } // cevap gönderme bitiş
       close(fd2);
     
       fd2 = open(PIPE_NAME2, O_WRONLY);
          if (fd2 < 0) {
            perror("open");
            return 1;
          } 
       

     }
     printf("write cikis yapti\n");
    
     pthread_mutex_unlock(&mutex);// dosya yazma işlemini kilidini ac
  }
  strcpy(buffer_write,"");
  close(fd_delete);

  
    printf("write file methodu bitti\n\n");
   
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

 

  
    buffer = malloc(BUFFER_SIZE);
      buffer_read = malloc(BUFFER_SIZE);
        buffer_create = malloc(BUFFER_SIZE);
          buffer_write = malloc(BUFFER_SIZE);
            buffer_exit = malloc(BUFFER_SIZE);
              buffer_delete = malloc(BUFFER_SIZE);


    char * myfifo = PIPE_NAME;
    char * myfifo2 = PIPE_NAME2;
    char * myfifocreate =PIPE_NAME_CREATE;
     char * myfifodelete =PIPE_NAME_DELETE;
     char * myfifowrite =PIPE_NAME_WRITE;
     char * myfiforead =PIPE_NAME_READ;
     char * myfifoexit =PIPE_NAME_EXIT;
    mkfifo(myfifo, 0666);
    mkfifo(myfifo2, 0666);
    mkfifo(myfifocreate, 0666);
    mkfifo(myfifodelete, 0666);
    mkfifo(myfifowrite, 0666);
    mkfifo(myfiforead, 0666);
    mkfifo(myfifocreate, 0666);
  
  // Open the named pipe for reading
  fd = open(PIPE_NAME, O_RDONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }
    fd_create= open(PIPE_NAME_CREATE, O_RDONLY);
  if (fd_create < 0) {
    perror("open");
    return 1;
  }
    fd_delete= open(PIPE_NAME_DELETE, O_RDONLY);
  if (fd_delete < 0) {
    perror("open");
    return 1;
  }
    fd_write= open(PIPE_NAME_WRITE, O_RDONLY);
  if (fd_write < 0) {
    perror("open");
    return 1;
  }
    fd_read= open(PIPE_NAME_READ, O_RDONLY);
  if (fd_read < 0) {
    perror("open");
    return 1;
  }
    fd_exit= open(PIPE_NAME_EXIT, O_RDONLY);
  if (fd_exit < 0) {
    perror("open");
    return 1;
  }
  fd2 = open(PIPE_NAME2, O_WRONLY);
  if (fd2 < 0) {
    perror("open");
    return 1;
  }
  // Allocate a buffer for reading from the named pipe
 
  if (buffer == NULL) {
    perror("malloc");
    return 1;
  }
  if (buffer_create == NULL) {
    perror("malloc");
    return 1;
  }
  if (buffer_delete == NULL) {
    perror("malloc");
    return 1;
  }
  if (buffer_exit == NULL) {
    perror("malloc");
    return 1;
  }
  if (buffer_write == NULL) {
    perror("malloc");
    return 1;
  }
  if (buffer_read == NULL) {
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
  free(buffer_write);
  free(buffer_read);
  free(buffer_delete);
  free(buffer_exit);
  free(buffer_create);
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

