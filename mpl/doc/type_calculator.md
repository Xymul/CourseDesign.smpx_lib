# type_calculator.h Documentation

## 功能

主要实现编译期的类型的一元谓词计算，利用重载运算符达到类似语法糖的效果。

## 语法

使用模板变量`mpl::type<T>`与其他该类的模板变量进行任意结合的*逻辑与*或*逻辑或*运算，
返回的对象可传入`mpl::pred<T>(v)`这一函数中，其中T为一元谓词，v为返回对象或`mpl::type<T>`自身。

例如，我们想要计算：
```c++
std::is_integral_v<int> && std::is_integral_v<char> || std::is_integral_v<double>
```

就可以：
```c++
pred<std::is_integral>(type<int> && type<char> || type<double>);
```

## 更新方向

对一个类型组内的某些类型进行单独约束
```c++
pred<is_xxx>(type<T1> | require<is_yyy> && type<T2>);
```
等价于
```c++
( (is_zzz<T1>) && (is_xxx<T1> && is_xxx<T2>) );
```


