package com.androidsqliteargumentssample

import android.app.Application
import android.content.Context
import timber.log.Timber
import timber.log.Timber.Forest.plant


class TimberApplication:Application() {


    private fun getApplication(): Application? {
        return sApplication
    }

    fun getContext(): Context? {
        return getApplication()?.applicationContext
    }
    companion object{

        private var sApplication: Application? = null
        fun getContext(): Context? {
            return sApplication
        }
    }


    override fun onCreate() {
        super.onCreate()
        sApplication = this
        plant(Timber.DebugTree())
        plant(FileLoggingTree(sApplication))
        
    }
}