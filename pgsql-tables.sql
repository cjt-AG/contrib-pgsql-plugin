-- 
-- Copyright (c) 2013 by Inteos sp. z o.o.
-- All rights reserved. See LICENSE.pgsql for details.
-- 
-- Catalog tables definitions
-- 

-- version table
DROP TABLE IF EXISTS pgsql_version CASCADE;
CREATE TABLE pgsql_version (
   versionid    INTEGER NOT NULL
);
INSERT INTO pgsql_version ( versionid ) VALUES ('1');

DROP TABLE IF EXISTS pgsql_status CASCADE;
CREATE TABLE pgsql_status (
   statusid    INTEGER UNIQUE NOT NULL,
   notes       varchar
);

-- 
-- /* pgsql_status definitions */
-- typedef enum {
--    PGSQL_STATUS_UNKNOWN             = 0,
--    PGSQL_STATUS_WAL_ARCH_START      = 1,
--    PGSQL_STATUS_WAL_ARCH_INPROG     = 2,
--    PGSQL_STATUS_WAL_ARCH_FINISH     = 3,
--    PGSQL_STATUS_WAL_ARCH_FAILED     = 4,
--    PGSQL_STATUS_WAL_ARCH_MULTI      = 5,
--    PGSQL_STATUS_WAL_BACK_START      = 6,
--    PGSQL_STATUS_WAL_BACK_INPROG     = 7,
--    PGSQL_STATUS_WAL_BACK_DONE       = 8,
--    PGSQL_STATUS_WAL_BACK_FAILED     = 9,
--    PGSQL_STATUS_DB_ONLINE_START     = 10, 
--    PGSQL_STATUS_DB_ONLINE_INPROG    = 11,
--    PGSQL_STATUS_DB_ONLINE_FINISH    = 12,
--    PGSQL_STATUS_DB_ONLINE_FAILED    = 13,
--    PGSQL_STATUS_DB_OFFLINE_START    = 14,
--    PGSQL_STATUS_DB_OFFLINE_INPROG   = 15,
--    PGSQL_STATUS_DB_OFFLINE_FINISH   = 16,
--    PGSQL_STATUS_DB_OFFLINE_FAILED   = 17
-- } PGSQL_STATUS;
-- 
INSERT INTO pgsql_status values ('0','Unknown');
INSERT INTO pgsql_status values ('1','WAL archiving started');
INSERT INTO pgsql_status values ('2','WAL archiving in progress');
INSERT INTO pgsql_status values ('3','WAL archiving finished');
INSERT INTO pgsql_status values ('4','WAL archiving failed');
INSERT INTO pgsql_status values ('5','WAL multiply archiving occured');
INSERT INTO pgsql_status values ('6','WAL backup started');
INSERT INTO pgsql_status values ('7','WAL backup in progress');
INSERT INTO pgsql_status values ('8','WAL backup done');
INSERT INTO pgsql_status values ('9','WAL backup failed');
INSERT INTO pgsql_status values ('10','DB online backup started');
INSERT INTO pgsql_status values ('11','DB online backup in progress');
INSERT INTO pgsql_status values ('12','DB online backup finished');
INSERT INTO pgsql_status values ('13','DB online backup failed');
INSERT INTO pgsql_status values ('14','DB offline backup started');
INSERT INTO pgsql_status values ('15','DB offline backup in progress');
INSERT INTO pgsql_status values ('16','DB offline backup finished');
INSERT INTO pgsql_status values ('17','DB offline backup failed');

DROP TABLE IF EXISTS pgsql_backupdbs CASCADE;
CREATE TABLE pgsql_backupdbs (
   id          serial,
   client      varchar not null,
   start_date  timestamp default now(),
   end_date    timestamp default null,
   start_xid   varchar not null,
   end_xid     varchar not null,
   blevel      integer not null default 0,
   status      integer not null default 0,
   FOREIGN KEY (status) REFERENCES pgsql_status (statusid)
);

DROP TABLE IF EXISTS pgsql_archivelogs CASCADE;
CREATE TABLE pgsql_archivelogs (
   id          serial,
   client      varchar not null,
   filename    varchar not null,
   create_date timestamp default now(),
   mod_date    timestamp default now(),
   status      integer not null default 0,
   UNIQUE (client, filename),
   FOREIGN KEY (status) REFERENCES pgsql_status (statusid)
);
