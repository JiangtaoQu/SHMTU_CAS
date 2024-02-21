package com.khm.shmtu.cas.auth

import com.khm.shmtu.cas.HtmlCommon
import com.khm.shmtu.cas.auth.common.CasAuth
import com.khm.shmtu.cas.captcha.Captcha
import okhttp3.OkHttpClient
import okhttp3.Request

class EpayAuth {

    private var savedCookie = ""
    private var htmlCode = ""

    private var loginUrl = ""
    private var loginCookie = ""

    fun getBill(
        pageNo: String = "1",
        tabNo: String = "1",
        cookie: String = ""
    ): Triple<Int, String, String> {
        val url =
            "https://ecard.shmtu.edu.cn/epay/consume/query" +
                    "?pageNo=$pageNo" +
                    "&tabNo=$tabNo"

        val client = OkHttpClient.Builder()
            .followRedirects(false)
            .followSslRedirects(false)
            .build()

        val request = Request.Builder()
            .url(url)
            .addHeader("Cookie", cookie)
            .get()
            .build()

        val response = client.newCall(request).execute()

        val responseCode = response.code

        return if (responseCode == 200) {
            htmlCode = (response.body?.string() ?: "").trim()

            Triple(responseCode, htmlCode, cookie)
        } else if (responseCode == 302) {
            val location =
                response.header("Location") ?: ""
            val newCookie =
                response.header("Set-Cookie") ?: cookie

            savedCookie = newCookie

            Triple(responseCode, location, newCookie)
        } else {
            Triple(responseCode, "", "")
        }
    }

    fun testLoginStatus(): Boolean {
        val resultBill =
            getBill(cookie = savedCookie)

        if (resultBill.first == 200) {
            // OK
            return true
        } else if (resultBill.first == 302) {
            this.loginUrl =
                resultBill.second
            this.loginCookie =
                resultBill.third

            return false
        } else {
            return false
        }
    }

    fun login(
        username: String,
        password: String
    ): Boolean {

        if (loginUrl.isBlank() || loginCookie.isBlank()) {
            if (testLoginStatus()) {
                return true
            }
        }

        val executionStr =
            CasAuth.getExecution(
                this.loginUrl,
                this.loginCookie
            )

        // 下载验证码
        val resultCaptcha =
            Captcha.getImageDataFromUrlUsingGet(cookie = savedCookie)

        // 检验下载的数据
        if (resultCaptcha == null) {
            println("获取验证码图片失败")
            return false
        }
        val imageData = resultCaptcha.first
        savedCookie = resultCaptcha.second
        if (imageData == null) {
            println("获取验证码失败")
            return false
        }

        // 调用远端识别接口
        val validateCode: String =
            Captcha.ocrByRemoteTcpServer(
                "127.0.0.1", 21601,
                imageData
            )
        val exprResult =
            Captcha.getExprResultByExprString(validateCode)

        val resultCas =
            CasAuth.casLogin(
                this.loginUrl,
                username,
                password,
                exprResult,
                executionStr,
                savedCookie
            )

        if (resultCas.first != 302) {
            println("程序出错，状态码：${resultCas.first}")
            return false
        }

        val resultRedirect =
            CasAuth.casRedirect(
                resultCas.second,
                resultCas.third
            )

        savedCookie = resultRedirect.third

        val resultBill =
            getBill(cookie = savedCookie)

        return resultBill.first == 200
    }

}
