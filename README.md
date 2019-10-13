# MotionViewer
 Data presentation for NDI and HoloLens based on Qt


# MotionViewer程序设计说明
## 一、概览
名称：MotionViewer
功能：
1. 采集NDI的capture点数据并做相应处理；
2. 将坐标数据发送给HoloLens；
3. 可视化HoloLens视频流与扫描设备（Kinect）视频流；
4. 配准设置与结果可视化；
	  注：*为扩展功能，暂时不实现，预留接口
依赖库：
1. Qt
2. OpenCV 视觉库
3. Eigen 矩阵运算库

## 二、具体设计

### 功能1
功能需求：
1. 串口或网口(预留)通信，同步收发指令（一问一答）；
2. 解码，根据NDI通信规则解码出各个marker的坐标(x,y,z)（Marker点大概在20个以内）；
3. 显示所有Marker点坐标
4. 通过下拉菜单选择需要计算方法，并点击按钮计算，将结果显示。显示标定件位姿（3-4个标定件）
5. 显示各标定件位置关系图(预留)