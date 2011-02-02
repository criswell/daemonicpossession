#ifndef systemState
#define systemState

//! The state of the daemon itself
/*!
 * This enum defines the various states the daemon can be in. When signals are
 * sent to the daemon, the signal handlers set the daemon in one of these states
 * to control execution of the daemon code.
 * \li \c S_STATE_Null This is an undefined state, the daemon should only be in
 * this state when it is in startup mode.
 * \li \c S_STATE_Running Normal running state. When the daemon is operating
 * normally, this is the state it is in.
 * \li \c S_STATE_Shutdown This is the state the daemon switches to when it has
 * received a SIGHUP or SIGINT. In this state it closes "nicely", meaning it
 * attempts to shut everything down properly and exit cleanly.
 * \li \c S_STATE_Terminate This the state the daemon switches to when it has
 * received a SIGTERM. This state tells the daemon to terminate execution as
 * soon as possible, and not worry about closing things down cleanly.
 */
enum SystemState
{
    S_STATE_Null,
    S_STATE_Running,
    S_STATE_Shutdown,
    S_STATE_Terminate
};

#endif
