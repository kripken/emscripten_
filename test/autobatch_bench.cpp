#include <emscripten.h>
#include <cstdio>

int frames = 0;
int total = 0;

double last = 0;

// Flush the autobatch command buffer.
void flush() {
  double t = emscripten_get_now();
  if (!t) exit(1); // use the value in a silly way

  // Code to paste into the JS:

  /*

    , autobatch: { flush:

      // autobatch JS output goes here

    }

  */
}

EM_JS(void, bump, (int i), {
  Module.bump += i;
});

void work() {
  for (int i = 0; i < 10*1024; i++) {
    for (int j = 0; j < 1024; j++) {
      bump(j);
    }
    flush();
  }
}

void update(void* user_data) {
  double ms = emscripten_get_now();

  // Calculate FPS
  if (ms - last >= 1000.0) {
    printf("FPS: %d\n", frames);
    frames = 0;
    last = ms;
  }
  frames++;
  total++;

  work();

  flush();
}

int main() {
  emscripten_set_main_loop_arg(update, nullptr, 0, 0);
  flush();

  return 0;
}
