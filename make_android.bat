

prompt $$
cd android

call android update project -t android-18 -p . -n Hollywood_Hunt
call ndk-build

@echo RMDIR /S /Q  bin src
copy jni\libsndfile.so  libs\armeabi\libsndfile.so

call ant debug

adb install -r bin\*debug.apk
cd ..
pause
