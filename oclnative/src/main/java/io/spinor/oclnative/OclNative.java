package io.spinor.oclnative;

import org.bytedeco.javacpp.Loader;
import org.bytedeco.javacpp.Pointer;
import org.bytedeco.javacpp.annotation.*;

import java.util.Properties;

/**
 * The Java interface to the Native OCL API.
 *
 * @author A. Mahmood (arshadm@spinor.io)
 */
@Platform(include = {"OclNative.h", "<vector>"}, compiler = "cpp11", link = "oclnative", library = "jnioclnative")
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

    public native PointerVector hello();

    /* */
    @Name("std::vector<void*>")
    @Namespace("")
    public static class PointerVector extends Pointer {
        static { Loader.load(); }
        public PointerVector()       { allocate();  }
        public PointerVector(long n) { allocate(n); }
        private native void allocate();                  // this = new vector<vector<void*> >()
        private native void allocate(long n);            // this = new vector<vector<void*> >(n)

        @Name("operator[]")
        public native Pointer get(long n);
        public native Pointer at(long n);

        public native long size();
    }
}
