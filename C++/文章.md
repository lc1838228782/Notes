**[元编程 好文!!](https://zhuanlan.zhihu.com/p/87917516)**

[c++ for_each 用法](https://blog.csdn.net/u014613043/article/details/50619254)

[bind1st bind2nd的使用](https://blog.csdn.net/simahao/article/details/405455)

[C++中的new、operator new与placement new](https://www.cnblogs.com/luxiaoxun/archive/2012/08/10/2631812.html)

[C++ explicit关键字详解 - 矮油~ - 博客园 (cnblogs.com)](https://www.cnblogs.com/rednodel/p/9299251.html)

[知无涯之C++ typename的起源与用法 (feihu.me)](https://feihu.me/blog/2014/the-origin-and-usage-of-typename/) 

[c++ - Where and why do I have to put the "template" and "typename" keywords? - Stack Overflow](https://stackoverflow.com/questions/610245/where-and-why-do-i-have-to-put-the-template-and-typename-keywords/613132#613132)

[C++ trivial和non-trivial构造函数及POD类型](https://blog.csdn.net/a627088424/article/details/48595525)

[C++ public、protected 、 private和friend（最通俗易懂）](https://blog.csdn.net/a3192048/article/details/82191795)

[C++友元函数和友元类用法详解](https://blog.csdn.net/fanyun_01/article/details/79122916)

异常处理 [C++ 异常处理](https://www.runoob.com/cplusplus/cpp-exceptions-handling.html)         |          [C++中try/catch/throw的使用](https://blog.csdn.net/fengbingchun/article/details/65939258)

[lambda表达式  Microsoft Doc](https://docs.microsoft.com/zh-cn/cpp/cpp/lambda-expressions-in-cpp?view=msvc-160)

constexpr 和 const  [ref1](http://c.biancheng.net/view/7807.html) [ref2](https://www.zhihu.com/question/35614219)

[using alias  c++11](https://blog.csdn.net/shift_wwx/article/details/78742459)

[C++11可变模版参数的妙用](https://www.cnblogs.com/qicosmos/p/4325949.html)

[C++11 右值引用](https://liam.page/2016/12/11/rvalue-reference-in-Cpp/)   [ref2_modern effective C++ item 23 推荐！](https://blog.csdn.net/boydfd/article/details/50822092)

[noexcept 关键字](https://www.cnblogs.com/sword03/p/10020344.html)

[智能指针]

几种cast

拷贝构造函数 复制构造函数 复制赋值运算符

mutable

[SFINAE](https://izualzhy.cn/SFINAE-and-enable_if)     [ref2](https://jguegant.github.io/blogs/tech/sfinae-introduction.html)

```
template <typename T, typename = void>   // https://izualzhy.cn/SFINAE-and-enable_if

//第二个参数通过enable_if指定只有存在成员函数logid下才有效
template <class T, class = typename std::enable_if<has_member_function_logid<T, void>::value>::type>
void dohana(T t) {
    t.logid();
}    // 同上

template <class IIter, typename std::enable_if<
     mystl::is_input_iterator<IIter>::value, int>::type = 0>
deque(IIter first, IIter last)
{
    copy_init(first, last, iterator_category(first));
}
```





## 问答

[Global scope vs global namespace](https://stackoverflow.com/questions/10269012/global-scope-vs-global-namespace)

[What are “::operator new” and “::operator delete”?](https://stackoverflow.com/questions/10513425/what-are-operator-new-and-operator-delete)

