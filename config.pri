
unix{
PROJECT_LIBS = $${PWD}/libs/unix
}

macx{
PROJECT_LIBS = $${PWD}/libs/osx
}

win32{
PROJECT_LIBS = $${PWD}/libs/win
}

android{
    config += ANDROID_NDK_PLATFORM=android-26
    PROJECT_LIBS = $${PWD}/libs/android
}






