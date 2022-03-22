package com.androidsqliteargumentssample

import android.app.Application

class MyApplication:Application() {
    override fun onCreate() {
        super.onCreate()
        myInstance = this

    }


    companion object {

         fun  getInstanceOfApp(): MyApplication {
            if (myInstance == null) myInstance =
                MyApplication()
            return myInstance as MyApplication
        }

        private var myInstance: MyApplication? = null

        @get:Synchronized
        val instance: MyApplication?
            get() {
                if (myInstance == null) myInstance =
                    MyApplication()
                return myInstance
            }


    }
}