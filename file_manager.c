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
#define FILE_BUFFER_SIZE 10
#define BUFFER_SIZE 1024
//char array size return;
int size_char(char *array);
void remove_newline_char(char *str);
// parse string to words by using delimiter
void splitP(char *str, char **parsed, char *delim);
//char elemanı boş mu değil mi buna göre return vermekte boş ise 1 dolu ise 0 dönmektedir.
int charIsEmpty(char *str);
//eger char degeri içeriyorsa 1 dönüyor.

char fileBuffer[FILE_BUFFER_SIZE][100]; // 10 adet file ismini tutacak olan array

pthread_mutex_t mutex;
int fd;
int fd2;
int fd_create;
int fd_delete;
int fd_write;
int fd_read;
int fd_exit;
 char* buffer ;
 char* response[400];
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
   
    char *fileName="yeni.txt";
     if(strcmp(command,"create")==0){
      
       
       int i=0;
       while(splitBuffer[i]!=NULL){
        
         // printf("create command %s \n",splitBuffer[i]);
        
          if(strstr(splitBuffer[i],".txt")!=NULL){ //eger iceriyorsa dosya adi yap
            fileName=splitBuffer[i];
          }
          i++;
          
       }

       printf("filename: %s\n", fileName);
       int result=fileControl(command,fileName);
       FILE *fptr;
       fptr = fopen(fileName,"ab+");
        if(fptr == NULL)
      {
          printf("Error!");   
                       
      }else{
        fputs("s", fptr);
      }
       
       fclose(fptr);
   
       
      
       
       // cevap gönderme başlangıç
    
       strcpy(response,"file basariyla olustu");
       int bufferSize = size_char(response);
      
      
       ssize_t bytes_written_create = write(fd2, response, bufferSize);
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
       strcpy(response,"                                                                           ");
    printf("create methodu tekrar dinlemeye başladi\n");
     pthread_mutex_unlock(&mutex);
  }
  close(fd_create);
 
  // dosya oluşturma işlemini kilitleyin
    printf("create file methodu bitti\n");
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
           
            fileName=splitBuffer[i];
            fileNameTemp=splitBuffer[i];
          }
           i++;
       }
       int result=fileControl(command,fileName);
       char *temp;
      // printf("filename: %s\n", fileName);
      //  printf("filenameTemp: %s\n", fileNameTemp);
       if (remove(fileName) ==0) {
        
          strcpy(response,"dosya bulundu ve silindi");
         
          
         //sprintf(buffer, "%s adli dosya basariyla silindi", fileName);
        
       } else {
       
         strcpy(response,"dosya bulunamadi");
          
       }

       remove_newline_char(buffer_delete);
     
       ssize_t bytes_written_delete = write(fd2, response, size_char(response));
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
          strcpy(response,"                                                                           ");
       

     }
  
      strcpy(buffer_delete,"");
     pthread_mutex_unlock(&mutex);
  }
  close(fd_delete);

  // dosya oluşturma işlemini kilitleyin
    printf("delete file\n\n");
    printf("delete methodu tekrar dinlemeye başladi\n");
}

void *write_file(void *arg)
{
   printf("write file komutu dinlemeye başladi\n");
  while (1) {
    
    ssize_t bytes_read_write = read(fd_write, buffer_write, BUFFER_SIZE);
      pthread_mutex_lock(&mutex); // dosya yazma işlemini kilitle
       remove_newline_char(buffer);
     
    
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
       int result=fileControl(command,fileName);
       printf("filename %s /n",fileName);
       FILE *fptr;
       fptr=fopen(fileName,"ab+");
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

   
       strcpy(buffer_write,"data basariyla yazildi");
       remove_newline_char(buffer_write);
       // cevap gönderme başlangıç

      
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
     strcpy(buffer_write,"                                                                           ");
      printf("write methodu tekrar dinlemeye başladi\n");
     pthread_mutex_unlock(&mutex);// dosya yazma işlemini kilidini ac
  }
  
  

  close(fd_write);
    printf("write file methodu bitti\n\n");
   
}

void *exit_program(void *arg)
{
   printf("exit komutuna girdi\n\n");
    while (1) {
    
    ssize_t bytes_read_read = read(fd_exit, buffer_exit, BUFFER_SIZE);
      pthread_mutex_lock(&mutex); // dosya yazma işlemini kilitle
       remove_newline_char(buffer_exit);
      
    
    if (bytes_read_read < 0) {
      perror("read");
      break;
    }
    if (bytes_read_read == 0) {
      break;
    }
    char command[BUFFER_SIZE];
    //printf("buffer: %s",buffer);
   char *splitBuffer[200] = {NULL};
        
    splitP(buffer_exit,splitBuffer," ");
    sscanf(buffer_exit, "%s", command);
    //printf("buffer: %s %d",buffer,bytes_read);

     
   
     if(strcmp(command,"exit")==0){
    
     
       printf("exit komutu alindi cikis yapti\n");
        exit(0);
     }
     
     
      strcpy(buffer_exit,"");
     
     pthread_mutex_unlock(&mutex); // çıkış işlemini kilidi açın
  }
    pthread_mutex_destroy(&mutex);
}

void *read_file(void *arg)
{


    printf("read file komutu dinlemeye başladi\n");
  while (1) {
    
    ssize_t bytes_read_read = read(fd_read, buffer_read, BUFFER_SIZE);
      pthread_mutex_lock(&mutex); // dosya yazma işlemini kilitle
       remove_newline_char(buffer_read);
 
    
    if (bytes_read_read < 0) {
      perror("read");
      break;
    }
    if (bytes_read_read == 0) {
      break;
    }
    char command[BUFFER_SIZE];
    //printf("buffer: %s",buffer);
   char *splitBuffer[200] = {NULL};
        
    splitP(buffer_read,splitBuffer," ");
    sscanf(buffer_read, "%s", command);
    //printf("buffer: %s %d",buffer,bytes_read);

     
    char *fileName="";
    char *fileNameTemp="yeni.txt";
     if(strcmp(command,"read")==0){
       
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
          int result=fileControl(command,fileName);
 

              
                      
                        FILE *fp;
                        int c;
                        char *temp[400];
                        if(strcmp(fileName,"")!=0){
                        fp = open(fileName,O_RDONLY);
                        read(fp, temp ,80);
                        close(fp);
                        }else{
                          strcpy(temp,"boyle bir dosya bulunamadi");
                        }
                        strcpy(buffer_read,"");
                      

                    
                      
                      strcpy(buffer_read,temp);
                      //remove_newline_char(buffer_read);
                      // cevap gönderme başlangıç
                      
                      
                      
                      //printf("buffer : %s  buffer boyu %d \n", buffer_read, size_char(buffer_read));
                      //fwrite(buffer, 1, bufferSize, stdout);
                      
                      ssize_t bytes_written_read = write(fd2, buffer_read, size_char(buffer_read));
                      if (bytes_written_read < 0)
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
     
      printf("read methodu tekrar dinlemeye başladi\n");
      strcpy(buffer_read,"       ");
     
     pthread_mutex_unlock(&mutex);// dosya yazma işlemini kilidini ac
  }

  close(fd_read);

  
    printf("read file methodu bitti\n\n");

  // çıkış işlemini kilidi açın
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
    mkfifo(myfifoexit, 0666);

   
  
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
    pthread_t thread1, thread2, thread3, thread4,thread5;
    pthread_create(&thread1, NULL, create_file, NULL);
    pthread_create(&thread2, NULL, delete_file, NULL);
    pthread_create(&thread3, NULL, write_file, NULL);
     pthread_create(&thread4, NULL, read_file, NULL);
    pthread_create(&thread5, NULL, exit_program, NULL);

    // thread'lerin bitmesini bekleyin
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
     pthread_join(thread5, NULL);

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

int fileControl(char* command, char* fileName)
{
    int i;
    if (strcmp(command, "create") == 0) {
        // dosya oluşturma komutu geldi
        // dosya ismi daha önce oluşturulmuş mu kontrol edin
        int result = 0;
        for (i = 0; i < FILE_BUFFER_SIZE; i++) {
            if (strcmp(fileBuffer[i], fileName) == 0) {
                result = 1;
            }
        }
        if (result) {
            // dosya ismi daha önce oluşturulmuş
            return result;
        }
        else {
            // dosya ismi daha önce oluşturulmamış, oluşturun
            for (i = 0; i < FILE_BUFFER_SIZE; i++) {
                if (fileBuffer[i][0] == '\0') {
                    strcpy(fileBuffer[i], fileName);
                    break;
                }
            }
            return result;
        }
    } else if (strcmp(command, "delete") == 0) {
      int result=0;
        // dosya silme komutu geldi
        // dosya ismini array'den silin
        for (i = 0; i < FILE_BUFFER_SIZE; i++) {
            if (strcmp(fileBuffer[i], fileName) == 0) {
                fileBuffer[i][0] = '\0';
                result =1;
                break;
            }
        }
        return result;
    }
    else if (strcmp(command, "write") == 0) {
        // dosya yazma komutu geldi
        // dosya ismi daha önce oluşturulmuş mu kontrol edin
        int result = 0;
        for (i = 0; i < FILE_BUFFER_SIZE; i++) {
            if (strcmp(fileBuffer[i], fileName) == 0) {
                result = 1;
            }
        }
        return result;
    }
    else if (strcmp(command, "read") == 0) {
        // dosya yazma komutu geldi
        // dosya ismi daha önce oluşturulmuş mu kontrol edin
        int result = 0;
        remove_newline_char(fileName);
        for (i = 0; i < FILE_BUFFER_SIZE; i++) {
        
         
            if (strcmp(fileBuffer[i], fileName) == 0) {
              
                result = 1;
                
            }
        }
        
       return result;
        
    }else{ 
      return 0;
    }
}