# NNblaster
AI Design Builder, AIoC, emulator

# 概述
“AI Design Builder”是由北京芯启科技有限公司（SiSCTECHnology CO.,LTD）所开发的AI片上系统软硬件方案，针对的是一方专用的AI硬件加速方案（PCBA、板卡、芯片、或片上子系统IP）。其中芯片上的硬件子系统部分AIoC属于芯片SoC架构“靖”级别的开发，其中包括“飒”级别IP例如为AI定制的SAR CPU（见首页repo列表）和张量MOA、矢量VOA、特殊计算SOA、特定DMA等加速IP，以及驱动和软件极其工具链的开发。这个AI片上子系统的软件评估将通过NNblaster来展示从神经网络算法到AIoC硬件运行结果等等丰富的信息，它的特点包括：
- 能够对应硬件IP精度，起到bit-by-bit仿真同样的效果，它的内部控制流与在SAR CPU上运行的完全一致，它底层调用的硬件模型就是各个IP的C model，从而支持在数学算法层面上的“所仿即所得”；
- 能够反应AIoC各IP的工作时间效率，比繁重的RTL仿真速度快，对时效的反应精度可达后者的85~96%，由此还支持精确性能和功耗分析、乃至信号波形分析。
- 灵活支持客户方案。

对于“方案”层面，我们定义了多个对接层（靖= 业务层+框架层+接口层+驱动层+硬件层），由多类“飒”级别IP和软件模块组成，便于拆卸和打包对接客户的各层开发或二次开发。源于我们从“飒”到“靖”扎实的经验了解，故我们没选择向开源IR或流行软件工具的对标为开发起点，但将其作为与客户对接、交付所可能借助的桥梁，从而给您“能参考、可改装”的技术服务。

在此我们将NNblaster做为一个子系统emulator的功能模块发放上来，还将会不断补充文档，展示其作为AI IP EDA工具的功用。在开放的代码中，您可以看到从NN demo到IP EDA的背后所需的工作，其中所调用的IR接口以及驱动层接口可见。祝您阅得灵思泉涌。祝您清鉴选择，洞察方案，买得放心。

# download: http://www.sisctech.cn/uploads/soft/sisc_security.tgz
# SiAiEm: SISCTECH AI EMULATION

SiAiEm : Sisctech AI Emulation
Contract:
Name: Sunlingge. email: sunlg@sisctech.cn.

SiAIEm is the sisctech ai emulation project.

Version: V100:
Proposal:
1. Support the YoloV3 ai network identification.
2. Support 1080p png file as input.
3. Support FP16 weights file and convert the FP32 weights file into FP16 file.
4. Support static memory allocation.
5. Support AI net layer memory reuse allocation to decrease the memory used.
6. Support cmodel golden data generation for sisctech ai chip core.
7. Support cmodel test for sistech ai chip core.
8. Support ai test for every individual layer. 
9. Support using only ansi c code but ui model.

Build:
1. You must download yolov3 weights from https://pjreddie.com/media/files/yolov3.weights.
2. Rename and locate the weights file into Config\AiNetConfigYoloV3\AiNetConfigYoloV3Coco\yolov3.weights.
3. If you use FP16, you may run the application in FP32 mode, and press [YoloV3][Weights][Convert] buttion to convert it.
4. Rename and locate the weights file into Config\AiNetConfigYoloV3\AiNetConfigYoloV3Coco\yolo3.f16.weights.
5. Open SiAiEm.sln in Microsoft visual studio 2015.
6. Open ai/AiBase/AiBaseDefine/ai_base_product.h
7. Modify the AI_PRODUCT_OPTION_ACCELARATOR_TYPE as AI_PRODUCT_ACCELERATOR_CPU or AI_PRODUCT_ACCELERATOR_SISCTECH_CMODEL1

	#define AI_PRODUCT_ACCELERATOR_CPU                (0)

	#define AI_PRODUCT_ACCELERATOR_SISCTECH_CMODEL1   (3)

	#define AI_PRODUCT_OPTION_ACCELERATOR_TYPE        (AI_PRODUCT_ACCELERATOR_SISCTECH_CMODEL1)

8. Modify the FLOAT_AI_T_TYPE_SIZE as AI_PRODUCT_FLOAT_SOFTWARE_FP16 or AI_PRODUCT_FLOAT_HARDWARE_FP32

	#define AI_PRODUCT_FLOAT_HARDWARE_FP32            (0)

	#define AI_PRODUCT_FLOAT_SOFTWARE_FP16            (1)

	#define FLOAT_AI_T_TYPE_SIZE                      (AI_PRODUCT_FLOAT_SOFTWARE_FP16)

9. Select the X64 release project type.
10. Build it.

Run：
1. Press [Browser] and select file Config\AiBaseAlphabetData\1080p.png in YoloV3 Ident tab.
2. Press [Ident] to run the identification function.
3. You may select the [Command] tab the view the sisctech ai command for every net layer.
4. You may select the [Float16] or [Float32] to view float value in the float16 or float32 data file.
5. You may select the [Weights] to convert the FP32 weights to FP16 weights and save it into file.
6. [Clock] tab will support in future.
7. All about usb and serial communication will support in future.
