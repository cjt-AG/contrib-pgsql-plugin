CREATE USER pgcat PASSWORD 'pgcat';

-- For tables
GRANT ALL ON pgsql_version TO pgcat;
GRANT ALL ON pgsql_status TO pgcat;
GRANT ALL ON pgsql_backupdbs TO pgcat;
GRANT ALL ON pgsql_archivelogs TO pgcat;

-- For sequences ON those tables
GRANT SELECT, UPDATE ON pgsql_archivelogs_id_seq TO pgcat;
GRANT SELECT, UPDATE ON pgsql_backupdbs_id_seq TO pgcat;
