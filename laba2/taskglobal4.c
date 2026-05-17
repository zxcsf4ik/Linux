#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void bar_is_now_closed(void) {
    int localvar = 5;
    printf("bar_is_now_closed  &localvar = %p\n", (void*)&localvar);
    printf("blocking on pause()...\n");
    pause();
}

static void bar(void) {
    int localvar = 5;
    printf("bar                &localvar = %p\n", (void*)&localvar);
    bar_is_now_closed();
}

static void foo(void) {
    int localvar = 5;
    printf("foo                &localvar = %p\n", (void*)&localvar);
    bar();
}

int main(int argc, char **argv) {
    int localvar = 5;
    printf("main               &localvar = %p\n", (void*)&localvar);
    foo();
    return 0;
}
