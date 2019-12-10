package com.util.iperf;

/**
 * Created by herman on 6/3/2017.
 */

public class IperfException extends Exception {

    public IperfException() {
    }

    public IperfException(String message) {
        super(message);
    }

    public IperfException(String message, Throwable cause) {
        super(message, cause);
    }

    public IperfException(Throwable cause) {
        super(cause);
    }

    public IperfException(String message, Throwable cause, boolean enableSuppression, boolean writableStackTrace) {
        super(message, cause, enableSuppression, writableStackTrace);
    }
}
