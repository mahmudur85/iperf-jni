package com.synaptictools.sample

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import android.widget.TextView
import androidx.databinding.DataBindingUtil
import androidx.lifecycle.ViewModelProvider
import com.synaptictools.iperf.IPerfConfig
import com.synaptictools.sample.databinding.ActivityMainBinding
import java.io.File

class MainActivity : AppCompatActivity() {
    companion object {
        private const val TAG = "MainActivity:"
    }

    private lateinit var activityMainBinding: ActivityMainBinding
    private lateinit var mainViewModel: MainViewModel

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        activityMainBinding = DataBindingUtil.setContentView(this, R.layout.activity_main)

        mainViewModel = ViewModelProvider(this).get(MainViewModel::class.java)

        with(activityMainBinding) {
            lifecycleOwner = this@MainActivity
            viewModel = mainViewModel
            btRequest.setOnClickListener {
                val hostname: String = etHostname.text.toString()
                val port = etPort.text.toString()
                if (hostname.isNotEmpty() && port.isNotEmpty()) {
                    val stream = File(applicationContext.filesDir, "iperf3.XXXXXX")
                    mainViewModel.startRequest(
                        IPerfConfig(
                            hostname = hostname,
                            port = port.toInt(),
                            stream = stream.path,
                            download = true,
                            useUDP = false,
                            json = false,
                            debug = false
                        ),
                        isAsync = true
                    )
                }
            }
        }
    }
}
