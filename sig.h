// signals.h
#ifndef SIGNALS_H
#define SIGNALS_H

#include <signal.h>

// Function declarations
void handle_sigint(int sig); // Handle SIGINT (Ctrl+C)
void setup_signal_handlers(); // Set up all signal handlers

#endif
