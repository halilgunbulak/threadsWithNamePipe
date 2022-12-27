#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define PIPE_NAME "my_named_pipe"
#define PIPE_NAME2 "my_named_pipe2"
#define BUFFER_SIZE 1024

int main(int argc, char** argv) {
    char * myfifo = PIPE_NAME;
    char * myfifo2 = PIPE_NAME2;
    mkfifo(myfifo, 0666);
    mkfifo(myfifo2, 0666);
  // Open the named pipe for reading
  int fd = open(PIPE_NAME, O_RDONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }
int fd2 = open(PIPE_NAME2, O_WRONLY);
  if (fd2 < 0) {
    perror("open");
    return 1;
  }
  // Allocate a buffer for reading from the named pipe
  char* buffer = malloc(BUFFER_SIZE);
  if (buffer == NULL) {
    perror("malloc");
    return 1;
  }

  // Read from the named pipe and print the data to the console
  while (1) {
    ssize_t bytes_read = read(fd, buffer, BUFFER_SIZE);
    if (bytes_read < 0) {
      perror("read");
      break;
    }
    if (bytes_read == 0) {
      break;
    }
    char command[BUFFER_SIZE];
    sscanf(buffer, "%s", command);
    //printf("buffer: %s %d",buffer,bytes_read);
    if(strcmp(command,"exit")==0){
        printf("Proggress exit : worked exit command");
        break;
    }
   
    
    fwrite(buffer, 1, bytes_read, stdout);
    ssize_t bytes_written = write(fd2, buffer, strlen(buffer));
    if (bytes_written < 0) {
      perror("write");
      break;
    }
  }

  // Clean up resources
  free(buffer);
  close(fd);
  return 0;
}