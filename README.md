# ft 交易引擎
| Platform | Build                                                                                                               |
| -------- | ------------------------------------------------------------------------------------------------------------------- |
| Linux    | [![Build Status](https://travis-ci.com/DuckDuckDuck0/ft.svg?branch=master)](https://travis-ci.com/DuckDuckDuck0/ft) |

不断更新中，大家有什么想法或是发现了bug，可以一起在issue上面自由讨论，也欢迎大家加入到开发中。如有问题我将全力提供支持。  

目录
=================


- [ft 交易引擎](#ft-%e4%ba%a4%e6%98%93%e5%bc%95%e6%93%8e)
- [目录](#%e7%9b%ae%e5%bd%95)
  - [简单示例](#%e7%ae%80%e5%8d%95%e7%a4%ba%e4%be%8b)
  - [更新说明](#%e6%9b%b4%e6%96%b0%e8%af%b4%e6%98%8e)
  - [1. 简介](#1-%e7%ae%80%e4%bb%8b)
    - [1.1. ft是什么](#11-ft%e6%98%af%e4%bb%80%e4%b9%88)
    - [1.2. 基本架构](#12-%e5%9f%ba%e6%9c%ac%e6%9e%b6%e6%9e%84)
    - [1.3. 目录结构](#13-%e7%9b%ae%e5%bd%95%e7%bb%93%e6%9e%84)
      - [1.3.1. include](#131-include)
      - [1.3.2. Gateway](#132-gateway)
      - [1.3.3. TradingSystem](#133-tradingsystem)
      - [1.3.4. Tools](#134-tools)
      - [1.3.5. RiskManagement](#135-riskmanagement)
  - [2. 使用方式](#2-%e4%bd%bf%e7%94%a8%e6%96%b9%e5%bc%8f)
    - [2.1. 编译策略引擎及加载器](#21-%e7%bc%96%e8%af%91%e7%ad%96%e7%95%a5%e5%bc%95%e6%93%8e%e5%8f%8a%e5%8a%a0%e8%bd%bd%e5%99%a8)
    - [2.2. 配置登录信息](#22-%e9%85%8d%e7%bd%ae%e7%99%bb%e5%bd%95%e4%bf%a1%e6%81%af)
    - [2.3. 让示例跑起来](#23-%e8%ae%a9%e7%a4%ba%e4%be%8b%e8%b7%91%e8%b5%b7%e6%9d%a5)
  - [3. 开发你的第一个策略](#3-%e5%bc%80%e5%8f%91%e4%bd%a0%e7%9a%84%e7%ac%ac%e4%b8%80%e4%b8%aa%e7%ad%96%e7%95%a5)

## 简单示例
下面是一个简单的演示图，CTP simnow的一个简单策略示例，每收到一个价格变动就发出 一个下单指令，左边运行的是TradingEngine，右边则是Strategy

![image](img/example.jpg)

## 更新说明
| 更新时间  | 更新内容                                                                                                      |
| --------- | ------------------------------------------------------------------------------------------------------------- |
| 2020.5.24 | redis中交易相关的键值对都带上了account id的简称，在策略加载时也需要指定account id，表示该策略挂载在哪个账户上 |

## 1. 简介
### 1.1. ft是什么
ft是一个基于C++的低延迟交易引擎，为策略提供简便的开发、回测环境。实盘目前只支持CTP，后续将根据需求加入其他的接口。回测系统暂时也未提供，但其会尽快提上日程。ft引擎与策略分离，引擎采用c++编写，高效低时延，而策略通过redis与引擎进行交互，可使用python编写，简洁易部署。

### 1.2. 基本架构
ft采用策略引擎与策略分离的体系，即策略引擎与策略引擎是不同的进程，策略引擎为策略提供仓位、订单等操作支持，一个引擎可以对接多个策略，每个策略都分别跑在一个进程上。
![image](img/framework.jpg)

订单发送

![image](img/Trading.png)

数据驱动

![image](img/MarketDataFlow.png)

策略引擎登录及初始化流程

![image](img/登录流程.png)

### 1.3. 目录结构
#### 1.3.1. include
include里面存放的是公共头文件，用户如果想自己为某个服务商的行情、交易接口写Gateway，或是自己写一个交易引擎，就需要用到Core里面的内容。
* 如果需要自定义Gateway，至少需要用到两个头文件：Core/Gateway.h 以及 Core/TradingEngineInterface.h。Gateway.h里面是Gateway的基类，用户需要继承该基类并实现一些交易接口（如下单、撤单等）。同时，Gateway也要在按照约定去回调TradingEngineInterface中的函数（如收到回执时），TradingEngineInterface的子类就是交易引擎，用于仓位管理、订单管理等，通过回调告知交易状态或是查询信息。详情参考这两个头文件里的注释，以及CtpGateway的实现
* 如果需要自定义交易引擎，也至少需要：Core/Gateway.h 和 Core/TradingEngineInterface.h。交易引擎通过继承TradingEngineInterface获知账户的各种交易信息并管理之，而通过调用gateway的交易接口或是查询接口去主动发出交易请求或是查询请求。可参考TradingSystem目录中的实现
* Core里其他的文件定义的都是一些基本的交易相关数据结构
#### 1.3.2. Gateway
Gateway里是各个经纪商的交易网关的具体实现，可参考Ctp的实现来实现自己需要的网关
#### 1.3.3. TradingSystem
TradingSystem是本人实现的一个交易引擎，向上通过redis和策略进行交互，向下通过Gateway和交易所进行交互
#### 1.3.4. Tools
一些小工具，但是很必要。主要是ContractCollector，用于查询所有的合约信息并保存到本地，供其他组件使用
#### 1.3.5. RiskManagement
风险管理。考虑是否要合并到其他目录

## 2. 使用方式
### 2.1. 编译策略引擎及加载器
策略引擎的源码文件为TradingEngine.cpp，策略加载器的源码为StrategyLoader.cpp，使用cmake进行编译
```bash
mkdir build && cd build
cmake .. && make -j4
```

### 2.2. 配置登录信息
```yml
# 可以参考config/login.yml
api: ctp  # api name.
front_addr: tcp://180.168.146.187:10130
md_server_addr: tcp://180.168.146.187:10131
broker_id: 9999
investor_id: 123456
passwd: 12345678
auth_code: 0000000000000000
app_id: simnow_client_test
subscription_list: rb2009,rb2005  # subscribed list (for market data).
```

### 2.3. 让示例跑起来
这里提供了一个网格策略的demo
```bash
# 在terminal 0 启动策略引擎
redis-server  # 启动redis，必须在启动策略引擎前启动redis
./trading-engine --loglevel=debug --login-config=../config/ctp_config.yml --contracts-file=../config/contracts.csv
```
```bash
# 在terminal 1 启动策略
./strategy-loader -l libgrid-strategy.so -loglevel=debug --contracts-file=../config/contracts.csv --account=1234 --id=grid001
```

## 3. 开发你的第一个策略
```c++
// MyStrategy.cpp

#include <Strategy/Strategy.h>

class MyStrategy : public ft::Strategy {
 public:
  // 策略加载完后回调
  bool on_init() override {
     // 订阅感兴趣的数据
     // 订阅之后才会在有新的行情数据后收到对应的on_tick回调
     subscribe({"rb2009"});  // 可以同时订阅多个合约
  }

  // tick数据到来时回调
  void on_tick(const ft::TickData* tick) override {
    buy_open("rb2009", 1, tick->ask[0]);
  }

  // 收到本策略发出的订单的订单回报
  void on_order_rsp(const ft::OrderResponse* order) override {
    // do sth.
  }

  void on_exit() override {
    // 暂时还没用到
  }
};

EXPORT_STRATEGY(MyStrategy);  // 导出你的策略
```
把上面的代码像网格策略demo一样编译即可通过strategy-loader进行加载了
