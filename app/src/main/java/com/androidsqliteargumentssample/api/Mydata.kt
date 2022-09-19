package com.androidsqliteargumentssample.api

import com.google.gson.annotations.SerializedName

data class SendData(

    @SerializedName("Hash")
    val hash: String
)
