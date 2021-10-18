package com.synaptictools.iperf

/**
 * Created by KMR on October 17, 2021.
 * Copyright (c) 2021 Augmedix. All rights reserved.
 */
class IPerfResultCallback {
    internal var onSuccess: (() -> Unit)? = null

    internal var onUpdate: ((text: String?) -> Unit)? = null

    internal var onError: ((e: Exception) -> Unit)? = null

    fun success(success: () -> Unit) {
        onSuccess = success
    }

    fun update(update: (test: String?) -> Unit) {
        onUpdate = update
    }

    fun error(error: (throwable: Exception?) -> Unit) {
        onError = error
    }
}