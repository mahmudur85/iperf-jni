package com.synaptictools.iperf

import java.lang.Exception

/**
 * Created by KMR on October 17, 2021.
 * Copyright (c) 2021 Augmedix. All rights reserved.
 */
object IPerf: iPerfNative() {
    const val TAG = "PerfNative:"
    private var requestResult: StringBuilder? = null

    private var callbacks: IPerfResultCallback? = null

    fun seCallBack(callback: IPerfResultCallback.() -> Unit) {
        val iPerfResultCallback = IPerfResultCallback()
        iPerfResultCallback.callback()
        this.callbacks = iPerfResultCallback
    }

    override fun onAppendResult(result: String?) {
        println("$TAG onAppendResult - $result")
        if (requestResult == null) {
            requestResult = StringBuilder()
        }
        requestResult?.append(result)
        callbacks?.onUpdate?.invoke(result)
    }

    override fun onClearResult() {
        println("$TAG onClearResult()")
        requestResult?.clear()
        requestResult = null
    }

    fun request(config: IPerfConfig): IPerfResult<String> {
        try {
            isDebug = config.debug
            init(
                config.hostname,
                config.port,
                config.stream,
                config.duration,
                config.interval,
                config.download,
                config.useUDP,
                config.json
            )
            val ret = execute()
            if (ret == 0) {
                callbacks?.onSuccess?.invoke()
                return IPerfResult.Success(requestResult.toString())
            }
        } catch (e: Exception) {
            callbacks?.onError?.invoke(IPerfException(e))
            return IPerfResult.Error(IPerfException(e))
        }
        callbacks?.onError?.invoke(IPerfException("IPerf Request failed"))
        return IPerfResult.Error(IPerfException("IPerf Request failed"))
    }
}