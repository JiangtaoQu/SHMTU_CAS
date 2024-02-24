package com.khm.shmtu.cas.demo

import com.khm.shmtu.cas.auth.EpayAuth

class BillDemo {

    companion object {

        fun testBill() {
            val epayAuth = EpayAuth()
            epayAuth.login("", "")
            val billResult =
                epayAuth.getBill(pageNo = "1")
            println(billResult.first)
            println(billResult.second)
        }

    }

}