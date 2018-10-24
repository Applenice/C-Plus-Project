#### 下载protobuf的CPP源码,依次执行

$sudo apt-get install autoconf automake libtool curl make g++ unzip

$ ./configure

$ make

$ make check

$ sudo make install

$ sudo ldconfig       # refresh shared library cache.

#### 程序处理：
参照

https://developers.google.com/protocol-buffers/docs/cpptutorial  进行操作

#### 当proto文件写完时

protoc -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/addressbook.proto

#### 具体例：

protoc -I=../protobuf_test/ --cpp_out=../protobuf_test ../protobuf_test/addressbook.proto

$ c++ writing.cc addressbook.pb.cc `pkg-config --cflags --libs protobuf`

$ c++ reading.cc addressbook.pb.cc `pkg-config --cflags --libs protobuf`

或

$ c++ writing.cc addressbook.pb.cc -o write `pkg-config --cflags --libs protobuf`

$ c++ reading.cc addressbook.pb.cc -o write `pkg-config --cflags --libs protobuf`
