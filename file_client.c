#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define PIPE_NAME "my_named_pipe"
#define PIPE_NAME2 "my_named_pipe2"
#define PIPE_NAME_CREATE "my_named_create"
#define PIPE_NAME_DELETE "my_named_delete"
#define PIPE_NAME_WRITE "my_named_write"
#define PIPE_NAME_READ "my_named_read"
#define PIPE_NAME_EXIT "my_named_exit"
#define BUFFER_SIZE 1024
void remove_newline_char(char *str);
int commandToInt(char* eleman);
int main(int argc, char** argv) {
  // Open the named pipe for writing
  int fd = open(PIPE_NAME, O_WRONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }
    int fd_create= open(PIPE_NAME_CREATE, O_WRONLY);
  if (fd_create < 0) {
    perror("open");
    return 1;
  }
    int fd_delete = open(PIPE_NAME_DELETE, O_WRONLY);
  if (fd_delete < 0) {
    perror("open");
    return 1;
  }
      int fd_write = open(PIPE_NAME_WRITE, O_WRONLY);
  if (fd_write < 0) {
    perror("open");
    return 1;
  }
      int fd_read = open(PIPE_NAME_READ, O_WRONLY);
  if (fd_read < 0) {
    perror("open");
    return 1;
  }
      int fd_exit = open(PIPE_NAME_EXIT, O_WRONLY);
  if (fd_exit < 0) {
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
   char* response = malloc(BUFFER_SIZE);
  if (buffer == NULL) {
    perror("malloc");
    return 1;
  }
    char command[BUFFER_SIZE];
    
  // Read data from stdin and write it to the named pipe
  while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
    
    
   
    sscanf(buffer, "%s", command);
    int commandId=commandToInt(command);
      printf("Command: %s %d \n",command,commandId);
    if(commandId==1){
              ssize_t bytes_written = write(fd_create, buffer, strlen(buffer));
             
              if (bytes_written < 0 || strcmp(command,"exit")==0) {
                perror("write");
                break;
              }
              strcpy(buffer,"                                          ");

              ssize_t bytes_read = read(fd2, response, BUFFER_SIZE); 
              remove_newline_char(response);
              printf("buffer: %s \n",response);
              if (bytes_read < 0) {
                perror("read");
                break;
              }
              if (bytes_read == 0) {
                break;
              }
              strcpy(response,"");
    }else if(commandId==2){
        ssize_t bytes_written = write(fd_delete, buffer, strlen(buffer));
         
              if (bytes_written < 0 || strcmp(command,"exit")==0) {
                perror("write");
                break;
              }
              strcpy(buffer,"                                          ");
              ssize_t bytes_read = read(fd2, buffer, BUFFER_SIZE); 
              remove_newline_char(buffer);
              printf("buffer: %s \n",buffer);
              if (bytes_read < 0) {
                perror("read");
                break;
              }
             
             
    }else if(commandId==3){
        ssize_t bytes_written = write(fd_write,buffer, strlen(buffer));
         
              if (bytes_written < 0 || strcmp(command,"exit")==0) {
                perror("write");
                break;
              }
             strcpy(buffer,"                                          ");
              ssize_t bytes_read = read(fd2, buffer, BUFFER_SIZE); 
              remove_newline_char(buffer);
              printf("buffer: yazildi %s \n",buffer);
              if (bytes_read < 0) {
                perror("read");
                break;
              }
             
    }else if(commandId==4){
        ssize_t bytes_written = write(fd_read,buffer, strlen(buffer));
         
             
              if (bytes_written < 0 || strcmp(command,"exit")==0) {
                perror("write");
                break;
              }
             strcpy(buffer,"                                          ");
              ssize_t bytes_read = read(fd2, buffer, BUFFER_SIZE); 
              //remove_newline_char(buffer);
              printf("read okundu :  %s \n",buffer);
              if (bytes_read < 0) {
                perror("read");
                break;
              }

    }else if(commandId==5){
        ssize_t bytes_written_exit = write(fd_exit,buffer, strlen(buffer));
              if (bytes_written_exit < 0) {
                perror("write");
                break;
              }   
              
                  printf("Proggress exit : worked exit command\n");
                  exit(0);
                  break;
              
    }else if(commandId==0){
        printf("Yanliş komut girdiniz lütfen yeniden deneyin\n");
        
    }
    strcpy(buffer,"");
  
    
  }

  // Clean up resources
  free(buffer);
  close(fd);
   close(fd_create);
    close(fd_delete);
     close(fd_read);
      close(fd_write);
       close(fd_exit);
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

int commandToInt(char* eleman){
   if(strcmp(eleman,"create")==0){
    return 1;
   }else if(strcmp(eleman,"delete")==0){
    return 2;
   }else if(strcmp(eleman,"write")==0){
    return 3;
   }else if(strcmp(eleman,"read")==0){
    return 4;
   }else if(strcmp(eleman,"exit")==0){
    return 5;
   }else return 0;
}

