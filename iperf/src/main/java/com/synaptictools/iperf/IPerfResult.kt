package com.synaptictools.iperf

/**
 * Created by KMR on October 17, 2021.
 * Copyright (c) 2021 Augmedix. All rights reserved.
 */
sealed class IPerfResult<out T>{
    data class Success<out T>(val data: T): IPerfResult<T>()
    data class Error(val exception: Throwable,
                     val code: Int? = null,
                     val error: Boolean? = null,
                     val errors: List<IPerfException>? = null,
                     val message: String? = null,
                     val method: String? = null,
                     val path: String? = null) : IPerfResult<Nothing>()
}
