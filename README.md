# rpi-lcd
适用于在树莓派上使用lcd屏幕

# 安装
1. clone
```
git clone https://github.com/walnutpi/rpi-lcd.git
```

2. 执行安装脚本
```
cd rpi-lcd
sudo ./install
```
3. 使自动补全生效
运行以下命令，让自动补全生效。就是输命令的时候可以按两下tab键，会把可选项给显示出来
```
source /etc/bash_completion.d/rpi-lcd
```


# 使用
```
sudo rpi-lcd [lcd-name] [function] [value]
```
支持自动补全，按两下tab键就知道当前位置有哪些可选项
- `[lcd-name]` 选择你的lcd屏
- `[function]` 要运行的功能
- `[value]` 有些功能需要再附带一些输入参数

## 启用lcd屏，并配置与hdmi同屏
```
sudo rpi-lcd lcd35-st7796 install
```

## 旋转lcd方向
对lcd的方向进行旋转。重启后生效
```
sudo rpi-lcd lcd35-st7796 set_rotate 90
```

## 移除lcd
禁用相关驱动，恢复桌面的各项设置。重启后生效
```
sudo rpi-lcd lcd35-st7796 remove
```
