[![Download](https://maven-badges.herokuapp.com/maven-central/com.synaptic-tools/iperf/badge.svg)](https://maven-badges.herokuapp.com/maven-central/com.synaptic-tools/iperf)

# iPerf Android

An iPerf3 implementation for Android

# Description

Iperf is a tool for network performance measurement and tuning. It is a cross-platform tool that can produce standardized performance measurements for any network. Iperf has client and server functionality, and can create data streams to measure the throughput between the two ends in one or both directions.

# How to use

```bash
implementation "com.synaptic-tools:iperf:1.0.0"
```

# Usages

## Asynchronous implementation 

Use `IPerf.setCallback` method to get the report asynchronously

```kotlin
val config = IPerfConfig(
    hostname = hostname,
    port = port.toInt(),
    stream = stream.path,
    download = true,
    json = false
)
IPerf.seCallBack {
    success { }
    update{ text -> }
    error { e -> }
}
IPerf.request(config)
```

## Synchronous implementation 

Get the response from `IPerf.request(...)` method 

```kotlin
val result = IPerf.request(config)
when (result) {
    is IPerfResult.Success -> {
        println("$TAG iPerf request done ${resultBuilder.toString()}")
        ...
        ...
    }
    is IPerfResult.Error -> {
        println("$TAG iPerf request failed-> ${result.error}")
        ...
        ...
    }
}
```

