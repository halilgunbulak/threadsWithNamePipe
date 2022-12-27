#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_FILES 10
#define MAX_FILENAME_LENGTH 256
#define MAX_THREADS 10

// FileList adında bir yapı tanımlayalım
typedef struct {
  char filenames[MAX_FILES][MAX_FILENAME_LENGTH];
  pthread_mutex_t lock;
} FileList;

// FileList yapısının bir örneğini oluşturalım
FileList file_list;

// FileClient adında bir yapı tanımlayalım
typedef struct {
  int id;
  pthread_t thread;
} FileClient;

// FileClient yapısının bir dizisini oluşturalım
FileClient file_clients[MAX_THREADS];

// Bu fonksiyon named pipe ile iletişim kurulan file_client'ları dinler
void *listen_to_client(void *arg) {
  FileClient *client = (FileClient *)arg;
  int fd = open("file_manager_named_pipe", O_RDONLY);

  // "exit" komutu gelene kadar dinleme işlemini sürdür
  while (1) {
    char command[MAX_FILENAME_LENGTH];
    read(fd, command, MAX_FILENAME_LENGTH);

    if (strcmp(command, "exit") == 0) {
      break;
    } else if (strcmp(command, "create") == 0) {
      // Dosya ismini oku ve dosyayı oluştur
      char filename[MAX_FILENAME_LENGTH];
      read(fd, filename, MAX_FILENAME_LENGTH);

      pthread_mutex_lock(&file_list.lock);

      // FileList'te boş bir yer ara
      int i;
      for (i = 0; i < MAX_FILES; i++) {
        if (strlen(file_list.filenames[i]) == 0) {
          strcpy(file_list.filenames[i], filename);
          break;
        }
      }

      pthread_mutex_unlock(&file_list.lock);

      // Dosyayı oluştur
      int fd = open(filename, O_CREAT, S_IRUSR | S_IWUSR);
      close(fd);
       printf("create success");
      // İşlem durumunu file_client'a gönder
      write(fd, "create success", 15);
    } else if (strcmp(command, "delete") == 0) {
      // Dosya ismini oku ve dosyayı sil
      char filename[MAX_FILENAME_LENGTH];
      read(fd, filename, MAX_FILENAME_LENGTH);

      pthread_mutex_lock(&file_list.lock);

      // FileList'te dosya ismini arama
      int i;
      for (i = 0; i < MAX_FILES; i++) {
        if (strcmp(file_list.filenames[i], filename) == 0) {
          // Dosyayı sil
          remove(filename);

          // FileList'ten dosyayı sil
          strcpy(file_list.filenames[i], "");
          break;
        }
      }

      pthread_mutex_unlock(&file_list.lock);

      // İşlem durumunu file_client'a gönder
      printf("delete success");
      write(fd, "delete success", 15);
    } else if (strcmp(command, "read") == 0) {
      // Dosya ismini oku ve dosyayı oku
      char filename[MAX_FILENAME_LENGTH];
      read(fd, filename, MAX_FILENAME_LENGTH);

      pthread_mutex_lock(&file_list.lock);

      // FileList'te dosya ismini arama
      int i;
      for (i = 0; i < MAX_FILES; i++) {
        if (strcmp(file_list.filenames[i], filename) == 0) {
          // Dosyayı oku ve file_client'a gönder
          int fd = open(filename, O_RDONLY);
          char buffer[MAX_FILENAME_LENGTH];
          int n = read(fd, buffer, MAX_FILENAME_LENGTH);
          write(fd, buffer, n);
          close(fd);
          break;
        }
      }

      pthread_mutex_unlock(&file_list.lock);
    printf("read success");
      // İşlem durumunu file_client'a gönder
      write(fd, "read success", 13);
    } else if (strcmp(command, "write") == 0) {
      // Dosya ismini oku ve file_client'tan gelen veriyi dosyaya yaz
      char filename[MAX_FILENAME_LENGTH];
      read(fd, filename, MAX_FILENAME_LENGTH);

      pthread_mutex_lock(&file_list.lock);

      // FileList'te dosya ismini arama
      int i;
      for (i = 0; i < MAX_FILES; i++) {
        if (strcmp(file_list.filenames[i], filename) == 0) {
          // File_client'tan veriyi oku ve dosyaya yaz

            char buffer[MAX_FILENAME_LENGTH];
          int n = read(fd, buffer, MAX_FILENAME_LENGTH);
          int fd = open(filename, O_WRONLY);
          write(fd, buffer, n);
          close(fd);
          break;
        }
      }

      pthread_mutex_unlock(&file_list.lock);

      // İşlem durumunu file_client'a gönder
      write(fd, "write success", 14);
    }
  }

  // İletişimi kopar
  close(fd);
}

int main(int argc, char *argv[]) {
  // FileList'i başlat
  pthread_mutex_init(&file_list.lock, NULL);
  int i;
  for (i = 0; i < MAX_FILES; i++) {
    strcpy(file_list.filenames[i], "");
  }

  // named pipe'ı oluştur
  mkfifo("file_manager_named_pipe", S_IRUSR | S_IWUSR);

  // file_client'ları dinlemeye başla
  for (i = 0; i < MAX_THREADS; i++) {
    file_clients[i].id = i;
    pthread_create(&file_clients[i].thread, NULL, listen_to_client, &file_clients[i]);
  }

  // file_client thread'lerini bekle
  for (i = 0; i < MAX_THREADS; i++) {
    pthread_join(file_clients[i].thread, NULL);
  }

  return 0;
}