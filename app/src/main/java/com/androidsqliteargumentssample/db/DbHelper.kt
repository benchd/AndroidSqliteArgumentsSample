package com.androidsqliteargumentssample.db

import android.content.ContentValues
import android.content.Context
import android.database.Cursor
import android.database.sqlite.SQLiteDatabase
import android.database.sqlite.SQLiteOpenHelper
import android.util.Log
import java.sql.SQLException


class DbHelper(private val myContext: Context) :
    SQLiteOpenHelper(myContext, DATABASE_NAME, null, DATABASE_VERSION)
/*SQLiteOpenHelper(DatabaseContext(myContext, dbPath), dbName, null, 1)*/ {
    private val dbNamePassed = DATABASE_NAME
    private val TAG = "DbHelper"
    override fun onOpen(db: SQLiteDatabase?) {
        super.onOpen(db)
        Log.d(TAG, "onOpen: Executed with $dbNamePassed")
    }


/*    @Throws(SQLException::class)
    fun openDataBaseOffline() {
        // Open the database
        val myPath = DB_PATH_OFFLINE
        this. SQLiteDatabase
            .openDatabase(
                myPath, null,
                SQLiteDatabase.OPEN_READWRITE or SQLiteDatabase.NO_LOCALIZED_COLLATORS
            )
    }*/

/*    @Throws(SQLException::class)
    fun openDataBaseMain() {
        // Open the database
        val myPath = DB_PATH_MAIN
        mainDatabase = SQLiteDatabase
            .openDatabase(
                myPath, null,
                SQLiteDatabase.OPEN_READWRITE or SQLiteDatabase.NO_LOCALIZED_COLLATORS
            )
    }*/

    fun executeQuery(sql: String) {
        //this.writableDatabase.execSQL(sql,null)
        this.writableDatabase.execSQL(sql)
    }

    fun insertQuery(tableName: String, values: ContentValues) {
        this.writableDatabase.insert(tableName, null, values)
    }

    fun updateQuery(
        tableName: String,
        values: ContentValues,
        whereClause: String,
        whereArgs: Array<String>
    ) {
        this.writableDatabase.update(tableName, values, whereClause, whereArgs)
    }

    suspend fun deleteQuery(
        tableName: String,
        values: ContentValues,
        whereClause: String,
        whereArgs: Array<String>
    ) {
        this.writableDatabase.delete(tableName, whereClause, whereArgs)
    }

    fun executeQueryWithResults(sql: String, args: Array<String>): Cursor {
        return readableDatabase.rawQuery(sql, args)
    }

    fun executeQueryNoArgs(sql: String): Cursor {
        return readableDatabase.rawQuery(sql, null)
    }

    @Synchronized
    override fun close() {
        super.close()
    }

    override fun onCreate(db: SQLiteDatabase) {
        Log.d(TAG, "onCreate: Executed")
        val CREATE_CONTACTS_TABLE = ("CREATE TABLE " + TABLE_CONTACTS + "("
                + KEY_ID + " INTEGER PRIMARY KEY autoincrement," + KEY_NAME + " TEXT,"
                + KEY_MESSAGE + " TEXT" + ")")
        db.execSQL(CREATE_CONTACTS_TABLE)
    }

    override fun onUpgrade(
        db: SQLiteDatabase,
        oldVersion: Int,
        newVersion: Int
    ) {
        // Not Used
    }
    fun addContact(name:String,message:String) {

        val values = ContentValues()
        values.put(KEY_NAME, name) // Contact Name
        values.put(KEY_MESSAGE, message) // Contact Phone
        writableDatabase.insert(TABLE_CONTACTS, null, values)
        writableDatabase.close()
    }

    companion object {
        private val DATABASE_VERSION = 1
        private val DATABASE_NAME = "contactsManager"
        private val TABLE_CONTACTS = "contacts"
        private val KEY_ID = "id"
        private val KEY_NAME = "name"
        private val KEY_MESSAGE = "message"
    }
}