/*-
 * Copyright (c) 2002, 2020 Oracle and/or its affiliates.  All rights reserved.
 *
 * See the file EXAMPLES-LICENSE for license information.
 *
 * $Id$
 */

#ifndef _EX_APPREC_H_
#define	_EX_APPREC_H_

#include "ex_apprec_auto.h"

int ex_apprec_mkdir_print
    __P((DB_ENV *, DBT *, DB_LSN *, db_recops));
int ex_apprec_mkdir_recover
    __P((DB_ENV *, DBT *, DB_LSN *, db_recops));
int ex_apprec_init_print __P((DB_ENV *, DB_DISTAB *));

#endif /* !_EX_APPREC_H_ */
