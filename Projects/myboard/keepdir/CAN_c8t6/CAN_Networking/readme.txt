/**
  该例子测试通过。
  用PB8 PB9接TJA1050
  另一端是一个叫USB-CAN-V3的模组，改模组需搭配上位机修改参数，如果熟悉模组的协议，纯uart也可使用模组。
  上位机端，需设置参数，使用标准帧，帧ID 00 00 03 21，数据两字节，02 00,00 01.
  上位机的can baud是1000k
  模组的TR可以不用接，文档写 canl和tr相接是使用120欧电阻，但是我的tja1050已经有了，不接也可以通信

  
  发数据还有点问题
  txmailbox不变为0，
  上位机收不到数据，
  多次发送后，进入error死循环


  可疑点：
  TransmitGlobalTime
  Txispending
  txfifofreelevel
  
  接5v供电后正常了
  
  500k bps can ,
  c8t6只有一个按键，按下后，控制远程板的led
  远程板按下按键，c8t6灯切换

  c8t6 id:0x321,  远程板 id:0x322

 */
