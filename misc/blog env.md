1. 安装Node.js 配置源`npm config set registry https://registry.npm.taobao.org`

2. 安装git
3. `npm install hexo-cli -g`
4. `npm install hexo --save`在blog文件夹中部署hexo的环境
5. `npm install`

**更换主题时，备份主题中的uploads，CNAME文件**

| 布局    | 路径             |
| :------ | :--------------- |
| `post`  | `source/_posts`  |
| `page`  | `source`         |
| `draft` | `source/_drafts` |

```shell
$ npm install hexo-generator-feed --save
$ npm install hexo-deployer-git --save
$ npm i hexo-renderer-swig   # hexo 5
```



## npm依赖更新

```shell
$ npm outdated
$ npm install -g npm-check-updates
$ ncu
$ rm node_modules # optional
$ npm install
```

