#ifndef INCLUDED_EVENTLOG_H
#define INCLUDED_EVENTLOG_H

#define EVENT_TIME_FORMAT "% %d %H:%M:%S"
#define EVENT_TIME_MAXLEN 48


/* functions */
int  D2GEEventLogInitialize(int d2ge_id);
void D2GEEventLogCleanup(void);
void D2GEEventLog(char const *module, char const *fmt, ...);
void LogAP(LPCSTR lpModule, LPCSTR lpFormat, va_list ap);

#endif
