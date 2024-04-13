#include <file_utils.h>



//Get the count of files in a directory
int GetFileCount(char* file_path) {
    int file_count = 0;
    file_t d;
    dirent_t *de;

    //open directory
    d = fs_open(file_path,O_RDONLY|O_DIR);

    if(d == -1) {
        printf("Can't open directory!\n");
        return file_count;
    }

    while((de = fs_readdir(d))) {
        file_count++;
    }

    fs_close(d);
    return file_count;
}

//
void CreateFileList(FileList* l, char* file_path, int offset, int file_count) {
    dirent_t *de;
    file_t d;

    if(file_count > LIST_MAX || file_count < 0) {
        printf("Invalid file_count %i!\n", file_count);
        file_count = LIST_MAX;
    }

    //open directory
    d = fs_open(file_path,O_RDONLY|O_DIR);
    for(int i = 0; i < offset; i++) { //approximation for non-implemented seek
        fs_readdir(d);
    }

    for(int i = 0; i < file_count; i++) {
        de = fs_readdir(d);
        strncpy(l->file_list[i], de->name, FILE_MAX);
        //printf("Copied %s \n", l->file_list[i]);
    }
    fs_close(d);
    l->index = offset;
    l->list_count = file_count;
}

int GetPage(FileList* l, char* file_path, int page) {
    int current_index = l->index;
    int new_index = 0;
    int file_count = l->total_items;
    int file_difference;

    switch (page) {
        case PAGE_INITIAL: //first scan, need to get total file count
            l->total_items = GetFileCount(file_path);
            break;
        case PAGE_PREV:
            new_index = current_index - LIST_MAX;
            break;
        case PAGE_NEXT:
            new_index = current_index + LIST_MAX;
            break;
        default:
            printf("Invalid page requested!");
            break;
    }

    if ((new_index >= 0) && (new_index <= l->total_items-1)) { //checking for the index to be in bounds
        file_difference = l->total_items-new_index;
        file_count = (file_difference > LIST_MAX) ? LIST_MAX : file_difference;
        CreateFileList(l, file_path, new_index, file_count);
    }

    return file_count;
}

int GetFileIndex(FileList* l, int selection) {
    return l->index + selection;
}

int LoadFile(char* file_path, int offset, void* output, int max_size) {
    dirent_t *de;
    file_t d;
    file_t rom_file;
    int status = 0;
    char file_location[NAME_MAX+10];

    d = fs_open(file_path,O_RDONLY|O_DIR);

    for(int i = 0; i < offset; i++) { //approximation for non-implemented seek
        fs_readdir(d);
    }

    de = fs_readdir(d);

    if (de->size > max_size) {
        printf("File size %i bytes is too large!", de->size);
        status = -1;
    }
    else {
        sprintf(file_location, "%s%s", file_path, de->name);
        rom_file = fs_open(file_location,O_RDONLY);
        fs_read(rom_file, output, de->size);

        fs_close(rom_file);
    }

    fs_close(d);


    return status;
}
