#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX 1000

char *ring[MAX];
int need = 10;
int first = 0;
int have = 0;

void add(char *s, int len) {
    int pos = (first + have) % need;
    if (have == need) {
        free(ring[pos]);
        first = (first + 1) % need;
    } else {
        have++;
    }
    ring[pos] = malloc(len + 1);
    memcpy(ring[pos], s, len);
    ring[pos][len] = '\0';
}

void print(int rev) {
    int i;
    for (i = 0; i < have; i++) {
        int idx;
        if (rev)
            idx = (first + have - 1 - i) % need;
        else
            idx = (first + i) % need;
        printf("%s\n", ring[idx]);
    }
}

void read_fgets(FILE *f) {
    char buf[4096];
    while (fgets(buf, sizeof(buf), f)) {
        int len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n')
            len--;
        add(buf, len);
    }
}

void read_mmap(char *name) {
    int fd = open(name, O_RDONLY);
    struct stat st;
    fstat(fd, &st);
    char *data = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    char *p = data + st.st_size - 1;
    char *end = p;
    char *lines[MAX];
    int lens[MAX];
    int found = 0;

    while (p > data && *p == '\n') {
        end = p - 1;
        p--;
    }

    while (p >= data && found < need) {
        if (p == data || *(p-1) == '\n') {
            lines[found] = p;
            lens[found] = end - p + 1;
            found++;
            end = p - 1;
        }
        p--;
    }

    int i;
    for (i = found - 1; i >= 0; i--)
        add(lines[i], lens[i]);

    munmap(data, st.st_size);
    close(fd);
}

int main(int argc, char **argv) {
    int rev = 0;
    int force_mmap = 0;
    char *fname = NULL;
    int i;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            need = atoi(argv[++i]);
            if (need > MAX) need = MAX;
        } else if (strcmp(argv[i], "-r") == 0) {
            rev = 1;
        } else if (strcmp(argv[i], "-m") == 0) {
            force_mmap = 1;
        } else {
            fname = argv[i];
        }
    }

    if (fname) {
        struct stat st;
        stat(fname, &st);
        if (force_mmap || st.st_size > 1024 * 1024)
            read_mmap(fname);
        else {
            FILE *f = fopen(fname, "r");
            read_fgets(f);
            fclose(f);
        }
    } else {
        read_fgets(stdin);
    }

    print(rev);

    for (i = 0; i < need; i++)
        free(ring[i]);

    return 0;
}
