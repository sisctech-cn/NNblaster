# NNblaster
AI Design Builder, AIoC, emulator

========================================================================
       SiAiEm: SISCTECH AI EMULATION
========================================================================

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
10.Build it.

Run：
1. Press [Browser] and select file Config\AiBaseAlphabetData\1080p.png in YoloV3 Ident tab.
2. Press [Ident] to run the identification function.
3. You may select the [Command] tab the view the sisctech ai command for every net layer.
4. You may select the [Float16] or [Float32] to view float value in the float16 or float32 data file.
5. You may select the [Weights] to convert the FP32 weights to FP16 weights and save it into file.
6. [Clock] tab will support in future.
7. All about usb and serial communication will support in future.
