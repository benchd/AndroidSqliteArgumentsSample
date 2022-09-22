package com.androidsqliteargumentssample.api

import com.androidsqliteargumentssample.Extra
import okhttp3.MultipartBody
import okhttp3.ResponseBody
import retrofit2.http.Header
import retrofit2.http.Multipart
import retrofit2.http.POST
import retrofit2.http.Part

interface UploadService {


    @Multipart
    @POST("/api/UploadFile/UploadLog")
 suspend   fun upload(

        @Header("Extra") Extra: String,
        @Header("LogUploadGuid") LogUploadGuid: String,
        @Header("UpDnCode") UpDnCode: String,








        @Part file: MultipartBody.Part?
    ):ResponseBody?
}