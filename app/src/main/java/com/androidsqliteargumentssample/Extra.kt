package com.androidsqliteargumentssample

import com.google.gson.annotations.SerializedName

data class Extra (@SerializedName("secondsFromGMT") val secondsFromGMT: String,
                  @SerializedName("totalDiskSpace") val totalDiskSpace: String,
                  @SerializedName("timeZoneInfo") val timeZoneInfo: String,
                  @SerializedName("freeDiskSpace") val freeDiskSpace: String){
}