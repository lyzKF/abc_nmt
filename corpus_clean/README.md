# cleanse corpus

## 编译
```
cd build
cmake ..
make
```

## 测试
```
cp ./build/multi_prep ./test
cd test
./multi_prep ./ ../config.ini
```

## 
- 利用配置文件读取参数
- 基于线程池的多线程
- 封装第三方库jieba，用于中文分词


------

