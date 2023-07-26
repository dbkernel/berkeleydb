/*-
 * Copyright (c) 1996, 2020 Oracle and/or its affiliates.  All rights reserved.
 *
 * See the file LICENSE for license information.
 *
 * $Id: db_upgrade.c,v a79fcece62a1 2016/06/06 14:15:33 yong $
 */

#include "db_config.h"

#include "db_int.h"

#ifndef lint
static const char copyright[] =
    "Copyright (c) 1996, 2020 Oracle and/or its affiliates.  All rights reserved.\n";
#endif

int db_upgrade_main __P((int, char *[]));
void db_upgrade_usage __P((void));

const char *progname;

int
db_upgrade(args)
	char *args;
{
	int argc;
	char **argv;

	__db_util_arg("db_upgrade", args, &argc, &argv);
	return (db_upgrade_main(argc, argv) ? EXIT_FAILURE : EXIT_SUCCESS);
}

#include <stdio.h>
#define	ERROR_RETURN	ERROR

int
db_upgrade_main(argc, argv)
	int argc;
	char *argv[];
{
	extern char *optarg;
	extern int optind, __db_getopt_reset;
	DB *dbp, *dbvp;
	DB_ENV *dbenv;
	u_int32_t flags, vflag;
	int ch, exitval, nflag, ret, t_ret, verbose;
	char *home, *msgpfx, *passwd, *vopt;

	progname = __db_util_arg_progname(argv[0]);

	if ((ret = __db_util_version_check(progname)) != 0)
		return (ret);

	dbenv = NULL;
	flags = nflag = verbose = vflag = 0;
	exitval = EXIT_SUCCESS;
	home = msgpfx = passwd = vopt = NULL;
	__db_getopt_reset = 1;
	while ((ch = getopt(argc, argv, "h:m:NP:S:sVv")) != EOF)
		switch (ch) {
		case 'h':
			home = optarg;
			break;
		case 'm':
			msgpfx = optarg;
			break;
		case 'N':
			nflag = 1;
			break;
		case 'P':
			if (__db_util_arg_password(progname, 
			    optarg, &passwd) != 0)
				goto err;
			break;
		case 'S':
			vopt = optarg;
			switch (*vopt) {
			case 'o':
				vflag = DB_NOORDERCHK;
				break;
			case 'v':
				vflag = 0;
				break;
			default:
				(void)db_upgrade_usage();
				goto err;
			}
			break;
		case 's':
			LF_SET(DB_DUPSORT);
			break;
		case 'V':
			printf("%s\n", db_version(NULL, NULL, NULL));
			goto done;
		case 'v':
			verbose = 1;
			break;
		case '?':
		default:
			goto usage_err;
		}
	argc -= optind;
	argv += optind;

	if (argc <= 0)
		goto usage_err;

	/* Handle possible interruptions. */
	__db_util_siginit();

	if (__db_util_env_create(&dbenv, progname, passwd, msgpfx) != 0)
		goto err;

	if (nflag) {
		if ((ret = dbenv->set_flags(dbenv, DB_NOLOCKING, 1)) != 0) {
			dbenv->err(dbenv, ret, "set_flags: DB_NOLOCKING");
			goto err;
		}
		if ((ret = dbenv->set_flags(dbenv, DB_NOPANIC, 1)) != 0) {
			dbenv->err(dbenv, ret, "set_flags: DB_NOPANIC");
			goto err;
		}
	}

	if (__db_util_env_open(dbenv, home, 0, 1, DB_INIT_MPOOL, 0, NULL) != 0)
		goto err;

	if (vopt != NULL && (db_create(&dbvp, dbenv, 0) != 0
	    || dbvp->verify(dbvp, argv[0], NULL, stdout, vflag) != 0))
		goto err;

	for (; !__db_util_interrupted() && argv[0] != NULL; ++argv) {
		if ((ret = db_create(&dbp, dbenv, 0)) != 0) {
			fprintf(stderr,
			    "%s: db_create: %s\n", progname, db_strerror(ret));
			goto err;
		}
		dbp->set_errfile(dbp, stderr);
		dbp->set_errpfx(dbp, progname);
		if ((ret = dbp->upgrade(dbp, argv[0], flags)) != 0)
			dbp->err(dbp, ret, "DB->upgrade: %s", argv[0]);
		if ((t_ret = dbp->close(dbp, 0)) != 0 && ret == 0) {
			dbenv->err(dbenv, ret, "DB->close: %s", argv[0]);
			ret = t_ret;
		}
		if (ret != 0)
			goto err;
		/*
		 * People get concerned if they don't see a success message.
		 * If verbose is set, give them one.
		 */
		if (verbose)
			printf(DB_STR_A("5019",
			    "%s: %s upgraded successfully\n",
			    "%s %s\n"), progname, argv[0]);
	}

	if (0) {
usage_err:	db_upgrade_usage();
err:		exitval = EXIT_FAILURE;
	}
done:	if (dbenv != NULL && (ret = dbenv->close(dbenv, 0)) != 0) {
		exitval = EXIT_FAILURE;
		fprintf(stderr,
		    "%s: dbenv->close: %s\n", progname, db_strerror(ret));
	}

	if (passwd != NULL)
		free(passwd);

	/* Resend any caught signal. */
	__db_util_sigresend();

	return (exitval);
}

void
db_upgrade_usage()
{
	fprintf(stderr, "usage: %s %s\n", progname,
	    "[-NsVv] [-h home] [-m msg_pfx] [-P password] [-S vo] db_file ...");
}
