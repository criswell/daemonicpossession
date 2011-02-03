/*
 * logger.cpp
 * ----------
 * Description:
 *   Basic logging class
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

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>

#include "logger.h"

Logger::Logger(char *logFile)
{
    logFilename = logFile;

    isLogging = false;
    useAltPipe = false;
}

Logger::Logger(FILE *altPipe)
{
    logPipe = altPipe;
    useAltPipe = true;
    isLogging = true;
}

Logger::~Logger()
{
    if(isLogging)
    {
        // TODO close out logging
        EndLogging();
    }
}

bool Logger::BeginLogging()
{
    if(useAltPipe) {
        return true;
    } else if(!isLogging) {
        if ( !(logPipe = (FILE*)fopen(logFilename, "a+")) )
        {
            perror("Problems opening log file for appending!");
            return false;
        }
        isLogging = true;

        return true;
    } else {
        return false;
    }
}

bool Logger::EndLogging()
{
    if(useAltPipe) {
        return true;
    } else if(isLogging)
    {
        fclose(logPipe);
        isLogging = false;

        return true;
    } else {
        return false;
    }
}

bool Logger::InLoggingSession()
{
    return isLogging;
}

bool Logger::innerLogEntry()
{
    if(isLogging)
    {
        fputs(logLine, logPipe);
        return true;
    } else {
        return false;
    }
}

bool Logger::LogEntry(char *text, ...)
{
    if(isLogging)
    {
        va_list argp;
        timer = time(NULL);
        ts = localtime(&timer);
        strftime(timeStamp, sizeof(timeStamp),
                 "%Y-%m-%d %H:%M:%S %Z", ts);
        va_start(argp, text);
        if( !(vsnprintf(tempLine, TEMP_LINE_LENGTH,
            text, argp)) )
        {
            perror("Log entry too long!");
            perror(text);
            return false;
        }
        va_end(argp);

        if( !(snprintf(
            logLine, LOG_LINE_LENGTH,
            "%s : %s\n", timeStamp, tempLine)) )
        {
            perror("Log entry too long!");
            perror(text);
            return false;
        }

        return innerLogEntry();
    } else {
        return false;
    }
}

void Logger::QuickLog(char *text, ...)
{
    if(isLogging) {
        // If we are already logging, then we must do the logical thing for
        // a quick log function, and that is:
        // - Log our entry
        // - Flush the cache
        // - Begin logging again, thus restoring state
        va_list argp;
        timer = time(NULL);
        ts = localtime(&timer);
        strftime(timeStamp, sizeof(timeStamp),
                 "%Y-%m-%d %H:%M:%S %Z", ts);
        va_start(argp, text);
        if( !(vsnprintf(tempLine, TEMP_LINE_LENGTH,
            text, argp)) )
        {
            perror("Log entry too long!");
            perror(text);
        }
        va_end(argp);

        if( !(snprintf(
            logLine, LOG_LINE_LENGTH,
            "%s : %s\n", timeStamp, tempLine)) )
        {
            perror("Log entry too long!");
            perror(text);
        }

        innerLogEntry();
        EndLogging();
        BeginLogging();
    } else {
        BeginLogging();
        va_list argp;
        timer = time(NULL);
        ts = localtime(&timer);
        strftime(timeStamp, sizeof(timeStamp),
                 "%Y-%m-%d %H:%M:%S %Z", ts);
        va_start(argp, text);
        if( !(vsnprintf(tempLine, TEMP_LINE_LENGTH,
            text, argp)) )
        {
            perror("Log entry too long!");
            perror(text);
        }
        va_end(argp);

        if( !(snprintf(
            logLine, LOG_LINE_LENGTH,
            "%s : %s\n", timeStamp, tempLine)) )
        {
            perror("Log entry too long!");
            perror(text);
        }

        innerLogEntry();
        EndLogging();
    }
}
