package io.spinor.oclnative.ocl;

import com.google.common.base.MoreObjects;
import com.google.common.base.Objects;
import io.spinor.oclnative.OclNative;
import org.bytedeco.javacpp.Pointer;
import org.bytedeco.javacpp.PointerPointer;

import java.util.ArrayList;
import java.util.Hashtable;
import java.util.List;
import java.util.Map;

/**
 * This class represents a platform.
 *
 * @author A. Mahmood (arshadm@spinor.io)
 */
public class Platform {
    /**
     * The platform name.
     */
    public static final String CL_PLATFORM_NAME = "CL_PLATFORM_NAME";

    /**
     * The platform vendor.
     */
    public static final String CL_PLATFORM_VENDOR = "CL_PLATFORM_VENDOR";

    /**
     * The platform version.
     */
    public static final String CL_PLATFORM_VERSION = "CL_PLATFORM_VERSION";

    /** The platform id. */
    private Pointer platformId;

    /**
     * The platform extensions.
     */
    private String extensions;

    /**
     * The platform info.
     */
    private Map<String, String> platformInfo = new Hashtable<>();

    /**
     * The list of devices
     */
    private List<Device> devices;

    /**
     * Create and initialize the platform with the specified id.
     *
     * @param platformId the platform id
     */
    public Platform(Pointer platformId) {
        final OclNative oclNative = OclNative.getInstance();

        this.platformId = platformId;
        this.extensions = oclNative.getPlatformExtensions(platformId);

        final OclNative.StringVector nativePlatformInfo = oclNative.getPlatformInfo(platformId);
        for(int i = 0; i < nativePlatformInfo.size(); i++) {
            final String value = nativePlatformInfo.get(i).getString();
            final int separatorIndex = value.indexOf(":");

            platformInfo.put(value.substring(0, separatorIndex), value.substring(separatorIndex + 1));
        }

        devices = Device.getDevices(platformId);
    }

    /**
     * Gets the platform id.
     *
     * @return the platform id
     */
    public Pointer getPlatformId() {
        return platformId;
    }

    /**
     * Gets the platform info.
     *
     * @return the platform info
     */
    public Map<String, String> getPlatformInfo() {
        return platformInfo;
    }

    /**
     *
     * @return
     */
    public String toString() {
        return MoreObjects
            .toStringHelper(this)
            .add("extensions", this.extensions)
            .add("platformInfo", platformInfo)
            .add("devices", devices)
            .toString();
    }

    /**
     * Get all platforms.
     *
     * @return the list fo platforms
     */
    public static List<Platform> getPlatforms() {
        final OclNative oclNative = OclNative.getInstance();
        final PointerPointer platformIds = oclNative.getPlatformIds();
        final List<Platform> platforms = new ArrayList<>();

        for(int i=0; i < platformIds.capacity(); i++) {
            platforms.add(new Platform(platformIds.get(i)));
        }

        return platforms;
    }
}
