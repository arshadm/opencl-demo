package io.spinor

import org.bytedeco.javacpp._
import org.bytedeco.javacpp.annotation._

@Platform(include = Array("../../../../../../native/NativeLibrary.h"))
@Namespace("NativeLibrary")
object NativeLibrary {

  object NativeClass {
    Loader.load()
  }

  class NativeClass extends Pointer {

    allocate()

    @native def allocate(): Unit

    // to call the getter and setter functions
    @native
    @StdString def get_property(): String

    @native def set_property(property: String): Unit

    // to access the member variable directly
    @native
    @StdString def property(): String

    @native def property(property: String): Unit
  }

  def main(args: Array[String]) {
    // Pointer objects allocated in Java get deallocated once they become unreachable,
    // but C++ destructors can still be called in a timely fashion with Pointer.deallocate()
    val nativeClass = new NativeClass()
    nativeClass.set_property("Hello World!")
    System.out.println(nativeClass.property())
  }
}
