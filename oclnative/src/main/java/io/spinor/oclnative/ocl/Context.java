package io.spinor.oclnative.ocl;

import io.spinor.oclnative.OclNative;
import org.bytedeco.javacpp.Pointer;

/**
 * This class represents an OCL context.
 *
 * @author A. Mahmood (arshadm@spinor.io)
 */
public class Context {
    /** The context id. */
    private Pointer contextId;

    /**
     * Create a new context for the specified device.
     *
     * @param device the device
     */
    public Context(Device device) {
        final OclNative oclNative = OclNative.getInstance();

        this.contextId = oclNative.createContext(device.getDeviceId());
    }
}
