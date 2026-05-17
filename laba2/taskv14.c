#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct map {
    unsigned long start;
    unsigned long end;
    char perm[5];
    unsigned long off;
    char dev[10];
    unsigned long inode;
    char path[256];
};

int parse(char *s, struct map *m) {
    int n = sscanf(s, "%lx-%lx %4s %lx %s %lu %255[^\n]",
                   &m->start, &m->end, m->perm, &m->off, m->dev, &m->inode, m->path);
    if (n < 6) {
        m->path[0] = 0;
        n = sscanf(s, "%lx-%lx %4s %lx %s %lu",
                   &m->start, &m->end, m->perm, &m->off, m->dev, &m->inode);
        if (n < 6) return 0;
    }
    return 1;
}

int main() {
    FILE *f = fopen("/proc/self/maps", "r");
    char buf[512];
    struct map m;
    int cnt = 0;
    unsigned long total = 0;

    printf("Address              Perms  Offset   Dev      Inode   Path\n");
    while (fgets(buf, sizeof(buf), f)) {
        if (parse(buf, &m)) {
            printf("%016lx-%016lx %s %08lx %s %lu %s\n",
                   m.start, m.end, m.perm, m.off, m.dev, m.inode, m.path);
            cnt++;
            total += m.end - m.start;
        }
    }

    printf("\nSegments: %d\n", cnt);
    printf("Total memory: %lu bytes (%.2f MB)\n", total, total / (1024.0 * 1024.0));

    fclose(f);
    return 0;
}
