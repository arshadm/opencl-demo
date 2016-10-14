#/usr/bin/env bash

JAVACPP=$HOME/.m2/repository/org/bytedeco/javacpp/1.2.5-SNAPSHOT/javacpp-1.2.5-SNAPSHOT.jar

sbt assembly

cd target/scala-2.11/classes
java -jar $JAVACPP io.spinor.NativeLibrary
java -cp $JAVACPP:. io.spinor.NativeLibrary
