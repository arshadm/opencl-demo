package io.spinor.nbodysimul

import io.spinor.oclnative.OclNative
import org.slf4j.LoggerFactory

/**
  * The companion object for the [[NBodySimulation]] class.
  *
  * @author A. Mahmood (arshadm@spinor.io)
  */
object NBodySimulation {
  val logger = LoggerFactory.getLogger(classOf[NBodySimulation])

  def main(args: Array[String]): Unit = {
    val oclNative = new OclNative()

    logger.info("Hello World: " + oclNative.hello().get(0))
  }
}

class NBodySimulation {
}
