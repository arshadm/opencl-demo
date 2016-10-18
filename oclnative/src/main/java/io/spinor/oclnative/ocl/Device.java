package io.spinor.oclnative.ocl;

import com.google.common.base.MoreObjects;
import io.spinor.oclnative.OclNative;
import org.bytedeco.javacpp.Pointer;
import org.bytedeco.javacpp.PointerPointer;

import java.util.ArrayList;
import java.util.Hashtable;
import java.util.List;
import java.util.Map;

/**
 * This class represents a device.
 *
 * @author A. Mahmood (arshadm@spinor.io)
 */
public class Device {
    /**
     * The device name.
     */
    public static final String CL_DEVICE_NAME = "CL_DEVICE_NAME";

    /**
     * The device id.
     */
    private Pointer deviceId;

    /**
     * The device info.
     */
    private Map<String, String> deviceInfo = new Hashtable<>();

    /**
     * Initialize a new device with the specified device id.
     *
     * @param deviceId the device id
     */
    public Device(Pointer deviceId) {
        final OclNative oclNative = OclNative.getInstance();

        this.deviceId = deviceId;

        final OclNative.StringVector nativeDeviceInfo = oclNative.getDeviceInfo(deviceId);
        for (int i = 0; i < nativeDeviceInfo.size(); i++) {
            final String value = nativeDeviceInfo.get(i).getString();
            final int separatorIndex = value.indexOf(":");

            deviceInfo.put(value.substring(0, separatorIndex), value.substring(separatorIndex + 1));
        }
    }

    /**
     * Gets the device id.
     *
     * @return
     */
    public Pointer getDeviceId() {
        return deviceId;
    }

    /**
     * Gets the device name.
     *
     * @return the device name
     */
    public String getDeviceName() {
        return deviceInfo.get(CL_DEVICE_NAME);
    }

    /**
     * Gets the string representation.
     *
     * @return the string representation
     */
    public String toString() {
        return MoreObjects.toStringHelper(this).add("deviceInfo", deviceInfo).toString();
    }

    /**
     * Gets the devices for the specified platform id.
     *
     * @param platformId the platform id
     *
     * @return the list of devices
     */
    public static List<Device> getDevices(final Pointer platformId) {
        final OclNative oclNative = OclNative.getInstance();
        final PointerPointer deviceIds = oclNative.getDeviceIds(platformId);
        final List<Device> devices = new ArrayList<>();

        for (int i = 0; i < deviceIds.capacity(); i++) {
            devices.add(new Device(deviceIds.get(i)));
        }

        return devices;
    }
}
