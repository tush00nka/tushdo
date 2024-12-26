#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct task {
    char status;
    char message[256];
};

typedef struct {
    struct task *array;
    size_t used;
    size_t size;
} Vector;

void init_vector(Vector *v, size_t initial_size) {
    v->array = malloc(initial_size * sizeof(struct task));
    v->used = 0;
    v->size = initial_size;
}

void insert_vecror(Vector *v, struct task new_task) {
    if (v->used == v->size) {
        v->size *= 2;
        v->array = realloc(v->array, v->size * sizeof(struct task));
    }
    v->array[v->used++] = new_task;
}

void remove_vector(Vector *v, int index) {
    struct task *temp = v->array;

    for (int i = index; i < v->used; i++) {
        v->array[i] = v->array[i+1];
    }

    v->used-=1;
}

void clear_vector(Vector *v) {
    free(v->array);
    v->array = NULL;
    v->used = v->size = 0;
}

void print_vector(Vector *v) {
    if (v->used <= 0) {
        printf("---Empty List---\n");
    }

    for (int i = 0; i < v->used; ++i) {
        printf("%d. [%c] %s\n", i+1, v->array[i].status, v->array[i].message);
    }
}

void read_from_file(Vector *v);
void save_to_file(Vector *v);

int main(int argc, char *argv[]) {    
    Vector task_pool;
    init_vector(&task_pool, 5);

    read_from_file(&task_pool);

    if (argc <= 1) {
        print_vector(&task_pool);
    }

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-a") == 0) {
            struct task new_task = {'-'};
            for (int j = 0; j < sizeof(argv[i+1]) + 1; ++j) {  // strangely i needed to add this +1 to the range,        
                new_task.message[j] = argv[i+1][j];            // so it would get the last char 
            }
            insert_vecror(&task_pool, new_task);
        }
        else if (strcmp(argv[i], "-p") == 0) {
            print_vector(&task_pool);
        }
        else if (strcmp(argv[i], "-c") == 0) {
            int index = atoi(argv[i+1]) - 1;
            task_pool.array[index].status='+';
        }
        else if (strcmp(argv[i], "-u") == 0) {
            int index = atoi(argv[i+1]) - 1;
            task_pool.array[index].status='-';
        }
        else if (strcmp(argv[i], "-r") == 0) {
            int index = atoi(argv[i+1]) - 1;
            if (index < 0 || index >= task_pool.used) {
                printf("There's no such task under that index!");
                return EXIT_FAILURE;
            }
            remove_vector(&task_pool, index);
        }
    }

    save_to_file(&task_pool);

    clear_vector(&task_pool);
    return EXIT_SUCCESS;
}

void read_from_file(Vector *v) {
    char* path;
    char* filename = "/.tushdo";

    path = malloc(strlen(getenv("HOME")) + strlen(filename) + 1);
    strcpy(path, getenv("HOME"));
    strcat(path, filename);

    FILE* file_ptr = fopen(path, "r");
    
    if (file_ptr != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), file_ptr)) {
            struct task read_task = {line[0]};
            for (int i = 0; i < sizeof(line); ++i) {
                if (line[i+2] != '\n') {
                    read_task.message[i] = line[i+2];
                }
            }

            insert_vecror(v, read_task);
        }
        fclose(file_ptr);
    }
}

void save_to_file(Vector *v) {
    char* path;
    char* filename = "/.tushdo";

    path = malloc(strlen(getenv("HOME")) + strlen(filename) + 1);
    strcpy(path, getenv("HOME"));
    strcat(path, filename);

    FILE* file_ptr = fopen(path, "w");

    for (int i = 0; i < v->used; ++i) {
        fprintf(file_ptr, "%c %s\n", v->array[i].status, v->array[i].message);
    }

    fclose(file_ptr);
}