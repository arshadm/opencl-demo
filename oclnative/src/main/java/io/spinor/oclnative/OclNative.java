package io.spinor.oclnative;

import org.bytedeco.javacpp.BytePointer;
import org.bytedeco.javacpp.Loader;
import org.bytedeco.javacpp.Pointer;
import org.bytedeco.javacpp.PointerPointer;
import org.bytedeco.javacpp.annotation.*;

import java.util.Properties;

/**
 * The Java interface to the Native OCL API.
 *
 * @author A. Mahmood (arshadm@spinor.io)
 */
@Platform(include = {"OclNative.h", "<vector>", "<string>"}, compiler = "cpp11", link = "oclnative", library = "jnioclnative")
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

    private static OclNative instance = new OclNative();

    public static OclNative getInstance() {
        return instance;
    }

    private OclNative() {
        allocate();
    }

    private native void allocate();

    public native @StdVector PointerPointer getPlatformIds();

    public native StringVector getPlatformInfo(Pointer platformId);

    public native @StdString String getPlatformExtensions(Pointer platformId);

    public native @StdVector PointerPointer getDeviceIds(Pointer platformId);

    public native StringVector getDeviceInfo(Pointer deviceId);

    public native Pointer createContext(Pointer deviceId);

    public native Pointer createProgram(Pointer contextId, @StdString String source);

    public native void buildProgram(Pointer deviceId, Pointer programId);

    public native Pointer createKernel(Pointer programId, @StdString String kernelName);

    public native Pointer createCommandQueue(Pointer deviceId, Pointer contextId);

    public native @StdVector float[] executeMatrixMultiplication(Pointer contextId, Pointer commands, Pointer kernelId,
        @StdString String deviceName, int ncols, @StdVector float[] vector1, @StdVector float[] vector2);

    @Namespace("")
    @Name("std::vector<std::string>")
    public static class StringVector extends Pointer {
        /**
         * Pointer cast constructor. Invokes {@link Pointer#Pointer(Pointer)}.
         */
        public StringVector(Pointer p) {
            super(p);
        }

        public StringVector(BytePointer... array) {
            this(array.length);
            put(array);
        }

        public StringVector(String... array) {
            this(array.length);
            put(array);
        }

        public StringVector() {
            allocate();
        }

        public StringVector(long n) {
            allocate(n);
        }

        private native void allocate();

        private native void allocate(@Cast("size_t") long n);

        public native @Name("operator=") @ByRef StringVector put(@ByRef StringVector x);

        public native long size();

        public native void resize(@Cast("size_t") long n);

        public native @Index @StdString BytePointer get(@Cast("size_t") long i);

        public native StringVector put(@Cast("size_t") long i, BytePointer value);

        public native @ValueSetter @Index StringVector put(@Cast("size_t") long i, @StdString String value);

        public StringVector put(BytePointer... array) {
            if (size() != array.length) {
                resize(array.length);
            }
            for (int i = 0; i < array.length; i++) {
                put(i, array[i]);
            }
            return this;
        }

        public StringVector put(String... array) {
            if (size() != array.length) {
                resize(array.length);
            }
            for (int i = 0; i < array.length; i++) {
                put(i, array[i]);
            }
            return this;
        }
    }
}
