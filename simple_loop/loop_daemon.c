/*
 * loop_daemon.c
 * -------------
 * Description:
 *  A basic looping daemon pattern
 *
 * Copyright (C) 2010-2011 by Sam Hart
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

//#include <stdio.h>
//#include <unistd.h>
//#include <stdlib.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <signal.h>
//#include <strings.h>

/* These can be defined here, or generated programatically, your choice.
 * For demonstration purposes, however, we just define them here and call it
 * good.
 */
#define LOGFILE "/var/log/loop_daemon.log"
#define PIDFILE "/var/run/loop_daemon.pid"

// Various helper libraries
#include "logger.h"
#include "systemState.h"

//! The current system state
static volatile SystemState S_STATE;

void handle_SIGHUP(int sig)
{
    S_STATE = S_STATE_Shutdown;
}

void handle_SIGINT(int sig)
{
    S_STATE = S_STATE_Shutdown;
}

void handle_SIGTERM(int sig)
{
    S_STATE = S_STATE_Terminate;
}

void setupSignalHandlers()
{
    static bool hasSetup = false;
    if(!hasSetup)
    {
        struct sigaction act_SIGHUP, act_SIGINT, act_SIGTERM, act_OLD;

        // Set up SIGHUP
        act_SIGHUP.sa_handler = handle_SIGHUP;
        sigemptyset (&act_SIGHUP.sa_mask);
        act_SIGHUP.sa_flags = 0;

        sigaction (SIGHUP, NULL, &act_OLD);
        if (act_OLD.sa_handler != SIG_IGN)
            sigaction (SIGHUP, &act_SIGHUP, NULL);
        else
        {
            sigaction (SIGHUP, &act_SIGHUP, &act_OLD);
        }

        // Set up SIGINT
        act_SIGINT.sa_handler = handle_SIGINT;
        sigemptyset (&act_SIGINT.sa_mask);
        act_SIGINT.sa_flags = 0;

        sigaction (SIGINT, NULL, &act_OLD);
        if (act_OLD.sa_handler != SIG_IGN)
            sigaction (SIGINT, &act_SIGINT, NULL);
        else
        {
            sigaction (SIGINT, &act_SIGINT, &act_OLD);
        }

        // Set up SIGTERM
        act_SIGTERM.sa_handler = handle_SIGTERM;
        sigemptyset (&act_SIGTERM.sa_mask);
        act_SIGTERM.sa_flags = 0;

        sigaction (SIGTERM, NULL, &act_OLD);
        if (act_OLD.sa_handler != SIG_IGN)
            sigaction (SIGTERM, &act_SIGTERM, NULL);
        else
        {
            sigaction (SIGTERM, &act_SIGTERM, &act_OLD);
        }
    }
}

int main(int argc, char *argv[])
{
    // Misc variables to be used by the daemon
    #ifndef DEBUG
    pid_t pid, sid;
    #endif
    FILE *filePipe;

    // The PID related variable
    char pidOut[256];

    #ifndef DEBUG
    // Obtain the PID file
    // FIXME

    // Since we're a daemon, let's start by forking from parent
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    // Exit if we have an acceptable pid
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    #endif
    // File mode mask so we can have full access
    umask(0);

    #ifndef DEBUG
    Logger logger(LOGFILE);
    #else
    Logger logger(stdout);
    #endif

    logger.BeginLogging();
    logger.LogEntry("-----------------------------------");
    logger.EndLogging();

    #ifndef DEBUG
    // Obtain a new session ID for child process
    sid = setsid();
    if (sid < 0) {
        logger.QuickLog(
            "Could not obtain new session ID for child process");
        exit(EXIT_FAILURE);
    }

    logger.QuickLog("New Session ID obtained");
    if ( !(filePipe = (FILE*)fopen(PIDFILE, "w")) )
    {
        perror("Problems opening PID file for writing!");
        return false;
    }
    snprintf(pidOut, 256, "%d\n", sid);
    fputs(pidOut, filePipe);
    pclose(filePipe);
    #endif

    // TODO - Change to working directory

    #ifndef DEBUG
    // File descriptors are a security hazard in a daemon
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    logger.QuickLog("STD i/o closed");
    #endif

    // Set up signal handling
    logger.QuickLog("Set up signal handlers...");
    setupSignalHandlers();

    S_STATE = S_STATE_Running;

    /* Sanity checks */

    // Main loop
    while (S_STATE == S_STATE_Running) {
        logger.BeginLogging();
        logger.LogEntry("Starting activity");

        logger.LogEntry("Sleeping for 30 seconds");
        logger.EndLogging();
        sleep(30);
    }

    logger.QuickLog("Signal caught, exit daemon...");
    if (S_STATE == S_STATE_Shutdown)
    {
        // We have time, let's do this right
        // TODO cleanup
        logger.EndLogging();
        logger.~Logger();
    }

    unlink(PIDFILE);
}
