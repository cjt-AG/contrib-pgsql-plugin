/*
 * Copyright (c) 2013 by Inteos sp. z o.o.
 * All rights reserved. See LICENSE.pgsql for details.
 *
 * Common definitions and utility functions for pgsql plugin.
 * Functions defines a common framework used in all utilities and plugins
 */

#ifndef _PGSQLLIB_H_
#define _PGSQLLIB_H_

#include <libpq-fe.h>
#include <sys/types.h>
#include <dirent.h>
#include <bareos.h>
#include "keylist.h"
#include "parseconfig.h"

/* definitions */

/* size of different string or SQL buffers */
#define CONNSTRLEN   256
#define BUFLEN       1024
#define SQLLEN       256
#define LOGMSGLEN    (6 + 32 + 7 + 24 + 1)

/* Assertions definitions */
#ifndef ASSERT_bfuncs
#define ASSERT_bfuncs \
    if (!bfuncs) \
    { \
        return bRC_Error; \
    }
#endif

#ifndef ASSERT_ctx
#define ASSERT_ctx \
    if (!ctx) \
    { \
        return bRC_Error; \
    }
#endif

#ifndef ASSERT_ctx_p_RET
#define ASSERT_ctx_p_RET \
    if (!ctx || !ctx->pContext) \
    { \
        return; \
    }
#endif

#ifndef ASSERT_ctx_p
#define ASSERT_ctx_p \
    if (!ctx || !ctx->pContext) \
    { \
        return bRC_Error; \
    }
#endif

#ifndef ASSERT_ctx_p_RET_NULL
#define ASSERT_ctx_p_RET_NULL \
    if (!ctx || !ctx->pContext) \
    { \
        return NULL; \
    }
#endif

/* check valid pointer if not simply return */
#ifndef ASSERT_NVAL_RET
#define ASSERT_NVAL_RET(value) \
    if (!value) { \
        return; \
    }
#endif

/* check valid pointer with Null return */
#ifndef ASSERT_NVAL_RET_NULL
#define ASSERT_NVAL_RET_NULL(value) \
    if (!value) \
    { \
        return NULL; \
    }
#endif

/* check valid pointer with int/err return */
#ifndef ASSERT_NVAL_RET_ONE
#define ASSERT_NVAL_RET_ONE(value) \
    if (!value) \
    { \
        return 1; \
    }
#endif

/* check valid pointer with int/err return */
#ifndef ASSERT_NVAL_RET_NONE
#define ASSERT_NVAL_RET_NONE(value) \
    if (!value) \
    { \
        return -1; \
    }
#endif

/* check valid pointer with bRC return */
#ifndef ASSERT_NVAL_RET_BRCERR
#define ASSERT_NVAL_RET_BRCERR(value) \
    if (!value) \
    { \
        return bRC_Error; \
    }
#endif

/* check valid pointer if not exit with error */
#ifndef ASSERT_NVAL_EXIT_BRCERR
#define ASSERT_NVAL_EXIT_BRCERR(value) \
    if (!value) { \
        exit(bRC_Error); \
    }
#endif

/* check error if not exit with error */
#ifndef ASSERT_NVAL_EXIT_ONE
#define ASSERT_NVAL_EXIT_ONE(value) \
    if (!value) { \
        exit (1); \
    }
#endif

/* check value if not simply return */
#ifndef ASSERT_VAL_RET
#define ASSERT_VAL_RET(value) \
    if (value) { \
        return; \
    }
#endif

/* checks error value then Null return */
#ifndef ASSERT_VAL_RET_NULL
#define ASSERT_VAL_RET_NULL(value) \
    if (value) { \
        return NULL; \
    }
#endif

/* checks error value then int/err return */
#ifndef ASSERT_VAL_RET_ONE
#define ASSERT_VAL_RET_ONE(value) \
    if (value) \
    { \
        return 1; \
    }
#endif

/* checks error value then int/err return */
#ifndef ASSERT_VAL_RET_NONE
#define ASSERT_VAL_RET_NONE(value) \
    if (value) \
    { \
        return -1; \
    }
#endif

/* checks error value then bRC return */
#ifndef ASSERT_VAL_RET_BRCERR
#define ASSERT_VAL_RET_BRCERR(value) \
    if (value) \
    { \
        return bRC_Error; \
    }
#endif

/* checks error value then exit one */
#ifndef ASSERT_VAL_EXIT_ONE
#define ASSERT_VAL_EXIT_ONE(value) \
    if (value) \
    { \
        exit(1); \
    }
#endif

/* memory allocation/deallocation */
#define MALLOC(size) \
    (char *) malloc(size);

#define FREE(ptr) \
    if (ptr) { \
        free(ptr); \
        ptr = NULL; \
    }

/* pgsql filetype */
typedef enum {
    PG_NONE  = 0,
    PG_FILE,
    PG_LINK,
    PG_DIR
} PGSQL_FILETYPE_T;

/* log levels (utils only) */
typedef enum {
    LOGERROR,
    LOGWARNING,
    LOGINFO
} LOG_LEVEL_T;

/* pgsql plugin or utils backup/restore modes */
typedef enum {
    PGSQL_NONE = 0,
    PGSQL_ARCH_BACKUP,
    PGSQL_ARCH_CTL_BACKUP,
    PGSQL_DB_BACKUP,
    PGSQL_DB_RESTORE,
    PGSQL_ARCH_RESTORE
} PGSQL_MODE_T;

/* restore point-in-time */
typedef enum {
    PITR_CURRENT,
    PITR_TIME,
    PITR_XID
} PGSQL_PITR_T;

/* pgsql_status definitions */
typedef enum {
    PGSQL_STATUS_UNKNOWN             = 0,

    PGSQL_STATUS_WAL_ARCH_START      = 1,
    PGSQL_STATUS_WAL_ARCH_INPROG     = 2,  /* unimplemented */
    PGSQL_STATUS_WAL_ARCH_FINISH     = 3,
    PGSQL_STATUS_WAL_ARCH_FAILED     = 4,
    PGSQL_STATUS_WAL_ARCH_MULTI      = 5,

    PGSQL_STATUS_WAL_BACK_START      = 6,
    PGSQL_STATUS_WAL_BACK_INPROG     = 7,  /* unimplemented */
    PGSQL_STATUS_WAL_BACK_DONE       = 8,
    PGSQL_STATUS_WAL_BACK_FAILED     = 9,  /* unimplemented */

    PGSQL_STATUS_DB_ONLINE_START     = 10, /* unimplemented */
    PGSQL_STATUS_DB_ONLINE_INPROG    = 11, /* unimplemented */
    PGSQL_STATUS_DB_ONLINE_FINISH    = 12, /* unimplemented */
    PGSQL_STATUS_DB_ONLINE_FAILED    = 13, /* unimplemented */

    PGSQL_STATUS_DB_OFFLINE_START    = 14, /* unimplemented */
    PGSQL_STATUS_DB_OFFLINE_INPROG   = 15, /* unimplemented */
    PGSQL_STATUS_DB_OFFLINE_FINISH   = 16, /* unimplemented */
    PGSQL_STATUS_DB_OFFLINE_FAILED   = 17  /* unimplemented */
} PGSQL_STATUS_T;

#define PGSQL_STATUS_WAL_OK    "3,5,8"
#define PGSQL_STATUS_WAL_ERR   "4,9"
#define PGSQL_STATUS_DB_OK     "12,16"
#define PGSQL_STATUS_DB_ERR    "13,17"

/* pgsqldata for pgsql-arch or pgsql-restore */
typedef struct _pgsqldata pgsqldata;
struct _pgsqldata
{
    keylist  * paramlist;
    int      mode;
    PGconn   * catdb;
    char     * configfile;
    char     * walfilename;
    char     * pathtowalfilename;
    int      pitr;
    char     * restorepit;
    char     * where;
    char     * restoreclient;
    int      verbose;
    int      bsock;
};

/* pgsqlpinst for pgsql-fd instance data */
typedef struct _pgsqlpinst pgsqlpinst;
struct _pgsqlpinst
{
    int      JobId;
    keylist  * paramlist;
    int      mode;
    PGconn   * catdb;
    char     * configfile;
    keylist  * filelist;
    keyitem  * curfile;
    int      curfd;
    int      diropen;
    char     * linkval;
    int      linkread;
};

/* uid/gid struct */
typedef struct _pgugid pgugid;
struct _pgugid {
    uid_t uid;
    gid_t gid;
};

/*
 * BAREOS console communication message structure
 */
#define MSGBUFLEN 1024
typedef struct _consmsgbuf consmsgbuf;
struct _consmsgbuf {
    int blen;
    char mbuf[MSGBUFLEN];
};

/*
 * IPC MSG Buffer
 */
typedef struct _PGSQL_MSG_BUF_T PGSQL_MSG_BUF_T;
struct _PGSQL_MSG_BUF_T {
    long mtype;
    char mtext[MSGBUFLEN];
};

/*
 * date/time verification function struct
 */
typedef struct _pg_time pg_time;
struct _pg_time {
    int y;
    int m;
    int d;
    int h;
    int mi;
    int s;
};


/* utility functions */
void pgsqllibinit(int argc, char * argv[]);
char * get_program_name(void);
char * get_program_directory(void);
pgsqldata * allocpdata(void);
pgsqlpinst * allocpinst(void);
void freepdata(pgsqldata * pdata);
void freepinst(pgsqlpinst * pinst);
char * logstr(char * msg, LOG_LEVEL_T level);
void logprg(LOG_LEVEL_T level, const char * msg);
void abortprg(pgsqldata * pdata, int err, const char * msg);
PGconn * catdbconnect(keylist * paramlist);
keylist * parse_pgsql_conf(char * configfile);
keylist * get_file_list(keylist * list, const char * base, const char * path);
int _get_walid_from_catalog(pgsqldata * pdata);
int _get_walstatus_from_catalog(pgsqldata * pdata);
int _insert_status_in_catalog(pgsqldata * pdata, int status);
int _update_status_in_catalog(pgsqldata * pdata, int pgid, int status);
int _copy_wal_file(pgsqldata * pdata, char * src, char * dst);
int _check_postgres_is_running(pgsqldata * pdata, char * pgdataloc);
const char * find_pgctl(pgsqldata * pdata);
int pgsql_msg_init(pgsqldata * pdata, int prg);
int pgsql_msg_send(pgsqldata * pdata, int msqid, uint type, const char * message);
int pgsql_msg_recv(pgsqldata * pdata, int msqid, int type, char * message);
void pgsql_msg_shutdown(pgsqldata * pdata, int msqid);
char * format_btime(const char * str);

#endif /* _PGSQLLIB_H_ */
