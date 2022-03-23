package com.androidsqliteargumentssample

import android.content.Context
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import com.androidsqliteargumentssample.databinding.FragmentFirstBinding
import com.androidsqliteargumentssample.db.DbHelper
import com.google.android.material.snackbar.Snackbar
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
        dbHelper = DbHelper(context)
    }
    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        binding.apply {

            buttonInsert5k.setOnClickListener {
                Log.e(TAG, "Insert5k: Begins")

                for (i in 1..5000) {
                    //-----------Trace begin-----------
                    dbHelper.addContact(createRandomString(), createRandomString())
                    //----------Trace End-----------
                }

                Log.e(TAG, "Insert5k: Ends XXXXX")
            }

            buttonInsert.setOnClickListener {
                //-----------Trace begin-----------
                dbHelper.addContact(editTextName.text.toString(), editTextMessage.text.toString())
                //------------Trace End------------
                editTextName.text.clear()
                editTextMessage.text.clear()
                Snackbar.make(view, "Data Inserted", Snackbar.LENGTH_LONG)
                    .setAction("Action", null).show()
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