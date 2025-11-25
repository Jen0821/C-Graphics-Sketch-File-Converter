// Basic program skeleton for a Sketch File (.sk) Viewer
#include "displayfull.h"
#include "sketch.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Allocate memory for a drawing state and initialise it
state *newState(void) {
    state *s = malloc(sizeof(state));
    *s = (state){0, 0, 0, 0, LINE, 0, 0, false};
    return s;
}

// Release all memory associated with the drawing state
void freeState(state *s) {
    free(s);
}

// Extract an opcode from a byte (two most significant bits).
int getOpcode(byte b) {
    return b >> 6;
}

// Extract an operand (-32..31) from the rightmost 6 bits of a byte.
int getOperand(byte b) {
    int rawOperand = b & 0x3F;
    return (rawOperand >= 32) ? rawOperand - 64 : rawOperand;
}

// Execute the next byte of the command sequence.
void obey(display *d, state *s, byte op) {
    int opcode = getOpcode(op);
    int operand = getOperand(op);

    if (opcode == 0) {
        s->tx += operand;
    } else if (opcode == 1) {
        s->ty += operand;
        if (s->tool == LINE) {
            line(d, s->x, s->y, s->tx, s->ty);
        } else if (s->tool == BLOCK) {
            block(d, s->x, s->y, s->tx - s->x, s->ty - s->y);
        }
        s->x = s->tx;
        s->y = s->ty;
    } else if (opcode == 2) {
        switch (operand) {
            case 0: s->tool = NONE; break;
            case 1: s->tool = LINE; break;
            case 2: s->tool = BLOCK; break;
            case 3: colour(d, s->data); break;
            case 4: s->tx = s->data; break;
            case 5: s->ty = s->data; break;
            case 6: show(d); break;
            case 7: pause(d, s->data); break;
            case 8: s->end = true; break;
        }
        s->data = 0;
    } else {
        s->data = (s->data << 6) | (operand & 0x3F);
    }
}

// Draw a frame of the sketch file.
bool processSketch(display *d, const char pressedKey, void *data) {
    if (!data) {
        return (pressedKey == 27);
    }

    state *s = (state *)data;
    char *filename = getName(d);
    FILE *file = fopen(filename, "rb");
    if (!file) return true;

    for (int i = 0; i < s->start && !feof(file); i++) {
        fgetc(file);
    }

    while (!feof(file) && !s->end) {
        byte instruction = fgetc(file);
        obey(d, s, instruction);
    }

    show(d);
    fclose(file);

    *s = (state){0, 0, 0, 0, LINE, s->start, 0, false};
    return (pressedKey == 27);
}

// Step 5: Advanced Sketch File Support
bool processSketchAdvanced(display *d, const char pressedKey, void *data) {
    if (!data) {
        return (pressedKey == 27);
    }

    state *s = (state *)data;
    char *filename = getName(d);
    FILE *file = fopen(filename, "rb");
    if (!file) return true;

    for (int i = 0; i < s->start && !feof(file); i++) {
        fgetc(file);
    }

    while (!feof(file) && !s->end) {
        byte instruction = fgetc(file);
        obey(d, s, instruction);
    }

    show(d);
    fclose(file);

    *s = (state){0, 0, 0, 0, LINE, s->start, 0, false};
    return (pressedKey == 27);
}

// View a sketch file in a 200x200 pixel window given the filename
void view(char *filename) {
    display *d = newDisplay(filename, 200, 200);
    state *s = newState();
    run(d, s, processSketch); // Change to `processSketchAdvanced` if Step 5 is active
    freeState(s);
    freeDisplay(d);
}

// Include a main function only if we are not testing (make sketch),
// otherwise use the main function of the test.c file (make test).
#ifndef TESTING
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Use ./sketch file\n");
        exit(1);
    }
    view(argv[1]);
    return 0;
}
#endif