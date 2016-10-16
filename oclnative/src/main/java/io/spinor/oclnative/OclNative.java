package io.spinor.oclnative;

import org.bytedeco.javacpp.Loader;
import org.bytedeco.javacpp.Pointer;
import org.bytedeco.javacpp.annotation.Platform;
import org.bytedeco.javacpp.annotation.StdString;

import java.util.Properties;

/**
 * The Java interface to the Native OCL API.
 *
 * @author A. Mahmood (arshadm@spinor.io)
 */
@Platform(include = "OclNative.h", compiler = "cpp11", link = "oclnative", library = "jnioclnative")
public class OclNative extends Pointer {
    static {
        // using our custom platform properties from resources, and on user request,
        // load in priority libraries found in the library path over bundled ones
        String platform = Loader.getPlatform();
        Properties properties = Loader.loadProperties(platform + "-oclnative", platform);
        String pathsFirst = System.getProperty("oclnative.pathsfirst", "false").toLowerCase();
        try {
            Loader.load(OclNative.class, properties, pathsFirst.equalsIgnoreCase("true"));
        } catch (UnsatisfiedLinkError ex) {
            throw new RuntimeException("OclNative is probably missing dependencies.", ex);
        }
    }

    public OclNative() {
        allocate();
    }

    private native void allocate();

    public native @StdString String hello();
}
