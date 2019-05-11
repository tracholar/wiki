---
title: "Flask"
layout: page
date: 2019-04-29
---

[TOC]

## 关于
Flask <http://flask.pocoo.org/docs/1.0/>

## 快速入门

### 简单应用
```python
from flask import Flask
app = Flask(__name__)

@app.route("/")
def hello():
    return "Hello World!"
```

- 运行命令 `export FLASK_APP=hello.py flask run`
- 外网可见 `flask run --host=0.0.0.0`
- Debug模式 `export FLASK_ENV=development`

### 路由
- 使用 `route()` 装饰, `@app.route('/')`, `@app.route('/hello')`
- 变量规则, `<variable_name>` 或者指定转化类型 `<converter:variable_name>`

```python
@app.route('/user/<username>')
def show_user_profile(username):
    # show the user profile for that user
    return 'User %s' % username

@app.route('/post/<int:post_id>')
def show_post(post_id):
    # show the post with the given id, the id is an integer
    return 'Post %d' % post_id

@app.route('/path/<path:subpath>')
def show_subpath(subpath):
    # show the subpath after /path/
    return 'Subpath %s' % subpath
```
- 转换类型包括:
    - `string` 默认类型,不包括`/`
    - `int` 整数
    - `float`
    - `path` 包括 `/`
    - `uuid` UUID?
- 路由`/projects/` 和 `/projects` 的区别: 后者可以通过前者访问,但是前者无法通过后者访问
- URL Building `url_for(func_name, **kwargs)`,  `test_request_context()` 可以用于模拟request

```python
from flask import Flask, url_for

app = Flask(__name__)

@app.route('/')
def index():
    return 'index'

@app.route('/login')
def login():
    return 'login'

@app.route('/user/<username>')
def profile(username):
    return '{}\'s profile'.format(username)

with app.test_request_context():
    print(url_for('index'))
    print(url_for('login'))
    print(url_for('login', next='/'))
    print(url_for('profile', username='John Doe'))

/
/login
/login?next=/
/user/John%20Doe
```

- HTTP Methods, 默认的路由只响应GET请求, 可以通过`methods`参数指定其他请求, `@app.route('/login', methods=['GET', 'POST'])`

```python
from flask import request

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        return do_the_login()
    else:
        return show_the_login_form()
```

- Static Files, `static` 目录用于放置静态文件, `url_for('static', filename='style.css')`
- 模板, 使用jinja2模板引擎, `render_template()`,模板目录 `templates/`,[模板引擎参考](http://jinja.pocoo.org/docs/templates)

```python
from flask import render_template

@app.route('/hello/')
@app.route('/hello/<name>')
def hello(name=None):
    return render_template('hello.html', name=name)
```

- 在模板内部, 可以访问的对象: `request`, `session`, [`g`](http://flask.pocoo.org/docs/1.0/api/#flask.g) 和函数`get_flashed_messages()`
- 模板中的变量会自动转移HTML特殊字符, 如果不想转义,可以使用 `Markup()` 或者 `|safe` filter
- 访问`request`数据: 通过全局对象 [`request`](http://flask.pocoo.org/docs/1.0/api/#flask.Request) 实现
    - `method` 请求方法 `'GET','POST'`
    - `form` 表单数据字典
    - `args` GET参数字典
    
```python
from flask import request
searchword = request.args.get('key', '')
```

- 文件上传, HTML中指定form编码 `enctype="multipart/form-data"`, 通过 `request.files` 字典访问文件,key是文件名, value是`file`对象,可以通过`.save()`方法保存文件

```python
from flask import request
from werkzeug.utils import secure_filename

@app.route('/upload', methods=['GET', 'POST'])
def upload_file():
    if request.method == 'POST':
        f = request.files['the_file']
        f.save('/var/www/uploads/' + secure_filename(f.filename))
```
- Cookies, 
    - `response.set_cookie(key, val)` 设置cookie, `response` 对象可以通过`make_response`创建
    - `request.cookies` 字典获取cookie

```python
from flask import request

@app.route('/')
def index():
    username = request.cookies.get('username')
    # use cookies.get(key) instead of cookies[key] to not get a
    # KeyError if the cookie is missing.
    resp = make_response(render_template(...))
    resp.set_cookie('username', 'the username')
    return resp
```

- 重定向与错误, `redirect(url)`, `abort(code)`, 错误的自定义处理通过装饰器`@app.errorhandler(int code)`实现

```python
from flask import abort, redirect, url_for

@app.route('/')
def index():
    return redirect(url_for('login'))

@app.route('/login')
def login():
    abort(401)
    this_is_never_executed()

@app.errorhandler(404)
def page_not_found(error):
    return render_template('page_not_found.html'), 404
```

- Responses, 
    - 如果返回的是 `response` 对象,则直接以这个对象响应
    - 如果返回的是 `string`, 则以string为内容,其他以默认参数,构造 `response` 对象响应
    - 如果返回的是 `tuple`, 则要求格式为 `(response, status, headers)` or `(response, headers) `, `status` 是状态码, `headers` 是list或者dict

```python
@app.errorhandler(404)
def not_found(error):
    resp = make_response(render_template('error.html'), 404)
    resp.headers['X-Something'] = 'A value'
    return resp
```

- Sessions

```python
from flask import Flask, session, redirect, url_for, escape, request

app = Flask(__name__)

# Set the secret key to some random bytes. Keep this really secret!
app.secret_key = b'_5#y2L"F4Q8z\n\xec]/'

@app.route('/')
def index():
    if 'username' in session:
        return 'Logged in as %s' % escape(session['username'])
    return 'You are not logged in'

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        session['username'] = request.form['username']
        return redirect(url_for('index'))
    return '''
        <form method="post">
            <p><input type=text name=username>
            <p><input type=submit value=Login>
        </form>
    '''

@app.route('/logout')
def logout():
    # remove the username from the session if it's there
    session.pop('username', None)
    return redirect(url_for('index'))
```

秘钥生成 

```bash
$ python -c 'import os; print(os.urandom(16))'
b'_5#y2L"F4Q8z\n\xec]/'
```

- Message Flashing, 不知道干啥的
- 日志 `logging`

```python
app.logger.debug('A value for debugging')
app.logger.warning('A warning occurred (%d apples)', 42)
app.logger.error('An error occurred')
```


## 教程
- 构建一个简单的blog系统 <http://flask.pocoo.org/docs/1.0/tutorial/>
- 通过工厂方法创建应用 `create_app`, 

```bash
export FLASK_APP=flaskr:create_app
export FLASK_ENV=development
flask run
```

- 链接数据库
    - `g`是一个特殊的对象, 每一个request对应唯一一个, 用于在多个函数中共享,类似于request作用域的全局变量
    - `current_app` 也是一个特殊全局变量,指向当前app实例
    - `click.command`  是基于[Click的命令行接口](http://flask.pocoo.org/docs/1.0/cli/#cli)
    - `app.teardown_appcontext` 注册request结束后的hook
    - `app.cli.add_command()` 将新的命令注册到flask的命令行
    

```python
import sqlite3

import click
from flask import current_app, g
from flask.cli import with_appcontext


def get_db():
    if 'db' not in g:
        g.db = sqlite3.connect(
            current_app.config['DATABASE'],
            detect_types=sqlite3.PARSE_DECLTYPES
        )
        g.db.row_factory = sqlite3.Row

    return g.db


def close_db(e=None):
    db = g.pop('db', None)

    if db is not None:
        db.close()

def init_db():
    db = get_db()

    with current_app.open_resource('schema.sql') as f:
        db.executescript(f.read().decode('utf8'))


@click.command('init-db')
@with_appcontext
def init_db_command():
    """Clear the existing data and create new tables."""
    init_db()
    click.echo('Initialized the database.')

def init_app(app):
    app.teardown_appcontext(close_db)
    app.cli.add_command(init_db_command)
```

`flaskr/__init__.py`

```python
def create_app():
    app = ...
    # existing code omitted

    import db
    db.init_app(app)

    return app
```
- 初始化数据库 `flask init-db`


## Blueprints and Views

### Blueprint
[Blueprint](http://flask.pocoo.org/docs/1.0/api/#flask.Blueprint)是一组相关的view,类似于MVC中的Controller吧。Flaskr有两个Blueprint,一个是认证相关函数,一个是blog。

- `bp = Blueprint('auth', __name__, url_prefix='/auth')` 创建Blueprint `auth`
- 通过 `app.register_blueprint()` 注册blueprint

`flaskr/auth.py`

```python
@bp.route('/register', methods=('GET', 'POST'))
def register():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        db = get_db()
        error = None

        if not username:
            error = 'Username is required.'
        elif not password:
            error = 'Password is required.'
        elif db.execute(
            'SELECT id FROM user WHERE username = ?', (username,)
        ).fetchone() is not None:
            error = 'User {} is already registered.'.format(username)

        if error is None:
            db.execute(
                'INSERT INTO user (username, password) VALUES (?, ?)',
                (username, generate_password_hash(password))
            )
            db.commit()
            return redirect(url_for('auth.login'))

        flash(error)

    return render_template('auth/register.html')

@bp.route('/login', methods=('GET', 'POST'))
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        db = get_db()
        error = None
        user = db.execute(
            'SELECT * FROM user WHERE username = ?', (username,)
        ).fetchone()

        if user is None:
            error = 'Incorrect username.'
        elif not check_password_hash(user['password'], password):
            error = 'Incorrect password.'

        if error is None:
            session.clear()
            session['user_id'] = user['id']
            return redirect(url_for('index'))

        flash(error)

    return render_template('auth/login.html')
```

- `register` 说明
    - `@bp.route` 是blueprint的路由, `/register` 对应的url path为 `/auth/register`
    - `GET`请求返回注册页面, `POST`请求注册用户
    - `generate_password_hash()` 用户创建密码hash, 数据库中只保存密码的hash值

- `login` 说明
    - `check_password_hash()` 校验密码
    - `session` 登陆成功后,清空session,并设置userid,session字典的值都会通过加密的cookie存到浏览器中
    
```python
@bp.before_app_request
def load_logged_in_user():
    user_id = session.get('user_id')

    if user_id is None:
        g.user = None
    else:
        g.user = get_db().execute(
            'SELECT * FROM user WHERE id = ?', (user_id,)
        ).fetchone()
```

- `load_logged_in_user` 说明
    - 从session中查找user_id,如果没有则将`g.user`置`None`,否则查询user保存到`g.user`
- `logout` 清空session
    
```python
@bp.route('/logout')
def logout():
    session.clear()
    return redirect(url_for('index'))
```

- 在其他的view中使用用户认证模块, 通过装饰器([参考](https://foofish.net/python-decorator.html))来实现. `@login_required`, 注意`@login_required` 装饰了函数 `func` 相当于 `func = login_required(func)` 。有了这个装饰器后,就可以给需要认证的view方便的添加认证。

```python
def login_required(view):
    @functools.wraps(view)
    def wrapped_view(**kwargs):
        if g.user is None:
            return redirect(url_for('auth.login'))

        return view(**kwargs)

    return wrapped_view
```

## Templates

`flaskr/templates/base.html`

```python
<!doctype html>
<title>{% block title %}{% endblock %} - Flaskr</title>
<link rel="stylesheet" href="{{ url_for('static', filename='style.css') }}">
<nav>
  <h1>Flaskr</h1>
  <ul>
    {% if g.user %}
      <li><span>{{ g.user['username'] }}</span>
      <li><a href="{{ url_for('auth.logout') }}">Log Out</a>
    {% else %}
      <li><a href="{{ url_for('auth.register') }}">Register</a>
      <li><a href="{{ url_for('auth.login') }}">Log In</a>
    {% endif %}
  </ul>
</nav>
<section class="content">
  <header>
    {% block header %}{% endblock %}
  </header>
  {% for message in get_flashed_messages() %}
    <div class="flash">{{ message }}</div>
  {% endfor %}
  {% block content %}{% endblock %}
</section>
```

- `base` 模板说明
    - `get_flashed_messages()` 获取 `flash` 中的message
    - `block` 模块可以被继承的模板修改
        - `{% block title %}`
    - 每一个 blueprint 都在子目录中
    
`flaskr/templates/auth/register.html`

```python
{% extends 'base.html' %}

{% block header %}
  <h1>{% block title %}Register{% endblock %}</h1>
{% endblock %}

{% block content %}
  <form method="post">
    <label for="username">Username</label>
    <input name="username" id="username" required>
    <label for="password">Password</label>
    <input type="password" name="password" id="password" required>
    <input type="submit" value="Register">
  </form>
{% endblock %}
```

- `register` 模块说明
    - `extends` 模板继承
    - `block` override 每一个子块, 将 `{% block title %}` 放到 `{% block header %}`内部,可以避免title 模块定义两次
    
    
## Static Files
- flask自动添加 `static` view 作为静态文件的目录


## 项目安装
- `setup.py` 文件, `packages` 告诉python有哪些包,通过`find_packages`自动搜索包,`include_package_data`告诉python是否需要打包数据文件,`MANIFEST.in`文件告诉python数据文件有哪些

```python
from setuptools import find_packages, setup

setup(
    name='flaskr',
    version='1.0.0',
    packages=find_packages(),
    include_package_data=True,
    zip_safe=False,
    install_requires=[
        'flask',
    ],
)
```

`MANIFEST.in`

```
include flaskr/schema.sql
graft flaskr/static
graft flaskr/templates
global-exclude *.pyc
```

- 安装 `pip install -e .` `-e`表示是edit或者dev模式,可以让修改后的文件立即生效

## 测试
- 用 [pytest](https://pytest.readthedocs.io/)和[coverage](https://coverage.readthedocs.io/)测试你的代码
- 测试代码放在与flaskr平行的`tests`目录下



## flask-sqlalchemy
