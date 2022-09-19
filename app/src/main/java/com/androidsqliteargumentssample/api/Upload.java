package com.androidsqliteargumentssample.api;

import android.net.Uri;

import java.io.File;

import okhttp3.MediaType;
import okhttp3.MultipartBody;
import okhttp3.RequestBody;

public class Upload {

    private void uploadFile(Uri fileUri) {
        // create upload service client


        // https://github.com/iPaulPro/aFileChooser/blob/master/aFileChooser/src/com/ipaulpro/afilechooser/utils/FileUtils.java
        // use the FileUtils to get the actual file by uri
        File file = new File(String.valueOf(fileUri));
        RequestBody requestFile =
                RequestBody.create(file,MediaType.parse("multipart/form-data"));


// MultipartBody.Part is used to send also the actual file name
        MultipartBody.Part body =
                MultipartBody.Part.createFormData("image", file.getName(), requestFile);

// add another part within the multipart request



    }
}
