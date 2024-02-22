package com.khm.shmtu.cas

import com.khm.shmtu.cas.auth.WechatAuth
import com.khm.shmtu.cas.captcha.Captcha
import com.khm.shmtu.cas.parser.BillParser


fun main() {

//    val wechatAuth = WechatAuth()
//    wechatAuth.login("", "")
//    val hotWaterResult = wechatAuth.getHotWater()
//    println(hotWaterResult.first)
//    println(hotWaterResult.second)

    Captcha.testLocalTcpServerOcrMultiThread()
}
