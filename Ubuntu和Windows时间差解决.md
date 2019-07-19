# 解决windows Ubuntu 时间差问题

让Ubuntu不使用UTC时间，从而与Windows保持一致。
关闭ubuntu系统UTC时间：
`sudo timedatectl set-local-rtc 1`
