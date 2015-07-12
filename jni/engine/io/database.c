/*
 * database.c
 *
 *  Created on: 2015年6月18日
 *      Author: ryan
 */
#include "database.h"

#define DB_FILE			"dbFile"

#define DB_FLAG_START		"#"
#define DB_FLAG_END		"\n"
#define DB_FLAG_VALUE		"|"


typedef struct {
	const char *name;
	char filePath[256];
	void *hash;
} Database;

PUBLIC BOOL database_init() {
	return file_op_mkdir(DB_FILE);
}

PUBLIC void *database_createDB(const char *name) {
	Database *db = MALLOC(sizeof(Database));
	if (NULL == db) {
//		LOGE("database_create() malloc failed!!!");
		return NULL;
	}
	db->name = name;
	db->hash = hash_init(9);
	sprintf(db->filePath, "%s/%s", DB_FILE, name);
	unsigned int size;
	char *buf = file_op_readFile(db->filePath, &size);
	if (NULL != buf) {
		char *anchor = buf;
		while ((unsigned int)(anchor - (char *)buf) < size) {
			void *start = strstr(anchor, DB_FLAG_START);
			if (NULL == start) {
				break;
			}
			char *end = strstr(anchor, DB_FLAG_END);
			if (NULL == end) {
				end = buf + size - 1;
			}
			*end = '\0';
			anchor = start + 1;
			char *valueFlag = strstr(anchor, DB_FLAG_VALUE);
			if (NULL == valueFlag) {
				goto _NEXT;
			}
			*valueFlag = '\0';
			char *value = MALLOC(strlen(valueFlag + 1) + 1);
			strcpy(value, valueFlag + 1);
			hash_set(db->hash, (void *)anchor, strlen(anchor)+1, (unsigned int)value, strlen(value)+1);
			FREE(value);
			*valueFlag = DB_FLAG_VALUE[0];
			*end = DB_FLAG_END[0];
		_NEXT:
			anchor = end + 1;
		}
		FREE(buf);
	}
	return db;
}

PRIVATE BOOL _write_proc(const void *key, unsigned int key_len,
											unsigned int value1, unsigned int value2,
											void *param) {
	FILE *fp = (FILE *)param;
	char str[1024];
	sprintf(str, "%s%s%s%u%s", DB_FLAG_START, (char *)key, DB_FLAG_VALUE, value1, DB_FLAG_END);
	fwrite((char *)str, sizeof(unsigned char), strlen(str), fp);
	return TRUE;
}

PUBLIC void database_write(void *database) {
	char path[256];
	Database *db = (Database *)database;
	file_op_remove(db->filePath);
	sprintf(path, "%s/%s", file_op_getPath(), db->filePath);
	FILE *fp = fopen(path, "a");
	if (NULL != fp) {
		hash_enum(db->hash, _write_proc, fp);
		fclose(fp);
	}
	return;
}

PUBLIC void database_destroyDB(void *database) {
	Database *db = (Database *)database;
	if (NULL != db) {
		database_write(db);
		hash_destroy(db->hash);
		FREE(db);
	}
	return;
}

PUBLIC void database_set(void *database, const char *key, const char *value) {
	Database *db = (Database *)database;
	if (NULL != db) {
		if (hash_get(db->hash, key, strlen(key)+1, NULL, NULL)) {
			hash_unset(db->hash, key, strlen(key)+1);
		}
		hash_set(db->hash, (void *)key, strlen(key)+1, (unsigned int)value, strlen(value)+1);
	}
	return;
}

PUBLIC char *database_get(void *database, const char *key, unsigned int *size) {
	Database *db = (Database *)database;
	char *value = NULL;
	if (NULL != db) {
		unsigned int _size;
		hash_get(db->hash, key, strlen(key)+1, (unsigned int *)&value, &_size);
		if (NULL != size) *size = _size;
	}
	return value;
}
