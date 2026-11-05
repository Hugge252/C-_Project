start the c++ application and open the unity project, once the camera has identified a marker, the object in unity will match it's rotation. there is a tick-box on the object "cubeToManage" in unity that lets you choose weather to isolate rotations around the y-axis.

required versions of OpenCV and other libraries;
cmake version 4.3.1-msvc1
git version 2.53.0.windows.4

complete configuration, build and run commands;

cmake --build build
./build/camera_grab

.\build\Debug\camera_grab.exe


camera, calibration, marker and resource requirements;
working camera, marker from aruco, good lightning

instructions for reproducing the principal demonstration.