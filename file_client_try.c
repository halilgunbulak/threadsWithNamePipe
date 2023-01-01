#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define PIPE_NAME "my_named_pipe"
#define PIPE_NAME2 "my_named_pipe2"
#define PIPE_NAME_CREATE "my_named_create"
#define PIPE_NAME_DELETE "my_named_delete"
#define PIPE_NAME_EXIT "my_named_exit"
#define PIPE_NAME_WRITE "my_named_write"
#define BUFFER_SIZE 1024
void remove_newline_char(char *str);
int main(int argc, char** argv) {
  // Open the named pipe for writing
  int fd = open(PIPE_NAME, O_WRONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }
    int fd_create = open(PIPE_NAME_CREATE, O_WRONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }
    int fd_delete = open(PIPE_NAME_DELETE, O_WRONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }
    int fd_write = open(PIPE_NAME_WRITE, O_WRONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }
    int fd_exit= open(PIPE_NAME_EXIT, O_WRONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }
 
  // Open the named pipe for reading
  int fd2 = open(PIPE_NAME2, O_RDONLY);
  if (fd2 < 0) {
    perror("open pipe2");
    return 1;
  }
  // Allocate a buffer for reading from stdin
  char* buffer = malloc(BUFFER_SIZE);
  if (buffer == NULL) {
    perror("malloc");
    return 1;
  }
    char command[BUFFER_SIZE];
    
  // Read data from stdin and write it to the named pipe
  while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
     printf("buffer1: %s \n",buffer);
    sscanf(buffer, "%s", command);
    remove_newline_char(command);
      printf("buffer2: %s \n",buffer);
    if(strcmp(command,"create")==0){
            printf("create içine girdi");
            ssize_t bytes_written = write(fd_create, buffer, strlen(buffer));
            sscanf(buffer, "%s", command);
        

    }else if(strcmp(command,"delete")==0){
         ssize_t bytes_written = write(fd_delete, buffer, strlen(buffer));
            sscanf(buffer, "%s", command);
            if (bytes_written < 0 || strcmp(command,"exit")==0) {
              perror("write");
              break;
            }
    }else if ( strcmp(command,"exit")==0) {
              perror("exit");
              break;
            }else{
       printf("Yanlıs komut girdiniz tekrar giriniz.");
    }
   
    /*ssize_t bytes_read = read(fd2, buffer, BUFFER_SIZE);
    remove_newline_char(buffer);
    printf("file : %s oluşturuldu \n",buffer);
    if (bytes_read < 0) {
      perror("read");
      break;
    }
    if (bytes_read == 0) {
      break;
    }
    char command[BUFFER_SIZE];
     sscanf(buffer, "%s", command);
    if(strcmp(command,"exit")==0){
        printf("Proggress exit : worked exit command");
        break;
    }*/
  }

  // Clean up resources
  free(buffer);
  close(fd);
  return 0;
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