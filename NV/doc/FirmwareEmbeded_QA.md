# 低级固件和嵌入式系统面试题集

## 目录
1. [基础概念题](#基础概念题)
2. [位操作与二进制运算](#位操作与二进制运算)
3. [内存管理](#内存管理)
4. [实时操作系统(RTOS)](#实时操作系统)
5. [中断处理](#中断处理)
6. [硬件接口与寄存器操作](#硬件接口与寄存器操作)
7. [并发与同步](#并发与同步)
8. [性能优化](#性能优化)
9. [调试与测试](#调试与测试)

---

## 基础概念题

### 1. C语言基础概念

**Q1: volatile关键字的作用是什么？什么时候必须使用？**

**A1:** 
volatile关键字告诉编译器该变量可能会被程序外部因素改变，禁止编译器对其进行优化。

**必须使用的场景：**
- 硬件寄存器访问
- 中断服务程序中修改的全局变量
- 多线程共享的变量
- 内存映射的I/O端口

**示例：**
```c
volatile uint32_t *GPIO_REG = (uint32_t*)0x40020000;
volatile int interrupt_flag = 0;  // 中断中会修改
```

**Q2: const关键字有哪些用法？在嵌入式系统中的意义？**

**A2:**
const关键字表示只读，有多种用法：

**用法分类：**
- `const int a` - 常量整数
- `const int *p` - 指向常量的指针
- `int * const p` - 常量指针
- `const int * const p` - 指向常量的常量指针

**嵌入式意义：**
- 将数据存储在Flash而不是RAM中
- 编译时优化和错误检查
- 提高代码安全性

**Q3: static关键字在嵌入式C中的作用？**

**A3:**
static关键字有两个主要作用：

**1. 限制作用域（文件级别）：**
```c
static int private_var;  // 只在当前文件可见
static void private_func(void);  // 只在当前文件可见
```

**2. 保持变量状态（函数级别）：**
```c
int counter(void) {
    static int count = 0;  // 函数调用间保持值
    return ++count;
}
```

**嵌入式优势：**
- 减少全局命名空间污染
- 编译器优化更好
- 降低RAM使用（局部静态变量）

### 2. 数据类型与内存布局

**Q4: 在32位ARM系统中，各种数据类型占用多少字节？**

**A4:**
标准数据类型大小：
- `char`: 1字节
- `short`: 2字节  
- `int`: 4字节
- `long`: 4字节（32位系统）
- `long long`: 8字节
- `float`: 4字节
- `double`: 8字节
- `指针`: 4字节（32位系统）

**Q5: 什么是内存对齐？为什么需要内存对齐？**

**A5:**
内存对齐是指数据在内存中按照特定边界存储。

**对齐规则：**
- char: 1字节对齐
- short: 2字节对齐
- int: 4字节对齐
- double: 8字节对齐

**需要对齐的原因：**
- 提高CPU访问效率
- 某些处理器要求必须对齐
- 避免硬件异常

**示例：**
```c
struct {
    char a;    // 偏移0，占1字节
    // 填充3字节
    int b;     // 偏移4，占4字节
    char c;    // 偏移8，占1字节
    // 填充3字节，总大小12字节
} example;
```

**Q6: 大端序和小端序的区别？如何检测？**

**A6:**
**定义：**
- 大端序（Big Endian）：高字节存储在低地址
- 小端序（Little Endian）：低字节存储在低地址

**检测方法：**
```c
int check_endian(void) {
    int num = 1;
    char *p = (char*)&num;
    return (*p == 1) ? 0 : 1;  // 0=小端，1=大端
}
```

**实际意义：**
- 网络传输（网络字节序是大端）
- 文件格式兼容性
- 跨平台数据交换

### 3. 指针与内存

**Q7: 野指针、悬空指针、空指针的区别？**

**A7:**
**野指针（Wild Pointer）：**
- 未初始化的指针
- 指向未知内存区域
- 最危险的指针类型

**悬空指针（Dangling Pointer）：**
- 指向已释放的内存
- 曾经有效，现在无效

**空指针（Null Pointer）：**
- 指向地址0的指针
- 明确表示不指向任何对象

**防范措施：**
```c
char *p = NULL;        // 初始化为空
p = malloc(100);       // 分配内存
free(p);               // 释放内存
p = NULL;              // 避免悬空指针
```

**Q8: 函数指针的用途和声明方式？**

**A8:**
**声明方式：**
```c
// 声明函数指针类型
typedef int (*FuncPtr)(int, int);

// 声明函数指针变量
int (*operation)(int, int);

// 函数指针数组
int (*operations[])(int, int) = {add, subtract, multiply};
```

**用途：**
- 回调函数
- 状态机实现
- 算法策略选择
- 中断向量表

### 4. 嵌入式系统基础

**Q9: 什么是看门狗？工作原理是什么？**

**A9:**
**定义：**
看门狗是一种硬件定时器，用于检测系统是否正常运行。

**工作原理：**
1. 系统启动后，看门狗开始倒计时
2. 程序必须在超时前"喂狗"（重置计数器）
3. 如果超时未喂狗，系统自动复位

**类型：**
- 独立看门狗（IWDG）：完全独立的时钟源
- 窗口看门狗（WWDG）：有时间窗口限制

**应用场景：**
- 防止程序死循环
- 检测系统崩溃
- 自动恢复系统

**Q10: 什么是DMA？有什么优势？**

**A10:**
**定义：**
DMA（Direct Memory Access）是一种数据传输方式，允许硬件设备直接访问内存，无需CPU干预。

**优势：**
- 减少CPU负载
- 提高数据传输效率
- 降低功耗
- 支持后台传输

**典型应用：**
- ADC数据采集
- 串口数据传输
- 音频数据处理
- 显示缓冲区更新

**Q11: 中断和轮询的区别？各自适用场景？**

**A11:**
**中断方式：**
- 事件驱动，CPU被动响应
- 实时性好，延迟低
- 开销相对较大
- 适合突发性、实时性要求高的事件

**轮询方式：**
- CPU主动查询，周期性检测
- 实时性差，但可预测
- 开销小，实现简单
- 适合频繁发生、实时性要求不高的事件

**选择原则：**
- 实时性要求高：选择中断
- 事件频繁发生：选择轮询
- 系统资源有限：权衡开销

**Q12: 什么是实时系统？硬实时和软实时的区别？**

**A12:**
**实时系统定义：**
能够在规定时间内完成特定任务的系统，正确性不仅取决于逻辑结果，还取决于时间。

**硬实时系统：**
- 必须在截止时间内完成任务
- 超时会导致系统失效或灾难
- 例如：飞行控制、核电站控制

**软实时系统：**
- 尽量在截止时间内完成
- 偶尔超时可以容忍
- 例如：音视频播放、游戏

**关键指标：**
- 响应时间
- 吞吐量  
- 确定性
- 可靠性

### 5. 通信协议

**Q13: SPI和I2C协议的区别？**

**A13:**
**SPI (Serial Peripheral Interface)：**
- 4线制：SCLK、MOSI、MISO、SS
- 全双工通信
- 速度快（可达几十MHz）
- 主从模式，支持多从设备
- 没有地址概念

**I2C (Inter-Integrated Circuit)：**
- 2线制：SDA、SCL
- 半双工通信
- 速度较慢（标准100kHz，快速400kHz）
- 多主多从，设备有地址
- 支持热插拔

**选择依据：**
- 速度要求高：选SPI
- 节省引脚：选I2C
- 设备较多：选I2C
- 简单通信：选SPI

**Q14: UART通信的特点和参数设置？**

**A14:**
**特点：**
- 异步串行通信
- 点对点连接
- 全双工
- 不需要时钟信号

**关键参数：**
- 波特率（Baud Rate）：每秒传输的符号数
- 数据位（Data Bits）：通常8位
- 停止位（Stop Bits）：1位或2位
- 奇偶校验（Parity）：无、奇校验、偶校验
- 流控制：无、硬件、软件

**常见配置：**
- 9600-8-N-1：9600波特率，8数据位，无校验，1停止位

### 6. 功耗管理

**Q15: 嵌入式系统有哪些低功耗技术？**

**A15:**
**硬件层面：**
- 时钟门控：关闭不用的时钟
- 电源门控：关闭不用的电源域
- 电压频率调节：DVFS技术
- 睡眠模式：Sleep、Stop、Standby

**软件层面：**
- 任务调度优化
- 中断驱动设计
- DMA使用
- 外设按需开启

**系统级优化：**
- 选择低功耗器件
- 优化PCB设计
- 合理的唤醒策略

**Q16: 什么是电源管理单元（PMU）？**

**A16:**
**定义：**
PMU是负责管理系统电源的专用硬件模块。

**主要功能：**
- 电压调节
- 时钟管理
- 功耗模式切换
- 电源监控
- 复位控制

**工作模式：**
- 正常模式：全功能运行
- 节能模式：降低频率和电压
- 睡眠模式：保持最小功能
- 关机模式：几乎无功耗

### 7. 系统启动与引导

**Q17: 嵌入式系统的启动流程是什么？**

**A17:**
**典型启动流程：**
1. **硬件复位**：系统上电或复位
2. **BootROM**：固化在芯片内的启动代码
3. **Bootloader**：初始化硬件，加载系统
4. **内核启动**：操作系统内核初始化
5. **用户程序**：应用程序开始运行

**各阶段任务：**
- BootROM：基本硬件初始化，寻找Bootloader
- Bootloader：内存初始化、外设配置、系统加载
- 内核：调度器、内存管理、驱动加载
- 应用：用户功能实现

**Q18: 什么是Bootloader？有什么作用？**

**A18:**
**定义：**
Bootloader是系统启动时运行的第一个软件程序。

**主要作用：**
- 硬件初始化（时钟、内存、外设）
- 加载操作系统或应用程序
- 提供系统升级功能
- 故障恢复和诊断

**常见类型：**
- U-Boot：通用引导加载程序
- 厂商专用Bootloader
- 简化的应用Bootloader

**关键特性：**
- 体积小，启动快
- 可靠性高
- 支持多种启动方式

### 8. 调试与开发工具

**Q19: 常用的嵌入式调试方法有哪些？**

**A19:**
**硬件调试：**
- JTAG/SWD：在线调试和编程
- 逻辑分析仪：分析数字信号
- 示波器：观察模拟和数字波形
- 万用表：基本电气测量

**软件调试：**
- GDB调试器：源码级调试
- Printf调试：串口输出信息
- LED指示：简单状态显示
- 断言（Assert）：运行时检查

**系统级调试：**
- 性能分析工具
- 内存泄漏检测
- 代码覆盖率分析
- 静态代码分析

**Q20: 什么是JTAG？有什么用途？**

**A20:**
**定义：**
JTAG（Joint Test Action Group）是一种标准的测试和调试接口。

**主要用途：**
- 在线调试：设置断点、单步执行
- 程序下载：将程序烧录到目标设备
- 边界扫描：测试芯片引脚连接
- 系统测试：检测硬件故障

**信号线：**
- TDI：测试数据输入
- TDO：测试数据输出  
- TCK：测试时钟
- TMS：测试模式选择
- TRST：测试复位（可选）

**优势：**
- 非侵入式调试
- 支持多设备链接
- 标准化接口

### 9. 存储器技术

**Q21: Flash存储器和EEPROM的区别？**

**A21:**
**Flash存储器：**
- 块擦除：必须按块（通常64KB）擦除
- 擦写次数：通常10万次
- 速度快：读取速度快，擦写相对较慢
- 成本低：单位成本较低
- 适用：程序存储、大量数据存储

**EEPROM：**
- 字节擦除：可以按字节擦除和写入
- 擦写次数：通常100万次
- 速度慢：读写速度较慢
- 成本高：单位成本较高
- 适用：配置参数、小量数据存储

**选择依据：**
- 频繁小量更新：选择EEPROM
- 大量数据存储：选择Flash
- 成本敏感：选择Flash

**Q22: 什么是内存映射？有什么优势？**

**A22:**
**定义：**
内存映射是将外设寄存器、Flash等映射到CPU的地址空间中，使其可以像访问内存一样访问。

**典型映射区域：**
- 0x00000000-0x1FFFFFFF：Flash存储器
- 0x20000000-0x3FFFFFFF：SRAM
- 0x40000000-0x5FFFFFFF：外设寄存器
- 0xE0000000-0xFFFFFFFF：系统区域

**优势：**
- 统一的访问方式
- 简化编程模型
- 支持DMA直接访问
- 便于调试和监控

### 10. 中断系统深入

**Q23: 中断优先级如何设置？什么是中断嵌套？**

**A23:**
**中断优先级：**
- 抢占优先级：高优先级可以打断低优先级
- 子优先级：同抢占优先级内的排序
- NVIC支持最多256个中断优先级

**中断嵌套：**
- 高优先级中断可以打断低优先级中断
- 需要合理设置优先级避免问题
- 嵌套深度有限制

**设置原则：**
- 实时性要求高的设置高优先级
- 执行时间长的设置低优先级
- 避免优先级反转

**Q24: 什么是中断延迟？如何减少中断延迟？**

**A24:**
**中断延迟组成：**
1. 硬件响应延迟：CPU识别中断信号
2. 上下文切换延迟：保存/恢复寄存器
3. 中断服务延迟：ISR执行时间
4. 调度延迟：返回用户程序

**减少延迟的方法：**
- 关键中断设置高优先级
- ISR尽量短小精悍
- 使用中断嵌套
- 优化上下文切换
- 减少关中断时间

### 11. 时钟与定时器

**Q25: 系统时钟树的作用是什么？**

**A25:**
**定义：**
时钟树是从时钟源到各个功能模块的时钟分配网络。

**主要组成：**
- 时钟源：晶振、RC振荡器、PLL
- 时钟分频器：产生不同频率
- 时钟门控：控制时钟开关
- 时钟多路选择器：选择时钟源

**作用：**
- 为各模块提供合适频率的时钟
- 实现功耗管理
- 保证时钟域同步
- 支持动态频率调节

**Q26: 定时器有哪些工作模式？**

**A26:**
**基本模式：**
- 计数模式：向上计数、向下计数
- 比较模式：输出比较、输入捕获
- PWM模式：脉宽调制输出
- 编码器模式：正交编码器接口

**高级功能：**
- 死区时间：防止功率器件直通
- 刹车功能：紧急停止输出
- 重复计数：减少中断频率
- 主从模式：多定时器同步

### 12. 模拟电路基础

**Q27: ADC的主要参数有哪些？**

**A27:**
**分辨率：**
- 定义：ADC能分辨的最小电压变化
- 计算：LSB = VREF / 2^n（n为位数）
- 影响：决定测量精度

**转换速度：**
- 采样率：每秒转换次数
- 转换时间：单次转换所需时间
- 影响：决定能测量的信号频率

**精度参数：**
- 积分非线性（INL）
- 差分非线性（DNL）
- 偏移误差
- 增益误差

**其他参数：**
- 信噪比（SNR）
- 有效位数（ENOB）
- 输入阻抗

**Q28: 什么是PWM？有什么应用？**

**A28:**
**定义：**
PWM（脉宽调制）是一种通过改变脉冲宽度来控制平均功率的技术。

**关键参数：**
- 频率：PWM信号的重复频率
- 占空比：高电平时间占总周期的比例
- 分辨率：占空比的最小调节步长

**应用场景：**
- 电机速度控制
- LED亮度调节
- 开关电源控制
- 音频信号生成
- 模拟信号输出（配合滤波器）

### 13. 系统可靠性

**Q29: 什么是故障安全设计？有哪些方法？**

**A29:**
**定义：**
故障安全设计是指系统在出现故障时能够保持安全状态或安全停机。

**设计方法：**
- 冗余设计：多套系统并行工作
- 故障检测：及时发现系统异常
- 故障隔离：限制故障影响范围
- 故障恢复：自动或手动恢复功能

**具体措施：**
- 看门狗定时器
- 错误检测和纠正（ECC）
- 双机热备份
- 安全状态设计

**Q30: 什么是EMC？如何提高EMC性能？**

**A30:**
**定义：**
EMC（电磁兼容性）是设备在电磁环境中正常工作且不对环境产生电磁干扰的能力。

**两个方面：**
- EMI（电磁干扰）：设备对外的干扰
- EMS（电磁敏感性）：设备对外界干扰的抗性

**提高方法：**
- PCB设计：良好的地线设计、信号完整性
- 屏蔽：金属外壳、屏蔽线缆
- 滤波：电源滤波、信号滤波
- 接地：单点接地、多点接地
- 布线：减小环路面积、差分信号

### 14. 网络与通信

**Q31: TCP和UDP的区别？在嵌入式中如何选择？**

**A31:**
**TCP特点：**
- 面向连接：需要建立连接
- 可靠传输：保证数据完整性
- 流控制：防止接收方溢出
- 拥塞控制：网络自适应
- 开销大：协议复杂，资源占用多

**UDP特点：**
- 无连接：直接发送数据
- 不可靠：不保证数据到达
- 无流控制：发送方不关心接收方状态
- 开销小：协议简单，效率高

**嵌入式选择：**
- 可靠性要求高：选择TCP
- 实时性要求高：选择UDP
- 资源受限：优先考虑UDP
- 简单数据传输：UDP足够

**Q32: 什么是CAN总线？有什么特点？**

**A32:**
**定义：**
CAN（Controller Area Network）是一种串行通信协议，主要用于汽车和工业控制。

**主要特点：**
- 多主机制：任意节点都可以发起通信
- 非破坏性仲裁：基于ID优先级
- 错误检测：CRC校验、帧检查
- 错误处理：自动重传、错误界定
- 实时性好：最高1Mbps传输速率

**应用优势：**
- 可靠性高
- 实时性好
- 成本低
- 易于扩展

### 15. 软件工程

**Q33: 嵌入式软件的版本管理有什么特点？**

**A33:**
**特殊要求：**
- 硬件相关性：不同硬件版本需要不同软件
- 实时性约束：更新不能影响系统运行
- 存储限制：版本信息占用空间有限
- 可追溯性：需要记录详细的版本历史

**版本管理策略：**
- 语义化版本：主版本.次版本.修订版本
- 分支管理：主分支、开发分支、发布分支
- 标签管理：里程碑版本标记
- 自动化构建：CI/CD流水线

**Q34: 什么是代码覆盖率？如何提高？**

**A34:**
**定义：**
代码覆盖率是测试用例执行时覆盖的代码比例。

**覆盖类型：**
- 语句覆盖：执行的语句比例
- 分支覆盖：执行的分支比例
- 条件覆盖：条件组合的覆盖
- 路径覆盖：执行路径的覆盖

**提高方法：**
- 设计全面的测试用例
- 使用边界值测试
- 异常情况测试
- 自动化测试工具
- 定期评审和改进

### 16. 安全与加密

**Q35: 嵌入式系统有哪些安全威胁？**

**A35:**
**物理安全：**
- 硬件篡改：芯片探针攻击
- 侧信道攻击：功耗分析、时间分析
- 故障注入：电压、时钟干扰

**软件安全：**
- 缓冲区溢出：栈溢出、堆溢出
- 代码注入：恶意代码植入
- 权限提升：获取系统控制权

**通信安全：**
- 窃听：数据被截获
- 重放攻击：重复发送数据包
- 中间人攻击：伪装成通信方

**防护措施：**
- 硬件安全模块（HSM）
- 安全启动（Secure Boot）
- 代码签名验证
- 数据加密传输

**Q36: 什么是安全启动？如何实现？**

**A36:**
**定义：**
安全启动是确保系统只运行经过验证的可信软件的机制。

**实现原理：**
1. **信任根**：硬件中的不可变代码
2. **信任链**：逐级验证下一级软件
3. **数字签名**：使用私钥签名，公钥验证
4. **哈希校验**：确保代码完整性

**实现步骤：**
1. BootROM验证Bootloader签名
2. Bootloader验证内核签名
3. 内核验证应用程序签名
4. 任何验证失败都拒绝启动

**优势：**
- 防止恶意软件运行
- 确保软件完整性
- 建立信任基础

---

## 位操作与二进制运算

### 题目1：32位整数位反转
**问题**: 给定一个32位无符号整数，反转其二进制位的顺序。

**详细解答**:
```c
// 方法1：逐位反转
uint32_t reverse_bits_basic(uint32_t n) {
    uint32_t result = 0;
    for (int i = 0; i < 32; i++) {
        result = (result << 1) | (n & 1);
        n >>= 1;
    }
    return result;
}

// 方法2：分治法（更高效）
uint32_t reverse_bits_divide_conquer(uint32_t n) {
    // 交换相邻位
    n = ((n & 0xAAAAAAAA) >> 1) | ((n & 0x55555555) << 1);
    // 交换相邻2位组
    n = ((n & 0xCCCCCCCC) >> 2) | ((n & 0x33333333) << 2);
    // 交换相邻4位组
    n = ((n & 0xF0F0F0F0) >> 4) | ((n & 0x0F0F0F0F) << 4);
    // 交换相邻8位组
    n = ((n & 0xFF00FF00) >> 8) | ((n & 0x00FF00FF) << 8);
    // 交换前16位和后16位
    return (n >> 16) | (n << 16);
}
```

**考察点**:
- 位操作基础知识
- 算法优化思维（从O(n)优化到O(log n)）
- 分治思想的应用
- 位掩码的使用

**应用场景**:
- 网络字节序转换
- 图像处理中的像素格式转换
- 加密算法中的位置换

### 题目2：交换两个位
**问题**: 给定一个无符号整数和两个位位置i和j，交换第i位和第j位的值。

**详细解答**:
```c
uint32_t swap_bits(uint32_t num, int i, int j) {
    // 检查边界
    if (i < 0 || i >= 32 || j < 0 || j >= 32 || i == j) {
        return num;
    }
    
    // 获取第i位和第j位的值
    int bit_i = (num >> i) & 1;
    int bit_j = (num >> j) & 1;
    
    // 如果两位不同，才需要交换
    if (bit_i != bit_j) {
        // 使用XOR翻转这两位
        num ^= (1U << i) | (1U << j);
    }
    
    return num;
}

// 更简洁的方法
uint32_t swap_bits_xor(uint32_t num, int i, int j) {
    if (((num >> i) & 1) != ((num >> j) & 1)) {
        num ^= (1U << i) | (1U << j);
    }
    return num;
}
```

**考察点**:
- XOR运算的特性
- 位掩码操作
- 边界条件处理
- 代码简洁性

### 题目3：计算二进制中1的个数
**问题**: 计算一个32位整数的二进制表示中有多少个1。

**详细解答**:
```c
// 方法1：逐位检查
int count_ones_basic(uint32_t n) {
    int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

// 方法2：Brian Kernighan算法
int count_ones_kernighan(uint32_t n) {
    int count = 0;
    while (n) {
        n &= n - 1;  // 清除最低位的1
        count++;
    }
    return count;
}

// 方法3：查表法
static const int lookup[16] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
int count_ones_lookup(uint32_t n) {
    int count = 0;
    while (n) {
        count += lookup[n & 0xF];
        n >>= 4;
    }
    return count;
}

// 方法4：并行计算（最快）
int count_ones_parallel(uint32_t n) {
    n = (n & 0x55555555) + ((n >> 1) & 0x55555555);
    n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
    n = (n & 0x0F0F0F0F) + ((n >> 4) & 0x0F0F0F0F);
    n = (n & 0x00FF00FF) + ((n >> 8) & 0x00FF00FF);
    n = (n & 0x0000FFFF) + ((n >> 16) & 0x0000FFFF);
    return n;
}
```

**考察点**:
- 多种算法实现
- 时间复杂度分析
- 空间时间权衡
- 并行计算思维

---

## 内存管理

### 题目4：实现简单的内存分配器
**问题**: 设计并实现一个简单的内存分配器，支持malloc和free功能。

**详细解答**:
```c
#define HEAP_SIZE 4096
#define BLOCK_SIZE sizeof(struct block)

struct block {
    size_t size;
    int free;
    struct block *next;
};

static char heap[HEAP_SIZE];
static struct block *free_list = NULL;
static int heap_initialized = 0;

void init_heap() {
    free_list = (struct block*)heap;
    free_list->size = HEAP_SIZE - BLOCK_SIZE;
    free_list->free = 1;
    free_list->next = NULL;
    heap_initialized = 1;
}

void* my_malloc(size_t size) {
    if (!heap_initialized) init_heap();
    
    struct block *current = free_list;
    
    // 对齐到4字节边界
    size = (size + 3) & ~3;
    
    while (current) {
        if (current->free && current->size >= size) {
            // 找到合适的块
            if (current->size > size + BLOCK_SIZE) {
                // 分割块
                struct block *new_block = (struct block*)((char*)current + BLOCK_SIZE + size);
                new_block->size = current->size - size - BLOCK_SIZE;
                new_block->free = 1;
                new_block->next = current->next;
                
                current->size = size;
                current->next = new_block;
            }
            
            current->free = 0;
            return (char*)current + BLOCK_SIZE;
        }
        current = current->next;
    }
    
    return NULL; // 没有足够的内存
}

void my_free(void *ptr) {
    if (!ptr) return;
    
    struct block *block = (struct block*)((char*)ptr - BLOCK_SIZE);
    block->free = 1;
    
    // 合并相邻的空闲块
    struct block *current = free_list;
    while (current && current->next) {
        if (current->free && current->next->free) {
            current->size += BLOCK_SIZE + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}
```

**考察点**:
- 链表操作
- 内存对齐
- 内存碎片处理
- 指针运算
- 数据结构设计

### 题目5：检测内存泄漏
**问题**: 如何在嵌入式系统中检测和预防内存泄漏？

**详细解答**:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALLOCS 1000

typedef struct {
    void *ptr;
    size_t size;
    const char *file;
    int line;
    int in_use;
} alloc_info_t;

static alloc_info_t alloc_list[MAX_ALLOCS];
static int alloc_count = 0;

void* debug_malloc(size_t size, const char *file, int line) {
    void *ptr = malloc(size);
    if (ptr && alloc_count < MAX_ALLOCS) {
        alloc_list[alloc_count].ptr = ptr;
        alloc_list[alloc_count].size = size;
        alloc_list[alloc_count].file = file;
        alloc_list[alloc_count].line = line;
        alloc_list[alloc_count].in_use = 1;
        alloc_count++;
    }
    return ptr;
}

void debug_free(void *ptr) {
    if (!ptr) return;
    
    for (int i = 0; i < alloc_count; i++) {
        if (alloc_list[i].ptr == ptr && alloc_list[i].in_use) {
            alloc_list[i].in_use = 0;
            free(ptr);
            return;
        }
    }
    
    printf("Warning: Attempting to free untracked pointer %p\n", ptr);
}

void check_memory_leaks() {
    int leak_count = 0;
    size_t total_leaked = 0;
    
    for (int i = 0; i < alloc_count; i++) {
        if (alloc_list[i].in_use) {
            printf("Memory leak: %zu bytes at %s:%d\n", 
                   alloc_list[i].size, alloc_list[i].file, alloc_list[i].line);
            leak_count++;
            total_leaked += alloc_list[i].size;
        }
    }
    
    if (leak_count == 0) {
        printf("No memory leaks detected.\n");
    } else {
        printf("Total: %d leaks, %zu bytes\n", leak_count, total_leaked);
    }
}

#define malloc(size) debug_malloc(size, __FILE__, __LINE__)
#define free(ptr) debug_free(ptr)
```

**考察点**:
- 调试技术
- 宏定义的使用
- 内存管理最佳实践
- 错误检测机制

---

## 实时操作系统(RTOS)

### 题目6：任务调度算法
**问题**: 实现一个简单的优先级调度器。

**详细解答**:
```c
#define MAX_TASKS 32
#define MAX_PRIORITY 31

typedef enum {
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_SUSPENDED
} task_state_t;

typedef struct {
    int task_id;
    int priority;
    task_state_t state;
    void (*task_func)(void);
    char *stack_ptr;
    int time_slice;
    int remaining_time;
} task_t;

static task_t tasks[MAX_TASKS];
static int current_task = -1;
static int task_count = 0;

// 优先级位图，用于快速查找最高优先级任务
static uint32_t ready_bitmap = 0;

int create_task(void (*func)(void), int priority) {
    if (task_count >= MAX_TASKS || priority > MAX_PRIORITY) {
        return -1;
    }
    
    int task_id = task_count++;
    tasks[task_id].task_id = task_id;
    tasks[task_id].priority = priority;
    tasks[task_id].state = TASK_READY;
    tasks[task_id].task_func = func;
    tasks[task_id].time_slice = 10; // 默认时间片
    tasks[task_id].remaining_time = 10;
    
    // 设置优先级位图
    ready_bitmap |= (1 << priority);
    
    return task_id;
}

int find_highest_priority_task() {
    if (ready_bitmap == 0) return -1;
    
    // 查找最高位（最高优先级）
    int highest_priority = 31 - __builtin_clz(ready_bitmap);
    
    // 在该优先级中找到就绪任务
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].priority == highest_priority && 
            tasks[i].state == TASK_READY) {
            return i;
        }
    }
    
    return -1;
}

void schedule() {
    int next_task = find_highest_priority_task();
    
    if (next_task == -1) {
        // 没有就绪任务，进入空闲
        return;
    }
    
    if (current_task != next_task) {
        // 任务切换
        if (current_task >= 0) {
            tasks[current_task].state = TASK_READY;
        }
        
        current_task = next_task;
        tasks[current_task].state = TASK_RUNNING;
        tasks[current_task].remaining_time = tasks[current_task].time_slice;
    }
}

void timer_interrupt_handler() {
    if (current_task >= 0) {
        tasks[current_task].remaining_time--;
        
        if (tasks[current_task].remaining_time <= 0) {
            // 时间片用完，重新调度
            schedule();
        }
    }
}
```

**考察点**:
- 调度算法设计
- 位操作优化
- 中断处理
- 数据结构设计
- 实时系统概念

### 题目7：信号量实现
**问题**: 实现二进制信号量和计数信号量。

**详细解答**:
```c
typedef struct {
    int count;
    int max_count;
    int waiting_tasks[MAX_TASKS];
    int waiting_count;
} semaphore_t;

void sem_init(semaphore_t *sem, int initial_count, int max_count) {
    sem->count = initial_count;
    sem->max_count = max_count;
    sem->waiting_count = 0;
}

int sem_wait(semaphore_t *sem, int task_id) {
    // 关中断，确保原子操作
    __disable_irq();
    
    if (sem->count > 0) {
        sem->count--;
        __enable_irq();
        return 0; // 成功获取信号量
    } else {
        // 需要等待，将任务加入等待队列
        if (sem->waiting_count < MAX_TASKS) {
            sem->waiting_tasks[sem->waiting_count++] = task_id;
            tasks[task_id].state = TASK_BLOCKED;
        }
        __enable_irq();
        
        // 触发调度
        schedule();
        return -1; // 任务被阻塞
    }
}

int sem_post(semaphore_t *sem) {
    __disable_irq();
    
    if (sem->count < sem->max_count) {
        sem->count++;
        
        // 唤醒等待的任务
        if (sem->waiting_count > 0) {
            int task_id = sem->waiting_tasks[0];
            
            // 从等待队列中移除任务
            for (int i = 0; i < sem->waiting_count - 1; i++) {
                sem->waiting_tasks[i] = sem->waiting_tasks[i + 1];
            }
            sem->waiting_count--;
            
            // 将任务设为就绪状态
            tasks[task_id].state = TASK_READY;
            ready_bitmap |= (1 << tasks[task_id].priority);
        }
        
        __enable_irq();
        return 0;
    } else {
        __enable_irq();
        return -1; // 信号量已满
    }
}

// 二进制信号量（互斥锁）
typedef struct {
    int locked;
    int owner_task;
} mutex_t;

void mutex_init(mutex_t *mutex) {
    mutex->locked = 0;
    mutex->owner_task = -1;
}

int mutex_lock(mutex_t *mutex, int task_id) {
    __disable_irq();
    
    if (!mutex->locked) {
        mutex->locked = 1;
        mutex->owner_task = task_id;
        __enable_irq();
        return 0;
    } else {
        __enable_irq();
        return -1; // 已被锁定
    }
}

int mutex_unlock(mutex_t *mutex, int task_id) {
    __disable_irq();
    
    if (mutex->locked && mutex->owner_task == task_id) {
        mutex->locked = 0;
        mutex->owner_task = -1;
        __enable_irq();
        return 0;
    } else {
        __enable_irq();
        return -1; // 不是锁的拥有者
    }
}
```

**考察点**:
- 同步原语实现
- 原子操作
- 中断控制
- 死锁预防
- 优先级继承

---

## 中断处理

### 题目8：中断服务程序设计
**问题**: 设计一个高效的中断服务程序，处理串口数据接收。

**详细解答**:
```c
#define UART_BUFFER_SIZE 256

typedef struct {
    char buffer[UART_BUFFER_SIZE];
    volatile int head;
    volatile int tail;
    volatile int count;
} circular_buffer_t;

static circular_buffer_t uart_rx_buffer = {0};
static circular_buffer_t uart_tx_buffer = {0};

// 环形缓冲区操作
int buffer_put(circular_buffer_t *buf, char data) {
    if (buf->count >= UART_BUFFER_SIZE) {
        return -1; // 缓冲区满
    }
    
    buf->buffer[buf->head] = data;
    buf->head = (buf->head + 1) % UART_BUFFER_SIZE;
    buf->count++;
    return 0;
}

int buffer_get(circular_buffer_t *buf, char *data) {
    if (buf->count == 0) {
        return -1; // 缓冲区空
    }
    
    *data = buf->buffer[buf->tail];
    buf->tail = (buf->tail + 1) % UART_BUFFER_SIZE;
    buf->count--;
    return 0;
}

// UART接收中断服务程序
void UART_RX_IRQHandler(void) {
    char received_data;
    
    // 读取接收到的数据
    received_data = UART_DATA_REG;
    
    // 将数据放入环形缓冲区
    if (buffer_put(&uart_rx_buffer, received_data) != 0) {
        // 缓冲区溢出处理
        UART_STATUS_REG |= UART_RX_OVERFLOW_FLAG;
    }
    
    // 清除中断标志
    UART_STATUS_REG |= UART_RX_INT_FLAG;
}

// UART发送中断服务程序
void UART_TX_IRQHandler(void) {
    char data_to_send;
    
    if (buffer_get(&uart_tx_buffer, &data_to_send) == 0) {
        // 发送数据
        UART_DATA_REG = data_to_send;
    } else {
        // 没有数据要发送，禁用发送中断
        UART_INT_ENABLE_REG &= ~UART_TX_INT_ENABLE;
    }
    
    // 清除中断标志
    UART_STATUS_REG |= UART_TX_INT_FLAG;
}

// 应用层接口
int uart_send_string(const char *str) {
    int len = strlen(str);
    
    for (int i = 0; i < len; i++) {
        while (buffer_put(&uart_tx_buffer, str[i]) != 0) {
            // 等待缓冲区有空间
        }
    }
    
    // 启用发送中断
    UART_INT_ENABLE_REG |= UART_TX_INT_ENABLE;
    
    return len;
}

int uart_receive_string(char *buffer, int max_len) {
    int count = 0;
    char ch;
    
    while (count < max_len - 1 && buffer_get(&uart_rx_buffer, &ch) == 0) {
        buffer[count++] = ch;
        if (ch == '\n' || ch == '\r') break;
    }
    
    buffer[count] = '\0';
    return count;
}
```

**考察点**:
- 中断处理机制
- 环形缓冲区设计
- 并发安全性
- 硬件寄存器操作
- 性能优化

---

## 硬件接口与寄存器操作

### 题目9：GPIO控制和位域操作
**问题**: 实现GPIO端口的控制功能，包括输入输出配置、电平设置等。

**详细解答**:
```c
// 假设的GPIO寄存器定义
#define GPIO_BASE_ADDR    0x40020000
#define GPIO_MODE_OFFSET  0x00
#define GPIO_OTYPE_OFFSET 0x04
#define GPIO_SPEED_OFFSET 0x08
#define GPIO_PUPD_OFFSET  0x0C
#define GPIO_IDR_OFFSET   0x10
#define GPIO_ODR_OFFSET   0x14
#define GPIO_BSRR_OFFSET  0x18

typedef struct {
    volatile uint32_t MODE;   // 模式寄存器
    volatile uint32_t OTYPE;  // 输出类型寄存器
    volatile uint32_t SPEED;  // 速度寄存器
    volatile uint32_t PUPD;   // 上下拉寄存器
    volatile uint32_t IDR;    // 输入数据寄存器
    volatile uint32_t ODR;    // 输出数据寄存器
    volatile uint32_t BSRR;   // 位设置/复位寄存器
} GPIO_TypeDef;

#define GPIOA ((GPIO_TypeDef*)(GPIO_BASE_ADDR + 0x000))
#define GPIOB ((GPIO_TypeDef*)(GPIO_BASE_ADDR + 0x400))
#define GPIOC ((GPIO_TypeDef*)(GPIO_BASE_ADDR + 0x800))

typedef enum {
    GPIO_MODE_INPUT  = 0,
    GPIO_MODE_OUTPUT = 1,
    GPIO_MODE_AF     = 2,
    GPIO_MODE_ANALOG = 3
} gpio_mode_t;

typedef enum {
    GPIO_SPEED_LOW    = 0,
    GPIO_SPEED_MEDIUM = 1,
    GPIO_SPEED_HIGH   = 2,
    GPIO_SPEED_VERY_HIGH = 3
} gpio_speed_t;

// GPIO配置函数
void gpio_config_pin(GPIO_TypeDef *gpio, int pin, gpio_mode_t mode, gpio_speed_t speed) {
    // 配置模式（每个引脚占用2位）
    gpio->MODE &= ~(3 << (pin * 2));
    gpio->MODE |= (mode << (pin * 2));
    
    // 配置速度（每个引脚占用2位）
    gpio->SPEED &= ~(3 << (pin * 2));
    gpio->SPEED |= (speed << (pin * 2));
}

// 设置引脚输出电平
void gpio_set_pin(GPIO_TypeDef *gpio, int pin) {
    gpio->BSRR = (1 << pin);  // 使用BSRR寄存器原子操作
}

void gpio_reset_pin(GPIO_TypeDef *gpio, int pin) {
    gpio->BSRR = (1 << (pin + 16));  // 复位位在高16位
}

void gpio_toggle_pin(GPIO_TypeDef *gpio, int pin) {
    gpio->ODR ^= (1 << pin);
}

// 读取引脚状态
int gpio_read_pin(GPIO_TypeDef *gpio, int pin) {
    return (gpio->IDR >> pin) & 1;
}

// 位域操作示例
typedef union {
    uint32_t value;
    struct {
        uint32_t pin0  : 1;
        uint32_t pin1  : 1;
        uint32_t pin2  : 1;
        uint32_t pin3  : 1;
        uint32_t pin4  : 1;
        uint32_t pin5  : 1;
        uint32_t pin6  : 1;
        uint32_t pin7  : 1;
        uint32_t pin8  : 1;
        uint32_t pin9  : 1;
        uint32_t pin10 : 1;
        uint32_t pin11 : 1;
        uint32_t pin12 : 1;
        uint32_t pin13 : 1;
        uint32_t pin14 : 1;
        uint32_t pin15 : 1;
        uint32_t reserved : 16;
    } bits;
} gpio_register_t;

// 使用位域的GPIO操作
void gpio_set_multiple_pins(GPIO_TypeDef *gpio, uint16_t pin_mask) {
    gpio_register_t *odr = (gpio_register_t*)&gpio->ODR;
    odr->value |= pin_mask;
}

// LED控制示例
#define LED_RED_PIN    0
#define LED_GREEN_PIN  1
#define LED_BLUE_PIN   2

void led_init(void) {
    gpio_config_pin(GPIOA, LED_RED_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_LOW);
    gpio_config_pin(GPIOA, LED_GREEN_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_LOW);
    gpio_config_pin(GPIOA, LED_BLUE_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_LOW);
}

void led_set_color(int red, int green, int blue) {
    if (red)   gpio_set_pin(GPIOA, LED_RED_PIN);
    else       gpio_reset_pin(GPIOA, LED_RED_PIN);
    
    if (green) gpio_set_pin(GPIOA, LED_GREEN_PIN);
    else       gpio_reset_pin(GPIOA, LED_GREEN_PIN);
    
    if (blue)  gpio_set_pin(GPIOA, LED_BLUE_PIN);
    else       gpio_reset_pin(GPIOA, LED_BLUE_PIN);
}
```

**考察点**:
- 寄存器操作
- 位域使用
- 硬件抽象层设计
- 原子操作
- 内存映射I/O

### 题目10：SPI通信实现
**问题**: 实现SPI主设备通信协议。

**详细解答**:
```c
// SPI寄存器定义
typedef struct {
    volatile uint32_t CR1;    // 控制寄存器1
    volatile uint32_t CR2;    // 控制寄存器2
    volatile uint32_t SR;     // 状态寄存器
    volatile uint32_t DR;     // 数据寄存器
} SPI_TypeDef;

#define SPI1 ((SPI_TypeDef*)0x40013000)

// SPI控制位定义
#define SPI_CR1_CPHA     (1 << 0)   // 时钟相位
#define SPI_CR1_CPOL     (1 << 1)   // 时钟极性
#define SPI_CR1_MSTR     (1 << 2)   // 主设备模式
#define SPI_CR1_BR_SHIFT 3          // 波特率控制
#define SPI_CR1_SPE      (1 << 6)   // SPI使能
#define SPI_CR1_LSBFIRST (1 << 7)   // 帧格式

#define SPI_SR_RXNE      (1 << 0)   // 接收缓冲区非空
#define SPI_SR_TXE       (1 << 1)   // 发送缓冲区空
#define SPI_SR_BSY       (1 << 7)   // 忙标志

typedef enum {
    SPI_MODE_0 = 0,  // CPOL=0, CPHA=0
    SPI_MODE_1 = 1,  // CPOL=0, CPHA=1
    SPI_MODE_2 = 2,  // CPOL=1, CPHA=0
    SPI_MODE_3 = 3   // CPOL=1, CPHA=1
} spi_mode_t;

typedef enum {
    SPI_SPEED_DIV2   = 0,
    SPI_SPEED_DIV4   = 1,
    SPI_SPEED_DIV8   = 2,
    SPI_SPEED_DIV16  = 3,
    SPI_SPEED_DIV32  = 4,
    SPI_SPEED_DIV64  = 5,
    SPI_SPEED_DIV128 = 6,
    SPI_SPEED_DIV256 = 7
} spi_speed_t;

void spi_init(SPI_TypeDef *spi, spi_mode_t mode, spi_speed_t speed) {
    // 禁用SPI
    spi->CR1 &= ~SPI_CR1_SPE;
    
    // 配置SPI模式
    uint32_t cr1 = spi->CR1;
    cr1 &= ~(SPI_CR1_CPHA | SPI_CR1_CPOL);
    
    if (mode & 1) cr1 |= SPI_CR1_CPHA;
    if (mode & 2) cr1 |= SPI_CR1_CPOL;
    
    // 设置为主设备模式
    cr1 |= SPI_CR1_MSTR;
    
    // 设置波特率
    cr1 &= ~(7 << SPI_CR1_BR_SHIFT);
    cr1 |= (speed << SPI_CR1_BR_SHIFT);
    
    spi->CR1 = cr1;
    
    // 启用SPI
    spi->CR1 |= SPI_CR1_SPE;
}

uint8_t spi_transfer_byte(SPI_TypeDef *spi, uint8_t data) {
    // 等待发送缓冲区空
    while (!(spi->SR & SPI_SR_TXE));
    
    // 发送数据
    spi->DR = data;
    
    // 等待接收缓冲区非空
    while (!(spi->SR & SPI_SR_RXNE));
    
    // 读取接收到的数据
    return spi->DR;
}

void spi_transfer_buffer(SPI_TypeDef *spi, uint8_t *tx_data, uint8_t *rx_data, int len) {
    for (int i = 0; i < len; i++) {
        uint8_t tx_byte = tx_data ? tx_data[i] : 0xFF;
        uint8_t rx_byte = spi_transfer_byte(spi, tx_byte);
        if (rx_data) rx_data[i] = rx_byte;
    }
    
    // 等待传输完成
    while (spi->SR & SPI_SR_BSY);
}

// CS引脚控制
#define SPI_CS_PIN 4

void spi_cs_low(void) {
    gpio_reset_pin(GPIOA, SPI_CS_PIN);
}

void spi_cs_high(void) {
    gpio_set_pin(GPIOA, SPI_CS_PIN);
}

// 高级SPI操作
int spi_write_register(SPI_TypeDef *spi, uint8_t reg_addr, uint8_t value) {
    spi_cs_low();
    
    spi_transfer_byte(spi, reg_addr);  // 发送寄存器地址
    spi_transfer_byte(spi, value);     // 发送数据
    
    spi_cs_high();
    return 0;
}

uint8_t spi_read_register(SPI_TypeDef *spi, uint8_t reg_addr) {
    spi_cs_low();
    
    spi_transfer_byte(spi, reg_addr | 0x80);  // 读命令（最高位置1）
    uint8_t value = spi_transfer_byte(spi, 0xFF);  // 读取数据
    
    spi_cs_high();
    return value;
}
```

**考察点**:
- 通信协议实现
- 时序控制
- 状态机设计
- 错误处理
- 硬件接口编程

---

## 并发与同步

### 题目11：生产者消费者问题
**问题**: 在嵌入式环境中实现生产者消费者模式，处理数据采集和处理。

**详细解答**:
```c
#define BUFFER_SIZE 16

typedef struct {
    int data[BUFFER_SIZE];
    volatile int head;
    volatile int tail;
    volatile int count;
    semaphore_t empty_sem;  // 空位信号量
    semaphore_t full_sem;   // 满位信号量
    mutex_t mutex;          // 互斥锁
} producer_consumer_t;

static producer_consumer_t pc_buffer;

void pc_init(void) {
    pc_buffer.head = 0;
    pc_buffer.tail = 0;
    pc_buffer.count = 0;
    
    sem_init(&pc_buffer.empty_sem, BUFFER_SIZE, BUFFER_SIZE);
    sem_init(&pc_buffer.full_sem, 0, BUFFER_SIZE);
    mutex_init(&pc_buffer.mutex);
}

// 生产者任务
void producer_task(void) {
    int data = 0;
    
    while (1) {
        // 生成数据
        data = read_sensor_data();
        
        // 等待空位
        sem_wait(&pc_buffer.empty_sem, get_current_task_id());
        
        // 获取互斥锁
        mutex_lock(&pc_buffer.mutex, get_current_task_id());
        
        // 放入数据
        pc_buffer.data[pc_buffer.head] = data;
        pc_buffer.head = (pc_buffer.head + 1) % BUFFER_SIZE;
        pc_buffer.count++;
        
        // 释放互斥锁
        mutex_unlock(&pc_buffer.mutex, get_current_task_id());
        
        // 通知有数据
        sem_post(&pc_buffer.full_sem);
        
        // 延时
        task_delay(100);
    }
}

// 消费者任务
void consumer_task(void) {
    int data;
    
    while (1) {
        // 等待数据
        sem_wait(&pc_buffer.full_sem, get_current_task_id());
        
        // 获取互斥锁
        mutex_lock(&pc_buffer.mutex, get_current_task_id());
        
        // 取出数据
        data = pc_buffer.data[pc_buffer.tail];
        pc_buffer.tail = (pc_buffer.tail + 1) % BUFFER_SIZE;
        pc_buffer.count--;
        
        // 释放互斥锁
        mutex_unlock(&pc_buffer.mutex, get_current_task_id());
        
        // 通知有空位
        sem_post(&pc_buffer.empty_sem);
        
        // 处理数据
        process_data(data);
    }
}

// 无锁实现（使用原子操作）
typedef struct {
    volatile int data[BUFFER_SIZE];
    volatile int head;
    volatile int tail;
} lockfree_buffer_t;

static lockfree_buffer_t lf_buffer;

int lockfree_put(int data) {
    int head = lf_buffer.head;
    int next_head = (head + 1) % BUFFER_SIZE;
    
    if (next_head == lf_buffer.tail) {
        return -1; // 缓冲区满
    }
    
    lf_buffer.data[head] = data;
    
    // 内存屏障，确保数据写入完成后再更新head
    __sync_synchronize();
    lf_buffer.head = next_head;
    
    return 0;
}

int lockfree_get(int *data) {
    int tail = lf_buffer.tail;
    
    if (tail == lf_buffer.head) {
        return -1; // 缓冲区空
    }
    
    *data = lf_buffer.data[tail];
    
    // 内存屏障
    __sync_synchronize();
    lf_buffer.tail = (tail + 1) % BUFFER_SIZE;
    
    return 0;
}
```

**考察点**:
- 同步机制设计
- 死锁避免
- 无锁编程
- 内存屏障
- 原子操作

### 题目12：优先级反转问题
**问题**: 什么是优先级反转？如何解决？

**详细解答**:

优先级反转是指高优先级任务被低优先级任务阻塞的现象。

**场景描述**:
- 任务H（高优先级）、任务M（中优先级）、任务L（低优先级）
- 任务L获得了共享资源的锁
- 任务H需要该资源，被阻塞
- 任务M抢占任务L，导致任务H间接被任务M阻塞

**解决方案1：优先级继承**
```c
typedef struct {
    int locked;
    int owner_task;
    int original_priority;
} priority_inherit_mutex_t;

int pi_mutex_lock(priority_inherit_mutex_t *mutex, int task_id) {
    __disable_irq();
    
    if (!mutex->locked) {
        mutex->locked = 1;
        mutex->owner_task = task_id;
        mutex->original_priority = tasks[task_id].priority;
        __enable_irq();
        return 0;
    } else {
        // 如果当前任务优先级更高，提升锁拥有者的优先级
        int current_priority = tasks[task_id].priority;
        int owner_priority = tasks[mutex->owner_task].priority;
        
        if (current_priority > owner_priority) {
            tasks[mutex->owner_task].priority = current_priority;
            // 重新调度
            schedule();
        }
        
        __enable_irq();
        return -1; // 需要等待
    }
}

int pi_mutex_unlock(priority_inherit_mutex_t *mutex, int task_id) {
    __disable_irq();
    
    if (mutex->locked && mutex->owner_task == task_id) {
        mutex->locked = 0;
        
        // 恢复原始优先级
        tasks[task_id].priority = mutex->original_priority;
        mutex->owner_task = -1;
        
        __enable_irq();
        
        // 重新调度
        schedule();
        return 0;
    } else {
        __enable_irq();
        return -1;
    }
}
```

**解决方案2：优先级上限协议**
```c
typedef struct {
    int locked;
    int owner_task;
    int ceiling_priority;
} priority_ceiling_mutex_t;

int pc_mutex_lock(priority_ceiling_mutex_t *mutex, int task_id) {
    __disable_irq();
    
    if (!mutex->locked) {
        mutex->locked = 1;
        mutex->owner_task = task_id;
        
        // 立即提升到上限优先级
        int original_priority = tasks[task_id].priority;
        tasks[task_id].priority = mutex->ceiling_priority;
        
        __enable_irq();
        return 0;
    } else {
        __enable_irq();
        return -1;
    }
}
```

**考察点**:
- 实时系统理论
- 调度算法
- 优先级管理
- 系统设计权衡

---

## 性能优化

### 题目13：缓存优化
**问题**: 如何优化代码以提高缓存命中率？

**详细解答**:
```c
// 不良示例：缓存不友好的矩阵乘法
void matrix_multiply_bad(int A[N][N], int B[N][N], int C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];  // B的访问模式缓存不友好
            }
        }
    }
}

// 优化示例：缓存友好的矩阵乘法
void matrix_multiply_good(int A[N][N], int B[N][N], int C[N][N]) {
    // 分块优化
    const int BLOCK_SIZE = 64;
    
    for (int ii = 0; ii < N; ii += BLOCK_SIZE) {
        for (int jj = 0; jj < N; jj += BLOCK_SIZE) {
            for (int kk = 0; kk < N; kk += BLOCK_SIZE) {
                // 在块内进行计算
                for (int i = ii; i < min(ii + BLOCK_SIZE, N); i++) {
                    for (int j = jj; j < min(jj + BLOCK_SIZE, N); j++) {
                        int sum = C[i][j];
                        for (int k = kk; k < min(kk + BLOCK_SIZE, N); k++) {
                            sum += A[i][k] * B[k][j];
                        }
                        C[i][j] = sum;
                    }
                }
            }
        }
    }
}

// 数据结构优化：AoS vs SoA
// Array of Structures (不友好)
typedef struct {
    float x, y, z;
    int id;
} particle_aos_t;

void update_particles_aos(particle_aos_t *particles, int count) {
    for (int i = 0; i < count; i++) {
        particles[i].x += 1.0f;  // 跳跃访问，缓存不友好
        particles[i].y += 1.0f;
        particles[i].z += 1.0f;
    }
}

// Structure of Arrays (友好)
typedef struct {
    float *x, *y, *z;
    int *id;
} particle_soa_t;

void update_particles_soa(particle_soa_t *particles, int count) {
    // 连续访问，缓存友好
    for (int i = 0; i < count; i++) {
        particles->x[i] += 1.0f;
    }
    for (int i = 0; i < count; i++) {
        particles->y[i] += 1.0f;
    }
    for (int i = 0; i < count; i++) {
        particles->z[i] += 1.0f;
    }
}

// 内存对齐优化
typedef struct __attribute__((packed)) {
    char a;      // 1字节
    int b;       // 4字节，可能不对齐
    char c;      // 1字节
} unaligned_struct_t;  // 总大小可能是6字节，但访问可能很慢

typedef struct {
    int b;       // 4字节，对齐
    char a;      // 1字节
    char c;      // 1字节
    char padding[2];  // 显式填充
} aligned_struct_t;    // 总大小8字节，访问快

// 预取优化
void process_array_with_prefetch(int *array, int size) {
    for (int i = 0; i < size; i++) {
        // 预取下一个缓存行
        if (i + 16 < size) {
            __builtin_prefetch(&array[i + 16], 0, 3);
        }
        
        // 处理当前元素
        array[i] = expensive_computation(array[i]);
    }
}
```

**考察点**:
- 缓存原理理解
- 内存访问模式
- 数据结构设计
- 编译器优化

### 题目14：功耗优化
**问题**: 在嵌入式系统中如何降低功耗？

**详细解答**:
```c
// 时钟管理
typedef enum {
    CLOCK_SPEED_LOW    = 1000000,   // 1MHz
    CLOCK_SPEED_MEDIUM = 8000000,   // 8MHz
    CLOCK_SPEED_HIGH   = 72000000   // 72MHz
} clock_speed_t;

void set_system_clock(clock_speed_t speed) {
    // 根据需要调整系统时钟频率
    switch (speed) {
        case CLOCK_SPEED_LOW:
            RCC_CFGR |= RCC_CFGR_SW_HSI;  // 使用内部时钟
            break;
        case CLOCK_SPEED_MEDIUM:
            RCC_CFGR |= RCC_CFGR_SW_HSE;  // 使用外部时钟
            break;
        case CLOCK_SPEED_HIGH:
            RCC_CFGR |= RCC_CFGR_SW_PLL;  // 使用PLL
            break;
    }
    
    // 更新系统时钟变量
    SystemCoreClock = speed;
}

// 外设时钟管理
void peripheral_clock_enable(uint32_t peripheral) {
    RCC_APB1ENR |= peripheral;
}

void peripheral_clock_disable(uint32_t peripheral) {
    RCC_APB1ENR &= ~peripheral;
}

// 睡眠模式管理
typedef enum {
    POWER_MODE_ACTIVE,
    POWER_MODE_SLEEP,
    POWER_MODE_STOP,
    POWER_MODE_STANDBY
} power_mode_t;

void enter_power_mode(power_mode_t mode) {
    switch (mode) {
        case POWER_MODE_SLEEP:
            // 进入睡眠模式，CPU停止，外设继续运行
            __WFI();  // Wait For Interrupt
            break;
            
        case POWER_MODE_STOP:
            // 进入停止模式，CPU和大部分外设停止
            PWR_CR |= PWR_CR_LPDS;  // 低功耗深度睡眠
            SCB_SCR |= SCB_SCR_SLEEPDEEP_Msk;
            __WFI();
            break;
            
        case POWER_MODE_STANDBY:
            // 进入待机模式，最低功耗
            PWR_CR |= PWR_CR_PDDS;  // 电源关闭深度睡眠
            SCB_SCR |= SCB_SCR_SLEEPDEEP_Msk;
            __WFI();
            break;
            
        default:
            break;
    }
}

// 智能功耗管理
typedef struct {
    int active_tasks;
    int idle_time;
    power_mode_t current_mode;
} power_manager_t;

static power_manager_t pm = {0};

void power_manager_update(void) {
    if (pm.active_tasks == 0) {
        pm.idle_time++;
        
        if (pm.idle_time > 1000) {  // 空闲1秒
            enter_power_mode(POWER_MODE_STOP);
        } else if (pm.idle_time > 100) {  // 空闲100ms
            enter_power_mode(POWER_MODE_SLEEP);
        }
    } else {
        pm.idle_time = 0;
    }
}

// 中断驱动的数据采集
volatile int data_ready = 0;
volatile int sensor_data;

void sensor_interrupt_handler(void) {
    sensor_data = ADC_DR;  // 读取ADC数据
    data_ready = 1;
    
    // 清除中断标志
    ADC_SR &= ~ADC_SR_EOC;
}

void low_power_data_acquisition(void) {
    // 配置ADC为中断模式
    ADC_CR1 |= ADC_CR1_EOCIE;  // 使能转换完成中断
    
    while (1) {
        if (data_ready) {
            // 处理数据
            process_sensor_data(sensor_data);
            data_ready = 0;
            
            // 启动下一次转换
            ADC_CR2 |= ADC_CR2_SWSTART;
        } else {
            // 进入低功耗模式等待中断
            enter_power_mode(POWER_MODE_SLEEP);
        }
    }
}

// DMA减少CPU负载
void setup_dma_transfer(void) {
    // 配置DMA从ADC到内存的传输
    DMA_CCR1 |= DMA_CCR1_CIRC;   // 循环模式
    DMA_CCR1 |= DMA_CCR1_MINC;   // 内存地址递增
    DMA_CCR1 |= DMA_CCR1_TCIE;   // 传输完成中断
    
    DMA_CPAR1 = (uint32_t)&ADC_DR;      // 外设地址
    DMA_CMAR1 = (uint32_t)adc_buffer;   // 内存地址
    DMA_CNDTR1 = BUFFER_SIZE;           // 传输数量
    
    DMA_CCR1 |= DMA_CCR1_EN;     // 启用DMA
}
```

**考察点**:
- 功耗管理策略
- 硬件特性理解
- 中断和DMA使用
- 系统级优化

---

## 调试与测试

### 题目15：嵌入式系统调试技术
**问题**: 在没有操作系统的嵌入式环境中如何进行调试？

**详细解答**:
```c
// 1. 串口调试输出
#include <stdarg.h>
#include <stdio.h>

void debug_uart_putchar(char c) {
    while (!(USART1_SR & USART_SR_TXE));
    USART1_DR = c;
}

void debug_printf(const char *format, ...) {
    char buffer[256];
    va_list args;
    
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    for (int i = 0; buffer[i]; i++) {
        debug_uart_putchar(buffer[i]);
    }
}

// 2. LED调试指示
#define DEBUG_LED_ERROR   0
#define DEBUG_LED_WARNING 1
#define DEBUG_LED_INFO    2

void debug_led_set(int led, int state) {
    if (state) {
        gpio_set_pin(GPIOC, led);
    } else {
        gpio_reset_pin(GPIOC, led);
    }
}

void debug_led_blink(int led, int count) {
    for (int i = 0; i < count; i++) {
        debug_led_set(led, 1);
        delay_ms(200);
        debug_led_set(led, 0);
        delay_ms(200);
    }
}

// 3. 断言机制
#ifdef DEBUG
#define ASSERT(expr) \
    do { \
        if (!(expr)) { \
            debug_printf("ASSERT FAILED: %s at %s:%d\n", \
                        #expr, __FILE__, __LINE__); \
            while(1) { \
                debug_led_blink(DEBUG_LED_ERROR, 3); \
                delay_ms(1000); \
            } \
        } \
    } while(0)
#else
#define ASSERT(expr) ((void)0)
#endif

// 4. 内存检查
void check_stack_overflow(void) {
    extern char _stack_end;
    volatile char stack_marker;
    
    if ((char*)&stack_marker < &_stack_end) {
        debug_printf("STACK OVERFLOW DETECTED!\n");
        ASSERT(0);
    }
}

// 5. 性能监控
typedef struct {
    uint32_t start_time;
    uint32_t max_time;
    uint32_t total_time;
    uint32_t count;
    const char *name;
} performance_counter_t;

#define MAX_COUNTERS 16
static performance_counter_t perf_counters[MAX_COUNTERS];
static int counter_count = 0;

int perf_counter_create(const char *name) {
    if (counter_count >= MAX_COUNTERS) return -1;
    
    perf_counters[counter_count].name = name;
    perf_counters[counter_count].max_time = 0;
    perf_counters[counter_count].total_time = 0;
    perf_counters[counter_count].count = 0;
    
    return counter_count++;
}

void perf_counter_start(int counter_id) {
    if (counter_id < 0 || counter_id >= counter_count) return;
    perf_counters[counter_id].start_time = get_system_tick();
}

void perf_counter_end(int counter_id) {
    if (counter_id < 0 || counter_id >= counter_count) return;
    
    uint32_t elapsed = get_system_tick() - perf_counters[counter_id].start_time;
    
    perf_counters[counter_id].total_time += elapsed;
    perf_counters[counter_id].count++;
    
    if (elapsed > perf_counters[counter_id].max_time) {
        perf_counters[counter_id].max_time = elapsed;
    }
}

void perf_counter_report(void) {
    debug_printf("\n=== Performance Report ===\n");
    for (int i = 0; i < counter_count; i++) {
        performance_counter_t *pc = &perf_counters[i];
        if (pc->count > 0) {
            uint32_t avg_time = pc->total_time / pc->count;
            debug_printf("%s: avg=%u, max=%u, count=%u\n", 
                        pc->name, avg_time, pc->max_time, pc->count);
        }
    }
}

// 6. 错误日志系统
#define LOG_BUFFER_SIZE 1024
#define MAX_LOG_ENTRIES 32

typedef struct {
    uint32_t timestamp;
    uint8_t level;
    char message[64];
} log_entry_t;

typedef enum {
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARNING = 1,
    LOG_LEVEL_INFO = 2,
    LOG_LEVEL_DEBUG = 3
} log_level_t;

static log_entry_t log_buffer[MAX_LOG_ENTRIES];
static int log_head = 0;
static int log_count = 0;

void log_message(log_level_t level, const char *format, ...) {
    va_list args;
    log_entry_t *entry = &log_buffer[log_head];
    
    entry->timestamp = get_system_tick();
    entry->level = level;
    
    va_start(args, format);
    vsnprintf(entry->message, sizeof(entry->message), format, args);
    va_end(args);
    
    log_head = (log_head + 1) % MAX_LOG_ENTRIES;
    if (log_count < MAX_LOG_ENTRIES) {
        log_count++;
    }
    
    // 实时输出到串口
    const char *level_str[] = {"ERROR", "WARN", "INFO", "DEBUG"};
    debug_printf("[%u] %s: %s\n", entry->timestamp, level_str[level], entry->message);
}

void log_dump(void) {
    debug_printf("\n=== Log Dump ===\n");
    int start = (log_head - log_count + MAX_LOG_ENTRIES) % MAX_LOG_ENTRIES;
    
    for (int i = 0; i < log_count; i++) {
        int idx = (start + i) % MAX_LOG_ENTRIES;
        log_entry_t *entry = &log_buffer[idx];
        const char *level_str[] = {"ERROR", "WARN", "INFO", "DEBUG"};
        debug_printf("[%u] %s: %s\n", entry->timestamp, level_str[entry->level], entry->message);
    }
}

// 7. 看门狗实现
#define WATCHDOG_TIMEOUT_MS 5000

static uint32_t watchdog_last_feed = 0;
static int watchdog_enabled = 0;

void watchdog_init(void) {
    // 配置独立看门狗
    IWDG_KR = 0x5555;  // 允许写入
    IWDG_PR = 0x06;    // 预分频器 /256
    IWDG_RLR = 0x0FFF; // 重载值
    IWDG_KR = 0xAAAA;  // 重载
    IWDG_KR = 0xCCCC;  // 启动
    
    watchdog_enabled = 1;
    watchdog_last_feed = get_system_tick();
}

void watchdog_feed(void) {
    if (watchdog_enabled) {
        IWDG_KR = 0xAAAA;  // 重载看门狗
        watchdog_last_feed = get_system_tick();
    }
}

void watchdog_check(void) {
    if (watchdog_enabled) {
        uint32_t now = get_system_tick();
        if (now - watchdog_last_feed > WATCHDOG_TIMEOUT_MS) {
            log_message(LOG_LEVEL_ERROR, "Watchdog timeout detected!");
            // 系统将复位
        }
    }
}

// 使用示例
void main_task(void) {
    int perf_counter = perf_counter_create("main_task");
    
    while (1) {
        perf_counter_start(perf_counter);
        
        // 检查栈溢出
        check_stack_overflow();
        
        // 执行主要逻辑
        process_data();
        
        // 检查看门狗
        watchdog_check();
        watchdog_feed();
        
        perf_counter_end(perf_counter);
        
        task_delay(100);
    }
}
```

**考察点**:
- 调试方法论
- 错误处理机制
- 性能分析
- 系统监控
- 故障诊断

---

## 总结

### 概念题答题技巧

**1. 结构化回答**
- 先给出定义或核心概念
- 然后解释工作原理
- 最后说明应用场景或意义
- 必要时举例说明

**2. 分类讨论**
- 将复杂概念分类讲解
- 比较不同方案的优缺点
- 说明适用场景

**3. 由浅入深**
- 从基本概念开始
- 逐步深入到技术细节
- 最后谈到实际应用

**4. 关联扩展**
- 将相关概念联系起来
- 展示知识的系统性
- 体现对技术的深度理解

### 常见概念题考点总结

**C语言基础（必考）：**
- volatile、const、static关键字
- 指针和内存管理
- 数据类型和内存对齐
- 函数指针和回调机制

**硬件相关（核心）：**
- 中断系统和优先级
- 时钟系统和定时器
- 存储器类型和特性
- 通信协议（SPI、I2C、UART、CAN）

**系统概念（重要）：**
- 实时系统特性
- 功耗管理技术
- 启动流程和引导
- 调试方法和工具

**高级话题（加分）：**
- 系统可靠性设计
- 安全和加密技术
- 网络通信协议
- 软件工程实践

### 面试准备策略

1. **理论基础扎实**
   - 深入理解计算机体系结构
   - 掌握操作系统核心概念
   - 熟悉数字电路和模拟电路基础

2. **实践经验丰富**
   - 动手实现各种算法和数据结构
   - 熟练使用调试工具和测试方法
   - 有实际项目开发经验

3. **问题解决能力**
   - 能够分析复杂问题并分解
   - 掌握多种优化技术
   - 具备系统性思维

4. **沟通表达清晰**
   - 能够清楚解释技术概念
   - 善于举例说明
   - 主动考虑边界条件和异常情况

### 常见面试流程

1. **基础知识问答** (30分钟)
   - C语言基础和高级特性
   - 数据结构和算法
   - 操作系统原理
   - 硬件相关知识

2. **编程实现** (45分钟)
   - 现场编写代码
   - 算法优化
   - 调试和测试

3. **系统设计** (30分钟)
   - 架构设计能力
   - 权衡分析
   - 扩展性考虑

4. **项目经验讨论** (15分钟)
   - 项目难点和解决方案
   - 技术选型理由
   - 团队协作经验

### 学习建议

1. **循序渐进**：从基础概念开始，逐步深入
2. **理论结合实践**：每个概念都要有代码实现
3. **多做练习**：大量的编程练习是必不可少的
4. **总结归纳**：定期总结学习成果和经验教训
5. **关注前沿**：了解最新的技术发展和行业趋势

希望这份面试题集能帮助您在嵌入式系统和固件开发的面试中取得成功！

