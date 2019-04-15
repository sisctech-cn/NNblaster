/**
 ******************************************************************************
 * file    ai_net_config.h
 * author  sunlingge
 * version V100
 * brief   the ai net config driver.
 ******************************************************************************
 */
#ifndef AI_NET_CONFIG_H
#define AI_NET_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_NET_CONFIG_LINE_LENGTH_MAX											(1024)
#define AI_NET_CONFIG_SECTION_MAX												(110)
/* set type */
#define AI_NET_CONFIG_SET_BY_NONE												(0)
#define AI_NET_CONFIG_SET_BY_DEFAULT											(1)
#define AI_NET_CONFIG_SET_BY_CUSTOM												(2)
/* secion name */
#define AI_NET_CONFIG_SECTION_NET_NAME											("[net]")
#define AI_NET_CONFIG_SECTION_CONVOLUTIONAL_NAME								("[convolutional]")
#define AI_NET_CONFIG_SECTION_SHORTCUT_NAME										("[shortcut]")
#define AI_NET_CONFIG_SECTION_YOLO_NAME											("[yolo]")
#define AI_NET_CONFIG_SECTION_ROUTE_NAME										("[route]")
#define AI_NET_CONFIG_SECTION_UPSAMPLE_NAME										("[upsample]")
#define AI_NET_CONFIG_SECTION_NETWORK_NAME										("[network]")
#define AI_NET_CONFIG_SECTION_DECONVOLUTIONAL_NAME								("[deconvolutional]")
#define AI_NET_CONFIG_SECTION_CONNECTED_NAME									("[connected]")
#define AI_NET_CONFIG_SECTION_MAXPOOL_NAME										("[maxpool]")
#define AI_NET_CONFIG_SECTION_SOFTMAX_NAME										("[softmax]")
#define AI_NET_CONFIG_SECTION_DETECTION_NAME									("[detection]")
#define AI_NET_CONFIG_SECTION_DROPOUT_NAME										("[dropout]")
#define AI_NET_CONFIG_SECTION_CROP_NAME											("[crop]")
#define AI_NET_CONFIG_SECTION_COST_NAME											("[cost]")
#define AI_NET_CONFIG_SECTION_NORMALIZATION_NAME								("[normalization]")
#define AI_NET_CONFIG_SECTION_AVGPOOL_NAME										("[avgpool]")
#define AI_NET_CONFIG_SECTION_LOCAL_NAME										("[local]")
#define AI_NET_CONFIG_SECTION_ACTIVE_NAME										("[active]")
#define AI_NET_CONFIG_SECTION_RNN_NAME											("[rnn]")
#define AI_NET_CONFIG_SECTION_GRU_NAME											("[gru]")
#define AI_NET_CONFIG_SECTION_CRNN_NAME											("[crnn]")
#define AI_NET_CONFIG_SECTION_BATCHNORM_NAME									("[batchnorm]")
#define AI_NET_CONFIG_SECTION_XNOR_NAME											("[xnor]")
#define AI_NET_CONFIG_SECTION_REGION_NAME										("[region]")
#define AI_NET_CONFIG_SECTION_REORG_NAME										("[reorg]")
#define AI_NET_CONFIG_SECTION_REORG_OLD_NAME									("[reorg_old]")
#define AI_NET_CONFIG_SECTION_BLANK_NAME										("[blank]")
/* section type */
#define AI_NET_CONFIG_SECTION_NET_TYPE											(0)
#define AI_NET_CONFIG_SECTION_CONVOLUTIONAL_TYPE								(1)
#define AI_NET_CONFIG_SECTION_SHORTCUT_TYPE										(2)
#define AI_NET_CONFIG_SECTION_YOLO_TYPE											(3)
#define AI_NET_CONFIG_SECTION_ROUTE_TYPE										(4)
#define AI_NET_CONFIG_SECTION_UPSAMPLE_TYPE										(5)
#define AI_NET_CONFIG_SECTION_NETWORK_TYPE										(6)
#define AI_NET_CONFIG_SECTION_DECONVOLUTIONAL_TYPE								(7)
#define AI_NET_CONFIG_SECTION_CONNECTED_TYPE									(8)
#define AI_NET_CONFIG_SECTION_MAXPOOL_TYPE										(9)
#define AI_NET_CONFIG_SECTION_SOFTMAX_TYPE										(10)
#define AI_NET_CONFIG_SECTION_DETECTION_TYPE									(11)
#define AI_NET_CONFIG_SECTION_DROPOUT_TYPE										(12)
#define AI_NET_CONFIG_SECTION_CROP_TYPE											(13)
#define AI_NET_CONFIG_SECTION_COST_TYPE											(14)
#define AI_NET_CONFIG_SECTION_NORMALIZATION_TYPE								(15)
#define AI_NET_CONFIG_SECTION_AVGPOOL_TYPE										(16)
#define AI_NET_CONFIG_SECTION_LOCAL_TYPE										(17)
#define AI_NET_CONFIG_SECTION_ACTIVE_TYPE										(18)
#define AI_NET_CONFIG_SECTION_RNN_TYPE											(19)
#define AI_NET_CONFIG_SECTION_GRU_TYPE											(20)
#define AI_NET_CONFIG_SECTION_CRNN_TYPE											(21)
#define AI_NET_CONFIG_SECTION_BATCHNORM_TYPE									(22)
#define AI_NET_CONFIG_SECTION_XNOR_TYPE											(23)
#define AI_NET_CONFIG_SECTION_REGION_TYPE										(24)
#define AI_NET_CONFIG_SECTION_REORG_TYPE										(25)
#define AI_NET_CONFIG_SECTION_REORG_OLD_TYPE									(26)
#define AI_NET_CONFIG_SECTION_BLANK_TYPE										(27)
/* option value array max */
#define AI_NET_CONFIG_OPTION_VALUE_ARRAY_NUMBER_MAX								(20)
#define AI_NET_CONFIG_OPTION_VALUE_VALID										(1)
#define AI_NET_CONFIG_OPTION_VALUE_INVALID										(0)
/* option value type */
#define AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER									(0)	
#define AI_NET_CONFIG_OPTION_VALUE_TYPE_FLOAT									(1)
#define AI_NET_CONFIG_OPTION_VALUE_TYPE_ENUM									(2)
#define AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER_ARRAY							(3)
#define AI_NET_CONFIG_OPTION_VALUE_TYPE_FLOAT_ARRAY								(4)
#define AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER_SIGNED							(5)
#define AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER_SIGNED_ARRAY						(6)
#define AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER_ENUM								(7)
/* options for net section */
/* option batch */
#define AI_NET_CONFIG_OPTION_KEY_BATCH_NAME										("batch")
#define AI_NET_CONFIG_OPTION_KEY_BATCH_TYPE										(0)
#define AI_NET_CONFIG_OPTION_VALUE_BATCH_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_BATCH_DEFAULT								((UINT8_T *)"1")
/* option subdivision */
#define AI_NET_CONFIG_OPTION_KEY_SUBDIVISIONS_NAME								("subdivisions")
#define AI_NET_CONFIG_OPTION_KEY_SUBDIVISIONS_TYPE								(1)
#define AI_NET_CONFIG_OPTION_VALUE_SUBDIVISIONS_TYPE							(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_SUBDIVISIONS_DEFAULT							((UINT8_T *)"1")
/* option width */
#define AI_NET_CONFIG_OPTION_KEY_WIDTH_NAME										("width")
#define AI_NET_CONFIG_OPTION_KEY_WIDTH_TYPE										(2)
#define AI_NET_CONFIG_OPTION_VALUE_WIDTH_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_WIDTH_DEFAULT								((UINT8_T *)"0")
/* option height */
#define AI_NET_CONFIG_OPTION_KEY_HEIGHT_NAME									("height")
#define AI_NET_CONFIG_OPTION_KEY_HEIGHT_TYPE									(3)
#define AI_NET_CONFIG_OPTION_VALUE_HEIGHT_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_HEIGHT_DEFAULT								((UINT8_T *)"0")
/* option channels */
#define AI_NET_CONFIG_OPTION_KEY_CHANNELS_NAME									("channels")
#define AI_NET_CONFIG_OPTION_KEY_CHANNELS_TYPE									(4)
#define AI_NET_CONFIG_OPTION_VALUE_CHANNELS_TYPE								(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_CHANNELS_DEFAULT								((UINT8_T *)"0")
/* option momentum */
#define AI_NET_CONFIG_OPTION_KEY_MOMENTUM_NAME									("momentum")
#define AI_NET_CONFIG_OPTION_KEY_MOMENTUM_TYPE									(5)
#define AI_NET_CONFIG_OPTION_VALUE_MOMENTUM_TYPE								(AI_NET_CONFIG_OPTION_VALUE_TYPE_FLOAT)
#define AI_NET_CONFIG_OPTION_VALUE_MOMENTUM_DEFAULT								((UINT8_T *)"0.9")
/* option decay */
#define AI_NET_CONFIG_OPTION_KEY_DECAY_NAME										("decay")
#define AI_NET_CONFIG_OPTION_KEY_DECAY_TYPE										(6)
#define AI_NET_CONFIG_OPTION_VALUE_DECAY_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_FLOAT)
#define AI_NET_CONFIG_OPTION_VALUE_DECAY_DEFAULT								((UINT8_T *)"0.0001")
/* option angle */
#define AI_NET_CONFIG_OPTION_KEY_ANGLE_NAME										("angle")
#define AI_NET_CONFIG_OPTION_KEY_ANGLE_TYPE										(7)
#define AI_NET_CONFIG_OPTION_VALUE_ANGLE_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_ANGLE_DEFAULT								((UINT8_T *)"0")
/* option saturation */
#define AI_NET_CONFIG_OPTION_KEY_SATURATION_NAME								("saturation")
#define AI_NET_CONFIG_OPTION_KEY_SATURATION_TYPE								(8)
#define AI_NET_CONFIG_OPTION_VALUE_SATURATION_TYPE								(AI_NET_CONFIG_OPTION_VALUE_TYPE_FLOAT)
#define AI_NET_CONFIG_OPTION_VALUE_SATURATION_DEFAULT							((UINT8_T *)"1")
/* option exposure */
#define AI_NET_CONFIG_OPTION_KEY_EXPOSURE_NAME									("exposure")
#define AI_NET_CONFIG_OPTION_KEY_EXPOSURE_TYPE									(9)
#define AI_NET_CONFIG_OPTION_VALUE_EXPOSURE_TYPE								(AI_NET_CONFIG_OPTION_VALUE_TYPE_FLOAT)
#define AI_NET_CONFIG_OPTION_VALUE_EXPOSURE_DEFAULT								((UINT8_T *)"1")
/* option hue */
#define AI_NET_CONFIG_OPTION_KEY_HUE_NAME										("hue")
#define AI_NET_CONFIG_OPTION_KEY_HUE_TYPE										(20)
#define AI_NET_CONFIG_OPTION_VALUE_HUE_TYPE										(AI_NET_CONFIG_OPTION_VALUE_TYPE_FLOAT)
#define AI_NET_CONFIG_OPTION_VALUE_HUE_DEFAULT									((UINT8_T *)"1")
/* option learning_rate */
#define AI_NET_CONFIG_OPTION_KEY_LEARNING_RATE_NAME								("learning_rate")
#define AI_NET_CONFIG_OPTION_KEY_LEARNING_RATE_TYPE								(21)
#define AI_NET_CONFIG_OPTION_VALUE_LEARNING_RATE_TYPE							(AI_NET_CONFIG_OPTION_VALUE_TYPE_FLOAT)
#define AI_NET_CONFIG_OPTION_VALUE_LEARNING_RATE_DEFAULT						((UINT8_T *)"0.001")
/* option burn_in */
#define AI_NET_CONFIG_OPTION_KEY_BURN_IN_NAME									("burn_in")
#define AI_NET_CONFIG_OPTION_KEY_BURN_IN_TYPE									(22)
#define AI_NET_CONFIG_OPTION_VALUE_BURN_IN_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_BURN_IN_DEFAULT								((UINT8_T *)"0")
/* option max_batches */
#define AI_NET_CONFIG_OPTION_KEY_MAX_BATCHS_NAME								("max_batches")
#define AI_NET_CONFIG_OPTION_KEY_MAX_BATCHS_TYPE								(23)
#define AI_NET_CONFIG_OPTION_VALUE_MAX_BATCHS_TYPE								(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_MAX_BATCHS_DEFAULT							((UINT8_T *)"0")
/* option policy */
#define AI_NET_CONFIG_OPTION_KEY_POLICY_NAME									("policy")
#define AI_NET_CONFIG_OPTION_KEY_POLICY_TYPE									(24)
#define AI_NET_CONFIG_OPTION_VALUE_POLICY_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_ENUM)
#define AI_NET_CONFIG_OPTION_VALUE_POLICY_CONSTANT_NAME							("constant")
#define AI_NET_CONFIG_OPTION_VALUE_POLICY_STEP_NAME								("step")
#define AI_NET_CONFIG_OPTION_VALUE_POLICY_EXP_NAME								("exp")
#define AI_NET_CONFIG_OPTION_VALUE_POLICY_POLY_NAME								("poly")
#define AI_NET_CONFIG_OPTION_VALUE_POLICY_STEPS_NAME							("steps")
#define AI_NET_CONFIG_OPTION_VALUE_POLICY_SIGMOID_NAME							("sigmoid")
#define AI_NET_CONFIG_OPTION_VALUE_POLICY_RANDOM_NAME							("random")
#define AI_NET_CONFIG_OPTION_VALUE_POLICY_CONSTANT								((UINT8_T *)"0")
#define AI_NET_CONFIG_OPTION_VALUE_POLICY_STEP									((UINT8_T *)"1")
#define AI_NET_CONFIG_OPTION_VALUE_POLICY_EXP									((UINT8_T *)"2")
#define AI_NET_CONFIG_OPTION_VALUE_POLICY_POLY									((UINT8_T *)"3")
#define AI_NET_CONFIG_OPTION_VALUE_POLICY_STEPS									((UINT8_T *)"4")
#define AI_NET_CONFIG_OPTION_VALUE_POLICY_SIGMOID								((UINT8_T *)"5")
#define AI_NET_CONFIG_OPTION_VALUE_POLICY_RANDOM								((UINT8_T *)"6")
#define AI_NET_CONFIG_OPTION_VALUE_POLICY_DEFAULT								((UINT8_T *)"0")
/* option step */
#define AI_NET_CONFIG_OPTION_KEY_STEP_NAME										("step")
#define AI_NET_CONFIG_OPTION_KEY_STEP_TYPE										(25)
#define AI_NET_CONFIG_OPTION_VALUE_STEP_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_STEP_DEFAULT									((UINT8_T *)"1")
/* option scale */
#define AI_NET_CONFIG_OPTION_KEY_SCALE_NAME										("scale")
#define AI_NET_CONFIG_OPTION_KEY_SCALE_TYPE										(26)
#define AI_NET_CONFIG_OPTION_VALUE_SCALE_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_FLOAT)
#define AI_NET_CONFIG_OPTION_VALUE_SCALE_DEFAULT								((UINT8_T *)"1")
/* option steps */
#define AI_NET_CONFIG_OPTION_KEY_STEPS_NAME										("steps")
#define AI_NET_CONFIG_OPTION_KEY_STEPS_TYPE										(27)
#define AI_NET_CONFIG_OPTION_VALUE_STEPS_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER_ARRAY)
#define AI_NET_CONFIG_OPTION_VALUE_STEPS_ARRAY_LENGTH_MAX						(2)
/* option scales */
#define AI_NET_CONFIG_OPTION_KEY_SCALES_NAME									("scales")
#define AI_NET_CONFIG_OPTION_KEY_SCALES_TYPE									(28)
#define AI_NET_CONFIG_OPTION_VALUE_SCALES_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_FLOAT_ARRAY)
#define AI_NET_CONFIG_OPTION_VALUE_SCALES_ARRAY_LENGTH_MAX						(2)
/* option gamma */
#define AI_NET_CONFIG_OPTION_KEY_GAMMA_NAME										("gamma")
#define AI_NET_CONFIG_OPTION_KEY_GAMMA_TYPE										(30)
#define AI_NET_CONFIG_OPTION_VALUE_GAMMA_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_FLOAT)
#define AI_NET_CONFIG_OPTION_VALUE_GAMMA_DEFAULT								((UINT8_T *)"1")
/* option adam */
#define AI_NET_CONFIG_OPTION_KEY_ADAM_NAME										("adam")
#define AI_NET_CONFIG_OPTION_KEY_ADAM_TYPE										(31)
#define AI_NET_CONFIG_OPTION_VALUE_ADAM_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_ADAM_DEFAULT									((UINT8_T *)"0")
/* option b1 */
#define AI_NET_CONFIG_OPTION_KEY_B1_NAME										("B1")
#define AI_NET_CONFIG_OPTION_KEY_B1_TYPE										(32)
#define AI_NET_CONFIG_OPTION_VALUE_B1_TYPE										(AI_NET_CONFIG_OPTION_VALUE_TYPE_FLOAT)
#define AI_NET_CONFIG_OPTION_VALUE_B1_DEFAULT									((UINT8_T *)"0.9")
/* option b2 */
#define AI_NET_CONFIG_OPTION_KEY_B2_NAME										("B2")
#define AI_NET_CONFIG_OPTION_KEY_B2_TYPE										(33)
#define AI_NET_CONFIG_OPTION_VALUE_B2_TYPE										(AI_NET_CONFIG_OPTION_VALUE_TYPE_FLOAT)
#define AI_NET_CONFIG_OPTION_VALUE_B2_DEFAULT									((UINT8_T *)"0.999")
/* option eps */
#define AI_NET_CONFIG_OPTION_KEY_EPS_NAME										("eps")
#define AI_NET_CONFIG_OPTION_KEY_EPS_TYPE										(34)
#define AI_NET_CONFIG_OPTION_VALUE_EPS_TYPE										(AI_NET_CONFIG_OPTION_VALUE_TYPE_FLOAT)
#define AI_NET_CONFIG_OPTION_VALUE_EPS_DEFAULT									((UINT8_T *)"0.000001")
/* option time_steps */
#define AI_NET_CONFIG_OPTION_KEY_TIME_STEPS_NAME								("time_steps")
#define AI_NET_CONFIG_OPTION_KEY_TIME_STEPS_TYPE								(35)
#define AI_NET_CONFIG_OPTION_VALUE_TIME_STEPS_TYPE								(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_TIME_STEPS_DEFAULT							((UINT8_T *)"1")
/* option inputs */
#define AI_NET_CONFIG_OPTION_KEY_INPUTS_NAME									("inputs")
#define AI_NET_CONFIG_OPTION_KEY_INPUTS_TYPE									(36)
#define AI_NET_CONFIG_OPTION_VALUE_INPUTS_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_INPUTS_DEFAULT								((UINT8_T *)"0")
/* option max_crop */
#define AI_NET_CONFIG_OPTION_KEY_MAX_CROP_NAME									("max_crop")
#define AI_NET_CONFIG_OPTION_KEY_MAX_CROP_TYPE									(37)
#define AI_NET_CONFIG_OPTION_VALUE_MAX_CROP_TYPE								(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_MAX_CROP_DEFAULT								((UINT8_T *)"0")
/* option min_crop */
#define AI_NET_CONFIG_OPTION_KEY_MIN_CROP_NAME									("min_crop")
#define AI_NET_CONFIG_OPTION_KEY_MIN_CROP_TYPE									(38)
#define AI_NET_CONFIG_OPTION_VALUE_MIN_CROP_TYPE								(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_MIN_CROP_DEFAULT								((UINT8_T *)"0")
/* option flip */
#define AI_NET_CONFIG_OPTION_KEY_FLIP_NAME										("flip")
#define AI_NET_CONFIG_OPTION_KEY_FLIP_TYPE										(39)
#define AI_NET_CONFIG_OPTION_VALUE_FLIP_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_FLIP_DEFAULT									((UINT8_T *)"1")
/* option small_object */
#define AI_NET_CONFIG_OPTION_KEY_SMALL_OBJECT_NAME								("small_object")
#define AI_NET_CONFIG_OPTION_KEY_SMALL_OBJECT_TYPE								(40)
#define AI_NET_CONFIG_OPTION_VALUE_SMALL_OBJECT_TYPE							(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_SMALL_OBJECT_DEFAULT							((UINT8_T *)"0")
/* option power */
#define AI_NET_CONFIG_OPTION_KEY_POWER_NAME										("power")
#define AI_NET_CONFIG_OPTION_KEY_POWER_TYPE										(41)
#define AI_NET_CONFIG_OPTION_VALUE_POWER_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_POWER_DEFAULT								((UINT8_T *)"4")
/* option aspect */
#define AI_NET_CONFIG_OPTION_KEY_ASPECT_NAME									("aspect")
#define AI_NET_CONFIG_OPTION_KEY_ASPECT_TYPE									(41)
#define AI_NET_CONFIG_OPTION_VALUE_ASPECT_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_ASPECT_DEFAULT								((UINT8_T *)"1")
/* options for convolutional section */
/* option batch_normalize */
#define AI_NET_CONFIG_OPTION_KEY_BATCH_NORMALIZE_NAME							("batch_normalize")
#define AI_NET_CONFIG_OPTION_KEY_BATCH_NORMALIZE_TYPE							(50)
#define AI_NET_CONFIG_OPTION_VALUE_BATCH_NORMALIZE_TYPE							(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER_ENUM)
#define AI_NET_CONFIG_OPTION_VALUE_BATCH_NORMALIZE_ENABLE						(1)
#define AI_NET_CONFIG_OPTION_VALUE_BATCH_NORMALIZE_DISABLE						(0)
#define AI_NET_CONFIG_OPTION_VALUE_BATCH_NORMALIZE_DEFAULT						((UINT8_T *)"0")
/* option binary */
#define AI_NET_CONFIG_OPTION_KEY_BINARY_NAME									("binary")
#define AI_NET_CONFIG_OPTION_KEY_BINARY_TYPE									(51)
#define AI_NET_CONFIG_OPTION_VALUE_BINARY_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER_ENUM)
#define AI_NET_CONFIG_OPTION_VALUE_BINARY_ENABLE								(1)
#define AI_NET_CONFIG_OPTION_VALUE_BINARY_DISABLE								(0)
#define AI_NET_CONFIG_OPTION_VALUE_BINARY_DEFAULT								((UINT8_T *)"0")
/* option xnor */
#define AI_NET_CONFIG_OPTION_KEY_XNOR_NAME										("xnor")
#define AI_NET_CONFIG_OPTION_KEY_XNOR_TYPE										(52)
#define AI_NET_CONFIG_OPTION_VALUE_XNOR_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER_ENUM)
#define AI_NET_CONFIG_OPTION_VALUE_XNOR_ENABLE									(1)
#define AI_NET_CONFIG_OPTION_VALUE_XNOR_DISABLE									(0)
#define AI_NET_CONFIG_OPTION_VALUE_XNOR_DEFAULT									((UINT8_T *)"0")
/* option bin output */
#define AI_NET_CONFIG_OPTION_KEY_BIN_OUTPUT_NAME								("bin_output")
#define AI_NET_CONFIG_OPTION_KEY_BIN_OUTPUT_TYPE								(53)
#define AI_NET_CONFIG_OPTION_VALUE_BIN_OUTPUT_TYPE								(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER_ENUM)
#define AI_NET_CONFIG_OPTION_VALUE_BIN_OUTPUT_ENABLE							(1)
#define AI_NET_CONFIG_OPTION_VALUE_BIN_OUTPUT_DISABLE							(0)
#define AI_NET_CONFIG_OPTION_VALUE_BIN_OUTPUT_DEFAULT							((UINT8_T *)"0")
/* option filters */
#define AI_NET_CONFIG_OPTION_KEY_FILTERS_NAME									("filters")
#define AI_NET_CONFIG_OPTION_KEY_FILTERS_TYPE									(54)
#define AI_NET_CONFIG_OPTION_VALUE_FILTERS_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_FILTERS_DEFAULT								((UINT8_T *)"1")
/* option size */
#define AI_NET_CONFIG_OPTION_KEY_SIZE_NAME										("size")
#define AI_NET_CONFIG_OPTION_KEY_SIZE_TYPE										(55)
#define AI_NET_CONFIG_OPTION_VALUE_SIZE_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_SIZE_DEFAULT									((UINT8_T *)"1")
/* option stride */
#define AI_NET_CONFIG_OPTION_KEY_STRIDE_NAME									("stride")
#define AI_NET_CONFIG_OPTION_KEY_STRIDE_TYPE									(56)
#define AI_NET_CONFIG_OPTION_VALUE_STRIDE_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER_SIGNED)
#define AI_NET_CONFIG_OPTION_VALUE_STRIDE_DEFAULT								((UINT8_T *)"1")
#define AI_NET_CONFIG_OPTION_VALUE_STRIDE_2										((UINT8_T *)"2")
/* option pad */
#define AI_NET_CONFIG_OPTION_KEY_PAD_NAME										("pad")
#define AI_NET_CONFIG_OPTION_KEY_PAD_TYPE										(57)
#define AI_NET_CONFIG_OPTION_VALUE_PAD_TYPE										(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_PAD_DEFAULT									((UINT8_T *)"0")
/* option padding */
#define AI_NET_CONFIG_OPTION_KEY_PADDING_NAME									("padding")
#define AI_NET_CONFIG_OPTION_KEY_PADDING_TYPE									(58)
#define AI_NET_CONFIG_OPTION_VALUE_PADDING_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_PADDING_DEFAULT								((UINT8_T *)"0")
/* option activation */
#define AI_NET_CONFIG_OPTION_KEY_ACTIVATION_NAME								("activation")
#define AI_NET_CONFIG_OPTION_KEY_ACTIVATION_TYPE								(59)
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_TYPE								(AI_NET_CONFIG_OPTION_VALUE_TYPE_ENUM)
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LOGISTIC_NAME						("logistic")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LOGISTIC							((UINT8_T *)"0")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_RELU_NAME							("relu")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_RELU								((UINT8_T *)"1")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_RELIE_NAME						("relie")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_RELIE								((UINT8_T *)"2")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LINEAR_NAME						("linear")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LINEAR							((UINT8_T *)"3")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_RAMP_NAME							("ramp")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_RAMP								((UINT8_T *)"4")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_TANH_NAME							("tanh")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_TANH								((UINT8_T *)"5")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_PLSE_NAME							("plse")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_PLSE								((UINT8_T *)"6")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LEAKY_NAME						("leaky")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LEAKY								((UINT8_T *)"7")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_ELU_NAME							("elu")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_ELU								((UINT8_T *)"8")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LOGGY_NAME						("loggy")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LOGGY								((UINT8_T *)"9")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_STAIR_NAME						("stair")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_STAIR								((UINT8_T *)"10")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_HARDTAN_NAME						("hardtan")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_HARDTAN							((UINT8_T *)"11")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LHTAN_NAME						("lhtan")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LHTAN								((UINT8_T *)"12")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_SELU_NAME							("selu")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_SELU								((UINT8_T *)"13")
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_DEFAULT							(AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LOGISTIC)

/* option flipped  */
#define AI_NET_CONFIG_OPTION_KEY_FLIPPED_NAME									("flipped")
#define AI_NET_CONFIG_OPTION_KEY_FLIPPED_TYPE									(60)
#define AI_NET_CONFIG_OPTION_VALUE_FLIPPED_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_FLIPPED_DEFAULT								((UINT8_T *)"0")
/* option dot  */
#define AI_NET_CONFIG_OPTION_KEY_DOT_NAME										("dot")
#define AI_NET_CONFIG_OPTION_KEY_DOT_TYPE										(61)
#define AI_NET_CONFIG_OPTION_VALUE_DOT_TYPE										(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_DOT_DEFAULT									((UINT8_T *)"0")
/* options for shorcut section */
/* option map not support */
/* option from */
#define AI_NET_CONFIG_OPTION_KEY_FROM_NAME										("from")
#define AI_NET_CONFIG_OPTION_KEY_FROM_TYPE										(70)
#define AI_NET_CONFIG_OPTION_VALUE_FROM_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER_SIGNED)
#define AI_NET_CONFIG_OPTION_VALUE_FROM_DEFAULT									((UINT8_T *)"0")
/* options for yolo section */
/* option mask */
#define AI_NET_CONFIG_OPTION_KEY_MASK_NAME										("mask")
#define AI_NET_CONFIG_OPTION_KEY_MASK_TYPE										(80)
#define AI_NET_CONFIG_OPTION_VALUE_MASK_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER_ARRAY)
#define AI_NET_CONFIG_OPTION_VALUE_MASK_ARRAY_LENGTH_MAX						(3)
#define AI_NET_CONFIG_OPTION_VALUE_MASK_DEFAULT									((UINT8_T *)"\0")
/* option anchors */
#define AI_NET_CONFIG_OPTION_KEY_ANCHORS_NAME									("anchors")
#define AI_NET_CONFIG_OPTION_KEY_ANCHORS_TYPE									(81)
#define AI_NET_CONFIG_OPTION_VALUE_ANCHORS_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_FLOAT_ARRAY)
#define AI_NET_CONFIG_OPTION_VALUE_ANCHORS_ARRAY_LENGTH_MAX						(18)
#define AI_NET_CONFIG_OPTION_VALUE_ANCHORS_DEFAULT								((UINT8_T *)"0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5")
/* option classes */
#define AI_NET_CONFIG_OPTION_KEY_CLASSES_NAME									("classes")
#define AI_NET_CONFIG_OPTION_KEY_CLASSES_TYPE									(82)
#define AI_NET_CONFIG_OPTION_VALUE_CLASSES_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_CLASSES_DEFAULT								((UINT8_T *)"20")
/* option num */
#define AI_NET_CONFIG_OPTION_KEY_NUM_NAME										("num")
#define AI_NET_CONFIG_OPTION_KEY_NUM_TYPE										(83)
#define AI_NET_CONFIG_OPTION_VALUE_NUM_TYPE										(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_NUM_DEFAULT									((UINT8_T *)"1")
/* option jitter */
#define AI_NET_CONFIG_OPTION_KEY_JITTER_NAME									("jitter")
#define AI_NET_CONFIG_OPTION_KEY_JITTER_TYPE									(84)
#define AI_NET_CONFIG_OPTION_VALUE_JITTER_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_FLOAT)
#define AI_NET_CONFIG_OPTION_VALUE_JITTER_DEFAULT								((UINT8_T *)"0.2")
/* option ignore_thresh */
#define AI_NET_CONFIG_OPTION_KEY_IGNORE_THRESH_NAME								("ignore_thresh")
#define AI_NET_CONFIG_OPTION_KEY_IGNORE_THRESH_TYPE								(85)
#define AI_NET_CONFIG_OPTION_VALUE_IGNORE_THRESH_TYPE							(AI_NET_CONFIG_OPTION_VALUE_TYPE_FLOAT)
#define AI_NET_CONFIG_OPTION_VALUE_IGNORE_THRESH_DEFAULT						((UINT8_T *)"0.5")
/* option truth_thresh */
#define AI_NET_CONFIG_OPTION_KEY_TRUTH_THRESH_NAME								("truth_thresh")
#define AI_NET_CONFIG_OPTION_KEY_TRUTH_THRESH_TYPE								(86)
#define AI_NET_CONFIG_OPTION_VALUE_TRUTH_THRESH_TYPE							(AI_NET_CONFIG_OPTION_VALUE_TYPE_FLOAT)
#define AI_NET_CONFIG_OPTION_VALUE_TRUTH_THRESH_DEFAULT							((UINT8_T *)"1.0")
/* option random */
#define AI_NET_CONFIG_OPTION_KEY_RANDOM_NAME									("random")
#define AI_NET_CONFIG_OPTION_KEY_RANDOM_TYPE									(87)
#define AI_NET_CONFIG_OPTION_VALUE_RANDOM_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER_ENUM)
#define AI_NET_CONFIG_OPTION_VALUE_RANDOM_DEFAULT								((UINT8_T *)"0")
#define AI_NET_CONFIG_OPTION_VALUE_RANDOM_ENABLE								(1)
#define AI_NET_CONFIG_OPTION_VALUE_RANDOM_DISABLE								(0)
/* option max */
#define AI_NET_CONFIG_OPTION_KEY_MAX_NAME										("max")
#define AI_NET_CONFIG_OPTION_KEY_MAX_TYPE										(88)
#define AI_NET_CONFIG_OPTION_VALUE_MAX_TYPE										(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_MAX_DEFAULT									((UINT8_T *)"90")
/* option focal_loss */
#define AI_NET_CONFIG_OPTION_KEY_FOCAL_LOSS_NAME								("focal_loss")
#define AI_NET_CONFIG_OPTION_KEY_FOCAL_LOSS_TYPE								(89)
#define AI_NET_CONFIG_OPTION_VALUE_FOCAL_LOSS_TYPE								(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_FOCAL_LOSS_DEFAULT							((UINT8_T *)"0")
/* options for route section */
/* option layers */
#define AI_NET_CONFIG_OPTION_KEY_LAYERS_NAME									("layers")
#define AI_NET_CONFIG_OPTION_KEY_LAYERS_TYPE									(100)
#define AI_NET_CONFIG_OPTION_VALUE_LAYERS_TYPE									(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER_SIGNED_ARRAY)
#define AI_NET_CONFIG_OPTION_VALUE_LAYERS_ARRAY_LENGTH_MAX						(2)
/* options for all section */
/* option only_forward */
#define AI_NET_CONFIG_OPTION_KEY_ONLYFORWARD_NAME								("onlyforward")
#define AI_NET_CONFIG_OPTION_KEY_ONLYFORWARD_TYPE								(120)
#define AI_NET_CONFIG_OPTION_VALUE_ONLYFORWARD_TYPE								(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_ONLYFORWARD_DEFAULT							((UINT8_T *)"0")
/* option stopbackward */
#define AI_NET_CONFIG_OPTION_KEY_STOPBACKWARD_NAME								("stopbackward")
#define AI_NET_CONFIG_OPTION_KEY_STOPBACKWARD_TYPE								(121)
#define AI_NET_CONFIG_OPTION_VALUE_STOPBACKWARD_TYPE							(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_STOPBACKWARD_DEFAULT							((UINT8_T *)"0")
/* option dontload */
#define AI_NET_CONFIG_OPTION_KEY_DONTLOAD_NAME									("dontload")
#define AI_NET_CONFIG_OPTION_KEY_DONTLOAD_TYPE									(122)
#define AI_NET_CONFIG_OPTION_VALUE_DONTLOAD_TYPE								(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_DONTLOAD_DEFAULT								((UINT8_T *)"0")
/* option dontloadscales */
#define AI_NET_CONFIG_OPTION_KEY_DONTLOADSCALES_NAME							("dontloadscales")
#define AI_NET_CONFIG_OPTION_KEY_DONTLOADSCALES_TYPE							(123)
#define AI_NET_CONFIG_OPTION_VALUE_DONTLOADSCALES_TYPE							(AI_NET_CONFIG_OPTION_VALUE_TYPE_NUMBER)
#define AI_NET_CONFIG_OPTION_VALUE_DONTLOADSCALES_DEFAULT						((UINT8_T *)"0")

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_net_config_option_uint32_t_struct {
	UINT8_T set_type;
	UINT8_T valid;
	UINT32_T value;
} AI_NET_CONFIG_OPTION_UINT32_T, *AI_NET_CONFIG_OPTION_UINT32_T_PTR; 

typedef struct ai_net_config_option_int32_t_struct {
	UINT8_T set_type;
	UINT8_T valid;
	INT32_T value;
} AI_NET_CONFIG_OPTION_INT32_T, *AI_NET_CONFIG_OPTION_INT32_T_PTR; 

typedef struct ai_net_config_option_float_ai_t_struct {
	UINT8_T set_type;
	UINT8_T valid;
	FLOAT_AI_T value;
} AI_NET_CONFIG_OPTION_FLOAT_AI_T, *AI_NET_CONFIG_OPTION_FLOAT_AI_T_PTR;

typedef struct ai_net_config_section_struct {
	AI_NET_CONFIG_OPTION_UINT32_T		type;
	AI_NET_CONFIG_OPTION_UINT32_T		batch;
	AI_NET_CONFIG_OPTION_UINT32_T		subdivisions;
	AI_NET_CONFIG_OPTION_UINT32_T		width;
	AI_NET_CONFIG_OPTION_UINT32_T		height;
	AI_NET_CONFIG_OPTION_UINT32_T		channels;
	AI_NET_CONFIG_OPTION_FLOAT_AI_T		momentum;
	AI_NET_CONFIG_OPTION_FLOAT_AI_T		decay;
	AI_NET_CONFIG_OPTION_UINT32_T		angle;
	AI_NET_CONFIG_OPTION_FLOAT_AI_T		saturation;
	AI_NET_CONFIG_OPTION_FLOAT_AI_T		exposure;
	AI_NET_CONFIG_OPTION_FLOAT_AI_T		hue;
	AI_NET_CONFIG_OPTION_FLOAT_AI_T		learning_rate;
	AI_NET_CONFIG_OPTION_UINT32_T		burn_in;
	AI_NET_CONFIG_OPTION_UINT32_T		max_batches;
	AI_NET_CONFIG_OPTION_UINT32_T		policy;
	AI_NET_CONFIG_OPTION_UINT32_T		step;
	AI_NET_CONFIG_OPTION_FLOAT_AI_T		scale;
	AI_NET_CONFIG_OPTION_UINT32_T		steps[AI_NET_CONFIG_OPTION_VALUE_STEPS_ARRAY_LENGTH_MAX];
	AI_NET_CONFIG_OPTION_FLOAT_AI_T		scales[AI_NET_CONFIG_OPTION_VALUE_SCALES_ARRAY_LENGTH_MAX];
	AI_NET_CONFIG_OPTION_FLOAT_AI_T		gamma;
	AI_NET_CONFIG_OPTION_UINT32_T		adam;
	AI_NET_CONFIG_OPTION_FLOAT_AI_T		b1;
	AI_NET_CONFIG_OPTION_FLOAT_AI_T		b2;
	AI_NET_CONFIG_OPTION_FLOAT_AI_T		eps;
	AI_NET_CONFIG_OPTION_UINT32_T		time_steps;
	AI_NET_CONFIG_OPTION_UINT32_T		inputs;
	AI_NET_CONFIG_OPTION_UINT32_T		max_crop;
	AI_NET_CONFIG_OPTION_UINT32_T		min_crop;
	AI_NET_CONFIG_OPTION_UINT32_T		flip;
	AI_NET_CONFIG_OPTION_UINT32_T		small_object;
	AI_NET_CONFIG_OPTION_UINT32_T		power;
	AI_NET_CONFIG_OPTION_UINT32_T		aspect;
	AI_NET_CONFIG_OPTION_UINT32_T		batch_normalize;
	AI_NET_CONFIG_OPTION_UINT32_T       binary;
	AI_NET_CONFIG_OPTION_UINT32_T		xnor;
	AI_NET_CONFIG_OPTION_UINT32_T		bin_output;
	AI_NET_CONFIG_OPTION_UINT32_T		filters;
	AI_NET_CONFIG_OPTION_UINT32_T		size;
	AI_NET_CONFIG_OPTION_INT32_T		stride;
	AI_NET_CONFIG_OPTION_UINT32_T		pad;
	AI_NET_CONFIG_OPTION_UINT32_T		padding;
	AI_NET_CONFIG_OPTION_UINT32_T		activation;
	AI_NET_CONFIG_OPTION_UINT32_T		flipped;
	AI_NET_CONFIG_OPTION_UINT32_T		dot;
	AI_NET_CONFIG_OPTION_INT32_T		from;
	AI_NET_CONFIG_OPTION_UINT32_T		mask[AI_NET_CONFIG_OPTION_VALUE_MASK_ARRAY_LENGTH_MAX];
	AI_NET_CONFIG_OPTION_FLOAT_AI_T		anchors[AI_NET_CONFIG_OPTION_VALUE_ANCHORS_ARRAY_LENGTH_MAX];
	AI_NET_CONFIG_OPTION_UINT32_T		classes;
	AI_NET_CONFIG_OPTION_UINT32_T		num;
	AI_NET_CONFIG_OPTION_FLOAT_AI_T		jitter;
	AI_NET_CONFIG_OPTION_FLOAT_AI_T		ignore_thresh;
	AI_NET_CONFIG_OPTION_FLOAT_AI_T		truth_thresh;
	AI_NET_CONFIG_OPTION_UINT32_T		random;
	AI_NET_CONFIG_OPTION_UINT32_T		max;
	AI_NET_CONFIG_OPTION_UINT32_T		focal_loss;
	AI_NET_CONFIG_OPTION_INT32_T		layers[AI_NET_CONFIG_OPTION_VALUE_LAYERS_ARRAY_LENGTH_MAX];
	AI_NET_CONFIG_OPTION_UINT32_T		onlyforward;
	AI_NET_CONFIG_OPTION_UINT32_T		stopbackward;
	AI_NET_CONFIG_OPTION_UINT32_T		dontload;
	AI_NET_CONFIG_OPTION_UINT32_T		dontloadscales;
} AI_NET_CONFIG_SECTION, *AI_NET_CONFIG_SECTION_PTR;

typedef struct ai_net_config_struct {
	UINT32_T section_number;
	AI_NET_CONFIG_SECTION section[AI_NET_CONFIG_SECTION_MAX];
} AI_NET_CONFIG, *AI_NET_CONFIG_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiNetConfigGetFileLine(FILE *fp, UINT8_T *line);
ERROR_T AiNetConfigStrip(UINT8_T *s);
ERROR_T AiNetConfigParseSectionType(UINT8_T *name, UINT32_T *section_type);
ERROR_T AiNetConfigReadOption(UINT8_T *s, UINT8_T *key, UINT8_T *value);
ERROR_T AiNetConfigReadOptionValue(UINT8_T *value_str, UINT8_T_PTR *option_array_ptr);
ERROR_T AiNetConfigSetOptionValueUint32(AI_NET_CONFIG_OPTION_UINT32_T *option, UINT8_T *value_str, UINT8_T set_type);
ERROR_T AiNetConfigSetOptionValueInt32(AI_NET_CONFIG_OPTION_INT32_T *option, UINT8_T *value_str, UINT8_T set_type);
ERROR_T AiNetConfigSetOptionValueFloat(AI_NET_CONFIG_OPTION_FLOAT_AI_T *option, UINT8_T *value_str, UINT8_T set_type);
ERROR_T AiNetConfigParseOptionKeyValue(UINT8_T *key_str, UINT8_T *value_str, AI_NET_CONFIG_SECTION *section);
ERROR_T AiNetConfigLoadFile(AI_NET_CONFIG *net_config);
ERROR_T AiNetConfigLoad(AI_NET_CONFIG *net_config);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
