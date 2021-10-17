package com.synaptictools.iperf

import java.lang.Exception

/**
 * Created by herman on 6/3/2017.
 */
class IPerfException : Exception {
    constructor() {}
    constructor(message: String?) : super(message) {}
    constructor(message: String?, cause: Throwable?) : super(message, cause) {}
    constructor(cause: Throwable?) : super(cause) {}
}