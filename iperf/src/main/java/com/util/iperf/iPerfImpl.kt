package com.util.iperf

class iPerfImpl(hostname: String?,
                port: Int,
                streamTemplate: String?,
                duration: Int,
                interval: Int,
                download: Boolean,
                private val reporter: iPerfReporter
) : iPerf(hostname, port, streamTemplate, duration, interval, download) {

    override fun onIntermediateResult(bits: Double, download: Boolean) {
        super.onIntermediateResult(bits, download)
        reporter.onIntermediateResult(bits, download)

    }

    override fun onSummaryResult(totalBits: Double, count: Int, download: Boolean) {
        super.onSummaryResult(totalBits, count, download)
        reporter.onSummaryResult(totalBits, count, download)
    }

    interface iPerfReporter{
        fun onIntermediateResult(bits: Double, download: Boolean)
        fun onSummaryResult(totalBits: Double, count: Int, download: Boolean)
    }
}