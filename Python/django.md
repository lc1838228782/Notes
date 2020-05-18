# django

创建项目：创建虚拟环境 ` django-admin startproject mysite `

运行服务器：` Python manage.py runserver `

创建应用：` Python manage.py startapp polls `

编写视图：polls/views.py

```python
from django.http import HttpResponse


def index(request):
    return HttpResponse("Hello, world. You're at the polls index.")
```

URL映射：

创建polls/urls.py

```python
from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='index'),
]
```

修改mysite/urls.py

```python
from django.contrib import admin
from django.urls import include, path

urlpatterns = [
    path('polls/', include('polls.urls')),
    path('admin/', admin.site.urls),
]
```

path的四个参数：

` route` : 

`view` :

` kwargs ` :任意个关键字参数可以作为一个字典传递给目标视图函数。本教程中不会使用这一特性.

` name `:为你的 URL 取名能使你在 Django 的任意地方唯一地引用它，尤其是在模板中。这个有用的特性允许你只改一个文件就能全局地修改某个 URL 模式。**存在疑问**

创建表：`python manage.py migrate `

添加应用到settings：

```python
INSTALLED_APPS = [
    'polls.apps.PollsConfig',
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
]
```

储存为一次迁移：

`python manage.py makemigrations polls `

将迁移同步到数据库(并没有。。只是输出相关命令）：

`python manage.py sqlmigrate polls 0001`

改变models之后的应用操作：

- 编辑 `models.py` 文件，改变模型。
- 运行 `python manage.py makemigrations`为模型的改变生成迁移文件。
- 运行 [`python manage.py migrate`](https://docs.djangoproject.com/zh-hans/2.2/ref/django-admin/#django-admin-migrate) 来应用数据库迁移。

