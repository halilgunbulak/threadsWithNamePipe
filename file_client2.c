#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define PIPE_NAME "file_manager_named_pipe"
#define BUFFER_SIZE 1024

int main(int argc, char** argv) {
  // Open the named pipe for writing
  int fd = open(PIPE_NAME, O_WRONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }

  // Allocate a buffer for reading from stdin
  char* buffer = malloc(BUFFER_SIZE);
  if (buffer == NULL) {
    perror("malloc");
    return 1;
  }

  // Read commands from stdin and send them to the file manager
  while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
    // Write the command to the named pipe
    ssize_t bytes_written = write(fd, buffer, strlen(buffer));
    if (bytes_written < 0) {
      perror("write");
      break;
    }

    // Read the response from the file manager
    ssize_t bytes_read = read(fd, buffer, BUFFER_SIZE);
    if (bytes_read < 0) {
      perror("read");
      break;
    }

    // Print the response
    fputs(buffer, stdout);
  }

  // Clean up resources
  free(buffer);
  close(fd);
  return 0;
}