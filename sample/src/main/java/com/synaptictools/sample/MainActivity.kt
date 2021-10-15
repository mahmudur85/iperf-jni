package com.synaptictools.sample

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import android.widget.Button
import android.widget.TextView
import com.synaptictools.iperf.IperfException
import com.synaptictools.iperf.iPerfImpl
import java.io.File

class MainActivity : AppCompatActivity(), iPerfImpl.iPerfReporter {

    private val handler = Handler()
    //    private lateinit var perf3: IPerfClient
    private var iperf: iPerfImpl? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        System.loadLibrary("iperf-jni")

        val button: Button = findViewById(R.id.button)
        val tv: TextView = findViewById<TextView>(R.id.editResult)

        button.setOnClickListener { view ->
            Thread(Runnable {
                try {
                    val streamTemplate = File(applicationContext.getCacheDir(), "iperf3tempXXXXXX")
                    iperf = iPerfImpl(
                        "iperf.scottlinux.com",
                        5201,
                        streamTemplate.path,
                        20,
                        1,
                        false,
                        this
                    )
                    iperf?.isDebug = false
                    iperf?.init()
                    iperf?.run()
                } catch (e: IperfException) {
                    e.printStackTrace()
                }
            }).start()
        }
    }
    override fun onIntermediateResult(bits: Double, download: Boolean) {
        println("${(if (download) "download" else "upload")} rate: $bits")
    }

    override fun onSummaryResult(totalBits: Double, count: Int, download: Boolean) {
        println("total ${(if (download) "downloaded" else "uploaded")} : ${totalBits/count}")
    }
}
