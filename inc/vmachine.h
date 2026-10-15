#pragma once

#define MAXSTACKSIZE 1024
extern double stack[MAXSTACKSIZE];
extern int sp; //stack pointer
extern double acc;
extern void push(void);
extern double pop(void);
extern void store(const char *name);
extern double recall(const char *name);
