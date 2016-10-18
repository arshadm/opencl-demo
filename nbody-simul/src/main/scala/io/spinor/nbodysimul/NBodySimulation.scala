package io.spinor.nbodysimul

import io.spinor.oclnative.OclNative
import io.spinor.oclnative.ocl.{Context, Device, Platform}
import org.apache.commons.io.IOUtils
import org.apache.spark.{SparkConf, SparkContext}
import org.slf4j.LoggerFactory

import scala.util.Random

/**
  * The companion object for the [[NBodySimulation]] class.
  *
  * @author A. Mahmood (arshadm@spinor.io)
  */
object NBodySimulation {
  val logger = LoggerFactory.getLogger(classOf[NBodySimulation])
  val oclNative = OclNative.getInstance()

  var devices: java.util.List[Device] = null

  def main(args: Array[String]): Unit = {
    val platforms = Platform.getPlatforms()

    // perform the matrix multiplication across each of the devices
    devices = platforms.get(0).getDevices()

    startSparkContext()
  }

  /**
    * Start the spark context, and process data.
    */
  private def startSparkContext(): Unit = {
    val conf = new SparkConf().setAppName("Matrix Multiplication").setMaster("local[2]")
    val sc = new SparkContext(conf)

    val rdd = sc.makeRDD(Array(0, 1, 2), 1)
    for (deviceNumber <- rdd) {
      matrixMultiplication(deviceNumber)
    }
  }

  /**
    * Carry out matrix multiplication using the given device.
    *
    */
  private def matrixMultiplication(deviceNumber: Int): Unit = {
    val device = devices.get(deviceNumber)
    val context = new Context(device)
    val matrixMultKernel = IOUtils.toString(getClass().getResourceAsStream("/matrixmul.cl"), "UTF-8")
    val programId = oclNative.createProgram(context.getContextId(), matrixMultKernel)

    oclNative.buildProgram(device.getDeviceId(), programId)

    val commandQueue = oclNative.createCommandQueue(device.getDeviceId, context.getContextId)
    val matrixMulKernel = oclNative.createKernel(programId, "matrixMul")

    val ncols = 1024
    val nrows = 1024
    val random = new Random()

    val matrix1 = new Array[Float](ncols * nrows)
    val matrix2 = new Array[Float](ncols * nrows)
    for (i <- 0 until nrows) {
      for (j <- 0 until ncols) {
        matrix1(i * ncols + j) = random.nextFloat()
      }
    }

    val startTime = System.currentTimeMillis()

    val matrix3 = oclNative.executeMatrixMultiplication(context.getContextId(), commandQueue, matrixMulKernel,
      device.getDeviceName(), ncols, matrix1, matrix2)

    val endTime = System.currentTimeMillis()

    logger.info("**** - " + device.getDeviceName + " - completed in " + (endTime - startTime) / 1000.0f + " seconds")
  }
}

class NBodySimulation {
}
