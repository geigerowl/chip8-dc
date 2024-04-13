#ifndef FILE_UTILS_H_INCLUDED
#define FILE_UTILS_H_INCLUDED

#include <kos.h>

#define LIST_MAX 10 //maximum files to display at once
#define FILE_MAX 20 //maximum file name for display

#define PAGE_INITIAL -1
#define PAGE_PREV 0
#define PAGE_NEXT 1


/* Type to hold a list of files in a folder and associated variables

- int index          - the offset from first file in directory
- int list_count     - total files present in the file list
- int total_items    - total files present in the scanned directory
- char file_list[][] - array containing the truncated file name strings
*/
typedef struct FileList{
    int index;
    int list_count;
    int total_items;
    char file_list[LIST_MAX][FILE_MAX+1];
} FileList;


/* Get the count of files in a directory

Inputs:
- char* file_path   - path to folder

Outputs
- int file_count    - count of files
*/
int GetFileCount(char* file_path);


/* Populate a FileList with the truncated file names in a folder

Inputs:
- FileList* l       - pointer to initialized FileList
- char* file_path   - path to folder on cd
- int offset        - offset from beginning of directory
- int file_count    - count of files past the directory offset to list

Outputs:
- None
*/
void CreateFileList(FileList* l, char* file_path, int offset, int file_count);


/* Load the FileList with the requested previous/next page if possible.

Inputs:
- FileList* l       - pointer to populated FileList
- int page          - Requested page, allowed values are PAGE_INITIAL, PAGE_PREV or PAGE_NEXT

Outputs
- int file_count    - count of files in the page
*/
int GetPage(FileList* l, char* file_path, int page);


/* Grab the offset of a selected file in a FileList

Inputs:
- FileList* l       - pointer to populated FileList
- int selection     - selected file in the list

Outputs:
- int offset        - offset of file in directory
*/
int GetFileIndex(FileList* l, int selection);

/* Load the file at offset from the start of file_path to the buffer at output

Inputs:
- char* file_path   - path to folder on cd
- int offset        - offset from beginning of directory
- void* output      - location to load created file
- int max_size      - maximum allowed file size in bytes

Outputs:
- int status        - 0 if load successful, -1 if file size too large
*/
int LoadFile(char* file_path, int offset, void* output, int max_size);
#endif // FILE_UTILS_H_INCLUDED
