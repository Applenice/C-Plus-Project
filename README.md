## CC-Project

1. test_gzip.cpp    g++ test_gzip.cpp -lz  gzip的压缩与解压

2. protobuf_test    学习protobuf的demo  

3. sort.cpp   同时找出最大最小值,最多3N/2次(算法导论第九章)  

4. cv_image.cpp 读取图片像素值  
https://blog.csdn.net/xiaowei_cqu/article/details/7557063  
```
g++ cv_image.cpp -o cv_image `pkg-config --libs --cflags opencv` -ldl
```

5. mnist_images 将mnist数据集中图片单独提取,需要opencv
