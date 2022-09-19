package com.androidsqliteargumentssample

import android.content.Context
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import com.androidsqliteargumentssample.api.Retrofit
import com.androidsqliteargumentssample.api.SendData
import com.androidsqliteargumentssample.api.ShaUtil256
import com.androidsqliteargumentssample.api.UploadService
import com.androidsqliteargumentssample.databinding.FragmentFirstBinding
import com.androidsqliteargumentssample.db.DbHelper
import com.androidsqliteargumentssample.db.NativeStore
import com.google.android.material.snackbar.Snackbar
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import okhttp3.Dispatcher
import okhttp3.MultipartBody.Part.Companion.createFormData
import okhttp3.RequestBody
import java.io.File


import okhttp3.MediaType
import okhttp3.MediaType.Companion.toMediaTypeOrNull

import okhttp3.MultipartBody
import okhttp3.MultipartBody.Part.Companion.create


import retrofit2.http.Part
import timber.log.Timber
import java.sql.Time

import java.util.*
import kotlin.random.Random


/**
 * A simple [Fragment] subclass as the default destination in the navigation.
 */
class FirstFragment : Fragment() {
    private val TAG = "FirstFragment"
    private var _binding: FragmentFirstBinding? = null

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    private lateinit var dbHelper: DbHelper
    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {

        _binding = FragmentFirstBinding.inflate(inflater, container, false)
        return binding.root

    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
        System.loadLibrary("sqliteX")

        dbHelper = DbHelper(context)
    }
    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        binding.apply {

            buttonInsert5k.setOnClickListener {
                Timber.tag(TAG).e("Insert5k: Begins")
                val nativeStore = NativeStore()
                for (i in 1..1000) {
                    //-----------Trace begin-----------
                    dbHelper.addContact(createRandomString(), createRandomString())
                    //----------Trace End-----------
                    dbHelper.nativeStore.ss1()
                    Timber.tag(TAG).e("Insert count : %s", i)
                    val mainDBFilePath = File(
                        context?.getDatabasePath("contactsManager")?.getAbsolutePath()
                            ?: "/data/data/com.androidsqliteargumentssample/databases/contactsManager"
                    )

                    val nativeStore = NativeStore()
                    val hash =
                        nativeStore.stringFromJNI("contactsManager", mainDBFilePath.absolutePath)
                    //Log.e(TAG, "hash -> $hash")

                }



                Timber.tag(TAG).e("Insert5k: Ends XXXXX")


            }

            buttonInsert.setOnClickListener {
                val property = System.getProperty("java.library.path")
                val parser = StringTokenizer(property, ";")
                while (parser.hasMoreTokens()) {
                    System.err.println(parser.nextToken())
                }

                //throw NullPointerException()

                //-----------Trace begin-----------
                dbHelper.addContact(editTextName.text.toString(), editTextMessage.text.toString())
                //------------Trace End------------
                editTextName.text.clear()
                editTextMessage.text.clear()
                Snackbar.make(view, "Data Inserted", Snackbar.LENGTH_LONG)
                    .setAction("Action", null).show()

                val mainDBFilePath = File(
                    context?.getDatabasePath("contactsManager")?.getAbsolutePath()
                        ?: "/data/data/com.androidsqliteargumentssample/databases/contactsManager"
                )
                val nativeStore = NativeStore()

                val hash = nativeStore.stringFromJNI("contactsManager", mainDBFilePath.absolutePath)
                Timber.tag(TAG).e("hash -> %s", hash)
            }
                val retrofit=Retrofit.provideRetrofit("http://smartip1.com:5011")
                val uploadService=retrofit.create(UploadService::class.java)
            val filePath=   requireContext().filesDir.toString()
            btnZip.setOnClickListener {



                Zipper.zipFileAtPath("$filePath/logs","$filePath/log.zip")

            }
            btnUpload.setOnClickListener{


                val file = File("$filePath/log.zip")
                val hash=ShaUtil256.hashFile(file)
                val requestFile =RequestBody.create("multipart/form-data".toMediaTypeOrNull(),file)
                val requestBodyHash=RequestBody.create("text/plain".toMediaTypeOrNull(),"123")
                val body = createFormData("logfileszip", file.name, requestFile)

                val sendData= SendData(hash)
                CoroutineScope(Dispatchers.IO).launch {

                    try {
                        uploadService.upload("","aa","299347341",requestBodyHash,body)
                    } catch (e: Exception) {

                        Timber.tag("UploadService").e(e.stackTraceToString())

                    }


                }

            }

        }
    }

    private val charPool: List<Char> = ('a'..'z') + ('A'..'Z') + ('0'..'9')
    private fun createRandomString() =
        (1..10)
            .map { Random.nextInt(0, charPool.size) }
            .map(charPool::get)
            .joinToString("")


    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}