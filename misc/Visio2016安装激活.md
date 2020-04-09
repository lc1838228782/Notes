**安装Visio 2016会导致office 2019被卸载。**

Visio 2016存在安装office 2019可以。

**批量授权版：Office 2016 Visio 专业版 64位版**
**文件名：**SW_DVD5_Visio_Pro_2016_64Bit_ChnSimp_MLF_X20-42759.ISO
**文件大小：**714913792 字节
**MD5:** 93BEB874F5A5870D5854519856047103
**SHA1:** 71E082174812F748AB1A70CA33E6004E1E1AACA8
**CRC32:** F813794B
**下载地址：**

```
ed2k://|file|SW_DVD5_Visio_Pro_2016_64Bit_ChnSimp_MLF_X20-42759.ISO|714913792|FC930AB97B366B3595FC2F28ABAC2A6F|/
```

**批量授权版：Visio2016 32位专业增强版下载**

文件名：SW_DVD5_Visio_Pro_2016_W32_ChnSimp_MLF_X20-41580.ISO

文件大小：609447936 字节

MD5: 96E008B110F308F1E424D11964D82CE0

SHA1: 780046411EB18874AA2DA7E4A11322557EB00D92

下载地址：ed2k://|file|SW_DVD5_Visio_Pro_2016_W32_ChnSimp_MLF_X20-41580.ISO|609447936|91EB248558F236AA66D234EA03FAD9A9|/

```
以管理员模式运行CMD.以下命令都是在命令行里运行.
进入OFFICE激活脚步所在的安装目录: cd "C:\Program Files\Microsoft Office\Office16"
						cd "C:\Program Files (x86)\Microsoft Office\Office16"
设置KMS地址: cscript ospp.vbs /sethst:kms.idina.cn
激活: cscript ospp.vbs /act
目前可用KMS地址有:

1. kms.03k.org
2. http://kms.luody.info

3. http://kms.idina.cn
```





### 零售(msdn itell you)改批量

```
@ECHO OFF&PUSHD %~DP0
 
setlocal EnableDelayedExpansion&color 3e & cd /d "%~dp0"
title office2016 retail转换vol版
 
%1 %2
mshta vbscript:createobject("shell.application").shellexecute("%~s0","goto :runas","","runas",1)(window.close)&goto :eof
:runas
 
if exist "%ProgramFiles%\Microsoft Office\Office16\ospp.vbs" cd /d "%ProgramFiles%\Microsoft Office\Office16"
if exist "%ProgramFiles(x86)%\Microsoft Office\Office16\ospp.vbs" cd /d "%ProgramFiles(x86)%\Microsoft Office\Office16"
 
:WH
cls
echo.
echo                         选择需要转化的office版本序号
echo.
echo --------------------------------------------------------------------------------                                                         
echo                 1. 零售版 Office Pro Plus 2016 转化为VOL版
echo.
echo                 2. 零售版 Office Visio Pro 2016 转化为VOL版
echo.
echo                 3. 零售版 Office Project Pro 2016 转化为VOL版
echo.
echo. --------------------------------------------------------------------------------
                                                        
set /p tsk="请输入需要转化的office版本序号【回车】确认（1-3）: "
if not defined tsk goto:err
if %tsk%==1 goto:1
if %tsk%==2 goto:2
if %tsk%==3 goto:3
 
:err
goto:WH
 
:1
cls
 
echo 正在重置Office2016零售激活...
cscript ospp.vbs /rearm
 
echo 正在安装 KMS 许可证...
for /f %%x in ('dir /b ..\root\Licenses16\proplusvl_kms*.xrm-ms') do cscript ospp.vbs /inslic:"..\root\Licenses16\%%x" >nul
 
echo 正在安装 MAK 许可证...
for /f %%x in ('dir /b ..\root\Licenses16\proplusvl_mak*.xrm-ms') do cscript ospp.vbs /inslic:"..\root\Licenses16\%%x" >nul
 
echo 正在安装 KMS 密钥...
cscript ospp.vbs /inpkey:XQNVK-8JYDB-WJ9W3-YJ8YR-WFG99
 
goto :e
 
:2
cls
 
echo 正在重置Visio2016零售激活...
cscript ospp.vbs /rearm
 
echo 正在安装 KMS 许可证...
for /f %%x in ('dir /b ..\root\Licenses16\visio???vl_kms*.xrm-ms') do cscript ospp.vbs /inslic:"..\root\Licenses16\%%x" >nul
 
echo 正在安装 MAK 许可证...
for /f %%x in ('dir /b ..\root\Licenses16\visio???vl_mak*.xrm-ms') do cscript ospp.vbs /inslic:"..\root\Licenses16\%%x" >nul
 
echo 正在安装 KMS 密钥...
cscript ospp.vbs /inpkey:PD3PC-RHNGV-FXJ29-8JK7D-RJRJK
 
goto :e
 
:3
cls
 
echo 正在重置Project2016零售激活...
cscript ospp.vbs /rearm
 
echo 正在安装 KMS 许可证...
for /f %%x in ('dir /b ..\root\Licenses16\project???vl_kms*.xrm-ms') do cscript ospp.vbs /inslic:"..\root\Licenses16\%%x" >nul
 
echo 正在安装 MAK 许可证...
for /f %%x in ('dir /b ..\root\Licenses16\project???vl_mak*.xrm-ms') do cscript ospp.vbs /inslic:"..\root\Licenses16\%%x" >nul
 
echo 正在安装 KMS 密钥...
cscript ospp.vbs /inpkey:YG9NW-3K39V-2T3HJ-93F3Q-G83KT
 
goto :e
 
:e
echo.
echo 转化完成，按任意键退出！
pause >nul
exit
```