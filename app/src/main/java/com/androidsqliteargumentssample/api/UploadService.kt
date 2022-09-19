package com.androidsqliteargumentssample.api

import okhttp3.MultipartBody
import okhttp3.RequestBody
import okhttp3.ResponseBody
import retrofit2.Call
import retrofit2.http.*

interface UploadService {


    @Multipart
    @POST("/api/UploadFile/UploadLog")
 suspend   fun upload(

        @Header("Extra") Extra: String,
        @Header("LogUploadGuid") LogUploadGuid: String,
        @Header("UpDnCode") UpDnCode: String,
        @Part("Hash") Hash: RequestBody?,
    /*    @Body sendData: SendData,*/

        @Part file: MultipartBody.Part?
    ):ResponseBody?
}