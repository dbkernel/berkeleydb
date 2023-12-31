This is the changelog for Berkeley DB Release 18.1 (library version 18.1.40).

### Changes between version 18.1.32 and version 18.1.40

1. Removed ASM library source files, ASM license from LICENSE file, and DPL test targets with ASM optimizations.
2. **The Berkeley DB client-server architecture is no longer supported. If you require the Berkeley DB client-server architecture you must use Berkeley DB 18.1.32 or earlier**.
3. **The SQL API is no longer supported**. If you require SQL support you must use Berkeley DB 18.1.32 or earlier.
4. Android is no longer supported. If you require Android support you must use Berkeley DB 18.1.32 or earlier.
5. Fixed several possible crashes when running db_verify on a corrupted database. [#27864]
6. Fixed several possible hangs when running db_verify on a corrupted database. [#27864]
7. Added a warning message when attempting to verify a queue database which has many extent files. Verification will take a long time if there are many extent files. [#27864]

### Changes between version 18.1.25 and version 18.1.32

1. Fixed a few null pointer crashes in the Tcl API when a function returned a null value. [#26331]
2. Automatically copy any databases in sub-directories when using the utility db_hotbackup or the ENV->backup() function by including the -C command line argument or the flag DB_BACKUP_DEEP_COPY, respectively [#26736].
3. Fixed a "malformed disk error" caused by not properly refreshing the thread info. [#26945]
4. If a database rename or remove operation fails due to replication lockout, DB_REP_LOCKOUT will now be returned instead of EINVAL. [#27010]
5. Fixed an "invalid page format" error when performing a key_range search on an empty database. [#27011]
6. Fixed a bug in the JNI API involving the function Environment.lockVector(). [#27068]
7. Added the option -S to the db_convert and db_tuner utilities, which causes db_verify to verify the database before running the utility. [#27080]
8. Improved stability with several changes that prevent crashes when running db_verify against corrupt files. [#27088]
9. Improved the reliability of DB_ENV->open(DB_REGISTER) when the Berkeley DB application is the principal process of a Docker container which is stopped and restarted. [#27100]
10. Fixed build failure in repmgr ssl support for openssl version 1.1.0. [#27152]
11. Added support for Java 10. [#27154]
12. Added the db_sqlthreadtest project to Windows. [#27170]
13. Fixed a possible crash when running db_verify on a corrupted DB_QUEUE database. [#27179]
14. Improved the performance of the purging process performed when the number of free mutexes is low. [#27219]
15. Fixed an error in the repmgr035.tcl test. [#27392]
16. The Berkeley DB client-server architecture has been updated to use Thrift 0.11.0. [#27407]

### Database or Log File On-Disk Format Changes

1. The log file format did not change in library version 18.1
2. The database file formats did not change in library version 18.1.

### New Features/Improvements

1. Multiversion concurrency control and snapshot isolation are now supported on replication clients. [#21968]
2. Several new types of objects have been added to the DB_ENV->set_memory_init() method to enable an application to account for any additional environment region memory needed for replication. [#24153]
3. There are several new pieces of information available to replicated applications. The DB_EVENT_REP_AUTOTAKEOVER event indicates which subordinate process has taken over as the new replication process. The st_group_stable_log_file Replication Manager statistic indicates the earliest log file that is still needed by any site in the replication group. The DB_REPMGR_SITE structure now contains a max_ack_lsn field that indicates to a master site the most recently acknowledged LSN on each replica. The DB_REPMGR_SITE structure now also sets a new flag value DB_REPMGR_ISELECTABLE. [#25073]
4. Added a utility program "db_convert" to convert between big-endian (e.g. SPARC) and little-endian (e.g., X86) machines. This supports multi-platform applications which standardize on one endianness, even when both big-endian and little-endian machines are used. Since db_upgrade requires the database to be in the local machine's endianness, such applications can use db_convert to translate the byte order of the database into the local format. Then, the application can run db_upgrade. [#25086]
5. Increased the number of instances in which failchk can successfully clean the environment without requiring full recovery after a thread crashes while using Berkeley DB. [#25087]
6. Added DPL APIs to Berkeley DB Java client driver. [#25089]
7. Added support for the C11/C++11 standards. [#25416]
8. Added a graphical user interface "db_gui" to Berkeley DB that allows easy browsing of database data and execution of some administrative tasks such as running recovery. [#25496]
9. The replication manager has been changed to use poll() and epoll() in preference to select() to manage connections. This scales better for applications with a very large number of databases, network connections, or files. [#25795]
10. Starting with this release, Berkeley DB products are licensed in two ways depending on the source of the package. Packages downloaded from the Oracle Technology Network (OTN) are licensed under the GNU AFFERO GENERAL PUBLIC LICENSE (AGPL), version 3. Packages downloaded from the Oracle Software Data Cloud (OSDC) are licensed under the Oracle Master Agreement. Example code included in the packages is in all cases licensed under the Universal Permissive License. [#25916]
11. Added the -S command line option to db_stat, db_dump, and db_upgrade, which verifies the correctness of the database before executing the utilities. [#26207]
12. The version numbering scheme for Berkeley DB changed from a five-part number to a three-part number of the form <year>.<release>.<build>. This release is numbered 18.1.x, indicating that it is the first release of the year 2018. 18.1.x is the successor to the 6.2.x (also known as 12.2.6.2.x) releases. The third and fourth parameters of db_full_version() are no longer meaningful and are now deprecated; if present, they are are set to zero. [#26743]

### Database Environment Changes

1. Improved the compatibility of DB_FAILCHK with DB_REGISTER. [#25204]
2. Improve environment 'panic' detection so that it is much less likely for processes to incorrectly use outdated environment handles after the environment has been recovered. [#25204]
3. Add the error code DB_SYSTEM_MEM_MISSING which is used when a System-V style shared memory segment is not found during DB_ENV->open(). [#25204]
4. Fixed a bug where setting the absolute path of a file on Windows could result in the file being created on the wrong disk. [#25284]
5. Syncing a database with external files will now also sync the meta database associated with the external files. [#25284]
6. Include a new field in the per-thread statistics output: the number of locked mutexes. [#25690]

### Concurrent Data Store Changes

1. None.

### Access Method Changes

1. Improved parallelism of DB->open() calls to open a DB_BTREE when the database name is NULL; that is, when opening a file that does not contain sub-databases. [#25624]
2. Fixed a bug on btree databases using unusually large keys that can occur when compaction is performed. [#25847]

### API Changes

1. Added DB->convert() which converts database file byte order. [#25086]

### SQL-specific API Changes

1. Fixed a hang which could occur when Berkeley DB errors are returned to the SQLite layer. [#23081]
2. SQLite user authentication extension now always requires encryption. To build with the authentication extension encryption must be enabled. Otherwise, it results in a compiler error. To convert a database to require authentication, the database must be encrypted. sqlite3_user_authenticate() now always returns SQLITE_AUTH if called on a database that does not require authentication. [#24598]
3. Added PRAGMA bdbsql_upgrade, which upgrades the connected database. If the database is encrypted, the encryption key must be set before calling this pragma. If the database uses keystore authentication and the encryption key is not unknown, a user must log in before calling this pragma. [#24598]
4. Added PRAGMA statistics, which prints out statistics about the database and its environment. [#25550]
5. Added PRAGMA statistics_files, which redirects output from the statistics PRAGMA to a file. [#25550]
6. Internal Berkeley DB messages are now redirected to the same file where internal Berkeley DB error messages are redirected. [#25568]
7. Updated SQL API build to be compatible with SQLite 3.18.2. [#25986]
8. Fixed a race condition if multiple threads simultaneously attempt to create a previously non-existing on-disk environment. [#26708]

### Tcl-specific API Changes

1. None.

### Java-specific API Changes

1. Added support for Java 9. [#26847]
2. Fixed a compile warning in BytecodeEnhancer.java. [#26824]
3. Fixed a possible crash when using the Slices feature through the Java API. [#24988]
4. Added Database.convert() which converts database file byte order. [#25086]
5. Fixed a bug that could have caused duplicate records to be returned via the iterator() method of the DPL and Collections API. The iterator reads records in batches, and if a record at the end of the last batch was deleted by another thread, fetching the next batch could have read (and later returned via the iterator) duplicate records, depending on thread timing. [#25976]

### C#-specific API Changes

1. Added Database.convert() which converts database file byte order. [#25086]

### Replication Changes

1. Fixed a bug preventing some changes to external files performed through the DB_STREAM API from replicating to the clients. [#25432]
2. Fixed a bug that could cause a client undergoing internal initialization to fail to request all the necessary logs for a correct recovery. The major symptom of this bug was that some client database files were left with empty pages that would later cause log sequence errors or other failures. [#25624]
3. Fixed a bug where clients in the replication group might panic during synchronization with the master. [#25800]
4. Fixed a bug where client specific log records resulted in incorrect calculation of subsequent log offset. This would later result in a DB panic on master. [#26090]

### Locking Subsystem Changes

1. Eliminated a spurious crash in CDB environments that use both --enable-diagnostic and DB_ENV->set_thread_count(). [#26149]

### Logging Subsystem Changes

1. None.

### Memory Pool Subsystem Changes

1. When the number of free mutexes is low, removed databases and closed unnamed in-memory databases are purged from the memory pool to free mutexes. [#24974]

### Mutex Subsystem Changes

1. Fixed a bug where an application might hang because it could not obtain a TAS lock when running Linux on Sparc architecture. [#24805]
2. Fixed a bug where mutexes could not be acquired on arm64-v8a Android systems. [#25752]
3. Fixed the panic "No space available in latch table" which could occur on systems where conventional (non-hybrid) Pthreads-based mutexes are used. [#26903].

### Transaction Subsystem Changes

1. Added a new error message when opening an environment which is still being recovered: Recovery is still running on the newly created (<start>) environment The <start> field contains the ctime(3) date when recovery started. [#25204]
2. Improve the robustness of applications that ignore certain error return codes from DB_ENV->txn_begin() and incorrectly use the transaction handle. Before this fix, DB_ENV->txn_begin() did not always set the returned transaction pointer to NULL when returning a DB_REP_LOCKOUT, EINVAL, or DB_RUNRECOVERY error. Now this pointer is set to NULL in all cases, making it more apparent if the application later ignores the DB_ENV->txn_begin() failure. [#25607]

### Test Suite Changes

1. Added test memp009 and mut004. [#24974]

### Utility Changes

1. Removed the db_sql_codegen utility. [#25030]
2. Fixed a possible segfault in db_verify when used with external files. [#25920]
3. Fixed several out-of-bounds memory reads and divide-by-zero bugs in the utility programs. [#26270]
4. The -c option of db_hotbackup can now work with replication manager applications. The -c option cannot be used with base replication applications. [#25702]

### Configuration, Documentation, Sample Apps, Portability and Build Changes

1. Removed support for Windows Mobile. [#24675]
2. Fixed a bug where built-in atomic functions are used only when --enable-cxx is specified. [#24974]
3. Added cross-compile support for detecting built-in atomic functions. [#25055]
4. Re-organized message numbers so that identical messages now have the same message number. [#25196]
5. Fixed a configure bug which prevented shared library support on FreeBSD 10. [#25251]
6. Added support for -with-mutex=BSD/OSSpinLockTry, to avoid using the undocumented spin_lock_try function, which is not permitted on iOS. [#25342]
7. Fixed a bug where ADO .Net package had debug pre-processors for release configurations. [#25549]
8. Fixed a bug where the configure script may fail to find the JNI include directories on macOS. [#25713]
9. Fixed a bug where the JAVACFLAGS environment variable was not passed to the JDBC driver. [#25752]
10. Added the ARM platform to Visual Studio 2015 projects and solutions. [#25860]
11. Added Visual Studio 2015 projects and solutions. [#25860]
12. Removed support for Visual Studio 2008. [#25860]
13. Environments are now automatically recreated the first time a new version of Berkeley DB opens the environment, if the flock() system call is available. This is typical for versions of Linux. [#25884]
14. Added support for VxWorks 7. [#25930]
15. When opening a database handle which was created outside of an environment, if the current working directory contains a DB_CONFIG file it will no longer be processed. [#26319]
16. Removed support for QNX. [#26339]
17. Fixed a documentation mistake that stated that the flag DB_VERB_BACKUP should be passed to DB_ENV->backup() instead of DB_ENV->set_verbose(). [#26501]
18. Added support for PHP 7. [#26566]
19. Removed support for PHP 5 and below and the php function DB4->pget. [#26566]
20. Third party JDBC driver code was removed from the distribution. Instructions have been added about how to download and integrate with Berkeley DB. [#26746]
21. Third party ADO.NET driver code was removed from the distribution. Instructions have been added about how to download and integrate with Berkeley DB. [#26746]

### Example Changes

1. Fixed some unchecked and deprecated warnings in the Java examples.[#26123]

### Miscellaneous Bug Fixes

1. Fixed a bug where SQL authentication APIs might raise assertion failures. [#24598]
2. Fixed undefined symbol errors for configurations including --enable-dtrace and -enable-perfmon_statistics when replication is also enabled. [#25061]
3. Improved db_verify's handling of corrupted database files. [#25239]
4. Fixed bugs where printed statistics might contain "unknown" flags. [#25461]
5. Fixed a bug where hot backup of queue extent files did not follow the original directory structure. [#25545]
6. The function flag DB_BACKUP_NO_LOGS now prevents logs from being backed up when passed to the backup function. [#25651]
7. Fixed a bug on Linux systems where temporary files would not be properly deleted. [#25997]
8. Fixed a bug where setting initial number of lockers might cause a segfault. [#26085]

### Deprecated Features

1. The BDB SQL test suite is no longer part of the release package. [#26749]
2. 32-bit Windows binaries will no longer be released. [#26823]

### Known Bugs

1. The STL interface does not work correctly on a Mac OS X platform.
2. The Solaris Studio 12.5 C++ compiler can fail to build Berkeley DB because of our support for built-in atomic functions. Falling back to Solaris Studio 12.4 should be a successful workaround.
3. Some Java tests ported from Berkeley DB JE, used to test the Direct Persistence Layer, fail when run in enhanced mode.
4. Asynchronous loggers cannot be used.