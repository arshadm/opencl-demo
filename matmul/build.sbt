
lazy val commonSettings = Seq(
  organization := "io.spinor",
  version := "0.0.1-SNAPSHOT",
  scalaVersion := "2.11.8"
)

lazy val root = (project in file(".")).
  settings(commonSettings: _*).
  settings(
    name := "matmul",
    libraryDependencies += "org.bytedeco" % "javacpp" % "1.2.4",
    mainClass in (Compile, packageBin) := Some("io.spinor.NativeLibrary"),

    // set the main class for the main 'run' task
    // change Compile to Test to set it for 'test:run'
    mainClass in (Compile, run) := Some("io.spinor.NativeLibrary"),

    mainClass in assembly := Some("io.spinor.NativeLibrary")
  )
