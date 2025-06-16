# 🔧 ESP32 Car Motor Troubleshooting Guide

## 问题: 只有一个马达在动

### 立即检查步骤

1. **访问诊断页面**
   ```
   http://esp32car.local/diagnostics
   ```

2. **逐个测试马达**
   - 点击每个马达的测试按钮
   - 观察哪些马达能正常工作
   - 检查串口监视器的输出

3. **验证新的引脚配置**
   
   **旧的有问题的引脚:**
   - Motor A: GPIO 2, 3 ❌ (GPIO 3 是保留引脚)
   - Motor B: GPIO 10, 6 ❌ (GPIO 10 在某些板子上不可用)
   
   **新的安全引脚:**
   - Motor A: GPIO 4, 5 ✅
   - Motor B: GPIO 16, 17 ✅  
   - Motor C: GPIO 18, 19 ✅
   - Motor D: GPIO 21, 22 ✅

### 硬件检查清单

- [ ] 电源连接是否正确 (5V 给 DRV8833)
- [ ] 所有杜邦线连接紧固
- [ ] DRV8833 芯片没有过热
- [ ] 马达电源线没有短路
- [ ] ESP32 板子的GPIO引脚没有损坏

### 软件诊断工具

1. **Web诊断界面**: `/diagnostics`
2. **API测试端点**:
   - `/test/motor/A` - 测试马达A
   - `/test/motor/B` - 测试马达B  
   - `/test/motor/C` - 测试马达C
   - `/test/motor/D` - 测试马达D
   - `/test/motor/all` - 测试所有马达

3. **状态检查**: `/status` - 获取系统状态

### 常见问题解决方案

#### 问题1: GPIO引脚冲突
**症状**: 某些马达完全不动
**解决**: 使用更新后的引脚配置，重新上传代码

#### 问题2: DRV8833功率不足  
**症状**: 马达启动无力或间歇性工作
**解决**: 
- 检查5V电源供应是否充足
- 确保电源线足够粗
- 检查DRV8833的电源指示灯

#### 问题3: PWM通道冲突
**症状**: 马达行为异常
**解决**: 代码已更新使用独立的PWM通道 (0-7)

#### 问题4: 马达接线错误
**症状**: 马达转向不正确
**解决**: 
- 交换马达的两根线
- 或在代码中调整方向逻辑

### 调试命令

在浏览器中尝试这些URL:

```
# 测试单个马达
http://esp32car.local/test/motor/A
http://esp32car.local/test/motor/B  
http://esp32car.local/test/motor/C
http://esp32car.local/test/motor/D

# 测试所有马达 (需要约30秒)
http://esp32car.local/test/motor/all

# 获取系统状态
http://esp32car.local/status
```

### 如果问题仍然存在

1. 检查串口监视器的输出
2. 使用万用表测试GPIO引脚的输出电压
3. 尝试单独测试DRV8833芯片
4. 考虑更换跳线或DRV8833模块

记住: 新的引脚配置避免了ESP32启动时的冲突，应该能解决大部分问题！
