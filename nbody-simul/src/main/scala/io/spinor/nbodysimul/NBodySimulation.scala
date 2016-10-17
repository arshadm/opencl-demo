package io.spinor.nbodysimul

import io.spinor.oclnative.OclNative
import io.spinor.oclnative.ocl.Platform
import org.bytedeco.javacpp.{Pointer, PointerPointer}
import org.slf4j.LoggerFactory

/**
  * The companion object for the [[NBodySimulation]] class.
  *
  * @author A. Mahmood (arshadm@spinor.io)
  */
object NBodySimulation {
  val logger = LoggerFactory.getLogger(classOf[NBodySimulation])
  val oclNative = OclNative.getInstance();

  def main(args: Array[String]): Unit = {
    val platforms = Platform.getPlatforms()
    logger.info(platforms.toString);
  }

  /**
    * Display platform info.
    *
    * @param platforms the platforms
    */
  private def displayPlatformInfo(platforms: PointerPointer[_ <: Pointer]): Unit = {
    logger.info("Number of platforms: " + platforms.capacity())
    for (i <- 0 until platforms.capacity().toInt) {
      val platformInfo = oclNative.getPlatformInfo(platforms.get(i))
      logger.info("Platform info: ")
      for (j <- 0 until platformInfo.size().toInt) {
        logger.info("  " + platformInfo.get(j).getString)
      }
      logger.info("Platform Extensions: " + oclNative.getPlatformExtensions(platforms.get(i)))

      val deviceIds = oclNative.getDeviceIds(platforms.get(i))
      logger.info("Number of devices: " + deviceIds.capacity())
      for(j <- 0 until deviceIds.capacity().toInt) {
        val deviceInfo = oclNative.getDeviceInfo(deviceIds.get(j))
        logger.info("Device Info:")
        for (j <- 0 until deviceInfo.size().toInt) {
          logger.info("    " + deviceInfo.get(j).getString)
        }
      }
    }
  }
}

class NBodySimulation {
}
