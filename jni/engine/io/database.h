/*
 * database.h
 *
 *  Created on: 2015年6月18日
 *      Author: ryan
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include <stdio.h>
#include <string.h>
#include <base.h>
#include <file_op.h>
#include <hashmap/hash.h>

BOOL database_init();
void *database_createDB();
void database_destroyDB(void *database);
void database_write(void *database);
void database_set(void *database, const char *key, const char *value);
char *database_get(void *database, const char *key, unsigned int *size);

#endif /* DATABASE_H_ */
