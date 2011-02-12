/*
 * logger header
 */

/*! \mainpage Logger
 *
 * \section intro_sec Introduction
 *
 * This is the class which handles the logging
 *
 * \section det_sec Details
 *
 * This logging class is intended to provide an abstracted interface which
 * wraps multiple ways to log to system files. It can be used in one of two
 * ways:
 *
 * \subsection dual_log Dual system and private logging
 *
 * This is the standard way to invoke and create the logging class instance.
 * When created this way, the class instance will log to a specified file
 * (a private log) as well as to syslog. This allows for a private log which
 * can be retained, moved, etc. and which \b only contains the log details for
 * this daemon. But at the same time, we will also be logging to a system-wide
 * log which will allow for external monitoring software which may be
 * aggregating data across multple systems to collect the log information from
 * our daemon.
 *
 * To invoke this logger class in this way, you must call
 * Logger::Logger(char *logFile).
 *
 */

#ifndef logger_H
#define logger_H

#include <stdio.h>
#include <time.h>
#include <syslog.h>

// Constants

//! Maximum length of the log line
#define LOG_LINE_LENGTH 512
#define TEMP_LINE_LENGTH 480

//! Logger
class Logger
{
    private:
        char *logFilename;
        bool isLogging;
        bool useAltPipe;
        bool useSyslog;
        int syslogPriority;
        FILE *logPipe;
        char logLine[LOG_LINE_LENGTH];
        char tempLine[TEMP_LINE_LENGTH];
        time_t timer;
        struct tm *ts;
        char timeStamp[80];

        //! private method for actually writing the log entry to the log file
        bool innerLogEntry();
    public:
        //! Constructor for the Logger
        /*!
         *  \param ident is the application identifier, used by syslog to prepend each log message
         *  \param logFile is a character string which details the path to the log file
         */
        Logger(const char *ident, char *logFile);

        //! Alternative constructor which forces the use of a specific pipe
        /*!
         * Use this constructor if you want to send to STDOUT (for example)
         *
         * \param altPipe is the alternative pipe to use
         */
        Logger(FILE *altPipe);

        //! Destructor for the Logger
        ~Logger();

        //! Set the priority for syslog
        /*!
         * This sets the priority for future syslog calls. Rather than doing
         * priority changes on each call to log entries, we just set them
         * periodically as needed.
         *
         * \note The default priority for this logging class will be \b LOG_INFO
         *
         * \param priority Bitwise ORed syslog priority level
         */
        void SetPriority(int priority);

        //! Begin a logging session
        /*!
         * Call this when you are ready to begin logging. The logger
         * will prepare the log file and put itself in a state where
         * future writes can be made.
         *
         * \return True on success, false on failure.
         */
        bool BeginLogging();

        //! End a logging session
        /*!
         * Call this when you are ready to end a logging session. Note
         * that you should not call this unless you are currently logging,
         * e.g., unless you have previously called BeginLogging().
         *
         * \return True on success, false on failure.
         */
        bool EndLogging();

        //! Determine if we are in a logging session or not
        /*!
         * \return True if we are currently in a logging session, false if not
         */
        bool InLoggingSession();

        //! Log an entry
        /*!
         * Called when you wish to log an entry. Note that you should
         * only call this during a logging session (between calls to
         * BeginLogging() and EndLogging()). If in doubt, consult
         * InLoggingSession().
         *
         * \return True if entry was successful, false if not.
         */
        bool LogEntry(char *text, ...);

        //! Quickly log an entry
        /*!
         * Call this when you wish to quickly log an entry without having
         * to call begin/end. Note that this does NOT eliminate the steps
         * required to enable logging, it simply masks them. This log
         * method should only be used when a quick one-liner is to be
         * logged, not when there are extended log entries to be written.
         */
        void QuickLog(char *text, ...);
};

#endif
