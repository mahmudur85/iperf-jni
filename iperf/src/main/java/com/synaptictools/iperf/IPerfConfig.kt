package com.synaptictools.iperf

/**
 * Created by KMR on October 17, 2021.
 * Copyright (c) 2021 Augmedix. All rights reserved.
 */
data class IPerfConfig(
    val hostname: String,
    val port: Int = 5201,
    val stream: String,
    val duration: Int = 10,
    val interval: Int = 1,
    val download: Boolean = false,
    val useUDP: Boolean = false,
    val json: Boolean = false,
    val debug: Boolean = false
) {
    override fun toString(): String {
        return "IPerfConfig(hostname='$hostname', port=$port, stream='$stream', duration=$duration, interval=$interval, download=$download, useUDP=$useUDP, json=$json, debug=$debug)"
    }
}
