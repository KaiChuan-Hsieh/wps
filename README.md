# wps
QT GUI application for WPS certification

The project is created by QT Creator. Building the project will get a binary wps.
You can get a simple UI for WPS operation.
To start the application simply use ./wps with root permission.
The default interface is wlan0 if no interface parameter being assigned.
Note that the project can only be used on Linux x86_64 platform.

Build:<br/>
$ sudo cp libwpa_client.so /usr/local/lib<br/>
$ qmake<br/>
$ make
