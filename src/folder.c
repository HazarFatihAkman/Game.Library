#include "../include/folder.h"
#include "../include/utils.h"

#include <fileapi.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <shlwapi.h>

#define MAX_PATH_1024 1024

static char *strsub(char *value) {
  int delimiter_index;
  int actual_size = 0;

  for (int value_size = strlen(value); value_size >= 0; value_size--) {
    if (value[value_size] == '.') {
      delimiter_index = value_size - 1;
      break;
    }
  }

  actual_size -= (actual_size - delimiter_index);
  ++actual_size;

  char *res = (char*)malloc(actual_size);
  if (res == NULL) {
    perror("Memory Allocation For res!\n");
    return NULL;
  }

  strncpy(res, value, actual_size);
  res[actual_size] = '\0';
  return res;
}

static list_item_t *create_new_item(char *url, char *name, int index) {
  file_t *temp_value = (file_t*) malloc(sizeof(file_t));
  if (temp_value == NULL) {
    perror("Memory Allocation For temp_value!\n");
    return NULL;
  }

  temp_value->url = (char*) malloc(strlen(url) + 1);
  if (temp_value->url == NULL) {
    perror("Memory Allocation For temp_value->url!\n");
    return NULL;
  }

  strcpy(temp_value->url, url);
  temp_value->url[strlen(url)] = '\0';

  char *ui_name = strsub(name);
  temp_value->name = (char*) malloc(strlen(ui_name) + 1);

  strcpy(temp_value->name, ui_name);
  temp_value->name[strlen(name)] = '\0';

  temp_value->src = (char*) malloc(strlen(ui_name) + sizeof(DEFAULT_COVER_FORMAT) + 1);
  if (temp_value->src == NULL) {
    perror("Memory Allocation For temp_value->src!\n");
    return NULL;
  }

  snprintf(temp_value->src, strlen(ui_name) + sizeof(DEFAULT_COVER_FORMAT), DEFAULT_COVER_FORMAT, ui_name);
  temp_value->src[strlen(temp_value->src)] = '\0';

  list_item_t *item = (list_item_t*) malloc(sizeof(list_item_t));
  if (item == NULL) {
    perror("Memory Allocation For item!\n");
    return NULL;
  }

  item->value = temp_value;
  item->index = index;

  return item;
}

list_t *load_folder() {
  list_t *folder = (list_t*)malloc(sizeof(list_t));
  if (folder == NULL) {
    perror("Memory Allocation For folder!\n");
    return NULL;
  }

  folder->size = 0;
  folder->capacity = DEFAULT_CAPACITY;

  WIN32_FIND_DATA fdFile;
  HANDLE hFind = NULL;
  int index = 0;

  hFind = FindFirstFile(DEFAULT_FOLDER, &fdFile);
  if (hFind == INVALID_HANDLE_VALUE) {
    return NULL;
  }

  do {
    if (strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0) {
      char full_path[MAX_PATH_1024];
      char absolute_path[MAX_PATH_1024];
      char url_buffer[MAX_PATH_1024] = { 0 };
      char *ext = PathFindExtension(fdFile.cFileName);

      snprintf(full_path, MAX_PATH_1024, "%s\\%s", DEFAULT_FOLDER_FORMAT_VERSION, fdFile.cFileName);

      if (_stricmp(ext, ".url") == 0) {
        GetFullPathName(full_path, MAX_PATH_1024, absolute_path, NULL);

        GetPrivateProfileString("InternetShortcut", "URL", "", url_buffer, MAX_PATH_1024, absolute_path); 
      }
      else {
        strncpy(url_buffer, fdFile.cFileName, MAX_PATH_1024);
      }

      list_item_t *temp_item = create_new_item(url_buffer, fdFile.cFileName, index);
      if (push_to(folder, temp_item) == true) {
        ++index;
        printf("temp_item pushed\n");
      }
      else {
        perror("temp_item cannot pushed!\n");
      }
    }
  } while (FindNextFile(hFind, &fdFile));

  folder->size = index;
  FindClose(hFind);
  return folder;
}

