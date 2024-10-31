# berkeleydb

```
Berkeley DB Release 18.1, library version 18.1.40: (May 29, 2020)

This is Berkeley DB Release 18.1.40 from Oracle.  To view release and
installation documentation, load the distribution file docs/index.html
into your web browser.  Technical support can be obtained by visiting 
the Berkeley DB Oracle Technical Network (OTN) forum, here:

https://forums.oracle.com/forums/forum.jspa?forumID=271
```

## 文档

### html 文档

源码中已包含html手册：[html 文档](docs/index.html)

### pdf 文档

另外，源码目录中包含绝大部分 PDF 文档：[pdf文档](./pdf) ，未包含的文档有：

**Programmatic APIs**

These fully-detailed references cover every public interface for Berkeley DB. For each interface, the documentation lists arguments and return values, and covers pre- and post-conditions.

- | C                                           | [PDF](api_reference/C/BDB-C_APIReference.pdf), [HTML](api_reference/C/frame_main.html)        |
  |-----------------------------------------------------------------------------------------------| ------------------------------------------------------------ |
  | C++                                         | [PDF](api_reference/CXX/BDB-CXX_APIReference.pdf), [HTML](api_reference/CXX/frame_main.html)  |
  | C#                                          | [HTML](csharp/Index.html)                                                                     |
  | STL                                         | [PDF](api_reference/STL/BDB-STL_APIReference.pdf) , [HTML](api_reference/STL/frame_main.html) |
  | Java API for Transactional Java Collections | [HTML](java/index.html)                                                                       |
  | TCL                                         | [PDF](api_reference/TCL/BDB-TCL_APIReference.pdf), [HTML](api_reference/TCL/frame_main.html)  |

> **PS**：只有 HTML 却无 PDF 的即为缺失的文档。

**Additional Booklets**

- | Writing In-Memory Berkeley DB Applications             | [PDF](articles/inmemory/C/InMemoryDBApplication.pdf), [HTML](articles/inmemory/C/index.html)                  |
  |---------------------------------------------------------------------------------------------------------------| ------------------------------------------------------------ |
  | Error Message Reference Guide                          | [PDF](articles/mssgtxt/DBMessageTextReference.pdf), [HTML](articles/mssgtxt/index.html)                       |
  | Creating a Transactional Environment                   | [HTML](http://www.oracle.com/webfolder/technetwork/tutorials/obe/bdb/create/get_started_bdb.html)             |
  | Verification and Backup of a Transactional Environment | [HTML](http://www.oracle.com/webfolder/technetwork/tutorials/obe/bdb/verify/verify_bdb.html)                  |
  | Berkeley DB User Interface Tour                        | [HTML](https://www.oracle.com/webfolder/technetwork/tutorials/tutorial/db/BerkleyDB/InterfaceTour/index.html) |

### 文档说明

BDB提供里非常详细的文档，可以官方网站获得html或pdf版本的文档。这里简单说明一下pdf版本的一些文档：

- BDB_Installation.pdf：安装文档，涵盖了不同操作系统，不同的编译工具，不同编程语言等多方面的详细信息；
- BDB_Prog_Reference.pdf：编程参考手册，面向开发人员，主要从BDB的各种功能和机制原理进行阐述，供使用BDB作为存储引擎来编写程序的各类程序员（C、Java、C#、Perl）阅读；
- BDB-Porting-Guide.pdf：迁移手册，帮助开发人员将应用程序从旧版本的 BerkeleyDB 数据库迁移到新版本；
- InMemoryDBApplication.pdf： 基于内存的BDB应用的相关知识；
- BDB-C_APIReference.pdf： C API参考手册，跟 BDB_Prog_Reference.pdf 结合使用；
- BerkeleyDB-Core-C-GSG.pdf： 为C语言开发人员提供的BDB入门手册；
- BerkeleyDB-Core-C-Txn.pdf： 为C语言开发人员提供的BDB事务方面的手册；
- Replication-C-GSG.pdf： 为C语言开发人员提供的BDB复制方面的手册。

## changelog

参考 [changelog](./changelog.md)

## 代码结构

从系统结构角度划分，可将其分为几个子系统：

- 存储管理子系统 (Storage Subsystem)
- 内存池管理子系统 (Memory Pool Subsystem)
- 事务子系统 (Transaction Subsystem)
- 锁子系统 (Locking Subsystem)
- 日志子系统 (Logging Subsystem)

以上的五个子系统完成了BDB作为一个Database所需要的大部分功能，如何驾驭以上子系统来完成我们需要的任务是关键。像MySQL这种独立的RDBMS，我们可以通过配置和SQL语句来控制和使用它的各种功能，
但BDB是一个嵌入式的数据库，最终还是需要程序员通过调用API来完成。所以要使用好BDB，需要先了解其原理，然后在合适的位置上调用合适的API。

## 使用说明

### 示例

写一个BDB程序的一般步骤：

- a. 创建、设置和打开Environment；
- b. 创建、设置和打开Database；
- c. 访问Database；
- d. 关闭Database；
- e. 关闭Environment。

此处的Database是从属于Environment，即db是在env这个环境上面建立起来的。为了便于快速把握重点，可以用BDB跟一般的RDBMS做个类比，这里的Database相当于数据表，Environment相当于数据库。

```cpp
    DB_ENV *dbenv;
    DB *dbp;
    int ret;

    if ((ret = db_env_create(&dbenv, 0)) != 0) {
            fprintf(errfp, "%s: %s\n", progname, db_strerror(ret));
            return (1);
    }

    dbenv->set_errfile(dbenv, errfp);
    dbenv->set_errpfx(dbenv, progname);
    if ((ret = dbenv->set_cachesize(dbenv, 0, 64 * 1024, 0)) != 0) {
            dbenv->err(dbenv, ret, "set_cachesize");
            dbenv->close(dbenv, 0);
            return (1);
    }

    (void)dbenv->set_data_dir(dbenv, data_dir);
    if ((ret = dbenv->open(dbenv, home, DB_CREATE | DB_INIT_LOCK | DB_INIT_LOG | DB_INIT_MPOOL | DB_INIT_TXN, 0644)) != 0) {
            dbenv->err(dbenv, ret, "environment open: %s", home);
            dbenv->close(dbenv, 0);
            return (1);
    }

    if ((ret = db_create(&dbp, dbenv, 0)) != 0){
            fprintf(errfp, "%s: %s\n", progname, db_strerror(ret));
            return (1);
    }

    if ((ret = dbp->open(dbp, NULL, "exenv_db1.db", NULL, DB_BTREE, DB_CREATE,0644)) != 0){
            fprintf(stderr, "database open: %s\n", db_strerror(ret));
            return (1);
    }

    if ((ret = dbp->close(dbp, 0)) != 0) {
            fprintf(stderr, "database close: %s\n", db_strerror(ret));
            return (1);
    }

    if ((ret = dbenv->close(dbenv, 0)) != 0) {
            fprintf(stderr, "DB_ENV->close: %s\n", db_strerror(ret));
            return (1);
    }

    return (0);
```

### 数据文件

一个BDB的实例会产生数据存储文件，数据文件的目录由 `dbenv->set_data_dir(dbenv, data_dir);` 这条语句来指定。涉及的文件类型有：Data Files，Log Files，Region Files，Temporary Files。

- Data Files：数据文件，存储实际的数据；
- Log Files：日志文件；
- Region Files：是各个子系统保存信息的文件，如果在Env中设置了`DB_PRIVATE`选项，这些信息是被一个进程私有，即它们保存在内存中，这些文件在此种情况下不产生；
- Temporary Files： 临时文件，特殊情况会被创建。

### 数据的存数格式

Berkeley DB提供了以下四种文件存储方法：哈希文件、B树、定长记录（队列）和变长记录（基于记录号的简单存储方式），应用程序可以从中选择最适合的文件组织结构。
以上代码通过`db->open`函数中设置了`DB_BTREE`这个选项指定其使用B树方式存储。其它的三种存储格式对应的类型为：`DB_HASH`，`DB_QUEUE`，`DB_RECNO`。

### 事务提交

BDB中的事务提交有两种方式：`DB_AUTO_COMMIT` 和显式提交事务。

- 如果设置为 `DB_AUTO_COMMIT`，则每步操作多作为单独的事务自动提交；
- 如果需要显示提交，则需要显示调用具体事务相关的begin/end API（相见文档 BerkeleyDB-Core-C-Txn.pdf）。

BDB在事务提交时也是遵循先写日志并刷新到磁盘的方式，但是为了提高性能，其又引入了两个选项：`DB_TXN_NOSYNC` 和 `DB_TXN_WRITE_NOSYNC`。
`DB_TXN_NOSYNC`的作用是使BDB在事务提交的时候不严格要求日志到磁盘，刷新与否取决于日志缓冲；`DB_TXN_WRITE_NOSYNC`会比DB_TXN_NOSYNC稍显严格，其含义是要求事务提交刷新日志，但只是刷到操作系统文件缓存当中。

BDB的事务隔离性级别有三个：READ UNCOMMITED、READ COMMITED、SERIALIZABLE

### CheckPoint

执行一个检查点会完成的工作有：

> Flushes all dirty pages from the in-memory cache to database files；Writes a checkpoint record；
>
> Flushes the log to log files；Writes a list of open databases.

调用API `DB_ENV->txn_checkpoint();` 即可完成，如果是非`DB_PRIVATE`的Env，也可以使用BDB自带的工具db_checkpoint。为了避免出现一个检查点提交大量数据的情况，BDB还提供了轻量级刷新脏页的API：`DB_ENV->memp_trickle();`

### Replication

BDB中提供了两种方式来支持复制技术：Replication Base API 和 Replication Manager

- Replication Base API 是最基础的API，实现方式灵活，功能强大，但是编码量大。
- Replication Manager 相当于框架，使用方式简单，编码量小。
- Replication Manager 可能能够满足大部分用户的需求，但不是所有需求，所以灵活性不足。如果您的需求是 Replication Manager 不能满足的，请使用 Replication Base API 自己实现复制策略。
- Replication Manager 的主从策略有两种：**指定主从、自动推举主从**。

### 分区

BDB的分区机制是从 db-4.8.x 之后引入的新功能，涉及到的API有两个：

`DB->set_partition()` 设置分区方式，包含了一个分区方式的回调函数，用户可以通过编写代码来自己实现分区方式，非常灵活。（详见 API 手册 BDB-C_APIReference.pdf）

`DB->set_partition_dirs()` 设置分区目录。（详见 API 手册 BDB-C_APIReference.pdf）

### 备份

BDB有三种备份方式：

- Offline Backups：离线备份，停服务拷贝数据目录；
- Hot Backups：使用API或者BDB自带工具db_backup在DB使用的情况下做备份；
- Incremental Backups：增量备份；

具体细节详见 BerkeleyDB-Core-C-Txn.pdf。
