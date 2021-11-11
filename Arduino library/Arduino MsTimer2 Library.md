# Arduino MsTimer2 Library



## 简介

a small and very easy to use library to interface Timer2 with humans. It's called MsTimer2 because it "hardcodes" a resolution of 1 millisecond on timer2.

## 函数

### set

#### 简介

这个函数为溢出设置一个毫秒的时间。每次溢出时，“f”将被调用。"f"必须不带参数声明为void。

#### 函数

```c++
MsTimer2::set(unsigned long ms, void (*f)());
```

#### 参数

ms : 每一次溢出的时间（ms）

f : 每一次溢出后调用的函数

#### 返回值

无



### start

#### 简介

使能定时器中断

#### 函数原型

```c++
MsTimer2::start();
```

#### 参数

无

#### 返回值

无





### Stop

#### 简介

禁用定时器中断

#### 函数原型

```c++
MsTimer2::stop();
```

#### 参数

无

#### 返回值

无