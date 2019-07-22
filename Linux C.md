## 7.1结构体

- 无论是定义一个结构体类型，还是在结尾声明变量均需要加`;`

```c
struct complex_struct {
	double x, y;
};

struct complex_struct {
	double x, y;
} z1, z2;
```

- 结构体变量的初始化

  `struct complex_struct z = { 3.0, 4.0 };`

  ```c
  double x = 3.0;
  struct complex_struct z1 = { x, 4.0, }; /* z1.x=3.0, z1.y=4.0 */
  struct complex_struct z2 = { 3.0, }; /* z2.x=3.0, z2.y=0.0 */
  struct complex_struct z3 = { 0 }; /* z3.x=0.0, z3.y=0.0 */
  struct complex_struct z1 = { .y = 4.0 }; /* z1.x=0.0, z1.y=4.0 */
  ```

- 结构体赋值是深拷贝，无论是初始化，还是后续赋值

