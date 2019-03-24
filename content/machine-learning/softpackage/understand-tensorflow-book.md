---
title: "升入理解TensorFlow"
layout: page
date: 2019-03-21
---
[TOC]

## 架构与设计
- 数据流图, 前向图和后向图, 前向和后向计算图是分开的
    - 节点, 操作 operator: 数学运算
    - 变量 Variable, 有内部状态
    - 占位符 placeholder, 用于输入数据
    - 梯度值
    - 更新参数操作
    - 更新后的参数
- 有向边: 
    - 数据边,传输数据
    - 控制依赖,控制执行顺序
- 执行原理:
    1. 创建散列表,节点名字做key,入度做value
    2. 创建可执行节点队列, 将入度为0的节点放入队列并从散列表中删除
    3. 依次执行队列中的节点, 每次执行都将执行节点的输出指向的所有节点的入度减1, 更新到散列表中
    4. 重复2-3直到所有节点都计算完毕
- 数据载体: 张量
    - Tensor, 张量实现的时候是通过句柄的方式, 实现复用, 通过引用计数实现内存释放.
        1. 属性: `name`, `dtype`, `graph`, `op`, `shape`, `value_index`(张量在该前置操作中所有输出值中的索引)
        2. 方法: `eval`, `get_shape`, `set_shape`, `consumers`(张量的后置操作) 
        3. 创建, `tf.constant` 以及操作 `tf.add` 都可以创建张量
        4. 执行, `sess.run` 和 `tensorf.eval`
    - SparseTensor, 稀疏张量, 包含 `indices` (N, ndims), `values` (N), `dense_shape` (ndims) 三个属性
        1. 创建, `tf.SparseTensor(indices=[[1],[3],[8]], values=[1,1,1], dense_shape=[10])`
        2. 操作, 
- 模型载体: 操作
    - 计算节点
        1. 属性: `name`, `type`, `inputs`, `control_inputs`, `outpus`, `device`, `graph`, `traceback`(调用栈)
        2. 典型操作
            - 算术: `add`, `multiply`, `mod`, `sqrt`, `sin`, `trace`, `fft`, `argmin` 已及numpy类似的矩阵操作
            - 数组: `size`, `rank`, `split`,`reverse`, `cast`, `one_hot`, `quantize`
            - 梯度裁剪: `clip_by_value`, `clip_by_norm`, `clip_by_global_norm`
            - 逻辑控制和调试: `identity`, `logical_and`, `equal`, `less`, `is_finite`, `is_nan`
            - 数据流控制: `enqueue`, `dequeue`, `size`, `take_grad`, `apply_grad`
            - 初始化操作: `zeros_initializer`, `random_normal_initializer`, `orthogonal_initializer`
            - 神经网络操作: `convolution`, `pool`, `dropout`
            - 随机运算: `random_normal`, `random_shuffle`
            - 字符串运算: `string_to_hash_bucket`, `reduce_join`, `substr`, `encode_base64`
            - 图像处理: `encode_png`, `resize_images`, `rot90`, `hsv_to_rgb`, `adjust_gamma`
    - 存储节点: 变量
        - 变量a其实由 `(a)`, `Assign`, `read`, `initial_value` 四个节点组成, `tf.add(a, b)`操作实际上读的是 `read` 子节点的值, 而 `tf.global_variables_initializer` 实际上是将 `initial_value` 传入 `Assign` 节点,实现初始化的
        - 变量操作, 支持两种初始化
            1. 用户指定的初始值
            2. VariableDef, 用protobuff定义的变量初始化, 用于继续训练场景
    - 数据节点: Placeholder
        - `tf.placeholder` , `tf.sparse_placeholder`
- 运行环境 : 会话
    - 普通会话, `sess = tf.Session(), sess.run, sess.close`
    - 通过 `tf.ConfigProto` 设置会话配置, 包括但不限于, GPU使用, 分布式环境RPC地址 etc
    - `with`语句可以不用指定session,就可以直接调用 `eval` 方法计算张量的值
    - 交互式会话 `tf.InteractiveSession` 会默认的将 `eval` 的会话设置为当前会话
    - `reset` 方法用于会话的资源释放
- 训练工具: 优化器
    - `_use_locking` 在并发更新的时候是否加锁
    - 子类实现 `_apply_dense` 和 `_apply_sparse` 方法, 这两个方法都返回数据流图上的操作
    - `minimize`方法调用 `compute_gradients` 和 `apply_gradients` 方法
    - `gate_gradients` 梯度计算的异步/同步控制
    - 更自由地控制优化过程
```python
grads_and_vars = optimizer.compute_gradients(loss)
for i, (g, v) in enumerate(grads_and_vars): 
    if g is not None: 
        grads_and_vars[i] = (tf.clip_by_norm(g, 5), v) # 裁剪梯度
train_op = optimizer.apply_gradients(grads_and_vars)
```

## 关键模块篇

### 数据处理方法
- 输入流水线
    - 创建文件名列表
        - python列表 或者 `tf.train.match_filenames_once`
    - 创建文件名队列
        - `tf.train.string_input_producer`
    - Reader 和 Decoder
        - CSV文件, `tf.TextLineReader`, `tf.decode_csv`
        - TFRecords 文件, `tf.TFRecordReader`, `tf.parse_single_example`
        - 自由格式, `tf.FixedLengthRecordReader`, `tf.decode_raw`
    - 创建样本队列
        - 使用 `tf.train.start_queue_runners` 启动后台线程读取队列
        - 使用后台线程协调器 `tf.train.Coordinator` 管理线程
        - 创建批量数据 `tf.train.shuffle_batch`
- 模型参数
    - 参数创建、初始化、更新  `tf.Variable` 实现
    - 模型文件存储和恢复 `tf.train.Saver` 实现
    - 初始化方法 `tf.global_variables_initilizer` 和 `tf.variables_initilizer(var_list)`, `var_list` 是变量集合, 创建变量时,可以通过 `collections` 参数指定不同的集合, 默认是 `GraphKeys.GLOBAL_VARIABLES`, 即全局变量集合, 如果显式指定 `trainable=True`,那么会加到 `TRAINABLE_VARABLES` 集合, 内置5类变量集合(最新版本支持更多了)
        - `GraphKeys.GLOBAL_VARIABLES`
        - `GraphKeys.LOCAL_VARIABLES`
        - `GraphKeys.TRAINABLE_VARABLES`
        - `GraphKeys.MODEL_VARIABLES`
        - `GraphKeys.MOVING_AVERAGE_VARIABLES`
        - `GraphKeys.REGULARIZATION_LOSSES`
    - 更新模型参数
        - 赋值, `tf.assign`, `tf.assign_add`, `tf.assign_sub`, 注意等于号没法实现变量赋值, 它只是创建了一个新的张量, 并没有改变变量的值
    - 使用 `tf.train.Saver` 保存和恢复模型
        - `saver = tf.train.Saver({'w' : W})` 要保存的模型, 通过变量名字来标示和恢复
        - `saver.restore`
    - 变量作用域 `tf.variable_scope`, `reuse` 参数表明是否可以复用, `initializer`指定该变量作用域下统一的初始化方法
- 命令行参数
    - `argparse` 和 `tf.app.flags`, 后者的好处: 自动生成使用方法信息, 自动生成帮组信息, 自动生成错误信息
    
    
### TensorFlow编程框架
- 显式创建数据流图 `tf.Graph()`, `with tf.Graph().as_default()` 语句添加作用域
- ps-worker: 模型分发,参数更新由PS实现; 模型推断和梯度计算由worker实现
- 分布式模型脚本需要从命令行参数获取集群配置参数
- `tf.ClusterSpec`, `tf.train.Server`
- 同步的梯度更新比异步的更快?
- 同步训练: 梯度聚合器
- 异步训练: 靠内部的锁机制实现
- supervisor: 管理模型训练
    - 定期保存模型到checkpoint
    - 重启的时候从checkpoint文件恢复,继续训练
    - 异常发生时,清理现场
    - 执行步骤
        1. 创建Supervisor实例,传入checkpoint文件路径和日志路径
        2. 获取会话实例 session
            - 检查点服务, 定期保存
            - 汇总服务, 汇总日志, 追加到logdir
            - 步数计数器
        3. 使用会话实例执行训练, 并检查停止条件
        
### TensorBoard
- `tf.Summary`
    - `audio`, `image`, `scalar`, `histogram`, `merge_all`
    - `FileWriter`
        - `add_summary`, `add_event`, `add_graph`
- 名字作用域与抽象节点, 抽象节点是一个子图, 通过name_scope可以将该scope下的所有节点自动汇聚到一个子图
- 汇总数据 summary.proto
- 折线图 `tf.summary.scalar(name, tensor)`, 汇总操作都会放到KEY:  GraphKeys.SUMMARIES 下
- 分布图 `tf.summary.histogram(name, tensor)`
- 图像 `tf.summary.image(name, tensor, max_outputs)`, tensor 是4阶张量 [batch_size, height, width, channels], channels 可取1(灰度图), 3(彩色图), 4(带Alpha通道的彩色图)
- 音频 `tf.summary.image(name, tensor, sample_rate, max_outputs)` tensor 是3阶张量 [batch_size, frames, channels] 或者2阶张量 [batch_size, frames], frames是音频的值-1到1之间 <https://magenta.tensorflow.org/>
- 可视化高维数据, embeddings: 支持t-SNE和PCA两种降维方式

## 模型托管 Tensorflow Serving
- 流水线
    - 持续训练, 即在线学习
    - 模型服务, gRPC协议
    - 客户端访问
- 自动感知模型更新

