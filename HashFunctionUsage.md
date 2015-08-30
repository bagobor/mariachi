# Introduction #

Typical usage of the hash function

# Details #

```
Md5 md5;
md5.init("hello world");
std::cout << "Md5 value: " << md5 << std::endl;
```

or in amore low level way

```
Md md5;
md5.init();
md5.update((unsigned char *) "hello world", 11);
md5.finalize();
std::string &value = md5.hexdigest();
std::cout << "Md5 value: " << md5 << std::endl;
```