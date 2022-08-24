package com.androidsqliteargumentssample.db

class NativeStore {
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
//    external fun stringFromJNI(): String
//    external fun Java_com_androidsqliteargumentssample_db_ss1(): String
//    external fun Java_com_androidsqliteargumentssample_db_NativeStore_Java_com_androidsqliteargumentssample_test111(): Int
//    external fun testFunction(): Int
    external fun test():Boolean
    external fun ss1():String
    external fun TraceCallbackCount():Int
    external fun ss2(foo: Int):String

    companion object {

        @JvmStatic
        fun testMethod(numOne: Int, numTwo: Int) = numOne + numTwo

        // Used to load the 'native-lib' library on application startup.
        init {
     //       System.loadLibrary("sqliteX")
        }
    }
}