# ImageFlippingHttpServer
Description:
HTTP service that accepts requests to mirror images horizontally. The image comes in JPEG format in the body of the request, the response is returned in the same way.

To build:
cd ./ImageFlippingHttpServer/build/
cmake ..
make

Usage example:
cd ./ImageFlippingHttpServer/build/
./ImageFlippingHttpServer 127.0.0.1 5557
Runs server.

sh ./ImageFlippingHttpServer/script.sh ./ImageFlippingHttpServer/testme 127.0.0.1 5557
Mirrored copies will appear in /testme with "_mirr" added to the original name.

