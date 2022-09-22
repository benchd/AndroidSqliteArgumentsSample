package com.androidsqliteargumentssample


import android.content.Context
import android.os.Build
import android.os.Bundle
import android.os.Environment
import android.os.StatFs
import android.provider.Settings
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.fragment.app.Fragment
import com.androidsqliteargumentssample.api.Retrofit
import com.androidsqliteargumentssample.api.ShaUtil256
import com.androidsqliteargumentssample.api.UploadService
import com.androidsqliteargumentssample.databinding.FragmentFirstBinding
import com.androidsqliteargumentssample.db.DbHelper
import com.androidsqliteargumentssample.db.NativeStore
import com.anggrayudi.storage.file.DocumentFileCompat
import com.anggrayudi.storage.file.StorageId
import com.google.android.material.snackbar.Snackbar
import com.google.gson.Gson
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import okhttp3.Headers
import okhttp3.MediaType.Companion.toMediaTypeOrNull
import okhttp3.MultipartBody
import okhttp3.MultipartBody.Part.Companion.create
import okhttp3.RequestBody
import timber.log.Timber
import java.io.File
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
            btnUpload.setOnClickListener {

                try {
                    val file = File("$filePath/log.zip")
                    val hash = ShaUtil256.hashFile(file)
                    val requestFile =
                        RequestBody.create("multipart/form-data".toMediaTypeOrNull(), file)

                    val body = createFormData("logfileszip", file.name, requestFile, hash)
                    val tz = TimeZone.getDefault()
                    val now = Date()
                    val offsetFromUtc = tz.getOffset(now.time) / 1000
                    val freeBytesOfPrimary =
                        context?.let { it1 ->
                            DocumentFileCompat.getFreeSpace(
                                it1,
                                StorageId.PRIMARY
                            )
                        }
                    val totalBytesOfPrimary =
                        context?.let { it1 ->
                            DocumentFileCompat.getStorageCapacity(
                                it1,
                                StorageId.PRIMARY
                            )
                        }

                    val extra = Extra(
                        "$offsetFromUtc",
                        "$totalBytesOfPrimary",
                        tz.displayName,
                        "$freeBytesOfPrimary"
                    )
                    CoroutineScope(Dispatchers.IO).launch {

                        try {
                            uploadService.upload(
                                Gson().toJson(extra), "aa", "299347341", body
                            )

                        } catch (e: Exception) {

                            Timber.tag("UploadService").e(e.stackTraceToString())

                        }

                    }
                } catch (e: Exception) {

                    Timber.tag("UploadService").e(e.stackTraceToString())

                }

            }
            btnViewStorage.setOnClickListener {

                val data =
                    "${getDeviceName()},                 ${getAndroidVersion()},                     ${Build.MODEL}"
                val stat = StatFs(Environment.getExternalStorageDirectory().path)
                val bytesAvailable = stat.blockSizeLong * stat.availableBlocksLong
                val megAvailable = bytesAvailable / (1024 * 1024)
                val freeBytesOfPrimary =
                    context?.let { it1 -> DocumentFileCompat.getFreeSpace(it1, StorageId.PRIMARY) }

                Timber.d("Available MB : $freeBytesOfPrimary")

                Toast.makeText(
                    context,
                    "Available MB : $freeBytesOfPrimary,$bytesAvailable,\r\n Info :$data ",
                    Toast.LENGTH_LONG
                ).show()
            }

        }
    }

    private fun getAndroidVersion(): String? {
        val release = Build.VERSION.RELEASE
        val sdkVersion = Build.VERSION.SDK_INT
        return "Android SDK: $sdkVersion ($release)"
    }

    private fun getDeviceName(): String? {
        var userDeviceName =
            Settings.Global.getString(TimberApplication.sApplication?.contentResolver, Settings.Global.DEVICE_NAME)
        if (userDeviceName == null) userDeviceName =
            Settings.Secure.getString(TimberApplication.sApplication?.contentResolver, "bluetooth_name")
        return userDeviceName
    }


    fun createFormData(
        name: String,
        filename: String?,
        body: RequestBody,
        hash: String
    ): MultipartBody.Part {
        val disposition = buildString {
            append("form-data; name=")
            append("\"$name\"")
            if (filename != null) {
                append("; filename=")
                append("\"$filename\"")
            }

        }
        val headers =
            Headers.headersOf("Hash", hash).newBuilder().add("Content-Disposition", disposition)
                .build()
        return create(headers, body)
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