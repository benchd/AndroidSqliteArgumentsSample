//#include <jni.h>
//#include <string>
//
//extern "C" JNIEXPORT jstring JNICALL
//Java_com_development_myprohelper_ui_WelcomeActivity_stringFromJNI(
//        JNIEnv* env,
//        jobject /* this */) {
//    std::string hello = "Hello from C++";
//    return env->NewStringUTF(hello.c_str());
//}
/*
** 2016-06-07
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
**
** This is a utility program that computes an SHA1 hash on the content
** of an SQLite database.
**
** The hash is computed over just the content of the database.  Free
** space inside of the database file, and alternative on-disk representations
** of the same content (ex: UTF8 vs UTF16) do not affect the hash.  So,
** for example, the database file page size, encoding, and auto_vacuum setting
** can all be changed without changing the hash.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
//#include "sqlite3.h"
#include "sqlite3.h"

#include <unistd.h>
#include <sys/stat.h>
#include <jni.h>
#include <android/log.h>
#include <stdbool.h>
#include <jni.h>
#include <jni.h>

#define  LOG_TAG    "called"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,"%s" , __VA_ARGS__)
//#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
/* Context for the SHA1 hash */
typedef struct SHA1Context SHA1Context;
struct SHA1Context {
    unsigned int state[5];
    unsigned int count[2];
    unsigned char buffer[64];
};

/*
** All global variables are gathered into the "g" singleton.
*/
struct GlobalVars {
    const char *zArgv0;       /* Name of program */
    unsigned fDebug;          /* Debug flags */
    sqlite3 *db;              /* The database connection */
    SHA1Context cx;           /* SHA1 hash context */
} g;

/*
** Debugging flags
*/
#define DEBUG_FULLTRACE   0x00000001   /* Trace hash to stderr */

/******************************************************************************
** The Hash Engine
**
** Modify these routines (and appropriate state fields in global variable 'g')
** in order to compute a different (better?) hash of the database.
*/
/*
 * blk0() and blk() perform the initial expand.
 * I got the idea of expanding during the round function from SSLeay
 *
 * blk0le() for little-endian and blk0be() for big-endian.
 */
#define SHA_ROT(x,l,r) ((x) << (l) | (x) >> (r))
#define rol(x,k) SHA_ROT(x,k,32-(k))
#define ror(x,k) SHA_ROT(x,32-(k),k)

#define blk0le(i) (block[i] = (ror(block[i],8)&0xFF00FF00) \
    |(rol(block[i],8)&0x00FF00FF))
#define blk0be(i) block[i]
#define blk(i) (block[i&15] = rol(block[(i+13)&15]^block[(i+8)&15] \
    ^block[(i+2)&15]^block[i&15],1))

/*
 * (R0+R1), R2, R3, R4 are the different operations (rounds) used in SHA1
 *
 * Rl0() for little-endian and Rb0() for big-endian.  Endianness is
 * determined at run-time.
 */
#define Rl0(v,w,x,y,z,i) \
    z+=((w&(x^y))^y)+blk0le(i)+0x5A827999+rol(v,5);w=ror(w,2);
#define Rb0(v,w,x,y,z,i) \
    z+=((w&(x^y))^y)+blk0be(i)+0x5A827999+rol(v,5);w=ror(w,2);
#define R1(v,w,x,y,z,i) \
    z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5);w=ror(w,2);
#define R2(v,w,x,y,z,i) \
    z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);w=ror(w,2);
#define R3(v,w,x,y,z,i) \
    z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5);w=ror(w,2);
#define R4(v,w,x,y,z,i) \
    z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);w=ror(w,2);

/*
 * Hash a single 512-bit block. This is the core of the algorithm.
 */
#define a qq[0]
#define b qq[1]
#define c qq[2]
#define d qq[3]
#define e qq[4]

int main1(int argc, char **argv);

int file_exists (char *filename) {
    struct stat   buffer;
    return (stat (filename, &buffer) == 0);
}

void david(char *docsDir)
{
//    LOGD("david world");
    char *FileName = "/299.db";

    char *buffer = malloc(strlen(FileName) + strlen(docsDir) + 10);

    strcpy(buffer, docsDir);
    strcat(buffer, FileName);

    if (file_exists(buffer))
    {
        printf("docs dir: %s \n", buffer);
        LOGD("docs dir: %s \n", buffer);
    }
    else
    {
        printf("cant find %s\n", buffer);
        LOGD("cant find %s\n", buffer);
        return;

    }

    char *arrop[3];
    arrop[0] = "dummyname";
    arrop[1] = buffer;

    main1(2, (char **) &arrop);

    free(buffer);

}
void SHA1Transform(unsigned int state[5], const unsigned char buffer[64]){
    unsigned int qq[5]; /* a, b, c, d, e; */
    static int one = 1;
    unsigned int block[16];
    memcpy(block, buffer, 64);
    memcpy(qq,state,5*sizeof(unsigned int));

    /* Copy g.cx.state[] to working vars */
    /*
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
    */

    /* 4 rounds of 20 operations each. Loop unrolled. */
    if( 1 == *(unsigned char*)&one ){
        Rl0(a,b,c,d,e, 0); Rl0(e,a,b,c,d, 1); Rl0(d,e,a,b,c, 2); Rl0(c,d,e,a,b, 3);
        Rl0(b,c,d,e,a, 4); Rl0(a,b,c,d,e, 5); Rl0(e,a,b,c,d, 6); Rl0(d,e,a,b,c, 7);
        Rl0(c,d,e,a,b, 8); Rl0(b,c,d,e,a, 9); Rl0(a,b,c,d,e,10); Rl0(e,a,b,c,d,11);
        Rl0(d,e,a,b,c,12); Rl0(c,d,e,a,b,13); Rl0(b,c,d,e,a,14); Rl0(a,b,c,d,e,15);
    }else{
        Rb0(a,b,c,d,e, 0); Rb0(e,a,b,c,d, 1); Rb0(d,e,a,b,c, 2); Rb0(c,d,e,a,b, 3);
        Rb0(b,c,d,e,a, 4); Rb0(a,b,c,d,e, 5); Rb0(e,a,b,c,d, 6); Rb0(d,e,a,b,c, 7);
        Rb0(c,d,e,a,b, 8); Rb0(b,c,d,e,a, 9); Rb0(a,b,c,d,e,10); Rb0(e,a,b,c,d,11);
        Rb0(d,e,a,b,c,12); Rb0(c,d,e,a,b,13); Rb0(b,c,d,e,a,14); Rb0(a,b,c,d,e,15);
    }
    R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
    R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
    R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
    R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
    R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
    R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
    R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
    R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
    R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
    R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
    R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);
    R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
    R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
    R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
    R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
    R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);

    /* Add the working vars back into context.state[] */
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
}


/* Initialize the SHA1 hash */
static void hash_init(void){
    /* SHA1 initialization constants */
    g.cx.state[0] = 0x67452301;
    g.cx.state[1] = 0xEFCDAB89;
    g.cx.state[2] = 0x98BADCFE;
    g.cx.state[3] = 0x10325476;
    g.cx.state[4] = 0xC3D2E1F0;
    g.cx.count[0] = g.cx.count[1] = 0;
}

/* Add new content to the SHA1 hash */
static void hash_step(const unsigned char *data,  unsigned int len){
    unsigned int i, j;

    j = g.cx.count[0];
    if( (g.cx.count[0] += len << 3) < j ){
        g.cx.count[1] += (len>>29)+1;
    }
    j = (j >> 3) & 63;
    if( (j + len) > 63 ){
        (void)memcpy(&g.cx.buffer[j], data, (i = 64-j));
        SHA1Transform(g.cx.state, g.cx.buffer);
        for(; i + 63 < len; i += 64){
            SHA1Transform(g.cx.state, &data[i]);
        }
        j = 0;
    }else{
        i = 0;
    }
    (void)memcpy(&g.cx.buffer[j], &data[i], len - i);
}


/* Add padding and compute and output the message digest. */
static char * hash_finish1(const char *zName){
    unsigned int i;
    unsigned char finalcount[8];
    unsigned char digest[20];
    static const char zEncode[] = "0123456789abcdef";
    char zOut[41];

    for (i = 0; i < 8; i++){
        finalcount[i] = (unsigned char)((g.cx.count[(i >= 4 ? 0 : 1)]
                >> ((3-(i & 3)) * 8) ) & 255); /* Endian independent */
    }
    hash_step((const unsigned char *)"\200", 1);
    while ((g.cx.count[0] & 504) != 448){
        hash_step((const unsigned char *)"\0", 1);
    }
    hash_step(finalcount, 8);  /* Should cause a SHA1Transform() */
    for (i = 0; i < 20; i++){
        digest[i] = (unsigned char)((g.cx.state[i>>2] >> ((3-(i & 3)) * 8) ) & 255);
    }
    for(i=0; i<20; i++){
        zOut[i*2] = zEncode[(digest[i]>>4)&0xf];
        zOut[i*2+1] = zEncode[digest[i] & 0xf];
    }
    zOut[i*2]= 0;
    return strdup(zOut);
    /* printf("%s %s\n", zOut, zName); */
}



/* Add padding and compute and output the message digest. */
static void hash_finish(const char *zName){
    unsigned int i;
    unsigned char finalcount[8];
    unsigned char digest[20];
    static const char zEncode[] = "0123456789abcdef";
    char zOut[41];

    for (i = 0; i < 8; i++){
        finalcount[i] = (unsigned char)((g.cx.count[(i >= 4 ? 0 : 1)]
                >> ((3-(i & 3)) * 8) ) & 255); /* Endian independent */
    }
    hash_step((const unsigned char *)"\200", 1);
    while ((g.cx.count[0] & 504) != 448){
        hash_step((const unsigned char *)"\0", 1);
    }
    hash_step(finalcount, 8);  /* Should cause a SHA1Transform() */
    for (i = 0; i < 20; i++){
        digest[i] = (unsigned char)((g.cx.state[i>>2] >> ((3-(i & 3)) * 8) ) & 255);
    }
    for(i=0; i<20; i++){
        zOut[i*2] = zEncode[(digest[i]>>4)&0xf];
        zOut[i*2+1] = zEncode[digest[i] & 0xf];
    }
    zOut[i*2]= 0;
    printf("%s %s\n", zOut, zName);
}


/* End of the hashing logic
*******************************************************************************/

/*
** Print an error resulting from faulting command-line arguments and
** abort the program.
*/
static void cmdlineError(const char *zFormat, ...){
    va_list ap;
    fprintf(stderr, "%s: ", g.zArgv0);
    va_start(ap, zFormat);
    vfprintf(stderr, zFormat, ap);
    va_end(ap);
    fprintf(stderr, "\n\"%s --help\" for more help\n", g.zArgv0);
    exit(1);
}

/*
** Print an error message for an error that occurs at runtime, then
** abort the program.
*/
static void runtimeError(const char *zFormat, ...){
    va_list ap;
    fprintf(stderr, "%s: ", g.zArgv0);
    va_start(ap, zFormat);
    vfprintf(stderr, zFormat, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    exit(1);
}

/*
** Prepare a new SQL statement.  Print an error and abort if anything
** goes wrong.
*/
static sqlite3_stmt *db_vprepare(const char *zFormat, va_list ap){
    char *zSql;
    int rc;
    sqlite3_stmt *pStmt;

    zSql = sqlite3_vmprintf(zFormat, ap);
    if( zSql==0 ) runtimeError("out of memory");
    rc = sqlite3_prepare_v2(g.db, zSql, -1, &pStmt, 0);
    if( rc ){
        runtimeError("SQL statement error: %s\n\"%s\"", sqlite3_errmsg(g.db),
                     zSql);
    }
    sqlite3_free(zSql);
    return pStmt;
}
static sqlite3_stmt *db_prepare(const char *zFormat, ...){
    va_list ap;
    sqlite3_stmt *pStmt;
    va_start(ap, zFormat);
    pStmt = db_vprepare(zFormat, ap);
    va_end(ap);
    return pStmt;
}

/*
** Compute the hash for all rows of the query formed from the printf-style
** zFormat and its argument.
*/
static void hash_one_query(const char *zFormat, ...){
    va_list ap;
    sqlite3_stmt *pStmt;        /* The query defined by zFormat and "..." */
    int nCol;                   /* Number of columns in the result set */
    int i;                      /* Loop counter */

    /* Prepare the query defined by zFormat and "..." */
    va_start(ap, zFormat);
    pStmt = db_vprepare(zFormat, ap);
    va_end(ap);
    nCol = sqlite3_column_count(pStmt);

    /* Compute a hash over the result of the query */
    while( SQLITE_ROW==sqlite3_step(pStmt) ){
        for(i=0; i<nCol; i++){
            switch( sqlite3_column_type(pStmt,i) ){
                case SQLITE_NULL: {
                    hash_step((const unsigned char*)"0",1);
                    if( g.fDebug & DEBUG_FULLTRACE ) fprintf(stderr, "NULL\n");
                    break;
                }
                case SQLITE_INTEGER: {
                    sqlite3_uint64 u;
                    int j;
                    unsigned char x[8];
                    sqlite3_int64 v = sqlite3_column_int64(pStmt,i);
                    memcpy(&u, &v, 8);
                    for(j=7; j>=0; j--){
                        x[j] = u & 0xff;
                        u >>= 8;
                    }
                    hash_step((const unsigned char*)"1",1);
                    hash_step(x,8);
                    if( g.fDebug & DEBUG_FULLTRACE ){
                        fprintf(stderr, "INT %s\n", sqlite3_column_text(pStmt,i));
                    }
                    break;
                }
                case SQLITE_FLOAT: {
                    sqlite3_uint64 u;
                    int j;
                    unsigned char x[8];
                    double r = sqlite3_column_double(pStmt,i);
                    memcpy(&u, &r, 8);
                    for(j=7; j>=0; j--){
                        x[j] = u & 0xff;
                        u >>= 8;
                    }
                    hash_step((const unsigned char*)"2",1);
                    hash_step(x,8);
                    if( g.fDebug & DEBUG_FULLTRACE ){
                        fprintf(stderr, "FLOAT %s\n", sqlite3_column_text(pStmt,i));
                    }
                    break;
                }
                case SQLITE_TEXT: {
                    int n = sqlite3_column_bytes(pStmt, i);
                    const unsigned char *z = sqlite3_column_text(pStmt, i);
                    hash_step((const unsigned char*)"3", 1);
                    hash_step(z, n);
                    if( g.fDebug & DEBUG_FULLTRACE ){
                        fprintf(stderr, "TEXT '%s'\n", sqlite3_column_text(pStmt,i));
                    }
                    break;
                }
                case SQLITE_BLOB: {
                    int n = sqlite3_column_bytes(pStmt, i);
                    const unsigned char *z = sqlite3_column_blob(pStmt, i);
                    hash_step((const unsigned char*)"4", 1);
                    hash_step(z, n);
                    if( g.fDebug & DEBUG_FULLTRACE ){
                        fprintf(stderr, "BLOB (%d bytes)\n", n);
                    }
                    break;
                }
            }
        }
    }
    sqlite3_finalize(pStmt);
}


/*
** Print sketchy documentation for this utility program
*/
static void showHelp(void){
    printf("Usage: %s [options] FILE ...\n", g.zArgv0);
    printf(
            "Compute a SHA1 hash on the content of database FILE.  System tables such as\n"
            "sqlite_stat1, sqlite_stat4, and sqlite_sequence are omitted from the hash.\n"
            "Options:\n"
            "   --debug N           Set debugging flags to N (experts only)\n"
            "   --like PATTERN      Only hash tables whose name is LIKE the pattern\n"
            "   --schema-only       Only hash the schema - omit table content\n"
            "   --without-schema    Only hash table content - omit the schema\n"
    );
}



//extern  JNIEXPORT int JNICALL
// Java_com_development_myprohelper_ui_WelcomeActivity_stringFromJNI(
//        JNIEnv* env, jobject obj, jint depth) {
//    printf("Global variable:");
//    return 10;
////    jstring jstr = (*env)->NewStringUTF(env, "This comes from jni.");
////    return (*env)->NewStringUTF(env, jstr);
////    return (*env)->NewStringUTF(jstr);
//}


extern  JNIEXPORT int JNICALL
//hashme(const char *fileName, char **try1)
hashme(const char *fileName, char **try1)
{
//    *try1 = strdup("Hello world!");
    int IgnoreVersionsTable = 1;


    const char *zDb = fileName;         /* Name of the database currently being hashed */
    int rc;                      /* Subroutine return code */
    char *zErrMsg;               /* Error message when opening database */
    const char *zLike = 0;       /* LIKE pattern of tables to hash */
    sqlite3_stmt *pStmt;         /* An SQLite query */
    int omitSchema = 0;          /* True to compute hash on content only */

    g.zArgv0 = "hash";
    sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    static const int openFlags =
            SQLITE_OPEN_READWRITE |     /* Read/write so hot journals can recover */
            SQLITE_OPEN_URI
    ;

    if( zLike==0 ) zLike = "%";

    rc = sqlite3_open_v2(zDb, &g.db, openFlags, 0);
    if( rc )
    {

        LOGD("cannot open database file '%s'\n", zDb);
        fprintf(stderr, "cannot open database file '%s'\n", zDb);
        free((void*)fileName);
        return 101;
    }

    rc = sqlite3_exec(g.db, "SELECT * FROM sqlite_schema", 0, 0, &zErrMsg);
    if( rc || zErrMsg ){
        sqlite3_close(g.db);
        g.db = 0;
        fprintf(stderr, "'%s' is not a valid SQLite database\n", zDb);
        free((void*)fileName);
        LOGD("is not a valid SQLite database");
        return 102;
    }

    /* Start the hash */
    hash_init();

    /* Hash table content */

    if (IgnoreVersionsTable)
    {
        pStmt = db_prepare(
                "SELECT name FROM sqlite_schema\n"
                " WHERE type='table' AND sql NOT LIKE 'CREATE VIRTUAL%%'\n"
                "   AND name NOT LIKE 'sqlite_%%'\n"
                "   AND name NOT LIKE 'Versions'\n"
                "   AND name NOT LIKE 'AuditTrail'\n"
                "   AND name LIKE '%q'\n"
                " ORDER BY name COLLATE nocase;\n",
                zLike
        );
        LOGD("SELECT name SQLite database 1");
    }
    else
    {
        pStmt = db_prepare(
                "SELECT name FROM sqlite_schema\n"
                " WHERE type='table' AND sql NOT LIKE 'CREATE VIRTUAL%%'\n"
                "   AND name NOT LIKE 'sqlite_%%'\n"
                "   AND name LIKE '%q'\n"
                " ORDER BY name COLLATE nocase;\n",
                zLike
        );
        LOGD("SELECT name  valid FROM sqlite_schema eslse");
    }

    while( SQLITE_ROW==sqlite3_step(pStmt) ){
        /* We want rows of the table to be hashed in PRIMARY KEY order.
        ** Technically, an ORDER BY clause is required to guarantee that
        ** order.  However, though not guaranteed by the documentation, every
        ** historical version of SQLite has always output rows in PRIMARY KEY
        ** order when there is no WHERE or GROUP BY clause, so the ORDER BY
        ** can be safely omitted. */
        hash_one_query("SELECT * FROM \"%w\"", sqlite3_column_text(pStmt,0));
    }
    sqlite3_finalize(pStmt);


    /* Hash the database schema */
    if( !omitSchema ){
        hash_one_query(
                "SELECT type, name, tbl_name, sql FROM sqlite_schema\n"
                " WHERE tbl_name LIKE '%q'\n"
                " ORDER BY name COLLATE nocase;\n",
                zLike
        );
    }

    /* Finish and output the hash and close the database connection. */
/*     hash_finish(zDb); */

    *try1 = hash_finish1(zDb);
    sqlite3_close(g.db);

    free((void*)fileName);
    LOGD(*try1);
    return 0;
}

extern  JNIEXPORT jstring JNICALL
Java_com_development_myprohelper_ui_WelcomeActivity_stringFromJNI(
        JNIEnv* env , jobject obj, jstring strx, jstring path ) {
    printf("Global variable:");
    // Get the parameters passed by java

//    jstring jstr = (*env)->NewStringUTF(env, strx);
//    jclass clazz = (*env)->FindClass(env, "com/development/myprohelper/ui/WelcomeActivity");
//    jmethodID stringFromJNI = (*env)->GetMethodID(env, clazz, "stringFromJNI", "(Ljava/lang/String;)Ljava/lang/String;");
//    jobject result = (*env)->CallObjectMethod(env, obj, stringFromJNI, jstr);

//    const char* str = (*env)->GetStringUTFChars(env,(jstring) result, &isCopy); // should be released but what a heck, it's a tutorial :)
//    printf("%s\n", str);
//    LOGD(str);
//    return (*env)->NewStringUTF(env, str);

//    jstring strx = (*env)->NewStringUTF(env, name);
//     char *nativeString = (*env)->NewStringUTF(env ,name);
    // use your string

//    (*env)->ReleaseStringUTFChars(name, nativeString);
//    LOGD(nativeString);
//    const char filename = 'x';
    jboolean isCopy;
    jboolean isPathCopy;
    const char *strCont = (*env)->GetStringUTFChars(env, strx,&isCopy);
    const char *strpathCont = (*env)->GetStringUTFChars(env, path,&isPathCopy);
    LOGD(strpathCont);
    char *FileName;
//    char *FileName = "/299.db";
//    char *FxileName = "dummyname";
    FileName = (char *) malloc(55);
//    strcpy(FileName, "/299.db");
    strcpy(FileName, strpathCont);
//    strcpy(FileName, "/data/user/0/com.development.myprohelper/databases/299.db");
//    char *buffer = malloc(strlen(FileName) + strlen(docsDir) + 10);
//    strcpy(buffer, docsDir);
//    strcat(buffer, FileName);
    int rows = 3;
    char **cppp = calloc (rows,sizeof(char*));
    cppp[0] = (char *)strCont;
    LOGD( cppp[0]);
//    cppp[0] = "cat";
//    cppp[1] = "dog";
//    cppp[2] = "mouse";
//    hashme((const char *) filename, (char **) arrop );
//    return hashme(&filename ,(char **) &arrop );
    hashme(FileName, cppp );
//    david((char *) filename);
//    jstring jstr = (*env)->NewStringUTF(env, "This comes from jni.");
//    return (*env)->NewStringUTF(env, jstr);
//    return (*env)->NewStringUTF(jstr);
//    LOGD(cppp[0]);
    jstring strp = (*env)->NewStringUTF(env, cppp[0]);
//    jstring str = env->NewStringUTF(cppp[0]);
    return strp;
//    return 10;
}
int main1(int argc, char **argv){
    LOGD("main1 world");
    const char *zDb = 0;         /* Name of the database currently being hashed */
    int i;                       /* Loop counter */
    int rc;                      /* Subroutine return code */
    char *zErrMsg;               /* Error message when opening database */
    sqlite3_stmt *pStmt;         /* An SQLite query */
    const char *zLike = 0;       /* LIKE pattern of tables to hash */
    int omitSchema = 0;          /* True to compute hash on content only */
    int omitContent = 0;         /* True to compute hash on schema only */
    int nFile = 0;               /* Number of input filenames seen */

    g.zArgv0 = argv[0];
    sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    for(i=1; i<argc; i++){
        const char *z = argv[i];
        if( z[0]=='-' ){
            z++;
            if( z[0]=='-' ) z++;
            if( strcmp(z,"debug")==0 ){
                if( i==argc-1 ) cmdlineError("missing argument to %s", argv[i]);
                g.fDebug = strtol(argv[++i], 0, 0);
            }else
            if( strcmp(z,"help")==0 ){
                showHelp();
                return 0;
            }else
            if( strcmp(z,"like")==0 ){
                if( i==argc-1 ) cmdlineError("missing argument to %s", argv[i]);
                if( zLike!=0 ) cmdlineError("only one --like allowed");
                zLike = argv[++i];
            }else
            if( strcmp(z,"schema-only")==0 ){
                omitContent = 1;
            }else
            if( strcmp(z,"without-schema")==0 ){
                omitSchema = 1;
            }else
            {
                cmdlineError("unknown option: %s", argv[i]);
            }
        }else{
            nFile++;
            if( nFile<i ) argv[nFile] = argv[i];
        }
    }
    if( nFile==0 ){
        cmdlineError("no input files specified - nothing to do");
    }
    if( omitSchema && omitContent ){
        cmdlineError("only one of --without-schema and --omit-schema allowed");
    }
    if( zLike==0 ) zLike = "%";

    for(i=1; i<=nFile; i++){
        static const int openFlags =
                SQLITE_OPEN_READWRITE |     /* Read/write so hot journals can recover */
                SQLITE_OPEN_URI
        ;
        zDb = argv[i];
        rc = sqlite3_open_v2(zDb, &g.db, openFlags, 0);
        if( rc ){
            fprintf(stderr, "cannot open database file '%s'\n", zDb);
            continue;
        }
        rc = sqlite3_exec(g.db, "SELECT * FROM sqlite_schema", 0, 0, &zErrMsg);
        if( rc || zErrMsg ){
            sqlite3_close(g.db);
            g.db = 0;
            fprintf(stderr, "'%s' is not a valid SQLite database\n", zDb);
            continue;
        }

        /* Start the hash */
        hash_init();

        /* Hash table content */
        if( !omitContent ){
            pStmt = db_prepare(
                    "SELECT name FROM sqlite_schema\n"
                    " WHERE type='table' AND sql NOT LIKE 'CREATE VIRTUAL%%'\n"
                    "   AND name NOT LIKE 'sqlite_%%'\n"
                    "   AND name LIKE '%q'\n"
                    " ORDER BY name COLLATE nocase;\n",
                    zLike
            );
            while( SQLITE_ROW==sqlite3_step(pStmt) ){
                /* We want rows of the table to be hashed in PRIMARY KEY order.
                ** Technically, an ORDER BY clause is required to guarantee that
                ** order.  However, though not guaranteed by the documentation, every
                ** historical version of SQLite has always output rows in PRIMARY KEY
                ** order when there is no WHERE or GROUP BY clause, so the ORDER BY
                ** can be safely omitted. */
                hash_one_query("SELECT * FROM \"%w\"", sqlite3_column_text(pStmt,0));
            }
            sqlite3_finalize(pStmt);
        }

        /* Hash the database schema */
        if( !omitSchema ){
            hash_one_query(
                    "SELECT type, name, tbl_name, sql FROM sqlite_schema\n"
                    " WHERE tbl_name LIKE '%q'\n"
                    " ORDER BY name COLLATE nocase;\n",
                    zLike
            );
        }

        /* Finish and output the hash and close the database connection. */
        hash_finish(zDb);
        sqlite3_close(g.db);
    }
    return 0;
}






extern  JNIEXPORT jstring JNICALL Java_com_androidsqliteargumentssample_db_NativeStore_stringFromJNI(JNIEnv *env, jobject thiz, jstring name,
                                                        jstring path) {
    printf("Global variable:");
    jboolean isCopy;
    jboolean isPathCopy;
    const char *strCont = (*env)->GetStringUTFChars(env, name,&isCopy);
    const char *strpathCont = (*env)->GetStringUTFChars(env, path,&isPathCopy);
    LOGD(strpathCont);
    char *FileName;
    FileName = (char *) malloc(55);
    strcpy(FileName, strpathCont);
    int rows = 3;
    char **cppp = calloc (rows,sizeof(char*));
    cppp[0] = (char *)strCont;
    LOGD( cppp[0]);
    hashme(FileName, cppp );
    jstring strp = (*env)->NewStringUTF(env, cppp[0]);
    return strp;

}

