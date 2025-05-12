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

如果你想对某些类型进行额外约束，例如：
```c++
std::is_class_v<A> && std::is_trivial_v<A> && std::is_trivial_v<B> || std::is_trivial_v<C>
```
可以写成
```c++
pred<std::is_trivial>(type<A> | must<is_class> && type<B> || type<C>);
```

例如：
```c++
(is_aaa<A> || is_xxx<A> || is_yyy<A>) && is_yyy<B> || is_yyy<C>
```
就等价于
```c++
pred<is_yyy>(type<A> | either<is_aaa> | either<is_xxx> && type<B> || type<C>); 
```
我们目前提供了两个操作`must<T>`和`either<T>`，`must`表示类型进行`pred`运算前所必须满足的条件，而`either`表示在该类型进行`pred`运算前，如果满足该条件，则pred对该类类型的运算恒为真，否则依赖于`pred`运算的结果。

严格地来讲，`pred<is_c>(type<A> | must<is_an> | either_<is_bn>)`等价于
```c++
(is_b1<A> || is_b2<A> || ... || is_bn<A>) ? true : ((is_a1<A> && is_a2<A> && ... && is_an<A>) ? is_c<A> : false)
```

## 更新方向

添加二元运算的支持
