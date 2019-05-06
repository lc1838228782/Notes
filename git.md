## config

`git config --global user.name hhdx`

` git config --global usr.email lc1838228782@gmail.com `

` git config --global color.ui auto `

` local ` repository

` global ` usr

` system `  system

## init

已经存在项目的目录下 ` git init `

新建项目 ` git init new-project ` 



## commit

` git add ` 暂存区

` git commit -m "descriptions" ` 提交



## diff

` git diff ` 没有` git add `

` git diff --staged `  已经  ` git add `

` git diff HEAD `  最后一次提交之后所有改变

` git diff --color-words ` 仅显示改变的地方颜色

` git diff --word-diff ` 同上

` git diff stat ` 只显示改变的地方



## log

` git log ` 最上面是最新

` git log --oneline `  一行显示一个提交

` git log --stat` 显示每次提交的文件

` git log --patch `显示减少了什么增加了什么

` git log --graph --all --decorate --oneline ` 



## remove

` git rm `  

` git add -u . ` 之前 rm删掉了文件

` git rm --cache file `  当我们需要删除`暂存区`或`分支`上的文件, 但本地又需要使用, 只是不希望这个文件被版本控制



## move

` git mv old new `           ` git rm old ` ` git add new ` 

` git add -A . `  用其他方式移动了文件，暂存缓存区



## ignore

.gitignore 文件  用#注释

directoryname/ 忽略整个文件夹

支持 正则

` git ls-files --others --ignored --exclude-standard `  显示被忽略的文件



## branch

` git branch branch-name ` 创建新分支

` git branch -d branch-name `  删除分支

` git checkout brach-name `  切换分支



## checkout

` git branch `  显示分支 型号旁边是当前分支

` git status `  也可显示

` git checkout -- filename ` 清理掉最后一次commit的内容

` git checkout -b branchname ` 创建分支并切换过去



