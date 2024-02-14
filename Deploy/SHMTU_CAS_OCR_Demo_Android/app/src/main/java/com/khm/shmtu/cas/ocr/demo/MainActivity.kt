package com.khm.shmtu.cas.ocr.demo

import android.R.attr.value
import android.annotation.SuppressLint
import android.app.Activity
import android.content.Intent
import android.content.pm.ActivityInfo
import android.graphics.Bitmap
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.Button
import android.widget.ImageView
import android.widget.TextView
import android.widget.Toast
import com.khm.shmtu.cas.ocr.SHMTU_NCNN
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.MainScope
import kotlinx.coroutines.launch
import java.io.FileNotFoundException


class MainActivity : Activity(), CoroutineScope by MainScope() {
    private val shmtuNcnn = SHMTU_NCNN()

    private var imageView: ImageView? = null
    private var innerBitmap: Bitmap? = null

    private var infoResult: TextView? = null

    @SuppressLint("SourceLockedOrientationActivity")
    public override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activate_main)

        // 设置屏幕方向为竖屏模式
        requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT;

        initWidget()
    }

    private fun doOcrDemo(useGpu: Boolean) {
        if (innerBitmap == null) return
        val result_obj = shmtuNcnn.predict_validate_code(innerBitmap, useGpu)
        if (result_obj == null) {
            Toast.makeText(
                this@MainActivity,
                "detect failed!",
                Toast.LENGTH_SHORT
            )
                .show()
            return
        }
        infoResult!!.text = result_obj[1] as String
    }

    private fun initWidget() {
        val retInit = shmtuNcnn.InitModel(assets)
        if (!retInit) {
            Log.e("MainActivity", "NCNN Init failed")
        }

        infoResult = findViewById<View>(R.id.infoResult) as TextView
        imageView = findViewById<View>(R.id.imageView) as ImageView

        val buttonGetFromNet = findViewById<View>(R.id.buttonGetFromNet) as Button
        buttonGetFromNet.setOnClickListener {
            val imageURL = "https://cas.shmtu.edu.cn/cas/captcha"
            this.launch {
                try {
                    val bitmap = ImageUtils.downloadImageFromURL(imageURL)
                    innerBitmap = bitmap
                    imageView!!.setImageBitmap(bitmap)
                } catch (e: Exception) {
                    // 处理异常，例如显示错误消息
                    e.printStackTrace()
                }
            }
        }

        (findViewById<View>(R.id.buttonInner1) as Button).setOnClickListener {
            val bitmap = ImageUtils.getBitmapFromAssets(
                this@MainActivity,
                "test1_20240102160004_server.png"
            )
            innerBitmap = bitmap
            imageView!!.setImageBitmap(bitmap)
        }
        (findViewById<View>(R.id.buttonInner2) as Button).setOnClickListener {
            val bitmap = ImageUtils.getBitmapFromAssets(
                this@MainActivity,
                "test2_20240102160811_server.png"
            )
            innerBitmap = bitmap
            imageView!!.setImageBitmap(bitmap)
        }

        val buttonImage = findViewById<View>(R.id.buttonSelectImageFromLocal) as Button
        buttonImage.setOnClickListener {
            val i = Intent(Intent.ACTION_PICK)
            i.setType("image/*")
            startActivityForResult(i, REQUEST_CODE_SELECT_IMAGE)
        }

        val buttonDetect = findViewById<View>(R.id.buttonDetect) as Button
        buttonDetect.setOnClickListener { doOcrDemo(false) }
        val buttonDetectGPU = findViewById<View>(R.id.buttonDetectGPU) as Button
        buttonDetectGPU.setOnClickListener { doOcrDemo(true) }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)

        if (resultCode == RESULT_OK) {
            // Get the image from data
            try {
                if (requestCode == REQUEST_CODE_SELECT_IMAGE) {
                    val selectedImage = data?.data
                    val bitmap = ImageUtils.decodeUri(this, selectedImage)
                    val rgba = bitmap!!.copy(Bitmap.Config.ARGB_8888, true)

                    // resize to 400x140
                    innerBitmap =
                        Bitmap.createScaledBitmap(rgba, 400, 140, false)
                    rgba.recycle()
                    imageView!!.setImageBitmap(bitmap)
                }
            } catch (e: FileNotFoundException) {
                Log.e("MainActivity", "FileNotFoundException")
                return
            }
        }
    }

    companion object {
        private const val REQUEST_CODE_SELECT_IMAGE = 1
    }
}
