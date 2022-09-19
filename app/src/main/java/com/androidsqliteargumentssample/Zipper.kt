package com.androidsqliteargumentssample

import java.io.*
import java.lang.Exception
import java.util.zip.ZipEntry
import java.util.zip.ZipOutputStream
import kotlin.Throws

object Zipper {
    fun zipFileAtPath(sourcePath: String, toLocation: String?): Boolean {
        val BUFFER = 2048
        val sourceFile = File(sourcePath)
        try {
            var origin: BufferedInputStream? = null
            val dest = FileOutputStream(toLocation)
            val out = ZipOutputStream(
                BufferedOutputStream(
                    dest
                )
            )
            if (sourceFile.isDirectory) {
                zipSubFolder(out, sourceFile, sourceFile.parent.length)
            } else {
                val data = ByteArray(BUFFER)
                val fi = FileInputStream(sourcePath)
                origin = BufferedInputStream(fi, BUFFER)
                val entry = ZipEntry(getLastPathComponent(sourcePath))
                entry.time = sourceFile.lastModified() // to keep modification time after unzipping
                out.putNextEntry(entry)
                var count: Int
                while (origin.read(data, 0, BUFFER).also { count = it } != -1) {
                    out.write(data, 0, count)
                }
            }
            out.close()
        } catch (e: Exception) {
            e.printStackTrace()
            return false
        }
        return true
    }

    /*
     *
     * Zips a subfolder
     *
     */
    @Throws(IOException::class)
    private fun zipSubFolder(
        out: ZipOutputStream, folder: File,
        basePathLength: Int
    ) {
        val BUFFER = 2048
        val fileList = folder.listFiles()
        var origin: BufferedInputStream? = null
        for (file in fileList) {
            if (file.isDirectory) {
                zipSubFolder(out, file, basePathLength)
            } else {
                val data = ByteArray(BUFFER)
                val unmodifiedFilePath = file.path
                val relativePath = unmodifiedFilePath
                    .substring(basePathLength)
                val fi = FileInputStream(unmodifiedFilePath)
                origin = BufferedInputStream(fi, BUFFER)
                val entry = ZipEntry(relativePath)
                entry.time = file.lastModified() // to keep modification time after unzipping
                out.putNextEntry(entry)
                var count: Int
                while (origin.read(data, 0, BUFFER).also { count = it } != -1) {
                    out.write(data, 0, count)
                }
                origin.close()
            }
        }
    }

    /*
     * gets the last path component
     *
     * Example: getLastPathComponent("downloads/example/fileToZip");
     * Result: "fileToZip"
     */
  private  fun getLastPathComponent(filePath: String): String {
        val segments = filePath.split("/").toTypedArray()
        return if (segments.size == 0) "" else segments[segments.size - 1]
    }
}